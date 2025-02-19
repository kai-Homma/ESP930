/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	at_inc.h										*/
/*																			*/
/*		モジュール名称	：	インクルードファイル定義						*/
/*																			*/
/*		作成年月日		：	2022/ 3/31	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 3/31	小山		recog01					*/
/*								個人認識機能削除							*/
/*							2022/ 4/ 1	小山		step01					*/
/*								ステップオン機能削除						*/
/*							2022/ 4/18	小山		lcd01					*/
/*								LCD削除										*/
/*							2022/ 4/25	小山		led01					*/
/*								LED追加										*/
/*							2022/ 5/11	小山		tmphmd01				*/
/*								温湿度測定追加								*/
/*							2022/ 5/12	小山		voice01					*/
/*								音声追加									*/
/*							2022/ 5/16	久保田		fcj01					*/
/*								フレイル代理計算追加						*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/
#include <stddef.h>					/* 共通定義							*/
// #include	"iodefine.h"				/* マイコン・レジスタ定義			*/
#include	"at_init.h"					/* 初期設定ファイル					*/
// #include	<stddef.h>					/* 共通定義							*/
#include	"at_def.h"					/* 定数定義							*/
// #include	"time.h"					/* 時計								*/
// #include	"key.h"						/* キー								*/
// #include	"i2c_drv.h"					/* Ｉ２Ｃドライバ					*/
// #include	"ble.h"						/* BLE通信							*/
// #include	"mem.h"						/* メモリ							*/
/* tmphmd01 add start														*/
// #include	"tmphmd.h"					/* 温湿度							*/
/* tmphmd01 add end															*/
/* voice01 add start														*/
// #include	"voice.h"					/* 音声								*/
/* voice01 add end															*/
/* lcd01 del start															*/
/*#include	"dsp.h"						 * 表示								*/
/* lcd01 del end															*/
/* led01 add start															*/
// #include	"led.h"						/* LED								*/
/* led01 add end															*/
// #include	"weight.h"					/* 荷重								*/
/* step01 del start															*/
/*#include	"step.h"					 * ステップON						*/
/* step01 del end															*/
#include	"imp.h"						/* インピーダンス					*/
// #include	"rev.h"						/* 回帰式							*/
/* recog01 del start														*/
/*#include	"recog.h"					 * 個人認識							*/
/* recog01 del end															*/
/* fcj01 add start															*/
// #include	"fcj.h"						/* フレイル代理計算					*/
/* fcj01 add end															*/
// #include	"req_adc.h"					/* ADC要求							*/
// #include	"req_etc.h"					/* その他要求						*/
/* voice01 del start														*/
/*#include	"buzz.h"					 * ブザー							*/
/* voice01 del end															*/
// #include	"lllib.h"					/* 低レベルライブラリ				*/
#include	"sub.h"						/* 共通関数							*/
// #include	"func.h"					/* ファンクション					*/
#include	"main.h"					/* メイン							*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c.h"

#include "adc.h"
#include "lcd.h"

