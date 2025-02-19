/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	imp.h											*/
/*																			*/
/*		���W���[������	�F	�C���s�[�_���X�w�b�_							*/
/*																			*/
/*		�쐬�N����		�F	2022/ 2/24	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 2/24	���R		imp01					*/
/*								�C���s����ύX�i�V���O���˃f���A�����g���j	*/
/*							2022/ 2/24	���R		imp02					*/
/*								�@��Ԉʑ��␳�W���ύX						*/
/*							2022/ 2/24	���R		imp03					*/
/*								�C���s�I�[�v������ύX						*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

//�ǉ���
#define	ADC_IMP_5		(uchar)3		/*	3	�C���s5kHz					*/
#define	ADC_IMP			(uchar)0x11		/*	5	�C���s						*/


/****************************************************************************/
/*		�萔��`															*/
/****************************************************************************/
#define	ADC_IMP_STORE_MAX	(uchar)8	/* �C���s�~�ω񐔍ő�				*/
#define	ADC_IMP_ROUND		(ushort)16	/* �C���s�J�E���g�ۂߒl(4096����1)	*/

#define IMP_DATA_NUM		(uchar)50	/*	�C���s���f�[�^��				*/ //50�ɕύX

#define MAX_IMP_DIZZER		(uchar)27	/* �Έړd��	�Έڗʍő�				*/
										/*	1/9 LSB * 27 taps = 3 LSB		*/
#define	DIZZER_CNT_INIT		(uchar)0	/*			�����l					*/
//imp01 chg start
//#define	DIZZER_STEP			(uchar)2	/*			�Έڗ�					*/
#define	DIZZER_STEP_50		(uchar)2	/*			�Έڗ�	50kHz�p			*/
#define	DIZZER_STEP_5		(uchar)12	/*			�Έڗ�	5kHz�p			*/
//imp01 chg end

										/*	�c�e�s�f�[�^�I�t�Z�b�g��		*/
//imp01 chg start
//#define DFT_DATA_OFFSET_50	(uchar)13	/*			������g���F50kHz		*/
//#define DFT_DATA_OFFSET_5	(uchar)11	/*			������g���F5kHz		*/
#define DFT_DATA_OFFSET_50	(uchar)1	/*			������g���F50kHz		*/ //1�ɕύX
#define DFT_DATA_OFFSET_5	(uchar)3	/*			������g���F5kHz		*/
//imp01 chg end

#define	IMP_FREQ_50KHZ	(uchar)0		/* �C���s������g��	50kHz			*/
#define	IMP_FREQ_5KHZ	(uchar)1		/*					5kHz			*/
#define	IMP_FREQ_MAX	(uchar)2		/* ���g��	�ő�					*/

#define	IMP_OBJ_IN		(uchar)0		/* �Ώ�		����					*/
#define	IMP_OBJ_OUT		(uchar)1		/*			�O��					*/
#define	IMP_OBJ_MAX		(uchar)2		/*			�ő�					*/

#define	IMP_BASE_H		(uchar)0		/* �		����R					*/
#define	IMP_BASE_L		(uchar)1		/*			���R					*/
#define	IMP_BASE_MAX	(uchar)2		/*			�ő�					*/

										/* �ϊ��W��							*/
										/*		�������p					*/
#define	IMPCOEF_CR_INIT	(ushort)32768	/*		�b�q	 1*8000H			*/
#define	IMPCOEF_CX_INIT	(short)0		/*		�b�w	 0*8000H			*/

//imp02 chg start
//#define	IMPCOEF_CR_DEF	(ushort)32522	/* 50kHz�b�q	 0.992495373*32768	*/
//#define	IMPCOEF_CX_DEF	(short)178		/*		�b�w	 0.005425553*32768	*/
//#define	IMPCOEF_CR_DEF_5	(ushort)32768
//										/*  5kHz�b�q	 1*8000H			*/
//#define	IMPCOEF_CX_DEF_5	(short)0	/*		�b�w	 0*8000H			*/
//kkkk �ʎY�����i�Őݒ肷��K�v����
//#define	IMPCOEF_CR_DEF	(ushort)32768	/* 50kHz�b�q	 0.992495373*32768	*/
//#define	IMPCOEF_CX_DEF	(short)0		/*		�b�w	 0.005425553*32768	*/
//#define	IMPCOEF_CR_DEF_5	(ushort)32768
//										/*  5kHz�b�q	 1*8000H			*/
//#define	IMPCOEF_CX_DEF_5	(short)0	/*		�b�w	 0*8000H			*/
#define	IMPCOEF_CR_DEF	(ushort)32771	/* 50kHz�b�q	 1*8000H			*/     //RD930���p
#define	IMPCOEF_CX_DEF	(short)-52		/*		�b�w	 0*8000H			*/     //RD930���p
#define	IMPCOEF_CR_DEF_5	(ushort)32723
										/*  5kHz�b�q	 1*8000H			*/     //RD930���p
