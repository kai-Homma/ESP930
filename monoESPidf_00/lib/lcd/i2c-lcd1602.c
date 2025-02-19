/*
 * MIT License
 *
 * Copyright (c) 2018 David Antliff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file
 *
 * @brief
 * The LCD1602 controller is an HD44780-compatible controller that normally operates
 * via an 8-bit or 4-bit wide parallel bus.
 *
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * The LCD1602 controller is connected to a PCF8574A I/O expander via the I2C bus.
 * Only the top four bits are connected to the controller's data lines. The lower
 * four bits are used as control lines:
 *
 *   - B7: data bit 3
 *   - B6: data bit 2
 *   - B5: data bit 1
 *   - B4: data bit 0
 *   - B3: backlight (BL): off = 0, on = 1
 *   - B2: enable (EN): change from 1 to 0 to clock data into controller
 *   - B1: read/write (RW): write = 0, read = 1
 *   - B0: register select (RS): command = 0, data = 1
 *
 * Therefore to send a command byte requires the following operations:
 *
 *   // First nibble:
 *   val = command & 0xf0              // extract top nibble
 *   val |= 0x04                       // RS = 0 (command), RW = 0 (write), EN = 1
 *   i2c_write_byte(i2c_address, val)
 *   sleep(2ms)
 *   val &= 0xfb                       // EN = 0
 *   i2c_write_byte(i2c_address, val)
 *
 *   // Second nibble:
 *   val = command & 0x0f              // extract bottom nibble
 *   val |= 0x04                       // RS = 0 (command), RW = 0 (write), EN = 1
 *   i2c_write_byte(i2c_address, val)
 *   sleep(2ms)
 *   val &= 0xfb                       // EN = 0
 *   i2c_write_byte(i2c_address, val)
 *
 * Sending a data byte is very similar except that RS = 1 (data)
 *
 * When the controller powers up, it defaults to:
 *
 *   - display cleared
 *   - 8-bit interface, 1 line display, 5x8 dots per character
 *   - increment by 1 set
 *   - no shift
 *
 * The controller must be set to 4-bit operation before proper communication can commence.
 * The initialisation sequence for 4-bit operation is:
 *
 *   0. wait > 15ms after Vcc rises to 4.5V, or > 40ms after Vcc rises to 2.7V
 *   1. send nibble 0x03     // select 8-bit interface
 *   2. wait > 4.1ms
 *   3. send nibble 0x03     // select 8-bit interface again
 *   4. wait > 100us
 *   5. send command 0x32    // select 4-bit interface
 *   6. send command 0x28    // set 2 lines and 5x7(8?) dots per character
 *   7. send command 0x0c    // display on, cursor off
 *   8. send command 0x06    // move cursor right when writing, no scroll
 *   9. send command 0x80    // set cursor to home position (row 1, column 1)
 */

#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "i2c-lcd1602.h"

#define TAG "i2c-lcd1602"



// Delays (microseconds)
#define DELAY_POWER_ON            50000  // wait at least 40us after VCC rises to 2.7V
#define DELAY_INIT_1               4500  // wait at least 4.1ms (fig 24, page 46)
#define DELAY_INIT_2               4500  // wait at least 4.1ms (fig 24, page 46)
#define DELAY_INIT_3                120  // wait at least 100us (fig 24, page 46)

#define DELAY_CLEAR_DISPLAY        2000
#define DELAY_RETURN_HOME          2000

#define DELAY_ENABLE_PULSE_WIDTH      1  // enable pulse must be at least 450ns wide
#define DELAY_ENABLE_PULSE_SETTLE    50  // command requires > 37us to settle (table 6 in datasheet)


// Commands
#define COMMAND_CLEAR_DISPLAY       0x01
#define COMMAND_RETURN_HOME         0x02
#define COMMAND_ENTRY_MODE_SET      0x04
#define COMMAND_DISPLAY_CONTROL     0x08
#define COMMAND_SHIFT               0x10
#define COMMAND_FUNCTION_SET        0x20
#define COMMAND_SET_CGRAM_ADDR      0x40
#define COMMAND_SET_DDRAM_ADDR      0x80

// COMMAND_ENTRY_MODE_SET flags
#define FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT       0x02
#define FLAG_ENTRY_MODE_SET_ENTRY_DECREMENT       0x00
#define FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_ON        0x01
#define FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_OFF       0x00

