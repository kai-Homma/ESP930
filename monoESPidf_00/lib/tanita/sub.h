/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	sub.h											*/
/*																			*/
/*		モジュール名称	：	共通関数ヘッダ									*/
/*																			*/
/*		作成年月日		：	2022/ 2/25	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 2/25	小山		rev01					*/
/*								回帰式表変更（B41XDJ⇒B42(B)J）筋質点数追加	*/
/*							2022/ 2/25	小山		key01					*/
/*								キー配置変更								*/
/*							2022/ 2/25	小山		pset01					*/
/*								個人設定モード削除							*/
/*							2022/ 4/19	小山		lcd01					*/
/*								LCD削除										*/
/*							2022/ 5/ 9	小山		item01					*/
/*								測定項目変更								*/
/*																			*/
/*					Copyright 2009-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*		定数定義															*/
/****************************************************************************/
										/* カウンタ制御						*/
#define	CNT_UP				(uchar)1	/*		カウントアップ				*/
#define	CNT_DOWN			(uchar)0xFF	/*		カウントダウン				*/


/****************************************************************************/
/*		テーブル定義														*/
/****************************************************************************/
struct	HTROUND_TBL_T					/* 身長丸めテーブル					*/
{
	ushort	usMAX;						/*	最大値							*/
	ushort	usROUND[DUAL];				/*	丸め値							*/
};


/****************************************************************************/
/*		外部宣言															*/
/****************************************************************************/
extern	uchar	FtoS(short *, float);	/* float⇒short変換					*/
extern	char	ConvAbs(ushort*);		/* 絶対値変換						*/
extern	char	ConvAbsCast(ulong *, long);
										/* 絶対値変換キャスト				*/
extern	ushort	DivRoundS(ushort, ushort);
										/* 2バイトデータ除算(四捨五入)		*/
extern	ulong	DivRoundL(ulong, ulong);/* 4バイトデータ除算(四捨五入)		*/
extern	long	DivRoundSL(long, long);	/* 符号付き4バイト除算(四捨五入)	*/
extern	void	ImAdd(float *, float *, float, float);
										/* 複素数加算						*/
extern	void	ImMul(float *, float *, float, float);
										/* 複素数乗算						*/
extern	void	ImDiv(float *, float *, float, float);
										/* 複素数除算						*/
extern	ushort	RoundS(ushort, ushort);	/* 2バイトデータ丸め処理			*/
extern	ushort	RoundHt(ushort);		/* 身長丸め							*/
extern	ushort	RoundWt(ushort, uchar);	/* 荷重丸め							*/
extern	ushort	RoundFtoS(float, uchar);/* float⇒ushort丸め				*/
extern	uchar	StoreData(ulong, ulong *, ulong *);
										/* カウントデータ蓄積				*/
extern	void	StoreDataSL(long, long *, long *);
										/* long型カウントデータ蓄積			*/
extern	ulong	SumData(uchar, ulong *);/* カウントデータ積算				*/
extern	long	SumDataSL(uchar, long *);
										/* long型カウントデータ積算			*/
extern	ulong	AvgData(uchar);			/* カウントデータ平均				*/
extern	long	AvgDataSL(uchar, uchar, ushort, long *);
										/* long型カウントデータ平均			*/
extern	uchar	GetChkSum(void *, uchar);
										/* チェックサム取得					*/
//lcd01 del start
//extern	void	HexToDec(uchar, char *, uchar, uchar);
//										/* 16進⇒10進変換(uchar)			*/
//extern	void	HexToDecS(ushort, char *, uchar, uchar);
//										/* 16進⇒10進変換(ushort)			*/
//extern	void	HexToDecL(ulong, char *, uchar, uchar);
//										/* 16進⇒10進変換(ulong)			*/
//extern	void	HexToAscS(ushort, char *, uchar);
//										/* 16進⇒アスキー変換(ushort)		*/
//extern	void	HexToAsc(uchar *, char *, uchar);
//										/* 16進⇒アスキー変換(uchar)		*/
//lcd01 del end
extern	void	Conv8BitTo4Bit(uchar *, uchar *, uchar);
										/* 8ビット⇒4ビット					*/
extern	void	Conv4BitTo8Bit(uchar *, uchar *);
										/* 4ビット4バイト⇒8ビット2バイト	*/
extern	ushort	CalSub(ushort, ushort);	/* 差計算（ushort型データ）			*/
extern	ulong	CalSubL(ulong, ulong);	/* 差計算（ulong型データ）			*/
extern	ulong	RoundFtoL(float, uchar);/* float⇒ulong丸め					*/
extern	float	ConvfAbs(float);		/* 絶対値変換(float)				*/
extern	void	StsChg(enum MSTS_ID);	/* 状態遷移							*/
extern	uchar	StsChgTerm(enum MSTS_ID, uchar, uchar);
										/* 条件付き状態遷移					*/
