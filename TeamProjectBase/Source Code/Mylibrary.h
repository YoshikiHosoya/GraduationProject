//*************************************************************************************************************
//
// 自分のライブラリの処理[Mylibrary.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include <stdio.h>
#include "XAudio2.h"
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"
#define _CRTDBG_MAP_ALLOC
#include <time.h>
#include <list>

//-------------------------------------------------------------------------------------------------------------
// 全体メモ
//-------------------------------------------------------------------------------------------------------------
/* コメント例 */
// * [contents]	-> 簡単な処理の中身
// * [output]	-> 出力
// * [input]	-> 入力
// * [return]	-> 返り値
/*
（operator）オーバーロードできる演算子
+		-		*		/		%		^
!		=		<		>		+=		-=
^=		&=		|=		<<		>>		<<=
<=		>=		&&		||		++		--
( )		[ ]		new		delete	&		|
~		*=		/=		%=		>>=		==
!=		,		->		->*
*/
/*
* [enm] enumの略
* [stm] statementの略
* [pIt] iteratorの略(ポインタ)
* [src] sourceの略
* [rhs] right-hand sideの略
* [lhs] left-hand sideの略
* [str] stringの略
* [tmp] temporaryの略
* [b]   bool型の略
* [t]   true(真)の略
* [f]   false(偽)の略
* [o]   outputの略
* [r]   resourceの略
*/

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define MYLIB_MEASURE												// 計測する

#define MYLIB_SUCCESS			0									// 成功
#define MYLIB_FAILURE			-1									// 失敗

#define MYLIB_TRUE				1									// 真
#define MYLIB_FALSE				0									// 偽

#define MYLIB_STRINGSIZE		128									// 文字列の基本の長さ

#define MYLIB_PNG				".png" ,".PNG"						// pngファイル
#define MYLIB_JPG				".jpg" ,".JPG"						// jpgファイル
#define MYLIB_JPEG				".jpeg" ,".JPEG"					// jpegファイル
#define MYLIB_IMAGE_EXTENSION	MYLIB_PNG ,MYLIB_JPG ,MYLIB_JPEG	// 一通りの画像ファイル

#define MYLIB_TXT				".txt" ,".TXT"						// テキストファイル
#define MYLIB_INI				".ini" ,".INI"						// 初期化ファイル
#define MYLIB_TXT_EXTENSION		MYLIB_TXT ,MYLIB_INI				// テキスト、初期化ファイル

#define MYLIB_X					".x" ,".X"							// Xファイル

#define MYLIB_CLEARARRAYONDECLA	{ NULL }							// 宣言時の配列をクリア

#define MYLIB_INITSTRUCT_WITHCONST	{ }								// コンストラクタのある構造体の初期化

#define MYLIB_D3DXCOR_UNSET			D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)	// 色設定なし
#define MYLIB_D3DXCOR_SET			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	// 色設定

#define MYLIB_3DVECTOR_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3Dベクトルをゼロに設定
#define MYLIB_SCALE_UNSET			D3DXVECTOR3(1.0f,1.0f,1.0f)		// スケールの初期化
#define MYLIB_VEC3_XONE				D3DXVECTOR3(1.0f,0.0f,0.0f)		// 3DベクトルXだけ1.0f
#define MYLIB_VEC3_YONE				D3DXVECTOR3(0.0f,1.0f,0.0f)		// 3DベクトルYだけ1.0f
#define MYLIB_VEC3_ZONE				D3DXVECTOR3(0.0f,0.0f,1.0f)		// 3DベクトルZだけ1.0f
#define MYLIB_VEC3_UNSET			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3Dベクトルの初期化

#define MYLIB_2DVECTOR_ZERO			D3DXVECTOR2(0.0f,0.0f)			// 2Dベクトルをゼロに設定
#define MYLIB_VEC2_XONE				D3DXVECTOR2(1.0f,0.0f)			// 2DベクトルXだけ1.0f
#define MYLIB_VEC2_YONE				D3DXVECTOR2(0.0f,1.0f)			// 2DベクトルYだけ1.0f
#define MYLIB_VEC2_UNSET			D3DXVECTOR2(0.0f,0.0f)			// 2Dベクトルの初期化

#define MYLIB_INT2_UNSET			INTEGER2(0,0)					// INT2設定なし

#define MYLIB_RANGE_UNSET			RANGE(0,0)						// レンジの設定なし

#define MYLIB_FLOAT_UNSET			(0.0f)							// float型の初期化
#define MYLIB_INT_UNSET				(0)								// int型の初期化
#define MYLIB_INT_NOELEM			(-1)							// int型要素なし
#define MYLIB_CHAR_UNSET			'\0'							// char型の初期化(文字列の一番目の初期化)
#define MYLIB_STRING_UNSET			{ MYLIB_CHAR_UNSET }			// 文字列の初期化

#define MYLIB_HALF_SIZE				(0.5f)							// 半分のサイズ
#define MYLIB_ONEQUARTER_SIZE		(0.25f)							// 4分の1のサイズ
#define MYLIB_THREEQUARTERS_SIZE	(0.75f)							// 4分の3のサイズ

#define MYLIB_VSHORT_UNSET			(0x000)							// 1バイト分の変数設定なし

#define MYLIB_ERRORCOUNT_MAX		(1048576)						// エラーカウント

#define MYLIB_VSHORT_UNSET			(0x000)							//

#define MYLIB_HALFSECONDCOUNT		(30)							// 1フレームカウント毎時のカウント

#define MYLIB_D3DXCORR_BIT			(0)								// 赤
#define MYLIB_D3DXCORG_BIT			(8)								// 緑
#define MYLIB_D3DXCORB_BIT			(16)							// 青
#define MYLIB_D3DXCORA_BIT			(24)							// 透明度

#define MYLIB_S8BYTES				(64)							// 文字列8バイト分
#define MYLIB_S4BYTES				(32)							// 文字列4バイト分

#define MYLIB_BRACKETS_L			"========================================================== \n"		// 区切りの括弧(大)
#define MYLIB_BRACKETS_M			"---------------------------------------------------------- \n"		// 区切りの括弧(中)
#define MYLIB_BRACKETS_S			"#*\n"																// 区切りの括弧(小)

#define MYLIB_OX_EPSILON			0.000001f							// 誤差

//-------------------------------------------------------------------------------------------------------------
// マクロ関数
//-------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define Mlf_Debug_brackets_L			std::cout << MYLIB_BRACKETS_L						// 区切りの括弧(大)の出力
#define Mlf_Debug_brackets_M			std::cout << MYLIB_BRACKETS_M						// 区切りの括弧(中)の出力
#define Mlf_Debug_brackets_S			std::cout << MYLIB_BRACKETS_S						// 区切りの括弧(小)の出力
#define Mlf_Debug_start(str)			std::cout << str << " == start!!\n"					// 開始(start!!)の出力
#define Mlf_Debug_end(str)				std::cout << str << " == end!!\n"					// 終了(end!!)の出力
#define Mlf_Debug_open(str)				std::cout << str << " == open!!\n"					// 開いた(open!!)の出力
#define Mlf_Debug_openfailed(str)		std::cout << str << " == open failed!!\n"			// 開くの失敗(open failed!!)の出力
#define Mlf_Debug_close(str)			std::cout << str << " == close!!\n"					// 閉じる(close!!)の出力
#define Mlf_Debug_error(str)			std::cout << "<<<<<"<< str << ">>>>>\n"				// エラーの出力
#define Mlf_ErrorMessa(title,str)		MessageBox(NULL,title,str "\nどのボタン押しても無視されます",MB_ABORTRETRYIGNORE|MB_ICONERROR)
#endif
#define Mlf_gate(b)						if(b)												// 扉:真の時 ※使わない
#define Mlf_iif(b)						(b == true) ? true: false							// 条件文から真偽を返す
#define Mlf_iifEx(b ,t ,f)				(b == true) ? t: f									// 条件文から真偽を返す(真偽引数)
#define Mlf_swap(lhs,rhs,tmp)			do{tmp = rhs; rhs = lhs; lhs = tmp;}while(false);	// 交換
#define Mlf_bit_clear(lhs,rhs)			(lhs &= ~(1 << rhs))								// ビットを消す
#define Mlf_bit_set(lhs,rhs)			(lhs |= (1 << rhs))									// ビットを立てる
#define Mlf_bit_comp(lhs,rhs)			(lhs & (1 << rhs))									// ビットを比較
#define Mlf_array(data)					{ data }											// 配列の初期化
#define Mlf_gt(data,hrs)				( data > hrs )										// 大なり
#define Mlf_ge(data,hrs)				( data >= hrs )										// 大なりイコール
#define Mlf_lt(data,hrs)				( data < hrs )										// 小なり
#define Mlf_le(data,hrs)				( data <= hrs )										// 小なりイコール
#define Mlf_OutRange(data, Max, Min)	( Mlf_gt(data, Max) || Mlf_lt(data, Min) )			// 範囲外
#define Mlf_OutIncRange(data, Max, Min)	( Mlf_ge(data, Max) || Mlf_le(data, Min) )			// 範囲外またはデータを含んだ
#define Mlf_InRange(data, Max, Min)		( Mlf_lt(data, Max) && Mlf_gt(data, Min) )			// 範囲内
#define Mlf_InIncRange(data, Max, Min)	( Mlf_le(data, Max) && Mlf_ge(data, Min) )			// 範囲内またはデータを含んだ

#define Mlf_GtSet(o ,src)				if(o > src) {o = src;}								// srcよりoが大きいとき設定する
#define Mlf_LtSet(o ,src)				if(o < src) {o = src;}								// srcよりoが小さいとき設定する
/* マクロキーワード */
#define ML_CASE(t)						break;case (t):										// ケース 条件
#define ML_DEFAULT						break;default:										// Switch文 それ以外
#define ML_CASEEND						break;default:break;								// ケース 最後

