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
		MODE_NONE = -1,				// 無し
		MODE_NEUTRAL,				// ニュートラル状態
		MODE_NORMAL,				// 通常
		MODE_MOVEING,				// 移動
		MODE_MAX					// 最大
	} MODE;
	enum
	{
		POS_NONE = -1,				// 無し
		POS_TABLET,					// タブレット
		POS_PEN,					// ペン
		POS_ERASER,					// 消しゴム
		POS_SEND,					// 送信
		POS_MAX,					// 最大
	};
	enum
	{
		SET_NONE = -1,				// 無し
		SET_GAME,					// ゲームの時(相対)
		SET_GAME_NEUT,				// ゲームニュートラルの時(絶対)
		SET_DECODING,				// 解読者側の時(相対)
		SET_DECODING_NEUT,			// 解読者側ニュートラルの時(絶対)
		SET_MAX						// 最大数
	};

	/* メンバ関数 */
	CTablet();																														// コンストラクタ
	~CTablet();																														// デストラクタ

	static HRESULT       Load(void);																								// 読み込み
	static HRESULT       LoadError(void);																							// エラーの検出

	HRESULT              Init();																									// 初期化
	void                 Update();																									// 更新
	void                 Draw();																									// 描画
	inline void          ShowDebugInfo() {};																						// デバッグ情報表記

	bool                 ItIsPressingButtons(void);																					// ボタンを押しているか

	void                 InitMemberVariables(void);																					// メンバ変数の初期化
	void                 NormalProc(void);																							// 通常処理
	void                 MoveingProc(void);																							// 移動処理
	void                 ConstantVelocityProc(void);																				// 等速処理
	void                 NonConstantVelocityProc(void);																				// 不等速処理
	void                 VelocityProcModeChange(void);																				// モードチェンジ

	// 生成
	static std::shared_ptr<CTablet> Create(void);																					// 生成

	// 設定関数
	inline void          SetMoveCoeff(const float fMoveCoeff) { m_fMoveCoeff = fMoveCoeff; }										// 移動係数の設定
	inline void          SetPosDest(CONST D3DXVECTOR3 &posDest) { m_posDest = posDest; }											// 目的地の設定
	inline void          SetMode(CONST MODE mode) { m_mode = mode; }																// モードの設定
	inline void          SetConstVelocity(const bool bConstVelocity) { m_bConstVelocity = bConstVelocity; }							// 等速フラグの設定
	inline void          SetMove(CONST D3DXVECTOR3& move) { m_move = move; }														// 移動量の設定
	inline static void   SetDestFrame(const int nDestFrame) { m_nDestFrame = nDestFrame; }											// 目的地までのフレームの設定

	void                 SetConstVelocityProc(const bool bConstVelocity);															// 等速処理の設定（目的地を設定する前）
	void                 SetDestinationProc(CONST D3DXVECTOR3 &posDest);															// 目的地を設定する処理

	// 取得関数
	inline float &       GetMoveCoeff(void) { return m_fMoveCoeff; }																// 移動係数の取得
	inline D3DXVECTOR3 & GetPosDest(void) { return m_posDest; }																		// 目的地の取得
	inline MODE &        GetMode(void) { return m_mode; }																			// モードの取得
	inline bool &        GetConstVelocity(void) { return m_bConstVelocity; }														// 等速フラグの取得
	inline VEC3 &        GetMove(void) { return m_move; }																			// モードの取得
	inline static int &  GetDestFrame(void) { return m_nDestFrame; }																// 目的地までのフレームの取得
	inline D3DXVECTOR3 * GetSetingPosDestPtr(const int nIndex) { return (nIndex <= -1) ? &m_aSetingPosDest[nIndex] : nullptr; }
private:
	/* メンバ関数 */
	void                 SetPosFromChattabInfo(void);																				// チャットタブの情報から位置を設定する
	static void          ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);					// 一行から読み込む
	static void          SetFromString(CONST_STRING str);																			// 文字列から設定する

	/* メンバ変数 */
	static D3DXVECTOR3                       m_aSetingModelPos[POS_MAX];									// 設定用の位置
	static D3DXVECTOR3                       m_aSetingPosDest[SET_MAX];										// 設定用の目的地
	static float                             m_fMoveCoeff;													// 移動係数
	static int                               m_nDestFrame;													// 目的地までのフレーム
	MyVector<std::shared_ptr<CTabletButton>> m_Button;														// ボタンのポインタ
	D3DXVECTOR3                              m_posDest;														// 目的地
	MODE                                     m_mode;														// モード
	bool                                     m_bConstVelocity;												// 等速フラグ
	VEC3                                     m_move;														// 移動量
	int                                      m_nCntFrame;													// フレーム数のカウント
};

#endif