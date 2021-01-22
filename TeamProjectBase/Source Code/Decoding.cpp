//------------------------------------------------------------------------------
//
//��Ǐ���  [Decoding.h]
//Author:Ikuto Sekine
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "Decoding.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "Map.h"
#include "ParticleManager.h"
#include "picture.h"
#include "scene2D.h"
#include "chatBase.h"
#include "light.h"
#include "client.h"
#include "tablet.h"
#include "TabletButton.h"
#include "DecodingManager.h"
#include "DecodingWindow.h"
#include "keyboard.h"
#include "game.h"

//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------
#define GAMEEND_UI_SIZE	(D3DXVECTOR3(800.0f, 250.0f, 0.0f))

//------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
CDecoding::GAMEENDSTATE CDecoding::m_endState = CDecoding::GAMEEND_NONE;
int CDecoding::m_nCntState = 0;

//------------------------------------------------------------------------------
// �R���X�g���N�^
//------------------------------------------------------------------------------
CDecoding::CDecoding()
{
}

//------------------------------------------------------------------------------
// �f�X�g���N�^
//------------------------------------------------------------------------------
CDecoding::~CDecoding()
{
	// �^�u���b�g�{�^���̊J��
	CTabletButton::Unload();

	// �s�N�`���̐ÓI�����o�̏I��
	CPicture::UninitStaticMember();

	// ��ǃE�B���h�E�̓ǂݍ���
	CDecodingWindow::UnLoad();
	CDecodingManager::Unload();
}

//------------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------------
HRESULT CDecoding::Init(HWND hWnd)
{
	//�J�������W�ݒ�
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);
	CManager::GetRenderer()->GetCamera()->SetDecodingCamera();
	//������
	SetState(STATE::STATE_NORMAL);
	//// �^�u���b�g�̓ǂݍ���
	//CTablet::Load();
	//// �^�u���b�g�{�^���̓ǂݍ���
	//CTabletButton::Load();
	//// �^�u���b�g�̐���
	//m_pTablet = CTablet::Create();
	//// �s�N�`���̐ÓI�����o�̏�����
	//CPicture::InitStaticMember();
	//m_pPicture = CPicture::Create(m_pTablet->GetMtxWorldPtr());
	// ��ǃ}�l�[�W���[�̓ǂݍ���
	CDecodingManager::Load();
	// ��ǃE�B���h�E�̓ǂݍ���
	CDecodingWindow::Load();
	// ��ǃ}�l�[�W���[�̐���
	m_pDecodingManager = CDecodingManager::Create();
	// �I����Ԃ̏�����
	m_endState = CDecoding::GAMEEND_NONE;

	//�}�b�v����
	CMap::Create();

	//�`���b�g�@�\
	CChatBase::Create();
	return S_OK;
}

//------------------------------------------------------------------------------
// �X�V
//------------------------------------------------------------------------------
void CDecoding::Update()
{
	// �Q�[�����I������Ƃ�
	if (m_endState != GAMEEND_NONE)
	{
		UpdateEndState();
	}

#ifdef _DEBUG
	//�Ȃ񂩃{�^�������ꂽ�Ƃ�
	if (CManager::GetKeyboard()->GetPress(DIK_RSHIFT) &&
		CManager::GetKeyboard()->GetPress(DIK_RETURN))
	{
		//�`���[�g���A���ɑJ��
		CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
	}
#endif
}

//------------------------------------------------------------------------------
// �`��
//------------------------------------------------------------------------------
void CDecoding::Draw()
{
}

//------------------------------------------------------------------------------
// �f�o�b�O���̕\��
//------------------------------------------------------------------------------
void CDecoding::ShowDebugInfo()
{
}

//------------------------------------------------------------------------------
// ��Ԃ̍X�V
//------------------------------------------------------------------------------
void CDecoding::UpdateState()
{
	m_nCntState--;

	switch (m_State)
	{
	case CDecoding::STATE_READY:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NORMAL);
		}
		break;
	case CDecoding::STATE_NORMAL:
		break;
	case CDecoding::STATE_PAUSE:
		break;
	case CDecoding::STATE_GAMEOVER:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
		}
		break;
	case CDecoding::STATE_GAMECLEAR:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT);
		}
		break;
	}
}

//------------------------------------------------------------------------------
//�@��Ԃ̐ݒ�
//------------------------------------------------------------------------------
void CDecoding::SetState(STATE state)
{
	//�X�e�[�g���i��ł���ꍇ
	if (m_State != CDecoding::STATE_NONE && m_State != state)
	{
		//�X�e�[�g�ύX
		m_State = state;

		switch (m_State)
		{
		case CDecoding::STATE_NONE:
			break;
		case CDecoding::STATE_READY:
			m_nCntState = 120;
			CManager::GetRenderer()->GetLight()->SetBlackLight();

			break;
		case CDecoding::STATE_NORMAL:
			CManager::GetRenderer()->GetLight()->SetDefaultLight();

			break;
		case CDecoding::STATE_PAUSE:
			break;
		case CDecoding::STATE_GAMEOVER:

			break;
		case CDecoding::STATE_GAMECLEAR:
			m_nCntState = 120;

			break;
		case CDecoding::STATE_DEBUG:
			break;
		case CDecoding::STATE_MAX:
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// �����̐ݒ�
//------------------------------------------------------------------------------
void CDecoding::SetGaze(GAZE gaze)
{
	//�O�̏�ԂƈႤ��
	if (m_Gaze != gaze)
	{
		//�������ݒ�
		m_Gaze = gaze;
		switch (m_Gaze)
		{
		case CDecoding::GAZE_DEFAULT:
			break;
		default:
			break;
		}


	}
}

//------------------------------------------------------------------------------
// �I�����̍X�V
//------------------------------------------------------------------------------
void CDecoding::UpdateEndState(void)
{
	// �ŏ�
	if (m_nCntState >= 120)
	{
		// UI�𐶐����A�N���A�t���O��ݒ�
		if (m_endState == GAMEEND_CLEAR)
		{
			CGame::SetClearFlag(true);
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, GAMEEND_UI_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_GAMECLEAR), CScene::OBJTYPE_FRONT);
		}
		else if (m_endState == GAMEEND_MISS)
		{
			CGame::SetClearFlag(false);
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, GAMEEND_UI_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_GAMEOVER), CScene::OBJTYPE_FRONT);
		}
	}

	m_nCntState--;

	// �J�E���g�o��
	if (m_nCntState <= 0)
	{
		SetState(CDecoding::STATE_NONE);
		// UI�𐶐�
		m_endState == GAMEEND_CLEAR ?
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT) :
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
	}
}
