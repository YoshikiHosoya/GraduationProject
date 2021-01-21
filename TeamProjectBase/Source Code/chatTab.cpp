//=============================================================================
//
// �`���b�g�̃^�u���� [ chatTab.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "chatTab.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "texture.h"
#include "polygon2D.h"
#include "chatText.h"
#include "client.h"
#include "ImGui/imgui.h"			// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_dx9.h"	// Imgui�̎����ɕK�v
#include "ImGui/imgui_impl_win32.h"	// Imgui�̎����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEEPTEXT			(20)	// �ێ��ł���e�L�X�g�̍ő吔

#define POS_OPENTAB				(D3DXVECTOR2(980.0f, 720.0f))	// �J�����^�u�̍��W
#define POS_CLOSETAB			(D3DXVECTOR2(1280.0f, 720.0f))	// �����^�u�̍��W

#define SIZE_TABBACK			(D3DXVECTOR2(300.0f, 720.0f))	// �^�u�w�i�̃T�C�Y
#define SIZE_TABTITLE			(D3DXVECTOR2(300.0f, 100.0f))	// �^�u�^�C�g���̃T�C�Y
#define SIZE_TABWRITEWINDOW		(D3DXVECTOR2(300.0f, 100.0f))	// �^�u�������݃E�B���h�E�̃T�C�Y
#define SIZE_TABBUTTON			(D3DXVECTOR2(40.0f, 100.0f))	// �^�u�J�{�^���̃T�C�Y
#define SIZE_X_TEXTBOX			(280.0f)	// �e�L�X�g�{�b�N�X�̃T�C�Y
#define SIZE_Y_TEXTBOX			(100.0f)	// �e�L�X�g�{�b�N�X�̃T�C�Y
#define SIZE_X_TEXTBOX_PIC		(280.0f)	// �e�L�X�g�{�b�N�X�̃T�C�Y
#define SIZE_Y_TEXTBOX_PIC		(240.0f)	// �e�L�X�g�{�b�N�X�̃T�C�Y
#define SIZE_X_PICTURE			(260.0f)	// �s�N�`���̃T�C�Y
#define SIZE_Y_PICTURE			(190.0f)	// �s�N�`���̃T�C�Y

#define DIFPOS_X_TEXTBOX		(10.0f)		// �e�L�X�g�{�b�N�X�̍��W�̍���
#define DIFPOS_Y_TEXTBOX		(100.0f)	// �e�L�X�g�{�b�N�X�̍��W�̍���
#define DIFPOS_X_PICBOX			(10.0f)		// �s�N�`���{�b�N�X�̍��W�̍���
#define DIFPOS_Y_PICBOX			(250.0f)	// �s�N�`���{�b�N�X�̍��W�̍���
#define DIFPOS_X_PICTURE		(140.0f)	// �s�N�`���̍��W�̍���
#define DIFPOS_Y_PICTURE		(138.0f)	// �s�N�`���̍��W�̍���
#define DIFPOS_Y_TEXTBOX_UNDER	(85.0f)		// �e�L�X�g�{�b�N�X�̍��W�̍���
#define DIFPOS_X_KEEPTEXT		(5.0f)		// �ێ�����e�L�X�g�̍��W�̍���
#define DIFPOS_Y_KEEPTEXT		(40.0f)		// �ێ�����e�L�X�g�̍��W�̍���
#define DIFPOS_X_SENDTEXT		(15.0f)		// ���M�p�e�L�X�g�̍��W�̍���
#define DIFPOS_Y_SENDTEXT		(63.0f)		// ���M�p�e�L�X�g�̍��W�̍���
#define DIFPOS_X_MAXCHAR		(190.0f)	// �������\���p�e�L�X�g�̍��W�̍���
#define DIFPOS_Y_MAXCHAR		(23.0f)		// �������\���p�e�L�X�g�̍��W�̍���

#define TIME_TABMOVE			(10)		// �^�u�J�̎���
#define TIME_PRESSKEY			(40)		// �L�[�̒���������

#define COLOR_TABCLICK			(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// �^�u�N���b�N���̃J���[

#define MOVE_SCROLL_TAB			(30.0f)	// �}�E�X�X�N���[���ɂ����W�ړ�

