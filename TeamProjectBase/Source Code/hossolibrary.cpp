//------------------------------------------------------------------------------
//
//ライブラリ処理  [HossoLibrary.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "hossolibrary.h"
#include "manager.h"
#include "keyboard.h"
#include "renderer.h"
#include "light.h"
#include "sound.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "particle.h"
#include "modelinfo.h"
#include "Pad_XInput.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define STICK_ON	(0.8f)
#define JOY_MAX_RANGE  (32768.0f)

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
CKeyboard *CHossoLibrary::m_pKeyboard = nullptr;
CPad_XInput *CHossoLibrary::m_pXInput = nullptr;

bool CHossoLibrary::m_bStickX = false;
bool CHossoLibrary::m_bStickY = false;
float CHossoLibrary::m_fLeftStickX = false;
float CHossoLibrary::m_fLeftStickY = false;
bool CHossoLibrary::m_WireFrame = false;
bool CHossoLibrary::m_Lighting = true;
int CHossoLibrary::m_Culling = false;

//------------------------------------------------------------------------------
//移動用のチェック関数
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckMove(DIRECTION direction)
{
	switch (direction)
	{
	case DIRECTION::UP:
		//↑
		if (m_pKeyboard->GetPress(DIK_W) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_UP))
		{
			return true;
		}
		break;
	case DIRECTION::DOWN:
		//↓
		if (m_pKeyboard->GetPress(DIK_S) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_DOWN))
		{
			return true;
		}
		break;
	case DIRECTION::LEFT:
		//←
		if (m_pKeyboard->GetPress(DIK_A) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_LEFT))
		{
			return true;
		}
		break;
	case DIRECTION::RIGHT:

		//→
		if (m_pKeyboard->GetPress(DIK_D) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_RIGHT))
		{
			return true;
		}
		break;
	}
	return false;
}

//------------------------------------------------------------------------------
//選択肢用チェック関数
//------------------------------------------------------------------------------
DIRECTION CHossoLibrary::CheckSelect()
{
	DIRECTION direction = DIRECTION::NONE;

	//↑
	if ((m_pKeyboard->GetTrigger(DIK_W) || m_pKeyboard->GetTrigger(DIK_UP) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_UP,1) || (m_fLeftStickY / JOY_MAX_RANGE > STICK_ON && m_bStickY == false)))
	{
		direction = DIRECTION::UP;
	}
	//↓
	else if ((m_pKeyboard->GetTrigger(DIK_S) || m_pKeyboard->GetTrigger(DIK_DOWN) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_DOWN, 1) || (m_fLeftStickY / JOY_MAX_RANGE < -STICK_ON && m_bStickY == false)))
	{
		direction = DIRECTION::DOWN;
	}
	//→
	else if ((m_pKeyboard->GetTrigger(DIK_D) || m_pKeyboard->GetTrigger(DIK_RIGHT) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_RIGHT, 1) || (m_fLeftStickX / JOY_MAX_RANGE > STICK_ON && m_bStickX == false)))
	{
		direction = DIRECTION::RIGHT;
	}
	//←
	else if ((m_pKeyboard->GetTrigger(DIK_A) || m_pKeyboard->GetTrigger(DIK_LEFT) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_LEFT, 1)  || (m_fLeftStickX / JOY_MAX_RANGE < -STICK_ON && m_bStickX == false)))
	{
		direction = DIRECTION::LEFT;
	}

	return direction;
}

//------------------------------------------------------------------------------
//決定 トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckDecision()
{
	if (m_pKeyboard->GetTrigger(DIK_RETURN) || m_pKeyboard->GetTrigger(DIK_SPACE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_A, 1))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
//キャンセル トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckCancel()
{
	if (m_pKeyboard->GetTrigger(DIK_BACKSPACE) || m_pKeyboard->GetTrigger(DIK_DELETE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_BACK, 1) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_B, 1))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_CANCEL);

		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//ポーズ トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckPause()
{
	if (m_pKeyboard->GetTrigger(DIK_P) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_START, 1))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE);
		return true;

	}
	return false;
}

