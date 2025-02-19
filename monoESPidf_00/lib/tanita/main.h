// ***** ユーザ設定 *****
#define KEY_WT                  15              //GPIO割り込みピン IO15 が負エッジでトリガ
#define KEY_DOWN                16              //GPIO割り込みピン IO15 が負エッジでトリガ
#define EXAMPLE_ADC_UNIT        ADC_UNIT_1      // IO10 => ADC1
#define EXAMPLE_ADC_CHANNEL     ADC_CHANNEL_9   // IO10 => CH9
#define EXAMPLE_SAMPLE_FREQ     49000          // 20.4us間隔 
// #define SAMPLES_PER_BURST       25              // 1バーストあたりのサンプル数
// #define BURST_COUNT             8               // バーストを繰り返す回数 (=8バースト)
// PWM定義
#define LEDC_TIMER_SPEED_MODE  LEDC_LOW_SPEED_MODE // ESP32-S3ではLOW_SPEED_MODEを使用
#define LEDC_TIMER_NUM         LEDC_TIMER_0
#define LEDC_CHANNEL           LEDC_CHANNEL_0
#define PWM_FREQ_HZ            4000000  // 4.04MHz
// 出力ピン定義
#define PIN_39    39   // IO39 -> Low ref
#define PIN_40    40   // IO40 -> Low 50kHz
#define PIN_42     42   // IO42 -> High start
#define PIN_4MHZ     41   // IO41 -> 4MHz PWM
#define PIN_47     47   // IO47 -> VCC ON

// LCD1602
#define LCD_NUM_ROWS               2
#define LCD_NUM_COLUMNS            32
#define LCD_NUM_VISIBLE_COLUMNS    16

// LCD2004
//#define LCD_NUM_ROWS               4
//#define LCD_NUM_COLUMNS            40
//#define LCD_NUM_VISIBLE_COLUMNS    20

// Undefine USE_STDIN if no stdin is available (e.g. no USB UART) - a fixed delay will occur instead of a wait for a keypress.
#define USE_STDIN  0
//#undef USE_STDIN

#define I2C_MASTER_NUM           I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN    0                     // disabled
#define I2C_MASTER_RX_BUF_LEN    0                     // disabled
#define I2C_MASTER_FREQ_HZ       100000
#define I2C_MASTER_SDA_IO        3
#define I2C_MASTER_SCL_IO        2

#define CONFIG_LCD1602_I2C_ADDRESS 0x27

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif




struct	HSRAM_WT_T						/* 荷重情報							*/
{
	ulong	ulSUM;						/*	積算データ						*/
	ulong	ulDATA;						/*	確定データ						*/
	ushort	usDELAY;					/*	遅延時間						*/
	uchar	ucINITCNT;					/*	初期化回数						*/
	uchar	ucSUMCNT;					/*	積算回数						*/
};

struct	HSRAM_SAR_T						/* 逐次比較情報						*/
{
	ulong	ulSUM;						/*	積算データ						*/
	ushort	usVOLT;						/*	電圧データ						*/
	ushort	usTEMP;						/*	温度データ						*/
/* step01 del start															*/
/*	ushort	usSTPON;					 *	ステップONデータ				*/
/* step01 del end															*/
	uchar	ucINITCNT;					/*	初期化回数						*/
	uchar	ucSUMCNT;					/*	積算回数						*/
};

struct	HSRAM_ADC_T						/* ADC情報							*/
{
	struct	HSRAM_WT_T	WT;				/*	荷重情報						*/
	// struct	HSRAM_SAR_T	SAR;			/*	逐次比較情報					*/
	uchar	ucSTS;						/*	状態							*/
	uchar	ucDUMMY;					/*	空き							*/
};

struct	HSRAM_T							/* 高速ＲＡＭ情報					*/
{
	struct	HSRAM_ADC_T	ADC;			/*	ADC情報							*/
};

struct	ADC_IMP_SUM_T					/* インピ積算情報					*/
{
	ulong	ulDATA[IMP_DATA_NUM];		/*	データ							*/
	uchar	ucDIZCNT;					/*	偏移電圧カウンタ				*/
	uchar	ucDUMMY;					/*	空き							*/
};

struct	ADC_IMP_STORE_T					/* インピ蓄積情報					*/
{
	long	lIM[ADC_IMP_STORE_MAX];		/*	虚部データ						*/
	long	lRE[ADC_IMP_STORE_MAX];		/*	実部データ						*/
};

