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
#include "texture.h"
#include "Debug/debugproc.h"
#include "ImGui/Imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define ZeroVector4			(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))		//初期化用 全部0.0f
#define ZeroVector3			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//初期化用 全部0.0f
#define ZeroVector2			(D3DXVECTOR2(0.0f, 0.0f))					//初期化用 全部0.0f
#define OneVector3			(D3DXVECTOR3(1.0f, 1.0f, 1.0f))				//初期化用 全部1.0f
#define OneVector2			(D3DXVECTOR2(1.0f, 1.0f))					//初期化用 全部1.0f
#define ZeroColor			(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))			//透明
#define WhiteColor			(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))			//白
#define RedColor			(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))			//赤
#define GreenColor			(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))			//緑
#define BlueColor			(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))			//青
#define YellowColor			(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))			//黄
#define OrangeColor			(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))			//橙
#define MagentaColor		(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))			//マゼンダ
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
	ENEMY,				// 敵
};

enum class DIRECTION
{
	NONE = -1,
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

	static bool CheckMove(DIRECTION);								//移動用関数(AWSD + CrossKey) Press
	static DIRECTION CheckSelect();									//セレクト用関数(AWSD + ARROW + LStick) Trigger

	static bool CheckDecision();									//決定ボタン
	static bool CheckCancel();										//キャンセルボタン
	static bool CheckPause();										//ポーズ
	static bool CheckAnyButton();									//何かボタンを。。
	static void ResetStickInfo();									//スティックのトリガー情報リセット

	static bool Check3DMoveStick(D3DXVECTOR3 &Move, float const fMove,float const fCameraRot,float &fRotGoal);	//3D用　LStick　移動量計算
	static bool Check3DCameraStick(D3DXVECTOR3 &Rot,float fHolizonMove,float fVerticalMove);					//3D用　RStick　移動量計算

	static void StartVibration(int nCntVibration);					//バイブレーション処理

	static void CalcMatrix(D3DXMATRIX *pMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot, D3DXVECTOR3 const &rScale);		//ワールドマトリックス計算
	static void CalcShadowMatrix(D3DXMATRIX &rShadowMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rNor);						//シャドーマトリックスの計算
	static D3DXVECTOR3 CalcMtxToVector3(D3DXMATRIX const & rMtx);																		//マトリックスをVector3に変換

	static void SetModelVertex(MODEL_VTX &pModelVtx, CModelInfo &pModelInfo);													//モデルの最大頂点と最少頂点を設定
	static void SetBillboard(D3DXMATRIX *pMtx);																					//ビルボード設定
	static void SetBillboard_XZ_Only(D3DXMATRIX *pMtx);																			//ビルボード設定　XとZのみ

	static void SelectVerticalMenu(int &nSelectNum, int const &nMaxSelect);														//縦メニューの選択
	static void SelectHorizonMenu(int &nSelectNum, int const &nMaxSelect);														//横メニューの選択

	static HRESULT InitImgui(HWND hWnd);	//Imgui生成処理

	static void UninitImgui();				//Imgui終了
	static void ShowDebugInfo();			//デバッグ情報表示
	static void CheckWireMode();			//ワイヤーフレーム確認
	static void CheckCulling();				//カリング確認
	static void CheckLighting();			//ライティング確認

