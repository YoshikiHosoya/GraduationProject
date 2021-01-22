// ===================================================================
//
// �`���b�g�̃^�u���� [ chatTab.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHATTAB_H_
#define _CHATTAB_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include "chatBase.h"

// ===================================================================
// �}�N����`
// ===================================================================

// ===================================================================
// �N���X��`
// ===================================================================
class CPolygon2D;
class CChatText;
class CChatBase;

class CChatTab
{
public:
	typedef enum
	{	// ��ɕ\������`���b�g�^�u�̎��
		POLY_BACK,			// �w�i
		POLY_TITLE,			// �^�C�g��
		POLY_WRITEWINDOW,	// �������݃E�B���h�E
		POLY_TAB,			// �J�^�u
		//POLY_TABLET,		// �^�u���b�g�̊J�^�u
		POLY_MAX
	} POLYTYPE;

	typedef enum
	{
		KEEPTYPE_TEXT,
		KEEPTYPE_PICTURE,
		KEEPTYPE_MAX
	} KEEPTYPE;

	typedef struct
	{	// �`���b�g�L�[�v�̏��
		CChatText *pKeepText;	// �e�L�X�g
		CPolygon2D *pPolyPic;	// �|���S�� (�s�N�`��)
		CPolygon2D *pPolyBack;	// �|���S�� (�w�i)
		CChatBase::TEXTOWNER owner;	// ������
		KEEPTYPE type;			// �L�[�v�̎��
	} CHATKEEP;

	typedef enum
	{	// �`���b�g�^�u�̃e�N�X�`���^�C�v
		CHATTAB_BACK,	// �w�i
		CHATTAB_OPEN,	// �J��
		CHATTAB_CLOSE,	// ����
		CHATTAB_MAX
	} CHATTAB_TEXTYPE;

	typedef enum
	{	// �^�u�̏��
		TABSTATE_NONE,		// �����Ȃ�
		TABSTATE_CLOSING,	// ������
		TABSTATE_CLOSED,	// �������
		TABSTATE_OPENNING,	// �J�����
		TABSTATE_OPENED,	// �J�������
		TABSTATE_MAX
	} TABSTATE;

	CChatTab();
	~CChatTab();

	HRESULT Init(void);			// ������
	void Update(void);			// �X�V
	void Draw(void);			// �`��
	void ShowDebugInfo() {};		//�f�o�b�O���\�L

	static CChatTab * Create(void);	// ����

	static D3DXVECTOR2	&GetTabPos(void)			{ return m_TabPos; }						// �^�u���W�̎擾
	static TABSTATE		&GetTabState(void)			{ return m_tabState; }						// �^�u�̏�Ԃ̎擾
	static TABSTATE		&GetTabletState(void)		{ return m_tabletState; }					// �^�u���b�g�̏�Ԏ擾
	static bool			&GetTabClick(void)			{ return m_bClickTab; }						// �^�u�N���b�N�̃t���O���擾
	static CHATKEEP		*GetChatKeep(int nIndex)	{ return &m_chatKeep[nIndex]; }				// �e�L�X�g�̔w�i�|���S���擾
	static CChatText	*GetSendText(void)			{ return m_SendText; }						// �e�L�X�g�̔w�i�|���S���擾
	static void			SetTabPos(D3DXVECTOR2 &pos)	{ m_TabPos = pos; }							// �^�u���W�̐ݒ�
	static void			SendChatText(void);														// �e�L�X�g�̑��M
	static void			ScrollUp(void);															// �`���b�g�����̏�X�N���[��
	static void			ScrollDown(void);														// �`���b�g�����̉��X�N���[��
	static void			AddRecvKeep(CChatBase::TEXTOWNER owner, char *cText);					// ��M�����L�[�v�̒ǉ�
	static void			AddRecvPicture(CChatBase::TEXTOWNER owner, LPDIRECT3DTEXTURE9 pTexture);	// �s�N�`���̒ǉ�

	CPolygon2D *GetChatPly(POLYTYPE polygon) { m_pChatPoly[polygon]; };
	bool CheckMouseHit(POLYTYPE polygon);

private:
	void		CreateChatKeep(void);
	void		CreateChatPic(void);
	void		ClickTab(void);											// �^�u�N���b�N
	void		ClickTabletTab(void);									// �^�u�N���b�N
	void		SlideTab(void);											// �^�u�X���C�h
	void		InputText(void);										// �L�[���͂̑���
	void		PressKey(int nKeyID, bool bShift);						// �L�[����
	void		SetChatKeyInfo(int nKeyID);								// �ʏ�̓���
	void		SetChatShiftKeyInfo(int nKeyID);						// �V�t�g�L�[��p��������

	static std::vector<CHATKEEP>	m_chatKeep;							// �ێ��ł���e�L�X�g
	static std::vector<CHATKEEP>	m_recvKeep;							// ��M�����e�L�X�g

	static TABSTATE					m_tabState;							// �^�u�̏��
	static D3DXVECTOR2				m_TabPos;							// �^�u�̐e���W
	static float					m_fScrollPosY;						// �}�E�X�X�N���[���̍��W
	static int						m_nCntPress;						// �������̃J�E���^
	static int						m_nPressKey;						// ���͂��ꂽ�L�[���i�[����
	static CChatText				*m_leftText;						// �c�莞����\������e�L�X�g
	static CChatText				*m_SendText;						// ����e�L�X�g
	CPolygon2D						*m_pChatPoly[POLY_MAX];				// �`���b�g�p�摜�̃|���S��
	int								m_nCntState;						// ��ԊǗ��̃J�E���^
	D3DXVECTOR2						m_moveDest;							// �^�u�ړ��̗�
	static TABSTATE					m_tabletState;						// �^�u���b�g�̏��
	static bool						m_bClickTab;						// �^�u�N���b�N�̃t���O
};

#endif