#define LINK_SENDPICTURE		("data/SAVEDATA/PictureTextures/PicTex.txt")	// ���M�p�s�N�`���̃p�X

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
D3DXVECTOR2			CChatTab::m_TabPos		= ZeroVector2;
float				CChatTab::m_fScrollPosY = 0.0f;
CChatTab::TABSTATE	CChatTab::m_tabState	= CChatTab::TABSTATE_OPENNING;
int					CChatTab::m_nCntPress = 0;
int					CChatTab::m_nPressKey = 0;
CChatText			*CChatTab::m_SendText = nullptr;
CChatText			*CChatTab::m_leftText = nullptr;
std::vector<CChatTab::CHATKEEP>	CChatTab::m_chatKeep = {};
std::vector<CChatTab::CHATKEEP>	CChatTab::m_recvKeep = {};
CChatTab::TABSTATE	CChatTab::m_tabletState = CChatTab::TABSTATE_CLOSED;
bool				CChatTab::m_bClickTab = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CChatTab::CChatTab()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CChatTab::~CChatTab()
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_pChatPoly[nCnt])
		{
			delete m_pChatPoly[nCnt];
			m_pChatPoly[nCnt] = nullptr;
		}
	}

	if (m_SendText)
	{
		m_SendText->Uninit();
		delete m_SendText;
		m_SendText = nullptr;
	}

	if (m_leftText)
	{
		m_leftText->Uninit();
		delete m_leftText;
		m_leftText = nullptr;
	}

	for (int nCnt = 0; nCnt < (int)m_chatKeep.size(); nCnt++)
	{
		if (m_chatKeep[nCnt].pPolyBack)
		{
			delete m_chatKeep[nCnt].pPolyBack;
			m_chatKeep[nCnt].pPolyBack = nullptr;
		}
		if (m_chatKeep[nCnt].pPolyPic)
		{
			delete m_chatKeep[nCnt].pPolyPic;
			m_chatKeep[nCnt].pPolyPic = nullptr;
		}
		if (m_chatKeep[nCnt].pKeepText)
		{
			m_chatKeep[nCnt].pKeepText->Uninit();
			delete m_chatKeep[nCnt].pKeepText;
			m_chatKeep[nCnt].pKeepText = nullptr;
		}
	}
	m_chatKeep.clear();
}

//=============================================================================
// �`���b�g�����̐���
//=============================================================================
void CChatTab::CreateChatKeep(void)
{
	// �ێ����Ă鐔�����J��Ԃ�
	for (int nRecv = 0; nRecv < (int)m_recvKeep.size(); nRecv++)
	{
		CHATKEEP keep;
		keep.pPolyBack = CPolygon2D::Create();
		keep.pPolyBack->SetPos(m_recvKeep[nRecv].pPolyBack->GetPos());
		keep.pPolyBack->SetSize(m_recvKeep[nRecv].pPolyBack->GetSize());
		keep.pPolyBack->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
		keep.owner = m_recvKeep[nRecv].owner;

		if (m_recvKeep[nRecv].type == KEEPTYPE_TEXT)
		{
			keep.pKeepText = CChatText::Create();
			keep.pKeepText->SetKeepColor(BlackColor);
			keep.pKeepText->SetKeepRectBegin(D3DXVECTOR2((float)m_recvKeep[nRecv].pKeepText->GetRect().left, (float)m_recvKeep[nRecv].pKeepText->GetRect().top));
			keep.pKeepText->SetChatText((char*)m_recvKeep[nRecv].pKeepText->GetChatText().c_str());
			keep.owner == CChatBase::OWNER_OWN ?
				keep.pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_00)) :
				keep.pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_01));
		}
		else if (m_recvKeep[nRecv].type == KEEPTYPE_PICTURE)
		{
			keep.pPolyPic = CPolygon2D::Create();
			keep.pPolyPic->SetPos(m_recvKeep[nRecv].pPolyPic->GetPos());
			keep.pPolyPic->SetSize(m_recvKeep[nRecv].pPolyPic->GetSize());
			keep.pPolyPic->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
			keep.pPolyPic->BindTexture(m_recvKeep[nRecv].pPolyPic->GetTexture());

			keep.owner == CChatBase::OWNER_OWN ?
				keep.pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_02)) :
				keep.pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_03));
		}

		keep.type = m_recvKeep[nRecv].type;

		// �����̖����ɒǉ�
		m_chatKeep.emplace_back(keep);

		// �����̔ԍ����擾
		int nNumber = (int)m_chatKeep.size() - 1;

		for (int nCnt = nNumber - 1; nCnt > -1; nCnt--)
		{
			if (!m_chatKeep[nCnt].pKeepText)
				continue;

			// ���W�X�V
			D3DXVECTOR2 pos = m_chatKeep[nCnt + 1].pPolyBack->GetPos();
			m_chatKeep[nCnt].pPolyBack->SetPos(D3DXVECTOR2(pos.x, pos.y + SIZE_Y_TEXTBOX));
			pos = m_chatKeep[nCnt].pPolyBack->GetPos();
			m_chatKeep[nCnt].pKeepText->SetKeepRectBegin(D3DXVECTOR2(pos.x + DIFPOS_X_KEEPTEXT, pos.y + DIFPOS_Y_KEEPTEXT));
		}

		// �`���b�g�E�s�N�`���̂ǂ��炩�̂�
		if (m_chatKeep[nNumber].type == KEEPTYPE_TEXT)
		{
			m_chatKeep[nNumber].pPolyPic = nullptr;
			m_recvKeep[nRecv].pPolyPic = nullptr;
		}
		else if (m_chatKeep[nNumber].type == KEEPTYPE_PICTURE)
		{
			m_chatKeep[nNumber].pKeepText = nullptr;
			m_recvKeep[nRecv].pKeepText = nullptr;
		}

		// �ő吔�𒴂�����A�Â��ق�����폜
		if (nNumber >= MAX_KEEPTEXT)
			m_chatKeep.erase(m_chatKeep.begin());
	}

	for (int nCnt = 0; nCnt < (int)m_recvKeep.size(); nCnt++)
	{
		if (m_recvKeep[nCnt].pPolyBack)
		{
			delete m_recvKeep[nCnt].pPolyBack;
			m_recvKeep[nCnt].pPolyBack = nullptr;
		}
		if (m_recvKeep[nCnt].pPolyPic)
		{
			delete m_recvKeep[nCnt].pPolyPic;
			m_recvKeep[nCnt].pPolyPic = nullptr;
		}
		if (m_recvKeep[nCnt].pKeepText)
		{
			m_recvKeep[nCnt].pKeepText->Uninit();
			delete m_recvKeep[nCnt].pKeepText;
			m_recvKeep[nCnt].pKeepText = nullptr;
		}
	}
	m_recvKeep.clear();
}

