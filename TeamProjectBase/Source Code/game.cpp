//------------------------------------------------------------------------------
//
//�Q�[������  [game.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "Bomb.h"
#include "Map.h"
#include "ParticleManager.h"
#include "picture.h"
#include "scene2D.h"
#include "chatBase.h"
#include "light.h"
#include "client.h"
#include "tablet.h"
#include "TabletButton.h"
#include "client.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
bool CGame::m_bClear = false;
CBomb::DIFFICULTY CGame::m_bDifficulty = CBomb::HARD;

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CGame::CGame()
{
	m_nCntState = 0;
	m_State = STATE::STATE_NORMAL;
	m_Gaze = GAZE_BOMB;
	m_pBomb.reset();

	m_bClear = false;

}
//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CGame::~CGame()
{
	m_pBomb.reset();

	// �^�u���b�g�{�^���̊J��
	CTabletButton::Unload();

	// �s�N�`���̐ÓI�����o�̏I��
	CPicture::UninitStaticMember();
}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CGame::Init(HWND hWnd)
{
	//�J�������W�ݒ�
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//������
	SetState(STATE::STATE_READY);

#ifdef _DEBUG

	//������
	SetState(STATE::STATE_NORMAL);

#endif

	//�p�[�e�B�N���̃}�l�[�W��
	CParticleManager::Create();

	//���e����
	m_pBomb	= CBomb::CreateBomb(D3DXVECTOR3(0.0f,200.0f,0.0f), ZeroVector3, m_bDifficulty);

	//���W���[������
	m_pBomb->CreateModule();

	//�}�b�v����
	CMap::Create();

	//// �^�u���b�g�̓ǂݍ���
	//CTablet::Load();
	//// �^�u���b�g�{�^���̓ǂݍ���
	//CTabletButton::Load();
	//// �^�u���b�g�̐���
	//m_pTablet = CTablet::Create();
	// �s�N�`���̐ÓI�����o�̏�����
	//CPicture::InitStaticMember();
	//m_pPicture = CPicture::Create(m_pTablet->GetMtxWorldPtr());

	// �`���b�g�̐���
	m_pChatBase = CChatBase::Create();

	return S_OK;
}


//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CGame::Update()
{
	UpdateState();
}

//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CGame::Draw()
{

}
//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CGame::ShowDebugInfo()
{
#ifdef _DEBUG


#endif //_DEBUG
}

//------------------------------------------------------------------------------
//�X�e�[�g�̍X�V
//------------------------------------------------------------------------------
void CGame::UpdateState()
{
	m_nCntState--;

	switch (m_State)
	{
	case CGame::STATE_READY:
		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NORMAL);
		}
		break;
	case CGame::STATE_NORMAL:
		break;
	case CGame::STATE_PAUSE:
		break;
	case CGame::STATE_GAMEOVER:
		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
		}
		break;
	case CGame::STATE_GAMECLEAR:
		m_bClear = true;

		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT);
		}
		break;
	}
}

//------------------------------------------------------------------------------
//�Q�[���X�e�[�g�Z�b�g
//------------------------------------------------------------------------------
void CGame::SetState(STATE state)
{
	//�X�e�[�g���i��ł���ꍇ
	if (m_State != CGame::STATE_NONE && m_State != state)
	{
		//�X�e�[�g�ύX
		m_State = state;

		switch (m_State)
		{
		case CGame::STATE_NONE:
			break;
		case CGame::STATE_READY:
			m_nCntState = 120;
			CManager::GetRenderer()->GetLight()->SetBlackLight();

			break;
		case CGame::STATE_NORMAL:
			CManager::GetRenderer()->GetLight()->SetDefaultLight();

			break;
		case CGame::STATE_PAUSE:
			break;
		case CGame::STATE_GAMEOVER:
			m_nCntState = 120;

			//��ʑS�̂ɐ^�����|���S��
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, SCREEN_SIZE, BlackColor, nullptr, CScene::OBJTYPE_FRONT);

			//���Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION_00);

			// �Q�[���I�[�o�[���M
			CClient::SendGameOver();

			break;
		case CGame::STATE_GAMECLEAR:
			m_nCntState = 120;
			// �Q�[���N���A���M
			CClient::SendGameClear();

			break;
		case CGame::STATE_DEBUG:
			break;
		case CGame::STATE_MAX:
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
//�����ݒ�
//------------------------------------------------------------------------------
void CGame::SetGaze(GAZE gaze)
{
	//�O�̏�ԂƈႤ��
	if (m_Gaze != gaze)
	{
		//�������ݒ�
		m_Gaze = gaze;
		switch (m_Gaze)
		{
		case CGame::GAZE_DEFAULT:

			break;
		case CGame::GAZE_BOMB:
			CManager::GetRenderer()->GetCamera()->LeaveCamera();
			break;
		case CGame::GAZE_MODULE:

			break;
		default:
			break;
		}


	}

}
