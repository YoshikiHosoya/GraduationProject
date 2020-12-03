//*************************************************************************************************************
//
// 解読書ウィンドウ[処理 [DecodingWindow.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _DECODINGWINDOW_H_
#define _DECODINGWINDOW_H_

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
class CDecodingWindow
{
public:
	// UIタイプ
	enum
	{
		TYPE_WINDOW = 0,			// ウィンドウ
		TYPE_CLOSEBUTTON,			// 閉じるボタン
		TYPE_SCROLLBAR,				// スクロールバー
		TYPE_SCROLLHANDLE,			// スクロールハンドル
		TYPE_MAX					// 最大
	};
	// UIのテクスチャ
	enum
	{
		TEX_NONE = -1,				// なし
		TEX_CLOSEBUTTON,			// 閉じるボタン
		TEX_SCROLLBAR,				// スクロールバー
		TEX_SCROLLHANDLE,			// スクロールハンドル
		TEX_MAX						// 最大
	};
	// ウィンドウタイプのテクスチャ
	enum
	{
		TEX_W_NONE = -1,			// なし
		TEX_W_TELLTHEPICTURE,		// 写真を教えてください
		TEX_W_SHAPEMEMORIZATION,	// 形の暗記
		TEX_W_CORDCUTTING,			// コードを切る
		TEX_W_MAZE,					// 迷路
		TEX_W_SIMON,				// サイモンゲーム
		TEX_W_NOWMAKING,			// 製作中
		TEX_W_MAX					// 最大
	};

	CDecodingWindow();																		// コンストラクタ
	~CDecodingWindow();																		// デストラクタ

	static void    MakeHash(void);															// ハッシュの作成
	static void    ReleaseHash(void);														// ハッシュの開放
	static HRESULT Load(void);																// 読み込み
	static void    UnLoad(void);															// 開放

	static std::shared_ptr<CDecodingWindow> Create(WINDOW_SETING &Seting);					// 生成

	HRESULT        Init(WINDOW_SETING &Seting);												//初期化
	void           Update();																//更新
	void           Draw();																	//描画

	void           InitAppearInfo(void);													// 出現情報の初期化
	void           InitDocumentUVInfo(WINDOW_SETING &Seting);								// ドキュメントのUV情報の初期化
	
	void           PreparingAppear(int nCntUi);												// 出現準備

	bool           Appearance(void);														// 出現
	bool           Disappearance(void);														// 消滅

	void           Display(void);															// 表示させる
	void           DoNotDisplay(void);														// 表示させない

	inline void    SetSign(int nSign) { if (nSign != 0)m_nSign = abs(nSign) / nSign; }		// 符号の設定
	inline void    SetScal(float fScal) { m_fScal = fScal; }								// スケールの設定
	inline static void SetScalMax(float fScal) { m_fSclaMax = fScal; }
	bool           CloseButtonProc(CMouse *pMouse, D3DXVECTOR2 *pMousePos);					// 閉じるボタンの処理
	void           ScrollProc(CMouse *pMouse, D3DXVECTOR2 *pMousePos);						// スクロール処理
	void           MoveScrollHandle(CMouse * pMouse, D3DXVECTOR2 * pMousePos);				// スクロールハンドルを動かす
	void           MouseWheelScroll(D3DXVECTOR2 * pMousePos);								// マウスホイールスクロール
	float          ScrollClamp(D3DXVECTOR2 *pMousePos, float fPosY);						// スクロールのクランプ
	void           ScrollScreen(void);														// 画面のスクロール

private:
	/* メンバ関数 */
	void InitSeting(WINDOW_SETING &Seting);					// 設定の初期化
	void MakeUI(void);										// UIの作成
	void SetPosAccordingParent(void);						// 親に従って位置を設定する
	void ChangeColor(CDecodingUI *pUi, D3DXCOLOR &col);		// 色を変更

	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る

	/* メンバ変数 */
	static LPDIRECT3DTEXTURE9    m_pTexture[TEX_MAX];		// テクスチャ情報
	static LPDIRECT3DTEXTURE9    m_pDocTexture[TEX_W_MAX];	// ウィンドウのテクスチャ情報
	static SETINGINFO            m_InitSeting[TYPE_MAX];	// 初期設定情報
	static CHash *               m_pHash;					// ハッシュのポインタ
	static int                   m_nFrameMax;				// 最大フレーム

	static FLOAT2                m_SpectRatio[TYPE_MAX];	// アスペクト比
	static FLOAT2                m_WindSizeScal[TYPE_MAX];	// ウィンドウの拡縮率

	static float                 m_fSclaMax;				// スケールの最大

	std::shared_ptr<CDecodingUI> m_pUi[TYPE_MAX];			// UI

	SETINGINFO                   m_Seting[TYPE_MAX];		// 設定情報

	int                          m_nSelectIndex;			// 選択している番号
	int                          m_nFrame;					// フレーム
	int                          m_nSign;					// 符号
	float                        m_fScalValue;				// 拡大値変化値
	float                        m_fScal;					// 拡大値

	bool                         m_bActive;					// アクティブフラグ

	// スクロール
	VEC2                         m_VecPinch_Center;			// つまんだ点から中心へのベクトル
	float                        m_fScrInitPos;				// スクロール初期位置
	CRange<float>                m_ScrollRange;				// スクロールの可動領域
	float                        m_fScrollRangeValue;		// スクロールの可動域値

	FLOAT2                       m_AspectRatio;
	FLOAT2                       m_SizeScal;
};

#endif