//------------------------------------------------------------------------------
//
//���e�̏���  [Bomb.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "Bomb.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"
#include "modelinfo.h"
#include "Bomb_Exterior.h"
#include "modelinfo.h"
#include "module_none.h"
#include "module_timer.h"
#include "module_No0_SymbolKeyPad.h"
#include "module_No1_ShapeKeypad.h"
#include "module_No2_LampAndWire.h"
#include "module_No3_PassWord.h"
#include "module_No4_4ColButton.h"
#include "sound.h"
#include "timer.h"
#include "chatTab.h"
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
#ifdef _DEBUG
bool CBomb::m_bCanExplosion = false;
#endif // _DEBUG


//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define MODULE_NUM_EASY		(2)
#define MODULE_NUM_NORMAL	(3)
#define MODULE_NUM_HARD		(5)
#define MODULE_NUM_TITLE	(11)


//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CBomb::CBomb()
{
	m_nModuleNum = 0;
	m_nSelectModuleNum = 0;
	m_pModuleList = {};
	m_pBombExterior.reset();
	m_bCameraDir = true;
	m_RotDest = GetRot();
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CBomb::~CBomb()
{
	m_pModuleList = {};
	m_pBombExterior.reset();
}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CBomb::Init()
{
	CSceneX::Init();
	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CBomb::Update()
{
	CSceneX::Update();

	if (CManager::GetMode() != CManager::MODE::MODE_GAME)
	{
		return;
	}


	fabsf(GetRot().y) >= D3DX_PI * 0.5f ?
		m_bCameraDir = true :
		m_bCameraDir = false;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "bCamera >> %d\n", m_bCameraDir);

	int nMin = 6 * (m_bCameraDir);
	int nMax = nMin + 5;

	//���\�؂�ւ������
	if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
	{
		SearchHeadCanSelectNum(nMin);
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}


	//�L�[�{�[�h����
	//CBomb::Operation_Keyboard();

	//�}�E�X����
	CBomb::Operation_Mouse();

	//////std::cout << typeid(m_pModuleList[0].get());
	//std::cout << "m_pModuleList[0]" << "Name >> " << typeid(m_pModuleList[0]).name() << NEWLINE;
	//std::cout << "m_pModuleList[0]" << "RawName >> " << typeid(m_pModuleList[0]).raw_name() << NEWLINE;

	//std::cout << "m_pModuleList[0].get()" << "Name >> " <<typeid(m_pModuleList[0].get()).name() << NEWLINE;
	//std::cout << "m_pModuleList[0].get()" << "RawName > " << typeid(m_pModuleList[0].get()).raw_name() << NEWLINE;

	//std::cout << "*m_pModuleList[0]" << "Name >> " << typeid(*m_pModuleList[0]).name() << NEWLINE;
	//std::cout << "*m_pModuleList[0]" << "RawName >> " << typeid(*m_pModuleList[0]).raw_name() << NEWLINE;

	//std::cout << "*m_pModuleList[0].get()" << "Name >> " << typeid(*m_pModuleList[0].get()).name() << NEWLINE;
	//std::cout << "*m_pModuleList[0].get()" << "RawName >> " << typeid(*m_pModuleList[0].get()).raw_name() << NEWLINE;

	//std::cout << NEWLINE;
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CBomb::Draw()
{
	CSceneX::Draw();

}
//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CBomb::ShowDebugInfo()
{
#ifdef _DEBUG

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------������@----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[��][��][��][��] �J�[�\���ړ� \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Enter] ����{�^�� \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[BackSpace] �߂�{�^�� \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F7] �������邩�ǂ����ݒ� >> [%d] \n",m_bCanExplosion);


	ImGui::DragFloat3("BombPos", GetPos());

	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		m_bCanExplosion ^= 1;
	}

#endif //DEBUG
}

//------------------------------------------------------------------------------
//�����֐�
//------------------------------------------------------------------------------
S_ptr<CBomb> CBomb::CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, DIFFICULTY const difficulty)
{
	//�������m��
	S_ptr<CBomb> pBomb = std::make_shared<CBomb>();

	//������
	pBomb->Init();

	//���W�ƃT�C�Y�ݒ�
	pBomb->SetPos(pos);
	pBomb->SetRot(rot);

	pBomb->m_difficulty = difficulty;

	//���f�����ݒ�
	pBomb->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBBOX));

	//�{���̊O������
	pBomb->m_pBombExterior = CBomb_Exterior::CreateBombExterior(pBomb->GetMtxWorldPtr());

	//Scene���ŊǗ�
	pBomb->SetObjType(CScene::OBJTYPE_BOMB);
	pBomb->AddSharedList(pBomb);

	return pBomb;
}

