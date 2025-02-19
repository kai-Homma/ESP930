/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	at_def.h										*/
/*																			*/
/*		モジュール名称	：	定数定義										*/
/*																			*/
/*		作成年月日		：	2022/ 2/15	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 2/15	小山		key01					*/
/*								キー配置変更								*/
/*							2022/ 2/15	小山		spec01					*/
/*								仕様変更（個人番号数、デフォルト年齢）		*/
/*							2022/ 2/21	小山		setup01					*/
/*								初期設定モード削除							*/
/*							2022/ 2/21	小山		pset01					*/
/*								個人設定モード削除							*/
/*							2022/ 2/21	小山		item01					*/
/*								項目表示モード変更（表示は無し）			*/
/*							2022/ 2/22	小山		step01					*/
/*								ステップオン機能削除						*/
/*							2022/ 3/ 3	小山		ble01					*/
/*								BLE通信仕様・データ変更						*/
/*							2022/ 4/ 4	小山		ble02					*/
/*								BLE通信で調整・検査							*/
/*							2022/ 5/12	小山		voice01					*/
/*								音声追加									*/
/*							2022/ 5/16	久保田		fcj01					*/
/*								フレイル代理計算追加						*/
/*							2022/ 6/23	小山		init01					*/
/*								初期化モード追加							*/
/*							2022/ 6/24	久保田		tmphmd02				*/
/*								温湿度測定追加								*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*		汎用																*/
/****************************************************************************/
#define	FLOAT_ULONG_MAX	(float)4.293E+9F/* float⇒ulongキャスト最大値		*/
#define	FLOAT_ROUND		(float)5E-1F	/* float⇒整数キャスト丸め値		*/

#define	ON				(uchar)1
#define	OFF				(uchar)0
#define	OK				(uchar)1 //変更
#define	NG				(uchar)0
#define	YES				(uchar)1
#define	NO				(uchar)0
#define	SINGLE			(uchar)1
#define	DUAL			(uchar)2
#define	READY			(uchar)0
#define	BUSY			(uchar)1 //変更
#define	ILAST			(uchar)0
#define	ITHIS			(uchar)1
#define	RST				(uchar)2
/* fcj01 add start															*/
#define	TRUE			(uchar)1
#define	FALSE			(uchar)0
/* fcj01 add end															*/


/****************************************************************************/
/*		共通	※コンパイルエラー回避の為,本ファイルで定義					*/
/****************************************************************************/
enum	COUNTRY_ID						/* 向け先							*/
{
	COUNTRY_MIN,						/*		最小						*/
	COUNTRY_US = COUNTRY_MIN,			/*		ＵＳＡ						*/
	COUNTRY_EU,							/*		ＥＵ						*/
	COUNTRY_AS,							/*		アジア						*/
	COUNTRY_JP,							/*		日本						*/
	COUNTRY_MAX							/*		最大						*/
};
#define	COUNTRY_DEF		COUNTRY_JP		/*		デフォルト					*/

enum	UNIT_ID							/* 単位								*/
{
	UNIT_MIN,							/*		最小						*/
	UNIT_KG = UNIT_MIN,					/*		ｋｇ						*/
	UNIT_LB,							/*		ｌｂ						*/
	UNIT_STLB,							/*		ｓｔ－ｌｂ					*/
	UNIT_MAX							/*		最大						*/
};
#define	UNIT_DEF		UNIT_KG			/*		デフォルト					*/

//key01 add start
enum	VOL_ID							/* 音量								*/
{
	VOL_MIN,							/*		最小						*/
	VOL_OFF = VOL_MIN,					/*		OFF							*/
	VOL_LO,								/*		音量小						*/
	VOL_HI,								/*		音量大						*/
	VOL_MAX								/*		最大						*/
};
//key01 add end

/* ble01 add start															*/
										/* 通信仕様							*/
#define	COMSPEC_NONE	(uchar)0x00		/*		無し						*/
#define	COMSPEC_BLE		(uchar)0x01		/*		BLE							*/
#define	COMSPEC_WLAN	(uchar)0x02		/*		WLAN						*/

#define	BLE_MASK		(uchar)0x01		/*		BLEマスク					*/
#define	WLAN_MASK		(uchar)0x02		/*		WLANマスク					*/
#define	COM_MASK		(uchar)0x03		/*		通信マスク					*/
/* ble01 add end															*/

										/* 個人番号							*/
#define	PNUM_1		(uchar)0			/*		1							*/
#define	PNUM_2		(uchar)1			/*		2							*/
#define	PNUM_3		(uchar)2			/*		3							*/
#define	PNUM_4		(uchar)3			/*		4							*/
/* spec01 chg start															*/
/*#define	PNUM_5		(uchar)4		 *		5							*/
/*#define	PNUM_MAX	PNUM_5			 *		最大番号	5				*/
#define	PNUM_MAX	PNUM_4				/*		最大番号	4				*/
/* spec01 chg end															*/
#define	PREG_MAX	(uchar)(PNUM_MAX + (uchar)1)
										/*		最大人数					*/