//=============================================================================
// �`���b�g�^�u�̃N���b�N
//=============================================================================
void CChatTab::ClickTab(void)
{
	// �X���C�h���Ă���
	if (m_nCntState > 0)
		return;

	// ��Ԃ�ύX
	switch (m_tabState)
	{
	case TABSTATE_OPENED:
		m_tabState = TABSTATE_CLOSING;
		break;
	case TABSTATE_CLOSED:
		m_tabState = TABSTATE_OPENNING;
		break;
	}
	// �J���[�X�V
	m_pChatPoly[POLY_TAB]->SetCol(COLOR_TABCLICK);
}

//=============================================================================
// �^�u���b�g�J�^�u�̃N���b�N
//=============================================================================
void CChatTab::ClickTabletTab(void)
{
	CMouse *pMouse = CManager::GetMouse();
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY());

	// �J���[�X�V
	if (m_pChatPoly[POLY_TABLET]->GetCol() != WhiteColor)
		m_pChatPoly[POLY_TABLET]->SetCol(WhiteColor);
	//	�t���O����
	if (m_bClickTab)
		m_bClickTab = false;

	if (pMouse->GetTrigger(0) && m_pChatPoly[POLY_TABLET]->ReturnHit(mousePos))
	{
		// �J���[�X�V
		m_pChatPoly[POLY_TABLET]->SetCol(COLOR_TABCLICK);

		m_tabletState == TABSTATE_OPENED ?
			m_tabletState = TABSTATE_CLOSED :
			m_tabletState = TABSTATE_OPENED;

		// �t���Oon
		m_bClickTab = true;
	}
}

//=============================================================================
// �`���b�g�^�u�̃X���C�h
//=============================================================================
void CChatTab::SlideTab(void)
{
	// �ŏ������ʂ�
	if (m_nCntState == 0)
	{
		// �^�u�̈ړ��ʂ��v�Z
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_moveDest = (POS_OPENTAB - m_TabPos) / TIME_TABMOVE;
			break;
		case TABSTATE_CLOSING:
			m_moveDest = (POS_CLOSETAB - m_TabPos) / TIME_TABMOVE;
			break;
		}
	}

	// �J���[�X�V
	if (m_nCntState == TIME_TABMOVE - 1)
		m_pChatPoly[POLY_TAB]->SetCol(WhiteColor);

	// �J�E���^�����Z
	m_nCntState++;
	// ���ԂŃX���C�h�I��
	if (m_nCntState > TIME_TABMOVE)
	{
		// �J�E���^�����Z�b�g
		m_nCntState = 0;
		// ��Ԃ�ύX
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_tabState = TABSTATE_OPENED;
			m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABCLOSE));
			break;
		case TABSTATE_CLOSING:
			m_tabState = TABSTATE_CLOSED;
			m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));
			break;
		}
		// �����̏I��
		return;
	}

	// �ړ��l�����Z���A���W�X�V
	m_TabPos += m_moveDest;
	m_pChatPoly[POLY_BACK]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TAB]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TABLET]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y - 100.0f));
	m_pChatPoly[POLY_TITLE]->SetPos(D3DXVECTOR2(m_TabPos.x, 0.0f));
	m_pChatPoly[POLY_WRITEWINDOW]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));

	m_SendText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_SENDTEXT, m_TabPos.y - DIFPOS_Y_SENDTEXT));
	m_leftText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_MAXCHAR, m_TabPos.y - DIFPOS_Y_MAXCHAR));
}

