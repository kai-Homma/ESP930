#include <stdio.h>
#include <stddef.h>					/* 共通定義							*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c.h"

#include "at_inc.h"					/* インクルードファイル				*/

// #include "rom/uart.h"
#include "smbus.h"
#include "i2c-lcd1602.h"


static const char *TAG = "ADC_CONTINUOUS_EXAMPLE";

union	ADC_T	ADC;					/* ADC情報							*/
struct	HSRAM_T	HSRAM;					/* 高速ＲＡＭ情報					*/
struct	MEAS_T	MEAS;					/* 測定情報							*/

// struct	MTR_T	MTR;					/* 監視情報							*/
union	SYS_T	SYS;					/* システム情報						*/
union	FUNC_T	FUNC;					/* ファンクション情報				*/

//なんかうまくいかない。から関数内で宣言
//spec01 all chg start
// const	union	SYS_T	SYS_INIT = 	/* システム情報初期化データ			*/
// {
//     {
//         {                                        /*	システム情報2（係数）			*/
//             IMPCOEF_CR_DEF,						/*		インピ補正係数	CR			*/
//             IMPCOEF_CX_DEF					/*						CX			*/
//         },
//         {
//             IMPCOEF_CR_DEF_5,					/*					5kHzCR			*/
//             IMPCOEF_CX_DEF_5					/*						CX			*/
//         },
//         {                                /*	システム情報4（インピ調整）		*/
//             (short)8150,//(short)IMP_REF,						/*		50kHz	CR					*/
//             32,//0,									/*				CX					*/
//             22,//0,									/*				OSR					*/
//             55//,									/*				OSX					*/
//         },
//         0,									/*				チェックサム		*/
//         0,									/*		空き						*/
//         {
//             (short)IMP_REF,						/*		5kHz	CR					*/
//             0,									/*				CX					*/
//             0,									/*				OSR					*/
//             0								/*				OSX					*/
//         },
//         0,									/*				チェックサム		*/
//         0									/*		空き						*/
//     }
// //key01 chg end
// };
//spec01 all chg end

// 割り込みイベント用のキュー
static QueueHandle_t s_gpio_evt_queue = NULL;
// static QueueHandle_t s_gpio_evt_queue2 = NULL;
// ADCドライバのハンドル
static adc_continuous_handle_t s_adc_handle = NULL;

static void i2c_master_init(void)
{
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
}

// uart_rx_one_char_block() causes a watchdog trigger, so use the non-blocking
// uart_rx_one_char() and delay briefly to reset the watchdog.
static uint8_t _wait_for_user(void)
{
    uint8_t c = 0;
    vTaskDelay(1000 / portTICK_RATE_MS);

// #ifdef USE_STDIN
//     while (!c)
//     {
//        STATUS s = uart_rx_one_char(&c);
//        if (s == OK) {
//           printf("%c", c);
//        }
//        vTaskDelay(1);
//     }
// #else
//     vTaskDelay(1000 / portTICK_RATE_MS);
// #endif
    return c;
}