#define	PNUM_GUEST	PREG_MAX			/*		ゲスト						*/
#define	PNUM_STPON	(uchar)(PNUM_GUEST + (uchar)1)
										/*		ステップON(個人番号未確定)	*/
#define	PNUM_WEIT	(uchar)(PNUM_STPON + (uchar)1)
										/*		体重測定					*/

#define	PSID_INIT		(uchar)0		/* ＩＤ無し							*/
#define	PSID_SETMIDL_AGE	(uchar)0x51	/*	設定途中ＩＤ	年齢(生年)済	*/
#define	PSID_SETMIDL_SEX	(uchar)0x52	/*					性別済			*/
#define	PSID_SETMIDL_HEIT	(uchar)0x53	/*					身長済			*/
#define	PSID_SETMIDL_AL		(uchar)0x54	/*					生活強度済		*/
#define	PSID_SET		(uchar)0x55		/* ＩＤ有り							*/

										/* 体型								*/

enum	FIG_ID							/* 		ＩＤ						*/
{
	FIG_MAN,							/*	0				男性			*/
	FIG_WOMAN,							/*	1				女性			*/
	FIG_GENDER,							/*	2				性別			*/
	FIG_AT_MAN = FIG_GENDER,			/*	2				アスリート男性	*/
	FIG_AT_WOMAN,						/*	3				アスリート女性	*/
	FIG_GENDER_AT,						/*	4				アスリート		*/
	FIG_BOY = FIG_GENDER_AT,			/*	4				男児			*/
	FIG_GIRL,							/*	5				女児			*/
	FIG_NUM,							/*	6				単体数			*/
	FIG_US_MAN = FIG_MAN,				/*	0		欧米	男性			*/
	FIG_US_WOMAN,						/*	1				女性			*/
	FIG_US_AT_MAN,						/*	2				アスリート男性	*/
	FIG_US_AT_WOMAN,					/*	3				アスリート女性	*/
	FIG_US_BOY,							/*	4				男児			*/
	FIG_US_GIRL,						/*	5				女児			*/
	FIG_AS_MAN,							/*	6		アジア	男性			*/
	FIG_AS_WOMAN,						/*	7				女性			*/
	FIG_AS_AT_MAN,						/*	8				アスリート男性	*/
	FIG_AS_AT_WOMAN,					/*	9				アスリート女性	*/
	FIG_AS_BOY,							/*	10				男児			*/
	FIG_AS_GIRL,						/*	11				女児			*/
	FIG_JP_MAN,							/*	12		日本	男性			*/
	FIG_JP_WOMAN,						/*	13				女性			*/
	FIG_JP_AT_MAN,						/*	14				アスリート男性	*/
	FIG_JP_AT_WOMAN,					/*	15				アスリート女性	*/
	FIG_JP_BOY,							/*	16				男児			*/
	FIG_JP_GIRL,						/*	17				女児			*/
	FIG_ALL								/*	18				全体数			*/
};

#define	SEX_MASK		(uchar)0x01		/*		性別マスク					*/
#define	AT_MASK			(uchar)0x02		/*		アスリートマスク			*/
#define	FIG_MASK		(uchar)0x03		/*		体型マスク					*/

#define	FIG_MIN			FIG_MAN			/*		最小						*/
#define	FIG_MAX			FIG_WOMAN		/*		最大						*/
#define	FIG_AT_MAX		FIG_AT_WOMAN	/*		アスリート最大				*/
#define	FIG_DEF			FIG_WOMAN		/*		デフォルト					*/

#define	AGE_MIN				(uchar)0	/* 年齢		最小値					*/
#define	AGE_MIN_ISCAN_US	(uchar)5	/*						ＵＳＡ		*/
#define	AGE_MIN_ISCAN_EU	(uchar)5	/*						ＥＵ		*/
#define	AGE_MIN_ISCAN_AS	(uchar)5	/*						アジア		*/
#define	AGE_MIN_ISCAN_JP	(uchar)6	/*						日本		*/
#define	AGE_MIN_AGE7		(uchar)7	/*						7才			*/
#define	AGE_ADLT			(uchar)18	/*			成人:18才				*/
#define	AGE_ADLT_US_EU		(uchar)16	/*			回帰式成人	ＵＳＡ&ＥＵ	*/
#define	AGE_ADLT_AS_JP		(uchar)15	/*						アジア&日本	*/
/* spec01 chg start															*/
/*#define	AGE_DEF			(uchar)40	 *			デフォルト:40才			*/
#define	AGE_DEF				(uchar)50	/*			デフォルト:50才			*/
/* spec01 chg end															*/
#define	AGE_MAX_ISCAN		(uchar)99	/*			体組成測定最大:99才		*/
#define	AGE_MAX				(uchar)255	/*			最大値					*/

										/* 単位カウント分解能				*/
#define	IN_PER_FT			(ushort)12	/*	12in/ft		12インチ＝1フィート	*/
										/*						＝約30.5cm	*/
