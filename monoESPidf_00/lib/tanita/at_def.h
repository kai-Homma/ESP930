/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	at_def.h										*/
/*																			*/
/*		���W���[������	�F	�萔��`										*/
/*																			*/
/*		�쐬�N����		�F	2022/ 2/15	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 2/15	���R		key01					*/
/*								�L�[�z�u�ύX								*/
/*							2022/ 2/15	���R		spec01					*/
/*								�d�l�ύX�i�l�ԍ����A�f�t�H���g�N��j		*/
/*							2022/ 2/21	���R		setup01					*/
/*								�����ݒ胂�[�h�폜							*/
/*							2022/ 2/21	���R		pset01					*/
/*								�l�ݒ胂�[�h�폜							*/
/*							2022/ 2/21	���R		item01					*/
/*								���ڕ\�����[�h�ύX�i�\���͖����j			*/
/*							2022/ 2/22	���R		step01					*/
/*								�X�e�b�v�I���@�\�폜						*/
/*							2022/ 3/ 3	���R		ble01					*/
/*								BLE�ʐM�d�l�E�f�[�^�ύX						*/
/*							2022/ 4/ 4	���R		ble02					*/
/*								BLE�ʐM�Œ����E����							*/
/*							2022/ 5/12	���R		voice01					*/
/*								�����ǉ�									*/
/*							2022/ 5/16	�v�ۓc		fcj01					*/
/*								�t���C���㗝�v�Z�ǉ�						*/
/*							2022/ 6/23	���R		init01					*/
/*								���������[�h�ǉ�							*/
/*							2022/ 6/24	�v�ۓc		tmphmd02				*/
/*								�����x����ǉ�								*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*		�ėp																*/
/****************************************************************************/
#define	FLOAT_ULONG_MAX	(float)4.293E+9F/* float��ulong�L���X�g�ő�l		*/
#define	FLOAT_ROUND		(float)5E-1F	/* float�ː����L���X�g�ۂߒl		*/

#define	ON				(uchar)1
#define	OFF				(uchar)0
#define	OK				(uchar)1 //�ύX
#define	NG				(uchar)0
#define	YES				(uchar)1
#define	NO				(uchar)0
#define	SINGLE			(uchar)1
#define	DUAL			(uchar)2
#define	READY			(uchar)0
#define	BUSY			(uchar)1 //�ύX
#define	ILAST			(uchar)0
#define	ITHIS			(uchar)1
#define	RST				(uchar)2
/* fcj01 add start															*/
#define	TRUE			(uchar)1
#define	FALSE			(uchar)0
/* fcj01 add end															*/


/****************************************************************************/
/*		����	���R���p�C���G���[����̈�,�{�t�@�C���Œ�`					*/
/****************************************************************************/
enum	COUNTRY_ID						/* ������							*/
{
	COUNTRY_MIN,						/*		�ŏ�						*/
	COUNTRY_US = COUNTRY_MIN,			/*		�t�r�`						*/
	COUNTRY_EU,							/*		�d�t						*/
	COUNTRY_AS,							/*		�A�W�A						*/
	COUNTRY_JP,							/*		���{						*/
	COUNTRY_MAX							/*		�ő�						*/
};
#define	COUNTRY_DEF		COUNTRY_JP		/*		�f�t�H���g					*/

enum	UNIT_ID							/* �P��								*/
{
	UNIT_MIN,							/*		�ŏ�						*/
	UNIT_KG = UNIT_MIN,					/*		����						*/
	UNIT_LB,							/*		����						*/
	UNIT_STLB,							/*		�����|����					*/
	UNIT_MAX							/*		�ő�						*/
};
#define	UNIT_DEF		UNIT_KG			/*		�f�t�H���g					*/

//key01 add start
enum	VOL_ID							/* ����								*/
{
	VOL_MIN,							/*		�ŏ�						*/
	VOL_OFF = VOL_MIN,					/*		OFF							*/
	VOL_LO,								/*		���ʏ�						*/
	VOL_HI,								/*		���ʑ�						*/
	VOL_MAX								/*		�ő�						*/
};
//key01 add end

/* ble01 add start															*/
										/* �ʐM�d�l							*/
#define	COMSPEC_NONE	(uchar)0x00		/*		����						*/
#define	COMSPEC_BLE		(uchar)0x01		/*		BLE							*/
#define	COMSPEC_WLAN	(uchar)0x02		/*		WLAN						*/

#define	BLE_MASK		(uchar)0x01		/*		BLE�}�X�N					*/
#define	WLAN_MASK		(uchar)0x02		/*		WLAN�}�X�N					*/
#define	COM_MASK		(uchar)0x03		/*		�ʐM�}�X�N					*/
/* ble01 add end															*/

										/* �l�ԍ�							*/
#define	PNUM_1		(uchar)0			/*		1							*/
#define	PNUM_2		(uchar)1			/*		2							*/
#define	PNUM_3		(uchar)2			/*		3							*/
#define	PNUM_4		(uchar)3			/*		4							*/
/* spec01 chg start															*/
/*#define	PNUM_5		(uchar)4		 *		5							*/
/*#define	PNUM_MAX	PNUM_5			 *		�ő�ԍ�	5				*/
#define	PNUM_MAX	PNUM_4				/*		�ő�ԍ�	4				*/
/* spec01 chg end															*/
#define	PREG_MAX	(uchar)(PNUM_MAX + (uchar)1)
										/*		�ő�l��					*/