//------------------------------------------------------------------------------
//�L�[�{�[�h����
//------------------------------------------------------------------------------
void CBomb::Operation_Keyboard()
{
	//�z�񂪋󂩑I��ԍ���-1�������ꍇ
	if (m_pModuleList.empty() || m_nSelectModuleNum < 0)
	{
		//return
		return;
	}

	//�L�[�{�[�h�̃|�C���^
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//Game�̎��_�ɂ���Ĕ���̑����ς���
	switch (CManager::GetGame()->GetGaze())
	{
	case CGame::GAZE_DEFAULT:
		break;

	case CGame::GAZE_BOMB:

		//���W���[���I��
		ModuleSelect();

		for (int nCnt = 0; nCnt < (int)m_pModuleList.size(); nCnt++)
		{
			//nullcheck
			if (m_pModuleList[nCnt].get())
			{
				//���݂̑I��ԍ��Ɠ������m����true�ɂ��Ă���
				nCnt == m_nSelectModuleNum ?
					m_pModuleList[nCnt]->SetSelect(true) :
					m_pModuleList[nCnt]->SetSelect(false);
			}
		}


		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			//���W���[������
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//�J�������߂Â���
			m_pModuleList[m_nSelectModuleNum]->CameraApproach();
		}

		break;

	case CGame::GAZE_MODULE:
		for (auto & rPtr : m_pModuleList)
		{
			//�I������
			rPtr->SetSelect(false);
			rPtr->SetOperating(false);
		}

		//nullcheck
		if (m_pModuleList[m_nSelectModuleNum].get())
		{

			//���ݑI������Ă��郂�W���[���𑀍�
			m_pModuleList[m_nSelectModuleNum]->Operation_Keyboard();
		}

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
//�}�E�X����
//------------------------------------------------------------------------------
void CBomb::Operation_Mouse()
{

	Operation_Camera();

	switch (CManager::GetGame()->GetGaze())
	{

		//�ʏ펞
	case CGame::GAZE_DEFAULT:

		break;

		//���e�����Ă鎞
	case CGame::GAZE_BOMB:

		//���C�̔���
		//���ƕ\�Ŕ���͈̔͂�ύX
		CHossoLibrary::RayCollision_ModuleSelect(m_pModuleList.begin() + (m_bCameraDir * 6), m_pModuleList.end() - ((!m_bCameraDir) * 6), m_nSelectModuleNum);

		//�����I������ĂȂ�����break
		if (m_nSelectModuleNum < 0)
		{
			break;
		}


		//if (CChatTab::GetTabletState() == CChatTab::TABSTATE_OPENED)
		//{
		//	return;
		//}

		//���\���]
		m_nSelectModuleNum += (m_bCameraDir * 6);

		//�}�E�X�N���b�N���ꂽ��
		if (CManager::GetMouse()->GetTrigger(0))
		{
			//���W���[������
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//��]�����ɖ߂�
			m_RotDest = ZeroVector3 + D3DXVECTOR3(0.0f, m_bCameraDir * D3DX_PI, 0.0f);

			//�I����ԉ���
			m_pModuleList[m_nSelectModuleNum]->SetSelect(false);

			//�J�����߂Â���
			m_pModuleList[m_nSelectModuleNum]->CameraApproach();
		}
		break;

		//���W���[�������Ă���Ƃ�
	case CGame::GAZE_MODULE:
		//nullcheck
		if (m_pModuleList[m_nSelectModuleNum].get())
		{
			//���ݑI������Ă��郂�W���[���𑀍�
			m_pModuleList[m_nSelectModuleNum]->Operation_Mouse();
		}
		break;
	}


}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
void CBomb::Operation_Camera()
{

	// �L�[�{�[�h�̎擾
	//Ckeyboard *pKeyboard = &CManager::GetKeyboard();

	// �}�E�X�̎擾
	CMouse *pMouse = CManager::GetMouse();
	// �}�E�X�̏�Ԃ��擾
	DIMOUSESTATE2* pMouseState = &pMouse->GetMouseState();


	static D3DXVECTOR3 m_MouseRotSave = ZeroVector3;

	D3DXVECTOR2 NewRotation = D3DXVECTOR2(0.0f, 0.0f);

	if (pMouse->GetPress(1) == true)
	{
		// ���[��]
		NewRotation.y = ((float)pMouseState->lX) / (D3DX_PI*2.0f) *0.02f;
		m_RotDest.y -= NewRotation.y;

		// ��]�ʂ�360�x�Ȃ��ɒ���
		CMylibrary::SetFixTheRotation(&m_RotDest.y);

		// �s�b�`���[��]
		NewRotation.x = ((float)pMouseState->lY) / (D3DX_PI*2.0f) *0.02f;

		// ��]��90�x�����ɗ}����
		CHossoLibrary::RangeLimit_Equal(NewRotation.x, -D3DX_PI * 0.49f, D3DX_PI * 0.49f);

		m_RotDest.x -= NewRotation.x;
	}

	if (pMouse->GetRelease(1))
	{
		m_MouseRotSave.y = m_RotDest.y;
		m_MouseRotSave.x = m_RotDest.x;
	}

	// �J�����̉�]��90�x�����ɗ}����
	CHossoLibrary::RangeLimit_Equal(m_RotDest.x, -D3DX_PI * 0.49f, D3DX_PI * 0.49f);

	// ��]�ʂ�360�x�Ȃ��ɒ���
	CHossoLibrary::CalcRotation(m_RotDest.y);

	//��]�̍���
	D3DXVECTOR3 rotDiff = m_RotDest - GetRot();

	//3.14�̒��Ɏ��߂�
	CHossoLibrary::CalcRotation_XYZ(rotDiff);

	//������ƂÂ�]
	GetRot() += (rotDiff * 0.15f);

	//3.14�̒��Ɏ��߂�
	CHossoLibrary::CalcRotation_XYZ(GetRot());


}

//------------------------------------------------------------------------------
//���W���[���N���A�������`�F�b�N
//------------------------------------------------------------------------------
void CBomb::ModuleClearCheck()
{
	//�N���A�t���O�̌������W���[�����ȏ�ɂȂ�����
	if (std::count_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> pModule) {return pModule->GetModuleClearFlag(); }) >= m_nModuleNum)
	{
		//�N���A�^�C���ݒ�
		GetModuleTimerPtr()->GetTimerPtr()->SaveClearTime();
		//�Q�[���N���A
		CManager::GetGame()->SetState(CGame::STATE_GAMECLEAR);
	}
}