#define CNT_PER_CM_DSP		(ushort)100	/*	100cnt/1cm	1cnt＝0.01cm		*/
#define CNT_PER_CM_DSP2		(ushort)10	/*	10cnt/1cm	1cnt＝0.1cm			*/
#define CNT_PER_IN_DSP		(ushort)254	/*	254cnt/in	25.4cnt＝0.1インチ	*/
#define CNT_PER_IN_DSP2		(ushort)10	/*	10cnt/in	1cnt＝0.1インチ		*/
#define CNT_PER_FT_DSP		(ushort)(IN_PER_FT * CNT_PER_IN_DSP)
										/*	3048cnt/ft	3048cnt＝1フィート	*/

										/* 身長								*/
#define	HEIT_CM_MIN			(ushort)(90 * CNT_PER_CM_DSP)
										/*		最小:90cm(9000)				*/
#define	HEIT_CM_MIN_US		(ushort)(100 * CNT_PER_CM_DSP)
										/*		USA最小:100cm(10000)		*/
#define	HEIT_CM_MAX			(ushort)(220 * CNT_PER_CM_DSP)
										/*		最大:220cm(22000)			*/
#define	HEIT_CM_DEF			(ushort)(170 * CNT_PER_CM_DSP)
										/*		デフォルト:170cm(17000)		*/
#define	HEIT_CM_DEF_JP		(ushort)(160 * CNT_PER_CM_DSP)
										/*		日本デフォルト:160cm(16000)	*/
#define	HEIT_CM_TEST		(ushort)(170 * CNT_PER_CM_DSP)
										/*		検査モード用:170.0cm(17000)	*/
//spec01 chg start
//#define	HEIT_CM_NOTCH		(ushort)(FLOAT_ROUND * CNT_PER_CM_DSP)
//										/*		刻み幅:0.5cm(50)			*/
#define	HEIT_CM_NOTCH		(ushort)10	/*		刻み幅:0.1cm(10)			*/
//spec01 chg end
#define	HEIT_CM_NOTCH_HI	(ushort)(1 * CNT_PER_CM_DSP)
										/*		刻み幅(高速時):1.0cm(100)	*/
#define	HEIT_FI_MIN			(ushort)(3 * CNT_PER_FT_DSP)
										/*		最小:3ft0in(9144)			*/
#define	HEIT_FI_MIN_US		(ushort)((3 * CNT_PER_FT_DSP) + (4 * CNT_PER_IN_DSP))
										/*		USA最小:3ft4in(10160)		*/
#define	HEIT_FI_MAX			(ushort)((7 * CNT_PER_FT_DSP) + (3 * CNT_PER_IN_DSP))
										/*		最大:7ft3in(22098)			*/
#define	HEIT_FI_DEF			(ushort)((5 * CNT_PER_FT_DSP) + (7 * CNT_PER_IN_DSP))
										/*		デフォルト:5ft7in(17018)	*/
#define	HEIT_IN_NOTCH		(ushort)(FLOAT_ROUND * CNT_PER_IN_DSP)
										/*		刻み幅:0.5in(127)			*/
#define	HEIT_IN_NOTCH_HI	(ushort)(FLOAT_ROUND * CNT_PER_IN_DSP)
										/*		刻み幅:0.5in(127)			*/

enum	AL_ID							/* 生活強度							*/
{
	AL_LO,								/*			低						*/
	AL_MID,								/*			中						*/
	AL_HI,								/*			高						*/
	AL_MAX								/*			最大					*/
};
#define	AL_DEF			AL_LO			/*			デフォルト				*/
#define	AL_ATH			AL_HI			/*			アスリート				*/

#define	NAME_LEN_LONG	(uchar)10		/* 名前		データ長（ロング）		*/
#define	NAME_LEN_SHORT	(uchar)5		/*			データ長（ショート）	*/
#define	NAME_LEN_NONE	(uchar)0		/*			データ長（0（なし））	*/

										/* SNS設定							*/
#define	SNS_DEF		OFF					/*			デフォルト				*/

										/* 脈拍測定							*/
#define	PLSMEAS_DEF		OFF				/*			デフォルト				*/

enum	MSTS_ID							/* メイン状態						*/
{
	MSTS_POFF,							/*	電源OFF							*/
	MSTS_PON,							/*	電源ON							*/
	MSTS_SAVE,							/*	データ保存						*/
	MSTS_NTC,							/*	通知画面　※データ保存はここから*/
//init01 add start
	MSTS_INIT,							/*	初期化							*/
//init01 add end
	MSTS_PAIR,							/*	ペアリング						*/
	MSTS_SND,							/*	データ送信						*/
	MSTS_SETUP,							/*	初期設定						*/
//pset01 del start
//	MSTS_PSET,							/*	個人設定						*/
//pset01 del end
//weit02 del start
//	MSTS_WEIT,							/*	体重測定						*/
//weit02 del end
	MSTS_ISCAN,							/*	体組成測定						*/
	MSTS_ITEM,							/*	測定項目				ここまで*/
//ble02 del start
//	MSTS_CNT,							/*	カウントモード					*/
//	MSTS_ADJ,							/*	調整モード						*/
//ble02 del end
	MSTS_MEM,							/*	メモリモード（調整解除モード）	*/
	MSTS_BLETEST,						/*	BLE通信検査モード				*/
//ble02 del start
//	MSTS_IMPCHK,						/*	インピ検査モード				*/
//ble02 del end
	MSTS_TEST							/*	テストモード					*/
};