//=============================================================================
// ����
//=============================================================================
CChatTab * CChatTab::Create(void)
{
	// �������m��
	CChatTab *pTab = new CChatTab();

	// �m�ێ��s
	if (!pTab)
		return nullptr;

	// ������
	pTab->Init();
	// �l��Ԃ�
	return pTab;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CChatTab::Init(void)
{
	// �v�f�̏�����
	m_TabPos = POS_CLOSETAB;
	m_fScrollPosY = 0.0f;
	m_moveDest = ZeroVector2;
	m_tabState = TABSTATE_CLOSED;
	m_tabletState = TABSTATE_CLOSED;
	m_nCntState = 0;
	m_bClickTab = false;

	// �w�i�̐���
	m_pChatPoly[POLY_BACK] = CPolygon2D::Create();
	m_pChatPoly[POLY_BACK]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_BACK]->SetSize(SIZE_TABBACK);
	m_pChatPoly[POLY_BACK]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pChatPoly[POLY_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BACK));

	m_pChatPoly[POLY_TITLE] = CPolygon2D::Create();
	m_pChatPoly[POLY_TITLE]->SetPos(D3DXVECTOR2(m_TabPos.x, 0.0f));
	m_pChatPoly[POLY_TITLE]->SetSize(SIZE_TABTITLE);
	m_pChatPoly[POLY_TITLE]->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	m_pChatPoly[POLY_TITLE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TITLE));

	m_pChatPoly[POLY_WRITEWINDOW] = CPolygon2D::Create();
	m_pChatPoly[POLY_WRITEWINDOW]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_WRITEWINDOW]->SetSize(SIZE_TABWRITEWINDOW);
	m_pChatPoly[POLY_WRITEWINDOW]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pChatPoly[POLY_WRITEWINDOW]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_WRITEWINDOW));

	// �J�^�u�̐���
	m_pChatPoly[POLY_TAB] = CPolygon2D::Create();
	m_pChatPoly[POLY_TAB]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TAB]->SetSize(SIZE_TABBUTTON);
	m_pChatPoly[POLY_TAB]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));

	// �^�u���b�g�J�̐���
	m_pChatPoly[POLY_TABLET] = CPolygon2D::Create();
	m_pChatPoly[POLY_TABLET]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y - 100.0f));
	m_pChatPoly[POLY_TABLET]->SetSize(SIZE_TABBUTTON);
	m_pChatPoly[POLY_TABLET]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pChatPoly[POLY_TABLET]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABLETOPEN));

	// ���M�p�e�L�X�g�̐���
	m_SendText = CChatText::Create();
	m_SendText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_SENDTEXT, m_TabPos.y - DIFPOS_Y_SENDTEXT));

	// �c�蕶�����e�L�X�g�̐���
	m_leftText = CChatText::Create();
	m_leftText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_MAXCHAR, m_TabPos.y - DIFPOS_Y_MAXCHAR));

	m_nCntPress = 0;

	return S_OK;
}

//=============================================================================
// �X�V
//=============================================================================
void CChatTab::Update(void)
{
	// ��M���Ă���΁A�`���b�g�����𐶐�
	if (m_recvKeep.size() > 0)
		CreateChatKeep();

	CMouse *pMouse = CManager::GetMouse();
	CKeyboard *pKey = CManager::GetKeyboard();
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY());

	// �^�u�̃X���C�h
	if (m_tabState == TABSTATE_CLOSING || m_tabState == TABSTATE_OPENNING)
		SlideTab();

	// ���N���b�N��F5�Ń^�u�̊J����
	if ((pMouse->GetTrigger(0) && CheckMouseHit(POLY_TAB))  ||
		CManager::GetKeyboard()->GetTrigger(DIK_F5))
		ClickTab();

	ClickTabletTab();

	// �}�E�X�ɂ��^�u�X�N���[��
	CMouse::MOUSE_SCROLL scroll = CMouse::GetScroll();
	if (scroll == CMouse::SCROLL_UP)
		ScrollUp();
	else if (scroll == CMouse::SCROLL_DOWN)
		ScrollDown();

	// �^�u�|���S���̍X�V
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_pChatPoly[nCnt])
			m_pChatPoly[nCnt]->Update();
	}

	float fAllChatSize = DIFPOS_Y_TEXTBOX;

	// �`���b�g�����̍X�V
	for (int nCnt = (int)m_chatKeep.size() - 1; nCnt > -1; nCnt--)
	{
		if (!m_chatKeep[nCnt].pPolyBack)
			continue;

		D3DXVECTOR2 pos;

		if (m_chatKeep[nCnt].pKeepText)
		{
			// ���W�ݒ�
			pos = D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX,
				fAllChatSize + m_fScrollPosY);
			// �w�i�|���S���̍��W������
			m_chatKeep[nCnt].pPolyBack->SetPos(pos);
			m_chatKeep[nCnt].pPolyBack->Update();
			// �w�i���W�����ƂɁA�e�L�X�g���W������
			m_chatKeep[nCnt].pKeepText->SetKeepRectBegin(D3DXVECTOR2(pos.x + DIFPOS_X_KEEPTEXT, pos.y + DIFPOS_Y_KEEPTEXT));

			fAllChatSize += DIFPOS_Y_TEXTBOX;
		}
		else if (m_chatKeep[nCnt].pPolyPic)
		{
			// ���W�ݒ�
			pos = D3DXVECTOR2(m_TabPos.x + DIFPOS_X_PICBOX,
				fAllChatSize + m_fScrollPosY);

			// �w�i�|���S���̍��W������
			m_chatKeep[nCnt].pPolyBack->SetPos(pos);
			m_chatKeep[nCnt].pPolyBack->Update();
			// �w�i���W�����ƂɁA�e�L�X�g���W������
			m_chatKeep[nCnt].pPolyPic->SetPos(D3DXVECTOR2(pos.x + DIFPOS_X_PICTURE, pos.y + DIFPOS_Y_PICTURE));
			m_chatKeep[nCnt].pPolyPic->Update();

			fAllChatSize += DIFPOS_Y_PICBOX;
		}
	}

	// �^�u���J���Ă���Ƃ��̂݁A��������͂ł���
	if (m_tabState == TABSTATE_OPENED)
		InputText();

	// �c�莞��������
	char cText[32];
	sprintf(cText, "Left %d/%d", SIZE_CHATTEXT - (int)m_SendText->GetChatText().size(), SIZE_CHATTEXT);
	m_leftText->SetChatText(cText);
}