//------------------------------------------------------------------------------
//何かボタン押して下さい用　トリガー
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckAnyButton()
{
	if (m_pKeyboard->GetTrigger(DIK_RETURN)						|| m_pKeyboard->GetTrigger(DIK_SPACE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_START, 1)	|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_BACK, 1) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_A, 1)		|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_B, 1) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_X, 1)		|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_Y, 1))
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//スティックの倒れてるかの情報をリセット
//------------------------------------------------------------------------------
void CHossoLibrary::ResetStickInfo()
{
	//1F前のスティック情報を元に倒れてるかチェック
	fabsf(m_fLeftStickX / JOY_MAX_RANGE) > STICK_ON ? m_bStickX = true : m_bStickX = false;
	fabsf(m_fLeftStickY / JOY_MAX_RANGE) > STICK_ON ? m_bStickY = true : m_bStickY = false;

	//今フレームのスティック情報取得
	m_pXInput->GetStickLeft(&m_fLeftStickX, &m_fLeftStickY);
}

void CHossoLibrary::StartVibration(int nCntVibration)
{
	m_pXInput->StartVibration(nCntVibration);
}

//------------------------------------------------------------------------------
//3D上での移動処理
//------------------------------------------------------------------------------
bool CHossoLibrary::Check3DMoveStick(D3DXVECTOR3 & Move, float const fMove, float const fCameraRot, float & fRotGoal)
{

	D3DXVECTOR3	Pad = ZeroVector3;

	//パッドの値取得
	m_pXInput->GetStickLeft(&Pad.x, &Pad.y);

	//joypadの値を出す
	Pad.x /= JOY_MAX_RANGE;
	Pad.y /= JOY_MAX_RANGE;

	float fAngle = atan2f((float)Pad.x, (float)Pad.y);				//角度
	float fLength = sqrtf(Pad.x * Pad.x + Pad.y * Pad.y);			//距離

	//一定以下の値の場合は判定しない
	if (fabsf(fLength) < 0.2f)
	{
		fLength = 0;
		return false;
	}

	//回転の計算
	fRotGoal = D3DX_PI + fAngle + fCameraRot;

	CHossoLibrary::CalcRotation(fRotGoal);

	//プレイヤーのmoveに加算
	Move.x += sinf(fAngle + fCameraRot) * fMove;
	Move.z += cosf(fAngle + fCameraRot) * fMove;


	return true;

}
//------------------------------------------------------------------------------
//Rスティックによるカメラ回転
//------------------------------------------------------------------------------
bool CHossoLibrary::Check3DCameraStick(D3DXVECTOR3 & Rot, float fHolizonMove, float fVerticalMove)
{
	D3DXVECTOR3 Pad = ZeroVector3;

	//パッドの値取得
	m_pXInput->GetStickRight(&Pad.x, &Pad.y);

	//joypadの値を出す
	Pad.x /= JOY_MAX_RANGE;
	Pad.y /= JOY_MAX_RANGE;

	float fAngle = atan2f((float)Pad.x, (float)Pad.y);				//角度
	float fLength = sqrtf(Pad.x * Pad.x + Pad.y * Pad.y);			//距離

	//一定以下の値の場合は判定しない
	if (fabsf(fLength) < 0.2f)
	{
		fLength = 0;
		return false;
	}

	//カメラの回転を加算
	Rot.y += sinf(fAngle) * (fHolizonMove * fLength);
	Rot.x += cosf(fAngle) * (fVerticalMove * fLength);

	return true;
}

//------------------------------------------------------------------------------
//マトリックス計算
//------------------------------------------------------------------------------
void CHossoLibrary::CalcMatrix(D3DXMATRIX *pMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot,D3DXVECTOR3 const &rScale)
{
	D3DXMATRIX	mtxRot, mtxTrans,mtxScale;			//計算用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtx);


	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rRot.y, rRot.x, rRot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);
	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, rScale.x, rScale.y, rScale.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxScale);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, rPos.x, rPos.y, rPos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}
//------------------------------------------------------------------------------
//マトリックス計算
//------------------------------------------------------------------------------
void CHossoLibrary::CalcShadowMatrix(D3DXMATRIX & rShadowMtx, D3DXVECTOR3 const & rPos, D3DXVECTOR3 const & rNor)
{
	D3DXPLANE planeField;
	D3DXVECTOR3 LightVec = CManager::GetRenderer()->GetLight()->GetLightDir(0);
	D3DXVECTOR4 ShadowLightVec = ZeroVector4;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&rShadowMtx);

	//影用のライトの設定
	ShadowLightVec = D3DXVECTOR4(-LightVec.x, -LightVec.y, -LightVec.z, 0);

	//影描画用のプレーン生成
	D3DXPlaneFromPointNormal(&planeField, &rPos, &rNor);

	//影のマトリックス設定
	D3DXMatrixShadow(&rShadowMtx, &ShadowLightVec, &planeField);
}

