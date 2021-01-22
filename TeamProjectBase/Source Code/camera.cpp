//-----------------------------------------------------------------------------
//
// �J�������� [Camera.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "chatTab.h"
#include "Map.h"
#include "mouse.h"
//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define CAMERA_MOVE_SPEED			(10.0f)			//�J�����ړ����x
#define CAMERA_HORIZON_ROTATION		(0.05f)			//�J�����̉���]
#define CAMERA_VERTICAL_ROTATION	(0.02f)			//�J�����̏c��]
#define DEFAULT_POS_R				(D3DXVECTOR3(0.0f,200.0f,0.0f))

#define SLIDE_POS_VALUE				(D3DXVECTOR3(120.0f,0.0f,0.0f))

#define DEFAULT_DISTANCE			(500.0f)		//�J�����̋���
#define APPROACH_DISTANCE			(300.0f)		//�J�����̋���

#define DEFAULT_CAMERA_ROTATION		(D3DXVECTOR3(0.13f,0.0f,0.0f))
#define TITLE_CAMERA_ROTATION		(D3DXVECTOR3(0.5f,0.0f,0.0f))

#define CAMERA_LENGTH_NEAR			(10.0f)			//�J�����̌����鋗���i�߁j
#define CAMERA_LENGTH_FAR			(5000.0f)		//�J�����̌����鋗���i���j

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//�R���X�g���N�^
//-----------------------------------------------------------------------------
CCamera::CCamera()
{
	//������
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
	m_CameraState = CCamera::CAMERA_GAME;
	m_MousePosCurrent = ZeroVector2;
	m_MousePosOld = ZeroVector2;
	m_MouseRotSave = ZeroVector2;

	m_bApproach = false;

	ResetCamera();

}
//-----------------------------------------------------------------------------
//�f�X�g���N�^
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{

}
//-----------------------------------------------------------------------------
//������
//-----------------------------------------------------------------------------
void CCamera::Init(void)
{
	//�}�g���b�N�X�v�Z
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot, OneVector3);

	//�����̋���������
	m_fDefaultDistace = 0.0f;

}

//-----------------------------------------------------------------------------
//�X�V
//-----------------------------------------------------------------------------
void CCamera::Update(void)
{
	//1F�O�̍��W�ۑ�
	m_posVOld = m_posV;


	switch (m_CameraState)
	{
	case CCamera::CAMERA_TITLE:
		//�^�C�g���̃J����
		TitleCamera();

		//�J������]�v�Z
		CalcRotationCamera();

		break;
	case CCamera::CAMERA_GAME:
		//�J������]
		CalcRotationCamera();
		break;
	case CCamera::CAMERA_DEBUG:
		////�}�E�X�ɂ��J��������
		//Operation();

		//�J�����ړ�
		//MoveCameraDebug();

		//�J������]
		CalcRotationCamera();
		break;
	}

	//�}�g���b�N�X�v�Z
	CHossoLibrary::CalcMatrix(&m_mtxWorld, m_posV, m_rot, OneVector3);

}
//-----------------------------------------------------------------------------
//�ݒ菈��
//-----------------------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),	//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//�A�X�y�N�g��
		CAMERA_LENGTH_NEAR,								//NearZ�l
		CAMERA_LENGTH_FAR);								//FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//-----------------------------------------------------------------------------
//�J�������
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

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------�J����----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[W] [A] [S] [D] [Q] [E] �J�����ړ� \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[���N���b�N] �J������] \n");

#endif //DEBUG
}

//-----------------------------------------------------------------------------
//�X�e�[�g�ݒ�
//-----------------------------------------------------------------------------
void CCamera::SetState(CAMERA_STATE state)
{
	//�X�e�[�g�ݒ�
	m_CameraState = state;

	switch (state)
	{
	case CAMERA_STATE::CAMERA_TITLE:
		m_rot = m_rotDest = TITLE_CAMERA_ROTATION;

		m_posV.x = m_posVDest.x = m_posRDest.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_posV.y = m_posVDest.y = m_posRDest.y + sinf(m_rot.x)	* m_fDistance;
		m_posV.z = m_posVDest.z = m_posRDest.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		break;

	case CAMERA_STATE::CAMERA_GAME:
		ResetCamera();
		break;

	case CAMERA_STATE::CAMERA_DEBUG:

		break;


	default:
		break;
	}


}


