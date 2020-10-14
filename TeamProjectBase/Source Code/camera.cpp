//-----------------------------------------------------------------------------
//
// カメラ処理 [Camera.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "collision.h"
#include "Map.h"
//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define CAMERA_MOVE_SPEED			(10.0f)			//カメラ移動速度
#define CAMERA_HORIZON_ROTATION		(0.05f)			//カメラの横回転
#define CAMERA_VERTICAL_ROTATION	(0.02f)			//カメラの縦回転

#define DEFAULT_DISTANCE			(500.0f)		//カメラの距離
#define DEFAULT_CAMERA_ROTATION		(D3DXVECTOR3(0.17f,0.0f,0.0f))

#define CAMERA_LENGTH_NEAR			(10.0f)			//カメラの見える距離（近）
#define CAMERA_LENGTH_FAR			(5000.0f)		//カメラの見える距離（遠）


//-----------------------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//コンストラクタ
//-----------------------------------------------------------------------------
CCamera::CCamera()
{
	//初期化
	m_posR = ZeroVector3;
	m_posV = ZeroVector3;
	m_posVOld = ZeroVector3;
	m_posRDest = ZeroVector3;
	m_posVDest = ZeroVector3;
	m_rot = ZeroVector3;
	m_rotDest = ZeroVector3;
	m_move = ZeroVector3;
	m_fDistance = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pCollision = nullptr;
	m_CameraState = CCamera::CAMERA_FIXED;
	m_MousePosCurrent = ZeroVector2;
	m_MousePosOld = ZeroVector2;
	m_MouseRotSave = ZeroVector2;

	ResetCamera();

}
//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{
	m_pCollision.reset();
}
//-----------------------------------------------------------------------------
//初期化
//-----------------------------------------------------------------------------
void CCamera::Init(void)
{
	//マトリックス計算
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot);

	//コリジョン生成
	m_pCollision = CCollision::Create(&m_mtxWorld, CCollision::COLLISION_CAMERA);

	//コリジョン情報設定
	m_pCollision->SetCollisionInfo(10.0f, 10.0f);

	//初期の距離初期化
	m_fDefaultDistace = 0.0f;

}
//-----------------------------------------------------------------------------
//終了
//-----------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}
//-----------------------------------------------------------------------------
//更新
//-----------------------------------------------------------------------------
void CCamera::Update(void)
{
	//1F前の座標保存
	m_posVOld = m_posV;

	switch (m_CameraState)
	{

		//ゲーム中
	case CCamera::CAMERA_PLAYER_FOLLOWING:
		//初期の距離に縮める
		m_fDistance += (m_fDefaultDistace - m_fDistance) * 0.03f;

		//カメラ移動
		MoveCameraGame();

		//カメラ回転
		RotCameraGame();

		if (m_fDistance <= 700.0f)
		{

		}
		break;

	case CCamera::CAMERA_DEBUG:
		//カメラ移動
		MoveCameraDebug();

		//カメラ回転
		RotCameraDebug();
		break;
	}
	//マトリックス計算
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot);

}
//-----------------------------------------------------------------------------
//設定処理
//-----------------------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),	//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//アスペクト比
		CAMERA_LENGTH_NEAR,								//NearZ値
		CAMERA_LENGTH_FAR);								//FarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}

//-----------------------------------------------------------------------------
//カメラ情報
//-----------------------------------------------------------------------------
void CCamera::ShowCameraInfo()
{
#ifdef _DEBUG

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (ImGui::CollapsingHeader("CameraInfo"))
	{
		ImGui::InputFloat3("PosV", m_posV);
		ImGui::InputFloat3("PosR", m_posR);
		ImGui::InputFloat3("Rot", m_rot);
		ImGui::InputFloat3("RotDest", m_rotDest);
		ImGui::InputFloat("distance", &m_fDistance, 10.0f, 10.0f);

		ImGui::Text("ViewVec %.2f %.2f %.2f", m_mtxView._31, m_mtxView._32, m_mtxView._33);
	}
#endif //DEBUG
}

