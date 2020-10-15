//*************************************************************************************************************
//
// 入力処理[keyboard.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "camera.h"
#include "keyboard.h"
#include "SceneX.h"
#include "DebugProc.h"
#include "gamepad.h"
#include "game.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CAMERA_V_POS_X					(100.0f)							// 視点のポジションX
#define CAMERA_V_POS_Y					(100.0f)							// 視点のポジションY
#define CAMERA_V_POS_Z					(100.0f)							// 視点のポジションZ

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CCamera::CCamera()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CCamera * CCamera::Create(void)
{
	CCamera *pCamera = new CCamera;
	pCamera->Init();
	return pCamera;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CCamera::Init(void)
{
	// 構造体の初期化
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	// 選択モードの時
	m_rot = CAMERA_ROT_UNSET;
	m_HeadPos = CAMERA_HEADPOS_UNSET;
	m_fMagnificat = CAMERA_MAGNIFICAT_UNSET;
	// 長さ
	m_fLength = sqrtf(CAMERA_V_POS_X*CAMERA_V_POS_Y*CAMERA_V_POS_Z) / 3;	// / xyz分
	// X軸回転保存用
	m_fRollRotationsave = m_rot.x;
	// 視点の行く先
	m_PosVDest.x = m_HeadPos.x - cosf(m_rot.x)*sinf(m_rot.y)*m_fLength*m_fMagnificat;
	m_PosVDest.y = m_HeadPos.y + sinf(m_rot.x)*m_fLength*m_fMagnificat;
	m_PosVDest.z = m_HeadPos.z - cosf(m_rot.x)*cosf(m_rot.y)*m_fLength*m_fMagnificat;
	// 視点の更新
	m_posV.x += (m_PosVDest.x - m_posV.x);
	m_posV.y += (m_PosVDest.y - m_posV.y);
	m_posV.z += (m_PosVDest.z - m_posV.z);

	// 注視点の行く先
	m_PosRDest.x = m_HeadPos.x;
	m_PosRDest.y = m_HeadPos.y;
	m_PosRDest.z = m_HeadPos.z;
	// 注視点の更新
	m_posR.x += (m_PosRDest.x - m_posR.x);
	m_posR.y += (m_PosRDest.y - m_posR.y);
	m_posR.z += (m_PosRDest.z - m_posR.z);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CCamera::Update(void)
{
	// カメラの操作理
	CCamera::Operation();

	/*---------------------------------------------
	//	視点の更新
	---------------------------------------------*/
	// 視点の行く先の更新
	m_PosVDest.x = m_HeadPos.x - cosf(m_rot.x)*sinf(m_rot.y)*m_fLength*m_fMagnificat;
	m_PosVDest.y = m_HeadPos.y + sinf(m_rot.x)*m_fLength*m_fMagnificat;
	m_PosVDest.z = m_HeadPos.z - cosf(m_rot.x)*cosf(m_rot.y)*m_fLength*m_fMagnificat;
	// 視点の更新
	m_posV += (m_PosVDest - m_posV)*CAMERA_FOLLOW_COEFFICIENT;

	/*---------------------------------------------
	//	注視点の更新
	---------------------------------------------*/
	// 注視点の行く先の更新
	m_PosRDest.x = m_HeadPos.x;
	m_PosRDest.y = m_HeadPos.y;
	m_PosRDest.z = m_HeadPos.z;
	// 注視点の更新
	m_posR += (m_PosRDest - m_posR)*CAMERA_FOLLOW_COEFFICIENT;

	// 向いている方向を算出
	m_DirecVector = m_posR - m_posV;
	// 単位ベクトルの生成
	CMylibrary::CreateUnitVector(&m_DirecVector, &m_DirecVector);
}

//-------------------------------------------------------------------------------------------------------------
// 設定
//-------------------------------------------------------------------------------------------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),							// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
		10.0f,											// NearZ値
		10000.0f);										// FearZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//-------------------------------------------------------------------------------------------------------------
// 操作
//-------------------------------------------------------------------------------------------------------------
void CCamera::Operation(void)
{
	// キーボードの取得
	Ckeyboard *pKeyboard = CManager::GetKeyboard();
	CGamepad *pGamepad = CManager::GetGamepad();
	// カメラの公転
	if (pKeyboard->GetPress(DIK_RIGHTARROW))
	{
		m_rot.y += CAMERA_ROTATION_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_LEFTARROW))
	{
		m_rot.y -= CAMERA_ROTATION_SPEED;
	}
	if (pKeyboard->GetPress(DIK_UPARROW))
	{
		m_rot.x += CAMERA_ROTATION_SPEED*0.3f;
		m_fRollRotationsave += CAMERA_ROTATION_SPEED*0.3f;
	}
	else if (pKeyboard->GetPress(DIK_DOWNARROW))
	{
		m_rot.x -= CAMERA_ROTATION_SPEED*0.3f;
		m_fRollRotationsave -= CAMERA_ROTATION_SPEED*0.3f;
	}

	// 変数宣言
	int nValueH = 0;//スティックの横
	int nValueV = 0;//スティックの縦
					//ジョイパッドでの処理
	pGamepad->GetStickRight(0, &nValueH, &nValueV);
	// 入力情報が存在するとき
	if (nValueH != 0)
	{
		m_rot.y += (nValueH / 1024) * CAMERA_ROTATION_SPEED;
	}
	if ( nValueV != 0)
	{
		m_rot.x -= (nValueV / 1024) * CAMERA_ROTATION_SPEED*0.3f;
		m_fRollRotationsave -= (nValueV / 1024) * CAMERA_ROTATION_SPEED*0.3f;
	}
	// 距離の倍率変更
	if (pKeyboard->GetPress(DIK_4))
	{
		m_fMagnificat += 0.01f;
	}
	else if (pKeyboard->GetPress(DIK_5))
	{
		m_fMagnificat -= 0.01f;
	}
	else if (pKeyboard->GetPress(DIK_1))
	{
		m_fMagnificat = 1.0f;
	}
	else if (pKeyboard->GetPress(DIK_2))
	{
		m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	}
	else if (pKeyboard->GetPress(DIK_3))
	{
		m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	}

	// 拡大率の制限
	if (m_fMagnificat >= CAMERA_MAGNIFICAT_MIN)
	{
		m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	}
	else if (m_fMagnificat <= CAMERA_MAGNIFICAT_MAX)
	{
		m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	}

	// カメラの回転を90度未満に抑える
	if (m_rot.x >= D3DX_PI*0.49f)
	{
		m_rot.x = D3DX_PI*0.49f;
	}
	else if (m_rot.x <= -D3DX_PI*0.49f)
	{
		m_rot.x = -D3DX_PI*0.49f;
	}
	// 回転量を360度ないに直す
	CMylibrary::FixTheRotation(&m_rot.y);
}