	static bool ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int & nValue);		//ImGuiのコンボボックス

	static int RandomRangeUnsigned(int nMin, int nMax);							//ランダムに値を返す関数　int型　符号なし
	static float Random_PI();													//-3.14から3.14までのランダムで返す
	static float Random(float fInputValue);										//入力された値の+-ランダムな値で返す
	static D3DXVECTOR3 RandomVector3(float Max);								//ランダムなvector3型で値を返す
	static void CalcRotation(float &fRot);										//回転を360度以内にする計算
	static void CalcRotation_XYZ(D3DXVECTOR3 &rot);								//回転を360度以内にする計算

	static bool Selecting(int &nSelectNum,int const &nSelectNumOld, int const nNumX, int const nNumY);
	static D3DXVECTOR2 CalcUV_StaticFunc(int nNumUV, int tex);

	//------------------------------------------------------------------------------
	//範囲内の値に修正する関数
	//intでもfloatでもいけるようにテンプレート
	//------------------------------------------------------------------------------
	template <class X> static bool RangeLimit_Equal(X &Value, X const &Min, X const &Max)
	{
		//最小値より小さい時
		if (Value < Min)
		{
			//最小値に合わす
			Value = Min;
			return true;
		}
		//最大値より大きい時
		if (Value > Max)
		{
			//最大値に合わす
			Value = Max;
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------
	//AとBを入れ替える処理
	//intでもfloatでもいけるようにテンプレート
	//------------------------------------------------------------------------------
	template <class X> static void Swap(X &nValueA, X &nValueB)
	{
		X SaveValue = nValueA;
		nValueA = nValueB;
		nValueB = SaveValue;
	}

	//------------------------------------------------------------------------------
	//vector型用　シャッフル処理
	//------------------------------------------------------------------------------
	template <class X> static void Vec_Shuffle(std::vector<X> &vec)
	{
		//サイズ分
		for (int nCnt = 0; nCnt < (int)vec.size(); nCnt++)
		{
			//交換先の配列番号をランダムで設定
			int SwapIterator = rand() % (vec.size() - 1);

			//入れ替え
			CHossoLibrary::Swap(vec[nCnt], vec[SwapIterator]);
		}
	}

	//------------------------------------------------------------------------------
	//Ray判定用　CSceneX継承前提
	//Rayで判定を取ってtrueが返ってきた時に
	//------------------------------------------------------------------------------
	template <class X> static bool RayCollision_ModuleSelect(std::vector<X> &vec, int &nSelectNum)
	{
		// 変数宣言
		D3DXVECTOR3*   pNearPos = &CManager::GetRay()->NearPos;	// レイの近い位置
		D3DXVECTOR3*   pFarPos = &CManager::GetRay()->FarPos;	// レイの遠い位置
		BOOL           bHit = FALSE;						// ヒットフラグ
		D3DXMATRIX     invmat;									// 算出した逆行列
		D3DXVECTOR3    ray;										// レイ
		D3DXVECTOR3    InvNirePos;								// 算出した近い位置
		D3DXVECTOR3    InvForePos;								// 算出した遠い位置
		LPD3DXMESH     pMesh;									// メッシュ情報

		bool bModuleHit = false;
		int nCnt = -1;

		//モジュールのリスト
		for (auto ptr : vec)
		{
			//カウントアップ
			nCnt++;

			//既に何かヒットしてた時
			if (bModuleHit)
			{
				//選択状態解除
				ptr->SetSelect(false);
				continue;
			}

			////選択できないモジュールだった場合
			//if (!ptr->GetCanModuleSelect())
			//{
			//	//計算しない
			//	continue;
			//}

			//メッシュ情報取得
			pMesh = ptr->GetModelInfo()->GetMesh();

			/* 対処いう物からみたレイに変換する */
			//	逆行列の取得
			D3DXMatrixInverse(&invmat, NULL, ptr->GetMtxWorldPtr());
			//	逆行列を使用し、レイ始点情報を変換　位置と向きで変換する関数が異なるので要注意
			D3DXVec3TransformCoord(&InvNirePos, pNearPos, &invmat);
			//	レイ終点情報を変換
			D3DXVec3TransformCoord(&InvForePos, pFarPos, &invmat);
			//	レイ方向情報を変換
			D3DXVec3Normalize(&ray, &(InvForePos - InvNirePos));
			//Rayを飛ばす
			D3DXIntersect(pMesh, &InvNirePos, &ray, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);

			// HITしている時
			if (bHit)
			{
				//選択状態
				ptr->SetSelect(true);

				//選択番号設定
				nSelectNum = nCnt;

				bModuleHit = true;
			}
			else
			{
				ptr->SetSelect(false);
			}
		}

		//何もヒットしなかった時はカウント無効
		if (!bModuleHit)
		{
			nSelectNum = -1;
		}

		//return
		return bModuleHit;
	}
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


};

#endif