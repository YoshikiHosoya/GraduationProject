// ===================================================================
//
// �T�[�o���� [ main.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "server.h"
#include <string.h>
#include <errno.h>

// ===================================================================
// �}�N����`
// ===================================================================
#define PORT_SERVER			(12345)			// �T�[�o�̃|�[�g�ԍ�
#define MAX_BACKLOG			(2)				// �ڑ��v���ł���ő�̐�
#define VERSION_WINSOCK		(2)				// winsock�̃o�[�W����

#define NUM_CMPCHAR			(6)				// ��r���镶����

#define LINK_SENDTEXT		("data/SendText.txt")					// ���M�p�e�L�X�g�̃p�X
#define LINK_SENDPICTURE	("data/SendPicture.txt")				// ���M�p�s�N�`���̃p�X

// ===================================================================
// �ÓI�����o�ϐ��̏�����
// ===================================================================
SOCKET CServer::m_socket = NULL;
std::vector<SOCKET> CServer::m_PlayerSocket = {};
bool CServer::m_bCloseServer = false;
bool CServer::m_bCloseSocket = false;
bool CServer::m_bAccept[2] = {};

// ===================================================================
// ���C���֐�
// ===================================================================
int CServer::main()
{
	printf("�T�[�o�[���J�n\n");
	
	while (!m_bCloseServer)
	{
		// winsock�̏�����
		if (InitServer() == E_FAIL)
		{
			// �G���[�ɂ�鋭���I��
			return 0;
		}

		struct sockaddr_in client;
		int len;

		// TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
		listen(m_socket, MAX_BACKLOG);

		// TCP�N���C�A���g����̐ڑ��v�����󂯕t����
		len = sizeof(client);

		SOCKET sock[2];

		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// �ڑ��m�F
			sock[nCnt] = accept(m_socket, (struct sockaddr *)&client, &len);

			// �G���[
			if (sock[nCnt] == -1)
			{
				// �G���[���|�[�g�\��
				SocketErrorReport(errno);
				return 0;
			}

			// �ڑ������v���C���[��\��
			printf("player[%d] �Ɛڑ�����\n", nCnt);
			// �\�P�b�g��ݒ�
			m_PlayerSocket.push_back(sock[nCnt]);
			m_bAccept[nCnt] = true;
		}

		for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
		{
			// �Q�X�g�Ƃ̐ڑ�����
			char cOrder[1];
			cOrder[0] = ORDER_GUEST_BE_ACCEPT;
			// ���M
			send(m_PlayerSocket[nPlayer], cOrder, 1, 0);
		}

		if (!m_bCloseSocket)
		{
			// �ʃX���b�h�ŏ���
			std::thread thread(CServer::Update);
			thread.detach();

			// �T�[�o�[�I���܂Ń��[�v
			while (!m_bCloseSocket) {}
		}

		// �T�[�o�[�̏I��
		UninitServer();
	}

	return 0;
}

// ===================================================================
// �T�[�o�[�̏�����
// ===================================================================
HRESULT CServer::InitServer(void)
{
	WSADATA wsaData;
	int err;
	m_bCloseSocket = false;
	m_bCloseServer = false;
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_bAccept[nCnt] = false;
	}

	// �T�[�o�E�N���C�A���g���ʂŊJ�n���̂݁A�K���s��
	err = WSAStartup(MAKEWORD(VERSION_WINSOCK, 0), &wsaData);

	// �G���[����
	if (err != 0)
	{
		// �R���\�[���ɕ\��
		printf("�T�[�o�[�̏��������s\n");
		// �G���[���b�Z�[�W
		InitErrorReport(err);
		// winsock2�̏I������
		WSACleanup();
		// ���s
		return E_FAIL;
	}

	// �R���\�[���ɕ\��
	printf("�T�[�o�[�̏���������\n");

	struct sockaddr_in addr;	// �\�P�b�g�Ɋi�[���邽�߂̃A�h���X

	// �\�P�b�g�̍쐬
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	// �ڑ��҂�����IP�A�h���X�ƃ|�[�g���\�P�b�g�ɐݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_SERVER);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	// �\�P�b�g���o�C���h
	bind(m_socket, (struct sockaddr *)&addr, sizeof(addr));

	// ����
	return S_OK;
}

