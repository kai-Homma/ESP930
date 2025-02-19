/****************************************************************************/
/*																			*/
/*							＜体組成計・FR-200L＞							*/
/*																			*/
/*		ファイル名		：	at_init.h										*/
/*																			*/
/*		モジュール名称	：	初期設定										*/
/*																			*/
/*		作成年月日		：	2022/ 2/15	小山		BC333L_V10ベース		*/
/*																			*/
/*		変更履歴		：	2022/ 2/15	小山		ver01					*/
/*								バージョンナンバー変更						*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

// #define DI      __DI()					/* 		割込禁止					*/
// #define EI      __EI()					/* 		割込解除					*/
// #define HALT    __halt()				/* 		ウェイト					*/
// #define STOP    __stop()				/* 		ストップ					*/
// #define NOP     __nop()					/* 		無処理						*/

										/* typedef定義						*/
typedef	unsigned	char	uchar;		/*	1byte 0～255					*/
typedef	unsigned	short	ushort;		/*	2byte 0～65535					*/
typedef	unsigned	long	ulong;		/*	4byte 0～4294967295				*/
typedef	const		char	const_c;	/*	1byte -128～127					*/
typedef	const		short	const_s;	/*	2byte -32768～32767				*/
typedef	const		long	const_l;	/*	4byte -2147483648～2147483647	*/
typedef	const		float	const_f;	/*	4byte |1.17E-38F～3.39E+38F|	*/
typedef	const unsigned char	 const_uc;	/*	1byte 0～255					*/
typedef	const unsigned short const_us;	/*	2byte 0～65535					*/
typedef	const unsigned long	 const_ul;	/*	4byte 0～4294967295				*/
typedef	signed		char	schar;		/*	1byte -128～127					*/
										/* コンパイラの仕様よりchar型は文字	*/
										/* で使用のこと。1Byteの符号付き数値*/
										/* は、signedを付けること			*/

										/* 条件コンパイル					*/


										/* マイコンバージョン				*/
/* ver01 allchg start														*/
// #define	VERSION			(uchar)0x10	 	/*	Ver1.0	量産					*/
//#define	VERSION			(uchar)0x08	 	/*	Ver0.8	統合テスト中			*/
										/*	Ver0.7	統合テスト中			*/
										/*	Ver0.6	統合テスト中			*/
										/*	VerB.B	温湿度基板検査専用		*/
										/*	Ver0.5	統合テスト中			*/
										/*	Ver0.4	認定ｻﾝﾌﾟﾙ・統合テスト中	*/
										/*	Ver0.3	量試・統合テスト中		*/
										/*	Ver0.2	量試・統合テスト中		*/
										/*	Ver0.1	量試・統合テスト中		*/
										/*	Ver0.0	量試・統合テスト中		*/
										/*	Ver0.F	量試・統合テスト開始	*/
										/*	Ver0.E	裏モード統合テスト開始	*/
										/*	Ver0.D	型試中(ｱﾌﾟﾘ業者へ)		*/
										/*	Ver0.C	型試中(秋田へ)			*/
										/*	Ver0.B	型試中(ｱﾌﾟﾘ業者へ)		*/
										/*	Ver0.A	型試中					*/
										/*	Ver0.9	型試開始時				*/
										/*	Ver0.8	動作確認用				*/
										/*	Ver0.7	動作サンプル用			*/
										/*	Ver0.6	動作確認用				*/
										/*	Ver0.5	動作確認用				*/
										/*	Ver0.4	動作確認用（型試基板）	*/
										/*	Ver0.3	動作ｻﾝﾌﾟﾙ用(ｱﾌﾟﾘ業者へ)	*/
										/*	Ver0.2	動作ｻﾝﾌﾟﾙ用				*/
										/*	Ver0.1	動作サンプル用(初版)	*/
/* ver01 allchg end															*/

