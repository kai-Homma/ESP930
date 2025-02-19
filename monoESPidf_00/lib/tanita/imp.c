/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	imp.c											*/
/*																			*/
/*		���W���[������	�F	�C���s�[�_���X����								*/
/*																			*/
/*		�쐬�N����		�F	2022/ 2/24	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 2/24	���R		imp01					*/
/*								�C���s����ύX�i�V���O���˃f���A�����g���j	*/
/*							2022/ 4/ 4	���R		ble02					*/
/*								BLE�ʐM�Œ����E����							*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

#include	"at_inc.h"					/* �C���N���[�h�t�@�C��				*/


/****************************************************************************/
/*		�q�`�l																*/
/****************************************************************************/


/****************************************************************************/
/*		�q�n�l																*/
/****************************************************************************/
/* 2^32/(10bitadc*int�ώZ*�Έړd��*�T���v�����O��*�ړ�����*����) = �e�[�u��	*/
/* 2^32/(1023*64 *   4   *   13   *      20      *   4    * 2  ) =    7		*/

//imp01 chg start
//const_s CosTbl[IMP_DATA_NUM] = 			/* COS�e�[�u��						*/
//{
//	 5,
//	 4,
//	 2,
//	-1,
//	-3,
//	-5,
//	-6,
//	-7,
//	-7,
//	-6,
//	-5,
//	-4,
//	-2,
//	 1,
//	 3,
//	 5,
//	 6,
//	 7,
//	 7,
//	 6
//};
//
//const_s SinTbl[IMP_DATA_NUM] =			/* SIN�e�[�u��						*/
//{
//	 5,
//	 6,
//	 7,
//	 7,
//	 6,
//	 5,
//	 4,
//	 2,
//	-1,
//	-3,
//	-5,
//	-6,
//	-7,
//	-7,
//	-6,
//	-5,
//	-4,
//	-2,
//	 1,
//	 3
//};
//

// const_s CosTbl[IMP_DATA_NUM] = 			/* COS�e�[�u��						*/
// {
// 	 7,
// 	 7,
// 	 6,
// 	 5,
// 	 3,
// 	 1,
// 	-2,
// 	-4,
// 	-5,
// 	-6,
// 	-7,
// 	-7,
// 	-6,
// 	-5,
// 	-3,
// 	-1,
// 	 2,
// 	 4,
// 	 5,
// 	 6
// };

// const_s SinTbl[IMP_DATA_NUM] =			/* SIN�e�[�u��						*/
// {
// 	 1,
// 	-2,
// 	-4,
// 	-5,
// 	-6,
// 	-7,
// 	-7,
// 	-6,
// 	-5,
// 	-3,
// 	-1,
// 	 2,
// 	 4,
// 	 5,
// 	 6,
// 	 7,
// 	 7,
// 	 6,
// 	 5,
// 	 3
// };

//imp01 chg end

// const_s CosTbl[IMP_DATA_NUM] = {
// 	15,	15,	13,	11,	8,
// 	5,	1,	-3,	-6,	-10,
// 	-12, -14, -15, -15,	-14,
// 	-12, -10, -6, -3,	1,
// 	5,	8,	11,	13,	15
// };

// const_s SinTbl[IMP_DATA_NUM] = {
//     0, 4, 7, 10, 13, 14,
// 	15, 15, 14, 12, 9,
// 	6, 2, -2, -6, -9,
// 	-12, -14, -15, -15, -14,
// 	-13, -10, -7, -4
// };

const_s CosTbl[IMP_DATA_NUM] = {
	15,15,15,14,13,12,11,10,8,6,5,3,1,
	-1,-3,-5,-6,-8,-10,-11,-12,-13,-14,-15,-15,-15,
	-15,-15,-14,-13,-12,-11,-10,-8,-6,-5,-3,-1,
	1,3,5,6,8,10,11,12,13,14,15,15
};

const_s SinTbl[IMP_DATA_NUM] = {
    0,2,4,6,7,9,10,12,13,14,14,15,15,
	15,15,14,14,13,12,10,9,7,6,4,2,0,
	-2,-4,-6,-7,-9,-10,-12,-13,-14,-14,-15,-15,
	-15,-15,-14,-14,-13,-12,-10,-9,-7,-6,-4,-2
};