#define	IMPCOEF_CX_DEF_5	(short)-33	/*		�b�w	 0*8000H			*/     //RD930���p
//kkkk
//imp02 chg end
#define	IMPCOEF_CR_MIN	(ushort)26214	/*		�b�q�ŏ� 					*/
#define	IMPCOEF_CR_MAX	(ushort)39321	/*		�b�q�ő� 					*/
#define	IMPCOEF_CX_MIN	(short)-6554	/*		�b�w�ŏ� 					*/
#define	IMPCOEF_CX_MAX	(short)6554		/*		�b�w�ő� 					*/
#define	IMPCOEF_MIN		(uchar)0		/*		�ŏ���						*/
#define	IMPCOEF_MAX		(uchar)2		/*		�ő吔						*/

#define	IMP_AVG			(long)(120 - 1)	/* 		���ϔ͈�(�}12������)		*/
#define	IMP_STB			(long)(10 - 1)	/* 		����͈�(�}1������)			*/
#define	IMP_REF			(long)8200		/* 		�����Q�ƒ�R(820��)			*/

//imp03 allchg start
										/* �C���s�I�[�v������				*/
//kkkk ���̒l�Ȃ̂ŕύX�̕K�v����
//#define	IMP_IN_OPEN_MIN		(float)+2.1E+9F
//										/* �C���s�I�[�v������ �ŏ��l 400��	*/
//#define	IMP_IN_OPEN_MAX		(float)+1.9E+10F
//										/* 					  �ő�l1.2k��	*/
//										/*   820��ref:+8.9E+9F(50kHz)		*/
//#define	IMP_OUT_OPEN_MIN	(float)+3.1E+7F
//										/* 		�I�[�v���O��  �ŏ��l50��	*/
//#define	IMP_OUT_OPEN_MAX	(float)+3.2E+10F
//										/* 					  �ő�l1.6k��	*/
//										/*   820��ref:+8.5E+9F(50kHz)		*/
//
//#define	IMP_IN_OPEN_MIN_5	(float)+9.1E+7F
//										/* �C���s�I�[�v������ �ŏ��l 400��	*/
//#define	IMP_IN_OPEN_MAX_5	(float)+8.5E+8F
//										/* 					  �ő�l1.2k��	*/
//										/*   820��ref:+8.9E+9F(5kHz)		*/
//#define	IMP_OUT_OPEN_MIN_5	(float)+1.2E+6F
//										/* 		�I�[�v���O��  �ŏ��l50��	*/
//#define	IMP_OUT_OPEN_MAX_5	(float)+1.6E+9F
//										/* 					  �ő�l1.6k��	*/
//										/*   820��ref:+8.5E+9F(5kHz)		*/
//kkkk
										/* ��FR-200L �^����ɂČv��		*/
#define	IMP_IN_OPEN_MIN		(float)+2.1E+9F
										/* �C���s�I�[�v������ �ŏ��l 400��	*/
#define	IMP_IN_OPEN_MAX		(float)+2.0E+10F
										/* 					  �ő�l1.2k��	*/
										/*   820��ref:+8.9E+9F(50kHz)		*/
#define	IMP_OUT_OPEN_MIN	(float)+3.2E+7F
										/* 		�I�[�v���O��  �ŏ��l50��	*/
#define	IMP_OUT_OPEN_MAX	(float)+3.4E+10F
										/* 					  �ő�l1.6k��	*/
										/*   820��ref:+8.8E+9F(50kHz)		*/

#define	IMP_IN_OPEN_MIN_5	(float)+9.6E+7F
										/* �C���s�I�[�v������ �ŏ��l 400��	*/
