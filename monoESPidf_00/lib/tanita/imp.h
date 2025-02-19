/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	imp.h											*/
/*																			*/
/*		モジュール名称	：	インピーダンスヘッダ							*/
/*																			*/
/*		作成年月日		：	2022/ 2/24	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 2/24	小山		imp01					*/
/*								インピ測定変更（シングル⇒デュアル周波数）	*/
/*							2022/ 2/24	小山		imp02					*/
/*								機種間位相補正係数変更						*/
/*							2022/ 2/24	小山		imp03					*/
/*								インピオープン判定変更						*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

//追加分
#define	ADC_IMP_5		(uchar)3		/*	3	インピ5kHz					*/
#define	ADC_IMP			(uchar)0x11		/*	5	インピ						*/


/****************************************************************************/
/*		定数定義															*/
/****************************************************************************/
#define	ADC_IMP_STORE_MAX	(uchar)8	/* インピ蓄積回数最大				*/
#define	ADC_IMP_ROUND		(ushort)16	/* インピカウント丸め値(4096分の1)	*/

#define IMP_DATA_NUM		(uchar)50	/*	インピ生データ数				*/ //50に変更

#define MAX_IMP_DIZZER		(uchar)27	/* 偏移電圧	偏移量最大				*/
										/*	1/9 LSB * 27 taps = 3 LSB		*/
#define	DIZZER_CNT_INIT		(uchar)0	/*			初期値					*/
//imp01 chg start
//#define	DIZZER_STEP			(uchar)2	/*			偏移量					*/
#define	DIZZER_STEP_50		(uchar)2	/*			偏移量	50kHz用			*/
#define	DIZZER_STEP_5		(uchar)12	/*			偏移量	5kHz用			*/
//imp01 chg end

										/*	ＤＦＴデータオフセット量		*/
//imp01 chg start
//#define DFT_DATA_OFFSET_50	(uchar)13	/*			測定周波数：50kHz		*/
//#define DFT_DATA_OFFSET_5	(uchar)11	/*			測定周波数：5kHz		*/
#define DFT_DATA_OFFSET_50	(uchar)1	/*			測定周波数：50kHz		*/ //1に変更
#define DFT_DATA_OFFSET_5	(uchar)3	/*			測定周波数：5kHz		*/
//imp01 chg end

#define	IMP_FREQ_50KHZ	(uchar)0		/* インピ測定周波数	50kHz			*/
#define	IMP_FREQ_5KHZ	(uchar)1		/*					5kHz			*/
#define	IMP_FREQ_MAX	(uchar)2		/* 周波数	最大					*/

#define	IMP_OBJ_IN		(uchar)0		/* 対象		内部					*/
#define	IMP_OBJ_OUT		(uchar)1		/*			外部					*/
#define	IMP_OBJ_MAX		(uchar)2		/*			最大					*/

#define	IMP_BASE_H		(uchar)0		/* 基準		高抵抗					*/
#define	IMP_BASE_L		(uchar)1		/*			低抵抗					*/
#define	IMP_BASE_MAX	(uchar)2		/*			最大					*/

										/* 変換係数							*/
										/*		初期化用					*/
#define	IMPCOEF_CR_INIT	(ushort)32768	/*		ＣＲ	 1*8000H			*/
#define	IMPCOEF_CX_INIT	(short)0		/*		ＣＸ	 0*8000H			*/

//imp02 chg start
//#define	IMPCOEF_CR_DEF	(ushort)32522	/* 50kHzＣＲ	 0.992495373*32768	*/
//#define	IMPCOEF_CX_DEF	(short)178		/*		ＣＸ	 0.005425553*32768	*/
//#define	IMPCOEF_CR_DEF_5	(ushort)32768
//										/*  5kHzＣＲ	 1*8000H			*/
//#define	IMPCOEF_CX_DEF_5	(short)0	/*		ＣＸ	 0*8000H			*/
//kkkk 量産相当品で設定する必要あり
//#define	IMPCOEF_CR_DEF	(ushort)32768	/* 50kHzＣＲ	 0.992495373*32768	*/
//#define	IMPCOEF_CX_DEF	(short)0		/*		ＣＸ	 0.005425553*32768	*/
//#define	IMPCOEF_CR_DEF_5	(ushort)32768
//										/*  5kHzＣＲ	 1*8000H			*/
//#define	IMPCOEF_CX_DEF_5	(short)0	/*		ＣＸ	 0*8000H			*/
#define	IMPCOEF_CR_DEF	(ushort)32771	/* 50kHzＣＲ	 1*8000H			*/     //RD930引用
#define	IMPCOEF_CX_DEF	(short)-52		/*		ＣＸ	 0*8000H			*/     //RD930引用
#define	IMPCOEF_CR_DEF_5	(ushort)32723
										/*  5kHzＣＲ	 1*8000H			*/     //RD930引用