//------------------------------------------------------------------------------
//マトリックスから座標上表をreturnする
//------------------------------------------------------------------------------
D3DXVECTOR3 CHossoLibrary::CalcMtxToVector3(D3DXMATRIX const & rMtx)
{
	return D3DXVECTOR3(rMtx._41, rMtx._42, rMtx._43);
}

//------------------------------------------------------------------------------
//マウスでのレイの判定
//------------------------------------------------------------------------------
bool CHossoLibrary::MouseRayCollision_Boolean(D3DXMATRIX *pMtx, LPD3DXMESH pMesh)
{
	// 変数宣言
	D3DXVECTOR3*   pNearPos = &CManager::GetRay()->NearPos;	// レイの近い位置
	D3DXVECTOR3*   pFarPos = &CManager::GetRay()->FarPos;	// レイの遠い位置
	BOOL           bHit = FALSE;						// ヒットフラグ
	D3DXMATRIX     invmat;									// 算出した逆行列
	D3DXVECTOR3    ray;										// レイ
	D3DXVECTOR3    InvNirePos;								// 算出した近い位置
	D3DXVECTOR3    InvForePos;								// 算出した遠い位置

	/* 対処いう物からみたレイに変換する */
	//	逆行列の取得
	D3DXMatrixInverse(&invmat, NULL, pMtx);
	//	逆行列を使用し、レイ始点情報を変換　位置と向きで変換する関数が異なるので要注意
	D3DXVec3TransformCoord(&InvNirePos, pNearPos, &invmat);
	//	レイ終点情報を変換
	D3DXVec3TransformCoord(&InvForePos, pFarPos, &invmat);
	//	レイ方向情報を変換
	D3DXVec3Normalize(&ray, &(InvForePos - InvNirePos));
	//Rayを飛ばす
	D3DXIntersect(pMesh, &InvNirePos, &ray, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);

	//BOOL型とboolean型が違うので計算
	//FALSEだったらfalseがreturn,TRUEだったらtrueをreturn
	return (bHit == 1);
}

//------------------------------------------------------------------------------
//Imgui初期化
//------------------------------------------------------------------------------
 HRESULT CHossoLibrary::InitImgui(HWND hWnd)
{
	//ゲームパッドとキーボードの情報取得
	//所有権は貰ってないから開放の必要はない
	m_pXInput = CManager::GetXInput();
	m_pKeyboard = CManager::GetKeyboard();

	//NULLチェック
	if (!m_pXInput || !m_pKeyboard)
	{
		return E_FAIL;
	}

	//create
	IMGUI_CHECKVERSION();

	//生成
	ImGui::CreateContext();

	//デフォルトカラー設定
	ImGui::StyleColorsDark();

	//初期化
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CManager::GetRenderer()->GetDevice());

	return S_OK;
}
//------------------------------------------------------------------------------
//Imgui終了
//------------------------------------------------------------------------------
void CHossoLibrary::UninitImgui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
//------------------------------------------------------------------------------
//Imgui デバッグ情報表示
//------------------------------------------------------------------------------
void CHossoLibrary::ShowDebugInfo()
{
#ifdef _DEBUG

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//FPS情報
	if (ImGui::TreeNode("FPS"))
	{
		//FPS
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::TreePop();
	}
	//Sceneの数
	if (ImGui::TreeNode("NumInfo"))
	{
		ImGui::Text("NumAll (%d)", CScene::GetNumAll());
		ImGui::Text("NumScene2D (%d)", CScene2D::GetNumScene2D());
		ImGui::Text("NumScene3D (%d)", CScene3D::GetNumScene3D());
		ImGui::Text("ParticleAll (%d)", COneParticle::GetNumAll());
		ImGui::TreePop();
	}

	//情報
	if (ImGui::TreeNode("DebugCommand"))
	{
		//ワイヤーフレーム
		if (ImGui::Checkbox("WireFrame", &m_WireFrame))
		{
			CheckWireMode();
		}

		//ライティング
		if (ImGui::Checkbox("Lighting", &m_Lighting))
		{
			pDevice->SetRenderState(D3DRS_LIGHTING, m_Lighting);		// ライティングモード切り替え
		}

		//カリング
		if (ImGui::TreeNode("tree1", "CULLING"))
		{
			if (ImGui::RadioButton("D3DCULL_CCW", &m_Culling, 0))
			{
				//裏面カリング
				CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CCW);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("D3DCULL_CW", &m_Culling, 1))
			{
				//表面カリング
				CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CW);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("D3DCULL_NONE", &m_Culling, 2))
			{
				//カリングしない
				CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_NONE);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

#endif //DEBUG
}
//------------------------------------------------------------------------------
//ワイヤーフレームかどうか確認
//Imguiのウィンドウだけワイヤーフレームにならない為に必要
//------------------------------------------------------------------------------
void CHossoLibrary::CheckWireMode()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//trueかfalseかで決める
	m_WireFrame ?
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_WIRE_ON):
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_WIRE_OFF);
}
//------------------------------------------------------------------------------
//カリングの状態確認と設定
//一時的に処理を変えた後何に戻すべきか確認用
//------------------------------------------------------------------------------
void CHossoLibrary::CheckCulling()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	switch (m_Culling)
	{
	case 0:
		//裏面カリング
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CCW);
		break;
	case 1:
		//表面カリング
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CW);
		break;
	case 2:
		//カリングしない
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_NONE);
		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------
