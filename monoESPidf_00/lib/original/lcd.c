
#include	"at_inc.h"					/* インクルードファイル				*/

#include "driver/ledc.h"
#include "driver/i2c.h"
#include "smbus.h"
#include "i2c-lcd1602.h"

i2c_lcd1602_info_t * lcd_info = NULL;

void LCD_init(void)
{

    //i2C init
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_LEN,
                       I2C_MASTER_TX_BUF_LEN, 0);
    
    //LCD init
    i2c_port_t i2c_num = I2C_MASTER_NUM;
    uint8_t address = CONFIG_LCD1602_I2C_ADDRESS;

    // Set up the SMBus
    smbus_info_t * smbus_info = smbus_malloc();
    ESP_ERROR_CHECK(smbus_init(smbus_info, i2c_num, address));
    ESP_ERROR_CHECK(smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS));

    // Set up the LCD1602 device with backlight off

    // lcd_info のメモリを確保する
    lcd_info = malloc(sizeof(i2c_lcd1602_info_t));
    if (lcd_info == NULL) {
        ESP_LOGE("LCD", "lcd_info のメモリ割り当てに失敗");
        return;
    }

    ESP_ERROR_CHECK(i2c_lcd1602_init(lcd_info, smbus_info, true,
                                     LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VISIBLE_COLUMNS));

    ESP_ERROR_CHECK(i2c_lcd1602_reset(lcd_info));

    i2c_lcd1602_clear(lcd_info);
    i2c_lcd1602_set_cursor(lcd_info, true);

}

void LCD_3num(float num1, float num2, float num3)
{
    char R[8];
    char X[8];
    char Z[8];
    
    // 数値を文字列に変換（%u は符号なし整数の書式）
    // ここで、必要に応じてゼロパディング（例：%04u）などに変更できます
    // snprintf(R, sizeof(R), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].sRDT/10.0f);
    // snprintf(X, sizeof(X), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].sXDT/10.0f);
    // snprintf(Z, sizeof(Z), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].usZDT/10.0f);

    snprintf(R, sizeof(R), "%5.1f", num1);
    snprintf(X, sizeof(X), "%5.1f", num2);
    snprintf(Z, sizeof(Z), "%5.1f", num3);
    // snprintf(buf, sizeof(buf), "%5.1f", number);

    // 変換した文字列をLCDに表示
    i2c_lcd1602_clear(lcd_info);
    // i2c_lcd1602_set_cursor(lcd_info, true);
    // i2c_lcd1602_write_string(lcd_info, buf);
    // ESP_LOGI(TAG, "imp = %5f", number); 

    // ---- 左上 (row=0, col=0)
    i2c_lcd1602_move_cursor(lcd_info, 0, 0);  
    i2c_lcd1602_write_string(lcd_info, R);

    // ---- 右上 (row=0, col=8)
    i2c_lcd1602_move_cursor(lcd_info, 10, 0);
    i2c_lcd1602_write_string(lcd_info, X);

    // ---- 左下 (row=1, col=0)
    i2c_lcd1602_move_cursor(lcd_info, 0, 1);  
    i2c_lcd1602_write_string(lcd_info, Z);
}
