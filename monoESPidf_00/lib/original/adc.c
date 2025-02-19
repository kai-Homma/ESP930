#include	"at_inc.h"					/* インクルードファイル				*/
#include <string.h>

#include "esp_log.h"
#include "freertos/task.h"
#include "esp_adc/adc_continuous.h"
#include "freertos/FreeRTOS.h"

extern const char *TAG;
adc_continuous_handle_t s_adc_handle;
// ----------------------------------------------------------------------
// ADC連続サンプリングドライバを初期化
void adc_continuous_init(void)
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

void imp_calib(void)
{
    FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN].lIM = -31118;
    FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN].lRE = 565;
    FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_OUT].lIM = -7425;
    FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_OUT].lRE = 90;
    FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN].lIM = 10655;
    FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN].lRE = -28884;
    FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_OUT].lIM = 10692;
    FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_OUT].lRE = -28190;
    AdjImp(&SYS.INF.IMPCALB,
							&FUNC.ADJ.CALB.IMP[IMP_BASE_H][IMP_OBJ_IN],
							&FUNC.ADJ.CALB.IMP[IMP_BASE_L][IMP_OBJ_IN]);

    SYS.INF.IMPCONV_5.usCOEFA = IMPCOEF_CR_DEF;
    SYS.INF.IMPCONV_5.sCOEFB = IMPCOEF_CX_DEF;
    SYS.INF.IMPCALB.sCR = SYS.INF.IMPCALB.sCR;
    SYS.INF.IMPCALB.sCX = SYS.INF.IMPCALB.sCX;
    SYS.INF.IMPCALB.sOSR = SYS.INF.IMPCALB.sOSR;
    SYS.INF.IMPCALB.sOSX = SYS.INF.IMPCALB.sOSX;
}


// ----------------------------------------------------------------------
//  トリガを待って => 連続サンプリング160回 => 結果出力 => 停止
void adc_capture_task(void *arg)
{
    
    HSRAM.ADC.ucSTS = ADC_IMP;

    
    ESP_LOGI(TAG, "adc_capture_task start");

    const int total_samples = ADC_IMP_STORE_MAX * IMP_DATA_NUM;
    int sample_index = 0;
    uint16_t sample_data[total_samples];
    imp_calib();
    gpio_set_level(PIN_47, 0);
    // ESP_ERROR_CHECK(adc_continuous_start(s_adc_handle));
    // gpio_set_level(PIN_42, 1); //NT1000スタート時の波形の位相は初期化される
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
            gpio_set_level(PIN_42, 0);
            // gpio_set_level(PIN_47, 1);

            // ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_TIMER_NUM, 0);
            // gpio_set_level(PIN_42, 0);

            // シリアル出力 (デバッグ表示)
            //  20サンプル×8バーストなど分割してもOK
            // for (int i = 0; i < total_samples; i++) {
            //     ESP_LOGI(TAG, "sample[%03d] = %d", i, sample_data[i]);
            // }

            //調整
            // ESP_LOGI(TAG, "sCS = %d, sCX = %d, sOCS = %d, sOCX = %d", SYS.INF.IMPCALB.sCR,SYS.INF.IMPCALB.sCX,SYS.INF.IMPCALB.sOSR,SYS.INF.IMPCALB.sOSX );
            

            //補正
            
            ConvImp();
            LCD_3num((float)MEAS.IMP[IMP_FREQ_50KHZ].sRDT/10.0f, (float)MEAS.IMP[IMP_FREQ_50KHZ].sXDT/10.0f, (float)MEAS.IMP[IMP_FREQ_50KHZ].usZDT/10.0f);
            // ESP_LOGI(TAG, "R = %d, X = %d, Z = %d", MEAS.IMPSRC[IMP_FREQ_50KHZ].sRDT, MEAS.IMPSRC[IMP_FREQ_50KHZ].sXDT, MEAS.IMPSRC[IMP_FREQ_50KHZ].usZDT);
            ESP_LOGI(TAG, "R = %d, X = %d, Z = %d", MEAS.IMP[IMP_FREQ_50KHZ].sRDT, MEAS.IMP[IMP_FREQ_50KHZ].sXDT, MEAS.IMP[IMP_FREQ_50KHZ].usZDT);
            // ESP_LOGI(TAG, "fRe = %lu, fIm = %lu, fIm = %d", ADC.IMP.SUM.ulDATA[0], ADC.IMP.SUM.ulDATA[1], SYS.INF.IMPCALB.sCR);
            // --- 次の割り込みトリガを待つ ---
        // }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
