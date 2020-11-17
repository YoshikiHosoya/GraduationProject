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
class CHash;

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CDecodingManager : public CScene
{
public:
	/* 列挙型定義 */
	enum
	{
		UI_NONE = -1,			// 無し
		UI_BACKLIGHT,			// バックライト
		UI_TELLTHEPICTURE,		// 写真を教えてください
		UI_SHAPEMEMORIZATION,	// 形の暗記
		UI_CORDCUTTING,			// コードを切る
		UI_MAZE,				// 迷路
		UI_SIMON,				// サイモンゲーム
		UI_NOWMAKING,			// 製作中
		UI_MAX					// 最大数
	};

	enum MODE
	{
		MODE_SELECT,			// 選択
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

private:
	/* メンバ関数 */
	void UpdateSelect(void);							// 選択するときの更新処理
	void UpdateShow(void);								// 解読書を表示しているときの更新処理
	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る

	/* メンバ変数 */
	static CHash *               m_pHash;				// ハッシュのポインタ
	static LPDIRECT3DTEXTURE9    m_pTexture[UI_MAX];	// テクスチャ情報
	static SETINGINFO            m_Seting[UI_MAX];		// 設定情報
	std::shared_ptr<CDecodingUI> m_pUi[UI_MAX];			// UI
	MODE                         m_mode;				// モード
};

#endif