extern	void	StsChgClr(enum MSTS_ID, uchar, uchar, uchar);
										/* 状態遷移＆初期化					*/
extern	void	StsChgErr(char);		/* エラー状態遷移					*/
extern	void	ChkSysinf(void);		/* システム情報チェック				*/
extern	void	SetSpid(const void *);	/* 仕様書込領域設定					*/

extern	uchar	GetArea(void);			/* 地域取得							*/
extern	uchar	ChkAscCode(char *, uchar);
										/* アスキーコードチェック			*/
extern	void	ChkPsinf(void);			/* 個人情報チェック					*/
//pset01 del start
//extern	void	ChkPsinfChg(void);		/* 個人情報変更チェック				*/
//pset01 del end
// extern	uchar	ChkPsDat(struct PSMEM_DAT_T *);
// 										/* 測定データチェック				*/
extern	uchar	ChkPs(void);			/* 個人情報簡易チェック				*/
extern	uchar	ChkPsId(uchar);			/* 個人ＩＤチェック					*/
extern	uchar	ChkAge(uchar);			/* 年齢チェック						*/
extern	uchar	GetAgeMin(void);		/* 年齢最小値取得					*/
extern	uchar	GetAgeMinMslBone(void);	/* 筋肉量/骨量年齢最小値取得		*/
extern	uchar	ChkAgeAdult(uchar);		/* 成年チェック						*/
extern	uchar	GetHtNotch(enum COUNTRY_ID);
										/* 身長目量取得						*/
extern	ushort	ChkHt(ushort);			/* 身長チェック						*/
extern	ushort	GetHtMin(enum UNIT_ID);	/* 身長最小値取得					*/
extern	ushort	GetHtMax(enum UNIT_ID);	/* 身長最大値取得					*/
extern	ushort	GetHtDef(enum UNIT_ID);	/* 身長デフォルト取得				*/
//spec01 del start
//extern	uchar	ChkAlEnable(uchar, uchar);
//										/* 生活強度有効チェック				*/
//spec01 del end
extern	uchar	ChkAl(enum AL_ID, uchar, uchar);
										/* 生活強度チェック					*/
extern	enum AL_ID	GetAlDef(uchar);	/* 生活強度デフォルト取得			*/
extern	uchar	ChkWt(ushort);			/* 体重チェック						*/
extern	ushort	GetWtMax(void);			/* 体重最大値取得					*/
// extern	void	SetPsAgeLimit(struct YMD_T *, uchar *, enum AL_ID *, uchar);
// 										/* 個人情報年齢制限設定				*/
extern	void	SetPsinfDef(uchar);		/* 個人情報デフォルト設定			*/
extern	void	SetPsinfInit(void);		/* 個人情報初期化					*/
extern	void	SetPsdatInit(void);		/* 個人測定データ初期化				*/
//pset01 del start
//extern	void	PsinfToPstmp(void);		/* 個人情報⇒一時領域				*/
//extern	void	PstmpToPsinf(void);		/* 一時領域⇒個人情報				*/
//pset01 del end
//key01 del start
//extern	void	CountUD(uchar);			/* カウンタ制御						*/
//key01 del end
// extern	void	LoadPsDat(enum PSTIME_ID);
// 										/* 測定データロード					*/
// extern	uchar	SavePsDat(enum PSTIME_ID);
// 										/* 測定データセーブ					*/
extern	void	SavePsRev(void);		/* 回帰式データセーブ				*/
//pset01 del start
//extern	void	SetLastPnum(void);		/* 最終呼出個人番号設定				*/
//pset01 del end
extern	void	GetPsinf(void);			/* 個人情報取得						*/

//item01 del start
//extern	void	SetPsdatThis(void);		/* 今回測定データ設定				*/
//item01 del end
/* rev01 add start															*/
//t020 del start
//extern	uchar	GetPsLastNum(void);		/* 測定データ件数取得				*/
//t020 del end
extern	void	ShiftPsDay(ushort, ushort, uchar);
										/* 個人日データシフト（筋質用）		*/
//extern	void	ShiftPsWtDay(ushort, ushort, ushort);
//										/* 個人日データシフト（体重急激用）	*/
/* rev01 add end															*/
extern	void	SetAdjLog(uchar);		/* 調整ログ設定						*/

extern	ushort	s_sqrt(ulong);			/*		平方根計算					*/



