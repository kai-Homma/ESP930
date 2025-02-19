/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	sub.h											*/
/*																			*/
/*		���W���[������	�F	���ʊ֐��w�b�_									*/
/*																			*/
/*		�쐬�N����		�F	2022/ 2/25	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 2/25	���R		rev01					*/
/*								��A���\�ύX�iB41XDJ��B42(B)J�j�؎��_���ǉ�	*/
/*							2022/ 2/25	���R		key01					*/
/*								�L�[�z�u�ύX								*/
/*							2022/ 2/25	���R		pset01					*/
/*								�l�ݒ胂�[�h�폜							*/
/*							2022/ 4/19	���R		lcd01					*/
/*								LCD�폜										*/
/*							2022/ 5/ 9	���R		item01					*/
/*								���荀�ڕύX								*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*		�萔��`															*/
/****************************************************************************/
										/* �J�E���^����						*/
#define	CNT_UP				(uchar)1	/*		�J�E���g�A�b�v				*/
#define	CNT_DOWN			(uchar)0xFF	/*		�J�E���g�_�E��				*/


/****************************************************************************/
/*		�e�[�u����`														*/
/****************************************************************************/
struct	HTROUND_TBL_T					/* �g���ۂ߃e�[�u��					*/
{
	ushort	usMAX;						/*	�ő�l							*/
	ushort	usROUND[DUAL];				/*	�ۂߒl							*/
};


/****************************************************************************/
/*		�O���錾															*/
/****************************************************************************/
extern	uchar	FtoS(short *, float);	/* float��short�ϊ�					*/
extern	char	ConvAbs(ushort*);		/* ��Βl�ϊ�						*/
extern	char	ConvAbsCast(ulong *, long);
										/* ��Βl�ϊ��L���X�g				*/
extern	ushort	DivRoundS(ushort, ushort);
										/* 2�o�C�g�f�[�^���Z(�l�̌ܓ�)		*/
extern	ulong	DivRoundL(ulong, ulong);/* 4�o�C�g�f�[�^���Z(�l�̌ܓ�)		*/
extern	long	DivRoundSL(long, long);	/* �����t��4�o�C�g���Z(�l�̌ܓ�)	*/
extern	void	ImAdd(float *, float *, float, float);
										/* ���f�����Z						*/
extern	void	ImMul(float *, float *, float, float);
										/* ���f����Z						*/
extern	void	ImDiv(float *, float *, float, float);
										/* ���f�����Z						*/
extern	ushort	RoundS(ushort, ushort);	/* 2�o�C�g�f�[�^�ۂߏ���			*/
extern	ushort	RoundHt(ushort);		/* �g���ۂ�							*/
extern	ushort	RoundWt(ushort, uchar);	/* �׏d�ۂ�							*/
extern	ushort	RoundFtoS(float, uchar);/* float��ushort�ۂ�				*/
extern	uchar	StoreData(ulong, ulong *, ulong *);
										/* �J�E���g�f�[�^�~��				*/
extern	void	StoreDataSL(long, long *, long *);
										/* long�^�J�E���g�f�[�^�~��			*/
extern	ulong	SumData(uchar, ulong *);/* �J�E���g�f�[�^�ώZ				*/
extern	long	SumDataSL(uchar, long *);
										/* long�^�J�E���g�f�[�^�ώZ			*/
extern	ulong	AvgData(uchar);			/* �J�E���g�f�[�^����				*/
extern	long	AvgDataSL(uchar, uchar, ushort, long *);
										/* long�^�J�E���g�f�[�^����			*/
extern	uchar	GetChkSum(void *, uchar);
										/* �`�F�b�N�T���擾					*/
//lcd01 del start
//extern	void	HexToDec(uchar, char *, uchar, uchar);
//										/* 16�i��10�i�ϊ�(uchar)			*/
//extern	void	HexToDecS(ushort, char *, uchar, uchar);
//										/* 16�i��10�i�ϊ�(ushort)			*/
//extern	void	HexToDecL(ulong, char *, uchar, uchar);
//										/* 16�i��10�i�ϊ�(ulong)			*/
//extern	void	HexToAscS(ushort, char *, uchar);
//										/* 16�i�˃A�X�L�[�ϊ�(ushort)		*/
//extern	void	HexToAsc(uchar *, char *, uchar);
//										/* 16�i�˃A�X�L�[�ϊ�(uchar)		*/
//lcd01 del end
extern	void	Conv8BitTo4Bit(uchar *, uchar *, uchar);
										/* 8�r�b�g��4�r�b�g					*/
extern	void	Conv4BitTo8Bit(uchar *, uchar *);
										/* 4�r�b�g4�o�C�g��8�r�b�g2�o�C�g	*/
extern	ushort	CalSub(ushort, ushort);	/* ���v�Z�iushort�^�f�[�^�j			*/
extern	ulong	CalSubL(ulong, ulong);	/* ���v�Z�iulong�^�f�[�^�j			*/
extern	ulong	RoundFtoL(float, uchar);/* float��ulong�ۂ�					*/
extern	float	ConvfAbs(float);		/* ��Βl�ϊ�(float)				*/
extern	void	StsChg(enum MSTS_ID);	/* ��ԑJ��							*/
extern	uchar	StsChgTerm(enum MSTS_ID, uchar, uchar);
										/* �����t����ԑJ��					*/