void lcd1602_task(void * pvParameter)
{
    // Set up I2C
    i2c_master_init();
    i2c_port_t i2c_num = I2C_MASTER_NUM;
    uint8_t address = CONFIG_LCD1602_I2C_ADDRESS;

    // Set up the SMBus
    smbus_info_t * smbus_info = smbus_malloc();
    ESP_ERROR_CHECK(smbus_init(smbus_info, i2c_num, address));
    ESP_ERROR_CHECK(smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS));

    // Set up the LCD1602 device with backlight off
    i2c_lcd1602_info_t * lcd_info = i2c_lcd1602_malloc();
    ESP_ERROR_CHECK(i2c_lcd1602_init(lcd_info, smbus_info, true,
                                     LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VISIBLE_COLUMNS));

    ESP_ERROR_CHECK(i2c_lcd1602_reset(lcd_info));

    ESP_LOGI(TAG, "display all characters (loop)");
    _wait_for_user();
    i2c_lcd1602_clear(lcd_info);
    i2c_lcd1602_set_cursor(lcd_info, true);
    // uint8_t c = 0;
    // uint8_t col = 0;
    // uint8_t row = 0;
    // ushort number = GetImpZ(ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE);
    // float number = GetImpZ(ADC.IMP.THIS[1].lIM, ADC.IMP.THIS[1].lRE)*820/GetImpZ(ADC.IMP.THIS[0].lIM, ADC.IMP.THIS[0].lRE);
    // float number = MEAS.IMP[IMP_FREQ_50KHZ].usZDT/10.0f;
    while (1)
    {
        // number = MEAS.IMP[IMP_FREQ_50KHZ].usZDT/10.0f;
        // // 数値が4桁を超えた場合の処理（必要なら桁数制限）
        // if (number > 9999) {
        //     number = 9999;
        // }
        
        // 4桁分のバッファ＋終端文字('\0')
        char R[8];
        char X[8];
        char Z[8];
        
        // 数値を文字列に変換（%u は符号なし整数の書式）
        // ここで、必要に応じてゼロパディング（例：%04u）などに変更できます
        snprintf(R, sizeof(R), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].sRDT/10.0f);
        snprintf(X, sizeof(X), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].sXDT/10.0f);
        snprintf(Z, sizeof(Z), "%5.1f", (float)MEAS.IMP[IMP_FREQ_50KHZ].usZDT/10.0f);
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

        vTaskDelay(pdMS_TO_TICKS(750));
    }

    vTaskDelete(NULL);
}


// ----------------------------------------------------------------------
//  GPIO設定
static void setup_gpio_imp(void)
{
    ESP_LOGI(TAG, "Configuring static GPIO outputs...");

    // 1) IO39, IO40, IO42 を出力設定
    //    ピンをまとめて設定するために pin_bit_mask を使用
    uint64_t pin_mask = (1ULL << PIN_39) | (1ULL << PIN_40) | (1ULL << PIN_42) | (1ULL << PIN_47);
    gpio_config_t io_conf = {
        .pin_bit_mask = pin_mask,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // 2) IO39, IO40 を Low 出力
    gpio_set_level(PIN_39, 0);
    gpio_set_level(PIN_40, 0);

    // 3) IO42 を High 出力
    gpio_set_level(PIN_42, 1);
    gpio_set_level(PIN_47, 1);

    ESP_LOGI(TAG, "Static GPIO done.");

    // 4) LEDC (PWM) で IO41 に 4MHz 出力する設定
    //    注: 4MHz はかなり高い周波数で安定しない場合があります。
    ESP_LOGI(TAG, "Configuring LEDC for 4MHz PWM on IO41...");

    // 4-1) LEDC タイマー設定
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_TIMER_SPEED_MODE, // HIGH_SPEED_MODE
        .timer_num        = LEDC_TIMER_NUM,
        .duty_resolution  = LEDC_TIMER_2_BIT,      // 2ビット分解能
        .freq_hz          = PWM_FREQ_HZ,          // 4MHzターゲット
        .clk_cfg          = LEDC_AUTO_CLK         // ESP32-S3 では自動選択
    };
    esp_err_t err = ledc_timer_config(&ledc_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ledc_timer_config failed, err=%s", esp_err_to_name(err));
    }

    // 4-2) LEDC チャネル設定
    ledc_channel_config_t ledc_channel_cfg = {
        .gpio_num       = PIN_4MHZ,
        .speed_mode     = LEDC_TIMER_SPEED_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER_NUM,
        .duty           = 2,  // 2bit 分解能の場合 半分が2
        .hpoint         = 0,
        .intr_type      = LEDC_INTR_DISABLE
    };
    err = ledc_channel_config(&ledc_channel_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ledc_channel_config failed, err=%s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "LEDC (4MHz) configured. IO41 should now toggle at ~4MHz.");

}

// static void ledc_timer_reset(ledc_mode_t speed_mode, ledc_timer_t timer_sel)
// {
//     // 1) タイマーを一旦停止
//     ledc_timer_pause(speed_mode, timer_sel);