struct	ADC_IMP_T						/* インピ情報						*/
{
	struct	ADC_IMP_SUM_T	SUM;		/*	積算情報						*/
	struct	ADC_IMP_STORE_T	STORE;		/*	インピ蓄積情報					*/
	struct	IMP_T	LAST[IMP_OBJ_MAX];	/*	インピ前回情報					*/
	struct	IMP_T	THIS[IMP_OBJ_MAX];	/*	インピ今回情報					*/
};

union	ADC_T							/* ADC情報							*/
{
	struct	ADC_IMP_T	IMP;			/*	インピ							*/
};

struct	MEAS_T							/* 測定情報							*/
{
	ushort	usTEMP;						/*	温度							*/
	ushort	usTEMPCNT;					/*	温度カウント					*/
	ulong	ulZERO;						/*	0点								*/
	ulong	ulLAST;						/*	前回値							*/
	ulong	ulWEIT;						/*	荷重							*/
/* step01 del start															*/
/*	ulong	ulSTPWEIT;					 *	ステップオン荷重（比較用）		*/
/* step01 del end															*/
	struct	MEAS_IMP_T	IMPSRC[IMP_FREQ_MAX];
										/*	生インピ情報(1経路)				*/
	struct	MEAS_IMP_T	IMP[IMP_FREQ_MAX];
										/*	インピ情報(1経路)				*/
	uchar	ucSTS;						/*	状態							*/
	uchar	ucCNT;						/*	測定回数						*/
	uchar	ucAVGCNT;					/*	平均回数						*/
	uchar	ucSTBCNT;					/*	安定回数						*/
	uchar	ucOBJ;						/*	対象							*/
/* ble02 chg start															*/
/*	uchar	ucDUMMY;					 *	空き							*/
	uchar	ucCNTSTS;					/*	カウント状態					*/
/* ble02 chg end															*/
//ble01 add start
	ushort	usMEASTIME_WEIT;			/*	測定時間	体重				*/
	ushort	usMEASTIME_IMP;				/*				インピーダンス		*/
//ble01 add end
};

struct	SYS_INF_T						/* システム情報						*/
{
										/*	システム情報1（仕様）			*/
	// uchar	ucDUMMY0[2];				/*		空き						*/
	// uchar	ucCALBWT;					/*		器差調整ＩＤ				*/
	// uchar	ucSYSID;					/*		システムＩＤ				*/
	// // union	SPECB_T		SPECB;			/*		基本仕様					*/
	// // union	SPECEX_T	SPECEX;			/*		拡張仕様					*/
	// ushort	usGRAV;						/*		調整地重力加速度			*/
	// uchar	ucGRAV_TO;					/*		向け先地域番号				*/
	// enum	COUNTRY_ID	COUNTRY;		/*		向け先回帰式				*/
	// uchar	ucSTPON;					/*		ステップON閾値（未使用）	*/
	// uchar	ucDUMMY1;					/*		空き						*/
	// ushort	usMODEL;					/*		ISMNモデルコード			*/
	// char	cMODEL[0];	/*		モデル名＝Device Name		*/

										/*	システム情報2（係数）			*/
	struct	IMP_CONV_T	IMPCONV;		/*		インピ変換					*/
	struct	IMP_CONV_T	IMPCONV_5;		/*		インピ変換(5kHz)			*/
	// short	sTEMPCOEF;					/*		温度補正係数				*/

	// 									/*	システム情報3（器差調整）		*/
	// ushort	usTEMPADJ;					/*		調整時温度					*/
	// ushort	usSPAN;						/*		スパン補正量				*/
	// short	sLIN;						/*		直線性補正量				*/
	// schar	scCALBWT3P;					/*		3点調整値					*/
	// uchar	ucZEROP;					/*		0点ポート					*/

