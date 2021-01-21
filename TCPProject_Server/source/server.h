// ===================================================================
//
// �T�[�o�[���� [ server.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"

// ===================================================================
// �}�N����`
// ===================================================================

// ===================================================================
// �N���X��`
// ===================================================================
class CServer
{
public:
	typedef enum
	{
		ORDER_NONE = 0,			// �w���Ȃ�
		ORDER_GUEST_BE_ACCEPT,	// �ڑ�����

		ORDER_SENDTEXT,			// �e�L�X�g���M
		ORDER_SENDPICTURE,		// �s�N�`�����M

		ORDER_SENDSELECT,		// �I�𑗐M
		ORDER_SENDWAIT,			// �҂���Ԃ̑��M

		ORDER_GAMEOVER,			// �Q�[���I�[�o�[
		ORDER_GAMECLEAR,		// �Q�[���N���A
		ORDER_END_GAME,			// �Q�[���I��
		ORDER_RETRY,			// ���g���C
		ORDER_CLOSED_SOCKET,	// �ڑ��I��

		ORDER_MAX
	} CLIENT_ORDER;

	CServer() {};	// �R���X�g���N�^
	~CServer() {};		// �f�X�g���N�^

	static int main(void);						// ���C������
	static HRESULT InitServer(void);			// ������
	static void InitErrorReport(int err);		// �G���[���|�[�g�̕\��
	static void SocketErrorReport(int err);		// �G���[���|�[�g�̕\��
	static void UninitServer(void);				// �����I��

	static void Update(void);

	static void SetClose(bool bClose) { m_bCloseServer = bClose; }

private:
	static void Recv(int nPlayer);
	static bool CountPacket(char *order, int nLen, int *nPacket);
	static void Order(int nPlayer, char* data);
	static void ReSendText(int nPlayer, char* data);
	static void ReSendPicture(int nPlayer, char* data);
	static void ReSendCommand(int nPlayer, char* data);
	static void ReSendGameEnd(int nPlayer, char* data);
	static void ReSendResult(int nPlayer, char * data);
	static void ClosePlayerSocket(int nPlayer);
	static bool m_bCloseServer;					// �T�[�o�[�̏I���t���O
	static bool m_bCloseSocket;					// �\�P�b�g�̏I���t���O
	static SOCKET m_socket;						// �����̃\�P�b�g
	static std::vector<SOCKET> m_PlayerSocket;	// 2�l�̃\�P�b�g���i�[����ϐ�

	static bool m_bAccept[2];
};

#endif