//     // 2) タイマーのリセットビットを立てて→下ろす (内部カウンタをクリア)
//     if (speed_mode == LEDC_LOW_SPEED_MODE) {
//         LEDC.timer_group[0].timer[timer_sel].conf.timer_rst = 1;
//         LEDC.timer_group[0].timer[timer_sel].conf.timer_rst = 0;
//     } else {
//         LEDC.timer_group[1].timer[timer_sel].conf.timer_rst = 1;
//         LEDC.timer_group[1].timer[timer_sel].conf.timer_rst = 0;
//     }

//     // 3) タイマーを再開
//     ledc_timer_resume(speed_mode, timer_sel);
// }

// ----------------------------------------------------------------------
//  GPIO割り込みハンドラ (ISR)
//  ここでは軽量処理のみ。Queueに通知する。
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    // 負エッジ割り込みを捉えたら、タスクに通知
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int dummy = 0;
    xQueueSendFromISR(s_gpio_evt_queue, &dummy, &xHigherPriorityTaskWoken);

    gpio_set_level(PIN_39, 0);

    // コンテキスト切り替え (必要なら)
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

static void IRAM_ATTR gpio_isr_handler2(void* arg)
{
    // 負エッジ割り込みを捉えたら、タスクに通知
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int dummy = 1;
    xQueueSendFromISR(s_gpio_evt_queue, &dummy, &xHigherPriorityTaskWoken);

    gpio_set_level(PIN_39, 1);

    // コンテキスト切り替え (必要なら)
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

// ----------------------------------------------------------------------
//  GPIO割り込み設定
static void setup_gpio_trigger(void)
{
    // 1) GPIO設定
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,           // 負エッジ
        .pin_bit_mask = (1ULL << KEY_WT),   // IO15
        .mode = GPIO_MODE_INPUT,                  // 入力
        .pull_up_en = false,
        .pull_down_en = false
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_config_t io_conf2 = {
        .intr_type = GPIO_INTR_NEGEDGE,           // 負エッジ
        .pin_bit_mask = (1ULL << KEY_DOWN),   // IO16
        .mode = GPIO_MODE_INPUT,                  // 入力
        .pull_up_en = false,
        .pull_down_en = false
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf2));

    // 2) ISRサービスのインストール
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // 3) ハンドラを登録
    ESP_ERROR_CHECK(
        gpio_isr_handler_add(KEY_WT, gpio_isr_handler, NULL)
    );
    ESP_ERROR_CHECK(
        gpio_isr_handler_add(KEY_DOWN, gpio_isr_handler2, NULL)
    );
}

// ----------------------------------------------------------------------
// ADC連続サンプリングドライバを初期化
static void adc_continuous_init(void)
{
    // 1) Handle用の設定
    adc_continuous_handle_cfg_t handle_cfg = {
        .max_store_buf_size = 1200,     // DMAバッファサイズ(要調整)
        .conv_frame_size = 200,         // 一度に読み取るフレームサイズ(要調整)
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&handle_cfg, &s_adc_handle));

    // 2) チャネル設定
    adc_continuous_config_t config = {0};
    config.sample_freq_hz = EXAMPLE_SAMPLE_FREQ;     // 連続サンプリング周波数(目標値)
    config.conv_mode = ADC_CONV_SINGLE_UNIT_1;
    config.format = ADC_DIGI_OUTPUT_FORMAT_TYPE2;

    // 連続サンプリングで使用するチャネル配列 (ここでは1つだけ)
    adc_digi_pattern_config_t pattern = {0};
    pattern.unit = 0;   // ADC1 => "0" (ADC_UNIT_2は index=1)
    pattern.channel = EXAMPLE_ADC_CHANNEL; // 0
    pattern.bit_width = SOC_ADC_DIGI_MAX_BITWIDTH; // 12bit
    pattern.atten = ADC_ATTEN_DB_12; // 入力ATTENを0dBに (必要に応じてDB_11などに)

    config.pattern_num = 1;
    config.adc_pattern = &pattern;
    

    ESP_ERROR_CHECK(adc_continuous_config(s_adc_handle, &config));
}