//カリングの状態確認と設定
//一時的にライティングを切った後に戻すべきか確認用
//------------------------------------------------------------------------------
void CHossoLibrary::CheckLighting()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, m_Lighting);		// ライティングモード切り替え
}

//------------------------------------------------------------------------------
//回転の計算　360度以内にする
//------------------------------------------------------------------------------
void CHossoLibrary::CalcRotation(float &fRot)
{
	//差分が3.14を以上の時
	if (fRot > D3DX_PI)
	{
		//6.28引く
		fRot -= D3DX_PI * 2;
	}
	//差分が-3.14以下の時
	if (fRot < -D3DX_PI)
	{
		//6.28加算
		fRot += D3DX_PI * 2;
	}
}


//------------------------------------------------------------------------------
//XYZ軸全部3.14以内に収める
//------------------------------------------------------------------------------
void CHossoLibrary::CalcRotation_XYZ(D3DXVECTOR3 & rot)
{
	CHossoLibrary::CalcRotation(rot.x);
	CHossoLibrary::CalcRotation(rot.y);
	CHossoLibrary::CalcRotation(rot.z);

}
//------------------------------------------------------------------------------
//複数個のオブジェクトを並べる時に均等になるように座標計算
//------------------------------------------------------------------------------
float CHossoLibrary::CalcEvenPosition(int nMaxNum, int nNowNum, float fInterVal)
{
	//偶数か奇数で計算が変わる
	float fValue = nMaxNum % 2 ?
		(-fInterVal * (nMaxNum / 2) + (fInterVal * nNowNum)) :								//偶数
		(-fInterVal * (nMaxNum / 2) + (fInterVal * 0.5f) + (fInterVal * nNowNum));			//奇数

	return fValue;
}