// ===================================================================
// �G���[���b�Z�[�W
// ===================================================================
void CServer::InitErrorReport(int err)
{
	printf("�I�I�I�G���[�����I�I�I\n");
	printf("ErrorCode : %d\n", WSAGetLastError());

	switch (err)
	{
		// �l�b�g���[�N�T�u�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��Ȃ�
	case WSASYSNOTREADY:
		printf("�l�b�g���[�N�T�u�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��܂���B\n");
		break;
		// �v�����ꂽwinsock�̃o�[�W�������T�|�[�g����Ă��Ȃ�
	case WSAVERNOTSUPPORTED:
		printf("�v�����ꂽwinsock�̃o�[�W�������T�|�[�g����Ă��܂���B\n");
		break;
		// �u���b�L���O����̎��s���ł��邩�A �܂��̓T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������Ă���
	case WSAEINPROGRESS:
		printf("�u���b�L���O����̎��s���ł��邩�A �܂��̓T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������Ă��܂��B\n");
		break;
		// winsock�������ł���ő�v���Z�X���ɒB����
	case WSAEPROCLIM:
		printf("winsock�������ł���ő�v���Z�X���ɒB���܂����B\n");
		break;
		// �������ł���lpWSAData �͗L���ȃ|�C���^�ł͂Ȃ�
	case WSAEFAULT:
		printf("�������ł��� lpWSAData �͗L���ȃ|�C���^�ł͂���܂���B\n");
		break;
	}

	// �L�[���͑҂�
	printf("�I�����܂��BEnter�L�[����͂��Ă��������B\n");
	getchar();
}

// ===================================================================
// �G���[���b�Z�[�W
// ===================================================================
void CServer::SocketErrorReport(int err)
{
	printf("�I�I�I�G���[�����I�I�I\n");
	printf("ErrorCode : %d\n", err);

	switch (err)
	{
		// �l�b�g���[�N���s��
	case ENETDOWN:
		printf("�l�b�g���[�N���s�ʂɂȂ��Ă��܂��B\n");
		break;
		// �v���g�R���G���[
	case EPROTO:
		printf("�v���g�R���G���[���������܂���\n");
		break;
		// �w�肳�ꂽ�v���g�R�����g�p�ł��Ȃ�
	case ENOPROTOOPT:
		printf("�w�肳�ꂽ�v���g�R�����g�p�ł��܂���ł����B\n");
		break;
		// �w�肳�ꂽ�\�P�b�g�ł̓T�|�[�g����Ă��Ȃ�����
	case EOPNOTSUPP:
		printf("���̑���͎w�肳�ꂽ�\�P�b�g�ł̓T�|�[�g����Ă��܂���B\n");
		break;
		// �z�X�g�ɓ��B�ł��Ă��Ȃ�
	case EHOSTUNREACH:
		printf("�z�X�g�ɓ��B�ł��Ă��܂���B\n");
		break;
	}
}

// ===================================================================
// �T�[�o�[�̋����I��
// ===================================================================
void CServer::UninitServer(void)
{
	for (int nCnt = 0; nCnt < (int)m_PlayerSocket.size(); nCnt++)
	{
		// �\�P�b�g�̏I��
		if (m_PlayerSocket[nCnt])
		{
			closesocket(m_PlayerSocket[nCnt]);
		}
	}

	// �������̃N���A
	m_PlayerSocket.clear();	

	printf("�T�[�o�[���N���[���A�b�v\n");
	// winsock2�̏I������
	WSACleanup();
}