#define	IMPCOEF_CX_DEF_5	(short)-33	/*		ＣＸ	 0*8000H			*/     //RD930引用
//kkkk
//imp02 chg end
#define	IMPCOEF_CR_MIN	(ushort)26214	/*		ＣＲ最小 					*/
#define	IMPCOEF_CR_MAX	(ushort)39321	/*		ＣＲ最大 					*/
#define	IMPCOEF_CX_MIN	(short)-6554	/*		ＣＸ最小 					*/
#define	IMPCOEF_CX_MAX	(short)6554		/*		ＣＸ最大 					*/
#define	IMPCOEF_MIN		(uchar)0		/*		最小数						*/
#define	IMPCOEF_MAX		(uchar)2		/*		最大数						*/

#define	IMP_AVG			(long)(120 - 1)	/* 		平均範囲(±12Ω未満)		*/
#define	IMP_STB			(long)(10 - 1)	/* 		安定範囲(±1Ω未満)			*/
#define	IMP_REF			(long)8200		/* 		内部参照抵抗(820Ω)			*/

//imp03 allchg start
										/* インピオープン判定				*/
//kkkk 仮の値なので変更の必要あり
//#define	IMP_IN_OPEN_MIN		(float)+2.1E+9F
//										/* インピオープン内部 最小値 400Ω	*/
//#define	IMP_IN_OPEN_MAX		(float)+1.9E+10F
//										/* 					  最大値1.2kΩ	*/
//										/*   820Ωref:+8.9E+9F(50kHz)		*/
//#define	IMP_OUT_OPEN_MIN	(float)+3.1E+7F
//										/* 		オープン外部  最小値50Ω	*/
//#define	IMP_OUT_OPEN_MAX	(float)+3.2E+10F
//										/* 					  最大値1.6kΩ	*/
//										/*   820Ωref:+8.5E+9F(50kHz)		*/
//
//#define	IMP_IN_OPEN_MIN_5	(float)+9.1E+7F
//										/* インピオープン内部 最小値 400Ω	*/
//#define	IMP_IN_OPEN_MAX_5	(float)+8.5E+8F
//										/* 					  最大値1.2kΩ	*/
//										/*   820Ωref:+8.9E+9F(5kHz)		*/
//#define	IMP_OUT_OPEN_MIN_5	(float)+1.2E+6F
//										/* 		オープン外部  最小値50Ω	*/
//#define	IMP_OUT_OPEN_MAX_5	(float)+1.6E+9F
//										/* 					  最大値1.6kΩ	*/
//										/*   820Ωref:+8.5E+9F(5kHz)		*/
//kkkk
										/* ※FR-200L 型試基板にて計測		*/
#define	IMP_IN_OPEN_MIN		(float)+2.1E+9F
										/* インピオープン内部 最小値 400Ω	*/
#define	IMP_IN_OPEN_MAX		(float)+2.0E+10F
										/* 					  最大値1.2kΩ	*/
										/*   820Ωref:+8.9E+9F(50kHz)		*/
#define	IMP_OUT_OPEN_MIN	(float)+3.2E+7F
										/* 		オープン外部  最小値50Ω	*/
#define	IMP_OUT_OPEN_MAX	(float)+3.4E+10F
										/* 					  最大値1.6kΩ	*/
										/*   820Ωref:+8.8E+9F(50kHz)		*/

#define	IMP_IN_OPEN_MIN_5	(float)+9.6E+7F
										/* インピオープン内部 最小値 400Ω	*/
