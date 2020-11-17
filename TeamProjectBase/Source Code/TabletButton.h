//*************************************************************************************************************
//
// タブレットボタンの基底処理 [TabletButton.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _TABLETBUTTON_H_
#define _TABLETBUTTON_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "sceneX.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene3D;
class CPicture;
class CPaintingPen;
class CTabletButton : public CSceneX
{
public:
	/* 列挙型定義 */
	typedef enum
	{
		TYPE_NOEN = -1,	// 無し
		TYPE_PEN,		// ペン
		TYPE_ERASER,	// 消しゴム
		TYPE_SEND,		// 送信
		TYPE_MAX,		// 最大
	} TYPE;
	enum PRESS
	{
		PRESSED = 0,	// 押された
		UNPRESSED,		// 押されていない
		PRESS_MAX,		// 最大
	};


	/* メンバ関数 */
	CTabletButton();																// コンストラクタ
	~CTabletButton();																// デストラクタ

	static void    Load(void);														// 読み込み
	static void    Unload(void);													// 開放
	static HRESULT LoadError(void);													// エラーの検出

	HRESULT     Init();																// 初期化
	void        Update();															// 更新
	void        Draw();																// 描画
	inline void ShowDebugInfo() {};													// デバッグ情報表記

	// 設定関数
	static void SetFromString(CONST_STRING str);									// 文字列から設定する
	void        FlagProcToChangeMode(void);											// モードを変更するためのフラグ処理
	void        SetChangeFlagProc(void);											// 変更フラグの設定処理
	inline void SetType(TYPE type) { m_Type = type; }								// タイプの設定
	inline void SetToOffPressFlag(void) { if (m_bPress)m_bPress = false; }			// 押されたフラグを消す設定
	inline void SetToSetPressFlag(void) { if (!m_bPress)m_bPress = true; }			// 押されたフラグを立てる設定
	inline void SetToOffChangeFlag(void) { if (m_bChange)m_bChange = false; }		// 変更フラグを消す設定
	inline void SetToSetChangeFlag(void) { if (!m_bChange)m_bChange = true; }		// 変更フラグを立てる設定
	inline void SetToOffInPressFlag(void) { if (m_bInPress)m_bInPress = false; }	// 内側で押されたフラグを消す設定
	inline void SetToSetInPressFlag(void) { if (!m_bInPress)m_bInPress = true; }	// 内側で押されたフラグを立てる設定

	// 取得関数
	inline TYPE &GetType(void) { return m_Type; }									// タイプの取得
	inline bool GetPressFlag(void) { return m_bPress; }							// 押されたフラグの取得
	inline bool GetChangeFlag(void) { return m_bChange; }							// 変更フラグフラグの取得
	inline bool GetInPressFlag(void) { return m_bInPress; }						// 内側で押されたフラグの取得

	// 生成
	static std::shared_ptr<CTabletButton> Create(D3DXMATRIX *pMtxParent,CONST D3DXVECTOR3 &pos, TYPE type);	// 生成

private:
	/* メンバ関数 */
	static CPicture *GetPicture(void);					// ピクチャの取得
	static CPaintingPen *GetPen(void);					// ペンの取得
	/* メンバ変数 */
	static LPDIRECT3DTEXTURE9 m_aTexture[TYPE_MAX];		// テクスチャ情報
	static float              m_aPressPos[PRESS_MAX];	// 押された位置
	std::shared_ptr<CScene3D> m_pImage;					// 画像
	TYPE                      m_Type;					// タイプ
	bool                      m_bPress;					// 押されたフラグ
	bool                      m_bChange;				// 変更フラグ
	bool                      m_bInPress;				// 内側で押されたフラグ

};

#endif