#define	PNUM_GUEST	PREG_MAX			/*		�Q�X�g						*/
#define	PNUM_STPON	(uchar)(PNUM_GUEST + (uchar)1)
										/*		�X�e�b�vON(�l�ԍ����m��)	*/
#define	PNUM_WEIT	(uchar)(PNUM_STPON + (uchar)1)
										/*		�̏d����					*/

#define	PSID_INIT		(uchar)0		/* �h�c����							*/
#define	PSID_SETMIDL_AGE	(uchar)0x51	/*	�ݒ�r���h�c	�N��(���N)��	*/
#define	PSID_SETMIDL_SEX	(uchar)0x52	/*					���ʍ�			*/
#define	PSID_SETMIDL_HEIT	(uchar)0x53	/*					�g����			*/
#define	PSID_SETMIDL_AL		(uchar)0x54	/*					�������x��		*/
#define	PSID_SET		(uchar)0x55		/* �h�c�L��							*/

										/* �̌^								*/

enum	FIG_ID							/* 		�h�c						*/
{
	FIG_MAN,							/*	0				�j��			*/
	FIG_WOMAN,							/*	1				����			*/
	FIG_GENDER,							/*	2				����			*/
	FIG_AT_MAN = FIG_GENDER,			/*	2				�A�X���[�g�j��	*/
	FIG_AT_WOMAN,						/*	3				�A�X���[�g����	*/
	FIG_GENDER_AT,						/*	4				�A�X���[�g		*/
	FIG_BOY = FIG_GENDER_AT,			/*	4				�j��			*/
	FIG_GIRL,							/*	5				����			*/
	FIG_NUM,							/*	6				�P�̐�			*/
	FIG_US_MAN = FIG_MAN,				/*	0		����	�j��			*/
	FIG_US_WOMAN,						/*	1				����			*/
	FIG_US_AT_MAN,						/*	2				�A�X���[�g�j��	*/
	FIG_US_AT_WOMAN,					/*	3				�A�X���[�g����	*/
	FIG_US_BOY,							/*	4				�j��			*/
	FIG_US_GIRL,						/*	5				����			*/
	FIG_AS_MAN,							/*	6		�A�W�A	�j��			*/
	FIG_AS_WOMAN,						/*	7				����			*/
	FIG_AS_AT_MAN,						/*	8				�A�X���[�g�j��	*/
	FIG_AS_AT_WOMAN,					/*	9				�A�X���[�g����	*/
	FIG_AS_BOY,							/*	10				�j��			*/
	FIG_AS_GIRL,						/*	11				����			*/
	FIG_JP_MAN,							/*	12		���{	�j��			*/
	FIG_JP_WOMAN,						/*	13				����			*/
	FIG_JP_AT_MAN,						/*	14				�A�X���[�g�j��	*/
	FIG_JP_AT_WOMAN,					/*	15				�A�X���[�g����	*/
	FIG_JP_BOY,							/*	16				�j��			*/
	FIG_JP_GIRL,						/*	17				����			*/
	FIG_ALL								/*	18				�S�̐�			*/
};

#define	SEX_MASK		(uchar)0x01		/*		���ʃ}�X�N					*/
#define	AT_MASK			(uchar)0x02		/*		�A�X���[�g�}�X�N			*/
#define	FIG_MASK		(uchar)0x03		/*		�̌^�}�X�N					*/

#define	FIG_MIN			FIG_MAN			/*		�ŏ�						*/
#define	FIG_MAX			FIG_WOMAN		/*		�ő�						*/
#define	FIG_AT_MAX		FIG_AT_WOMAN	/*		�A�X���[�g�ő�				*/
#define	FIG_DEF			FIG_WOMAN		/*		�f�t�H���g					*/

#define	AGE_MIN				(uchar)0	/* �N��		�ŏ��l					*/
#define	AGE_MIN_ISCAN_US	(uchar)5	/*						�t�r�`		*/
#define	AGE_MIN_ISCAN_EU	(uchar)5	/*						�d�t		*/
#define	AGE_MIN_ISCAN_AS	(uchar)5	/*						�A�W�A		*/
#define	AGE_MIN_ISCAN_JP	(uchar)6	/*						���{		*/
#define	AGE_MIN_AGE7		(uchar)7	/*						7��			*/
#define	AGE_ADLT			(uchar)18	/*			���l:18��				*/
#define	AGE_ADLT_US_EU		(uchar)16	/*			��A�����l	�t�r�`&�d�t	*/
#define	AGE_ADLT_AS_JP		(uchar)15	/*						�A�W�A&���{	*/
/* spec01 chg start															*/
/*#define	AGE_DEF			(uchar)40	 *			�f�t�H���g:40��			*/
#define	AGE_DEF				(uchar)50	/*			�f�t�H���g:50��			*/
/* spec01 chg end															*/
#define	AGE_MAX_ISCAN		(uchar)99	/*			�̑g������ő�:99��		*/
#define	AGE_MAX				(uchar)255	/*			�ő�l					*/

										/* �P�ʃJ�E���g����\				*/