										/*	システム情報4（インピ調整）		*/
	struct	IMP_CALB_T	IMPCALB;		/*		インピ調整値				*/
	uchar	ucIMPCHKSUM;				/*		インピチェックサム			*/
	uchar	ucDUMMY2;					/*		空き						*/
	struct	IMP_CALB_T	IMPCALB_5;		/*		インピ調整値(5kHz)			*/
	uchar	ucIMPCHKSUM_5;				/*		インピチェックサム(5kHz)	*/
	uchar	ucDUMMY3;					/*		空き						*/

// 										/*	システム情報5（ユーザー情報）	*/
// 	uchar	ucAREA;						/*		地域						*/
// 	enum	UNIT_ID	SAVE_UNIT;			/*		保存単位					*/
// 	ulong	ulMEASCNT;					/*		測定回数（調整後）			*/
// 	ulong	ulMEASCNT_TOTAL;			/*		測定回数（累計）			*/
// 	uchar	ucBDADR[0];	/*		ＢＤアドレス				*/
// /* ble01 add start															*/
// //err01 add start
// 	// struct	CLOCK_T	ERRINF;				/*		エラー年月日時分秒			*/
// //err01 add end
// 	uchar	ucERRINF;					/*		エラー情報					*/
// 	uchar	ucLASTCOM_STS;				/*		前回通信時の状態			*/
// 	// struct	CLOCK_T	LASTCOM;			/*		前回通信年月日時分秒		*/
// /* ble01 add end															*/
// 	uchar	ucADJLOG[1];			/*		ログ						*/

// 										/*	一時領域(12byte)				*/
// 	uchar	ucMODE;						/*		モード						*/
	uchar	ucIMPCONVSTS;				/*		インピ変換状態				*/
// 	uchar	ucPARAM;					/*		パラメータ					*/
// 	uchar	ucPNUM;						/*		個人番号					*/
// 	char	cERRNO;						/*		エラー番号					*/
// 	enum	UNIT_ID	UNIT;				/*		単位						*/
// 	uchar	ucBLEVER;					/*		BLEモジュールバージョン		*/
// 	uchar	ucRENEW;					/*		個人情報更新				*/
// //ble02 add start
// 	char	cLINE;						/*		ライン番号					*/
// 	uchar	ucDUMMY4;					/*		空き						*/
// //ble02 add end
// //key01 add start
// 	enum	VOL_ID	VOICE;				/*		音声音量					*/
// 	enum	VOL_ID	BUZZ;				/*		ブザー音量					*/
//key01 add end
};
/* spec01 allchg end*/

// struct	MEM_SYS_INF_T					/* メモリ用システム情報				*/
// {
// 	uchar	ucSPID[0];		/*	ＩＤ仕様領域					*/
// 	uchar	ucSPCOEF[0];	/*	係数仕様領域					*/
// 	uchar	ucCSCL[0];		/*	器差調整領域					*/
// 	uchar	ucCIMP[0];		/*	インピ調整領域					*/
// 	uchar	ucUSER[0];		/*	ユーザー領域					*/
// };

union	SYS_T							/* システム情報共用体				*/
{
	struct	SYS_INF_T		INF;		/*	システム情報					*/
	// struct	MEM_SYS_INF_T	BLK;		/*	ブロック分け用					*/
};

//func
union	CALB_T							/* 調整情報							*/
{
	// struct	CALB_WT_T	WT;				/*	荷重							*/
	struct	IMP_T		IMP[IMP_BASE_MAX][IMP_OBJ_MAX];
//unit06 del start
//	struct	IMP_T		IMP_5[IMP_BASE_MAX][IMP_OBJ_MAX];
//unit06 del end
										/*	インピ							*/
};

struct	ADJ_T							/* 調整モード						*/
{
	union	CALB_T	CALB;				/*	調整情報						*/
//step01 del start
//	ushort	usSTEPCONTI;				/*	連続VccステップONカウント		*/
//step01 del end
// 	uchar	ucCNT;						/*	カウンタ						*/
// 	uchar	ucKEYSTS;					/*	キー状態						*/
//ble02 del start
//	enum	MSTS_ID	CHGSTS;				/*	遷移先状態						*/
//	uchar	ucSDDATA;					/*	ＳＤデータ						*/
//ble02 del end
};

union	FUNC_T							/* ファンクション情報				*/
{
//setup01 del start
//	struct	SETUP_T	SETUP;				/*	初期設定						*/
//setup01 del end
	// struct	PSET_T	PSET;				/*	個人設定						*/
//item01 del start
//	struct	ITEM_T	ITEM;				/*	測定項目						*/
//item01 del end
//	struct	CNT_T	CNT;				/*	カウントモード					*/
	struct	ADJ_T	ADJ;				/*	調整モード						*/
	// struct	FMEM_T	MEM;				/*	メモリモード					*/
//	struct	TEST_T	TEST;				/*	検査モード						*/
};


/****************************************************************************/
/*		外部宣言															*/
/****************************************************************************/
extern	union	FUNC_T	FUNC;			/* ファンクション情報				*/

/****************************************************************************/
/*		外部宣言															*/
/****************************************************************************/
extern	struct	HSRAM_T	HSRAM;			/* 高速ＲＡＭ情報					*/
extern	union	ADC_T	ADC;			/* ADC情報							*/
extern	struct	MEAS_T	MEAS;			/* 測定情報							*/

// extern	struct	MTR_T	MTR;			/* 監視情報							*/
extern	union	SYS_T	SYS;			/* システム情報						*/
extern	const	union	SYS_T	SYS_INIT;
										/* システム情報初期化テーブル		*/