extern	void	StsChgClr(enum MSTS_ID, uchar, uchar, uchar);
										/* ��ԑJ�ځ�������					*/
extern	void	StsChgErr(char);		/* �G���[��ԑJ��					*/
extern	void	ChkSysinf(void);		/* �V�X�e�����`�F�b�N				*/
extern	void	SetSpid(const void *);	/* �d�l�����̈�ݒ�					*/

extern	uchar	GetArea(void);			/* �n��擾							*/
extern	uchar	ChkAscCode(char *, uchar);
										/* �A�X�L�[�R�[�h�`�F�b�N			*/
extern	void	ChkPsinf(void);			/* �l���`�F�b�N					*/
//pset01 del start
//extern	void	ChkPsinfChg(void);		/* �l���ύX�`�F�b�N				*/
//pset01 del end
// extern	uchar	ChkPsDat(struct PSMEM_DAT_T *);
// 										/* ����f�[�^�`�F�b�N				*/
extern	uchar	ChkPs(void);			/* �l���ȈՃ`�F�b�N				*/
extern	uchar	ChkPsId(uchar);			/* �l�h�c�`�F�b�N					*/
extern	uchar	ChkAge(uchar);			/* �N��`�F�b�N						*/
extern	uchar	GetAgeMin(void);		/* �N��ŏ��l�擾					*/
extern	uchar	GetAgeMinMslBone(void);	/* �ؓ���/���ʔN��ŏ��l�擾		*/
extern	uchar	ChkAgeAdult(uchar);		/* ���N�`�F�b�N						*/
extern	uchar	GetHtNotch(enum COUNTRY_ID);
										/* �g���ڗʎ擾						*/
extern	ushort	ChkHt(ushort);			/* �g���`�F�b�N						*/
extern	ushort	GetHtMin(enum UNIT_ID);	/* �g���ŏ��l�擾					*/
extern	ushort	GetHtMax(enum UNIT_ID);	/* �g���ő�l�擾					*/
extern	ushort	GetHtDef(enum UNIT_ID);	/* �g���f�t�H���g�擾				*/
//spec01 del start
//extern	uchar	ChkAlEnable(uchar, uchar);
//										/* �������x�L���`�F�b�N				*/
//spec01 del end
extern	uchar	ChkAl(enum AL_ID, uchar, uchar);
										/* �������x�`�F�b�N					*/
extern	enum AL_ID	GetAlDef(uchar);	/* �������x�f�t�H���g�擾			*/
extern	uchar	ChkWt(ushort);			/* �̏d�`�F�b�N						*/
extern	ushort	GetWtMax(void);			/* �̏d�ő�l�擾					*/
// extern	void	SetPsAgeLimit(struct YMD_T *, uchar *, enum AL_ID *, uchar);
// 										/* �l���N����ݒ�				*/
extern	void	SetPsinfDef(uchar);		/* �l���f�t�H���g�ݒ�			*/
extern	void	SetPsinfInit(void);		/* �l��񏉊���					*/
extern	void	SetPsdatInit(void);		/* �l����f�[�^������				*/
//pset01 del start
//extern	void	PsinfToPstmp(void);		/* �l���ˈꎞ�̈�				*/
//extern	void	PstmpToPsinf(void);		/* �ꎞ�̈�ˌl���				*/
//pset01 del end
//key01 del start
//extern	void	CountUD(uchar);			/* �J�E���^����						*/
//key01 del end
// extern	void	LoadPsDat(enum PSTIME_ID);
// 										/* ����f�[�^���[�h					*/
// extern	uchar	SavePsDat(enum PSTIME_ID);
// 										/* ����f�[�^�Z�[�u					*/
extern	void	SavePsRev(void);		/* ��A���f�[�^�Z�[�u				*/
//pset01 del start
//extern	void	SetLastPnum(void);		/* �ŏI�ďo�l�ԍ��ݒ�				*/
//pset01 del end
extern	void	GetPsinf(void);			/* �l���擾						*/

//item01 del start
//extern	void	SetPsdatThis(void);		/* ���񑪒�f�[�^�ݒ�				*/
//item01 del end
/* rev01 add start															*/
//t020 del start
//extern	uchar	GetPsLastNum(void);		/* ����f�[�^�����擾				*/
//t020 del end
extern	void	ShiftPsDay(ushort, ushort, uchar);
										/* �l���f�[�^�V�t�g�i�؎��p�j		*/
//extern	void	ShiftPsWtDay(ushort, ushort, ushort);
//										/* �l���f�[�^�V�t�g�i�̏d�}���p�j	*/
/* rev01 add end															*/
extern	void	SetAdjLog(uchar);		/* �������O�ݒ�						*/

extern	ushort	s_sqrt(ulong);			/*		�������v�Z					*/