#define	IN_PER_FT			(ushort)12	/*	12in/ft		12�C���`��1�t�B�[�g	*/
										/*						����30.5cm	*/
#define CNT_PER_CM_DSP		(ushort)100	/*	100cnt/1cm	1cnt��0.01cm		*/
#define CNT_PER_CM_DSP2		(ushort)10	/*	10cnt/1cm	1cnt��0.1cm			*/
#define CNT_PER_IN_DSP		(ushort)254	/*	254cnt/in	25.4cnt��0.1�C���`	*/
#define CNT_PER_IN_DSP2		(ushort)10	/*	10cnt/in	1cnt��0.1�C���`		*/
#define CNT_PER_FT_DSP		(ushort)(IN_PER_FT * CNT_PER_IN_DSP)
										/*	3048cnt/ft	3048cnt��1�t�B�[�g	*/

										/* �g��								*/
#define	HEIT_CM_MIN			(ushort)(90 * CNT_PER_CM_DSP)
										/*		�ŏ�:90cm(9000)				*/
#define	HEIT_CM_MIN_US		(ushort)(100 * CNT_PER_CM_DSP)
										/*		USA�ŏ�:100cm(10000)		*/
#define	HEIT_CM_MAX			(ushort)(220 * CNT_PER_CM_DSP)
										/*		�ő�:220cm(22000)			*/
#define	HEIT_CM_DEF			(ushort)(170 * CNT_PER_CM_DSP)
										/*		�f�t�H���g:170cm(17000)		*/
#define	HEIT_CM_DEF_JP		(ushort)(160 * CNT_PER_CM_DSP)
										/*		���{�f�t�H���g:160cm(16000)	*/
#define	HEIT_CM_TEST		(ushort)(170 * CNT_PER_CM_DSP)
										/*		�������[�h�p:170.0cm(17000)	*/
//spec01 chg start
//#define	HEIT_CM_NOTCH		(ushort)(FLOAT_ROUND * CNT_PER_CM_DSP)
//										/*		���ݕ�:0.5cm(50)			*/
#define	HEIT_CM_NOTCH		(ushort)10	/*		���ݕ�:0.1cm(10)			*/
//spec01 chg end
#define	HEIT_CM_NOTCH_HI	(ushort)(1 * CNT_PER_CM_DSP)
										/*		���ݕ�(������):1.0cm(100)	*/
#define	HEIT_FI_MIN			(ushort)(3 * CNT_PER_FT_DSP)
										/*		�ŏ�:3ft0in(9144)			*/
#define	HEIT_FI_MIN_US		(ushort)((3 * CNT_PER_FT_DSP) + (4 * CNT_PER_IN_DSP))
										/*		USA�ŏ�:3ft4in(10160)		*/
#define	HEIT_FI_MAX			(ushort)((7 * CNT_PER_FT_DSP) + (3 * CNT_PER_IN_DSP))
										/*		�ő�:7ft3in(22098)			*/
#define	HEIT_FI_DEF			(ushort)((5 * CNT_PER_FT_DSP) + (7 * CNT_PER_IN_DSP))
										/*		�f�t�H���g:5ft7in(17018)	*/
#define	HEIT_IN_NOTCH		(ushort)(FLOAT_ROUND * CNT_PER_IN_DSP)
										/*		���ݕ�:0.5in(127)			*/
#define	HEIT_IN_NOTCH_HI	(ushort)(FLOAT_ROUND * CNT_PER_IN_DSP)
										/*		���ݕ�:0.5in(127)			*/

enum	AL_ID							/* �������x							*/
{
	AL_LO,								/*			��						*/
	AL_MID,								/*			��						*/
	AL_HI,								/*			��						*/
	AL_MAX								/*			�ő�					*/
};
#define	AL_DEF			AL_LO			/*			�f�t�H���g				*/
#define	AL_ATH			AL_HI			/*			�A�X���[�g				*/

#define	NAME_LEN_LONG	(uchar)10		/* ���O		�f�[�^���i�����O�j		*/
#define	NAME_LEN_SHORT	(uchar)5		/*			�f�[�^���i�V���[�g�j	*/
#define	NAME_LEN_NONE	(uchar)0		/*			�f�[�^���i0�i�Ȃ��j�j	*/

										/* SNS�ݒ�							*/
#define	SNS_DEF		OFF					/*			�f�t�H���g				*/

										/* ��������							*/
#define	PLSMEAS_DEF		OFF				/*			�f�t�H���g				*/