/****************************************************************************/
/*		�v���g�^�C�v�錾													*/
/****************************************************************************/
void	Dft(struct IMP_T *, ulong *);	/* �c�e�s							*/
void	ConvImpBase(struct MEAS_IMP_T *, struct IMP_T *, struct IMP_CALB_T *);
										/* �C���s�x�[�X�␳					*/
void	ConvImpZRX(struct MEAS_IMP_T *, struct MEAS_IMP_T *, ushort, short);
										/* �y�q�w�␳						*/
void	ChkImpStable(void);				/* �C���s����`�F�b�N				*/
float	ConvScalar(long, long);			/* �x�N�g���˃X�J���[�ϊ�			*/

void	s_memcpy(void *, const void *, size_t);
// void	s_memset(void *, uchar , size_t );


// /****************************************************************************/
// /*																			*/
// /*		�֐���			�F	InitImpConv										*/
// /*																			*/
// /*		�֐�����		�F	�C���s�␳�W��������							*/
// /*																			*/
// /*		�쐬�N����		�F	2009/ 5/14	���R								*/
// /*																			*/
// /*		�P�̎�������	�F	2019/12/12	���R								*/
// /*																			*/
// /****************************************************************************/
// void	InitImpConv(void)
// {
// 	s_memcpy(SYS.BLK.ucCIMP, SYS_INIT.BLK.ucCIMP, sizeof(SYS.BLK.ucCIMP));
// 										/* �C���s�������������̈揉����		*/
// 	SYS.INF.ucIMPCHKSUM = (uchar)(GetChkSum((uchar *)&SYS.INF.IMPCALB,
// 							(uchar)sizeof(SYS.INF.IMPCALB)) ^ (uchar)0xFF);
// 										/* �`�F�b�N�T���ُ�ݒ�				*/
// //imp01 add start
// 	SYS.INF.ucIMPCHKSUM_5 =
// 				(uchar)(GetChkSum((uchar *)&SYS.INF.IMPCALB_5,
// 					(uchar)sizeof(SYS.INF.IMPCALB_5)) ^ (uchar)0xFF);
// 										/* �`�F�b�N�T���ُ�ݒ�				*/
// //imp01 add end
// }
// 