enum	SSTS_ID							/* サブ状態							*/
{
	SSTS_INIT = 0,						/*	初期(共通)						*/

										/* 電源OFF							*/
/* step01 chg start															*/
/*	SSTS_POFF_STPON = SSTS_INIT + 1,	 *	ステップON						*/
/*	SSTS_POFF_STPOFF,					 *	ステップOFF						*/
/*	SSTS_POFF_IMP,						 *	インピ							*/
/*	SSTS_POFF_ZERO,						 *	０点							*/
	SSTS_POFF_NORM = SSTS_INIT + 1,		/*	通常							*/
/* step01 chg end															*/
	SSTS_POFF_MAX,						/*	最大							*/

										/* 電源ON							*/
	SSTS_PON_NORM = SSTS_INIT + 1,		/*	通常							*/
	SSTS_PON_FN0,						/*	機能0	無効					*/
	SSTS_PON_FN1,						/*	機能1	無効					*/
	SSTS_PON_FN2,						/*	機能2	無効					*/
/* key01 chg start															*/
/*	SSTS_PON_FN3_IMP,					 *	機能3	インピ検査モード		*/
/*	SSTS_PON_FN4_MEM,					 *	機能4	メモリモード			*/
/*	SSTS_PON_FN5_ISCAN,					 *	機能5	脂肪率確認				*/
/*	SSTS_PON_FN6_BLETEST,				 *	機能6	BLE通信検査				*/
/*	SSTS_PON_FN7_WEIT,					 *	機能7	器差確認				*/
/*	SSTS_PON_SET_PAIR,					 *	ペアリング						*/
/*	SSTS_PON_SET_AREA,					 *	地域設定						*/
/*	SSTS_PON_SET_TIME,					 *	時計設定						*/
	SSTS_PON_FN3,						/*	機能3							*/
	SSTS_PON_FN4,						/*	機能4							*/
	SSTS_PON_FN5,						/*	機能5							*/
	SSTS_PON_FN6,						/*	機能6							*/
	SSTS_PON_SET_PAIR,					/*	ペアリング						*/
/* key01 chg end															*/
//init01 add start
	SSTS_PON_INIT,						/*	初期化							*/
//init01 add end
	SSTS_PON_MAX,						/*	最大							*/

										/* データ保存						*/
	SSTS_SAVE_MAX = SSTS_INIT + 1,		/*	最大							*/

										/* 通知								*/
//voice01 chg start
//	SSTS_NTC_EXE = SSTS_INIT + 1,		/*	実行							*/
	SSTS_NTC_BUZZ = SSTS_INIT + 1,		/*	ブザー通知						*/
	SSTS_NTC_EXE,						/*	実行（音声通知）				*/
//voice01 chg end
	SSTS_NTC_MAX,						/*	最大							*/

//init01 add start
										/* 初期化							*/
	SSTS_INIT_STEP1 = SSTS_INIT + 1,	/*	操作1待ち						*/
	SSTS_INIT_STEP2,					/*	操作2待ち						*/
	SSTS_INIT_STEP3,					/*	操作3待ち						*/
	SSTS_INIT_STEP4,					/*	操作4待ち						*/
//t024 add start
	SSTS_INIT_DEL_1,					/*	個人1消去待ち					*/
	SSTS_INIT_DEL_2,					/*	個人2消去待ち					*/
	SSTS_INIT_DEL_3,					/*	個人3消去待ち					*/
	SSTS_INIT_DEL_4,					/*	個人4消去待ち					*/
	SSTS_INIT_DEL_EXE,					/*	消去実行						*/
//t024 add end
	SSTS_INIT_EXE,						/*	初期化実行						*/
	SSTS_INIT_MAX,						/*	最大							*/
//init01 add end
										/* ペアリング						*/
	SSTS_PAIR_DATE = SSTS_INIT + 1,		/*	日時取得						*/
	SSTS_PAIR_SYS,						/*	機器情報取得					*/
/* ble01 add start															*/
	SSTS_PAIR_PSNUM,					/*	個人番号						*/
//t023 add start
	SSTS_PAIR_CLR_WAIT,					/*	個人上書き確認					*/
//t023 add end
/* ble01 add end															*/
	SSTS_PAIR_PSINF,					/*	個人情報						*/
	SSTS_PAIR_WT_UUID,					/*	ＵＵＩＤ書込					*/
	SSTS_PAIR_ISCAN,					/*	体組成測定						*/
	SSTS_PAIR_START_MEAS,				/*	測定開始						*/
//ble01 del start
//	SSTS_PAIR_NUM,						/*	番号							*/
//ble01 del end
	SSTS_PAIR_CNF,						/*	ペアリング結果確認				*/
	SSTS_PAIR_MAX,						/*	最大							*/