// COMMAND_DISPLAY_CONTROL flags
#define FLAG_DISPLAY_CONTROL_DISPLAY_ON  0x04
#define FLAG_DISPLAY_CONTROL_DISPLAY_OFF 0x00
#define FLAG_DISPLAY_CONTROL_CURSOR_ON   0x02
#define FLAG_DISPLAY_CONTROL_CURSOR_OFF  0x00
#define FLAG_DISPLAY_CONTROL_BLINK_ON    0x01
#define FLAG_DISPLAY_CONTROL_BLINK_OFF   0x00

// COMMAND_SHIFT flags
#define FLAG_SHIFT_MOVE_DISPLAY          0x08
#define FLAG_SHIFT_MOVE_CURSOR           0x00
#define FLAG_SHIFT_MOVE_LEFT             0x04
#define FLAG_SHIFT_MOVE_RIGHT            0x00

// COMMAND_FUNCTION_SET flags
#define FLAG_FUNCTION_SET_MODE_8BIT      0x10
#define FLAG_FUNCTION_SET_MODE_4BIT      0x00
#define FLAG_FUNCTION_SET_LINES_2        0x08
#define FLAG_FUNCTION_SET_LINES_1        0x00
#define FLAG_FUNCTION_SET_DOTS_5X10      0x04
#define FLAG_FUNCTION_SET_DOTS_5X8       0x00

// Control flags
#define FLAG_BACKLIGHT_ON    0b00001000      // backlight enabled (disabled if clear)
#define FLAG_BACKLIGHT_OFF   0b00000000      // backlight disabled
#define FLAG_ENABLE          0b00000100
#define FLAG_READ            0b00000010      // read (write if clear)
#define FLAG_WRITE           0b00000000      // write
#define FLAG_RS_DATA         0b00000001      // data (command if clear)
#define FLAG_RS_COMMAND      0b00000000      // command

static bool _is_init(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    bool ok = false;
    if (i2c_lcd1602_info != NULL)
    {
        if (i2c_lcd1602_info->init)
        {
            ok = true;
        }
        else
        {
            ESP_LOGE(TAG, "i2c_lcd1602_info is not initialised");
        }
    }
    else
    {
        ESP_LOGE(TAG, "i2c_lcd1602_info is NULL");
    }
    return ok;
}

 // Set or clear the specified flag depending on condition
 static uint8_t _set_or_clear(uint8_t flags, bool condition, uint8_t flag)
 {
     if (condition)
     {
         flags |= flag;
     }
     else
     {
         flags &= ~flag;
     }
     return flags;
 }

// send data to the I/O Expander
static esp_err_t _write_to_expander(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t data)
{
    // backlight flag must be included with every write to maintain backlight state
    ESP_LOGD(TAG, "_write_to_expander 0x%02x", data | i2c_lcd1602_info->backlight_flag);
    return smbus_send_byte(i2c_lcd1602_info->smbus_info, data | i2c_lcd1602_info->backlight_flag);
}

// IMPORTANT - for the display to stay "in sync" it is important that errors do not interrupt the
// 2 x nibble sequence.

// clock data from expander to LCD by causing a falling edge on Enable
static esp_err_t _strobe_enable(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t data)
{
    esp_err_t err1 = _write_to_expander(i2c_lcd1602_info, data | FLAG_ENABLE);
    esp_rom_delay_us(DELAY_ENABLE_PULSE_WIDTH);
    esp_err_t err2 = _write_to_expander(i2c_lcd1602_info, data & ~FLAG_ENABLE);
    esp_rom_delay_us(DELAY_ENABLE_PULSE_SETTLE);
    return err1 ? err1 : err2;
}

// send top nibble to the LCD controller
static esp_err_t _write_top_nibble(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t data)
{
    ESP_LOGD(TAG, "_write_top_nibble 0x%02x", data);
    esp_err_t err1 = _write_to_expander(i2c_lcd1602_info, data);
    esp_err_t err2 = _strobe_enable(i2c_lcd1602_info, data);
    return err1 ? err1 : err2;
}

// send command or data to controller
static esp_err_t _write(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t value, uint8_t register_select_flag)
{
    ESP_LOGD(TAG, "_write 0x%02x | 0x%02x", value, register_select_flag);
    esp_err_t err1 = _write_top_nibble(i2c_lcd1602_info, (value & 0xf0) | register_select_flag);
    esp_err_t err2 = _write_top_nibble(i2c_lcd1602_info, ((value & 0x0f) << 4) | register_select_flag);
    return err1 ? err1 : err2;
}