//-------------------------------------------------------------------------------------------------------------
// 列挙型定義
//-------------------------------------------------------------------------------------------------------------
// 頂点点の原点(対キーパッド)
typedef enum ORIGINVERTEXTYPE
{
	ORIGINVERTEXTYPE_NONE = 0,			// なし
	ORIGINVERTEXTYPE_LOWERLEFT,			// 左下
	ORIGINVERTEXTYPE_LOWERCENTER,		// 中央下
	ORIGINVERTEXTYPE_LOWERRIGHT,		// 右下
	ORIGINVERTEXTYPE_CENTERLEFT,		// 左中央
	ORIGINVERTEXTYPE_CENTER,			// 中央
	ORIGINVERTEXTYPE_CENTERRIGHT,		// 右中央
	ORIGINVERTEXTYPE_UPPERLEFT,			// 左上
	ORIGINVERTEXTYPE_UPPERCENTER,		// 中央上
	ORIGINVERTEXTYPE_UPPERRIGHT,		// 右上
	ORIGINVERTEXTYPE_ROTATION = 10,		// 回転中央
	ORIGINVERTEXTYPE_ROTLOWERLEFT,		// 回転左下
	ORIGINVERTEXTYPE_ROTLOWERCENTER,	// 回転中央下
	ORIGINVERTEXTYPE_ROTLOWERRIGHT,		// 回転右下
	ORIGINVERTEXTYPE_ROTCENTERLEFT,		// 回転左中央
	ORIGINVERTEXTYPE_ROTCENTER,			// 回転中央
	ORIGINVERTEXTYPE_ROTCENTERRIGHT,	// 回転右中央
	ORIGINVERTEXTYPE_ROTUPPERLEFT,		// 回転左上
	ORIGINVERTEXTYPE_ROTUPPERCENTER,	// 回転中央上
	ORIGINVERTEXTYPE_ROTUPPERRIGHT,		// 回転右上
	ORIGINVERTEXTYPE_MAX				// 最大数
} ORIGINVERTEXTYPE;

// 演算子ID
typedef enum
{
	OPERATOR_ID_NONE = -1,				// 無し
	OPERATOR_ID_ADDITION,				// 加算
	OPERATOR_ID_SUBTRACTION,			// 減算
	OPERATOR_ID_MULTIPLICATION,			// 乗算
	OPERATOR_ID_DIVISION,				// 除算
	OPERATOR_ID_MAX						// 最大数
} OPERATOR_ID;

//-------------------------------------------------------------------------------------------------------------
// 型、構造体定義
//-------------------------------------------------------------------------------------------------------------
typedef const char *       CONST_STRING;	// 変更不可文字列
typedef char *             STRING;			// 文字列

typedef signed char        VSHORT;			// 符号あり1バイト分の変数
typedef unsigned char      UVSHORT;			// 符号なし1バイト分の変数

typedef long long          VLONG;			// 符号あり8バイト分の変数
typedef unsigned long long UVLONG;			// 符号なし8バイト分の変数

typedef const char *       FILE_NAME;		// ファイル名
typedef unsigned int       FILE_SIZE;		// ファイルデータサイズ
typedef char *             FILE_DATA;		// ファイルデータ
typedef const char *       OPEN_MODE;		// ファイルを開くモード

typedef const INT          CINT;			// コンストINT
typedef const UINT         CUINT;			// コンスト符号なしINT
typedef const FLOAT        CFLOAT;			// コンストFLOAT

// ビットフィールド
typedef struct _BITS4
{
	_BITS4() {}										// コンストラクタ
	_BITS4(UVSHORT& rhs) : data(rhs){}				// コンストラクタ
	inline _BITS4& operator = (const int &rhs);		// 代入演算子
	union {
		UVSHORT data;
		struct {
			unsigned char _1 : 1;
			unsigned char _2 : 1;
			unsigned char _3 : 1;
			unsigned char _4 : 1;
		};
	};
}BITS4;

typedef struct _INTEGER2SOURCE
{
	int nX;	// 最大値
	int nY;	// 最小値
}INTEGER2SOURCE;

