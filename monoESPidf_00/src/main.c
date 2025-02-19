#include <stdio.h>
#include <stddef.h>					/* 共通定義							*/
#include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "esp_log.h"
// #include "esp_err.h"
// #include "esp_adc/adc_continuous.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "driver/i2c.h"

#include "at_inc.h"					/* インクルードファイル				*/
#include "i2c-lcd1602.h"

// #include "rom/uart.h"
// #include "smbus.h"
// #include "i2c-lcd1602.h"


const char *TAG = "EXAMPLE";

union	ADC_T	ADC;					/* ADC情報							*/
struct	HSRAM_T	HSRAM;					/* 高速ＲＡＭ情報					*/
struct	MEAS_T	MEAS;					/* 測定情報							*/

// struct	MTR_T	MTR;					/* 監視情報							*/
union	SYS_T	SYS;					/* システム情報						*/
union	FUNC_T	FUNC;					/* ファンクション情報				*/

// 割り込みイベント用のキュー
QueueHandle_t s_gpio_evt_queue = NULL;
// static QueueHandle_t s_gpio_evt_queue2 = NULL;
// ADCドライバのハンドル
// adc_continuous_handle_t s_adc_handle = NULL;
// extern i2c_lcd1602_info_t *lcd_info;

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

void app_main(void)
{
    // 割り込み通知キューを作成
    s_gpio_evt_queue = xQueueCreate(10, sizeof(int));
    vTaskDelay(pdMS_TO_TICKS(5000));
    LCD_init();
    
    // IMP_GPIO設定
    setup_gpio_imp();
    // vTaskDelay(pdMS_TO_TICKS(500));

    // GPIO割り込み設定 (IO15 負エッジ)
    setup_gpio_trigger();

    //LCD
    // xTaskCreate(&lcd1602_task, "lcd1602_task", 4096, NULL, 5, NULL);

    // ADC連続サンプリングドライバ 初期化
    adc_continuous_init();

    // タスク起動
    xTaskCreate(adc_capture_task, "adc_capture_task", 12288, NULL, 6, NULL);
}
