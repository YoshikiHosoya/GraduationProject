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
	enum
	{
		TYPE_WINDOW = 0,		// ウィンドウ
		TYPE_CLOSEBUTTON,		// 閉じるボタン
		TYPE_SCROLLBAR,			// スクロールバー
		TYPE_SCROLLHANDLE,		// スクロールハンドル
		TYPE_MAX				// 最大
	};

	enum
	{
		TEX_NONE = -1,			// なし
		TEX_TELLTHEPICTURE,		// 写真を教えてください
		TEX_SHAPEMEMORIZATION,	// 形の暗記
		TEX_CORDCUTTING,		// コードを切る
		TEX_MAZE,				// 迷路
		TEX_SIMON,				// サイモンゲーム
		TEX_NOWMAKING,			// 製作中
		TEX_MAX					// 最大
	};

	CDecodingWindow();										// コンストラクタ
	~CDecodingWindow();										// デストラクタ

	static void    MakeHash(void);						// ハッシュの作成
	static void    ReleaseHash(void);					// ハッシュの開放
	static HRESULT Load(void);								// 読み込み
	static void UnLoad(void);							// 開放

	static CDecodingWindow *Create(void);	// 生成

	HRESULT        Init();									//初期化
	void           Update();								//更新
	void           Draw();									//描画

	void           Appearance(void);						// 出現
	void           Disappearance(void);						// 消滅

	void           Display(void);							// 表示させる
	void           DoNotDisplay(void);						// 表示させない

	inline void    SetSign(int nSign) { if (nSign != 0)m_nSign = abs(nSign) / nSign; }
	inline void    SetScal(float fScal) { m_fScal = fScal; }
private:
	/* メンバ関数 */
	void InitSeting(void);								// 設定の初期化
	void MakeUI(void);									// UIの作成
	void SetPosAccordingParent(void);					// 親に従って位置を設定する


	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る

	/* メンバ変数 */
	static LPDIRECT3DTEXTURE9    m_pTexture[TEX_MAX];	// テクスチャ情報
	static SETINGINFO            m_InitSeting[TYPE_MAX];// 初期設定情報
	static CHash *               m_pHash;				// ハッシュのポインタ
	std::shared_ptr<CDecodingUI> m_pUi[TYPE_MAX];		// UI

	SETINGINFO                  m_Seting[TYPE_MAX];		// 設定情報

	int                          m_nSign;				// 符号
	float                        m_fScalValue;			// 拡大値変化値
	float                        m_fScal;				// 拡大値

	// スクロール
	VEC2                         m_VecPinch_center;		// つまんだ点から中心へののベクトル
	CRange<float>                m_ScrollRange;			// スクロールの可動領域
	float                        m_fScrollRangeValue;	// スクロールの可動域値
};

#endif