										/* データ送信						*/
	SSTS_SND_CHK_UUID = SSTS_INIT + 1,	/*	ＵＵＩＤ確認					*/
	SSTS_SND_DATE,						/*	日時取得						*/
	SSTS_SND_SYS,						/*	機器情報取得					*/
	SSTS_SND_PSINF,						/*	個人情報						*/
	SSTS_SND_ISCAN,						/*	体組成測定						*/
	SSTS_SND_START_MEAS,				/*	測定開始						*/
//ble01 del start
//	SSTS_SND_NUM,						/*	番号							*/
//ble01 del end
	SSTS_SND_NONSND,					/*	未送信データ					*/
	SSTS_SND_CNF,						/*	送信結果確認					*/
//fcj01 add start
	SSTS_SND_FRJD,						/*	フレイル判定代理計算			*/
//fcj01 add end
//ble01 add start
	SSTS_SND_PSET_DEL,					/*	個人消去						*/
//ble01 add end
	SSTS_SND_MAX,						/*	最大							*/

//setup01 del start
//										/* 初期設定							*/
//	SSTS_SETUP_AREA = SSTS_INIT + 1,	/*	地域							*/
//	SSTS_SETUP_YEAR,					/*	年								*/
//	SSTS_SETUP_MONTH,					/*	月								*/
//	SSTS_SETUP_DAY,						/*	日								*/
//	SSTS_SETUP_HOUR,					/*	時								*/
//	SSTS_SETUP_MIN,						/*	分								*/
//	SSTS_SETUP_CNF_AREA,				/*	地域確認						*/
//	SSTS_SETUP_CNF_YEAR,				/*	年確認							*/
//	SSTS_SETUP_CNF_MD,					/*	月日確認						*/
//	SSTS_SETUP_CNF_HM,					/*	時分確認						*/
//	SSTS_SETUP_ZERO_WAIT,				/*	０点待ち						*/
//	SSTS_SETUP_MAX,						/*	最大							*/
//setup01 del end
//pset01 del start
//										/* 個人設定							*/
//	SSTS_PSET_NUM = SSTS_INIT + 1,		/*	番号							*/
//	SSTS_PSET_AGE,						/*	年齢							*/
//	SSTS_PSET_YEAR,						/*	年								*/
//	SSTS_PSET_MONTH,					/*	月								*/
//	SSTS_PSET_DAY,						/*	日								*/
//	SSTS_PSET_SEX_WAIT,					/*	性別待ち						*/
//	SSTS_PSET_SEX,						/*	性別/体型						*/
//	SSTS_PSET_HEIT,						/*	身長							*/
//	SSTS_PSET_HEIT_WRITE,				/*	身長書込み						*/
//	SSTS_PSET_AL,						/*	生活強度						*/
//	SSTS_PSET_AL_WRITE,					/*	生活強度書込み					*/
//	SSTS_PSET_CLR_WAIT,					/*	消去待ち						*/
//	SSTS_PSET_CLR,						/*	消去選択						*/
//	SSTS_PSET_CNF_CLR,					/*	消去確認						*/
//	SSTS_PSET_CNF_AGE,					/*	年齢確認						*/
//	SSTS_PSET_CNF_HEIT,					/*	身長確認						*/
//	SSTS_PSET_CNF_AL,					/*	生活強度確認					*/
//	SSTS_PSET_DEL_AGE,					/*	削除年齢						*/
//	SSTS_PSET_DEL_HEIT,					/*	削除身長						*/
//	SSTS_PSET_DEL_AL,					/*	削除生活強度					*/
//	SSTS_PSET_DEL_WAIT,					/*	削除待ち						*/
//	SSTS_PSET_DEL_SEL,					/*	削除選択						*/
//	SSTS_PSET_DEL_CNF,					/*	削除確認						*/
//	SSTS_PSET_DEL_CNF_BAR,				/*	削除バー確認					*/
//	SSTS_PSET_MAX,						/*	最大							*/
//pset01 del end
//weit02 del start
//										/* 体重測定							*/
//	SSTS_WEIT_ZERO = SSTS_INIT + 1,		/*	0点待ち							*/
//	SSTS_WEIT_OFF,						/*	2kg未満							*/
//	SSTS_WEIT_ON,						/*	2kg以上							*/
//	SSTS_WEIT_HOLD,						/*	ホールド						*/
//	SSTS_WEIT_MAX,						/*	最大							*/
//weit02 del end