#define	IMP_IN_OPEN_MAX_5	(float)+8.8E+8F
										/* 					  �ő�l1.2k��	*/
										/*   820��ref:+4.1E+8F(5kHz)		*/
#define	IMP_OUT_OPEN_MIN_5	(float)+1.4E+6F
										/* 		�I�[�v���O��  �ŏ��l50��	*/
#define	IMP_OUT_OPEN_MAX_5	(float)+1.6E+9F
										/* 					  �ő�l1.6k��	*/
										/*   820��ref:+4.1E+8F(5kHz)		*/
//imp03 allchg end

#define	IMP_ADJ_RANGE		(short)100	/* ���������R�͈�	10��		*/

#define	IMP_ADJ_800_MAX		(short)(8000 + IMP_ADJ_RANGE)
										/* 		���������R	800��	�ő�*/
#define	IMP_ADJ_800_MIN		(short)(8000 - IMP_ADJ_RANGE)
										/* 								�ŏ�*/
#define	IMP_ADJ_200_MAX		(short)(2000 + IMP_ADJ_RANGE)
										/* 						200��	�ő�*/
#define	IMP_ADJ_200_MIN		(short)(2000 - IMP_ADJ_RANGE)
										/* 								�ŏ�*/

#define	IMP_STB_MEAS		(uchar)7	/* ���莞���茟�o��				*/
#define	IMP_STB_MEAS_STPON	(uchar)3	/* �X�e�b�vON�����茟�o��			*/
#define	IMP_STB_TEST		(uchar)3	/* ���������茟�o��				*/
#define	IMP_STB_ADJ			(uchar)32	/* ���������茟�o��				*/
#define	IMP_CHGCNT			(uchar)16	/* �ؑ։�							*/
#define	IMP_WAIT_DSP		(uchar)160	/* �\���҂��񐔁i�X�e�b�vON�j		*/
//imp03 add start
#define	IMP_WAIT_OPEN		(uchar)16	/* �I�[�v���\���҂���				*/
//imp03 add end
#define	IMP_WAIT_MEAS		(uchar)64	/* ����҂���						*/
#define	IMP_WAIT_ADJ		(uchar)64	/* ��������҂���					*/
#define	IMP_WAIT_MAX		(uchar)255	/* ����҂��񐔍ő�					*/



/****************************************************************************/
/*		�e�[�u����`														*/
/****************************************************************************/
struct	IMP_T							/* �C���s���						*/
{
	long	lIM;						/*	�����f�[�^						*/
	long	lRE;						/*	�����f�[�^						*/
};

struct	IMP_CONV_T						/* �C���s�ϊ�						*/
{
	ushort	usCOEFA;					/*		�W���`						*/
	short	sCOEFB;						/*		�W���a(�Q�̕␔)			*/
};

struct	IMP_CALB_T						/* �C���s����						*/
{
	short	sCR;						/*		�b�q						*/
	short	sCX;						/*		�b�w						*/
	short	sOSR;						/*		�n�r�q						*/
	short	sOSX;						/*		�n�r�w						*/
};

struct	MEAS_IMP_T						/* �C���s���						*/
{
	ushort	usZDT;						/*	�C���s�[�_���X					*/
	short	sRDT;						/*	���W�X�^���X					*/
	short	sXDT;						/*	���A�N�^���X					*/
};




/****************************************************************************/
/*		�O���錾															*/
/****************************************************************************/
extern	void	InitImpConv(void);		/* �C���s�␳�W��������				*/
extern	uchar	ChkImpBase(uchar);		/* �C���s���R�͈̓`�F�b�N		*/
extern	void	SetImp(struct IMP_T *, struct IMP_T *);
										/* �C���s���ݒ�					*/
extern	enum EVT_ID	MeasImp(void);		/* �C���s����						*/
extern	void	ConvImp(void);			/* �C���s�␳						*/
extern	uchar	AdjImp(struct IMP_CALB_T *, struct IMP_T *, struct IMP_T *);
										/* �C���s����						*/
//ble02 del start
//extern	uchar	ChkConvImp(uchar);		/* �C���s�␳�L���`�F�b�N			*/
//ble02 del end
extern	ulong	GetImpZ(long, long);	/* �y�擾							*/


extern	void	s_memset(void*, uchar, size_t);

