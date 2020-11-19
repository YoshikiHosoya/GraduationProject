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
class CMouse;
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
		//UI_DOCUMENT,			// ドキュメント
		//UI_CLOSE,				// 閉じる
		//UI_SCROLLBAR,			// スクロールバー
		//UI_SCROLLHANDLE,		// スクロールハンドル
		UI_MAX					// 最大数
	};

	enum
	{
		TEX_BACKLIGHT,
		TEX_TELLTHEPICTURE,
		TEX_SHAPEMEMORIZATION,
		TEX_CORDCUTTING,
		TEX_MAZE,
		TEX_SIMON,
		TEX_NOWMAKING,
		//TEX_D_TELLTHEPICTURE,
		//TEX_D_SHAPEMEMORIZATION,
		//TEX_D_CORDCUTTING,
		//TEX_D_MAZE,
		//TEX_D_SIMON,
		//TEX_D_NOWMAKING,
		//TEX_CLOSE,
		//TEX_SCROLLBAR,
		//TEX_SCROLLHANDLE,
		TEX_MAX
	};

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
	void SetPosAccordingParent(void);					// 親に従って位置を設定する
	void InitDocumentUVInfo(void);						// ドキュメントのUV情報の初期化
	void InitConnect(void);								// 接続モードの初期化
	void UpdateSelect(void);							// 選択するときの更新処理
	void UpdateConnect(void);							// 接続モード更新処理
	void UpdateShow(void);								// 解読書を表示しているときの更新処理

	void CloseButtonProc(CMouse *pMouse, D3DXVECTOR2 *pMousePos);		// 閉じるボタンの処理
	void ScrollProc(CMouse *pMouse, D3DXVECTOR2 *pMousePos);			// スクロールの処理
	void PinchScrollHandle(CMouse * pMouse, D3DXVECTOR2 * pMousePos);	// スクロールハンドルをつまむ
	void MoveScrollHandle(CMouse * pMouse, D3DXVECTOR2 * pMousePos);	// スクロールハンドルを動かす
	void MouseWheelScroll(D3DXVECTOR2 * pMousePos);						// マウスホイールスクロール
	float ScrollClamp(D3DXVECTOR2 *pMousePos, float fPosY);				// スクロールのクランプ
	void ScrollThroughDocument(void);									// ドキュメントのスクロール

	void DisplayDocumentsInPlace(void);					// ドキュメントを特定の位置に表示する

	void AppeaDocument(int nCntUi);						// ドキュメントの出現
	void DisAppeaDocument(void);						// ドキュメントの消滅

	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る

	/* メンバ変数 */
	static CHash *               m_pHash;				// ハッシュのポインタ
	static LPDIRECT3DTEXTURE9    m_pTexture[TEX_MAX];	// テクスチャ情報
	static SETINGINFO            m_Seting[UI_MAX];		// 設定情報
	static int                   m_nFrameMax;			// 最大フレーム
	std::shared_ptr<CDecodingUI> m_pUi[UI_MAX];			// UI
	MODE                         m_mode;				// モード
	MODE                         m_modeNext;			// 次のモード
	int                          m_nSelectIndex;		// 選択している番号

	int                          m_nFrame;				// フレームカウント
	//int                          m_nSign;				// 符号
	//float                        m_fScalValue;			// 拡大値変化値
	//float                        m_fScal;				// 拡大値

	//VEC2                         m_VecPinch_center;		// つまんだ点から中心へののベクトル

	//RANGE                        m_ScrollRange;			// スクロールの可動領域
	//float                        m_fScrollRangeValue;	// スクロールの可動域値

};

#endif
