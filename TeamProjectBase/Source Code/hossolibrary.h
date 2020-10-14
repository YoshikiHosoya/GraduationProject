//------------------------------------------------------------------------------
//
//ライブラリ処理  [hossolibrary.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _HOSSOLIBRARY_H_
#define _HOSSOLIBRARY_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "texture.h"
#include "Debug/debugproc.h"
#include "ImGui/Imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define ZeroVector4			(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))		//初期化用
#define ZeroVector3			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//初期化用
#define ZeroVector2			(D3DXVECTOR2(0.0f, 0.0f))					//初期化用
#define ZeroColor			(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))			//透明
#define WhiteColor			(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))			//白
#define RedColor			(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))			//赤
#define GreenColor			(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))			//緑
#define BlueColor			(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))			//青
#define YellowColor			(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))			//黄
#define OrangeColor			(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))			//橙
#define BlackColor			(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))			//黒

#define MAX_TEXT			(128)
#define SCREEN_CENTER_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f ,0.0f))			//画面中央の座標
#define SCREEN_SIZE			(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT,0.0f))							//画面のサイズ
#define NEWLINE				("\n")																	//改行
#define COMMENT01			("//\n")																//テキスト用　コメント　改行
#define COMMENT02			("//------------------------------------------------------------\n")	//テキスト用　線
#define EQUAL				("=")																	//テキスト用　イコール

//------------------------------------------------------------------------------
//構造体定義
//------------------------------------------------------------------------------
//int型を二つ格納できる
typedef struct INTEGER2
{
	int x;
	int y;

	//コンストラクタ
	INTEGER2() {};
	INTEGER2(int nX, int nY)
	{
		x = nX;
		y = nY;
	}

}INTEGER2;

//int型を3つ格納できる
typedef struct INTEGER3
{
	int x;
	int y;
	int z;

	//コンストラクタ
	INTEGER3() {};
	INTEGER3(int nX, int nY,int nZ)
	{
		x = nX;
		y = nY;
		z = nZ;
	}
}INTEGER3;

//int型を3つ格納できる
typedef struct MODEL_VTX
{
	D3DXVECTOR3 VtxMax;	//最大頂点
	D3DXVECTOR3 VtxMin;	//最少頂点

	//コンストラクタ
	MODEL_VTX() {};
	MODEL_VTX(D3DXVECTOR3 MinSize, D3DXVECTOR3 MaxSize)
	{
		VtxMin = MinSize;
		VtxMax = MaxSize;
	};

}MODEL_VTX;

// タグ
enum class TAG
{
	NONE = -1,
	PLAYER_1,			// プレイヤー1
	PLAYER_2,			// プレイヤー2
	ENEMY,				// 敵
	PRISONER,			// 捕虜
	OBSTACLE,			// オブジェクト
	SHIELD,				// 盾
};

enum class DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

//------------------------------------------------------------------------------
//エイリアス宣言
//------------------------------------------------------------------------------
using FILENAME_LIST = std::vector<std::string>;		//ファイル名のリスト

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CKeyboard;
class CPad_XInput;
class CModelInfo;


class CHossoLibrary
{
public:

	//入力の種類
	enum INPUTTYPE
	{
		PRESS = 0,	//プレス
		TRIGGER,	//トリガー
		MAX
	};

	//方向
	enum DIRECTION
	{
		NONE = -1,
		UP,			//↑
		DOWN,		//↓
		LEFT,		//←
		RIGHT		//→
	};

	static bool CheckMove(DIRECTION);								//移動用関数(AWSD + CrossKey) Press
	static DIRECTION CheckSelect();									//セレクト用関数(AWSD + ARROW + LStick) Trigger