//------------------------------------------------------------------------------
//���W���[���~�X��
//------------------------------------------------------------------------------
void CBomb::ModuleMiss()
{
#ifdef _DEBUG
	//�������Ȃ���Ԃ̂Ƃ���return
	if (!m_bCanExplosion) return;
#endif // _DEBUG


	//�~�X�J�E���g�A�b�v
	//�S���~�X�����Ƃ�
	if (GetModuleTimerPtr()->MissCountUp())
	{
		//�N���A�^�C���ݒ�
		GetModuleTimerPtr()->GetTimerPtr()->SaveClearTime();
		//�Q�[���I�[�o�[
		CManager::GetGame()->SetState(CGame::STATE_GAMEOVER);


	}
	else
	{
		//���Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_MISS);
	}
}

//------------------------------------------------------------------------------
//���W���[������
//------------------------------------------------------------------------------
CModule_Timer * CBomb::GetModuleTimerPtr()
{
	//�^�C�}�[�̃N���X�̃C�e���[�^�擾
	auto itr = std::find_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> const ptr) {return typeid(*ptr.get()) == typeid(CModule_Timer); });

	//�C�e���[�^�����������`�F�b�N
	if (itr != m_pModuleList.end())
	{
		//�^�C�}�[�^�ɕϊ�
		CModule_Timer *pTimer = dynamic_cast<CModule_Timer*>(itr->get());  // �_�C�i�~�b�N�L���X�g

		return pTimer;
	}

	return nullptr;
}

//------------------------------------------------------------------------------
//���W���[������
//------------------------------------------------------------------------------
void CBomb::CreateModule()
{

	//�������W���[����\���ł���͈͊O�̎��͎��߂�
	CHossoLibrary::RangeLimit_Equal(m_nModuleNum, 0, MAX_MODULE_NUM);

	//���W���[�������_������
	CreateModule_Random();

//Debug�p
#ifdef _DEBUG
	//CreateModuleDebug();
#endif //_DEBUG


	////���f�������
	//m_nModuleNum = 4;


	////1�Ԗ�
	//CBomb::CreateModuleOne<CModule_Timer>();
	////2�Ԗ�
	//CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();
	////3�Ԗ�
	//CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();
	////4�Ԗ�
	//CBomb::CreateModuleOne<CModule_No2_LampAndWire>();
	////5�Ԗ�
	//CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	////6�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();

	////7�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();
	////8�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();
	////9�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();
	////10�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();
	////11�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();
	////12�Ԗ�
	//CBomb::CreateModuleOne<CModule_None>();


	//�����������X�g�S�Ă�
	for (auto &ptr : m_pModuleList)
	{
		//�{���̃|�C���^�ݒ�
		ptr->SetBombPtr(shared_from_this());
	}

	//�I���\�ȃ��W���[������
	SearchHeadCanSelectNum(0);
}