// send command to controller
static esp_err_t _write_command(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t command)
{
    ESP_LOGD(TAG, "_write_command 0x%02x", command);
    return _write(i2c_lcd1602_info, command, FLAG_RS_COMMAND);
}

// send data to controller
static esp_err_t _write_data(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t data)
{
    ESP_LOGD(TAG, "_write_data 0x%02x", data);
    return _write(i2c_lcd1602_info, data, FLAG_RS_DATA);
}


// Public API

i2c_lcd1602_info_t * i2c_lcd1602_malloc(void)
{
    i2c_lcd1602_info_t * i2c_lcd1602_info = malloc(sizeof(*i2c_lcd1602_info));
    if (i2c_lcd1602_info != NULL)
    {
        memset(i2c_lcd1602_info, 0, sizeof(*i2c_lcd1602_info));
        ESP_LOGD(TAG, "malloc i2c_lcd1602_info_t %p", i2c_lcd1602_info);
    }
    else
    {
        ESP_LOGE(TAG, "malloc i2c_lcd1602_info_t failed");
    }
    return i2c_lcd1602_info;
}

void i2c_lcd1602_free(i2c_lcd1602_info_t ** i2c_lcd1602_info)
{
    if (i2c_lcd1602_info != NULL && (*i2c_lcd1602_info != NULL))
    {
        ESP_LOGD(TAG, "free i2c_lcd1602_info_t %p", *i2c_lcd1602_info);
        free(*i2c_lcd1602_info);
        *i2c_lcd1602_info = NULL;
    }
    else
    {
        ESP_LOGE(TAG, "free i2c_lcd1602_info_t failed");
    }
}

esp_err_t i2c_lcd1602_init(i2c_lcd1602_info_t * i2c_lcd1602_info, smbus_info_t * smbus_info,
                           bool backlight, uint8_t num_rows, uint8_t num_columns, uint8_t num_visible_columns)
{
    esp_err_t err = ESP_FAIL;
    if (i2c_lcd1602_info != NULL)
    {
        i2c_lcd1602_info->smbus_info = smbus_info;
        i2c_lcd1602_info->backlight_flag = backlight ? FLAG_BACKLIGHT_ON : FLAG_BACKLIGHT_OFF;
        i2c_lcd1602_info->num_rows = num_rows;
        i2c_lcd1602_info->num_columns = num_columns;
        i2c_lcd1602_info->num_visible_columns = num_visible_columns;

        // display on, no cursor, no blinking
        i2c_lcd1602_info->display_control_flags = FLAG_DISPLAY_CONTROL_DISPLAY_ON | FLAG_DISPLAY_CONTROL_CURSOR_OFF | FLAG_DISPLAY_CONTROL_BLINK_OFF;

        // left-justified left-to-right text
        i2c_lcd1602_info->entry_mode_flags = FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT | FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_OFF;

        i2c_lcd1602_info->init = true;

        // See page 45/46 of HD44780 data sheet for the initialisation procedure.

        // Wait at least 40ms after power rises above 2.7V before sending commands.
        esp_rom_delay_us(DELAY_POWER_ON);

        err = i2c_lcd1602_reset(i2c_lcd1602_info);
    }
    else
    {
        ESP_LOGE(TAG, "i2c_lcd1602_info is NULL");
        err = ESP_FAIL;
    }
    return err;
}