										/* 体組成測定						*/
//pset01 chg start
//	SSTS_ISCAN_SEL_AGE = SSTS_INIT + 1,	/*	年齢選択						*/
//	SSTS_ISCAN_SEL_HEIT,				/*	身長選択						*/
//	SSTS_ISCAN_SEL_AL,					/*	生活強度選択					*/
//	SSTS_ISCAN_ZERO_AGE,				/*	年齢０点待ち					*/
//	SSTS_ISCAN_ZERO_HEIT,				/*	身長０点待ち					*/
//	SSTS_ISCAN_ZERO_AL,					/*	生活強度０点待ち				*/
//	SSTS_ISCAN_AGE,						/*	年齢							*/
//	SSTS_ISCAN_HEIT,					/*	身長							*/
//	SSTS_ISCAN_AL,						/*	生活強度						*/
	SSTS_ISCAN_ZERO = SSTS_INIT + 1,	/*	０点待ち						*/
//pset01 chg end
//t026 add start
	SSTS_ISCAN_ERR_PNUM,				/*	個人未登録通知					*/
//t026 add end
//weit01 add start
	SSTS_ISCAN_MINOL,					/*	マイナスＯＬ					*/
//weit01 add end
	SSTS_ISCAN_OFF,						/*	体重測定2kg未満					*/
	SSTS_ISCAN_ON,						/*	体重測定2kg以上					*/
	SSTS_ISCAN_IMP,						/*	インピ							*/
//imp03 add start
	SSTS_ISCAN_IMPOPEN,					/*	インピオープン					*/
//imp03 add end
//step01 del start
//	SSTS_ISCAN_STPOFF_WAIT,				/*	降り待ち						*/
//	SSTS_ISCAN_STPOFF_ZERO,				/*	降りてからの０点取得			*/
//	SSTS_ISCAN_STPOFF_DACOR,			/*	降りてからのデータ補正			*/
//step01 del end
	SSTS_ISCAN_MAX,						/*	最大							*/

										/* 測定項目							*/
//item01 chg start
//	SSTS_ITEM_PNUMNONE = SSTS_INIT + 1,	/*	個人情報なし					*/
//	SSTS_ITEM_SEL_AGE,					/*	年齢選択						*/
//	SSTS_ITEM_SEL_HEIT,					/*	身長選択						*/
//	SSTS_ITEM_SEL_AL,					/*	生活強度選択					*/
//	SSTS_ITEM_AGE,						/*	年齢							*/
//	SSTS_ITEM_HEIT,						/*	身長							*/
//	SSTS_ITEM_AL,						/*	生活強度						*/
//	SSTS_ITEM_MYSUP,					/*	マイサポ表示					*/
//	SSTS_ITEM_NONSND,					/*	未送信データ数					*/
//	SSTS_ITEM_ITEM,						/*	測定項目						*/
	SSTS_ITEM_STPOFF_WAIT = SSTS_INIT + 1,
										/*	降り待ち						*/
	SSTS_ITEM_ITEM,						/*	測定項目						*/
	SSTS_ITEM_END,						/*	終了							*/
//item01 chg end
	SSTS_ITEM_MAX,						/*	最大							*/

//ble02 del start
//										/* カウントモード					*/
//	SSTS_CNT_WT_ZERO = SSTS_INIT + 1,	/*	荷重0点							*/
//	SSTS_CNT_WT_WAIT,					/*	荷重待ち						*/
//	SSTS_CNT_WT,						/*	荷重							*/
//	SSTS_CNT_TEMP,						/*	温度							*/
//	SSTS_CNT_Z,							/*	Ｚ								*/
//	SSTS_CNT_R,							/*	Ｒ								*/
//	SSTS_CNT_X,							/*	Ｘ								*/
/* step01 del start															*/
/*	SSTS_CNT_STPON,						 *	ステップON						*/
/* step01 del end															*/
//	SSTS_CNT_MAX,						/*	最大							*/
//
//										/* 調整モード						*/
//	SSTS_ADJ_DP_ALL = SSTS_INIT + 1,	/*	全表示							*/
//	SSTS_ADJ_SPECB,						/*	基本仕様						*/
//	SSTS_ADJ_SPECEX,					/*	拡張仕様						*/
//	SSTS_ADJ_ZERO_START,				/*	0点ポート設定開始				*/
//
//	SSTS_ADJ_ZERO_PORT,					/*	0点ポート設定					*/
//	SSTS_ADJ_ZERO_WAIT,					/*	0点待ち							*/
//	SSTS_ADJ_ZERO_FIX,					/*	0点確定							*/
//	SSTS_ADJ_MIN_FIX,					/*	最小値g確定						*/
//	SSTS_ADJ_HALF_FIX,					/*	中間値確定						*/
//	SSTS_ADJ_ALLWT_FIX,					/*	秤量確定						*/
//
//	SSTS_ADJ_IMP_START,					/*	抵抗補正開始					*/
//	SSTS_ADJ_IMP800,					/*	抵抗補正800Ω待ち				*/
//	SSTS_ADJ_IMP200,					/*	抵抗補正200Ω待ち				*/
//
//	SSTS_ADJ_MAX,						/*	最大							*/
//ble02 del end