// 範囲
typedef struct _RANGE
{
	_RANGE() {}														// コンストラクタ
	_RANGE(const int nMax, const int nMin);							// コンストラクタ
	_RANGE(CONST INTEGER2SOURCE& rhs);								// コンストラクタ
	_RANGE(const int rhs);											// コンストラクタ

	inline _RANGE  operator + (void) const;							// 単項演算子 +
	inline _RANGE  operator - (void) const;							// 単項演算子 -

	inline _RANGE  operator + (CONST _RANGE& rhs) const;			// 四則演算子 +
	inline _RANGE  operator + (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 +
	inline _RANGE  operator + (CONST int rhs) const;				// 四則演算子 +
	inline _RANGE  operator - (CONST _RANGE& rhs) const;			// 四則演算子 -
	inline _RANGE  operator - (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 -
	inline _RANGE  operator - (CONST int rhs) const;				// 四則演算子 -
	inline _RANGE  operator * (CONST _RANGE& rhs) const;			// 四則演算子 *
	inline _RANGE  operator * (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 *
	inline _RANGE  operator * (CONST int rhs) const;				// 四則演算子 *
	inline _RANGE  operator / (CONST _RANGE& rhs) const;			// 四則演算子 /
	inline _RANGE  operator / (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 /
	inline _RANGE  operator / (CONST int rhs) const;				// 四則演算子 /

	inline bool    operator == (CONST _RANGE& rhs) const;			// 比較演算子 ==
	inline bool    operator == (CONST INTEGER2SOURCE& rhs) const;	// 比較演算子 ==
	inline bool    operator == (CONST int rhs) const;				// 比較演算子 ==
	inline bool    operator != (CONST _RANGE& rhs) const;			// 比較演算子 !=
	inline bool    operator != (CONST INTEGER2SOURCE& rhs) const;	// 比較演算子 !=
	inline bool    operator != (CONST int rhs) const;				// 比較演算子 !=

	inline _RANGE& operator += (CONST _RANGE& rhs);					// 代入演算子 +=
	inline _RANGE& operator += (CONST INTEGER2SOURCE& rhs);			// 代入演算子 +=
	inline _RANGE& operator += (CONST int rhs);						// 代入演算子 +=
	inline _RANGE& operator -= (CONST _RANGE& rhs);					// 代入演算子 -=
	inline _RANGE& operator -= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 -=
	inline _RANGE& operator -= (CONST int rhs);						// 代入演算子 -=
	inline _RANGE& operator *= (CONST _RANGE& rhs);					// 代入演算子 *=
	inline _RANGE& operator *= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 *=
	inline _RANGE& operator *= (CONST int rhs);						// 代入演算子 *=
	inline _RANGE& operator /= (CONST _RANGE& rhs);					// 代入演算子 /=
	inline _RANGE& operator /= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 /=
	inline _RANGE& operator /= (CONST int rhs);						// 代入演算子 /=

	inline int     operator[] (unsigned int nIdx) const;			// 配列添え字
	inline int&    operator[] (unsigned int nIdx);					// 配列添え字

	inline int     GetRand(void);									// 乱数の取得
	inline bool    IsItGet(void);									// 取得できるか？

	inline int     GetIntValue(void)
	{
		return nMax - nMin;
	}
	inline float     GetFloatValue(void)
	{
		return fMax - fMin;
	}
	union
	{
		struct
		{
			int nMax;	// 最大値
			int nMin;	// 最小値
		};
		struct
		{
			float fMax;	// 最大値
			float fMin;	// 最小値
		};
	};
}RANGE;

/* * int型2つ分 */
typedef struct INTEGER2 : public INTEGER2SOURCE
{
public:
	INTEGER2() {}													// コンストラクタ
	INTEGER2(const int nMax, const int nMin);						// コンストラクタ
	INTEGER2(CONST _RANGE& rhs);									// コンストラクタ
	INTEGER2(const int rhs);										// コンストラクタ

	inline INTEGER2  operator + (void) const;						// 単項演算子+
	inline INTEGER2  operator - (void) const;						// 単項演算子-

	inline INTEGER2  operator + (CONST INTEGER2& rhs) const;		// 四則演算子 +
	inline INTEGER2  operator + (CONST _RANGE& rhs) const;			// 四則演算子 +
	inline INTEGER2  operator + (CONST int rhs) const;				// 四則演算子 +
	inline INTEGER2  operator - (CONST INTEGER2& rhs) const;		// 四則演算子 -
	inline INTEGER2  operator - (CONST _RANGE& rhs) const;			// 四則演算子 -
	inline INTEGER2  operator - (CONST int rhs) const;				// 四則演算子 -
	inline INTEGER2  operator * (CONST INTEGER2& rhs) const;		// 四則演算子 *
	inline INTEGER2  operator * (CONST _RANGE& rhs) const;			// 四則演算子 *
	inline INTEGER2  operator * (CONST int rhs) const;				// 四則演算子 *
	inline INTEGER2  operator / (CONST INTEGER2& rhs) const;		// 四則演算子 /
	inline INTEGER2  operator / (CONST _RANGE& rhs) const;			// 四則演算子 /
	inline INTEGER2  operator / (CONST int rhs) const;				// 四則演算子 /

	inline bool      operator == (CONST INTEGER2& rhs) const;		// 比較演算子 ==
	inline bool      operator == (CONST _RANGE& rhs) const;			// 比較演算子 ==
	inline bool      operator == (CONST int rhs) const;				// 比較演算子 ==
	inline bool      operator != (CONST INTEGER2& rhs) const;		// 比較演算子 !=
	inline bool      operator != (CONST _RANGE& rhs) const;			// 比較演算子 !=
	inline bool      operator != (CONST int rhs) const;				// 比較演算子 !=

	inline INTEGER2& operator += (CONST INTEGER2& rhs);				// 代入演算子 +=
	inline INTEGER2& operator += (CONST _RANGE& rhs);				// 代入演算子 +=
	inline INTEGER2& operator += (CONST int rhs);					// 代入演算子 +=
	inline INTEGER2& operator -= (CONST INTEGER2& rhs);				// 代入演算子 -=
	inline INTEGER2& operator -= (CONST _RANGE& rhs);				// 代入演算子 -=
	inline INTEGER2& operator -= (CONST int rhs);					// 代入演算子 -=
	inline INTEGER2& operator *= (CONST INTEGER2& rhs);				// 代入演算子 *=
	inline INTEGER2& operator *= (CONST _RANGE& rhs);				// 代入演算子 *=
	inline INTEGER2& operator *= (CONST int rhs);					// 代入演算子 *=
	inline INTEGER2& operator /= (CONST INTEGER2& rhs);				// 代入演算子 /=
	inline INTEGER2& operator /= (CONST _RANGE& rhs);				// 代入演算子 /=
	inline INTEGER2& operator /= (CONST int rhs);					// 代入演算子 /=

	inline int       operator[] (unsigned int nIdx) const;			// 配列添え字
	inline int&      operator[] (unsigned int nIdx);				// 配列添え字

	inline bool      IsZero(void) { return (nX == MYLIB_INT_UNSET && nY == MYLIB_INT_UNSET); }		// ゼロ以外の時
	inline bool      IsNotZero(void) { return (nX != MYLIB_INT_UNSET && nY != MYLIB_INT_UNSET); }	// ゼロの時
	inline bool      OneIsZero(void) { return (nX == MYLIB_INT_UNSET || nY == MYLIB_INT_UNSET); }	// 片方がゼロの時

}INTEGER2, *PINTEGER2;

/* * int型3つ分 */
typedef struct INTEGER3
{
public:
	INTEGER3() {}													// コンストラクタ
	INTEGER3(int nX, int nY, int nZ);								// コンストラクタ
	INTEGER3(int nRhs);												// コンストラクタ

	inline INTEGER3  operator + (void) const;						// 単項演算子 +
	inline INTEGER3  operator - (void) const;						// 単項演算子 -

	inline INTEGER3  operator + (CONST INTEGER3& rhs) const;		// 四則演算子 +
	inline INTEGER3  operator + (CONST int rhs) const;				// 四則演算子 +
	inline INTEGER3  operator - (CONST INTEGER3& rhs) const;		// 四則演算子 -
	inline INTEGER3  operator - (CONST int rhs) const;				// 四則演算子 -
	inline INTEGER3  operator * (CONST INTEGER3& rhs) const;		// 四則演算子 *
	inline INTEGER3  operator * (CONST int rhs) const;				// 四則演算子 *
	inline INTEGER3  operator / (CONST INTEGER3& rhs) const;		// 四則演算子 /
	inline INTEGER3  operator / (CONST int rhs) const;				// 四則演算子 /

	inline bool      operator == (CONST INTEGER3& rhs) const;		// 比較演算子 ==
	inline bool      operator == (CONST int rhs) const;				// 比較演算子 ==
	inline bool      operator != (CONST INTEGER3& rhs) const;		// 比較演算子 !=
	inline bool      operator != (CONST int rhs) const;				// 比較演算子 !=

	inline INTEGER3& operator += (CONST INTEGER3& rhs);				// 代入演算子 +=
	inline INTEGER3& operator += (CONST int rhs);					// 代入演算子 +=
	inline INTEGER3& operator -= (CONST INTEGER3& rhs);				// 代入演算子 -=
	inline INTEGER3& operator -= (CONST int rhs);					// 代入演算子 -=
	inline INTEGER3& operator *= (CONST INTEGER3& rhs);				// 代入演算子 *=
	inline INTEGER3& operator *= (CONST int rhs);					// 代入演算子 *=
	inline INTEGER3& operator /= (CONST INTEGER3& rhs);				// 代入演算子 /=
	inline INTEGER3& operator /= (CONST int rhs);					// 代入演算子 /=

	inline int       operator[] (unsigned int nIdx) const;			// 配列添え字
	inline int&      operator[] (unsigned int nIdx);				// 配列添え字

	int x;		// X軸成分
	int y;		// Y軸成分
	int z;		// Z軸成分
} INTEGER3, *PINTEGER3;

/* * int型4つ分 */
typedef struct INTEGER4
{
public:
	INTEGER4() {}													// コンストラクタ
	INTEGER4(int nX, int nY, int nZ, int nW);						// コンストラクタ
	INTEGER4(int nRhs);												// コンストラクタ

	inline INTEGER4  operator + (void) const;						// 単項演算子 +
	inline INTEGER4  operator - (void) const;						// 単項演算子 -

	inline INTEGER4  operator + (CONST INTEGER4& rhs) const;		// 四則演算子 +
	inline INTEGER4  operator + (CONST int rhs) const;				// 四則演算子 +
	inline INTEGER4  operator - (CONST INTEGER4& rhs) const;		// 四則演算子 -
	inline INTEGER4  operator - (CONST int rhs) const;				// 四則演算子 -
	inline INTEGER4  operator * (CONST INTEGER4& rhs) const;		// 四則演算子 *
	inline INTEGER4  operator * (CONST int rhs) const;				// 四則演算子 *
	inline INTEGER4  operator / (CONST INTEGER4& rhs) const;		// 四則演算子 /
	inline INTEGER4  operator / (CONST int rhs) const;				// 四則演算子 /

	inline bool      operator == (CONST INTEGER4& rhs) const;		// 比較演算子 ==
	inline bool      operator == (CONST int rhs) const;				// 比較演算子 ==
	inline bool      operator != (CONST INTEGER4& rhs) const;		// 比較演算子 !=
	inline bool      operator != (CONST int rhs) const;				// 比較演算子 !=

	inline INTEGER4& operator += (CONST INTEGER4& rhs);				// 代入演算子 +=
	inline INTEGER4& operator += (CONST int rhs);					// 代入演算子 +=
	inline INTEGER4& operator -= (CONST INTEGER4& rhs);				// 代入演算子 -=
	inline INTEGER4& operator -= (CONST int rhs);					// 代入演算子 -=
	inline INTEGER4& operator *= (CONST INTEGER4& rhs);				// 代入演算子 *=
	inline INTEGER4& operator *= (CONST int rhs);					// 代入演算子 *=
	inline INTEGER4& operator /= (CONST INTEGER4& rhs);				// 代入演算子 /=
	inline INTEGER4& operator /= (CONST int rhs);					// 代入演算子 /=

	inline int  operator[] (unsigned int nIdx) const;				// 配列添え字
	inline int& operator[] (unsigned int nIdx);						// 配列添え字

	int x;		// X軸成分
	int y;		// Y軸成分
	int z;		// Z軸成分
	int w;		// W軸成分
} INTEGER4, *PINTEGER4;

// 2成分のfloat
typedef struct FLOAT2 : public D3DXVECTOR2
{
	FLOAT2() {}																									// コンストラクタ
	FLOAT2(float x, float y) : D3DXVECTOR2(x, y) {}																// コンストラクタ
	FLOAT2(CONST FLOAT2& rhs) : D3DXVECTOR2(rhs) {}																// コンストラクタ
	FLOAT2(CONST D3DXVECTOR2& rhs) : D3DXVECTOR2(rhs) {}														// コンストラクタ
	~FLOAT2() {}																								// デストラクタ

	inline FLOAT2        operator -(void) const;																// 単項演算子 -

	inline FLOAT2        operator +(const FLOAT2 &rhs) const;													// 四則演算子 +
	inline FLOAT2        operator +(const INTEGER2 &rhs) const;													// 四則演算子 +
	inline FLOAT2        operator -(const FLOAT2 &rhs) const;													// 四則演算子 -
	inline FLOAT2        operator -(const INTEGER2 &rhs) const;													// 四則演算子 -
	inline FLOAT2        operator *(const FLOAT2 &rhs) const;													// 四則演算子 *
	inline FLOAT2        operator *(const INTEGER2 &rhs) const;													// 四則演算子 *
	inline FLOAT2        operator *(float rhs) const;															// 四則演算子 *
	inline FLOAT2        operator /(float rhs) const;															// 四則演算子 /
	inline FLOAT2        operator /(const FLOAT2 &rhs) const;													// 四則演算子 /
	inline FLOAT2        operator /(const INTEGER2 &rhs) const;													// 四則演算子 /

	inline friend FLOAT2 operator *(float lhs, const FLOAT2 &rhs) { return FLOAT2(rhs.x * lhs, rhs.y * lhs); }	// 四則演算子 *フレンド関数
	inline friend FLOAT2 operator /(float lhs, const FLOAT2 &rhs) { return FLOAT2(rhs.x / lhs, rhs.y / lhs); }	// 四則演算子 /フレンド関数

	inline float         Dot(const FLOAT2 &rhs) const;															// 内積
	inline float         Cross(const FLOAT2 &rhs) const;														// 外積
	inline float         Length(void) const;																	// 長さ
	inline float         LengthSq(void) const;																	// べき乗長さ
	inline void          Norm(void);																			// 正規化
	inline FLOAT2        GetNorm(void) const;																	// 正規化し取得
}FLOAT2;

// 2Dベクトル
typedef struct VEC2 : public FLOAT2
{
	VEC2() {}												// コンストラクタ
	VEC2(float x, float y) : FLOAT2(x, y) {}				// コンストラクタ
	VEC2(const FLOAT2 &rhs) : FLOAT2(rhs) {}				// コンストラクタ
	~VEC2() {}												// デストラクタ

	inline VEC2& operator =(const FLOAT2 &rhs);				// 代入演算子
	inline bool  IsVertical(const VEC2 &rhs) const;			// 垂直関係にある？
	inline bool  IsParallel(const VEC2 &rhs) const;			// 平行関係にある？
	inline bool  IsSharpAngle(const VEC2 &rhs) const;		// 鋭角関係？
}VEC2;

// 2D直線
typedef struct LINE_2D
{
	LINE_2D() : pos(0.0f, 0.0f), vec(1.0f, 0.0f) {}								// コンストラクタ
	LINE_2D(const FLOAT2 &pos, const VEC2 &vec) : pos(pos), vec(vec) {}			// コンストラクタ
	~LINE_2D() {}																// デストラクタ

	inline FLOAT2 GetPoint(float fCoffi) const;									// 点上の座標を取得

	FLOAT2 pos;																	// 位置
	VEC2   vec;																	// 方向ベクトル
}LINE_2D;

// 2D線分
typedef struct SEGMENT_2D : public LINE_2D
{
	SEGMENT_2D() {}																// コンストラクタ
	SEGMENT_2D(const FLOAT2 &p, const VEC2 &v) : LINE_2D(p, v) {}				// コンストラクタ
	SEGMENT_2D(const FLOAT2 &p1, const FLOAT2 &p2) : LINE_2D(p1, p2 - p1) {}	// コンストラクタ
	~SEGMENT_2D() {}															// デストラクタ

	inline FLOAT2      GetEndPoint(void) const;									// 終点を取得

	inline SEGMENT_2D& operator = (const FLOAT2 &rhs);							// 代入演算子
	inline SEGMENT_2D& operator = (const VEC2 &rhs);							// 代入演算子
}SEGMENT_2D;

// 球
typedef struct SPHERE_2D
{
	SPHERE_2D() : Point(0.0f, 0.0f), fRadius(0.5f) {}							// コンストラクタ
	SPHERE_2D(const FLOAT2 &p, float r) : Point(p), fRadius(r) {}				// コンストラクタ
	~SPHERE_2D() {}																// デストラクタ

	FLOAT2 Point;																// 中心点
	float  fRadius;																// 半径
}SPHERE_2D;

// 2Dカプセル
typedef struct CAPSULE_2D
{
	CAPSULE_2D() : fRadius(0.5f) {}																// コンストラクタ
	CAPSULE_2D(const SEGMENT_2D &s, float r) : Segment(s), fRadius(r) {}						// コンストラクタ
	CAPSULE_2D(const FLOAT2 &p1, const FLOAT2 &p2, float r) : Segment(p1, p2), fRadius(r) {}	// コンストラクタ
	~CAPSULE_2D() {}																			// デストラクタ

	inline void        set(SEGMENT_2D &seg, float &fRadius);									// 設定
	inline void        set(CONST FLOAT2 &pos, CONST VEC2 &vec, float &fRadius);					// 設定

	inline CAPSULE_2D& operator = (const float &rhs);											// 代入演算子
	inline CAPSULE_2D& operator = (const FLOAT2 &rhs);											// 代入演算子
	inline CAPSULE_2D& operator = (const VEC2 &rhs);											// 代入演算子

	SEGMENT_2D Segment;																			// 線分
	float      fRadius;																			// 半径
}CAPSULE_2D;

// 3成分float
typedef struct FLOAT3 : public D3DXVECTOR3
{
	FLOAT3() {}																												// コンストラクタ
	FLOAT3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}																// コンストラクタ
	FLOAT3(CONST FLOAT3& rhs) : D3DXVECTOR3(rhs) {}																			// コンストラクタ
	FLOAT3(CONST D3DXVECTOR3& rhs) : D3DXVECTOR3(rhs) {}																	// コンストラクタ
	FLOAT3(CONST D3DXVECTOR3* rhs) : D3DXVECTOR3(*rhs) {}																	// コンストラクタ
	~FLOAT3() {}																											// デストラクタ

	inline FLOAT3        operator = (CONST D3DXVECTOR3 rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}

	inline FLOAT3        operator +(const FLOAT3 &rhs) const;																// 四則演算子+
	inline FLOAT3        operator -(const FLOAT3 &rhs) const;																// 四則演算子-
	inline FLOAT3        operator -(void) const;																			// 四則演算子-
	inline FLOAT3        operator *(const FLOAT3 &rhs) const;																// 四則演算子*
	inline FLOAT3        operator *(float rhs) const;																		// 四則演算子*
	inline FLOAT3        operator /(const FLOAT3 &rhs) const;																// 四則演算子/
	inline FLOAT3        operator /(float rhs) const;																		// 四則演算子/

	inline friend FLOAT3 operator *(float lhs, const FLOAT3 &rhs) { return FLOAT3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }	// 四則演算子*フレンド関数
	inline friend FLOAT3 operator /(float lhs, const FLOAT3 &rhs) { return FLOAT3(rhs.x / lhs, rhs.y / lhs, rhs.z / lhs); }	// 四則演算子/フレンド関数

	inline float         Dot(const FLOAT3 &rhs) const;																		// 内積
	inline FLOAT3        Cross(const FLOAT3 &rhs) const;																	// 外積
	inline float         Length(void) const;																				// 長さ
	inline float         LengthSq(void) const;																				// べき乗長さ
	inline void          Norm(void);																						// 正規化
	inline FLOAT3        GetNorm(void) const;																				// 正規化し取得
}FLOAT3;

// ベクトル
typedef struct VEC3 : public FLOAT3
{
	VEC3() {}															// コンストラクタ
	VEC3(float x, float y, float z) : FLOAT3(x, y, z) {}				// コンストラクタ
	VEC3(const FLOAT3 &rhs) : FLOAT3(rhs) {}							// コンストラクタ
	VEC3(const D3DXVECTOR3 *rhs) : FLOAT3(rhs) {}						// コンストラクタ
	~VEC3() {}															// デストラクタ

	inline VEC3& operator =(const FLOAT3 &rhs);							// 代入演算子
	inline bool  IsVertical(const VEC3 &rhs) const;						// 垂直関係にある？
	inline bool  IsParallel(const VEC3 &rhs) const;						// 平行関係にある？
	inline bool  IsSharpAngle(const VEC3 &rhs) const;					// 鋭角関係？
}VEC3;

// 直線
typedef struct LINE
{
	LINE() : pos(0.0f, 0.0f, 0.0f), vec(1.0f, 0.0f, 0.0f) {}			// コンストラクタ
	LINE(const FLOAT3 &pos, const VEC3 &vec) : pos(pos), vec(vec) {}	// コンストラクタ
	~LINE() {}															// デストラクタ

	inline FLOAT3 GetPoint(float fCoffi) const;							// 点上の座標を取得

	FLOAT3 pos;															// 位置
	VEC3   vec;															// 方向ベクトル
}LINE;

// 線分
typedef struct SEGMENT : public LINE
{
	SEGMENT() {}														// コンストラクタ
	SEGMENT(const FLOAT3 &p, const VEC3 &v) : LINE(p, v) {}				// コンストラクタ
	SEGMENT(const FLOAT3 &p1, const FLOAT3 &p2) : LINE(p1, p2 - p1) {}	// コンストラクタ
	~SEGMENT() {}														// デストラクタ

	inline FLOAT3 GetEndPoint(void) const;								// 終点を取得
}SEGMENT;

// 球
typedef struct SPHERE
{
	SPHERE() : Point(0.0f, 0.0f, 0.0f), fRadius(0.5f) {}				// コンストラクタ
	SPHERE(const FLOAT3 &p, float r) : Point(p), fRadius(r) {}			// コンストラクタ
	~SPHERE() {}														// デストラクタ

	FLOAT3 Point;			// 中心点
	float  fRadius;			// 半径
}SPHERE;

// カプセル
typedef struct CAPSULE
{
	CAPSULE() : fRadius(0.5f) {}															// コンストラクタ
	CAPSULE(const SEGMENT &s, float r) : Segment(s), fRadius(r) {}							// コンストラクタ
	CAPSULE(const FLOAT3 &p1, const FLOAT3 &p2, float r) : Segment(p1, p2), fRadius(r) {}	// コンストラクタ
	~CAPSULE() {}																			// デストラクタ

	SEGMENT Segment;		// 線分
	float   fRadius;		// 半径
}CAPSULE;

// AABB
typedef struct AABB
{
	AABB() {}																// コンストラクタ
	AABB(const FLOAT3 &p, const FLOAT3 &hl) : Point(p), HalLength(hl) {}	// コンストラクタ
	inline float LenX(void) const { return HalLength.x * 2.0f; };			// X軸辺の長さを取得
	inline float LenY(void) const { return HalLength.y * 2.0f; };			// Y軸辺の長さを取得
	inline float LenZ(void) const { return HalLength.z * 2.0f; };			// Z軸辺の長さを取得
	inline float Len(int i) { return *((&HalLength.x) + i) * 2.0f; }		// 辺の長さを取得

	FLOAT3 Point;															// 中心点
	FLOAT3 HalLength;														// 各軸の辺の長さの半分
}AABB;

// マトリックス
typedef struct MATRIX : public D3DXMATRIX
{
	MATRIX() {}																// コンストラクタ
	MATRIX(CONST MATRIX& rhs) : D3DXMATRIX(rhs) {}							// コンストラクタ
	MATRIX(CONST D3DXMATRIX& rhs) : D3DXMATRIX(rhs) {}						// コンストラクタ
	MATRIX(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
		   FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
		   FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
		   FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44) :
	D3DXMATRIX(  _11,  _12,  _13,  _14,
				 _21,  _22,  _23,  _24,
				 _31,  _32,  _33,  _34,
				 _41,  _42,  _43,  _44) {}									// コンストラクタ
	~MATRIX() {}															// デストラクタ

	inline MATRIX operator +(CONST MATRIX &rhs) const;						// 四則演算子+
	inline MATRIX operator -(CONST MATRIX &rhs) const;						// 四則演算子-
	inline MATRIX operator -(void) const;									// 四則演算子-
	inline MATRIX operator *(CONST MATRIX &rhs) const;						// 四則演算子*
	inline MATRIX operator * (float rhs) const;								// 四則演算子*
	inline MATRIX operator / (float rhs) const;								// 四則演算子/

	inline void   Identity(void);											// マトリックスの初期化
	inline void   Inverse(MATRIX *pMat, float *pDeterminant = NULL);		// 逆行列
} MATRIX;

// 入力キーのセル
typedef struct _INPUTKEYCELL
{
	int            nKey;		// キー
	int            nData;		// データ
	_INPUTKEYCELL* pNext;		// 次のデータポインタ
}INPUTKEYCELL;

// タイマー情報
typedef struct _TIMER_INFO
{
	_TIMER_INFO() {};					// コンストラクタ
	_TIMER_INFO(int nStart, int nEnd);	// コンストラクタ

	int nStart;							// 開始
	int nEnd;							// 終了
}TIMER_INFO;

/* *
* テクスチャのUV座標の設定用の情報
* Previous version TEXTUREVARIABLES
*/
typedef struct _SETING_UV
{
	_SETING_UV() {}													// コンストラクタ
	_SETING_UV(int nCntU, int nNumU, int nCntV, int nNumV);			// コンストラクタ

	inline _SETING_UV& operator ++(void);							// 前置インクリメント
	inline _SETING_UV  operator ++(int);							// 後置インクリメント
	inline _SETING_UV& operator --(void);							// 前置デクリメント
	inline _SETING_UV  operator --(int);							// 後置デクリメント

	inline _SETING_UV& operator =(const int &rhs);					// 代入演算子

	inline int         operator [](unsigned int nIdx) const;		// 配列添え字
	inline int&        operator [](unsigned int nIdx);				// 配列添え字

	inline void        SetCnt(const int nCntU, const int nCntV);	// カウントの設定
	inline void        SetNum(const int nNumU, const int nNumV);	// 個数の設定
	union {
		struct {
			int nCntU;		// 横のカウント
			int nNumU;		// 横の個数
			int nCntV;		// 縦のカウント
			int nNumV;		// 縦の個数
		};
		int m[2][2];		// 配列 [U、V] [カウント、個数]
	};
}SETING_UV;

/* *
* テクスチャアニメーション情報
* Previous version ANIMTEXVARIABLES
*/
typedef struct _TEXTUREANIMEINFO
{
	_TEXTUREANIMEINFO() {}											// コンストラクタ
	_TEXTUREANIMEINFO(int nFrame, int nFrameMax, int nIteration);	// コンストラクタ

	inline int& operator++ (void);									// 前置インクリメント
	inline int  operator++ (int);									// 後置インクリメント
	inline int& operator-- (void);									// 前置デクリメント
	inline int  operator-- (int);									// 後置デクリメント

	inline void update(void);										// 更新
	inline void clamp(void);										// クランプ
	inline bool IsEqual(void) const;								// 同じとき
	inline bool IsGreater(void) const;								// 大なり
	inline bool IsGreaterEqual(void) const;							// 大なりイコール
	inline bool IsSmall(void) const;								// 小なり
	inline bool IsSmallEqual(void) const;							// 小なりイコール

	union {
		struct {
			int nFrame;			// フレーム
			int nFrameMax;		// 最大フレーム数
			int nIteration;		// 切り返し(1か-1)
		};
		int m[3];				// 配列[1:フレーム、2:最大フレーム、3:切り返し(1か-1)]
	};

}TEXTUREANIMEINFO;

// UV情報
typedef struct _UVINFO
{
	_UVINFO() {}
	_UVINFO(_UVINFO &rhs) :u(rhs.u), v(rhs.v) {}
	_UVINFO(float u, float v) :u(u), v(v) {}

	inline _UVINFO & operator = (D3DXVECTOR2 &rhs)
	{
		this->u = rhs.x;
		this->v = rhs.y;
		return *this;
	}

	inline _UVINFO & operator += (D3DXVECTOR2 &rhs)
	{
		this->u += rhs.x;
		this->v += rhs.y;
		return *this;
	}
	inline _UVINFO & operator -= (D3DXVECTOR2 &rhs)
	{
		this->u -= rhs.x;
		this->v -= rhs.y;
		return *this;
	}
	inline _UVINFO & operator *= (D3DXVECTOR2 &rhs)
	{
		this->u *= rhs.x;
		this->v *= rhs.y;
		return *this;
	}
	inline _UVINFO & operator /= (D3DXVECTOR2 &rhs)
	{
		this->u /= rhs.x;
		this->v /= rhs.y;
		return *this;
	}

	inline _UVINFO operator + (D3DXVECTOR2 &rhs)
	{
		return _UVINFO(u + rhs.x, v + rhs.y);
	}
	inline _UVINFO operator - (D3DXVECTOR2 &rhs)
	{
		return _UVINFO(u - rhs.x, v - rhs.y);
	}
	inline _UVINFO operator * (D3DXVECTOR2 &rhs)
	{
		return _UVINFO(u * rhs.x, v * rhs.y);
	}
	inline _UVINFO operator / (D3DXVECTOR2 &rhs)
	{
		return _UVINFO(u / rhs.x, v / rhs.y);
	}

	inline _UVINFO operator * (float rhs)
	{
		return _UVINFO(u * rhs, v * rhs);
	}
	inline _UVINFO operator / (float rhs)
	{
		return _UVINFO(u / rhs, v / rhs);
	}

	float u;	// 横
	float v;	// 縦
}UVINFO;




// トランスフォーム情報
typedef struct TRANSFORM
{
	TRANSFORM() : pos(MYLIB_3DVECTOR_ZERO), rot(MYLIB_3DVECTOR_ZERO), scal(MYLIB_SCALE_UNSET) {}														// コンストラクタ
	TRANSFORM(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &scal, D3DXMATRIX &mtxWorld) : pos(pos), rot(rot), scal(scal), mtxWorld(mtxWorld) {}		// コンストラクタ
	TRANSFORM(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &scal) : pos(pos), rot(rot), scal(scal) {}												// コンストラクタ

	inline void Identity(void);	// マトリックスの初期化

	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 回転
	D3DXVECTOR3 scal;			// スケール
	D3DXMATRIX  mtxWorld;		// マトリックス
} TRANSFORM, *PTRANSFORM;


/*-- モーションキー --*/
typedef struct
{
	int           nFlame;		// フレーム数
	D3DXVECTOR3 * pPos;			// 位置
	D3DXVECTOR3 * pRot;			// 回転量
} MOTION_KEY;

/*-- モーション --*/
typedef struct
{
	int          nLoop;			// ループするかどうか
	int          nNumKey;		// キー数
	MOTION_KEY * pMotionKey;	// モーションキー
}MOTION_INFO;


// ゲージ情報
typedef struct GAGE_INFO
{
	GAGE_INFO() {}
	GAGE_INFO(RANGE SourceRange, int nSourceValue, float fSourceCoefficient);

	RANGE range;			// 範囲
	int   nValue;			// 値
	float fCoefficient;		// 係数
} GAGE_INFO;


typedef struct
{
	UVSHORT b;	// 青
	UVSHORT g;	// 緑
	UVSHORT r;	// 赤
} BGR;

typedef struct
{
	UVSHORT b;	// 青
	UVSHORT g;	// 緑
	UVSHORT r;	// 赤
	UVSHORT a;	// 透明度
} BGRA;

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
// 範囲クラス
template <class T>
class CRange
{
public:
	enum MEM
	{
		M_MIN = 0,	// 最小
		M_MAX,		// 最大
		M_NUM,		// 個数
	};

	CRange() {}														// コンストラクタ
	CRange(CONST T nMax, CONST T nMin);								// コンストラクタ
	CRange(CONST INTEGER2SOURCE& rhs);								// コンストラクタ
	CRange(CONST RANGE& rhs);										// コンストラクタ
	CRange(CONST T rhs);											// コンストラクタ

	//inline CRange  operator + (CONST RANGE& rhs) const;				// 四則演算子 +
	//inline CRange  operator + (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 +
	//inline CRange  operator + (CONST T rhs) const;					// 四則演算子 +
	//inline CRange  operator + (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 +
	//inline CRange  operator - (CONST RANGE& rhs) const;				// 四則演算子 -
	//inline CRange  operator - (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 -
	//inline CRange  operator - (CONST T rhs) const;					// 四則演算子 -
	//inline CRange  operator - (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 -
	//inline CRange  operator * (CONST RANGE& rhs) const;				// 四則演算子 *
	//inline CRange  operator * (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 *
	//inline CRange  operator * (CONST T rhs) const;					// 四則演算子 *
	//inline CRange  operator * (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 *
	//inline CRange  operator / (CONST RANGE& rhs) const;				// 四則演算子 /
	//inline CRange  operator / (CONST INTEGER2SOURCE& rhs) const;	// 四則演算子 /
	//inline CRange  operator / (CONST T rhs) const;					// 四則演算子 /
	//inline CRange  operator / (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 /

	//inline bool    operator == (CONST RANGE& rhs) const;			// 比較演算子 ==
	//inline bool    operator == (CONST INTEGER2SOURCE& rhs) const;	// 比較演算子 ==
	//inline bool    operator == (CONST T rhs) const;					// 比較演算子 ==
	//inline bool    operator != (CONST RANGE& rhs) const;			// 比較演算子 !=
	//inline bool    operator != (CONST INTEGER2SOURCE& rhs) const;	// 比較演算子 !=
	//inline bool    operator != (CONST T rhs) const;					// 比較演算子 !=

	//inline CRange& operator += (CONST RANGE& rhs);					// 代入演算子 +=
	//inline CRange& operator += (CONST INTEGER2SOURCE& rhs);			// 代入演算子 +=
	//inline CRange& operator += (CONST T rhs);						// 代入演算子 +=
	//inline CRange& operator += (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 +=
	//inline CRange& operator -= (CONST RANGE& rhs);					// 代入演算子 -=
	//inline CRange& operator -= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 -=
	//inline CRange& operator -= (CONST T rhs);						// 代入演算子 -=
	//inline CRange& operator -= (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 -=
	//inline CRange& operator *= (CONST RANGE& rhs);					// 代入演算子 *=
	//inline CRange& operator *= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 *=
	//inline CRange& operator *= (CONST T rhs);						// 代入演算子 *=
	//inline CRange& operator *= (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 *=
	//inline CRange& operator /= (CONST RANGE& rhs);					// 代入演算子 /=
	//inline CRange& operator /= (CONST INTEGER2SOURCE& rhs);			// 代入演算子 /=
	//inline CRange& operator /= (CONST T rhs);						// 代入演算子 /=
	//inline CRange& operator /= (CONST D3DXVECTOR2 rhs) const;		// 四則演算子 /=

	//inline int     GetRand(void);									// 乱数の取得
	//inline bool    IsItGetRand(void);								// 乱数を取得できるか？

	inline T         GetDifference(void);								// 差の取得

	union {
		struct {
			T min;			// 最小
			T max;			// 最大
		};
		T m[M_NUM];			// [enum]
	};
};


/* 計測クラス（リリース時は動作しない） */
class CMeasurement
{
public:
	// コンストラクタ
	inline CMeasurement() {};
	// デストラクタ
	inline ~CMeasurement() {};

	// 計測開始
	inline void Start(void);
	// 計測終了
	inline void End(void);
	// 描画
	inline void Draw(void);
	// 秒数の取得
	inline double GetTime(void);
private:
#ifdef MYLIB_MEASURE
	LARGE_INTEGER m_freq;		// フリップ
	LARGE_INTEGER m_start;		// 開始時の時間
	LARGE_INTEGER m_end;		// 終了時の時間

#endif
};

/* ベクター型 */
template<class T>
class MyVector
{
public:
	/* メモ
	* T* = イテレーター
	*/
	// コンストラクタ
	inline MyVector();
	// コンストラクタ
	inline MyVector(const MyVector<T>& source);
	// デストラクタ
	inline ~MyVector();
	// = のオーバーロード
	inline MyVector<T>& operator=(const MyVector<T>& source);

	// コンテナが空かどうか
	inline bool empty(void) const;
	// サイズの取得
	inline int &size(void);
	// 内容量の取得
	inline int &capacity(void);
	// []のオーバーロード
	inline T& operator[](int nArray);
	// []のオーバーロード
	inline const T& operator[](int nArray) const;
	// クリア
	inline void clear(void);

	// 先頭要素を指すイテレータを取得する。
	inline T* begin(void);
	// 先頭要素を指すイテレータを取得する。
	inline const T* begin(void) const;
	// 末尾の次を指すイテレータを取得する
	inline T* end(void);
	// 末尾の次を指すイテレータを取得する
	inline const T* end(void) const;
	// 先頭要素への参照を取得する。
	inline T& front(void);
	// 先頭要素への参照を取得する。
	inline const T& front(void) const;
	// 末尾要素への参照を取得する
	inline T& back(void);
	// 末尾要素への参照を取得する
	inline const T& back(void) const;

	// 他のvectorオブジェクトとデータを入れ替える。
	inline void swap(MyVector<T>& rhs);
	// 要素数を変更する
	inline void resize(int nNewSize);
	// 要素数を変更する
	inline void resize(int nNewSize, const T& tValue);
	// 内容量を変更する
	inline void reserve(int nNewCapacity);
	// 新たな要素を末尾に追加する
	inline void push_back(const T& tValue);
	// 新たな要素を末尾に追加する
	inline void push_back(void);
	// 指定した要素を削除する。
	inline T* erase(const T* pIt);
	// 指定した範囲の要素を削除
	inline T* erase(const T* pIt, const T* pIt_last);
	// 任意の位置に新たな要素を挿入する
	inline T* insert(const T* pIt, const T& tValue);
	// 任意の値が要素内に含まれているか
	inline bool contains(const T& tValue) const;
	// 指定された値を検索する
	inline T* find(const T& tValue);
	// 指定された値を検索する
	inline const T* find(const T& tValue) const;
	// 指定された値を検索し削除する
	inline bool find_erase(const T& tValue);
	// イテレーターからインデックスにする
	inline int index_from_ptr(const T* pIt) const;
	// 末尾要素を削除する
	inline void pop_back(void);
private:

	// 内容量を増やす
	inline int AddCapacity(int nSize) const;

	int m_nSize;		// サイズ
	int m_nCapacity;	// 内容量
	T*  m_pData;		// データポインタ
};

/* *
* ファイルから初期設定を読みこむクラス
*/
class CLoadInitSetingfromFile
{
public:
	// * [contents] 列挙型定義
	// * [memo]
	// * LOADRESULT -> LR
	// * MAKE -> M
	// * BUFFER -> B
	enum LOADRESULT
	{
		LR_FAILURE = -1,		// 失敗(エラー)
		LR_SUCCESS,				// 成功

		M_CREATE_FAIL,			// 作成に失敗した
		M_OPEN_FAIL,			// ファイルを開けなかった
		M_SIZE_FAIL,			// サイズの取得失敗
		M_FILEDATA_FAIL,		// ファイルデータのエラー
		M_READ_FAIL,			// 読み込み失敗

		B_FAIL,					// バッファ領域取得エラー
	};

	// * [contents] コンストラクタ
	inline CLoadInitSetingfromFile() {};
	// * [contents] デストラクタ
	inline ~CLoadInitSetingfromFile() {};

	// * [contents] ファイルから一行ずつ読みこむ
	// * [input] ファイル名 関数のアドレス
	// * [return] enum型(結果)
	inline static LOADRESULT ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData));

	// * [contents] ファイル情報の読み込み
	// * [input] ファイル名 関数のアドレス
	// * [return] enum型(結果)
	inline static LOADRESULT ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine));

	// * [contents] ファイル情報の読み込み
	// * [input] ファイル名 関数のアドレス
	// * [return] enum型(結果)
	inline static LOADRESULT ReadLineByLineFromFile(FILE_NAME pFileName, void(*ReadLine)(CONST_STRING cnpLine, void * pVoidPtr), void * pVoidPtr);

	// * [contents] ファイル情報の作成
	// * [output] クラスのポインタ
	// * [input] ファイル名
	// * [return] enum型(結果)
	inline static LOADRESULT MakeLoadFile(CLoadInitSetingfromFile *pOutput, FILE_NAME pFileName);

	// * [contents] ファイル情報の作成
	// * [input] ファイル名
	// * [return] 作成したクラス
	inline static CLoadInitSetingfromFile* MakeLoadFile(FILE_NAME pFileName);

	// * [contents] ファイル情報の開放
	// * [output] 開放するクラスのポインタ
	inline static void Release(CLoadInitSetingfromFile *pClass);

	// * [contents] ファイルデータの破棄
	inline void DeleteFileData(void);

	// * [contents] ファイルサイズの取得
	// * [input] ファイルのポインタ
	// * [return] サイズ
	inline UVLONG GetFileSize(FILE * pFile);

	// * [contents]　文字列へファイルをロードする
	// * [input]　ファイル名,開くモード
	// * [return]　結果
	inline LOADRESULT LoadFileIntoString(FILE_NAME pFileName, OPEN_MODE Mode = "rb");

	// * [contents]　文字列から設定を読み込む
	inline LOADRESULT SetingfromString(void(*ReadLine)(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData));

	// * [contents]　文字列から設定を読み込む
	inline LOADRESULT SetingfromString(void(*ReadLine)(CONST_STRING cnpLine));

	// * [contents]　文字列から設定を読み込む
	inline LOADRESULT SetingfromString(void(*ReadLine)(CONST_STRING cnpLine, void * pVoidPtr), void * pVoidPtr);

	// * [contents] 文字列から任意の文字以降の範囲を取得
	// * [input] 文字列、文字列の終わり、任意の文字
	// * [return] 取得した文字列
	inline static CONST_STRING GetRangeStrfromChar(CONST_STRING cnStr, CONST_STRING cnStrEnd, char marker);

	/* メンバ変数 */
	FILE_SIZE m_nuFileSize;	// ファイルサイズ
	FILE_DATA m_pFileData;	// ファイルデータ
};