enum	MSTS_ID							/* ���C�����						*/
{
	MSTS_POFF,							/*	�d��OFF							*/
	MSTS_PON,							/*	�d��ON							*/
	MSTS_SAVE,							/*	�f�[�^�ۑ�						*/
	MSTS_NTC,							/*	�ʒm��ʁ@���f�[�^�ۑ��͂�������*/
//init01 add start
	MSTS_INIT,							/*	������							*/
//init01 add end
	MSTS_PAIR,							/*	�y�A�����O						*/
	MSTS_SND,							/*	�f�[�^���M						*/
	MSTS_SETUP,							/*	�����ݒ�						*/
//pset01 del start
//	MSTS_PSET,							/*	�l�ݒ�						*/
//pset01 del end
//weit02 del start
//	MSTS_WEIT,							/*	�̏d����						*/
//weit02 del end
	MSTS_ISCAN,							/*	�̑g������						*/
	MSTS_ITEM,							/*	���荀��				�����܂�*/
//ble02 del start
//	MSTS_CNT,							/*	�J�E���g���[�h					*/
//	MSTS_ADJ,							/*	�������[�h						*/
//ble02 del end
	MSTS_MEM,							/*	���������[�h�i�����������[�h�j	*/
	MSTS_BLETEST,						/*	BLE�ʐM�������[�h				*/
//ble02 del start
//	MSTS_IMPCHK,						/*	�C���s�������[�h				*/
//ble02 del end
	MSTS_TEST							/*	�e�X�g���[�h					*/
};

enum	SSTS_ID							/* �T�u���							*/
{
	SSTS_INIT = 0,						/*	����(����)						*/

										/* �d��OFF							*/
/* step01 chg start															*/
/*	SSTS_POFF_STPON = SSTS_INIT + 1,	 *	�X�e�b�vON						*/
/*	SSTS_POFF_STPOFF,					 *	�X�e�b�vOFF						*/
/*	SSTS_POFF_IMP,						 *	�C���s							*/
/*	SSTS_POFF_ZERO,						 *	�O�_							*/
	SSTS_POFF_NORM = SSTS_INIT + 1,		/*	�ʏ�							*/
/* step01 chg end															*/
	SSTS_POFF_MAX,						/*	�ő�							*/

										/* �d��ON							*/
	SSTS_PON_NORM = SSTS_INIT + 1,		/*	�ʏ�							*/
	SSTS_PON_FN0,						/*	�@�\0	����					*/
	SSTS_PON_FN1,						/*	�@�\1	����					*/
	SSTS_PON_FN2,						/*	�@�\2	����					*/
/* key01 chg start															*/
/*	SSTS_PON_FN3_IMP,					 *	�@�\3	�C���s�������[�h		*/
/*	SSTS_PON_FN4_MEM,					 *	�@�\4	���������[�h			*/
/*	SSTS_PON_FN5_ISCAN,					 *	�@�\5	���b���m�F				*/
/*	SSTS_PON_FN6_BLETEST,				 *	�@�\6	BLE�ʐM����				*/
/*	SSTS_PON_FN7_WEIT,					 *	�@�\7	�퍷�m�F				*/
/*	SSTS_PON_SET_PAIR,					 *	�y�A�����O						*/
/*	SSTS_PON_SET_AREA,					 *	�n��ݒ�						*/
/*	SSTS_PON_SET_TIME,					 *	���v�ݒ�						*/
	SSTS_PON_FN3,						/*	�@�\3							*/
	SSTS_PON_FN4,						/*	�@�\4							*/
	SSTS_PON_FN5,						/*	�@�\5							*/
	SSTS_PON_FN6,						/*	�@�\6							*/
	SSTS_PON_SET_PAIR,					/*	�y�A�����O						*/
/* key01 chg end															*/
//init01 add start
	SSTS_PON_INIT,						/*	������							*/
//init01 add end
	SSTS_PON_MAX,						/*	�ő�							*/

										/* �f�[�^�ۑ�						*/
	SSTS_SAVE_MAX = SSTS_INIT + 1,		/*	�ő�							*/

										/* �ʒm								*/
//voice01 chg start
//	SSTS_NTC_EXE = SSTS_INIT + 1,		/*	���s							*/
	SSTS_NTC_BUZZ = SSTS_INIT + 1,		/*	�u�U�[�ʒm						*/
	SSTS_NTC_EXE,						/*	���s�i�����ʒm�j				*/
//voice01 chg end
	SSTS_NTC_MAX,						/*	�ő�							*/

//init01 add start
										/* ������							*/
	SSTS_INIT_STEP1 = SSTS_INIT + 1,	/*	����1�҂�						*/
	SSTS_INIT_STEP2,					/*	����2�҂�						*/
	SSTS_INIT_STEP3,					/*	����3�҂�						*/
	SSTS_INIT_STEP4,					/*	����4�҂�						*/
//t024 add start
	SSTS_INIT_DEL_1,					/*	�l1�����҂�					*/
	SSTS_INIT_DEL_2,					/*	�l2�����҂�					*/
	SSTS_INIT_DEL_3,					/*	�l3�����҂�					*/
	SSTS_INIT_DEL_4,					/*	�l4�����҂�					*/
	SSTS_INIT_DEL_EXE,					/*	�������s						*/
//t024 add end
	SSTS_INIT_EXE,						/*	���������s						*/
	SSTS_INIT_MAX,						/*	�ő�							*/
//init01 add end
										/* �y�A�����O						*/
	SSTS_PAIR_DATE = SSTS_INIT + 1,		/*	�����擾						*/
	SSTS_PAIR_SYS,						/*	�@����擾					*/
/* ble01 add start															*/
	SSTS_PAIR_PSNUM,					/*	�l�ԍ�						*/
//t023 add start
	SSTS_PAIR_CLR_WAIT,					/*	�l�㏑���m�F					*/
//t023 add end
/* ble01 add end															*/
	SSTS_PAIR_PSINF,					/*	�l���						*/
	SSTS_PAIR_WT_UUID,					/*	�t�t�h�c����					*/
	SSTS_PAIR_ISCAN,					/*	�̑g������						*/
	SSTS_PAIR_START_MEAS,				/*	����J�n						*/
//ble01 del start
//	SSTS_PAIR_NUM,						/*	�ԍ�							*/
//ble01 del end
	SSTS_PAIR_CNF,						/*	�y�A�����O���ʊm�F				*/
	SSTS_PAIR_MAX,						/*	�ő�							*/