//-----------------------------------------------------------------------------
//�ړ�����
//-----------------------------------------------------------------------------
void CCamera::MoveCameraDebug(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//[W]�L�[����������
	if (pKeyboard->GetPress(DIK_W))
	{
		//��
		m_posRDest.x += sinf(D3DX_PI * 0.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 0.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[S]�L�[����������
	if (pKeyboard->GetPress(DIK_S))
	{
		//�O
		m_posRDest.x += sinf(D3DX_PI * 1.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 1.0f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[A]�L�[����������
	if (pKeyboard->GetPress(DIK_A))
	{
		//��
		m_posRDest.x += sinf(-D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(-D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}
	//[D]�L�[����������
	if (pKeyboard->GetPress(DIK_D))
	{
		//�E
		m_posRDest.x += sinf(D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
		m_posRDest.z += cosf(D3DX_PI * 0.5f + m_rotDest.y) * CAMERA_MOVE_SPEED;
	}

	//[1]�L�[����������
	else if (pKeyboard->GetPress(DIK_Q))
	{
		//��Ɉړ�
		m_posRDest.x += sinf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
		m_posRDest.y += cosf(D3DX_PI * 0.0f) * CAMERA_MOVE_SPEED;
	}
	//[3]�L�[����������
	else if (pKeyboard->GetPress(DIK_E))
	{
		//���Ɉړ�
		m_posRDest.x += sinf(D3DX_PI * 1.0f) * CAMERA_MOVE_SPEED;
		m_posRDest.y += cosf(D3DX_PI * 1.0f) * CAMERA_MOVE_SPEED;
	}
}
//-----------------------------------------------------------------------------
//�^�C�g���̃J����
//-----------------------------------------------------------------------------
void CCamera::TitleCamera()
{
	//��]
	m_rotDest.y += 0.007f;
}

//-----------------------------------------------------------------------------
//��]����
//-----------------------------------------------------------------------------
void CCamera::CalcRotationCamera()
{
	//3.14�͈̔͂ɗ}����
	CHossoLibrary::CalcRotation(m_rotDest.y);

	//��]��
	m_rot = m_rotDest;

	/*---------------------------------------------
	//	���_�̍X�V
	---------------------------------------------*/
	// ���_�̍s����̍X�V
	m_posVDest.x = m_posR.x - cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posR.z - cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// ���_�̍X�V
	m_posV += (m_posVDest - m_posV) * 0.15f;

	/*---------------------------------------------
	//	�����_�̍X�V
	---------------------------------------------*/
	// �����_�̍s����̍X�V

	//���ɂ������W
	D3DXVECTOR3 LocalPos = m_posRDest;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "TabState >> %d\n", CChatTab::GetTabState());
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "TabletState >> %d\n", CChatTab::GetTabletState());


	//if (CManager::GetMode() == CManager::MODE_GAME)
	//{
	//	//�`���b�g���^�u���b�g���J���Ă��鎞
	//	if (CChatTab::GetTabletState() == CChatTab::TABSTATE_OPENED ||
	//		CChatTab::GetTabState() == CChatTab::TABSTATE_OPENED ||
	//		CChatTab::GetTabState() == CChatTab::TABSTATE_OPENNING)

	//	{
	//		//�X���C�h���W
	//		LocalPos += SLIDE_POS_VALUE;
	//	}
	//}

	// �����_�̍X�V
	m_posR += (LocalPos - m_posR) * 0.15f;
}

//-----------------------------------------------------------------------------
//�J�������������W�ɐݒ�
//-----------------------------------------------------------------------------
void CCamera::ResetCamera()
{
	//�������W��
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
// �J�������߂Â��鏈��
//-------------------------------------------------------------------------------------------------------------
void CCamera::ApproachCamera(D3DXVECTOR3 posRDest)
{
	m_posRDest = posRDest;
	m_rot = DEFAULT_CAMERA_ROTATION;
	m_fDistance = APPROACH_DISTANCE;

	m_bApproach = true;
}

//-------------------------------------------------------------------------------------------------------------
// �J�������痣���
//-------------------------------------------------------------------------------------------------------------
void CCamera::LeaveCamera()
{
	m_fDistance = DEFAULT_DISTANCE;
	m_posRDest = DEFAULT_POS_R;
	m_bApproach = false;

}

//-------------------------------------------------------------------------------------------------------------
// �J�����ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCamera::SetDecodingCamera()
{
	m_fDistance = DEFAULT_DISTANCE + 400.0f;
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
void CCamera::Operation(void)
{
	// �L�[�{�[�h�̎擾
	//Ckeyboard *pKeyboard = &CManager::GetKeyboard();

	// �}�E�X�̎擾
	CMouse *pMouse = CManager::GetMouse();
	// �}�E�X�̏�Ԃ��擾
	DIMOUSESTATE2* pMouseState = &pMouse->GetMouseState();

	D3DXVECTOR2 NewRotation = D3DXVECTOR2(0.0f, 0.0f);

	if (pMouse->GetPress(1) == true)
	{
		// ���[��]
		NewRotation.y = ((float)pMouseState->lX) / (D3DX_PI*2.0f) *0.02f;
		m_rotDest.y += NewRotation.y;

		// ��]�ʂ�360�x�Ȃ��ɒ���
		CMylibrary::SetFixTheRotation(&m_rotDest.y);

		// �s�b�`���[��]
		NewRotation.x = ((float)pMouseState->lY) / (D3DX_PI*2.0f) *0.02f;

		// ��]��90�x�����ɗ}����
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

	// �J�����̉�]��90�x�����ɗ}����
	if (m_rot.x >= D3DX_PI*0.49f)
	{
		m_rot.x = D3DX_PI*0.49f;
	}
	else if (m_rot.x <= -D3DX_PI*0.49f)
	{
		m_rot.x = -D3DX_PI*0.49f;
	}
	// ��]�ʂ�360�x�Ȃ��ɒ���
	CMylibrary::SetFixTheRotation(&m_rot.y);
}