// ===================================================================
// �X�V
// ===================================================================
void CServer::Update(void)
{
	fd_set fds, readfds;
	// fd_set�̏�����
	FD_ZERO(&readfds);
	// select�ő҂ǂݍ��݃\�P�b�g�Ƃ��� playerSocket ��o�^
	for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
	{
		FD_SET(m_PlayerSocket[nPlayer], &readfds);
	}

	// �T�[�o�[�̏I���܂Ń��[�v
	while (!m_bCloseSocket)
	{
		// �ǂݍ��ݗpfd_set�̏����� 
		// (select�Ŗ���㏑�������̂ŁA����������񏉊�������)
		memcpy(&fds, &readfds, sizeof(fd_set));

		// fds�ɐݒ肳�ꂽ�\�P�b�g���ǂݍ��݉\�ɂȂ�܂ő҂�
		select(0, &fds, NULL, NULL, NULL);

		for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
		{
			if (!m_bAccept[nPlayer])
				continue;

			// �\�P�b�g��ǂݍ��߂Ȃ���΁A�������Ȃ�
			if (!FD_ISSET(m_PlayerSocket[nPlayer], &fds))
				continue;

			// ��M����
			Recv(nPlayer);
		}

		if (!m_bAccept[0] && !m_bAccept[1])
			m_bCloseSocket = true;
	}
}

// ===================================================================
// ��M����
// ===================================================================
void CServer::Recv(int nPlayer)
{
	int nPacket = 0;
	int nLen = 0;
	// ��M�p
	char cData[16600];
	memset(cData, 0, sizeof(cData));

	// ���[�v
	while (1)
	{
		// �\�P�b�g�j����͏������Ȃ�
		if (m_PlayerSocket.size() <= 0)
			return;

		// ��M
		nLen = recv(m_PlayerSocket[nPlayer], &cData[nPacket], 16600, 0);
		// ��M���Ă��Ȃ�
		if (nLen <= 0)
			return;

		// �p�P�b�g�S��M�ŃJ�E���g�I��
		if (CountPacket(&cData[0], nLen, &nPacket))
			break;
	}

	// �w�����̏���
	Order(nPlayer, cData);

	// �p�P�b�g���Z�b�g
	nPacket = 0;
}

// ===================================================================
// �p�P�b�g�̃J�E���g
// ===================================================================
bool CServer::CountPacket(char *order, int nLen, int * nPacket)
{
	// �e�L�X�g�E�s�N�`���̓p�P�b�g�𓝍�����
	if (order[0] == ORDER_SENDTEXT || order[0] == ORDER_SENDPICTURE)
	{
		// ��M�����p�P���𑍃p�P���ɉ��Z
		if (*nPacket < *(int*)&order[1])
		{
			*nPacket += nLen;
		}

		// ���݂̎�M��
		if (order[0] == ORDER_SENDTEXT)
			printf("%dbyte��M ( %d / %dbyte )\n", nLen, *nPacket, *(int*)&order[1] + 5);
		else
			printf("%dbyte��M ( %d / %dbyte )\n", nLen, *nPacket, *(int*)&order[1] + 6);

		// ���p�P�����ڕW�ɒB����
		if (*nPacket >= *(int*)&order[1])
		{
			return true;
		}
		return false;
	}
	// ���̎w���͑�����
	else
	{
		*nPacket += nLen;
		// ���݂̎�M��
		printf("%dbyte��M ( %d / %dbyte )\n", nLen, *nPacket, strlen(order));
		// ���݂̊m�F
		if (nLen >= 1)
		{
			return true;
		}
		return false;
	}
}

// ===================================================================
// �w�����̏���
// ===================================================================
void CServer::Order(int nPlayer, char * data)
{
	// �w������
	switch (data[0])
	{
	case ORDER_SENDPICTURE:		ReSendPicture(nPlayer, data); break;
	case ORDER_SENDTEXT:		ReSendText(nPlayer, data); break;
	case ORDER_SENDSELECT:		ReSendCommand(nPlayer, data); break;
	case ORDER_SENDWAIT:		ReSendCommand(nPlayer, data); break;
	case ORDER_CLOSED_SOCKET:	ClosePlayerSocket(nPlayer); break;
	case ORDER_END_GAME:		ReSendResult(nPlayer, data); break;
	case ORDER_GAMEOVER:		ReSendGameEnd(nPlayer, data); break;
	case ORDER_GAMECLEAR:		ReSendGameEnd(nPlayer, data); break;
	case ORDER_RETRY:			ReSendResult(nPlayer, data);  break;
	}
}