esp_err_t i2c_lcd1602_reset(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t first_err = ESP_OK;
    esp_err_t last_err = ESP_FAIL;

    // put Expander into known state - Register Select and Read/Write both low
    if ((last_err = _write_to_expander(i2c_lcd1602_info, 0)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_to_expander 1 failed: %d", last_err);
    }

    esp_rom_delay_us(1000);

    // select 4-bit mode on LCD controller - see datasheet page 46, figure 24.
    if ((last_err = _write_top_nibble(i2c_lcd1602_info, 0x03 << 4)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_top_nibble 1 failed: %d", last_err);
    }

    esp_rom_delay_us(DELAY_INIT_1);

    // repeat
    if ((last_err = _write_top_nibble(i2c_lcd1602_info, 0x03 << 4)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_top_nibble 2 failed: %d", last_err);
    }

    esp_rom_delay_us(DELAY_INIT_2);

    // repeat
    if ((last_err = _write_top_nibble(i2c_lcd1602_info, 0x03 << 4)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_top_nibble 3 failed: %d", last_err);
    }

    esp_rom_delay_us(DELAY_INIT_3);

    // select 4-bit mode
    if ((last_err = _write_top_nibble(i2c_lcd1602_info, 0x02 << 4)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_top_nibble 4 failed: %d", last_err);
    }

    // now we can use the command()/write() functions
    if ((last_err = _write_command(i2c_lcd1602_info, COMMAND_FUNCTION_SET | FLAG_FUNCTION_SET_MODE_4BIT | FLAG_FUNCTION_SET_LINES_2 | FLAG_FUNCTION_SET_DOTS_5X8)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_command 1 failed: %d", last_err);
    }

    if ((last_err = _write_command(i2c_lcd1602_info, COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info->display_control_flags)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_command 2 failed: %d", last_err);
    }

    if ((last_err = i2c_lcd1602_clear(i2c_lcd1602_info)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: i2c_lcd1602_clear failed: %d", last_err);
    }

    if ((last_err = _write_command(i2c_lcd1602_info, COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info->entry_mode_flags)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: _write_command 3 failed: %d", last_err);
    }

    if ((last_err = i2c_lcd1602_home(i2c_lcd1602_info)) != ESP_OK)
    {
        if (first_err == ESP_OK)
            first_err = last_err;
        ESP_LOGE(TAG, "reset: i2c_lcd1602_home failed: %d", last_err);
    }

    return first_err;
}

esp_err_t i2c_lcd1602_clear(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        err = _write_command(i2c_lcd1602_info, COMMAND_CLEAR_DISPLAY);
        if (err == ESP_OK)
        {
            esp_rom_delay_us(DELAY_CLEAR_DISPLAY);
        }
    }
    return err;
}

esp_err_t i2c_lcd1602_home(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        err = _write_command(i2c_lcd1602_info, COMMAND_RETURN_HOME);
        if (err == ESP_OK)
        {
            esp_rom_delay_us(DELAY_RETURN_HOME);
        }
    }
    return err;
}

esp_err_t i2c_lcd1602_move_cursor(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t col, uint8_t row)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
        if (row > i2c_lcd1602_info->num_rows)
        {
            row = i2c_lcd1602_info->num_rows - 1;
        }
        if (col > i2c_lcd1602_info->num_columns)
        {
            col = i2c_lcd1602_info->num_columns - 1;
        }
        err = _write_command(i2c_lcd1602_info, COMMAND_SET_DDRAM_ADDR | (col + row_offsets[row]));
    }
    return err;
}

esp_err_t i2c_lcd1602_set_backlight(i2c_lcd1602_info_t * i2c_lcd1602_info, bool enable)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->backlight_flag = _set_or_clear(i2c_lcd1602_info->backlight_flag, enable, FLAG_BACKLIGHT_ON);
        err = _write_to_expander(i2c_lcd1602_info, 0);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_display(i2c_lcd1602_info_t * i2c_lcd1602_info, bool enable)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->display_control_flags = _set_or_clear(i2c_lcd1602_info->display_control_flags, enable, FLAG_DISPLAY_CONTROL_DISPLAY_ON);
        err = _write_command(i2c_lcd1602_info, COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info->display_control_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_cursor(i2c_lcd1602_info_t * i2c_lcd1602_info, bool enable)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->display_control_flags = _set_or_clear(i2c_lcd1602_info->display_control_flags, enable, FLAG_DISPLAY_CONTROL_CURSOR_ON);
        err = _write_command(i2c_lcd1602_info, COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info->display_control_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_blink(i2c_lcd1602_info_t * i2c_lcd1602_info, bool enable)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->display_control_flags = _set_or_clear(i2c_lcd1602_info->display_control_flags, enable, FLAG_DISPLAY_CONTROL_BLINK_ON);
        err = _write_command(i2c_lcd1602_info, COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info->display_control_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_left_to_right(i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->entry_mode_flags |= FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT;
        err = _write_command(i2c_lcd1602_info, COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info->entry_mode_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_right_to_left(i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->entry_mode_flags &= ~FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT;
        err = _write_command(i2c_lcd1602_info, COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info->entry_mode_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_set_auto_scroll(i2c_lcd1602_info_t * i2c_lcd1602_info, bool enable)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        i2c_lcd1602_info->entry_mode_flags = _set_or_clear(i2c_lcd1602_info->entry_mode_flags, enable, FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_ON);
        err = _write_command(i2c_lcd1602_info, COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info->entry_mode_flags);
    }
    return err;
}

esp_err_t i2c_lcd1602_scroll_display_left(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        // RAM is not changed
        err = _write_command(i2c_lcd1602_info, COMMAND_SHIFT | FLAG_SHIFT_MOVE_DISPLAY | FLAG_SHIFT_MOVE_LEFT);
    }
    return err;
}

esp_err_t i2c_lcd1602_scroll_display_right(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        // RAM is not changed
        err = _write_command(i2c_lcd1602_info, COMMAND_SHIFT | FLAG_SHIFT_MOVE_DISPLAY | FLAG_SHIFT_MOVE_RIGHT);
    }
    return err;
}

esp_err_t i2c_lcd1602_move_cursor_left(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        // RAM is not changed. Shift direction is inverted.
        err = _write_command(i2c_lcd1602_info, COMMAND_SHIFT | FLAG_SHIFT_MOVE_CURSOR | FLAG_SHIFT_MOVE_RIGHT);
    }
    return err;
}

esp_err_t i2c_lcd1602_move_cursor_right(const i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        // RAM is not changed. Shift direction is inverted.
        err = _write_command(i2c_lcd1602_info, COMMAND_SHIFT | FLAG_SHIFT_MOVE_CURSOR | FLAG_SHIFT_MOVE_LEFT);
    }
    return err;
}

esp_err_t i2c_lcd1602_define_char(const i2c_lcd1602_info_t * i2c_lcd1602_info, i2c_lcd1602_custom_index_t index, const uint8_t pixelmap[])
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        index &= 0x07;  // only the first 8 indexes can be used for custom characters
        err = _write_command(i2c_lcd1602_info, COMMAND_SET_CGRAM_ADDR | (index << 3));
        for (int i = 0; err == ESP_OK && i < 8; ++i)
        {
            err = _write_data(i2c_lcd1602_info, pixelmap[i]);
        }
    }
    return err;
}

esp_err_t i2c_lcd1602_write_char(const i2c_lcd1602_info_t * i2c_lcd1602_info, uint8_t chr)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        err = _write_data(i2c_lcd1602_info, chr);
    }
    return err;
}

esp_err_t i2c_lcd1602_write_string(const i2c_lcd1602_info_t * i2c_lcd1602_info, const char * string)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        //ESP_LOGI(TAG, "i2c_lcd1602_write_string: %s", string);
        err = ESP_OK;
        for (int i = 0; err == ESP_OK && string[i]; ++i)
        {
            err = _write_data(i2c_lcd1602_info, string[i]);
        }
    }
    return err;
}


