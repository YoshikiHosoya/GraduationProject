//------------------------------------------------------------------------------
//
//メイン処理  [main.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#define	DIRECTINPUT_VERSION	(0x0800)
#define _WINSOCKAPI_					// windows.hの後にwinsock2.hを呼び出すときに必要

#ifndef _MAIN_H_
#define _MAIN_H_


//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include <iostream>				// std::coutとか
#include <stdlib.h>				// ライブラリ
#include <windows.h>			// window生成
#include <WinSock2.h>			// winsock2の使用に必要
#include <crtdbg.h>				// CRTライブラリでメモリーリーク検出に必要
#include <vector>				// vector型に必要
#include <algorithm>			// find_if関数等に必要
#include <memory>				// unique_ptr等に必要
#include <thread>				// マルチスレッドに必要

#include "d3dx9.h"
#include "hossolibrary.h"

#include "Resource/resource.h"

//------------------------------------------------------------------------------
//ライブラリのリンク
//------------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3dx9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時間取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要 DirectInput
#pragma comment(lib,"xinput.lib")	//入力処理に必要 XInput

//------------------------------------------------------------------------------
//マクロ定義
//------------------------------------------------------------------------------
#define SCREEN_WIDTH	(1280)		//ウィンドウの幅
#define SCREEN_HEIGHT	(720)		//ウィンドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					// 2Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 頂点カラー / テクスチャ座標)



//------------------------------------------------------------------------------
//構造体
//------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3	pos;	//頂点座標
	float		rhw;	//1.0で固定　同次座標
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2	tex;	//テクスチャ座標
} VERTEX_2D;

// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線のベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;



//デバッグ用
#ifdef _DEBUG
int			GetFPS();
int			GetFPSInterval();
#endif
#endif