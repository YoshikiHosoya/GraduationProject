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
#include "Map.h"
#include "mouse.h"
//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define CAMERA_MOVE_SPEED			(10.0f)			//カメラ移動速度
#define CAMERA_HORIZON_ROTATION		(0.05f)			//カメラの横回転
#define CAMERA_VERTICAL_ROTATION	(0.02f)			//カメラの縦回転
#define DEFAULT_POS_R				(D3DXVECTOR3(0.0f,200.0f,0.0f))


#define DEFAULT_DISTANCE			(500.0f)		//カメラの距離
#define APPROACH_DISTANCE			(300.0f)		//カメラの距離

#define DEFAULT_CAMERA_ROTATION		(D3DXVECTOR3(0.13f,0.0f,0.0f))

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
	m_CameraState = CCamera::CAMERA_FIXED;
	m_MousePosCurrent = ZeroVector2;
	m_MousePosOld = ZeroVector2;
	m_MouseRotSave = ZeroVector2;

	m_bApproach = false;

	ResetCamera();

}
//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{

}
//-----------------------------------------------------------------------------
//初期化
//-----------------------------------------------------------------------------
void CCamera::Init(void)
{
	//マトリックス計算
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot, OneVector3);

	//初期の距離初期化
	m_fDefaultDistace = 0.0f;

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

		break;

	case CCamera::CAMERA_DEBUG:

		////マウスによるカメラ操作
		//Operation();

		//カメラ移動
		MoveCameraDebug();

		//カメラ回転
		RotCameraDebug();
		break;
	}
	//マトリックス計算
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot, OneVector3);

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

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------カメラ----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[W] [A] [S] [D] [Q] [E] カメラ移動 \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[左クリック] カメラ回転 \n");

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
	else if (pKeyboard->GetPress(DIK_Q))
	{
		//上に移動
		m_posRDest.x += sinf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
		m_posRDest.y += cosf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
	}
	//[3]キーを押した時
	else if (pKeyboard->GetPress(DIK_E))
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
	m_posV += (m_posVDest - m_posV) * 0.15f;

	/*---------------------------------------------
	//	注視点の更新
	---------------------------------------------*/
	// 注視点の行く先の更新
	m_posRDest.x;
	m_posRDest.y;
	m_posRDest.z;

	// 注視点の更新
	m_posR += (m_posRDest - m_posR) * 0.15f;
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

	m_posR = m_posRDest = DEFAULT_POS_R;

	m_posV.x = m_posVDest.x = m_posRDest.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	m_posV.y = m_posVDest.y = m_posRDest.y + sinf(m_rot.x)	* m_fDistance;
	m_posV.z = m_posVDest.z = m_posRDest.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------------------------------------------------------------------------
// カメラを近づける処理
//-------------------------------------------------------------------------------------------------------------
void CCamera::ApproachCamera(D3DXVECTOR3 posRDest)
{
	m_posRDest = posRDest;
	m_rot = DEFAULT_CAMERA_ROTATION;
	m_fDistance = APPROACH_DISTANCE;

	m_bApproach = true;
}

//-------------------------------------------------------------------------------------------------------------
// カメラから離れる
//-------------------------------------------------------------------------------------------------------------
void CCamera::LeaveCamera()
{
	m_fDistance = DEFAULT_DISTANCE;
	m_posRDest = DEFAULT_POS_R;
	m_bApproach = false;

}

//-------------------------------------------------------------------------------------------------------------
// 操作
//-------------------------------------------------------------------------------------------------------------
void CCamera::Operation(void)
{
	// キーボードの取得
	//Ckeyboard *pKeyboard = &CManager::GetKeyboard();

	// マウスの取得
	CMouse *pMouse = CManager::GetMouse();
	// マウスの状態を取得
	DIMOUSESTATE2* pMouseState = &pMouse->GetMouseState();

	D3DXVECTOR2 NewRotation = D3DXVECTOR2(0.0f, 0.0f);

	if (pMouse->GetPress(1) == true)
	{
		// ヨー回転
		NewRotation.y = ((float)pMouseState->lX) / (D3DX_PI*2.0f) *0.02f;
		m_rotDest.y += NewRotation.y;

		// 回転量を360度ないに直す
		CMylibrary::SetFixTheRotation(&m_rotDest.y);

		// ピッチロー回転
		NewRotation.x = ((float)pMouseState->lY) / (D3DX_PI*2.0f) *0.02f;

		// 回転を90度未満に抑える
		if (NewRotation.x >= D3DX_PI*0.49f)
		{
			NewRotation.x = D3DX_PI*0.49f;
		}
		else if (NewRotation.x <= -D3DX_PI*0.49f)
		{
			NewRotation.x = -D3DX_PI*0.49f;
		}
		m_rotDest.x += NewRotation.x;
	}

	if (pMouse->GetRelease(1))
	{
		m_MouseRotSave.y = m_rotDest.y;
		m_MouseRotSave.x = m_rotDest.x;
	}

	//// カメラの公転
	//if (pKeyboard->GetPress(DIK_RIGHTARROW))
	//{
	//	m_rot.y += CAMERA_ROTATION_SPEED;
	//}
	//else if (pKeyboard->GetPress(DIK_LEFTARROW))
	//{
	//	m_rot.y -= CAMERA_ROTATION_SPEED;
	//}
	//if (pKeyboard->GetPress(DIK_UPARROW))
	//{
	//	m_rot.x += CAMERA_ROTATION_SPEED*0.3f;
	//}
	//else if (pKeyboard->GetPress(DIK_DOWNARROW))
	//{
	//	m_rot.x -= CAMERA_ROTATION_SPEED*0.3f;
	//}
	//// 距離の倍率変更
	//if (pKeyboard->GetPress(DIK_4))
	//{
	//	m_fMagnificat += 0.01f;
	//}
	//else if (pKeyboard->GetPress(DIK_5))
	//{
	//	m_fMagnificat -= 0.01f;
	//}
	//else if (pKeyboard->GetPress(DIK_1))
	//{
	//	m_fMagnificat = 1.0f;
	//}
	//else if (pKeyboard->GetPress(DIK_2))
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	//}
	//else if (pKeyboard->GetPress(DIK_3))
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	//}

	//// 拡大率の制限
	//if (m_fMagnificat >= CAMERA_MAGNIFICAT_MIN)
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	//}
	//else if (m_fMagnificat <= CAMERA_MAGNIFICAT_MAX)
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	//}

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
	CMylibrary::SetFixTheRotation(&m_rot.y);
}