										/* メモリモード（調整解除モード）	*/
//ble02 chg start
//	SSTS_MEM_DSP_DAT = SSTS_INIT + 1,	/*	データ表示						*/
//	SSTS_MEM_DSP_ADR,					/*	アドレス表示					*/
//	SSTS_MEM_EDIT,						/*	編集							*/
//	SSTS_MEM_WRI1,						/*	書込1							*/
//	SSTS_MEM_WRI2,						/*	書込2							*/
//	SSTS_MEM_WRI3,						/*	書込3							*/
	SSTS_MEM_START = SSTS_INIT + 1,		/*	調整解除モード開始				*/
	SSTS_MEM_SOFTINF,					/*	機器ソフトウェア情報取得待ち	*/
//ble02 chg end
	SSTS_MEM_MAX,						/*	最大							*/

										/* BLE通信検査モード				*/
/* ble02 chg start															*/
/*	SSTS_BLETEST_SEL_PLACE = SSTS_INIT + 1,									*/
/*										 *	検査場所選択					*/
/*	SSTS_BLETEST_SEL_LNUM,				 *	ライン番号選択					*/
/*	SSTS_BLETEST_SOFTINF,				 *	機器ソフトウェア情報取得		*/
/*	SSTS_BLETEST_ACK,					 *	acknowledge通知					*/
/*	SSTS_BLETEST_DISCONECT,				 *	切断							*/
/*	SSTS_BLETEST_RESULT,				 *	結果表示						*/
/*	SSTS_BLETEST_PSINF,					 *	個人情報						*/
/*	SSTS_BLETEST_MAX,					 *	最大							*/
//
//										/* インピ検査モード					*/
//	SSTS_IMPCHK_800 = SSTS_INIT + 1,	/*	800Ωの測定						*/
//	SSTS_IMPCHK_200,					/*  200Ωの測定						*/
//	SSTS_IMPCHK_WAIT_200,				/*  検査治具回路切替待ち			*/
//	SSTS_IMPCHK_END,					/*	チェック終了					*/
//	SSTS_IMPCHK_MAX,					/*	最大							*/

	SSTS_BLE_ADJ_WT = SSTS_INIT + 1,	/*	器差調整モード			0x01	*/
	SSTS_BLE_ADJ_IMP,					/*	インピ調整モード		0x02	*/
	SSTS_BLE_MEM,						/*	メモリモード			0x03	*/
	SSTS_BLE_CNT,						/*	カウントモード			0x04	*/
	SSTS_BLE_DSP,						/*	表示目確認モード		0x05	*/
	SSTS_BLE_TEST_PCB,					/*	基板検査モード			0x06	*/
	SSTS_BLE_TEST_IMP,					/*	インピ検査モード		0x07	*/
	SSTS_BLE_TEST_VOICE,				/*	音声確認モード			0x08	*/
	SSTS_BLE_START,						/*	調整・検査モード開始			*/
	SSTS_BLE_SOFTINF,					/*	機器ソフトウェア情報取得待ち	*/
	SSTS_BLE_MAIN,						/*	メインスタンバイモード			*/
//test02 add start
	SSTS_BLE_TEST_LED,					/*	LED検査モード					*/
//test02 add end
										/*  ※ここからLED駆動100%			*/
	SSTS_BLE_ADJ_ZERO_PORT,				/*	0点ポート設定					*/
	SSTS_BLE_ADJ_ZERO_WAIT,				/*	0点待ち							*/
	SSTS_BLE_ADJ_ZERO_FIX,				/*	0点確定							*/
	SSTS_BLE_ADJ_MIN_FIX,				/*	最小値g確定						*/
	SSTS_BLE_ADJ_HALF_FIX,				/*	中間値確定						*/
	SSTS_BLE_ADJ_ALLWT_FIX,				/*	秤量確定						*/
	SSTS_BLE_ADJ_IMP800,				/*	抵抗補正800Ω待ち				*/
	SSTS_BLE_ADJ_IMP200,				/*	抵抗補正200Ω待ち				*/
	SSTS_BLE_IMPCHK_800,				/*	800Ωの測定						*/
	SSTS_BLE_IMPCHK_200,				/*  200Ωの測定						*/
	SSTS_BLE_PCB_ZERO,					/*	基板検査	0点チェック			*/
	SSTS_BLE_PCB_IMP,					/*	基板検査	インピチェック		*/
										/*  ※ここまで						*/

	SSTS_BLE_PCB_KEY,					/*	基板検査	キーチェック		*/

	SSTS_BLETEST_MAX,					/*	最大							*/
/* ble02 chg end															*/