										/* �f�[�^���M						*/
	SSTS_SND_CHK_UUID = SSTS_INIT + 1,	/*	�t�t�h�c�m�F					*/
	SSTS_SND_DATE,						/*	�����擾						*/
	SSTS_SND_SYS,						/*	�@����擾					*/
	SSTS_SND_PSINF,						/*	�l���						*/
	SSTS_SND_ISCAN,						/*	�̑g������						*/
	SSTS_SND_START_MEAS,				/*	����J�n						*/
//ble01 del start
//	SSTS_SND_NUM,						/*	�ԍ�							*/
//ble01 del end
	SSTS_SND_NONSND,					/*	�����M�f�[�^					*/
	SSTS_SND_CNF,						/*	���M���ʊm�F					*/
//fcj01 add start
	SSTS_SND_FRJD,						/*	�t���C������㗝�v�Z			*/
//fcj01 add end
//ble01 add start
	SSTS_SND_PSET_DEL,					/*	�l����						*/
//ble01 add end
	SSTS_SND_MAX,						/*	�ő�							*/

//setup01 del start
//										/* �����ݒ�							*/
//	SSTS_SETUP_AREA = SSTS_INIT + 1,	/*	�n��							*/
//	SSTS_SETUP_YEAR,					/*	�N								*/
//	SSTS_SETUP_MONTH,					/*	��								*/
//	SSTS_SETUP_DAY,						/*	��								*/
//	SSTS_SETUP_HOUR,					/*	��								*/
//	SSTS_SETUP_MIN,						/*	��								*/
//	SSTS_SETUP_CNF_AREA,				/*	�n��m�F						*/
//	SSTS_SETUP_CNF_YEAR,				/*	�N�m�F							*/
//	SSTS_SETUP_CNF_MD,					/*	�����m�F						*/
//	SSTS_SETUP_CNF_HM,					/*	�����m�F						*/
//	SSTS_SETUP_ZERO_WAIT,				/*	�O�_�҂�						*/
//	SSTS_SETUP_MAX,						/*	�ő�							*/
//setup01 del end
//pset01 del start
//										/* �l�ݒ�							*/
//	SSTS_PSET_NUM = SSTS_INIT + 1,		/*	�ԍ�							*/
//	SSTS_PSET_AGE,						/*	�N��							*/
//	SSTS_PSET_YEAR,						/*	�N								*/
//	SSTS_PSET_MONTH,					/*	��								*/
//	SSTS_PSET_DAY,						/*	��								*/
//	SSTS_PSET_SEX_WAIT,					/*	���ʑ҂�						*/
//	SSTS_PSET_SEX,						/*	����/�̌^						*/
//	SSTS_PSET_HEIT,						/*	�g��							*/
//	SSTS_PSET_HEIT_WRITE,				/*	�g��������						*/
//	SSTS_PSET_AL,						/*	�������x						*/
//	SSTS_PSET_AL_WRITE,					/*	�������x������					*/
//	SSTS_PSET_CLR_WAIT,					/*	�����҂�						*/
//	SSTS_PSET_CLR,						/*	�����I��						*/
//	SSTS_PSET_CNF_CLR,					/*	�����m�F						*/
//	SSTS_PSET_CNF_AGE,					/*	�N��m�F						*/
//	SSTS_PSET_CNF_HEIT,					/*	�g���m�F						*/
//	SSTS_PSET_CNF_AL,					/*	�������x�m�F					*/
//	SSTS_PSET_DEL_AGE,					/*	�폜�N��						*/
//	SSTS_PSET_DEL_HEIT,					/*	�폜�g��						*/
//	SSTS_PSET_DEL_AL,					/*	�폜�������x					*/
//	SSTS_PSET_DEL_WAIT,					/*	�폜�҂�						*/
//	SSTS_PSET_DEL_SEL,					/*	�폜�I��						*/
//	SSTS_PSET_DEL_CNF,					/*	�폜�m�F						*/
//	SSTS_PSET_DEL_CNF_BAR,				/*	�폜�o�[�m�F					*/
//	SSTS_PSET_MAX,						/*	�ő�							*/
//pset01 del end
//weit02 del start
//										/* �̏d����							*/
//	SSTS_WEIT_ZERO = SSTS_INIT + 1,		/*	0�_�҂�							*/
//	SSTS_WEIT_OFF,						/*	2kg����							*/
//	SSTS_WEIT_ON,						/*	2kg�ȏ�							*/
//	SSTS_WEIT_HOLD,						/*	�z�[���h						*/
//	SSTS_WEIT_MAX,						/*	�ő�							*/
//weit02 del end

