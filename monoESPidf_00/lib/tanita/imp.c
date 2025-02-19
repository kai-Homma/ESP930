/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	imp.c											*/
/*																			*/
/*		モジュール名称	：	インピーダンス処理								*/
/*																			*/
/*		作成年月日		：	2022/ 2/24	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 2/24	小山		imp01					*/
/*								インピ測定変更（シングル⇒デュアル周波数）	*/
/*							2022/ 4/ 4	小山		ble02					*/
/*								BLE通信で調整・検査							*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

#include	"at_inc.h"					/* インクルードファイル				*/


/****************************************************************************/
/*		ＲＡＭ																*/
/****************************************************************************/


/****************************************************************************/
/*		ＲＯＭ																*/
/****************************************************************************/
/* 2^32/(10bitadc*int積算*偏移電圧*サンプリング数*移動平均*符号) = テーブル	*/
/* 2^32/(1023*64 *   4   *   13   *      20      *   4    * 2  ) =    7		*/

//imp01 chg start
//const_s CosTbl[IMP_DATA_NUM] = 			/* COSテーブル						*/
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
//const_s SinTbl[IMP_DATA_NUM] =			/* SINテーブル						*/
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

// const_s CosTbl[IMP_DATA_NUM] = 			/* COSテーブル						*/
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

// const_s SinTbl[IMP_DATA_NUM] =			/* SINテーブル						*/
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
/*		プロトタイプ宣言													*/
/****************************************************************************/
void	Dft(struct IMP_T *, ulong *);	/* ＤＦＴ							*/
void	ConvImpBase(struct MEAS_IMP_T *, struct IMP_T *, struct IMP_CALB_T *);
										/* インピベース補正					*/
void	ConvImpZRX(struct MEAS_IMP_T *, struct MEAS_IMP_T *, ushort, short);
										/* ＺＲＸ補正						*/
void	ChkImpStable(void);				/* インピ安定チェック				*/
float	ConvScalar(long, long);			/* ベクトル⇒スカラー変換			*/

void	s_memcpy(void *, const void *, size_t);
// void	s_memset(void *, uchar , size_t );


// /****************************************************************************/
// /*																			*/
// /*		関数名			：	InitImpConv										*/
// /*																			*/
// /*		関数名称		：	インピ補正係数初期化							*/
// /*																			*/
// /*		作成年月日		：	2009/ 5/14	小山								*/
// /*																			*/
// /*		単体試験履歴	：	2019/12/12	小山								*/
// /*																			*/
// /****************************************************************************/
// void	InitImpConv(void)
// {
// 	s_memcpy(SYS.BLK.ucCIMP, SYS_INIT.BLK.ucCIMP, sizeof(SYS.BLK.ucCIMP));
// 										/* インピ調整時初期化領域初期化		*/
// 	SYS.INF.ucIMPCHKSUM = (uchar)(GetChkSum((uchar *)&SYS.INF.IMPCALB,
// 							(uchar)sizeof(SYS.INF.IMPCALB)) ^ (uchar)0xFF);
// 										/* チェックサム異常設定				*/
// //imp01 add start
// 	SYS.INF.ucIMPCHKSUM_5 =
// 				(uchar)(GetChkSum((uchar *)&SYS.INF.IMPCALB_5,
// 					(uchar)sizeof(SYS.INF.IMPCALB_5)) ^ (uchar)0xFF);
// 										/* チェックサム異常設定				*/
// //imp01 add end
// }
// 

