//*************************************************************************************************************
//
// タブレット処理 [tablet.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _TABLET_H_
#define _TABLET_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "sceneX.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CTabletButton;
class CTablet : public CSceneX
{
public:
	/* 列挙型定義 */
	typedef enum
	{
		MODE_NONE = -1,
		MODE_NORMAL,
		MODE_MOVEING,
		MODE_MAX
	} MODE;

	/* メンバ関数 */
	CTablet();																								// コンストラクタ
	~CTablet();																								// デストラクタ

	HRESULT              Init();																			// 初期化
	void                 Update();																			// 更新
	void                 Draw();																			// 描画
	inline void          ShowDebugInfo() {};																// デバッグ情報表記

	bool                 ItIsPressingButtons(void);															// ボタンを押しているか

	void                 InitMemberVariables(void);															// メンバ変数の初期化
	void                 NormalProc(void);																	// 通常処理
	void                 MoveingProc(void);																	// 移動処理
	void                 ConstantVelocityProc(void);														// 等速処理
	void                 NonConstantVelocityProc(void);														// 不等速処理

	// 生成
	static std::shared_ptr<CTablet> Create(CONST D3DXVECTOR3 &pos);											// 生成

	// 設定関数
	inline void          SetMoveCoeff(const float fMoveCoeff) { m_fMoveCoeff = fMoveCoeff; }				// 移動係数の設定
	inline void          SetPosDest(CONST D3DXVECTOR3 &posDest) { m_posDest = posDest; }					// 目的地の設定
	inline void          SetMode(CONST MODE mode) { m_mode = mode; }										// モードの設定
	inline void          SetConstVelocity(const bool bConstVelocity) { m_bConstVelocity = bConstVelocity; }	// 等速フラグの設定
	inline void          SetMove(CONST D3DXVECTOR3& move) { m_move = move; }								// 移動量の設定
	inline void          SetDestFrame(const int nDestFrame) { m_nDestFrame = nDestFrame; }					// 目的地までのフレームの設定

	void                 SetConstVelocityProc(const bool bConstVelocity, const int nDestFrame);				// 等速処理の設定（目的地を設定する前）
	void                 SetDestinationProc(CONST D3DXVECTOR3 &posDest);									// 目的地を設定する処理

	// 取得関数
	inline float &       GetMoveCoeff(void) { return m_fMoveCoeff; }										// 移動係数の取得
	inline D3DXVECTOR3 & GetPosDest(void) { return m_posDest; }												// 目的地の取得
	inline MODE &        GetMode(void) { return m_mode; }													// モードの取得
	inline bool &        GetConstVelocity(void) { return m_bConstVelocity; }								// 等速フラグの取得
	inline VEC3 &        GetMove(void) { return m_move; }													// モードの取得
	inline int &         GetDestFrame(void) { return m_nDestFrame; }										// 目的地までのフレームの取得

private:
	/* メンバ変数 */
	MyVector<std::shared_ptr<CTabletButton>> m_Button;														// ボタンのポインタ
	float                                    m_fMoveCoeff;													// 移動係数
	D3DXVECTOR3                              m_posDest;														// 目的地
	MODE                                     m_mode;														// モード
	bool                                     m_bConstVelocity;												// 等速フラグ
	VEC3                                     m_move;														// 移動量
	int                                      m_nDestFrame;													// 目的地までのフレーム
	int                                      m_nCntFrame;													// フレーム数のカウント
};

#endif