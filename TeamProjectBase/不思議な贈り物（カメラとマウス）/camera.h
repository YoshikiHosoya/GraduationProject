//*************************************************************************************************************
//
// カメラ処理 [camera.h]
// Author : IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CAMERA_FOLLOW_ROTCOEFFICIENT	(0.2f)									// 追従係数(回転)
#define CAMERA_FOLLOW_COEFFICIENT		(0.3f)									// 追従係数
#define CAMERA_MAGNIFICAT_MIN			(5.0f)									// 最大拡大率
#define CAMERA_MAGNIFICAT_MAX			(0.25f)									// 最小拡大率
#define CAMERA_MAGNIFICAT_ADDVALUE		(0.01f)									// 拡大率の加算値
#define CAMERA_MAGNIFICAT_UNSET			(1.7f)									// 拡大率の初期値(設定なし)
#define CAMERA_MAGNIFICAT_SETTITLE		(4.5f)									// 拡大率の初期値(タイトルモード)
#define CAMERA_MAGNIFICAT_SETRESULT		(4.5f)									// 拡大率の初期値(リザルトモード)
#define CAMERA_MAGNIFICAT_SETSELECT		(1.36f)									// 拡大率の初期値(選択モード)
#define CAMERA_ROTATION_SPEED			(0.05f)									// 回転速度
#define CAMERA_ROT_UNSET				(D3DXVECTOR3(0.35f, 0.0f, 0.0f))		// 回転の初期値(設定なし)
#define CAMERA_ROT_SETSELECT			(D3DXVECTOR3(0.25f, 0.0f, 0.0f))		// 回転の初期値(選択モード)

#define CAMERA_ROTSPEED_RESULT_TITLE	(0.001f)								// リザルトとタイトルの回転速度


#define CAMERA_HEADPOS_UNSET			(MYLIB_3DVECTOR_ZERO)					// 向かう位置の初期値(設定なし)
#define CAMERA_HEADPOS_SETSELECT		(D3DXVECTOR3(-100.0f, 100.0f, 0.0f))	// 向かう位置の初期値(選択モード)
#define CAMARA_INITPOS_TITLE			(D3DXVECTOR3(0.0f,0.0f,0.0f))			// タイトルの初期化位置
//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CCamera
{
public:
	// コンストラクタ
	CCamera();
	// デストラクタ
	~CCamera();
	// 生成
	static CCamera *Create(void);
	// 初期化
	void Init(void);
	// 終了
	void Uninit(void);
	// 更新
	void Update(void);
	// 設定
	void Set(void);
	// 操作
	void Operation(void);
	// カメラが向いている方向の取得
	D3DXVECTOR3 *GetDirecVector(void);
	// カメラが向いている方向の取得(Y軸なし)
	D3DXVECTOR3 GetFlatDirecVector(void);
	// カメラの位置と向きと距離の拡大率の設定
	void SetCameraPosAndMagni(D3DXVECTOR3 HeadPos, D3DXVECTOR3 rot,float fMagnificat);

	// カメラの回転の設定
	void SetRotation(D3DXVECTOR3 *rotation);
	// カメラの回転の設定(Y軸)
	void SetYawRotation(float fYawrot);
	// カメラの回転の設定(X軸)
	void SetRollRotation(float fRollrot);
	// カメラの回転の保存用設定(X軸)
	void SetRollRotationsave(float fRollrotsave);
	// カメラの回転の取得
	D3DXVECTOR3 *GetRotation(void);
	// カメラの回転の取得(Y軸)
	float GetYawRotation(void);
	// カメラの回転の取得(X軸)
	float GetRollRotation(void);
	// カメラの回転の保存用取得(X軸)
	float GetRollRotationsave(void);
private:
	D3DXVECTOR3	m_posR;				// 注視点
	D3DXVECTOR3	m_PosRDest;			// 注視点の行く先
	D3DXVECTOR3	m_posV;				// 視点
	D3DXVECTOR3	m_PosVDest;			// 視点の行く先
	D3DXVECTOR3 m_vecU;				// ベクトル
	D3DXVECTOR3	m_rot;				// 視点の回転
	D3DXVECTOR3 m_move;				// 移動量
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	float		m_fLength;			// 長さ
	float		m_fMagnificat;		// 長さの変化率
	D3DXVECTOR3 m_DirecVector;		// 向いてるベクトル
	D3DXVECTOR3 m_HeadPos;			// 向かう位置
	float		m_fRollRotationsave;// X軸回転保存用
};


#endif