//=============================================================================
// �`��
//=============================================================================
void CChatTab::Draw(void)
{
	if (m_pChatPoly[POLY_TAB])
		m_pChatPoly[POLY_TAB]->Draw();

	if (m_pChatPoly[POLY_TABLET])
		m_pChatPoly[POLY_TABLET]->Draw();

	if (m_pChatPoly[POLY_BACK])
		m_pChatPoly[POLY_BACK]->Draw();

	for (int nCnt = 0; nCnt < (int)m_chatKeep.size(); nCnt++)
	{
		if (m_chatKeep[nCnt].pPolyBack)
			m_chatKeep[nCnt].pPolyBack->Draw();
		if (m_chatKeep[nCnt].pKeepText)
			m_chatKeep[nCnt].pKeepText->Draw();
		if (m_chatKeep[nCnt].pPolyPic)
			m_chatKeep[nCnt].pPolyPic->Draw();
	}

	if (m_pChatPoly[POLY_TITLE])
		m_pChatPoly[POLY_TITLE]->Draw();

	if (m_pChatPoly[POLY_WRITEWINDOW])
		m_pChatPoly[POLY_WRITEWINDOW]->Draw();

	if (m_SendText)
		m_SendText->Draw();
	if (m_leftText)
		m_leftText->Draw();
}

//==========================================================================================================================================================
// �L�[�{�[�h����
//==========================================================================================================================================================
void CChatTab::InputText(void)
{
	bool bPress = false;

	// �L�[�{�[�h�̎擾
	CKeyboard *pKey = CManager::GetKeyboard();

	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		// ������Ă��Ȃ��Ȃ�΁A�I��
		if (pKey->GetPress(nCnt) != true ||
			nCnt == DIK_LSHIFT || nCnt == DIK_RSHIFT)
			continue;

		// ���͂���
		bPress = true;

		// ���̃L�[����͂��Ă�����A�J�E���^���Z�b�g
		if (m_nPressKey != nCnt)
			m_nCntPress = 0;
		// �L�[�ԍ���ۑ�
		m_nPressKey = nCnt;

		// �L�[����
		PressKey(nCnt, (pKey->GetPress(DIK_LSHIFT) || pKey->GetPress(DIK_RSHIFT)));

		// �������J�E���^���Z
		if (m_nCntPress < TIME_PRESSKEY)
			m_nCntPress++;
	}

	if (!bPress)
	{
		m_nCntPress = 0;
		m_nPressKey = 0;
	}

	// ������̑��M
	if ((pKey->GetTrigger(DIK_RETURN) || pKey->GetTrigger(DIK_NUMPADENTER)) &&
		m_SendText->GetChatText().size() > 0)
	{
		SendChatText();
	}
}

//==========================================================================================================================================================
// �L�[����
//==========================================================================================================================================================
void CChatTab::PressKey(int nKeyID, bool bShift)
{
	// �V�t�g�L�[���L��
	if (bShift)
	{
		// �P������
		if (m_nCntPress == 1)
			SetChatShiftKeyInfo(nKeyID);
		// �A������
		if (m_nCntPress >= TIME_PRESSKEY)
			SetChatShiftKeyInfo(nKeyID);
		// �������I����
		return;
	}

	if (nKeyID != DIK_BACKSPACE)
	{
		// �P������
		if (m_nCntPress == 1)
			SetChatKeyInfo(nKeyID);
		// �A������
		if (m_nCntPress >= TIME_PRESSKEY)
			SetChatKeyInfo(nKeyID);
	}
	else
	{
		// �P������
		if (m_nCntPress == 1 && (int)m_SendText->GetChatText().size() > 0)
			m_SendText->GetChatText().pop_back();
		// �A������
		if (m_nCntPress >= TIME_PRESSKEY && (int)m_SendText->GetChatText().size() > 0)
			m_SendText->GetChatText().pop_back();
	}
}

//==========================================================================================================================================================
// �e�L�X�g�̑��M
//==========================================================================================================================================================
void CChatTab::SendChatText(void)
{
	// �L�������e�L�X�g�𑗐M
	std::thread t2(CClient::SendText);
	t2.detach();
}

//==========================================================================================================================================================
// �}�E�X�X�N���[���ɂ����W�㏸
//==========================================================================================================================================================
void CChatTab::ScrollUp(void)
{
	// �Q�[�����̂�
	if (CManager::GetMode() != CManager::MODE_GAME || m_tabState != TABSTATE_OPENED)
		return;
	// �㏸
	m_fScrollPosY += MOVE_SCROLL_TAB;

	if (m_fScrollPosY > -DIFPOS_Y_TEXTBOX_UNDER + (int)m_chatKeep.size() * SIZE_Y_TEXTBOX)
		m_fScrollPosY = -DIFPOS_Y_TEXTBOX_UNDER + (int)m_chatKeep.size() * SIZE_Y_TEXTBOX;
}

