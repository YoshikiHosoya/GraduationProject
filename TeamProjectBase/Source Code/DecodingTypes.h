//*************************************************************************************************************
//
// 解読書の構造体[処理 [DecodingTypes.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _DECODINGTYPES_H_
#define _DECODINGTYPES_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// 構造体定義
//-------------------------------------------------------------------------------------------------------------
typedef struct _SETINGINFO
{
	_SETINGINFO() :								// コンストラクタ
		pos(MYLIB_VEC3_UNSET),
		size(MYLIB_VEC2_UNSET),
		col(MYLIB_D3DXCOR_SET),
		fAngle(MYLIB_FLOAT_UNSET),
		OriginType(ORIGINVERTEXTYPE_CENTER),
		bDisp(true) {}
	_SETINGINFO(								// コンストラクタ
		D3DXVECTOR3& pos, D3DXVECTOR2& size, D3DXCOLOR& col, float fAngle, ORIGINVERTEXTYPE OriginType, bool bDisp) :
		pos(pos), size(size), col(col), fAngle(fAngle), OriginType(OriginType), bDisp(bDisp) {}
	FLOAT3           pos;			// 位置
	FLOAT2           size;			// 大きさ
	D3DXCOLOR        col;			// 色
	float            fAngle;		// 角度
	ORIGINVERTEXTYPE OriginType;	// 原点タイプ
	bool             bDisp;			// 描画フラグ
}SETINGINFO;

#endif