//------------------------------------------------------------------------------
//���W���[�������@�����_������
//------------------------------------------------------------------------------
void CBomb::CreateModule_Random()
{
	//���[�J���̃��X�g
	Vec<CModule_Base::MODULE_TYPE> LocalList = {};

	//�^�C�}�[����
	CBomb::CreateModuleOne<CModule_Timer>();

	switch (m_difficulty)
	{
		//�^�C�g��
	case CBomb::TITLE:
		m_nModuleNum = MODULE_NUM_TITLE;
		GetModuleTimerPtr()->GetTimerPtr()->SetStop(true);
		break;

		//�C�[�W�[
	case CBomb::EASY:
		m_nModuleNum = MODULE_NUM_EASY;
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NO1_SHAPE);
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NO3_PASSWORD);

		break;

		//�m�[�}��
	case CBomb::NORMAL:
		m_nModuleNum = MODULE_NUM_NORMAL;

		break;

		//�n�[�h
	case CBomb::HARD:
		m_nModuleNum = MODULE_NUM_HARD;
		break;

	}

	//���W���[�����ɒB����܂Ń����_���Ń��W���[���ݒ�
	while ((int)LocalList.size() < m_nModuleNum)
	{
		//Button����4Button�܂ł̃����_��
		LocalList.emplace_back((CModule_Base::MODULE_TYPE)CHossoLibrary::RandomRangeUnsigned((int)CModule_Base::MODULE_TYPE::NO0_SYMBOL, (int)CModule_Base::MODULE_TYPE::MAX));
	}

	//�ő吔���܂�NONE�̃��W���[�������Ă���
	while (LocalList.size() < MAX_MODULE_NUM - 1)
	{
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NONE);
	}

	//�v�f�̃V���b�t��
	CHossoLibrary::Vec_Shuffle(LocalList);

	//���W���[������
	for(auto const &ModuleType : LocalList)
	{
		//���W���[���^�C�v�ɉ����Đ���
		switch (ModuleType)
		{
			//���W���[������
		case CModule_Base::MODULE_TYPE::NONE:
			CBomb::CreateModuleOne<CModule_None>();

			break;

			//�V���{��
		case CModule_Base::MODULE_TYPE::NO0_SYMBOL:
			CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();

			break;

			//�}�`
		case CModule_Base::MODULE_TYPE::NO1_SHAPE:
			CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();

			break;

			//���C���[
		case CModule_Base::MODULE_TYPE::NO2_WIRE:
			CBomb::CreateModuleOne<CModule_No2_LampAndWire>();

			break;

			//�p�X���[�h
		case CModule_Base::MODULE_TYPE::NO3_PASSWORD:
			CBomb::CreateModuleOne<CModule_No3_PassWord>();

			break;

			//4�F�{�^��
		case CModule_Base::MODULE_TYPE::NO4_4COLBUTTON:
			CBomb::CreateModuleOne<CModule_No4_4ColButton>();

			break;
		}
	}
}