										/* �̑g������						*/
//pset01 chg start
//	SSTS_ISCAN_SEL_AGE = SSTS_INIT + 1,	/*	�N��I��						*/
//	SSTS_ISCAN_SEL_HEIT,				/*	�g���I��						*/
//	SSTS_ISCAN_SEL_AL,					/*	�������x�I��					*/
//	SSTS_ISCAN_ZERO_AGE,				/*	�N��O�_�҂�					*/
//	SSTS_ISCAN_ZERO_HEIT,				/*	�g���O�_�҂�					*/
//	SSTS_ISCAN_ZERO_AL,					/*	�������x�O�_�҂�				*/
//	SSTS_ISCAN_AGE,						/*	�N��							*/
//	SSTS_ISCAN_HEIT,					/*	�g��							*/
//	SSTS_ISCAN_AL,						/*	�������x						*/
	SSTS_ISCAN_ZERO = SSTS_INIT + 1,	/*	�O�_�҂�						*/
//pset01 chg end
//t026 add start
	SSTS_ISCAN_ERR_PNUM,				/*	�l���o�^�ʒm					*/
//t026 add end
//weit01 add start
	SSTS_ISCAN_MINOL,					/*	�}�C�i�X�n�k					*/
//weit01 add end
	SSTS_ISCAN_OFF,						/*	�̏d����2kg����					*/
	SSTS_ISCAN_ON,						/*	�̏d����2kg�ȏ�					*/
	SSTS_ISCAN_IMP,						/*	�C���s							*/
//imp03 add start
	SSTS_ISCAN_IMPOPEN,					/*	�C���s�I�[�v��					*/
//imp03 add end
//step01 del start
//	SSTS_ISCAN_STPOFF_WAIT,				/*	�~��҂�						*/
//	SSTS_ISCAN_STPOFF_ZERO,				/*	�~��Ă���̂O�_�擾			*/
//	SSTS_ISCAN_STPOFF_DACOR,			/*	�~��Ă���̃f�[�^�␳			*/
//step01 del end
	SSTS_ISCAN_MAX,						/*	�ő�							*/

										/* ���荀��							*/
//item01 chg start
//	SSTS_ITEM_PNUMNONE = SSTS_INIT + 1,	/*	�l���Ȃ�					*/
//	SSTS_ITEM_SEL_AGE,					/*	�N��I��						*/
//	SSTS_ITEM_SEL_HEIT,					/*	�g���I��						*/
//	SSTS_ITEM_SEL_AL,					/*	�������x�I��					*/
//	SSTS_ITEM_AGE,						/*	�N��							*/
//	SSTS_ITEM_HEIT,						/*	�g��							*/
//	SSTS_ITEM_AL,						/*	�������x						*/
//	SSTS_ITEM_MYSUP,					/*	�}�C�T�|�\��					*/
//	SSTS_ITEM_NONSND,					/*	�����M�f�[�^��					*/
//	SSTS_ITEM_ITEM,						/*	���荀��						*/
	SSTS_ITEM_STPOFF_WAIT = SSTS_INIT + 1,
										/*	�~��҂�						*/
	SSTS_ITEM_ITEM,						/*	���荀��						*/
	SSTS_ITEM_END,						/*	�I��							*/
//item01 chg end
	SSTS_ITEM_MAX,						/*	�ő�							*/

//ble02 del start
//										/* �J�E���g���[�h					*/
//	SSTS_CNT_WT_ZERO = SSTS_INIT + 1,	/*	�׏d0�_							*/
//	SSTS_CNT_WT_WAIT,					/*	�׏d�҂�						*/
//	SSTS_CNT_WT,						/*	�׏d							*/
//	SSTS_CNT_TEMP,						/*	���x							*/
//	SSTS_CNT_Z,							/*	�y								*/
//	SSTS_CNT_R,							/*	�q								*/
//	SSTS_CNT_X,							/*	�w								*/
/* step01 del start															*/
/*	SSTS_CNT_STPON,						 *	�X�e�b�vON						*/
/* step01 del end															*/
//	SSTS_CNT_MAX,						/*	�ő�							*/
//
//										/* �������[�h						*/
//	SSTS_ADJ_DP_ALL = SSTS_INIT + 1,	/*	�S�\��							*/
//	SSTS_ADJ_SPECB,						/*	��{�d�l						*/
//	SSTS_ADJ_SPECEX,					/*	�g���d�l						*/
//	SSTS_ADJ_ZERO_START,				/*	0�_�|�[�g�ݒ�J�n				*/
//
//	SSTS_ADJ_ZERO_PORT,					/*	0�_�|�[�g�ݒ�					*/
//	SSTS_ADJ_ZERO_WAIT,					/*	0�_�҂�							*/
//	SSTS_ADJ_ZERO_FIX,					/*	0�_�m��							*/
//	SSTS_ADJ_MIN_FIX,					/*	�ŏ��lg�m��						*/
//	SSTS_ADJ_HALF_FIX,					/*	���Ԓl�m��						*/
//	SSTS_ADJ_ALLWT_FIX,					/*	���ʊm��						*/
//
//	SSTS_ADJ_IMP_START,					/*	��R�␳�J�n					*/
//	SSTS_ADJ_IMP800,					/*	��R�␳800���҂�				*/
//	SSTS_ADJ_IMP200,					/*	��R�␳200���҂�				*/
//
//	SSTS_ADJ_MAX,						/*	�ő�							*/
//ble02 del end