//==========================================================================================================================================================
// �}�E�X�X�N���[���ɂ����W���~
//==========================================================================================================================================================
void CChatTab::ScrollDown(void)
{
	// �Q�[�����̂�
	if (CManager::GetMode() != CManager::MODE_GAME || m_tabState != TABSTATE_OPENED)
		return;
	// ���~
	m_fScrollPosY -= MOVE_SCROLL_TAB;

	if (m_fScrollPosY < DIFPOS_Y_TEXTBOX + (int)m_chatKeep.size() * -SIZE_Y_TEXTBOX)
		m_fScrollPosY = DIFPOS_Y_TEXTBOX + (int)m_chatKeep.size() * -SIZE_Y_TEXTBOX;
}

//==========================================================================================================================================================
// �`���b�g�L�[�v�̐���
//==========================================================================================================================================================
void CChatTab::AddRecvKeep(CChatBase::TEXTOWNER owner, char *cText)
{
	// �T�C�Y�m��
	char *cRecvText = new char[strlen(cText) + 1];
	// �e�L�X�g���i�[
	strcpy(cRecvText, cText);

	// ���i�[�p
	CHATKEEP keep;
	// �w�i�|���S��
	keep.pPolyBack = CPolygon2D::Create();
	keep.pPolyBack->SetPos(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX, DIFPOS_Y_TEXTBOX));
	keep.pPolyBack->SetSize(D3DXVECTOR2(SIZE_X_TEXTBOX, SIZE_Y_TEXTBOX));
	keep.pPolyBack->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
		// ������
	D3DXVECTOR3 BackPos = keep.pPolyBack->GetPos();
	keep.pKeepText = CChatText::Create();
	keep.pKeepText->SetKeepColor(BlackColor);
	keep.pKeepText->SetKeepRectBegin(D3DXVECTOR2(BackPos.x + DIFPOS_X_KEEPTEXT, BackPos.y + DIFPOS_Y_KEEPTEXT));
	D3DXVECTOR2 pos = keep.pPolyBack->GetPos();
	keep.pPolyBack->SetPos(D3DXVECTOR2(pos.x, pos.y + SIZE_Y_TEXTBOX));
	pos = keep.pPolyBack->GetPos();
	keep.pKeepText->SetKeepRectBegin(D3DXVECTOR2(pos.x + DIFPOS_X_KEEPTEXT, pos.y + DIFPOS_Y_KEEPTEXT));
	// ������i�[
	keep.pKeepText->SetChatText(cRecvText);

	keep.owner = owner;
	keep.type = KEEPTYPE_TEXT;
	// ��M�p�L�[�v�Ɋi�[
	m_recvKeep.push_back(keep);

	delete[] cRecvText;
}

//==========================================================================================================================================================
// �s�N�`���̒ǉ�
//==========================================================================================================================================================
void CChatTab::AddRecvPicture(CChatBase::TEXTOWNER owner, LPDIRECT3DTEXTURE9 pTexture)
{
	// �e�L�X�g�𖖔��ɒǉ�
	CHATKEEP keep;

	// �w�i�|���S���̐���
	keep.pPolyBack = CPolygon2D::Create();
	keep.pPolyBack->SetPos(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX, DIFPOS_Y_TEXTBOX));
	keep.pPolyBack->SetSize(D3DXVECTOR2(SIZE_X_TEXTBOX_PIC, SIZE_Y_TEXTBOX_PIC));
	keep.pPolyBack->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	keep.owner = owner;
	keep.type = KEEPTYPE_PICTURE;

	// �s�N�`���̐���
	D3DXVECTOR2 BackPos = keep.pPolyBack->GetPos();
	keep.pPolyPic = CPolygon2D::Create();
	keep.pPolyPic->SetPos(BackPos);
	keep.pPolyPic->SetSize(D3DXVECTOR2(SIZE_X_PICTURE, SIZE_Y_PICTURE));
	keep.pPolyPic->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	keep.pPolyPic->BindTexture(pTexture);

	// �L�[�v�̖����ɒǉ�
	m_recvKeep.push_back(keep);
}

//==========================================================================================================================================================
// �}�E�X�ɓ����������ǂ����`�F�b�N
//==========================================================================================================================================================
bool CChatTab::CheckMouseHit(POLYTYPE polygon)
{
	return 	m_pChatPoly[polygon]->ReturnHit(CManager::GetMouse()->GetMousePos());
}