//------------------------------------------------------------------------------
//選択
//------------------------------------------------------------------------------
bool CHossoLibrary::Selecting(int &nSelectNum, int const &nSelectNumOld, int const nNumX, int const nNumY)
{

	//←→↑↓どれか入力されていた場合
	if (m_pKeyboard->GetTrigger(DIK_LEFT) || m_pKeyboard->GetTrigger(DIK_RIGHT) ||
		m_pKeyboard->GetTrigger(DIK_UP) || m_pKeyboard->GetTrigger(DIK_DOWN))
	{
		//←
		if (m_pKeyboard->GetTrigger(DIK_LEFT))
		{
			//選択番号--
			nSelectNum--;

			if (nSelectNumOld / nNumX != nSelectNum / nNumX)
			{
				nSelectNum = nSelectNumOld;
			}
		}
		//→
		else if (m_pKeyboard->GetTrigger(DIK_RIGHT))
		{
			//選択番号++
			nSelectNum++;
			if (nSelectNumOld / nNumX != nSelectNum / nNumX)
			{
				nSelectNum = nSelectNumOld;
			}
		}
		//↑
		else if (m_pKeyboard->GetTrigger(DIK_UP))
		{
			nSelectNum -= nNumX;
		}
		//↓
		else if (m_pKeyboard->GetTrigger(DIK_DOWN))
		{
			nSelectNum += nNumX;
		}

		//範囲内に抑える
		if (CHossoLibrary::RangeLimit_Equal(nSelectNum, 0, (nNumX * nNumY) - 1))
		{
			nSelectNum = nSelectNumOld;

			//これ以上動けないのでreturn
			return false;
		}

		//trueを返す
		return true;
	}

	//入力が無かったのでreturn
	return false;
}
//------------------------------------------------------------------------------
//モデルの最大頂点と最少頂点を求める
//------------------------------------------------------------------------------
void CHossoLibrary::SetModelVertex(MODEL_VTX & pModelVtx, CModelInfo & pModelInfo)
{
	int		nNumVertices;
	DWORD	sizeFVF;
	BYTE	*pVertexBuffer;

	pModelVtx.VtxMax = ZeroVector3;
	pModelVtx.VtxMin = ZeroVector3;

	//頂点数取得
	nNumVertices = pModelInfo.GetMesh()->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(pModelInfo.GetMesh()->GetFVF());

	// 頂点バッファのロック
	pModelInfo.GetMesh()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

	//頂点の数だけ繰り返す
	for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
	{
		//頂点情報を取得
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		//最大の頂点座標と最少の頂点座標を比較して出す
		if (pModelVtx.VtxMin.x > vtx.x)
		{
			pModelVtx.VtxMin.x = vtx.x;
		}
		if (pModelVtx.VtxMax.x < vtx.x)
		{
			pModelVtx.VtxMax.x = vtx.x;
		}
		if (pModelVtx.VtxMin.z > vtx.z)
		{
			pModelVtx.VtxMin.z = vtx.z;
		}
		if (pModelVtx.VtxMax.z < vtx.z)
		{
			pModelVtx.VtxMax.z = vtx.z;
		}
		if (pModelVtx.VtxMin.y > vtx.y)
		{
			pModelVtx.VtxMin.y = vtx.y;
		}
		if (pModelVtx.VtxMax.y < vtx.y)
		{
			pModelVtx.VtxMax.y = vtx.y;
		}
		//バッファ分進める
		pVertexBuffer += sizeFVF;
	}
	//アンロック
	pModelInfo.GetMesh()->UnlockVertexBuffer();

}

//------------------------------------------------------------------------------
//ビルボード設定 XとZのみ
//------------------------------------------------------------------------------
void CHossoLibrary::SetBillboard_XZ_Only(D3DXMATRIX * pMtx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用変数
	D3DXMATRIX mtxView;

	//ビューマトリックス初期化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ビルボード設定
	pMtx->_11 = mtxView._11;
	pMtx->_12 = mtxView._21;
	pMtx->_13 = mtxView._31;
	//pMtx->_21 = mtxView._12;
	//pMtx->_22 = mtxView._22;
	//pMtx->_23 = mtxView._32;
	pMtx->_31 = mtxView._13;
	pMtx->_32 = mtxView._23;
	pMtx->_33 = mtxView._33;
}
//------------------------------------------------------------------------------
//ビルボード設定
//------------------------------------------------------------------------------
void CHossoLibrary::SetBillboard(D3DXMATRIX * pMtx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用変数
	D3DXMATRIX mtxView;

	//ビューマトリックス初期化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ビルボード設定
	pMtx->_11 = mtxView._11;
	pMtx->_12 = mtxView._21;
	pMtx->_13 = mtxView._31;
	pMtx->_21 = mtxView._12;
	pMtx->_22 = mtxView._22;
	pMtx->_23 = mtxView._32;
	pMtx->_31 = mtxView._13;
	pMtx->_32 = mtxView._23;
	pMtx->_33 = mtxView._33;
}