typedef CLoadInitSetingfromFile CLoadFile;

// キーインプット情報型
class CInputkey
{
	/* * コンストラクタ */
	CInputkey() {}
	/* * デストラクタ */
	~CInputkey() {}
	/* * 初期化 */
	void Init(void);
	/* * キーデータの初期化 */
	void KeyInit(void);
	/* * キーデータの生成 */
	void KeyCreate(void);
	/* * キーデータリストの開放 */
	void KeyListRelease(void);
	/* * キーデータの挿入 */
	void KeyInsert(int nData, int nKey);
	/* * キーデータを抜き取る　*/
	void KeyPullout(int nKey);
	/* * キーデータの取得 */
	int GetKeyData(int nKey);
	/* * サイズの取得 */
	int GetSize(void);
private:
	int           m_nSize;		// サイズ（データ数)
	INPUTKEYCELL* m_pKeyTop;	// キーの先頭ポインタ
};

// 文字列型
class CString
{
public:
	/* * コンストラクタ */
	CString();
	/* * デストラクタ */
	~CString();

	inline STRING operator = (CONST_STRING string);

	/* * 文字列を追加する */
	inline STRING operator += (CONST_STRING string);

	/* * 初期化 */
	inline void Init(void);
	/* * 開放処理 */
	inline void release(void);
	/* * 文字列の設定(設定していた場合は再設定する) */
	inline void Set(STRING string);
	inline void Set(CONST_STRING string);