// ===================================================================
// �s�N�`���̎�M
// ===================================================================
void CServer::ReSendPicture(int nPlayer, char* data)
{
	// �T�C�Y
	int *pSize = (int*)&data[1];
	// �s�N�`���̃f�[�^���i�[
	char *cPicData = new char[*pSize + 1];
	memcpy(cPicData, &data[5], *pSize);

	FILE *fp;

	// �t�@�C�����J��
	if ((fp = fopen(LINK_SENDPICTURE, "wb")) == NULL)
	{
		// �G���[
		printf("�e�L�X�g�ǂݍ��݃G���[\n");
		delete[] cPicData;
		return;
	}

	fwrite(cPicData, *pSize, 1, fp);

	// �t�@�C�������
	fclose(fp);

	if (nPlayer == 0)
	{
		printf("player[0] > player[1] �փs�N�`�����M\n");
		send(m_PlayerSocket[1], data, *pSize + 5 + 1, 0);
	}
	else
	{
		printf("player[1] > player[0] �փs�N�`�����M\n");
		send(m_PlayerSocket[0], data, *pSize + 5 + 1, 0);
	}

	delete[] cPicData;
}

// ===================================================================
// �e�L�X�g�̎�M
// ===================================================================
void CServer::ReSendText(int nPlayer, char* data)
{
	// �T�C�Y
	int *pSize = (int*)&data[1];

	char *cTextData = new char[*pSize];
	memcpy(cTextData, &data[5], *pSize);

	FILE *fp;

	// �t�@�C�����J��
	if ((fp = fopen(LINK_SENDTEXT, "wb")) == NULL)
	{
		// �G���[
		printf("�e�L�X�g�ǂݍ��݃G���[\n");
		delete[] cTextData;
		return;
	}

	fwrite(cTextData, *pSize, 1, fp);

	// �t�@�C�������
	fclose(fp);

	if (nPlayer == 0)
	{
		printf("player[0] > player[1] �փe�L�X�g���M\n");
		send(m_PlayerSocket[1], data, *pSize + 5, 0);
	}
	else
	{
		printf("player[1] > player[0] �փe�L�X�g���M\n");
		send(m_PlayerSocket[0], data, *pSize + 5, 0);
	}

	delete[] cTextData;
}

// ===================================================================
// �I���̎�M
// ===================================================================
void CServer::ReSendCommand(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] �փI�[�_�[���M\n");
		send(m_PlayerSocket[1], data, 2, 0);
	}
	else
	{
		printf("player[1] > player[0] �փI�[�_�[���M\n");
		send(m_PlayerSocket[0], data, 2, 0);
	}
}

// ===================================================================
// �Q�[�������̎�M
// ===================================================================
void CServer::ReSendGameEnd(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] �փQ�[�������𑗐M\n");
		send(m_PlayerSocket[1], data, 5, 0);
	}
	else
	{
		printf("player[1] > player[0] �փQ�[�������𑗐M\n");
		send(m_PlayerSocket[0], data, 5, 0);
	}
}

// ===================================================================
// �Q�[���̏I��
// ===================================================================
void CServer::ReSendResult(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] �փ��U���g�̑I���𑗐M\n");
		send(m_PlayerSocket[1], data, 2, 0);
	}
	else
	{
		printf("player[1] > player[0] �փ��U���g�̑I���𑗐M\n");
		send(m_PlayerSocket[0], data, 2, 0);
	}
}

// ===================================================================
// �\�P�b�g�̏I��
// ===================================================================
void CServer::ClosePlayerSocket(int nPlayer)
{
	printf("player[%d] �̃\�P�b�g���I��\n", nPlayer);
	// �v���C���[�\�P�b�g��j��
	if (m_PlayerSocket[nPlayer])
		closesocket(m_PlayerSocket[nPlayer]);
	m_bAccept[nPlayer] = false;
}