/****************************************************************************/
/*																			*/
/*		�֐���			�F	ChkImpBase										*/
/*																			*/
/*		�֐�����		�F	�C���s���R�͈̓`�F�b�N						*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2022/ 7/27	���R								*/
/*																			*/
/****************************************************************************/
uchar	ChkImpBase(uchar ucBase)
{
										/* ucBase	:���R				*/

//imp01 chg start
//	short	sRdt;						/* ���W�X�^���X						*/
//
//	ConvImp();							/* �C���s�␳						*/
//	sRdt = MEAS.IMPSRC[IMP_FREQ_50KHZ].sRDT;
//										/* ���W�X�^���X�ݒ�					*/
	ushort	usZdt;						/* �C���s�[�_���X					*/


	ConvImp();							/* �C���s�␳						*/

	if(HSRAM.ADC.ucSTS == ADC_IMP)		/* 50kHz�̏ꍇ						*/
	{
		usZdt = MEAS.IMPSRC[IMP_FREQ_50KHZ].usZDT;
										/* �C���s�[�_���X�ݒ�				*/
	}
	else								/* 5kHz�̏ꍇ						*/
	{
		usZdt = MEAS.IMPSRC[IMP_FREQ_5KHZ].usZDT;
										/* �C���s�[�_���X�ݒ�				*/
	}
//imp01 chg end

	if(ucBase == IMP_BASE_H)
	{
//imp01 chg start
//		if((sRdt > IMP_ADJ_800_MIN) && (sRdt < IMP_ADJ_800_MAX))
//		{								/* ���R800���}100�������̏ꍇ	*/
		if((IMP_ADJ_800_MIN <= usZdt) && (usZdt <= IMP_ADJ_800_MAX))
		{								/* ���R800���͈͈̔ȓ��ꍇ		*/
//imp01 chg end
			return(OK);					/* �n�j								*/
		}
	}
	else
	{
//imp01 chg start
//		if((sRdt > IMP_ADJ_200_MIN) && (sRdt < IMP_ADJ_200_MAX))
//		{								/* ���R200���}100�������̏ꍇ	*/
		if((IMP_ADJ_200_MIN <= usZdt) && (usZdt <= IMP_ADJ_200_MAX))
		{								/* ���R200���͈͈̔ȓ��ꍇ		*/
//imp01 chg end
			return(OK);					/* �n�j								*/
		}
	}

	return(NG);							/* �ُ�͈͂̏ꍇ�͂m�f				*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	SetImp											*/
/*																			*/
/*		�֐�����		�F	�C���s���ݒ�									*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/****************************************************************************/
void	SetImp(struct IMP_T *pOut, struct IMP_T *pIn)
{
										/* pOut	:�o�͏��					*/
										/* pIn	:���͏��					*/

	(pOut + IMP_OBJ_IN)->lRE = (pIn + IMP_OBJ_IN)->lRE;
	(pOut + IMP_OBJ_IN)->lIM = (pIn + IMP_OBJ_IN)->lIM;
	(pOut + IMP_OBJ_OUT)->lRE = (pIn + IMP_OBJ_OUT)->lRE;
	(pOut + IMP_OBJ_OUT)->lIM = (pIn + IMP_OBJ_OUT)->lIM;
										/* �C���s���ݒ�					*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	MeasImp											*/
/*																			*/
/*		�֐�����		�F	�C���s����										*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2022/ 7/27	���R								*/
/*																			*/
/****************************************************************************/
enum EVT_ID	MeasImp(void)
{
	struct	IMP_T	Imp;				/* �C���s�f�[�^						*/
	struct	IMP_T	*pImp;				/* �C���s�|�C���^					*/

// //imp01 chg start
// //	if(ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP))
// 	if(((HSRAM.ADC.ucSTS == ADC_IMP)
// 		&& (ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP_50)))
// 		|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 		&& (ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP_5))))
// //imp01 chg end
// 	{									/* �Έړd���ώZ�����̏ꍇ			*/
	pImp = &ADC.IMP.THIS[MEAS.ucOBJ];
									/* �C���s�|�C���^�擾				*/
	Dft(&Imp, ADC.IMP.SUM.ulDATA);	/* �c�e�s							*/
	StoreDataSL(Imp.lRE, &ADC.IMP.STORE.lRE[0],
						&ADC.IMP.STORE.lRE[ADC_IMP_STORE_MAX - 1]);
									/* �C���s�����~��					*/
	pImp->lRE = AvgDataSL(MEAS.ucAVGCNT, ADC_IMP_STORE_MAX,
						ADC_IMP_ROUND, &ADC.IMP.STORE.lRE[0]);
									/* �C���s��������					*/
	StoreDataSL(Imp.lIM, &ADC.IMP.STORE.lIM[0],
						&ADC.IMP.STORE.lIM[ADC_IMP_STORE_MAX - 1]);
									/* �C���s�����~��					*/
	pImp->lIM = AvgDataSL(MEAS.ucAVGCNT, ADC_IMP_STORE_MAX,
						ADC_IMP_ROUND, &ADC.IMP.STORE.lIM[0]);
									/* �C���s��������					*/
	ChkImpStable();					/* �C���s����`�F�b�N				*/

	s_memset(&ADC.IMP.SUM.ulDATA, (uchar)0,
									sizeof(ADC.IMP.SUM.ulDATA));
	// 								/* �C���s�ώZ��񏉊���				*/
	// ADC.IMP.SUM.ucDIZCNT = DIZZER_CNT_INIT;
	// 								/* �Έړd���J�E���^������			*/
	// m_dizzerset();					/* �Έړd���ݒ�						*/

	return(EVT_IMP);				/* �L���C�x���g						*/
	// }

// //imp01 chg start
// //	ADC.IMP.SUM.ucDIZCNT += DIZZER_STEP;/* �Έړd�����Z						*/
// 	ADC.IMP.SUM.ucDIZCNT += (uchar)((HSRAM.ADC.ucSTS == ADC_IMP)
// 									? DIZZER_STEP_50 : DIZZER_STEP_5);
// 										/* �Έړd�����Z						*/
// //imp01 chg end
// 	m_dizzerset();						/* �Έړd���ݒ�						*/

	// return(EVT_NOP);					/* �������̏ꍇ�͖����C�x���g		*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	ConvImp											*/
/*																			*/
/*		�֐�����		�F	�C���s�␳										*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2019/12/12	���R								*/
/*																			*/
/****************************************************************************/
void	ConvImp(void)
{
	struct	MEAS_IMP_T	*pIn;			/* �C���s�X��/�ʑ��␳�O�ް��߲��	*/
	struct	MEAS_IMP_T	*pOut;			/* �C���s�X��/�ʑ��␳���ް��߲��	*/
//imp01 add start
	struct	IMP_CALB_T	*pImpCalb;		/* �C���s�x�[�X�␳�ް��߲��		*/
	ushort	usCoefA;					/* �␳�W���`						*/
	short	sCoefB;						/* �␳�W���a						*/
//imp01 add end

	if(HSRAM.ADC.ucSTS == ADC_IMP)		/* 50kHz							*/
	{
		pIn = &MEAS.IMPSRC[IMP_FREQ_50KHZ];
		pOut = &MEAS.IMP[IMP_FREQ_50KHZ];
//imp01 add start
		pImpCalb = &SYS.INF.IMPCALB;
		usCoefA = SYS.INF.IMPCONV.usCOEFA;
		sCoefB = SYS.INF.IMPCONV.sCOEFB;
//imp01 add end
	}
	else								/* 5kHz								*/
	{
		pIn = &MEAS.IMPSRC[IMP_FREQ_5KHZ];
		pOut = &MEAS.IMP[IMP_FREQ_5KHZ];
//imp01 add start
		pImpCalb = &SYS.INF.IMPCALB_5;
		usCoefA = SYS.INF.IMPCONV_5.usCOEFA;
		sCoefB = SYS.INF.IMPCONV_5.sCOEFB;
//imp01 add end
	}

//imp01 chg start
//	ConvImpBase(pIn, ADC.IMP.THIS, &SYS.INF.IMPCALB);
//										/* �C���s�x�[�X�␳					*/
//	s_memcpy(pOut, pIn, sizeof(struct MEAS_IMP_T));
//										/* �C���s��񏉊���					*/
//	if((HSRAM.ADC.ucSTS == ADC_IMP) && (SYS.INF.ucIMPCONVSTS == ON))
//	{									/* 50kHz���C���s�␳����̏ꍇ	*/
//		ConvImpZRX(pOut, pIn, SYS.INF.IMPCONV.usCOEFA,
//												SYS.INF.IMPCONV.sCOEFB);
//										/* �y�q�w�␳						*/
//	}
	ConvImpBase(pIn, ADC.IMP.THIS, pImpCalb);
										/* �C���s�x�[�X�␳					*/
	s_memcpy(pOut, pIn, sizeof(struct MEAS_IMP_T));
										/* �C���s��񏉊���					*/
	if(SYS.INF.ucIMPCONVSTS == ON)
	{									/* �C���s�␳����̏ꍇ				*/
		ConvImpZRX(pOut, pIn, usCoefA, sCoefB);
										/* �y�q�w�␳						*/
	}
//imp01 chg end
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	AdjImp											*/
/*																			*/
/*		�֐�����		�F	�C���s����										*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/*		C  = 6000*In200*In800 / (Out800*In200 - Out200*In800)				*/
/*		OS = 8000 - 6000*Out800*In200 / (Out800*In200 - Out200*In800)		*/
/*																			*/
/****************************************************************************/
uchar	AdjImp(struct IMP_CALB_T *pCalb, struct IMP_T *pHi, struct IMP_T *pLo)
{
										/* pCalb	:�����W���ւ̃|�C���^	*/
										/* pHi		:����R�ւ̃|�C���^		*/
										/* pLo		:���R�ւ̃|�C���^		*/

	float	fReA;						/* �e�탏�[�N						*/
	float	fReB;
	float	fImA;
	float	fImB;

	fReA = (float)(pHi + IMP_OBJ_OUT)->lRE;
	fImA = (float)(pHi + IMP_OBJ_OUT)->lIM;
										/* A = Out800						*/
	fReB = (float)-(pLo + IMP_OBJ_OUT)->lRE;
	fImB = (float)-(pLo + IMP_OBJ_OUT)->lIM;
										/* B = -Out200						*/
	ImMul(&fReA, &fImA,
			(float)(pLo + IMP_OBJ_IN)->lRE, (float)(pLo + IMP_OBJ_IN)->lIM);
										/* A = Out800 * In200				*/
	ImMul(&fReB, &fImB,
			(float)(pHi + IMP_OBJ_IN)->lRE, (float)(pHi + IMP_OBJ_IN)->lIM);
										/* B = -Out200 * In800				*/
	ImAdd(&fReB, &fImB, fReA, fImA);	/* B = Out800*In200 - Out200*In800	*/
	ImDiv(&fReB, &fImB, (float)6000, (float)0);
										/* B = (Out800*In200-Out200*In800)  */
										/*     / 6000						*/
	ImDiv(&fReA, &fImA, fReB, fImB);	/* A = 8000 - (6000*Out800*In200) /	*/
	fReA = (float)((float)8000 - fReA);	/*	   (Out800*In200-Out200*In800)	*/

	if(FtoS(&pCalb->sOSR, fReA) == NG)	/* OS�����ݒ�						*/
	{
		return(NG);
	}
	if(FtoS(&pCalb->sOSX, -fImA) == NG)	/* OS�����ݒ�						*/
	{
		return(NG);
	}

	fReA = (float)(pLo + IMP_OBJ_IN)->lRE;
	fImA = (float)(pLo + IMP_OBJ_IN)->lIM;
										/* A = In200						*/
	ImMul(&fReA, &fImA,
			(float)(pHi + IMP_OBJ_IN)->lRE, (float)(pHi + IMP_OBJ_IN)->lIM);
										/* A = In200 * In800				*/
	ImDiv(&fReA, &fImA, fReB, fImB);	/* A = (6000*In200*In800) /			*/
										/*	   (Out800*In200-Out200*In800)	*/
	if(FtoS(&pCalb->sCR, fReA) == NG)	/* C�����ݒ�						*/
	{
		return(NG);
	}
	if(FtoS(&pCalb->sCX, fImA) == NG)	/* C�����ݒ�						*/
	{
		return(NG);
	}

	return(OK);
}


//ble02 del start
/****************************************************************************/
/*																			*/
/*		�֐���			�F	ChkConvImp										*/
/*																			*/
/*		�֐�����		�F	�C���s�␳�L���`�F�b�N							*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/****************************************************************************/
//uchar	ChkConvImp(uchar ucEnable)
//{
//										/* ucEnable	:�L�������t���O			*/
////imp01 chg start
////	if((ucEnable == ON) && (MTR.MSTS == MSTS_CNT)
////			&& (HSRAM.ADC.ucSTS == ADC_IMP))
////	{									/* �L��and�J�E���g���[�hand50kHz	*/
//	if((ucEnable == ON) && (MTR.MSTS == MSTS_CNT))
//	{									/* �L��and�J�E���g���[�h			*/
////imp01 chg end
//		if((MTR.SSTS == SSTS_CNT_Z)
//			|| (MTR.SSTS == SSTS_CNT_R)
//			|| (MTR.SSTS == SSTS_CNT_X))
//		{								/* �y�A���͂q�A���͂w�̏ꍇ			*/
//			return(OK);					/* �n�j�ԋp							*/
//		}
//	}
//	return(NG);							/* ���̑��̏ꍇ�͂m�f�ԋp			*/
//}
//
//ble02 del end

/****************************************************************************/
/*																			*/
/*		�֐���			�F	GetImpZ											*/
/*																			*/
/*		�֐�����		�F	�y�擾											*/
/*																			*/
/*		�쐬�N����		�F	2010/ 9/22	�[�c								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/****************************************************************************/
ulong	GetImpZ(long sR, long sX)
{
										/* sR	:R							*/
										/* sX	:X							*/
	long	lR;							/* R���[�N							*/
	long	lX;							/* X���[�N							*/

	lR = (long)sR;						/* �^�ϊ�							*/
	lX = (long)sX;
	return((ulong)((s_sqrt((ulong)((lR * lR * (long)4)
									+ (lX * lX * (long)4)))
										+ (ulong)1) / (ulong)2));
										/* �y�ԋp(�l�̌ܓ�)					*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	Dft												*/
/*																			*/
/*		�֐�����		�F	�c�e�s											*/
/*																			*/
/*		�쐬�N����		�F	2010/12/ 2	�[�c								*/
/*																			*/
/*		�P�̎�������	�F	2020/ 7/30	����								*/
/*																			*/
/****************************************************************************/
void	Dft(struct IMP_T *pOut, ulong *pIn)
{
										/* pIn	:���̓f�[�^�|�C���^			*/
										/* pOut	:�o�̓f�[�^�|�C���^			*/
	uchar	ucOffset;					/* �I�t�Z�b�g						*/
	uchar	ucCnt;						/* �J�E���^							*/

	pOut->lRE = (long)0;				/* �o�̓f�[�^������					*/
	pOut->lIM = (long)0;
	ucOffset = (uchar)0;				/* �I�t�Z�b�g������					*/

	for(ucCnt = (uchar)0; ucCnt < IMP_DATA_NUM; ucCnt++)
	{									/* �f�[�^�ώZ						*/
		pOut->lRE += (long)((long)CosTbl[ucOffset]*(long)(*pIn));
		pOut->lIM += (long)((long)SinTbl[ucOffset]*(long)(*pIn));
		pIn++;
		ucOffset += (uchar)((HSRAM.ADC.ucSTS == ADC_IMP)
							? DFT_DATA_OFFSET_50 : DFT_DATA_OFFSET_5);
		if(ucOffset >= IMP_DATA_NUM)	/* �I�t�Z�b�g�X�V					*/
		{
			ucOffset -= IMP_DATA_NUM;
		}
	}
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	ConvImpBase										*/
/*																			*/
/*		�֐�����		�F	�C���s�x�[�X�␳								*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/*		Z = (C * PATV / REF) + OS											*/
/*																			*/
/****************************************************************************/
void	ConvImpBase(struct MEAS_IMP_T *pOut, struct IMP_T *pIn,
											struct IMP_CALB_T *pCalb)
{
										/* pOut		:�o�̓f�[�^�ւ̃|�C���^	*/
										/* pIn		:���̓f�[�^�ւ̃|�C���^	*/
										/* pCalb	:�W���ւ̃|�C���^		*/

	float	fRe;						/* ����								*/
	float	fIm;						/* ����								*/

	fRe = (float)pCalb->sCR;			/* A = C							*/
	fIm = (float)pCalb->sCX;
	ADC.IMP.SUM.ulDATA[0]= fRe;
	ImMul(&fRe, &fIm,
			(float)(pIn + IMP_OBJ_OUT)->lRE, (float)(pIn + IMP_OBJ_OUT)->lIM);
										/* A = C * PATV						*/
	ADC.IMP.SUM.ulDATA[1]= fRe;
	ImDiv(&fRe, &fIm,
			(float)(pIn + IMP_OBJ_IN)->lRE, (float)(pIn + IMP_OBJ_IN)->lIM);
										/* A = C * PATV / REF				*/
	ADC.IMP.SUM.ulDATA[2]= fRe;
	ImAdd(&fRe, &fIm, (float)pCalb->sOSR, (float)pCalb->sOSX);
										/* A = (C * PATV / REF) + OS		*/

	if(FtoS(&pOut->sRDT, fRe) == NG)	/* C�����ݒ�						*/
	{
		pOut->sRDT = (short)0;
	}
	if(FtoS(&pOut->sXDT, fIm) == NG)	/* X�ݒ�							*/
	{
		pOut->sXDT = (short)0;
	}
	pOut->usZDT = GetImpZ(pOut->sRDT, pOut->sXDT);
										/* �y�擾							*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	ConvImpZRX										*/
/*																			*/
/*		�֐�����		�F	�y�q�w�␳										*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 9	�[�c								*/
/*																			*/
/*		�����Ԃ̏ꍇ�͂q�w�␳												*/
/*			�q'�{���w�f���i�����{�������j���i�q�{�����j						*/
/*					   ���i�������q�|���������j�{���i���������{�������q�j	*/
/*								�q�F�␳�O�q���P�O�i���j					*/
/*								�w�F�␳�O�w���P�O�i���j					*/
/*								�����F�ʑ��␳�W���`���W�O�O�O��			*/
/*								�����F�ʑ��␳�W���a���W�O�O�O��(�Q�̕␔)	*/
/*																			*/
/****************************************************************************/
void	ConvImpZRX(struct MEAS_IMP_T *pOut, struct MEAS_IMP_T *pIn,
											ushort usConvA, short sConvB)
{
										/* pOut		:�o�̓f�[�^�ւ̃|�C���^	*/
										/* pIn		:���̓f�[�^�ւ̃|�C���^	*/
										/* usConvA	:�����W��A				*/
										/* sConvB	:�����W��B				*/

	pOut->sRDT = (short)DivRoundSL((long)(((long)usConvA * (long)pIn->sRDT)
										- ((long)sConvB * (long)pIn->sXDT)),
									(long)32768);
										/* �q�␳							*/
	pOut->sXDT = (short)DivRoundSL((long)(((long)usConvA * (long)pIn->sXDT)
										+ ((long)sConvB * (long)pIn->sRDT)),
									(long)32768);
										/* �w�␳							*/

	pOut->usZDT = GetImpZ(pOut->sRDT, pOut->sXDT);
										/* �y�擾							*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	ChkImpStable									*/
/*																			*/
/*		�֐�����		�F	�C���s����`�F�b�N								*/
/*																			*/
/*		�쐬�N����		�F	2010/10/ 8	�[�c								*/
/*																			*/
/*		�P�̎�������	�F	2022/ 8/ 8	���R								*/
/*																			*/
/****************************************************************************/
void	ChkImpStable(void)
{
	float	fComp;						/* ��r�l							*/
	float	fScl;						/* �X�J���[��						*/
	float	fSclThisIn;					/* ��������X�J���[��				*/

	fComp = (float)(IMP_AVG + (float)1);/* ��r�l������						*/
	fSclThisIn = ConvScalar(ADC.IMP.THIS[IMP_OBJ_IN].lRE,
								ADC.IMP.THIS[IMP_OBJ_IN].lIM);
										/* ��������X�J���[�ʎ擾			*/
	if(MEAS.ucOBJ == IMP_OBJ_IN)		/* ��������̏ꍇ					*/
	{
//imp01 chg start
//		if((IMP_IN_OPEN_MIN < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX))
// 		if(((HSRAM.ADC.ucSTS == ADC_IMP)
// 			&& (IMP_IN_OPEN_MIN < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX))
// 			|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 			&& (IMP_IN_OPEN_MIN_5 < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX_5)))
// //imp01 chg end
// 		{								/* �I�[�v������						*/
			fComp = (long)(((IMP_REF
							* ConvScalar(ADC.IMP.LAST[IMP_OBJ_IN].lRE,
											ADC.IMP.LAST[IMP_OBJ_IN].lIM))
							/ fSclThisIn) - IMP_REF);
										/* (8200*�O�����/�������)-8200	*/
		// }
	}
	else								/* �O������̏ꍇ					*/
	{
		// if((float)(((float)ADC.IMP.THIS[IMP_OBJ_OUT].lRE
		// 				* (float)ADC.IMP.THIS[IMP_OBJ_IN].lRE)
		// 			+ ((float)ADC.IMP.THIS[IMP_OBJ_OUT].lIM
		// 				* (float)ADC.IMP.THIS[IMP_OBJ_IN].lIM)) > (float)0)
		// {								/* �I�[�v������1					*/
			fScl = ConvScalar(ADC.IMP.THIS[IMP_OBJ_OUT].lRE,
								ADC.IMP.THIS[IMP_OBJ_OUT].lIM);
										/* ����O���X�J���[�ʎ擾			*/
// 			if(fSclThisIn
// //imp01 chg start
// //				&& (IMP_OUT_OPEN_MIN < fScl) && (fScl < IMP_OUT_OPEN_MAX))
// 				&& (((HSRAM.ADC.ucSTS == ADC_IMP)
// 					&& (IMP_OUT_OPEN_MIN < fScl) && (fScl < IMP_OUT_OPEN_MAX))
// 					|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 					&& (IMP_OUT_OPEN_MIN_5 < fScl) && (fScl < IMP_OUT_OPEN_MAX_5))))
// //imp01 chg end
// 			{							/* �I�[�v������2					*/
				fComp = (float)(IMP_REF * (fScl
						- ConvScalar(ADC.IMP.LAST[IMP_OBJ_OUT].lRE,
										ADC.IMP.LAST[IMP_OBJ_OUT].lIM))
						/ fSclThisIn);
										/* 8200*(����O��-�O��O��)/�������*/
			// }
		// }
	}

	if(fComp < (float)0)				/* ��r�l�̐�Βl���擾				*/
	{
		fComp = -fComp;
	}

	if(fComp > IMP_AVG)					/* ���ϔ͈͊O�̏ꍇ					*/
	{
		MEAS.ucAVGCNT = (uchar)0;		/* ���ω񐔏�����					*/
		MEAS.ucSTBCNT = (uchar)0;		/* ����񐔏�����					*/
	}
	else								/* ���ϔ͈͓��̏ꍇ					*/
	{
		if(MEAS.ucAVGCNT < IMP_WAIT_MAX)
		{								/* ���ω񐔍ő�l�����̏ꍇ			*/
			MEAS.ucAVGCNT++;			/* ���ω�+1						*/
		}

		if(fComp > IMP_STB)				/* ����͈͊O�̏ꍇ	�A				*/
		{
			MEAS.ucSTBCNT = (uchar)0;	/* ����񐔏�����					*/
		}
		else if(MEAS.ucSTBCNT < IMP_WAIT_MAX)
		{								/* ����񐔍ő�l�����̏ꍇ			*/
			MEAS.ucSTBCNT++;			/* �����+1						*/
		}
	}

// //ble02 chg start
// //	if((MTR.MSTS != MSTS_CNT) && (MEAS.ucOBJ == IMP_OBJ_OUT)
// //			&& !MEAS.ucSTBCNT && MEAS.ucCNT)
// 	if(!((MTR.MSTS == MSTS_BLETEST) && (MTR.SSTS == SSTS_BLE_CNT))
// 			&& (MEAS.ucOBJ == IMP_OBJ_OUT)
// 			&& !MEAS.ucSTBCNT && MEAS.ucCNT)
// //ble02 chg end
// 	{									/* �J�E���g���[�h�ȊO���l�̂�		*/
// 										/* �s�����Ԃ������񑱂����ꍇ		*/
// 		MEAS.ucCNT = IMP_WAIT_MAX;		/* ����񐔂��ő�ɐݒ肵��			*/
// 										/* ���t�@�����X����đ��肷��		*/
// 	}
// //ble02 chg start
// //	else if((MTR.MSTS == MSTS_CNT) || (MEAS.ucCNT < IMP_WAIT_MAX))
// 	else if(((MTR.MSTS == MSTS_BLETEST) && (MTR.SSTS == SSTS_BLE_CNT))
// 		|| (MEAS.ucCNT < IMP_WAIT_MAX))
// //ble02 chg end
// 	{									/* �J�E���g���[�h���͑���񐔔͈͓�	*/
// 		MEAS.ucCNT++;					/* �̏ꍇ�͑���񐔍X�V				*/
// 	}

	s_memcpy(&ADC.IMP.LAST[MEAS.ucOBJ], &ADC.IMP.THIS[MEAS.ucOBJ],
										sizeof(struct IMP_T));
										/* �O��l�X�V						*/
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	ConvScalar										*/
/*																			*/
/*		�֐�����		�F	�x�N�g���˃X�J���[�ϊ�							*/
/*																			*/
/*		�쐬�N����		�F	2010/10/ 8	�[�c								*/
/*																			*/
/*		�P�̎�������	�F	2014/ 1/16	����								*/
/*																			*/
/****************************************************************************/
float	ConvScalar(long lX, long lY)
{
										/* lX	:X����						*/
										/* lY	:Y����						*/
	return((float)(((float)lX * (float)lX) + ((float)lY * (float)lY)));
										/* �X�J���[�ʕԋp					*/
}


//lllib����ǉ�
/****************************************************************************/
/*																			*/
/*		�֐���			�F	s_memcpy										*/
/*																			*/
/*		�֐�����		�F	�������R�s�[									*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 7	�[�c								*/
/*																			*/
/****************************************************************************/
void	s_memcpy(void *pOut, const void *pIn, size_t Size)
{
										/* pOut		:�o�̓f�[�^�ւ̃|�C���^	*/
										/* pIn		:���̓f�[�^�ւ̃|�C���^	*/
										/* Size		:�f�[�^��				*/
	for(; Size > (size_t)0; Size--)
	{
		(*(uchar*)pOut) = (*(uchar*)pIn);
		pOut = (void*)(((uchar*)pOut) + 1);
										/* �o�̓f�[�^�ɓ��̓f�[�^���R�s�[	*/
		pIn = (void*)(((uchar*)pIn) + 1);
										/* ���̃f�[�^�A�h���X��				*/
	}
}


/****************************************************************************/
/*																			*/
/*		�֐���			�F	s_memset										*/
/*																			*/
/*		�֐�����		�F	�������Z�b�g									*/
/*																			*/
/*		�쐬�N����		�F	2009/ 5/14	���R								*/
/*																			*/
/*		�P�̎�������	�F	2010/12/ 7	�[�c								*/
/*																			*/
/****************************************************************************/
void	s_memset(void *pOut, uchar ucData, size_t Size)
{
										/* pOut		:�o�̓f�[�^�ւ̃|�C���^	*/
										/* ucData	:���̓f�[�^				*/
										/* Size		:�f�[�^��				*/

	for(; Size > (size_t)0; Size--)
	{
		*(uchar*)pOut = ucData;			/* �o�̓f�[�^����̓f�[�^�Ŗ��߂�	*/
		pOut = (void*)(((uchar*)pOut) + 1);
										/* ���̃f�[�^�A�h���X��				*/
	}
}