	static bool CheckAttack_X(INPUTTYPE type);						//攻撃ボタン
	static bool CheckAttack_Y(INPUTTYPE type);						//攻撃ボタン
	static bool CheckAttack_B(INPUTTYPE type);						//攻撃ボタン
	static bool CheckJump(INPUTTYPE type);							//ジャンプボタン
	static bool CheckDash(INPUTTYPE type);							//ダッシュボタン
	static bool CheckCameraReset(INPUTTYPE type);					//カメラリセットボタン
	static bool CheckDecision();									//決定ボタン
	static bool CheckSkipTutorial();								//チュートリアルスキップ
	static bool CheckCancel();										//キャンセルボタン
	static bool CheckPause();										//ポーズ
	static bool CheckAnyButton();									//何かボタンを。。
	static void ResetStickInfo();									//スティックのトリガー情報リセット

	static bool Check3DMoveStick(D3DXVECTOR3 &Move, float const fMove,float const fCameraRot,float &fRotGoal);	//3D用　LStick　移動量計算
	static bool Check3DCameraStick(D3DXVECTOR3 &Rot,float fHolizonMove,float fVerticalMove);					//3D用　RStick　移動量計算

	static void StartVibration(int nCntVibration);					//バイブレーション処理

	static float Vec2Cross(D3DXVECTOR2 const &rVecA, D3DXVECTOR2 const &rVecB);									//2D 外積計算
	static void CalcMatrix(D3DXMATRIX *pMtx,D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot);					//ワールドマトリックス計算
	static void CalcShadowMatrix(D3DXMATRIX &rShadowMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rNor);		//シャドーマトリックスの計算
	static void SetModelVertex(MODEL_VTX &pModelVtx, CModelInfo &pModelInfo);									//モデルの最大頂点と最少頂点を設定
	static void SetModelVertexRotation(D3DXMATRIX & pMtx, MODEL_VTX & pModelVtx, CModelInfo & pModelInfo);		//モデルの最大頂点と最少頂点を設定　回転した分を補正
	static void SetBillboard(D3DXMATRIX *pMtx);																	//ビルボード設定
	static void SetBillboard_XZ_Only(D3DXMATRIX *pMtx);															//ビルボード設定　XとZのみ

	static bool RangeLimit_Equal_Int(int &nValue, int nMin, int nMax);											//範囲内に抑える(int)
	static bool RangeLimit_Equal_Float(float &nValue, float nMin, float nMax);									//範囲内に抑える(float)

	static void SelectVerticalMenu(int &nSelectNum, int const &nMaxSelect);										//縦メニューの選択
	static void SelectHorizonMenu(int &nSelectNum, int const &nMaxSelect);										//横メニューの選択


	static HRESULT InitImgui(HWND hWnd);	//Imgui生成処理


	static void UninitImgui();				//Imgui終了
	static void ShowDebugInfo();			//デバッグ情報表示
	static void CheckWireMode();			//ワイヤーフレーム確認
	static void CheckCulling();				//カリング確認
	static void CheckLighting();			//ライティング確認

	static float Random_PI();																					//-3.14から3.14までのランダムで返す
	static float Random(float fInputValue);																		//入力された値の+-ランダムな値で返す
	static D3DXVECTOR3 RandomVector3(float Max);																//ランダムなvector3型で値を返す
	static void CalcRotation(float &fRot);																		//回転を360度以内にする計算
	static void CalcRotation_XYZ(D3DXVECTOR3 &rot);																	//回転を360度以内にする計算

	static bool CheckDebugPlayer() { return m_bDebugPlayer; };		//デバッグ用のプレイヤー

private:
	static CKeyboard *m_pKeyboard;		//キーボードへのポインタ
	static CPad_XInput *m_pXInput;		//XInputのパッドへのポインタ
	static float m_fLeftStickX;			//左スティックの情報
	static float m_fLeftStickY;			//左スティックの情報
	static bool m_bStickX;				//スティックが倒れてるかどうか
	static bool m_bStickY;				//スティックが倒れてるかどうか6

	static bool m_WireFrame;			//ワイヤーフレームか
	static bool m_Lighting;				//ライティングするか
	static int m_Culling;				//カリング情報
	static bool m_bDebugPlayer;			//プレイヤー自動操作化


};

#endif