										/* ���������[�h�i�����������[�h�j	*/
//ble02 chg start
//	SSTS_MEM_DSP_DAT = SSTS_INIT + 1,	/*	�f�[�^�\��						*/
//	SSTS_MEM_DSP_ADR,					/*	�A�h���X�\��					*/
//	SSTS_MEM_EDIT,						/*	�ҏW							*/
//	SSTS_MEM_WRI1,						/*	����1							*/
//	SSTS_MEM_WRI2,						/*	����2							*/
//	SSTS_MEM_WRI3,						/*	����3							*/
	SSTS_MEM_START = SSTS_INIT + 1,		/*	�����������[�h�J�n				*/
	SSTS_MEM_SOFTINF,					/*	�@��\�t�g�E�F�A���擾�҂�	*/
//ble02 chg end
	SSTS_MEM_MAX,						/*	�ő�							*/

										/* BLE�ʐM�������[�h				*/
/* ble02 chg start															*/
/*	SSTS_BLETEST_SEL_PLACE = SSTS_INIT + 1,									*/
/*										 *	�����ꏊ�I��					*/
/*	SSTS_BLETEST_SEL_LNUM,				 *	���C���ԍ��I��					*/
/*	SSTS_BLETEST_SOFTINF,				 *	�@��\�t�g�E�F�A���擾		*/
/*	SSTS_BLETEST_ACK,					 *	acknowledge�ʒm					*/
/*	SSTS_BLETEST_DISCONECT,				 *	�ؒf							*/
/*	SSTS_BLETEST_RESULT,				 *	���ʕ\��						*/
/*	SSTS_BLETEST_PSINF,					 *	�l���						*/
/*	SSTS_BLETEST_MAX,					 *	�ő�							*/
//
//										/* �C���s�������[�h					*/
//	SSTS_IMPCHK_800 = SSTS_INIT + 1,	/*	800���̑���						*/
//	SSTS_IMPCHK_200,					/*  200���̑���						*/
//	SSTS_IMPCHK_WAIT_200,				/*  ���������H�֑ؑ҂�			*/
//	SSTS_IMPCHK_END,					/*	�`�F�b�N�I��					*/
//	SSTS_IMPCHK_MAX,					/*	�ő�							*/

	SSTS_BLE_ADJ_WT = SSTS_INIT + 1,	/*	�퍷�������[�h			0x01	*/
	SSTS_BLE_ADJ_IMP,					/*	�C���s�������[�h		0x02	*/
	SSTS_BLE_MEM,						/*	���������[�h			0x03	*/
	SSTS_BLE_CNT,						/*	�J�E���g���[�h			0x04	*/
	SSTS_BLE_DSP,						/*	�\���ڊm�F���[�h		0x05	*/
	SSTS_BLE_TEST_PCB,					/*	��������[�h			0x06	*/
	SSTS_BLE_TEST_IMP,					/*	�C���s�������[�h		0x07	*/
	SSTS_BLE_TEST_VOICE,				/*	�����m�F���[�h			0x08	*/
	SSTS_BLE_START,						/*	�����E�������[�h�J�n			*/
	SSTS_BLE_SOFTINF,					/*	�@��\�t�g�E�F�A���擾�҂�	*/
	SSTS_BLE_MAIN,						/*	���C���X�^���o�C���[�h			*/
//test02 add start
	SSTS_BLE_TEST_LED,					/*	LED�������[�h					*/
//test02 add end
										/*  ����������LED�쓮100%			*/
	SSTS_BLE_ADJ_ZERO_PORT,				/*	0�_�|�[�g�ݒ�					*/
	SSTS_BLE_ADJ_ZERO_WAIT,				/*	0�_�҂�							*/
	SSTS_BLE_ADJ_ZERO_FIX,				/*	0�_�m��							*/
	SSTS_BLE_ADJ_MIN_FIX,				/*	�ŏ��lg�m��						*/
	SSTS_BLE_ADJ_HALF_FIX,				/*	���Ԓl�m��						*/
	SSTS_BLE_ADJ_ALLWT_FIX,				/*	���ʊm��						*/
	SSTS_BLE_ADJ_IMP800,				/*	��R�␳800���҂�				*/
	SSTS_BLE_ADJ_IMP200,				/*	��R�␳200���҂�				*/
	SSTS_BLE_IMPCHK_800,				/*	800���̑���						*/
	SSTS_BLE_IMPCHK_200,				/*  200���̑���						*/
	SSTS_BLE_PCB_ZERO,					/*	�����	0�_�`�F�b�N			*/
	SSTS_BLE_PCB_IMP,					/*	�����	�C���s�`�F�b�N		*/
										/*  �������܂�						*/

	SSTS_BLE_PCB_KEY,					/*	�����	�L�[�`�F�b�N		*/

	SSTS_BLETEST_MAX,					/*	�ő�							*/
/* ble02 chg end															*/