	/* * 文字列を合成する(MAX256) */
	inline void Synthesize(char * fmt, ...);

	/* * 文字列の取得 */
	inline STRING Get(void);

	/* * サイズの取得 */
	inline int & size(void);
private:
	STRING m_string;		// 文字列ポインタ
	int m_nStringLength;	// 文字列の長さ
};

class CMylibrary
{
public:
	//----------------------------------------------------------------------------------------------------
	// 2D
	//----------------------------------------------------------------------------------------------------

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 3DposA、3DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 2DposA、2DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 3DposA、2DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 2DposA、3DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2Dの外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した2D外積
	static float Create2DCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 2Dの外積を生成する
	//* [in] 2DvecA、2DvecB
	//* [return] 生成した2D外積
	static float Create2DCross(CONST D3DXVECTOR2 *vecA, CONST D3DXVECTOR2 *vecB);

	//* [contents] 2Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した2D外積
	static float Create2DDot(CONST D3DXVECTOR3* vecA, CONST D3DXVECTOR3* vecB);

	//* [contents] 2Dの内積を生成する
	//* [in] 2DvecA、2DvecB
	//* [return] 生成した2D外積
	static float Create2DDot(CONST D3DXVECTOR2* vecA, CONST D3DXVECTOR2* vecB);

	//* [contents] 2Dのベクトルの長さを求める
	//* [in] 2Dvec
	//* [return] ベクトルの長さ
	static float Get2DVecLength(D3DXVECTOR2 *vec);

