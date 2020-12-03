//*************************************************************************************************************
//
// 解読書のマネージャー[処理 [DecodingManager.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _DECODINGMANAGER_H_
#define _DECODINGMANAGER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"
#include "DecodingTypes.h"

//-------------------------------------------------------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------------------------------------------------------
class CDecodingUI;
class CDecodingWindow;
class CHash;
class CMouse;
//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CDecodingManager : public CScene
{
public:
	/* 列挙型定義 */
	// UI
	enum
	{
		UI_NONE = -1,			// 無し
		UI_BACKLIGHT,			// バックライト
		UI_TELLTHEPICTURE,		// 写真を教えてください
		UI_SHAPEMEMORIZATION,	// 形の暗記
		UI_CORDCUTTING,			// コードを切る
		UI_MAZE,				// 迷路
		UI_SIMON,				// サイモンゲーム
		UI_MAX					// 最大数
	};
	// ウィンドウ
	enum
	{
		WIND_NONE = -1,			// 無し
		WIND_TELLTHEPICTURE,	// 写真を教えてください
		WIND_SHAPEMEMORIZATION,	// 形の暗記
		WIND_CORDCUTTING,		// コードを切る
		WIND_MAZE,				// 迷路
		WIND_SIMON,				// サイモンゲーム
		WIND_MAX,				// 最大数
	};
	// テクスチャ
	enum
	{
		TEX_NONE = -1,			// 無し
		TEX_BACKLIGHT,			// バックライト
		TEX_TELLTHEPICTURE,		// 絵を教えてもらう
		TEX_SHAPEMEMORIZATION,	// 形の暗記
		TEX_CORDCUTTING,		// コードを切る
		TEX_MAZE,				// 迷路
		TEX_SIMON,				// サイモンゲーム
		TEX_MAX					// 最大数
	};
	// モード
	enum MODE
	{
		MODE_SELECT,			// 選択
		MODE_CONNECT,			// 接続
		MODE_SHOW,				// 解読書を表示している
		MODE_MAX				// 最大
	};

	/* メンバ関数 */
	CDecodingManager();									// コンストラクタ
	~CDecodingManager();								// デストラクタ

	static void    MakeHash(void);						// ハッシュの作成
	static void    ReleaseHash(void);					// ハッシュの開放

	static HRESULT Load(void);							// 読み込み
	static void    Unload(void);						// 開放

	static std::shared_ptr<CDecodingManager>Create(void);	// 生成

	HRESULT        Init();								//初期化
	void           Update();							//更新
	void           Draw();								//描画
	void           ShowDebugInfo() {}					//デバッグ情報表記

	void           SstMode(MODE mode);					// モードの設定

	static CHash * GetHash(void) { return m_pHash; }	// ハッシュポインタの取得
	CDecodingUI * GetUi(int nIndex) { return m_pUi[nIndex].get(); }

private:
	/* メンバ関数 */
	void CreateUi(void);								// UIの生成
	void CreateWind(void);								// ウィンドウの生成
	void SetPosAccordingParent(void);					// 親に従って位置を設定する
	void InitConnect(void);								// 接続モードの初期化
	void UpdateSelect(void);							// 選択するときの更新処理
	void UpdateConnect(void);							// 接続モード更新処理
	void UpdateShow(void);								// 解読書を表示しているときの更新処理

	static void UiInfoReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);		// 1行から情報を読み取る
	static void WindInfoReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る

	/* メンバ変数 */
	static CHash *                   m_pHash;					// ハッシュのポインタ
	static LPDIRECT3DTEXTURE9        m_pTexture[TEX_MAX];		// テクスチャ情報
	static SETINGINFO                m_Seting[UI_MAX];			// 設定情報
	static WINDOW_SETING             m_WindSeting[WIND_MAX];	// ウィンドウの設定情報
	std::shared_ptr<CDecodingUI>     m_pUi[UI_MAX];				// UI
	std::shared_ptr<CDecodingWindow> m_pWind[WIND_MAX];			// ウィンドウ
	MODE                             m_mode;					// モード
	MODE                             m_modeNext;				// 次のモード
	int                              m_nSelectIndex;			// 選択している番号
	CDecodingWindow*                 m_pActiveWind;				// アクティブなウィンドウのポインタ
};

#endif