// TEMPLATE
#if 0
esp_err_t i2c_lcd1602_XXX(i2c_lcd1602_info_t * i2c_lcd1602_info)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
    }
    return err;
}


    // turn off backlight
    ESP_LOGI(TAG, "backlight off");
    _wait_for_user();
    i2c_lcd1602_set_backlight(lcd_info, false);

    // turn on backlight
    ESP_LOGI(TAG, "backlight on");
    _wait_for_user();
    i2c_lcd1602_set_backlight(lcd_info, true);

    ESP_LOGI(TAG, "cursor on");
    _wait_for_user();
    i2c_lcd1602_set_cursor(lcd_info, true);

    ESP_LOGI(TAG, "display A at 0,0");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    i2c_lcd1602_write_char(lcd_info, 'A');

    ESP_LOGI(TAG, "display B at 8,0");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 8, 0);
    i2c_lcd1602_write_char(lcd_info, 'B');

    ESP_LOGI(TAG, "display C at 15,1");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 15, 1);
    i2c_lcd1602_write_char(lcd_info, 'C');

    ESP_LOGI(TAG, "move to 0,1 and blink");  // cursor should still be on
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 0, 1);
    i2c_lcd1602_set_blink(lcd_info, true);

    ESP_LOGI(TAG, "display DE and move cursor back onto D");
    _wait_for_user();
    i2c_lcd1602_write_char(lcd_info, 'D');
    i2c_lcd1602_set_right_to_left(lcd_info);
    i2c_lcd1602_write_char(lcd_info, 'E');
    i2c_lcd1602_set_left_to_right(lcd_info);

    ESP_LOGI(TAG, "disable display");
    _wait_for_user();
    i2c_lcd1602_set_display(lcd_info, false);

    ESP_LOGI(TAG, "display F at 7,1 (display disabled)");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 7, 1);
    i2c_lcd1602_write_char(lcd_info, 'F');

    ESP_LOGI(TAG, "enable display");
    _wait_for_user();
    i2c_lcd1602_set_display(lcd_info, true);

    ESP_LOGI(TAG, "disable blink");
    _wait_for_user();
    i2c_lcd1602_set_blink(lcd_info, false);  // cursor should still be on

    ESP_LOGI(TAG, "disable cursor");
    _wait_for_user();
    i2c_lcd1602_set_cursor(lcd_info, false);

    ESP_LOGI(TAG, "display alphabet from 0,0");  // should overflow to second line at "ABC..."
    _wait_for_user();
    i2c_lcd1602_home(lcd_info);
    i2c_lcd1602_write_string(lcd_info, "abcdefghijklmnopqrstuvwxyz0123456789.,-+ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    ESP_LOGI(TAG, "scroll display left 8 places slowly");
    _wait_for_user();
    for (int i = 0; i < 8; ++i)
    {
        i2c_lcd1602_scroll_display_left(lcd_info);
        vTaskDelay(200 / portTICK_RATE_MS);
    }

    ESP_LOGI(TAG, "scroll display right 8 places quickly");
    _wait_for_user();
    for (int i = 0; i < 8; ++i)
    {
        i2c_lcd1602_scroll_display_right(lcd_info);
    }

    ESP_LOGI(TAG, "move to 8,0 and show cursor");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 8, 0);
    i2c_lcd1602_set_cursor(lcd_info, true);

    ESP_LOGI(TAG, "move cursor 5 places to the right");
    _wait_for_user();
    for (int i = 0; i < 5; ++i)
    {
        i2c_lcd1602_move_cursor_right(lcd_info);
    }

    ESP_LOGI(TAG, "move cursor 3 places to the left");
    _wait_for_user();
    for (int i = 0; i < 3; ++i)
    {
        i2c_lcd1602_move_cursor_left(lcd_info);
    }

    ESP_LOGI(TAG, "enable auto-scroll and display >>>>>");
    _wait_for_user();
    i2c_lcd1602_set_auto_scroll(lcd_info, true);
    for (int i = 0; i < 5; ++i)
    {
        i2c_lcd1602_write_char(lcd_info, '>');
        vTaskDelay(200 / portTICK_RATE_MS);
    }

    ESP_LOGI(TAG, "change address counter to decrement (right to left) and display <<<<<");
    _wait_for_user();
    i2c_lcd1602_set_right_to_left(lcd_info);
    for (int i = 0; i < 5; ++i)
    {
        i2c_lcd1602_write_char(lcd_info, '<');
        vTaskDelay(200 / portTICK_RATE_MS);
    }

    ESP_LOGI(TAG, "disable auto-scroll and display +++++");
    _wait_for_user();
    i2c_lcd1602_set_auto_scroll(lcd_info, false);
    for (int i = 0; i < 5; ++i)
    {
        i2c_lcd1602_write_char(lcd_info, '+');
        vTaskDelay(200 / portTICK_RATE_MS);
    }

    ESP_LOGI(TAG, "set left_to_right and display >>>>>");
    _wait_for_user();
    i2c_lcd1602_set_left_to_right(lcd_info);
    for (int i = 0; i < 5; ++i)
    {
        i2c_lcd1602_write_char(lcd_info, '>');
        vTaskDelay(200 / portTICK_RATE_MS);
    }

    ESP_LOGI(TAG, "clear display and disable cursor");
    _wait_for_user();
    i2c_lcd1602_clear(lcd_info);
    i2c_lcd1602_set_cursor(lcd_info, false);

    ESP_LOGI(TAG, "create and display custom characters");
    _wait_for_user();
    // https://github.com/agnunez/ESP8266-I2C-LCD1602/blob/master/examples/CustomChars/CustomChars.ino
    uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
    uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
    uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
    uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
    uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
    uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
    uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
    uint8_t retarrow[8] = { 0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_0, bell);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_1, note);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_2, clock);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_3, heart);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_4, duck);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_5, check);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_6, cross);
    i2c_lcd1602_define_char(lcd_info, I2C_LCD1602_INDEX_CUSTOM_7, retarrow);

    // after defining custom characters, DDRAM address must be set by home() or moving the cursor
    i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_0);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_1);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_2);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_3);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_4);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_5);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_6);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_CUSTOM_7);

    ESP_LOGI(TAG, "display special characters");
    _wait_for_user();
    i2c_lcd1602_move_cursor(lcd_info, 0, 1);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_ALPHA);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_BETA);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_THETA);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_PI);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_OMEGA);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_SIGMA);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_INFINITY);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_DEGREE);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_ARROW_LEFT);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_ARROW_RIGHT);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_SQUARE);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_DOT);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_DIVIDE);
    i2c_lcd1602_write_char(lcd_info, I2C_LCD1602_CHARACTER_BLOCK);

#endif
