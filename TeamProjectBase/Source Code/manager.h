//------------------------------------------------------------------------------
//
//マネージャー処理  [manager.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MANAGER_H_
#define _MANAGER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "Mylibrary.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------

//前方宣言
class CRenderer;
class CSound;
class CKeyboard;
class CMouse;
class CBaseMode;
class CGame;
class CSound;
class CPad_XInput;
class CDecoding;
class CManager
{
public:

	enum MODE
	{
		MODE_NONE = -1,						//無し
		MODE_TITLE,							//タイトル
		MODE_DECODING,						// 解読
		MODE_TUTORIAL,						//チュートリアル
		MODE_GAME,							//ゲーム
		MODE_RESULT,						//リザルト
		MODE_TEST,							//テスト画面
		MODE_CONNECT_SERVER,				//サーバー接続
		MODE_EFFECT_VIEWER,					//エフェクトビューワ
		MODE_END							//ゲーム終了
	};

	typedef struct
	{
		FLOAT3 NearPos;		// 近い位置
		FLOAT3 FarPos;		// 遠い位置
		VEC3 vec;			// レイのベクトル
	} RAY;

	CManager() {};	// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);					//初期化
	void Uninit();																//終了
	void Update();																//更新
	void Draw();																//描画
	static void SetMode(MODE nextmode);											//モード設定

	//Get関数
	static MODE GetMode()				{ return m_mode; };						//モード取得
	static HWND GethWnd()				{ return m_hWnd; };						//ウィンドウハンドル取得
	static CRenderer *GetRenderer()		{ return m_pRenderer.get();	};			//レンダラー取得処理
	static CSound *GetSound()			{ return m_pSound.get(); };				//サウンド取得
	static CKeyboard *GetKeyboard()		{ return m_pKeyboard.get(); };			//キーボード取得
	static CMouse *GetMouse()			{ return m_pMouse.get(); };				//マウス取得
	static CPad_XInput *GetXInput()		{ return m_pXInput.get(); };			//XInputの取得
	static RAY *GetRay()				{ return &m_ray; }						// レイの取得

	static CBaseMode *GetBaseMode();											//ベースのモード取得
	static CGame *GetGame();													//ゲーム取得 簡単に呼び出せるように
	static CDecoding *GetDecoding();											// 解読の取得
private:
	static std::unique_ptr<CRenderer> m_pRenderer;		//レンダラーへのポインタ
	static std::unique_ptr <CSound > m_pSound;			//サウンドへのポインタ
	static std::unique_ptr<CKeyboard> m_pKeyboard;		//キーボードへのポインタ
	static std::unique_ptr<CMouse> m_pMouse;			//マウスへのポインタ
	static std::unique_ptr<CPad_XInput> m_pXInput;		//XInputへのポインタ
	static std::unique_ptr<CBaseMode> m_pBaseMode;		//ベースのモードへのポインタ

	static MODE m_mode;		//モード
	static HWND m_hWnd;		//ウィンドウハンドル

	static int m_nNumChangeMode;	//モード変えた回数

	static RAY m_ray;				// レイの情報
};

#endif