// ----------------------------------------------------------------------
//  トリガを待って => 連続サンプリング160回 => 結果出力 => 停止
static void adc_capture_task(void *arg)
{
    
    HSRAM.ADC.ucSTS = ADC_IMP;
    SYS.INF.IMPCONV_5.usCOEFA = IMPCOEF_CR_DEF;
    SYS.INF.IMPCONV_5.sCOEFB = IMPCOEF_CX_DEF;
    SYS.INF.IMPCALB.sCR = 8100;
    SYS.INF.IMPCALB.sCX = 82;
    SYS.INF.IMPCALB.sOSR = 67;
    SYS.INF.IMPCALB.sOSX = -8;
    
    ESP_LOGI(TAG, "adc_capture_task start");

    const int total_samples = ADC_IMP_STORE_MAX * IMP_DATA_NUM;
    int sample_index = 0;
    uint16_t sample_data[total_samples];
    gpio_set_level(PIN_47, 0);
    // ESP_ERROR_CHECK(adc_continuous_start(s_adc_handle));
    // gpio_set_level(PIN_47, 0);
    while (1) {
        
        // // --- IO15 負エッジ割り込みを待つ ---
        // int dummy;
        // if (xQueueReceive(s_gpio_evt_queue, &dummy, portMAX_DELAY)) {
            // 割り込みで通知があったら、ADC連続サンプリング開始
            // ESP_LOGI(TAG, "Trigger detected on IO15 -> Start ADC capture!");

            // ESP_ERROR_CHECK(adc_continuous_start(s_adc_handle));
            // ledc_timer_rst(LEDC_LOW_SPEED_MODE, LEDC_TIMER_NUM);
            // ledc_timer_reset(LEDC_LOW_SPEED_MODE,LEDC_TIMER_NUM);
            // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_TIMER_NUM, 2);

            // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_TIMER_NUM);
            // gpio_set_level(PIN_47, 0);
            adc_continuous_start(s_adc_handle);
            gpio_set_level(PIN_42, 1); //NT1000スタート時の波形の位相は初期化される
            // adc_continuous_start(s_adc_handle);

            // 必要なら20µs×200=約4ms程度で終了する想定(誤差あり)
            // 読み取り用バッファ
            uint8_t result_buffer[2048];

            //内部
            int cycle = 0;
            MEAS.ucOBJ = 0;
            MEAS.ucSTBCNT = 0;
            MEAS.ucAVGCNT = 0;
            // gpio_set_level(PIN_42, 1);
            
            gpio_set_level(PIN_39, 0);
            memset(&ADC.IMP.STORE, (uchar)0, sizeof(struct ADC_IMP_STORE_T));
            // 初期データをフラッシュ（読み取って捨てる）
            while (cycle < 6) {
                uint32_t flush_ret_num = 0;
                esp_err_t flush_ret = adc_continuous_read(
                    s_adc_handle,
                    result_buffer,
                    sizeof(result_buffer),
                    &flush_ret_num,
                    100 // タイムアウト [ms]
                );
                if (flush_ret == ESP_OK && flush_ret_num > 0) {
                    // ESP_LOGI(TAG, "Initial ADC data flushed");
                } else if (flush_ret == ESP_ERR_TIMEOUT) {
                    ESP_LOGW(TAG, "Initial ADC flush read timeout");
                } else {
                    ESP_LOGE(TAG, "ADC flush read error: %s", esp_err_to_name(flush_ret));
                    // 必要に応じてエラーハンドリング
                }
                cycle++;
            }
            // total 200サンプル (25 x 8バースト)
            
            memset(ADC.IMP.SUM.ulDATA, 0, sizeof(ADC.IMP.SUM.ulDATA));
            while (MEAS.ucSTBCNT < IMP_STB_MEAS) {
                sample_index = 0;
                
                while (sample_index < total_samples) {
                    uint32_t ret_num = 0;
                    esp_err_t ret = adc_continuous_read(
                        s_adc_handle,
                        result_buffer,
                        sizeof(result_buffer),
                        &ret_num,
                        1000 // timeout [ms]
                    );
                    if (ret == ESP_OK && ret_num > 0) {
                        // NORMALフォーマット: 1サンプル4バイト
                        //  下位2バイト: RAW (12bit)
                        //  上位バイト: channel etc.
                        int idx = 0;
                        
                        while (idx < ret_num && sample_index < total_samples) {
                            uint16_t raw_val = result_buffer[idx] |
                                            (result_buffer[idx+1] << 8);
                            // channel = result_buffer[idx+2]; など
                            // sample_data[sample_index] = ((raw_val>>= 2) & 0xFFF); // 12bit
                            sample_data[sample_index] = (raw_val & 0xFFF); // 12bit
                            ADC.IMP.SUM.ulDATA[sample_index % IMP_DATA_NUM] += sample_data[sample_index];///ADC_IMP_STORE_MAX;
                            sample_index++; 
                            // if ((sample_index%25)==0){
                            //     MeasImp();                                               
                            // }

                            idx += 4; // 次へ
                        }
                        
                        // ESP_LOGI(TAG, "lIM = %ld, lRE = %ld, ZZZ = %d", ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE, GetImpZ(ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE)); 
                    } else if (ret == ESP_ERR_TIMEOUT) {
                        ESP_LOGW(TAG, "adc_continuous_read timeout");
                    } else {
                        ESP_LOGE(TAG, "adc_continuous_read error: %s", esp_err_to_name(ret));
                        break;
                    }

                }
                MeasImp();
                
            }
            ESP_LOGI(TAG, "lIM = %ld, lRE = %ld, ZZZ = %ld", ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE, GetImpZ(ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE)); 

            //外部
            MEAS.ucOBJ = 1;
            MEAS.ucSTBCNT = 0;
            MEAS.ucAVGCNT = 0;
            gpio_set_level(PIN_39, 1);
            memset(&ADC.IMP.STORE, (uchar)0, sizeof(struct ADC_IMP_STORE_T));
            // 初期データをフラッシュ（読み取って捨てる）
            cycle = 0;
            while (cycle < 6) {
                uint32_t flush_ret_num = 0;
                esp_err_t flush_ret = adc_continuous_read(
                    s_adc_handle,
                    result_buffer,
                    sizeof(result_buffer),
                    &flush_ret_num,
                    100 // タイムアウト [ms]
                );
                if (flush_ret == ESP_OK && flush_ret_num > 0) {
                    // ESP_LOGI(TAG, "Initial ADC data flushed");
                } else if (flush_ret == ESP_ERR_TIMEOUT) {
                    ESP_LOGW(TAG, "Initial ADC flush read timeout");
                } else {
                    ESP_LOGE(TAG, "ADC flush read error: %s", esp_err_to_name(flush_ret));
                    // 必要に応じてエラーハンドリング
                }
                cycle++;
            }
            // total 200サンプル (25 x 8バースト)
            // const int total_samples = ADC_IMP_STORE_MAX * IMP_DATA_NUM;
            // sample_data[200] = {0};
            cycle = 0;
            memset(ADC.IMP.SUM.ulDATA, 0, sizeof(ADC.IMP.SUM.ulDATA));
            while (MEAS.ucSTBCNT < IMP_STB_MEAS) {
                sample_index = 0;
                while (sample_index < total_samples) {
                    uint32_t ret_num = 0;
                    esp_err_t ret = adc_continuous_read(
                        s_adc_handle,
                        result_buffer,
                        sizeof(result_buffer),
                        &ret_num,
                        1000 // timeout [ms]
                    );
                    if (ret == ESP_OK && ret_num > 0) {
                        // NORMALフォーマット: 1サンプル4バイト
                        //  下位2バイト: RAW (12bit)
                        //  上位バイト: channel etc.
                        int idx = 0;
                        
                        while (idx < ret_num && sample_index < total_samples) {
                            uint16_t raw_val = result_buffer[idx] |
                                            (result_buffer[idx+1] << 8);
                            // channel = result_buffer[idx+2]; など
                            // sample_data[sample_index] = ((raw_val>>= 2) & 0xFFF); // 12bit 下2bitつぶし
                            sample_data[sample_index] = (raw_val & 0xFFF); // 12bit
                            ADC.IMP.SUM.ulDATA[sample_index % IMP_DATA_NUM] += sample_data[sample_index];///ADC_IMP_STORE_MAX;
                            sample_index++; 
                            // if ((sample_index%25)==0){
                            //     MeasImp();                                               
                            // }

                            idx += 4; // 次へ
                        }
                        
                        // ESP_LOGI(TAG, "lIM = %ld, lRE = %ld, Z = %d", ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE, GetImpZ(ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE)); 
                    } else if (ret == ESP_ERR_TIMEOUT) {
                        ESP_LOGW(TAG, "adc_continuous_read timeout");
                    } else {
                        ESP_LOGE(TAG, "adc_continuous_read error: %s", esp_err_to_name(ret));
                        break;
                    }
                    
                }
                MeasImp();
                 
            }
            ESP_LOGI(TAG, "lIM = %ld, lRE = %ld, Z = %ld", ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE, GetImpZ(ADC.IMP.THIS[MEAS.ucOBJ].lIM, ADC.IMP.THIS[MEAS.ucOBJ].lRE)); 
            
            // 規定数(200)サンプル読み取り完了 -> 停止
            ESP_ERROR_CHECK(adc_continuous_stop(s_adc_handle));
            // gpio_set_level(PIN_47, 1);
            gpio_set_level(PIN_42, 0);
            // ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_TIMER_NUM, 0);
            // gpio_set_level(PIN_42, 0);

            // シリアル出力 (デバッグ表示)
            //  20サンプル×8バーストなど分割してもOK
            // for (int i = 0; i < total_samples; i++) {
            //     ESP_LOGI(TAG, "sample[%03d] = %d", i, sample_data[i]);
            // }

            //調整
            // FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN].lIM = -31118;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN].lRE = 565;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_OUT].lIM = -7425;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_OUT].lRE = 90;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN].lIM = 10655;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN].lRE = -28884;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_OUT].lIM = 10692;
            // FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_OUT].lRE = -28190;
            // AdjImp(&SYS.INF.IMPCALB,
			// 				&FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN],
			// 				&FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN]);
            // ESP_LOGI(TAG, "sCS = %d, sCX = %d, sOCS = %d, sOCX = %d", SYS.INF.IMPCALB.sCR,SYS.INF.IMPCALB.sCX,SYS.INF.IMPCALB.sOSR,SYS.INF.IMPCALB.sOSX );



            //補正
            
            ConvImp();
            // ESP_LOGI(TAG, "R = %d, X = %d, Z = %d", MEAS.IMPSRC[IMP_FREQ_50KHZ].sRDT, MEAS.IMPSRC[IMP_FREQ_50KHZ].sXDT, MEAS.IMPSRC[IMP_FREQ_50KHZ].usZDT);
            ESP_LOGI(TAG, "R = %d, X = %d, Z = %d", MEAS.IMP[IMP_FREQ_50KHZ].sRDT, MEAS.IMP[IMP_FREQ_50KHZ].sXDT, MEAS.IMP[IMP_FREQ_50KHZ].usZDT);
            // ESP_LOGI(TAG, "fRe = %lu, fIm = %lu, fIm = %d", ADC.IMP.SUM.ulDATA[0], ADC.IMP.SUM.ulDATA[1], SYS.INF.IMPCALB.sCR);
            // --- 次の割り込みトリガを待つ ---
        // }
    vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    // 割り込み通知キューを作成
    s_gpio_evt_queue = xQueueCreate(10, sizeof(int));
    
    // IMP_GPIO設定
    setup_gpio_imp();
    // vTaskDelay(pdMS_TO_TICKS(500));

    // GPIO割り込み設定 (IO15 負エッジ)
    setup_gpio_trigger();

    //LCD
    xTaskCreate(&lcd1602_task, "lcd1602_task", 4096, NULL, 5, NULL);

    // ADC連続サンプリングドライバ 初期化
    adc_continuous_init();

    // タスク起動
    xTaskCreate(adc_capture_task, "adc_capture_task", 8192, NULL, 6, NULL);
}