										/* テストモード						*/
//ble02 chg start
//	SSTS_TEST_ZERO = SSTS_INIT + 1,		/*  0点チェック						*/
//	SSTS_TEST_IMP,						/*  インピチェック					*/
//	SSTS_TEST_BLE_SOFTINF,				/*	BLEﾁｪｯｸ(機器ｿﾌﾄｳｪｱ情報取得)		*/
//	SSTS_TEST_BLE_ACK,					/*	BLEﾁｪｯｸ(acknowledge通知)		*/
//	SSTS_TEST_BLE_DISCONECT,			/*	BLEﾁｪｯｸ(切断)					*/
//	SSTS_TEST_EXE_WAIT,					/*  (キーチェック)実行待ち			*/
//	SSTS_TEST_EXE,						/*	(キーチェック)実行				*/
	SSTS_TEST_EXE = SSTS_INIT + 1,		/*	(キーチェック)実行				*/
//ble02 chg end
	SSTS_TEST_MAX						/*	最大							*/
};

enum	EVT_ID							/* 割込みイベント					*/
{										/*					プライオリティ高*/
	EVT_POFF,							/*	電源OFF					↑	0	*/
	EVT_FINIT,							/*	ファンクション初期		↑	1	*/
	EVT_TMR0,							/*	タイマ0満了				↑	2	*/
	EVT_TMR1,							/*	タイマ1満了				↑	3	*/
	EVT_TMR2,							/*	タイマ2満了				↑	4	*/
	EVT_TMR3,							/*	タイマ3満了				↑	5	*/
	EVT_TMR4,							/*	タイマ4満了				↑	6	*/
	EVT_TMR5,							/*	タイマ5満了				↑	7	*/
	EVT_TMRCLK,							/*	時計タイマ満了			↑	8	*/
	EVT_VOLT,							/*	電圧					↑	9	*/
	EVT_LOBAT = EVT_VOLT,				/*	低電圧					〃	9 	*/
	EVT_TEMP,							/*	温度					↑	10	*/
	EVT_IMP,							/*	インピ					↑	11	*/
	EVT_WEIT,							/*	荷重					↑	12	*/
//step01 del start
//	EVT_ZEROTMR,						/*	0点タイマ要求			↑	13	*/
//	EVT_ZEROTEMP,						/*	0点温度要求				↑	14	*/
//	EVT_STPTMR,							/*	ステップタイマ要求		↑	15	*/
//	EVT_STPWEIT,						/*	ステップ荷重			↑	16	*/
//step01 del end
/* tmphmd02 add start														*/
	EVT_TMPHMDTMR,						/*	温湿度タイマ			↑	13	*/
/* tmphmd02 add end															*/
	EVT_BLESTART,						/*	BLE通信開始				↑	14	*/
	EVT_BLEEND,							/*	BLE通信終了				↑	15	*/
	EVT_BLEERR,							/*	BLE通信エラー			↑	16	*/
	EVT_BLERCV,							/*	BLE受信完了				↑	17	*/
	EVT_KEY,							/*	キー					↑	18	*/
	EVT_MAX,							/*	割込みイベント総数			19	*/

/* key01 chg start															*/
/*	EVT_KEY_WEIT_OFF = EVT_KEY,			 *	体重/OFFキーOFF			↑	21	*/
/*	EVT_KEY_UP_OFF,						 *	UPキーOFF				↑	22	*/
/*	EVT_KEY_DOWN_OFF,					 *	DOWNキーOFF				↑	23	*/
/*	EVT_KEY_SET_OFF,					 *	SETキーOFF				↑	24	*/
/*																			*/
/*	EVT_KEY_WEIT,						 *	体重/OFFキー			↑	25	*/
/*	EVT_KEY_UP,							 *	UPキー					↑	26	*/
/*	EVT_KEY_DOWN,						 *	DOWNキー				↑	27	*/
/*	EVT_KEY_SET,						 *	SETキー					↑	28	*/
	EVT_KEY_SW1_OFF = EVT_KEY,			/*	個人1キーOFF			↑	18	*/
	EVT_KEY_SW2_OFF,					/*	個人2キーOFF			↑	19	*/
	EVT_KEY_SW3_OFF,					/*	個人3キーOFF			↑	20	*/
	EVT_KEY_SW4_OFF,					/*	個人4キーOFF			↑	21	*/
	EVT_KEY_PAIR_OFF,					/*	ペアリングキーOFF		↑	22	*/
	EVT_KEY_VOICE_OFF,					/*	音声音量キーOFF			↑	23	*/
	EVT_KEY_BUZZ_OFF,					/*	ブザー音量キーOFF		↑	24	*/

	EVT_KEY_SW1,						/*	個人1キー				↑	25	*/
	EVT_KEY_SW2,						/*	個人2キー				↑	26	*/
	EVT_KEY_SW3,						/*	個人3キー				↑	27	*/
	EVT_KEY_SW4,						/*	個人4キー				↑	28	*/
	EVT_KEY_PAIR,						/*	ペアリングキー			↑	29	*/
	EVT_KEY_VOICE,						/*	音声音量キー			↑	30	*/
	EVT_KEY_BUZZ,						/*	ブザー音量キー			↑	31	*/
/* key01 chg end															*/

	EVT_NOP								/*	無効イベント	プライオリティ低*/
};

