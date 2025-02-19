/****************************************************************************/
/*																			*/
/*							���̑g���v�EFR-200L��							*/
/*																			*/
/*		�t�@�C����		�F	at_init.h										*/
/*																			*/
/*		���W���[������	�F	�����ݒ�										*/
/*																			*/
/*		�쐬�N����		�F	2022/ 2/15	���R		BC333L_V10�x�[�X		*/
/*																			*/
/*		�ύX����		�F	2022/ 2/15	���R		ver01					*/
/*								�o�[�W�����i���o�[�ύX						*/
/*																			*/
/*					Copyright 2014-2022 TANITA Corporation					*/
/*																			*/
/****************************************************************************/

// #define DI      __DI()					/* 		�����֎~					*/
// #define EI      __EI()					/* 		��������					*/
// #define HALT    __halt()				/* 		�E�F�C�g					*/
// #define STOP    __stop()				/* 		�X�g�b�v					*/
// #define NOP     __nop()					/* 		������						*/

										/* typedef��`						*/
typedef	unsigned	char	uchar;		/*	1byte 0�`255					*/
typedef	unsigned	short	ushort;		/*	2byte 0�`65535					*/
typedef	unsigned	long	ulong;		/*	4byte 0�`4294967295				*/
typedef	const		char	const_c;	/*	1byte -128�`127					*/
typedef	const		short	const_s;	/*	2byte -32768�`32767				*/
typedef	const		long	const_l;	/*	4byte -2147483648�`2147483647	*/
typedef	const		float	const_f;	/*	4byte |1.17E-38F�`3.39E+38F|	*/
typedef	const unsigned char	 const_uc;	/*	1byte 0�`255					*/
typedef	const unsigned short const_us;	/*	2byte 0�`65535					*/
typedef	const unsigned long	 const_ul;	/*	4byte 0�`4294967295				*/
typedef	signed		char	schar;		/*	1byte -128�`127					*/
										/* �R���p�C���̎d�l���char�^�͕���	*/
										/* �Ŏg�p�̂��ƁB1Byte�̕����t�����l*/
										/* �́Asigned��t���邱��			*/

										/* �����R���p�C��					*/


										/* �}�C�R���o�[�W����				*/
/* ver01 allchg start														*/
// #define	VERSION			(uchar)0x10	 	/*	Ver1.0	�ʎY					*/
//#define	VERSION			(uchar)0x08	 	/*	Ver0.8	�����e�X�g��			*/
										/*	Ver0.7	�����e�X�g��			*/
										/*	Ver0.6	�����e�X�g��			*/
										/*	VerB.B	�����x�������p		*/
										/*	Ver0.5	�����e�X�g��			*/
										/*	Ver0.4	�F�����فE�����e�X�g��	*/
										/*	Ver0.3	�ʎ��E�����e�X�g��		*/
										/*	Ver0.2	�ʎ��E�����e�X�g��		*/
										/*	Ver0.1	�ʎ��E�����e�X�g��		*/
										/*	Ver0.0	�ʎ��E�����e�X�g��		*/
										/*	Ver0.F	�ʎ��E�����e�X�g�J�n	*/
										/*	Ver0.E	�����[�h�����e�X�g�J�n	*/
										/*	Ver0.D	�^����(���؋Ǝ҂�)		*/
										/*	Ver0.C	�^����(�H�c��)			*/
										/*	Ver0.B	�^����(���؋Ǝ҂�)		*/
										/*	Ver0.A	�^����					*/
										/*	Ver0.9	�^���J�n��				*/
										/*	Ver0.8	����m�F�p				*/
										/*	Ver0.7	����T���v���p			*/
										/*	Ver0.6	����m�F�p				*/
										/*	Ver0.5	����m�F�p				*/
										/*	Ver0.4	����m�F�p�i�^����j	*/
										/*	Ver0.3	�������ٗp(���؋Ǝ҂�)	*/
										/*	Ver0.2	�������ٗp				*/
										/*	Ver0.1	����T���v���p(����)	*/
/* ver01 allchg end															*/