#define	IMP_IN_OPEN_MAX_5	(float)+8.8E+8F
										/* 					  最大値1.2kΩ	*/
										/*   820Ωref:+4.1E+8F(5kHz)		*/
#define	IMP_OUT_OPEN_MIN_5	(float)+1.4E+6F
										/* 		オープン外部  最小値50Ω	*/
#define	IMP_OUT_OPEN_MAX_5	(float)+1.6E+9F
										/* 					  最大値1.6kΩ	*/
										/*   820Ωref:+4.1E+8F(5kHz)		*/
//imp03 allchg end

#define	IMP_ADJ_RANGE		(short)100	/* 調整時基準抵抗範囲	10Ω		*/

#define	IMP_ADJ_800_MAX		(short)(8000 + IMP_ADJ_RANGE)
										/* 		調整時基準抵抗	800Ω	最大*/
#define	IMP_ADJ_800_MIN		(short)(8000 - IMP_ADJ_RANGE)
										/* 								最小*/
#define	IMP_ADJ_200_MAX		(short)(2000 + IMP_ADJ_RANGE)
										/* 						200Ω	最大*/
#define	IMP_ADJ_200_MIN		(short)(2000 - IMP_ADJ_RANGE)
										/* 								最小*/

#define	IMP_STB_MEAS		(uchar)7	/* 測定時安定検出回数				*/
#define	IMP_STB_MEAS_STPON	(uchar)3	/* ステップON時安定検出回数			*/
#define	IMP_STB_TEST		(uchar)3	/* 検査時安定検出回数				*/
#define	IMP_STB_ADJ			(uchar)32	/* 調整時安定検出回数				*/
#define	IMP_CHGCNT			(uchar)16	/* 切替回数							*/
#define	IMP_WAIT_DSP		(uchar)160	/* 表示待ち回数（ステップON）		*/
//imp03 add start
#define	IMP_WAIT_OPEN		(uchar)16	/* オープン表示待ち回数				*/
//imp03 add end
#define	IMP_WAIT_MEAS		(uchar)64	/* 測定待ち回数						*/
#define	IMP_WAIT_ADJ		(uchar)64	/* 調整安定待ち回数					*/
#define	IMP_WAIT_MAX		(uchar)255	/* 安定待ち回数最大					*/



/****************************************************************************/
/*		テーブル定義														*/
/****************************************************************************/
struct	IMP_T							/* インピ情報						*/
{
	long	lIM;						/*	虚部データ						*/
	long	lRE;						/*	実部データ						*/
};

struct	IMP_CONV_T						/* インピ変換						*/
{
	ushort	usCOEFA;					/*		係数Ａ						*/
	short	sCOEFB;						/*		係数Ｂ(２の補数)			*/
};

struct	IMP_CALB_T						/* インピ調整						*/
{
	short	sCR;						/*		ＣＲ						*/
	short	sCX;						/*		ＣＸ						*/
	short	sOSR;						/*		ＯＳＲ						*/
	short	sOSX;						/*		ＯＳＸ						*/
};

struct	MEAS_IMP_T						/* インピ情報						*/
{
	ushort	usZDT;						/*	インピーダンス					*/
	short	sRDT;						/*	レジスタンス					*/
	short	sXDT;						/*	リアクタンス					*/
};




/****************************************************************************/
/*		外部宣言															*/
/****************************************************************************/
extern	void	InitImpConv(void);		/* インピ補正係数初期化				*/
extern	uchar	ChkImpBase(uchar);		/* インピ基準抵抗範囲チェック		*/
extern	void	SetImp(struct IMP_T *, struct IMP_T *);
										/* インピ情報設定					*/
extern	enum EVT_ID	MeasImp(void);		/* インピ測定						*/
extern	void	ConvImp(void);			/* インピ補正						*/
extern	uchar	AdjImp(struct IMP_CALB_T *, struct IMP_T *, struct IMP_T *);
										/* インピ調整						*/
//ble02 del start
//extern	uchar	ChkConvImp(uchar);		/* インピ補正有効チェック			*/
//ble02 del end
extern	ulong	GetImpZ(long, long);	/* Ｚ取得							*/


extern	void	s_memset(void*, uchar, size_t);