//------------------------------------------------------------------------------
//���W���[���I��
//�I��s�ȃ��W���[��������̂Ń��C�u�����ɂ����Ƃ͕ʊ֐�
//------------------------------------------------------------------------------
void CBomb::ModuleSelect()
{
	//�I��ԍ�
	//1F�O�̑I��ԍ�
	int m_SelectNumOld = m_nSelectModuleNum;


	//�L�[�{�[�h�̃|�C���^
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	int nNumX = 3;
	int nNumY = 2;

	int nMin = (nNumX * nNumY) * m_bCameraDir;
	int nMax = nMin + (nNumX * nNumY) - 1;

	//���W���[���I������
	//���͂�������������break
	//���������ǂꂩ���͂���Ă����ꍇ
	if (pKeyboard->GetTrigger(DIK_LEFT) || pKeyboard->GetTrigger(DIK_RIGHT) ||
		pKeyboard->GetTrigger(DIK_UP) || pKeyboard->GetTrigger(DIK_DOWN))
	{
		while (1)
		{
			//��
			if (pKeyboard->GetTrigger(DIK_LEFT))
			{
				//�I��ԍ�--
				m_nSelectModuleNum--;

				//�͈͓��ɗ}����
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�񂪕ς��Ƃ�
				if (m_SelectNumOld / nNumX != m_nSelectModuleNum / nNumX)
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�I���ł��鎞
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
			}

			//��
			else if (pKeyboard->GetTrigger(DIK_RIGHT))
			{
				//�I��ԍ�++
				m_nSelectModuleNum++;


				//�͈͓��ɗ}����
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�񂪕ς��Ƃ�
				if (m_SelectNumOld / nNumX != m_nSelectModuleNum / nNumX)
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�I���ł��鎞
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
			}

			//��
			else if (pKeyboard->GetTrigger(DIK_UP))
			{
				//�I��ԍ� -= ������
				m_nSelectModuleNum -= nNumX;

				//�͈͓��ɗ}����
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�I���\�ȃ��W���[����������
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
				else
				{
					//�ړ���̗�̍��[�ɖ߂�
					int nValue = m_nSelectModuleNum / nNumX;
					m_nSelectModuleNum = nValue * nNumX;

					while (m_nSelectModuleNum <= nValue * nNumX + nNumX)
					{
						//�I���\�Ȏ�
						if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
						{
							//������
							break;
						}

						//�J�E���g�A�b�v
						m_nSelectModuleNum++;
					}

					//���̗�ɓ˓����Ă��܂����ꍇ
					if (nValue != m_nSelectModuleNum / nNumX)
					{
						//�߂�
						m_nSelectModuleNum = m_SelectNumOld;
					}
					break;
				}
			}

			//��
			else if (pKeyboard->GetTrigger(DIK_DOWN))
			{
				//�I��ԍ� += ������
				m_nSelectModuleNum += nNumX;

				//�͈͓��ɗ}����
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//���̒l�ɖ߂�
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//�I���\�ȃ��W���[����������
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}

				else
				{
					//�ړ���̗�̍��[�ɖ߂�
					int nValue = m_nSelectModuleNum / nNumX;
					m_nSelectModuleNum = nValue * nNumX;

					while (m_nSelectModuleNum <= nValue * nNumX + nNumX)
					{
						//�I���\�Ȏ�
						if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
						{
							//������
							break;
						}

						//�J�E���g�A�b�v
						m_nSelectModuleNum++;
					}

					//���̗�ɓ˓����Ă��܂����ꍇ
					if (nValue != m_nSelectModuleNum / nNumX)
					{
						//�߂�
						m_nSelectModuleNum = m_SelectNumOld;
					}
					break;
				}

			}
		}
	}
}

//------------------------------------------------------------------------------
//�I���\�ȃ��W���[������
//��ԍŏ��ɐ��������Ƃ��Ɨ��\�؂�ւ����Ɏg��
//------------------------------------------------------------------------------
void CBomb::SearchHeadCanSelectNum(int nStartNum)
{
	//�ŏ��̑I��ԍ��ݒ�
	for (int nCnt = nStartNum; nCnt < (int)m_pModuleList.size(); nCnt++)
	{
		//�I���\��������
		if (m_pModuleList[nCnt]->GetCanModuleSelect())
		{
			//���݂̑I��ԍ��ɐݒ�
			m_nSelectModuleNum = nCnt;
			return;
		}
	}
	//�I���\�ȃ��W���[�������݂��Ȃ������ꍇ
	m_nSelectModuleNum = -1;
}


#ifdef _DEBUG
//------------------------------------------------------------------------------
//���W���[�������@�f�o�b�O�p�@�����Ń��W���[�������߂��
//------------------------------------------------------------------------------
void CBomb::CreateModuleDebug()
{
	//���W���[�����������Ō��߂� debug�p
	m_nModuleNum = 1;

	//1�Ԗ�
	CBomb::CreateModuleOne<CModule_Timer>();
	//2�Ԗ�
	CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	//3�Ԗ�
	CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	//4�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//5�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//6�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();

	//7�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//8�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//9�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//10�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//11�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();
	//12�Ԗ�
	CBomb::CreateModuleOne<CModule_None>();

	//���W���[�����m�F
	m_nModuleNum = std::count_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> const &ptr) {return ptr->GetCanModuleSelect(); });

}
#endif //_DEBUG