										/* �e�X�g���[�h						*/
//ble02 chg start
//	SSTS_TEST_ZERO = SSTS_INIT + 1,		/*  0�_�`�F�b�N						*/
//	SSTS_TEST_IMP,						/*  �C���s�`�F�b�N					*/
//	SSTS_TEST_BLE_SOFTINF,				/*	BLE����(�@���ĳ�����擾)		*/
//	SSTS_TEST_BLE_ACK,					/*	BLE����(acknowledge�ʒm)		*/
//	SSTS_TEST_BLE_DISCONECT,			/*	BLE����(�ؒf)					*/
//	SSTS_TEST_EXE_WAIT,					/*  (�L�[�`�F�b�N)���s�҂�			*/
//	SSTS_TEST_EXE,						/*	(�L�[�`�F�b�N)���s				*/
	SSTS_TEST_EXE = SSTS_INIT + 1,		/*	(�L�[�`�F�b�N)���s				*/
//ble02 chg end
	SSTS_TEST_MAX						/*	�ő�							*/
};

enum	EVT_ID							/* �����݃C�x���g					*/
{										/*					�v���C�I���e�B��*/
	EVT_POFF,							/*	�d��OFF					��	0	*/
	EVT_FINIT,							/*	�t�@���N�V��������		��	1	*/
	EVT_TMR0,							/*	�^�C�}0����				��	2	*/
	EVT_TMR1,							/*	�^�C�}1����				��	3	*/
	EVT_TMR2,							/*	�^�C�}2����				��	4	*/
	EVT_TMR3,							/*	�^�C�}3����				��	5	*/
	EVT_TMR4,							/*	�^�C�}4����				��	6	*/
	EVT_TMR5,							/*	�^�C�}5����				��	7	*/
	EVT_TMRCLK,							/*	���v�^�C�}����			��	8	*/
	EVT_VOLT,							/*	�d��					��	9	*/
	EVT_LOBAT = EVT_VOLT,				/*	��d��					�V	9 	*/
	EVT_TEMP,							/*	���x					��	10	*/
	EVT_IMP,							/*	�C���s					��	11	*/
	EVT_WEIT,							/*	�׏d					��	12	*/
//step01 del start
//	EVT_ZEROTMR,						/*	0�_�^�C�}�v��			��	13	*/
//	EVT_ZEROTEMP,						/*	0�_���x�v��				��	14	*/
//	EVT_STPTMR,							/*	�X�e�b�v�^�C�}�v��		��	15	*/
//	EVT_STPWEIT,						/*	�X�e�b�v�׏d			��	16	*/
//step01 del end
/* tmphmd02 add start														*/
	EVT_TMPHMDTMR,						/*	�����x�^�C�}			��	13	*/
/* tmphmd02 add end															*/
	EVT_BLESTART,						/*	BLE�ʐM�J�n				��	14	*/
	EVT_BLEEND,							/*	BLE�ʐM�I��				��	15	*/
	EVT_BLEERR,							/*	BLE�ʐM�G���[			��	16	*/
	EVT_BLERCV,							/*	BLE��M����				��	17	*/
	EVT_KEY,							/*	�L�[					��	18	*/
	EVT_MAX,							/*	�����݃C�x���g����			19	*/

/* key01 chg start															*/
/*	EVT_KEY_WEIT_OFF = EVT_KEY,			 *	�̏d/OFF�L�[OFF			��	21	*/
/*	EVT_KEY_UP_OFF,						 *	UP�L�[OFF				��	22	*/
/*	EVT_KEY_DOWN_OFF,					 *	DOWN�L�[OFF				��	23	*/
/*	EVT_KEY_SET_OFF,					 *	SET�L�[OFF				��	24	*/
/*																			*/
/*	EVT_KEY_WEIT,						 *	�̏d/OFF�L�[			��	25	*/
/*	EVT_KEY_UP,							 *	UP�L�[					��	26	*/
/*	EVT_KEY_DOWN,						 *	DOWN�L�[				��	27	*/
/*	EVT_KEY_SET,						 *	SET�L�[					��	28	*/
	EVT_KEY_SW1_OFF = EVT_KEY,			/*	�l1�L�[OFF			��	18	*/
	EVT_KEY_SW2_OFF,					/*	�l2�L�[OFF			��	19	*/
	EVT_KEY_SW3_OFF,					/*	�l3�L�[OFF			��	20	*/
	EVT_KEY_SW4_OFF,					/*	�l4�L�[OFF			��	21	*/
	EVT_KEY_PAIR_OFF,					/*	�y�A�����O�L�[OFF		��	22	*/
	EVT_KEY_VOICE_OFF,					/*	�������ʃL�[OFF			��	23	*/
	EVT_KEY_BUZZ_OFF,					/*	�u�U�[���ʃL�[OFF		��	24	*/

	EVT_KEY_SW1,						/*	�l1�L�[				��	25	*/
	EVT_KEY_SW2,						/*	�l2�L�[				��	26	*/
	EVT_KEY_SW3,						/*	�l3�L�[				��	27	*/
	EVT_KEY_SW4,						/*	�l4�L�[				��	28	*/
	EVT_KEY_PAIR,						/*	�y�A�����O�L�[			��	29	*/
	EVT_KEY_VOICE,						/*	�������ʃL�[			��	30	*/
	EVT_KEY_BUZZ,						/*	�u�U�[���ʃL�[			��	31	*/
/* key01 chg end															*/

	EVT_NOP								/*	�����C�x���g	�v���C�I���e�B��*/
};