	//* [contents] 2Dの差分の距離
	//* [in] 3D差分
	//* [return] 計算した距離
	static float Get2DToPointDistance(D3DXVECTOR2 *diffPos);

	//* [contents] 2Dの単位ベクトルを生成
	//* [in] 2Dvec
	//* [out] 生成した単位ベクトル
	static void CreateUnitVec2D(D3DXVECTOR2 *Output, D3DXVECTOR2 *Vec);

	//* [contents] 2Dの差分の角度
	//* [in] 3DposA, 3DposB
	//* [out] 計算した角度
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 2Dの差分の角度
	//* [in] 3D差分
	//* [out] 計算した角度
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *diffPos);

	//* [contents] 2D内積を用いて直線同士の交点を求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfStraightLines(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D外積を用いて線分同士の交点を求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D, 法線
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfLineSegments(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D直線同士が交差しているか求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfStraightLineSegments(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD);

	//* [contents] 2D四角形の当たり判定
	//* [in] 位置A,位置B, 大きさA, 大きさB
	//* [return] bool （true 接触している、false 接触していない）
	static bool Test2DCollisionRect(D3DXVECTOR3 *posA, D3DXVECTOR3 *posB, D3DXVECTOR2 *sizeA = NULL, D3DXVECTOR2 *sizeB = NULL);

	//* [contents] 頂点位置の設定
	//* [in] 原点タイプ, 位置, 大きさ, 半径, 回転量
	//* [out] 頂点位置
	static void SetVatexPostion(D3DXVECTOR3 **ppOut, ORIGINVERTEXTYPE type, D3DXVECTOR3 *pPos, D3DXVECTOR2 *pSize, float fRadius = 0.0f, float fRotation = 0.0f);

	//-------------------------------------------------------------------------------------------------------------
	// 3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] Y軸をなくした3Dベクトルを生成する
	//* [in] 3DposA、3DposB
	//* [out] 生成した3Dベクトル
	static void CreateFlatVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 3DposA、3DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 2DposA、2DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 2DposA、3DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 3DposA、2DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3Dの外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D外積
	static void Create3DCross(D3DXVECTOR3 *resulut, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの平面外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した平面3D外積
	static float CreateflatCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 3Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した3D内積
	static float Create3DDot(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの内積を生成する
	//* [in] ベクトル
	//* [return] 生成した3D内積
	static float Create3DDot(D3DXVECTOR3 *vec);

	//* [contents] 3Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D内積
	static void Create3DDot(float *out, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの2D上のベクトルの長さを求める
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D内積
	static float GetFlatVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3Dの2D上のベクトルの長さを求める
	//* [in] 3Dvec
	//* [out] 生成したベクトルの長さ
	static float Get3DVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3Dの2点のベクトルの長さを求める
	//* [in] 3DvecA、3DvecB
	//* [return] 生成したベクトルの長さ
	static float Get3DClossLength(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの2点のベクトルの長さを求める
	//* [in] 3Dvec
	//* [out] 生成したベクトルの長さ
	static void Get3DVecLength(float *out, D3DXVECTOR3 * vec);

	//* [contents] 3Dの2点の外積の長さを求める
	//* [in] 3DposA、3DposB
	//* [out] 生成したベクトルの長さ
	static void Get3DClossLength(float *out, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 3Dの単位ベクトルを生成
	//* [in] 3Dvec
	//* [out] 生成した単位ベクトル
	static void CreateUnitVector(D3DXVECTOR3 *out, D3DXVECTOR3 *Vec);

	//* [contents] 3Dの頂点間の距離の取得
	//* [in] 3DposA、3DposB
	//* [return] 計算した距離
	static float Get3DToPointDistance(CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dの差分の距離
	//* [in] 3D差分
	//* [return] 計算した距離
	static float Get3DToDiffDistance(D3DXVECTOR3 *diffPos);

	//* [contents] 3Dの差分の距離
	//* [in] 3D差分
	//* [out] 計算した距離
	static void Get3DToPointDistance(float *Length, D3DXVECTOR3 *diffPos);

	//* [contents] 3D平面の差分の距離
	//* [in] 3D位置A、3D位置B
	//* [out] 計算した距離
	static float GetFlatToPointDistance(CONST D3DXVECTOR3 * pPosA, CONST D3DXVECTOR3 * pPosB);

	//* [contents] ポリゴンの裏にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool TestIfPolygonBack(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3 &nom);

	//* [contents] ポリゴン内にいるかテストする
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool PolygonInTest(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP);

	//* [contents] 平面のポリゴンの範囲にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool Test3DInsidePlanePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP);

	//* [contents] ポリゴンの範囲にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP);

	//* [contents] ポリゴンの範囲にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P), ポリゴンの法線
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3 & nom);

	//* [contents] 直線同士の距離と最短点座標の取得
	//* [in] 位置A , ベクトルA, 位置B, ベクトルB
	//* [out] 距離, 最短点座標A, 最短点座標B
	static bool Calc2LineNearestDistAndPos(D3DXVECTOR3* posA, D3DXVECTOR3* vecA, D3DXVECTOR3* posB, D3DXVECTOR3* vecB, float* fOutDist, D3DXVECTOR3* pOutPosA, D3DXVECTOR3* pOutPosB);

	//* [contents] 外積で内外位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （内 == true 外 == false）
	static bool CrossRelationship(D3DXVECTOR3 &posP, D3DXVECTOR3 &posA, D3DXVECTOR3 &posB);

	//* [contents] 内積で表裏位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （表 == true 裏 == false）
	static bool DotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 2D一辺を内積で位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （表 == true 裏 == false）
	static bool OneSideDotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 3D点と直線の距離
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] 計算した距離
	static float Get3DLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D線上の最近点を算出
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [out] 最近点
	static void Get3DNearestPointOnline(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D平面上の点と直線のの距離
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] 計算した距離
	static float Get3DFlatLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 平面との距離
	//* [in] 平面上の位置、平面の方向ベクトル、直線の位置、直線の方向ベクトル
	//* [return] 計算した距離
	static float GetPlaneDistance(D3DXVECTOR3* pPosOnPlane, D3DXVECTOR3* pPlaneUnitVec, D3DXVECTOR3* pLinePos, D3DXVECTOR3* pLineUnitVec);

	//* [contents] 平面と直線の交点を算出
	//* [in] 平面上の位置、平面の法線、直線上の位置、直線の方向ベクトル
	//* [out] 計算した交点
	static bool CalIntersectionPointToPlaneAndLine(D3DXVECTOR3 * pOut, D3DXVECTOR3 * pPlanePos, D3DXVECTOR3 * pPlaneNor, D3DXVECTOR3 * pStartPos, D3DXVECTOR3 * pVecDir);

	//* [contents] ビルボードの設定
	//* [in] デバイスポインタ
	//* [out] マトリックス
	static void SetBillboard(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//* [contents] ビルボードロー回転の打ち消しの設定
	//* [in] デバイスポインタ
	//* [out] マトリックス
	static void SetBillboardNotRoll(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//* [contents] 線分上に頂点があるか判定
	//* [in] 頂点位置、線分の始まり、線分の終わり、線分の向き
	//* [return] bool （いる == true いない == false）
	static bool Test3DVertexOnLine(D3DXVECTOR3 * pPos, D3DXVECTOR3 *LineBegin, D3DXVECTOR3 *LineEnd, D3DXVECTOR3*pLineDir);

	//* [contents] 法線ベクトルからオイラー角に変換
	//* [in] 法線ベクトル
	//* [out]　変換した角度
	static void ConvertEulerAngleFromNormal(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pNor);

	//-------------------------------------------------------------------------------------------------------------
	// 2,3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 減速運動処理
	//* [in] (減速)係数
	//* [out] 3DVec移動量
	static void SlowingMove(D3DXVECTOR3 *pMove, const float fCoefficient);

	//* [contents] 減速運動処理
	//* [in] (減速)係数
	//* [out] 移動量
	static void SlowingMove(float *pMove, const float fCoefficient);

	//* [contents] 追従移動処理
	//* [in] 追う位置, 追われる位置, (追従)係数
	//* [out] 3DVec移動量
	static void FollowingMove(D3DXVECTOR3 *move, const D3DXVECTOR3 *ChasPos, const D3DXVECTOR3 *ChasedPos, const float *SpeedFactor);

	//* [contents] 点と点の距離を比較する
	//* [in] 距離, ソース（差分の場合追加ソースなし）, 追加ソース
	//* [return] bool （true 設定した距離より近い、false 設定した距離より遠い）
	static bool TestToPointDistance(float fDistance, CONST D3DXVECTOR3 *Source, CONST D3DXVECTOR3 *AddSource = NULL);

	//* [contents] 回転量を直す
	//* [out] 向きor回転量
	static void SetFixTheRotation(float *Rotation);

	//* [contents] 回転量を直す
	//* [out] Vec3向きor回転量
	static void SetVec3FixTheRotation(D3DXVECTOR3 *Rotation);

	//----------------------------------------------------------------------------------------------------
	// カプセル
	//----------------------------------------------------------------------------------------------------
	//* [contents] ∠p1p2p3は鋭角かどうか
	//* [in] Point（点）, Line（直線）
	//* [return] 鋭角かどうか（true : 鋭角, false : 鋭角ではない）
	static bool IsSharpAngle(CONST FLOAT2 &Point1, CONST FLOAT2 &Point2, CONST FLOAT2 &Point3);

	//* [contents] 点と直線の最短距離2D
	//* [in] Point（点）, Line（直線）
	//* [out] Perp（垂線）, fVecCoeffi（ベクトル係数）
	//* [return] 最短距離
	static float calcPointLineDist2D(const FLOAT2 &Point, const LINE_2D &Line);

	//* [contents] 点と線分の最短距離2D
	//* [in] Point（点）, Seg（線分）
	//* [out] EndPtShortdist（最短距離となる端点）, EndPoint（端点）
	//* [return] 最短距離
	static float CalPointSegmentDist2D(const FLOAT2 &Point, const SEGMENT_2D &Seg);

	//* [contents] 0～1の間に制限する
	//* [out] fValue（制限する値）
	static void Limit0to1(float &fValue);

	//* [contents] 線分と球の距離2D
	//* [in] Seg1（線分1）, Seg2（線分2）
	//* [out] PerpendFoot1（線分1側の垂線の足）,PerpendFoot2（線分2側の垂線の足）,fVecCoeffi1（線分1側のベクトル係数）, fVecCoeffi2（線分2側のベクトル係数）
	//* [return] 最短距離
	static float calSegmentPosDist2D(const SEGMENT_2D &Seg1, const D3DXVECTOR2 &pos);

	//* [contents] カプセル同士の衝突判定2D
	//* [in] Cap1（カプセル1）, Cap2（カプセル2）
	//* [return] 衝突している時true
	static bool calCapsuleSphere2D(const CAPSULE_2D &Cap1, const D3DXVECTOR2 &pos);

	//-------------------------------------------------------------------------------------------------------------
	// WorldMatrix
	//-------------------------------------------------------------------------------------------------------------
	//* [contents] スクリーン座標をワールド座標に変換
	//* [in] スクリーン座標、射影空間でのZ値、スクリーンの大きさ、ビュー行列、プロジェクション行列
	//* [out] 算出したワールド座標
	static void CalScreenToWorld(FLOAT3* pOut, INTEGER2 *pPos, float fZ, INTEGER2 *pScreenSize, MATRIX* pView, MATRIX* pPrj);

	//* [contents] スクリーン座標のレイを算出する
	//* [in] スクリーン座標、射影空間でのZ値、スクリーンの大きさ、ビュー行列、プロジェクション行列
	//* [out] 算出したレイ、近い位置、遠い位置
	static void CalScreenRay(VEC3 *pOutRay, FLOAT3 *pOutNearPos, FLOAT3 *pOutFarPos, INTEGER2 *pPos, INTEGER2 *pScreenSize, MATRIX* pView, MATRIX* pPrj);

	//-------------------------------------------------------------------------------------------------------------
	// Quaternion
	//-------------------------------------------------------------------------------------------------------------
	//* [contents] 方向ベクトルからクォータニオンを求める
	//* [in] 方向ベクトル(正規化)、回転量
	//* [out] 算出したクォータニオン
	static void CalQuaternionRotationAxis(D3DXQUATERNION * pOutput, CONST D3DXVECTOR3 &Norm,const float &fAngle);

	//* [contents] クォータニオンを使って位置を計算する
	//* [in] 方向ベクトル(正規化)、原点からの距離
	//* [out] 算出した位置
	static void UsingQuaternionCalculatePos(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3& Nom, CONST D3DXVECTOR3& Vec, const float& fRadian);


	//-------------------------------------------------------------------------------------------------------------
	// COLLAR
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] COLORREFからD3DXCOLORに変換する(アルファ値なし)
	//* [in] システムカラー
	//* [out] DirectX3Dカラー
	static void SetD3DXCOLORfromREF_WithoutAlpha(COLORREF *SystemColor, D3DXCOLOR *col);

	//* [contents]  COLORREFからD3DXCOLORに変換する
	//* [in] システムカラー
	//* [out] DirectX3Dカラー
	static void SetD3DXCOLORfromREF(COLORREF *SystemColor, D3DXCOLOR *col);

	//-------------------------------------------------------------------------------------------------------------
	// Misc
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 桁数を取得する
	//* [in] ソース（数字）
	//* [out] 桁数
	static void GetDigits(int &out, int nSource);

	//* [contents] 桁数を取得する
	//* [in] ソース（数字）
	//* [return] 桁数
	static int GetDigits(int nSource);

	//* [contents] 桁数を取得する(引数あり)
	//* [in] ソース, 数字, 基数（進数）
	//* [out] 桁数
	static void GetDigitsEx(int &out, int nSource, int nRadix);

	//* [contents] 桁数を取得する(引数あり)
	//* [in] ソース, 数字, 基数（進数）
	//* [return] 桁数
	static int GetDigitsEx(int nSource, int nRadix);

	//* [contents] キーIDの参照したキー名を取得
	//* [in] キーID
	//* [out] キー名
	static void GetKeyInputInfo(int nKeyID, STRING KeyName = NULL);

	//* [contents] 繰り上げた数を取得
	//* [in] ソース, 基数
	//* [return] 繰り上げた数
	static int GetRoundedUp(int nSource, int nBase);

	//-------------------------------------------------------------------------------------------------------------
	// FILE
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] テキストファイルから個数を取得する
	//* [in] ファイル名, 項目名
	//* [return] 個数
	static int GetNumberFromText(CONST_STRING TextFile, CONST_STRING SetItem);

	//* [contents] テキストファイルにスクリプトが存在するかチェック
	//* [in] ファイルポインタ
	//* [return] bool (true 存在した、false 存在しない)
	static bool CheckforScript(FILE *pFile);

	//-------------------------------------------------------------------------------------------------------------
	// STRING
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 文字列の確保、設定
	//* [in] ソース(文字列)
	//* [return] 文字列ポインタ
	static STRING SetStringAlloc(CONST_STRING source);

	//* [contents] 文字列からint型に変換する(文字列から数字を抜き取る)－を考慮しない
	//* [in] 文字列
	//* [return] 変換した数字
	static int GetConvertStoI(STRING str);

	//* [contents] 文字列からint型に変換する(文字列から数字を抜き取る)－を考慮しない
	//* [in] 文字列
	//* [return] 変換した数字
	static int GetConvertStoI(CONST_STRING str);

	//* [contents] int型から文字列に変換する
	//* [in] 変換する値、基数（進数）
	//* [out] 変換した文字列
	static void ConvertItoS(STRING pOut, int nVal, int nRadix);

	//* [contents] int型から文字列を生成し、変換する
	//* [in] 基数（進数）
	//* [return] 変換した文字列
	static STRING ConvertItoSAlloc(int nVal, int nRadix);

	//* [contents] int型から文字列を生成し、変換する
	//* [in] 変換する値、基数（進数）
	//* [out] 変換した文字列
	static void ConvertItoSAlloc(char **ppOut, int nVal, int nRadix);

	//* [contents] パスを取り除く
	//* [in] 文字列
	//* [return] 取り除いた文字列
	static STRING RemovePath(STRING src);

	//* [contents] パスを取り除く
	//* [in] 文字列
	//* [out] 取り除いた文字列
	static void RemovePath(CString& out, CString& src);


	//-------------------------------------------------------------------------------------------------------------
	// TEMPLATE
	//-------------------------------------------------------------------------------------------------------------
	/* MEMO
	* エラーを回避するためにinline関数にする
	*/

	// 違う値の時に設定
	// input : (変更するデータ、)
	template <class T>
	inline static bool Setdifferentvalue(T &tData, CONST T tSource)
	{
		if (tData != tSource)
		{
			tData = tSource;
			return true;
		}
		return false;
	}
};

// インライン関数用ファイルをインクルード
#include "mylibrary.inl"

#endif