/****************************************************************************/
/*																			*/
/*		関数名			：	ChkImpBase										*/
/*																			*/
/*		関数名称		：	インピ基準抵抗範囲チェック						*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2022/ 7/27	小山								*/
/*																			*/
/****************************************************************************/
uchar	ChkImpBase(uchar ucBase)
{
										/* ucBase	:基準抵抗				*/

//imp01 chg start
//	short	sRdt;						/* レジスタンス						*/
//
//	ConvImp();							/* インピ補正						*/
//	sRdt = MEAS.IMPSRC[IMP_FREQ_50KHZ].sRDT;
//										/* レジスタンス設定					*/
	ushort	usZdt;						/* インピーダンス					*/


	ConvImp();							/* インピ補正						*/

	if(HSRAM.ADC.ucSTS == ADC_IMP)		/* 50kHzの場合						*/
	{
		usZdt = MEAS.IMPSRC[IMP_FREQ_50KHZ].usZDT;
										/* インピーダンス設定				*/
	}
	else								/* 5kHzの場合						*/
	{
		usZdt = MEAS.IMPSRC[IMP_FREQ_5KHZ].usZDT;
										/* インピーダンス設定				*/
	}
//imp01 chg end

	if(ucBase == IMP_BASE_H)
	{
//imp01 chg start
//		if((sRdt > IMP_ADJ_800_MIN) && (sRdt < IMP_ADJ_800_MAX))
//		{								/* 基準抵抗800Ω±100Ω未満の場合	*/
		if((IMP_ADJ_800_MIN <= usZdt) && (usZdt <= IMP_ADJ_800_MAX))
		{								/* 基準抵抗800Ωの範囲以内場合		*/
//imp01 chg end
			return(OK);					/* ＯＫ								*/
		}
	}
	else
	{
//imp01 chg start
//		if((sRdt > IMP_ADJ_200_MIN) && (sRdt < IMP_ADJ_200_MAX))
//		{								/* 基準抵抗200Ω±100Ω未満の場合	*/
		if((IMP_ADJ_200_MIN <= usZdt) && (usZdt <= IMP_ADJ_200_MAX))
		{								/* 基準抵抗200Ωの範囲以内場合		*/
//imp01 chg end
			return(OK);					/* ＯＫ								*/
		}
	}

	return(NG);							/* 異常範囲の場合はＮＧ				*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	SetImp											*/
/*																			*/
/*		関数名称		：	インピ情報設定									*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/****************************************************************************/
void	SetImp(struct IMP_T *pOut, struct IMP_T *pIn)
{
										/* pOut	:出力情報					*/
										/* pIn	:入力情報					*/

	(pOut + IMP_OBJ_IN)->lRE = (pIn + IMP_OBJ_IN)->lRE;
	(pOut + IMP_OBJ_IN)->lIM = (pIn + IMP_OBJ_IN)->lIM;
	(pOut + IMP_OBJ_OUT)->lRE = (pIn + IMP_OBJ_OUT)->lRE;
	(pOut + IMP_OBJ_OUT)->lIM = (pIn + IMP_OBJ_OUT)->lIM;
										/* インピ情報設定					*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	MeasImp											*/
/*																			*/
/*		関数名称		：	インピ測定										*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2022/ 7/27	小山								*/
/*																			*/
/****************************************************************************/
enum EVT_ID	MeasImp(void)
{
	struct	IMP_T	Imp;				/* インピデータ						*/
	struct	IMP_T	*pImp;				/* インピポインタ					*/

// //imp01 chg start
// //	if(ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP))
// 	if(((HSRAM.ADC.ucSTS == ADC_IMP)
// 		&& (ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP_50)))
// 		|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 		&& (ADC.IMP.SUM.ucDIZCNT > (uchar)(MAX_IMP_DIZZER - DIZZER_STEP_5))))
// //imp01 chg end
// 	{									/* 偏移電圧積算完了の場合			*/
	pImp = &ADC.IMP.THIS[MEAS.ucOBJ];
									/* インピポインタ取得				*/
	Dft(&Imp, ADC.IMP.SUM.ulDATA);	/* ＤＦＴ							*/
	StoreDataSL(Imp.lRE, &ADC.IMP.STORE.lRE[0],
						&ADC.IMP.STORE.lRE[ADC_IMP_STORE_MAX - 1]);
									/* インピ実部蓄積					*/
	pImp->lRE = AvgDataSL(MEAS.ucAVGCNT, ADC_IMP_STORE_MAX,
						ADC_IMP_ROUND, &ADC.IMP.STORE.lRE[0]);
									/* インピ実部平均					*/
	StoreDataSL(Imp.lIM, &ADC.IMP.STORE.lIM[0],
						&ADC.IMP.STORE.lIM[ADC_IMP_STORE_MAX - 1]);
									/* インピ虚部蓄積					*/
	pImp->lIM = AvgDataSL(MEAS.ucAVGCNT, ADC_IMP_STORE_MAX,
						ADC_IMP_ROUND, &ADC.IMP.STORE.lIM[0]);
									/* インピ虚部平均					*/
	ChkImpStable();					/* インピ安定チェック				*/

	s_memset(&ADC.IMP.SUM.ulDATA, (uchar)0,
									sizeof(ADC.IMP.SUM.ulDATA));
	// 								/* インピ積算情報初期化				*/
	// ADC.IMP.SUM.ucDIZCNT = DIZZER_CNT_INIT;
	// 								/* 偏移電圧カウンタ初期化			*/
	// m_dizzerset();					/* 偏移電圧設定						*/

	return(EVT_IMP);				/* 有効イベント						*/
	// }

// //imp01 chg start
// //	ADC.IMP.SUM.ucDIZCNT += DIZZER_STEP;/* 偏移電圧加算						*/
// 	ADC.IMP.SUM.ucDIZCNT += (uchar)((HSRAM.ADC.ucSTS == ADC_IMP)
// 									? DIZZER_STEP_50 : DIZZER_STEP_5);
// 										/* 偏移電圧加算						*/
// //imp01 chg end
// 	m_dizzerset();						/* 偏移電圧設定						*/

	// return(EVT_NOP);					/* 未完了の場合は無効イベント		*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	ConvImp											*/
/*																			*/
/*		関数名称		：	インピ補正										*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2019/12/12	小山								*/
/*																			*/
/****************************************************************************/
void	ConvImp(void)
{
	struct	MEAS_IMP_T	*pIn;			/* インピ傾き/位相補正前ﾃﾞｰﾀﾎﾟｲﾝﾀ	*/
	struct	MEAS_IMP_T	*pOut;			/* インピ傾き/位相補正後ﾃﾞｰﾀﾎﾟｲﾝﾀ	*/
//imp01 add start
	struct	IMP_CALB_T	*pImpCalb;		/* インピベース補正ﾃﾞｰﾀﾎﾟｲﾝﾀ		*/
	ushort	usCoefA;					/* 補正係数Ａ						*/
	short	sCoefB;						/* 補正係数Ｂ						*/
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
//										/* インピベース補正					*/
//	s_memcpy(pOut, pIn, sizeof(struct MEAS_IMP_T));
//										/* インピ情報初期化					*/
//	if((HSRAM.ADC.ucSTS == ADC_IMP) && (SYS.INF.ucIMPCONVSTS == ON))
//	{									/* 50kHzかつインピ補正ありの場合	*/
//		ConvImpZRX(pOut, pIn, SYS.INF.IMPCONV.usCOEFA,
//												SYS.INF.IMPCONV.sCOEFB);
//										/* ＺＲＸ補正						*/
//	}
	ConvImpBase(pIn, ADC.IMP.THIS, pImpCalb);
										/* インピベース補正					*/
	s_memcpy(pOut, pIn, sizeof(struct MEAS_IMP_T));
										/* インピ情報初期化					*/
	if(SYS.INF.ucIMPCONVSTS == ON)
	{									/* インピ補正ありの場合				*/
		ConvImpZRX(pOut, pIn, usCoefA, sCoefB);
										/* ＺＲＸ補正						*/
	}
//imp01 chg end
}


/****************************************************************************/
/*																			*/
/*		関数名			：	AdjImp											*/
/*																			*/
/*		関数名称		：	インピ調整										*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/*		C  = 6000*In200*In800 / (Out800*In200 - Out200*In800)				*/
/*		OS = 8000 - 6000*Out800*In200 / (Out800*In200 - Out200*In800)		*/
/*																			*/
/****************************************************************************/
uchar	AdjImp(struct IMP_CALB_T *pCalb, struct IMP_T *pHi, struct IMP_T *pLo)
{
										/* pCalb	:調整係数へのポインタ	*/
										/* pHi		:高抵抗へのポインタ		*/
										/* pLo		:低抵抗へのポインタ		*/

	float	fReA;						/* 各種ワーク						*/
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

	if(FtoS(&pCalb->sOSR, fReA) == NG)	/* OS実部設定						*/
	{
		return(NG);
	}
	if(FtoS(&pCalb->sOSX, -fImA) == NG)	/* OS虚部設定						*/
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
	if(FtoS(&pCalb->sCR, fReA) == NG)	/* C実部設定						*/
	{
		return(NG);
	}
	if(FtoS(&pCalb->sCX, fImA) == NG)	/* C虚部設定						*/
	{
		return(NG);
	}

	return(OK);
}


//ble02 del start
/****************************************************************************/
/*																			*/
/*		関数名			：	ChkConvImp										*/
/*																			*/
/*		関数名称		：	インピ補正有効チェック							*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/****************************************************************************/
//uchar	ChkConvImp(uchar ucEnable)
//{
//										/* ucEnable	:有効無効フラグ			*/
////imp01 chg start
////	if((ucEnable == ON) && (MTR.MSTS == MSTS_CNT)
////			&& (HSRAM.ADC.ucSTS == ADC_IMP))
////	{									/* 有効andカウントモードand50kHz	*/
//	if((ucEnable == ON) && (MTR.MSTS == MSTS_CNT))
//	{									/* 有効andカウントモード			*/
////imp01 chg end
//		if((MTR.SSTS == SSTS_CNT_Z)
//			|| (MTR.SSTS == SSTS_CNT_R)
//			|| (MTR.SSTS == SSTS_CNT_X))
//		{								/* Ｚ、又はＲ、又はＸの場合			*/
//			return(OK);					/* ＯＫ返却							*/
//		}
//	}
//	return(NG);							/* その他の場合はＮＧ返却			*/
//}
//
//ble02 del end

/****************************************************************************/
/*																			*/
/*		関数名			：	GetImpZ											*/
/*																			*/
/*		関数名称		：	Ｚ取得											*/
/*																			*/
/*		作成年月日		：	2010/ 9/22	深田								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/****************************************************************************/
ulong	GetImpZ(long sR, long sX)
{
										/* sR	:R							*/
										/* sX	:X							*/
	long	lR;							/* Rワーク							*/
	long	lX;							/* Xワーク							*/

	lR = (long)sR;						/* 型変換							*/
	lX = (long)sX;
	return((ulong)((s_sqrt((ulong)((lR * lR * (long)4)
									+ (lX * lX * (long)4)))
										+ (ulong)1) / (ulong)2));
										/* Ｚ返却(四捨五入)					*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	Dft												*/
/*																			*/
/*		関数名称		：	ＤＦＴ											*/
/*																			*/
/*		作成年月日		：	2010/12/ 2	深田								*/
/*																			*/
/*		単体試験履歴	：	2020/ 7/30	野﨑								*/
/*																			*/
/****************************************************************************/
void	Dft(struct IMP_T *pOut, ulong *pIn)
{
										/* pIn	:入力データポインタ			*/
										/* pOut	:出力データポインタ			*/
	uchar	ucOffset;					/* オフセット						*/
	uchar	ucCnt;						/* カウンタ							*/

	pOut->lRE = (long)0;				/* 出力データ初期化					*/
	pOut->lIM = (long)0;
	ucOffset = (uchar)0;				/* オフセット初期化					*/

	for(ucCnt = (uchar)0; ucCnt < IMP_DATA_NUM; ucCnt++)
	{									/* データ積算						*/
		pOut->lRE += (long)((long)CosTbl[ucOffset]*(long)(*pIn));
		pOut->lIM += (long)((long)SinTbl[ucOffset]*(long)(*pIn));
		pIn++;
		ucOffset += (uchar)((HSRAM.ADC.ucSTS == ADC_IMP)
							? DFT_DATA_OFFSET_50 : DFT_DATA_OFFSET_5);
		if(ucOffset >= IMP_DATA_NUM)	/* オフセット更新					*/
		{
			ucOffset -= IMP_DATA_NUM;
		}
	}
}


/****************************************************************************/
/*																			*/
/*		関数名			：	ConvImpBase										*/
/*																			*/
/*		関数名称		：	インピベース補正								*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/*		Z = (C * PATV / REF) + OS											*/
/*																			*/
/****************************************************************************/
void	ConvImpBase(struct MEAS_IMP_T *pOut, struct IMP_T *pIn,
											struct IMP_CALB_T *pCalb)
{
										/* pOut		:出力データへのポインタ	*/
										/* pIn		:入力データへのポインタ	*/
										/* pCalb	:係数へのポインタ		*/

	float	fRe;						/* 実部								*/
	float	fIm;						/* 虚部								*/

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

	if(FtoS(&pOut->sRDT, fRe) == NG)	/* C虚部設定						*/
	{
		pOut->sRDT = (short)0;
	}
	if(FtoS(&pOut->sXDT, fIm) == NG)	/* X設定							*/
	{
		pOut->sXDT = (short)0;
	}
	pOut->usZDT = GetImpZ(pOut->sRDT, pOut->sXDT);
										/* Ｚ取得							*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	ConvImpZRX										*/
/*																			*/
/*		関数名称		：	ＺＲＸ補正										*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 9	深田								*/
/*																			*/
/*		両足間の場合はＲＸ補正												*/
/*			Ｒ'＋ｊＸ’＝（ｃｒ＋ｊｃｘ）＊（Ｒ＋ｊｘ）						*/
/*					   ＝（ｃｒ＊Ｒ－ｃｘ＊ｘ）＋ｊ（ｃｒ＊ｘ＋ｃｘ＊Ｒ）	*/
/*								Ｒ：補正前Ｒ＊１０（Ω）					*/
/*								Ｘ：補正前Ｘ＊１０（Ω）					*/
/*								ｃｒ：位相補正係数Ａ＊８０００ｈ			*/
/*								ｃｘ：位相補正係数Ｂ＊８０００ｈ(２の補数)	*/
/*																			*/
/****************************************************************************/
void	ConvImpZRX(struct MEAS_IMP_T *pOut, struct MEAS_IMP_T *pIn,
											ushort usConvA, short sConvB)
{
										/* pOut		:出力データへのポインタ	*/
										/* pIn		:入力データへのポインタ	*/
										/* usConvA	:調整係数A				*/
										/* sConvB	:調整係数B				*/

	pOut->sRDT = (short)DivRoundSL((long)(((long)usConvA * (long)pIn->sRDT)
										- ((long)sConvB * (long)pIn->sXDT)),
									(long)32768);
										/* Ｒ補正							*/
	pOut->sXDT = (short)DivRoundSL((long)(((long)usConvA * (long)pIn->sXDT)
										+ ((long)sConvB * (long)pIn->sRDT)),
									(long)32768);
										/* Ｘ補正							*/

	pOut->usZDT = GetImpZ(pOut->sRDT, pOut->sXDT);
										/* Ｚ取得							*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	ChkImpStable									*/
/*																			*/
/*		関数名称		：	インピ安定チェック								*/
/*																			*/
/*		作成年月日		：	2010/10/ 8	深田								*/
/*																			*/
/*		単体試験履歴	：	2022/ 8/ 8	小山								*/
/*																			*/
/****************************************************************************/
void	ChkImpStable(void)
{
	float	fComp;						/* 比較値							*/
	float	fScl;						/* スカラー量						*/
	float	fSclThisIn;					/* 今回内部スカラー量				*/

	fComp = (float)(IMP_AVG + (float)1);/* 比較値初期化						*/
	fSclThisIn = ConvScalar(ADC.IMP.THIS[IMP_OBJ_IN].lRE,
								ADC.IMP.THIS[IMP_OBJ_IN].lIM);
										/* 今回内部スカラー量取得			*/
	if(MEAS.ucOBJ == IMP_OBJ_IN)		/* 内部測定の場合					*/
	{
//imp01 chg start
//		if((IMP_IN_OPEN_MIN < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX))
// 		if(((HSRAM.ADC.ucSTS == ADC_IMP)
// 			&& (IMP_IN_OPEN_MIN < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX))
// 			|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 			&& (IMP_IN_OPEN_MIN_5 < fSclThisIn) && (fSclThisIn < IMP_IN_OPEN_MAX_5)))
// //imp01 chg end
// 		{								/* オープン判定						*/
			fComp = (long)(((IMP_REF
							* ConvScalar(ADC.IMP.LAST[IMP_OBJ_IN].lRE,
											ADC.IMP.LAST[IMP_OBJ_IN].lIM))
							/ fSclThisIn) - IMP_REF);
										/* (8200*前回内部/今回内部)-8200	*/
		// }
	}
	else								/* 外部測定の場合					*/
	{
		// if((float)(((float)ADC.IMP.THIS[IMP_OBJ_OUT].lRE
		// 				* (float)ADC.IMP.THIS[IMP_OBJ_IN].lRE)
		// 			+ ((float)ADC.IMP.THIS[IMP_OBJ_OUT].lIM
		// 				* (float)ADC.IMP.THIS[IMP_OBJ_IN].lIM)) > (float)0)
		// {								/* オープン判定1					*/
			fScl = ConvScalar(ADC.IMP.THIS[IMP_OBJ_OUT].lRE,
								ADC.IMP.THIS[IMP_OBJ_OUT].lIM);
										/* 今回外部スカラー量取得			*/
// 			if(fSclThisIn
// //imp01 chg start
// //				&& (IMP_OUT_OPEN_MIN < fScl) && (fScl < IMP_OUT_OPEN_MAX))
// 				&& (((HSRAM.ADC.ucSTS == ADC_IMP)
// 					&& (IMP_OUT_OPEN_MIN < fScl) && (fScl < IMP_OUT_OPEN_MAX))
// 					|| ((HSRAM.ADC.ucSTS == ADC_IMP_5)
// 					&& (IMP_OUT_OPEN_MIN_5 < fScl) && (fScl < IMP_OUT_OPEN_MAX_5))))
// //imp01 chg end
// 			{							/* オープン判定2					*/
				fComp = (float)(IMP_REF * (fScl
						- ConvScalar(ADC.IMP.LAST[IMP_OBJ_OUT].lRE,
										ADC.IMP.LAST[IMP_OBJ_OUT].lIM))
						/ fSclThisIn);
										/* 8200*(今回外部-前回外部)/今回内部*/
			// }
		// }
	}

	if(fComp < (float)0)				/* 比較値の絶対値を取得				*/
	{
		fComp = -fComp;
	}

	if(fComp > IMP_AVG)					/* 平均範囲外の場合					*/
	{
		MEAS.ucAVGCNT = (uchar)0;		/* 平均回数初期化					*/
		MEAS.ucSTBCNT = (uchar)0;		/* 安定回数初期化					*/
	}
	else								/* 平均範囲内の場合					*/
	{
		if(MEAS.ucAVGCNT < IMP_WAIT_MAX)
		{								/* 平均回数最大値未満の場合			*/
			MEAS.ucAVGCNT++;			/* 平均回数+1						*/
		}

		if(fComp > IMP_STB)				/* 安定範囲外の場合	、				*/
		{
			MEAS.ucSTBCNT = (uchar)0;	/* 安定回数初期化					*/
		}
		else if(MEAS.ucSTBCNT < IMP_WAIT_MAX)
		{								/* 安定回数最大値未満の場合			*/
			MEAS.ucSTBCNT++;			/* 安定回数+1						*/
		}
	}

// //ble02 chg start
// //	if((MTR.MSTS != MSTS_CNT) && (MEAS.ucOBJ == IMP_OBJ_OUT)
// //			&& !MEAS.ucSTBCNT && MEAS.ucCNT)
// 	if(!((MTR.MSTS == MSTS_BLETEST) && (MTR.SSTS == SSTS_BLE_CNT))
// 			&& (MEAS.ucOBJ == IMP_OBJ_OUT)
// 			&& !MEAS.ucSTBCNT && MEAS.ucCNT)
// //ble02 chg end
// 	{									/* カウントモード以外かつ人体で		*/
// 										/* 不安定状態が複数回続いた場合		*/
// 		MEAS.ucCNT = IMP_WAIT_MAX;		/* 測定回数を最大に設定して			*/
// 										/* リファレンスから再測定する		*/
// 	}
// //ble02 chg start
// //	else if((MTR.MSTS == MSTS_CNT) || (MEAS.ucCNT < IMP_WAIT_MAX))
// 	else if(((MTR.MSTS == MSTS_BLETEST) && (MTR.SSTS == SSTS_BLE_CNT))
// 		|| (MEAS.ucCNT < IMP_WAIT_MAX))
// //ble02 chg end
// 	{									/* カウントモード又は測定回数範囲内	*/
// 		MEAS.ucCNT++;					/* の場合は測定回数更新				*/
// 	}

	s_memcpy(&ADC.IMP.LAST[MEAS.ucOBJ], &ADC.IMP.THIS[MEAS.ucOBJ],
										sizeof(struct IMP_T));
										/* 前回値更新						*/
}


/****************************************************************************/
/*																			*/
/*		関数名			：	ConvScalar										*/
/*																			*/
/*		関数名称		：	ベクトル⇒スカラー変換							*/
/*																			*/
/*		作成年月日		：	2010/10/ 8	深田								*/
/*																			*/
/*		単体試験履歴	：	2014/ 1/16	高橋								*/
/*																			*/
/****************************************************************************/
float	ConvScalar(long lX, long lY)
{
										/* lX	:X成分						*/
										/* lY	:Y成分						*/
	return((float)(((float)lX * (float)lX) + ((float)lY * (float)lY)));
										/* スカラー量返却					*/
}


//lllibから追加
/****************************************************************************/
/*																			*/
/*		関数名			：	s_memcpy										*/
/*																			*/
/*		関数名称		：	メモリコピー									*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 7	深田								*/
/*																			*/
/****************************************************************************/
void	s_memcpy(void *pOut, const void *pIn, size_t Size)
{
										/* pOut		:出力データへのポインタ	*/
										/* pIn		:入力データへのポインタ	*/
										/* Size		:データ長				*/
	for(; Size > (size_t)0; Size--)
	{
		(*(uchar*)pOut) = (*(uchar*)pIn);
		pOut = (void*)(((uchar*)pOut) + 1);
										/* 出力データに入力データをコピー	*/
		pIn = (void*)(((uchar*)pIn) + 1);
										/* 次のデータアドレスへ				*/
	}
}


/****************************************************************************/
/*																			*/
/*		関数名			：	s_memset										*/
/*																			*/
/*		関数名称		：	メモリセット									*/
/*																			*/
/*		作成年月日		：	2009/ 5/14	小山								*/
/*																			*/
/*		単体試験履歴	：	2010/12/ 7	深田								*/
/*																			*/
/****************************************************************************/
void	s_memset(void *pOut, uchar ucData, size_t Size)
{
										/* pOut		:出力データへのポインタ	*/
										/* ucData	:入力データ				*/
										/* Size		:データ長				*/

	for(; Size > (size_t)0; Size--)
	{
		*(uchar*)pOut = ucData;			/* 出力データを入力データで埋める	*/
		pOut = (void*)(((uchar*)pOut) + 1);
										/* 次のデータアドレスへ				*/
	}
}