//==========================================================================================================================================================
// �`���b�g�̃L�[���̐ݒ�
//==========================================================================================================================================================
void CChatTab::SetChatKeyInfo(int nKeyID)
{
	// �������𒴂��Ă�����A�������Ȃ�
	if ((int)m_SendText->GetChatText().size() >= SIZE_CHATTEXT)
		return;

	// ������ǉ�
	switch (nKeyID)
	{
	case DIK_1:				m_SendText->GetChatText().push_back('1'); break;
	case DIK_2:				m_SendText->GetChatText().push_back('2'); break;
	case DIK_3:				m_SendText->GetChatText().push_back('3'); break;
	case DIK_4:				m_SendText->GetChatText().push_back('4'); break;
	case DIK_5:				m_SendText->GetChatText().push_back('5'); break;
	case DIK_6:				m_SendText->GetChatText().push_back('6'); break;
	case DIK_7:				m_SendText->GetChatText().push_back('7'); break;
	case DIK_8:				m_SendText->GetChatText().push_back('8'); break;
	case DIK_9:				m_SendText->GetChatText().push_back('9'); break;
	case DIK_0:				m_SendText->GetChatText().push_back('0'); break;
	case DIK_A:				m_SendText->GetChatText().push_back('a'); break;
	case DIK_B:				m_SendText->GetChatText().push_back('b'); break;
	case DIK_C:				m_SendText->GetChatText().push_back('c'); break;
	case DIK_D:				m_SendText->GetChatText().push_back('d'); break;
	case DIK_E:				m_SendText->GetChatText().push_back('e'); break;
	case DIK_F:				m_SendText->GetChatText().push_back('f'); break;
	case DIK_G:				m_SendText->GetChatText().push_back('g'); break;
	case DIK_H:				m_SendText->GetChatText().push_back('h'); break;
	case DIK_I:				m_SendText->GetChatText().push_back('i'); break;
	case DIK_J:				m_SendText->GetChatText().push_back('j'); break;
	case DIK_K:				m_SendText->GetChatText().push_back('k'); break;
	case DIK_L:				m_SendText->GetChatText().push_back('l'); break;
	case DIK_M:				m_SendText->GetChatText().push_back('m'); break;
	case DIK_N:				m_SendText->GetChatText().push_back('n'); break;
	case DIK_O:				m_SendText->GetChatText().push_back('o'); break;
	case DIK_P:				m_SendText->GetChatText().push_back('p'); break;
	case DIK_Q:				m_SendText->GetChatText().push_back('q'); break;
	case DIK_R:				m_SendText->GetChatText().push_back('r'); break;
	case DIK_S:				m_SendText->GetChatText().push_back('s'); break;
	case DIK_T:				m_SendText->GetChatText().push_back('t'); break;
	case DIK_U:				m_SendText->GetChatText().push_back('u'); break;
	case DIK_V:				m_SendText->GetChatText().push_back('v'); break;
	case DIK_W:				m_SendText->GetChatText().push_back('w'); break;
	case DIK_X:				m_SendText->GetChatText().push_back('x'); break;
	case DIK_Y:				m_SendText->GetChatText().push_back('y'); break;
	case DIK_Z:				m_SendText->GetChatText().push_back('z'); break;
	case DIK_MINUS:			m_SendText->GetChatText().push_back('-'); break;
	case DIK_CIRCUMFLEX:	m_SendText->GetChatText().push_back('^'); break;
	case DIK_YEN:			m_SendText->GetChatText().push_back('\\'); break;
	case DIK_AT:			m_SendText->GetChatText().push_back('@'); break;
	case DIK_LBRACKET:		m_SendText->GetChatText().push_back('['); break;
	case DIK_SEMICOLON:		m_SendText->GetChatText().push_back(';'); break;
	case DIK_COLON:			m_SendText->GetChatText().push_back(':'); break;
	case DIK_RBRACKET:		m_SendText->GetChatText().push_back(']'); break;
	case DIK_COMMA:			m_SendText->GetChatText().push_back(','); break;
	case DIK_PERIOD:		m_SendText->GetChatText().push_back('.'); break;
	case DIK_SLASH:			m_SendText->GetChatText().push_back('/'); break;
	case DIK_BACKSLASH:		m_SendText->GetChatText().push_back('\\'); break;
	case DIK_SPACE:			m_SendText->GetChatText().push_back(' '); break;
	case DIK_NUMPAD0:		m_SendText->GetChatText().push_back('0'); break;
	case DIK_NUMPAD1:		m_SendText->GetChatText().push_back('1'); break;
	case DIK_NUMPAD2:		m_SendText->GetChatText().push_back('2'); break;
	case DIK_NUMPAD3:		m_SendText->GetChatText().push_back('3'); break;
	case DIK_NUMPAD4:		m_SendText->GetChatText().push_back('4'); break;
	case DIK_NUMPAD5:		m_SendText->GetChatText().push_back('5'); break;
	case DIK_NUMPAD6:		m_SendText->GetChatText().push_back('6'); break;
	case DIK_NUMPAD7:		m_SendText->GetChatText().push_back('7'); break;
	case DIK_NUMPAD8:		m_SendText->GetChatText().push_back('8'); break;
	case DIK_NUMPAD9:		m_SendText->GetChatText().push_back('9'); break;
	case DIK_DECIMAL:		m_SendText->GetChatText().push_back('.'); break;
	case DIK_ADD:			m_SendText->GetChatText().push_back('+'); break;
	case DIK_SUBTRACT:		m_SendText->GetChatText().push_back('-'); break;
	case DIK_MULTIPLY:		m_SendText->GetChatText().push_back('*'); break;
	case DIK_DIVIDE:		m_SendText->GetChatText().push_back('/'); break;
	}
}

