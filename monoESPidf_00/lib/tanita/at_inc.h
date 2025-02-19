/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	at_inc.h										*/
/*																			*/
/*		���W���[������	�F	�C���N���[�h�t�@�C����`						*/
/*																			*/
/*		�쐬�N����		�F	2022/ 3/31	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 3/31	���R		recog01					*/
/*								�l�F���@�\�폜							*/
/*							2022/ 4/ 1	���R		step01					*/
/*								�X�e�b�v�I���@�\�폜						*/
/*							2022/ 4/18	���R		lcd01					*/
/*								LCD�폜										*/
/*							2022/ 4/25	���R		led01					*/
/*								LED�ǉ�										*/
/*							2022/ 5/11	���R		tmphmd01				*/
/*								�����x����ǉ�								*/
/*							2022/ 5/12	���R		voice01					*/
/*								�����ǉ�									*/
/*							2022/ 5/16	�v�ۓc		fcj01					*/
/*								�t���C���㗝�v�Z�ǉ�						*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/
#include <stddef.h>					/* ���ʒ�`							*/
// #include	"iodefine.h"				/* �}�C�R���E���W�X�^��`			*/
#include	"at_init.h"					/* �����ݒ�t�@�C��					*/
// #include	<stddef.h>					/* ���ʒ�`							*/
#include	"at_def.h"					/* �萔��`							*/
// #include	"time.h"					/* ���v								*/
// #include	"key.h"						/* �L�[								*/
// #include	"i2c_drv.h"					/* �h�Q�b�h���C�o					*/
// #include	"ble.h"						/* BLE�ʐM							*/
// #include	"mem.h"						/* ������							*/
/* tmphmd01 add start														*/
// #include	"tmphmd.h"					/* �����x							*/
/* tmphmd01 add end															*/
/* voice01 add start														*/
// #include	"voice.h"					/* ����								*/
/* voice01 add end															*/
/* lcd01 del start															*/
/*#include	"dsp.h"						 * �\��								*/
/* lcd01 del end															*/
/* led01 add start															*/
// #include	"led.h"						/* LED								*/
/* led01 add end															*/
// #include	"weight.h"					/* �׏d								*/
/* step01 del start															*/
/*#include	"step.h"					 * �X�e�b�vON						*/
/* step01 del end															*/
#include	"imp.h"						/* �C���s�[�_���X					*/
// #include	"rev.h"						/* ��A��							*/
/* recog01 del start														*/
/*#include	"recog.h"					 * �l�F��							*/
/* recog01 del end															*/
/* fcj01 add start															*/
// #include	"fcj.h"						/* �t���C���㗝�v�Z					*/
/* fcj01 add end															*/
// #include	"req_adc.h"					/* ADC�v��							*/
// #include	"req_etc.h"					/* ���̑��v��						*/
/* voice01 del start														*/
/*#include	"buzz.h"					 * �u�U�[							*/
/* voice01 del end															*/
// #include	"lllib.h"					/* �჌�x�����C�u����				*/
#include	"sub.h"						/* ���ʊ֐�							*/
// #include	"func.h"					/* �t�@���N�V����					*/
#include	"main.h"					/* ���C��							*/

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