//-------------------------------------------------------------------------------------------------------------
// カメラが向いている方向を取得する
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCamera::GetDirecVector(void)
{
	return &m_DirecVector;
}

//-------------------------------------------------------------------------------------------------------------
// カメラが向いている平面での方向を取得する
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCamera::GetFlatDirecVector(void)
{
	return D3DXVECTOR3(m_DirecVector.x, 0.0f, m_DirecVector.z);
}

//-------------------------------------------------------------------------------------------------------------
// カメラの位置と向きと距離の拡大率の設定
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetCameraPosAndMagni(D3DXVECTOR3 HeadPos, D3DXVECTOR3 rot, float fMagnificat)
{
	m_HeadPos		= HeadPos;		// 向かう位置
	m_rot			= rot;			// 向き
	m_fMagnificat	= fMagnificat;	// 拡大率
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の設定
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetRotation(D3DXVECTOR3 * rotation)
{
	m_rot = *rotation;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の設定(Y軸)
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetYawRotation(float fYawrot)
{
	m_rot.y = fYawrot;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の設定(X軸)
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetRollRotation(float fRollrot)
{
	m_rot.x = fRollrot;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の保存用設定(X軸)
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetRollRotationsave(float fRollrotsave)
{
	m_fRollRotationsave = fRollrotsave;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCamera::GetRotation(void)
{
	return &m_rot;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の取得(Y軸)
//-------------------------------------------------------------------------------------------------------------
float CCamera::GetYawRotation(void)
{
	return m_rot.y;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の取得(X軸)
//-------------------------------------------------------------------------------------------------------------
float CCamera::GetRollRotation(void)
{
	return m_rot.x;
}

//-------------------------------------------------------------------------------------------------------------
// カメラの回転の保存用取得(X軸)
//-------------------------------------------------------------------------------------------------------------
float CCamera::GetRollRotationsave(void)
{
	return m_fRollRotationsave;
}