//==========================================================================================================================================================
// �`���b�g�̃V�t�g�L�[���̐ݒ�
//==========================================================================================================================================================
void CChatTab::SetChatShiftKeyInfo(int nKeyID)
{
	// �������𒴂��Ă�����A�������Ȃ�
	if ((int)m_SendText->GetChatText().size() >= SIZE_CHATTEXT)
		return;

	// ������ǉ�
	switch (nKeyID)
	{
	case DIK_1:				m_SendText->GetChatText().push_back('!'); break;
	case DIK_2:				m_SendText->GetChatText().push_back('"'); break;
	case DIK_3:				m_SendText->GetChatText().push_back('#'); break;
	case DIK_4:				m_SendText->GetChatText().push_back('$'); break;
	case DIK_5:				break;
	case DIK_6:				m_SendText->GetChatText().push_back('&'); break;
	case DIK_7:				break;
	case DIK_8:				m_SendText->GetChatText().push_back('('); break;
	case DIK_9:				m_SendText->GetChatText().push_back(')'); break;
	case DIK_0:				break;
	case DIK_A:				m_SendText->GetChatText().push_back('A'); break;
	case DIK_B:				m_SendText->GetChatText().push_back('B'); break;
	case DIK_C:				m_SendText->GetChatText().push_back('C'); break;
	case DIK_D:				m_SendText->GetChatText().push_back('D'); break;
	case DIK_E:				m_SendText->GetChatText().push_back('E'); break;
	case DIK_F:				m_SendText->GetChatText().push_back('F'); break;
	case DIK_G:				m_SendText->GetChatText().push_back('G'); break;
	case DIK_H:				m_SendText->GetChatText().push_back('H'); break;
	case DIK_I:				m_SendText->GetChatText().push_back('I'); break;
	case DIK_J:				m_SendText->GetChatText().push_back('J'); break;
	case DIK_K:				m_SendText->GetChatText().push_back('K'); break;
	case DIK_L:				m_SendText->GetChatText().push_back('L'); break;
	case DIK_M:				m_SendText->GetChatText().push_back('M'); break;
	case DIK_N:				m_SendText->GetChatText().push_back('N'); break;
	case DIK_O:				m_SendText->GetChatText().push_back('O'); break;
	case DIK_P:				m_SendText->GetChatText().push_back('P'); break;
	case DIK_Q:				m_SendText->GetChatText().push_back('Q'); break;
	case DIK_R:				m_SendText->GetChatText().push_back('R'); break;
	case DIK_S:				m_SendText->GetChatText().push_back('S'); break;
	case DIK_T:				m_SendText->GetChatText().push_back('T'); break;
	case DIK_U:				m_SendText->GetChatText().push_back('U'); break;
	case DIK_V:				m_SendText->GetChatText().push_back('V'); break;
	case DIK_W:				m_SendText->GetChatText().push_back('W'); break;
	case DIK_X:				m_SendText->GetChatText().push_back('X'); break;
	case DIK_Y:				m_SendText->GetChatText().push_back('Y'); break;
	case DIK_Z:				m_SendText->GetChatText().push_back('Z'); break;
	case DIK_MINUS:			m_SendText->GetChatText().push_back('='); break;
	case DIK_CIRCUMFLEX:	m_SendText->GetChatText().push_back('~'); break;
	case DIK_YEN:			m_SendText->GetChatText().push_back('|'); break;
	case DIK_AT:			m_SendText->GetChatText().push_back('`'); break;
	case DIK_LBRACKET:		m_SendText->GetChatText().push_back('{'); break;
	case DIK_SEMICOLON:		m_SendText->GetChatText().push_back('+'); break;
	case DIK_COLON:			m_SendText->GetChatText().push_back('*'); break;
	case DIK_RBRACKET:		m_SendText->GetChatText().push_back('}'); break;
	case DIK_COMMA:			m_SendText->GetChatText().push_back('<'); break;
	case DIK_PERIOD:		m_SendText->GetChatText().push_back('>'); break;
	case DIK_SLASH:			m_SendText->GetChatText().push_back('?'); break;
	case DIK_BACKSLASH:		m_SendText->GetChatText().push_back('_'); break;
	case DIK_SPACE:			break;
	case DIK_NUMPAD0:		break;
	case DIK_NUMPAD1:		break;
	case DIK_NUMPAD2:		break;
	case DIK_NUMPAD3:		break;
	case DIK_NUMPAD4:		break;
	case DIK_NUMPAD5:		break;
	case DIK_NUMPAD6:		break;
	case DIK_NUMPAD7:		break;
	case DIK_NUMPAD8:		break;
	case DIK_NUMPAD9:		break;
	case DIK_DECIMAL:		break;
	case DIK_ADD:			break;
	case DIK_SUBTRACT:		break;
	case DIK_MULTIPLY:		break;
	case DIK_DIVIDE:		break;
	}
}