//------------------------------------------------------------------------------
//縦のメニュー選択
//------------------------------------------------------------------------------
void CHossoLibrary::SelectVerticalMenu(int & nSelectNum, int const & nMaxSelect)
{
	//↑
	if (CHossoLibrary::CheckSelect() == DIRECTION::UP)
	{
		nSelectNum--;
		//一番↑にいったら
		if (nSelectNum < 0)
		{
			//一番↑に合わせる
			nSelectNum = 0;
		}
		else
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
	//↓
	if (CHossoLibrary::CheckSelect() == DIRECTION::DOWN)
	{
		nSelectNum++;
		//一番↓にいったら
		if (nSelectNum > nMaxSelect - 1)
		{
			//一番↓に合わせる
			nSelectNum = nMaxSelect - 1;
		}
		else
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
}
//------------------------------------------------------------------------------
//横のメニュー選択
//------------------------------------------------------------------------------
void CHossoLibrary::SelectHorizonMenu(int & nSelectNum, int const & nMaxSelect)
{
	//←
	if (CHossoLibrary::CheckSelect() == DIRECTION::LEFT)
	{
		nSelectNum--;
		if (nSelectNum < 0)
		{
			nSelectNum = 0;
		}
		else
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
	//→
	if (CHossoLibrary::CheckSelect() == DIRECTION::RIGHT)
	{
		nSelectNum++;
		if (nSelectNum > nMaxSelect - 1)
		{
			nSelectNum = nMaxSelect - 1;
		}
		else
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
}

//------------------------------------------------------------------------------
//ImGuiのコンボボックス
//------------------------------------------------------------------------------
bool CHossoLibrary::ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int & nValue)
{
	bool bChange = false;

#ifdef _DEBUG
	//combo開始
	if (ImGui::BeginCombo(aTitle.data(), aItemNameList[nValue].data()))
	{
		//要素分繰り返す
		for (size_t nCnt = 0; nCnt < aItemNameList.size(); nCnt++)
		{

			//選択番号があってるかどうか
			bool is_selected = (aItemNameList[nValue] == aItemNameList[nCnt]);

			//選択された時の処理
			if (ImGui::Selectable(aItemNameList[nCnt].data(), is_selected))
			{
				//現在の選択項目設定
				nValue = nCnt;
				bChange = true;
			}
		}
		//combo終了
		ImGui::EndCombo();

	}
#endif //DEBUG
	return bChange;
}
//------------------------------------------------------------------------------
//範囲内の値からランダムに値を返す 符号なし
//------------------------------------------------------------------------------
int CHossoLibrary::RandomRangeUnsigned(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}
//------------------------------------------------------------------------------
//-3.14～3.14を返す
//------------------------------------------------------------------------------
float CHossoLibrary::Random_PI()
{
	return float(rand() % 314 / 100.0f) - float(rand() % 314) / 100.0f;
}

//------------------------------------------------------------------------------
//入力された値の+-ランダムな値で返す
//------------------------------------------------------------------------------
float CHossoLibrary::Random(float fInputValue)
{
	//0除算防止
	if (fInputValue <= 0.0f)
	{
		return 0.0f;
	}

	int nValue = int(fInputValue * 100);

	float fValue;

	rand() % 2 ?
		fValue = float(rand() % nValue / 100.0f) :
		fValue = -float(rand() % nValue / 100.0f);

	return fValue;
}

//------------------------------------------------------------------------------
//入力された値の+-ランダムな値で返す
//------------------------------------------------------------------------------
D3DXVECTOR3 CHossoLibrary::RandomVector3(float Max)
{
	D3DXVECTOR3 Value = ZeroVector3;

	Value.x = Random(Max);
	Value.y = Random(Max);
	Value.z = Random(Max);


	return Value;
}

//------------------------------------------------------------------------------
//UV計算
//------------------------------------------------------------------------------
D3DXVECTOR2 CHossoLibrary::CalcUV_StaticFunc(int nNumUV, int tex)
{
	//UV座標計算
	D3DXVECTOR2 UV;
	UV.x = nNumUV % (int)CTexture::GetSparateTex_UVCnt((CTexture::SEPARATE_TEX_TYPE)tex).x * CTexture::GetSparateTex_UVSize((CTexture::SEPARATE_TEX_TYPE)tex).x;
	UV.y = nNumUV / (int)CTexture::GetSparateTex_UVCnt((CTexture::SEPARATE_TEX_TYPE)tex).x * CTexture::GetSparateTex_UVSize((CTexture::SEPARATE_TEX_TYPE)tex).y;

	return UV;
}