//-----------------------------------------------------------------------------
//移動処理
//-----------------------------------------------------------------------------
void CCamera::MoveCameraDebug(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//[W]キーを押した時
	if (pKeyboard->GetPress(DIK_W))
	{
		//奥
		m_posRDest.x += sinf(D3DX_PI * 0.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 0.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[S]キーを押した時
	if (pKeyboard->GetPress(DIK_S))
	{
		//前
		m_posRDest.x += sinf(D3DX_PI * 1.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 1.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[A]キーを押した時
	if (pKeyboard->GetPress(DIK_A))
	{
		//左
		m_posRDest.x += sinf(-D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(-D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[D]キーを押した時
	if (pKeyboard->GetPress(DIK_D))
	{
		//右
		m_posRDest.x += sinf(D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}

	//[1]キーを押した時
	else if (pKeyboard->GetPress(DIK_1))
	{
		//上に移動
		m_posRDest.x += sinf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
		m_posRDest.y += cosf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
	}
	//[3]キーを押した時
	else if (pKeyboard->GetPress(DIK_3))
	{
		//下に移動
		m_posRDest.x += sinf(D3DX_PI * 1.0f) * CAMERA_MOVE_SPEED;
		m_posRDest.y += cosf(D3DX_PI * 1.0f) * CAMERA_MOVE_SPEED;
	}
}
//-----------------------------------------------------------------------------
//回転処理
//-----------------------------------------------------------------------------
void CCamera::RotCameraDebug(void)
{
	//3.14の範囲に抑える
	CHossoLibrary::CalcRotation(m_rotDest.y);

	//回転先
	m_rot = m_rotDest;

	/*---------------------------------------------
	//	視点の更新
	---------------------------------------------*/
	// 視点の行く先の更新
	m_posVDest.x = m_posR.x - cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posR.z - cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// 視点の更新
	m_posV += (m_posVDest - m_posV) * 0.2f;

	/*---------------------------------------------
	//	注視点の更新
	---------------------------------------------*/
	// 注視点の行く先の更新
	m_posRDest.x;
	m_posRDest.y;
	m_posRDest.z;

	// 注視点の更新
	m_posR += (m_posRDest - m_posR) * 0.2f;
}

//-----------------------------------------------------------------------------
//移動処理
//-----------------------------------------------------------------------------
void CCamera::MoveCameraGame(void)
{
	CKeyboard	*pKeyboard	= CManager::GetKeyboard();

	float fCameraRotation_X = CAMERA_HORIZON_ROTATION;
	float fCameraRotation_Y = CAMERA_VERTICAL_ROTATION;

	// 上昇
	if (pKeyboard->GetPress(DIK_W))
	{
		// 時計回りに回転
		m_rotDest.x += fCameraRotation_Y;
	}

	// 下降
	if (pKeyboard->GetPress(DIK_S))
	{
		// 時計回りに回転
		m_rotDest.x -= fCameraRotation_Y;
	}

	// 視点の回転
	if (pKeyboard->GetPress(DIK_D))
	{
		// 時計回りに回転
		m_rotDest.y -= fCameraRotation_X;
	}

	if (pKeyboard->GetPress(DIK_A))
	{
		// 時計回りに回転
		m_rotDest.y += fCameraRotation_X;
	}

	//右スティックでカメラの回転
	CHossoLibrary::Check3DCameraStick(m_rotDest, fCameraRotation_X, fCameraRotation_Y);

	//縦方向　回転制限
	if (m_rotDest.x < 2.0f)
	{
		m_rotDest.x = 2.0f;
	}
	if (m_rotDest.x > 3.1f)
	{
		m_rotDest.x = 3.1f;
	}

	//3.14の範囲に抑える
	CHossoLibrary::CalcRotation(m_rotDest.y);
}
//-----------------------------------------------------------------------------
//回転処理
//-----------------------------------------------------------------------------
void CCamera::RotCameraGame(void)
{
	//キーボードのポインタ
	CKeyboard	*pKeyboard = CManager::GetKeyboard();

	// posRとposVのそれぞれの差分
	D3DXVECTOR3 difposR, difposV;

	////プレイヤーの情報取得
	//CPlayer *pPlayer = CManager::GetBaseMode()->GetPlayer();

	////nullcheck
	//if (pPlayer)
	//{
	//	//3Dのプレイヤーにキャスト
	//	CPlayer_3D *pPlayer3D = dynamic_cast<CPlayer_3D*>(pPlayer);		//注視点をプレイヤーに合わす
	//	//nullcheck
	//	if (pPlayer3D)
	//	{
	//		m_posRDest = pPlayer3D->GetPos();
	//	}
	//}

	//回転の差分
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;

	//3.14超えないようにする
	CHossoLibrary::CalcRotation(rotDiff.x);
	CHossoLibrary::CalcRotation(rotDiff.y);

	//回転
	m_rot += rotDiff * 0.1f;

	// 現在の注視点と目的の注視点の差分の計算
	difposR = m_posRDest - m_posR;

	// 注視点を追いかける速度
	m_posR += difposR * 0.1f;

	m_posVDest.x = m_posR.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posR.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// 現在の視点と目的の視点の差分の計算
	difposV = m_posVDest - m_posV;

	// 視点が注視点に追いつく速度
	m_posV += difposV * 0.15f;

	//3.14超えないようにする
	CHossoLibrary::CalcRotation(m_rot.x);
	CHossoLibrary::CalcRotation(m_rot.y);

}

//-----------------------------------------------------------------------------
//カメラを初期座標に設定
//-----------------------------------------------------------------------------
void CCamera::ResetCamera()
{
	//初期座標へ
	m_rotDest = m_rot = DEFAULT_CAMERA_ROTATION;
	m_fDistance = DEFAULT_DISTANCE;

	m_posV.x = m_posVDest.x = m_posRDest.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	m_posV.y = m_posVDest.y = m_posRDest.y + sinf(m_rot.x)	* m_fDistance;
	m_posV.z = m_posVDest.z = m_posRDest.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}