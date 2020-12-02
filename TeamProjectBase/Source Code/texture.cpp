//-----------------------------------------------------------------------------
//
//テクスチャ管理  [texture.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "texture.h"
#include "manager.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
//静的メンバ変数の初期化
//-----------------------------------------------------------------------------
Vec<LPDIRECT3DTEXTURE9> CTexture::m_apTextureList = {};

//テクスチャ名の設定
FILENAME_LIST CTexture::m_aTexFileName =
{
	{ "data/TEXTURE/other/TexNone.png" },					//白紙
	{ "data/TEXTURE/Effect/particle.jpg" },					//パーティクル用の丸いの

	{ "data/TEXTURE/Material/field000.jpg" },					//マテリアル
	{ "data/TEXTURE/Material/field001.jpg" },					//マテリアル
	{ "data/TEXTURE/Material/field002.jpg" },					//マテリアル
	{ "data/TEXTURE/Material/field003.jpg" },					//マテリアル
	{ "data/TEXTURE/Material/field004.jpg" },					//マテリアル

	{ "data/TEXTURE/CHAT/chatTab_back.png" },					// チャットタブの背景
	{ "data/TEXTURE/CHAT/chatTab_title.png" },					// チャットタブのタイトル
	{ "data/TEXTURE/CHAT/chatTab_writeWindow.png" },			// チャットタブの書き込みウィンドウ
	{ "data/TEXTURE/CHAT/chatTab_open.png" },					// チャットタブ (開く)
	{ "data/TEXTURE/CHAT/chatTab_close.png" },					// チャットタブ (閉じる)
	{ "data/TEXTURE/CHAT/chatBox00.png" },						// チャットボックス00
	{ "data/TEXTURE/CHAT/chatBox01.png" },						// チャットボックス01
	{ "data/TEXTURE/CHAT/chatBox02.png" },						// チャットボックス02
	{ "data/TEXTURE/CHAT/chatBox03.png" },						// チャットボックス03

	{ "data/TEXTURE/CONNECTMODE/LoadIcon.png" },				// 接続中のロードアイコン

	{ "data/TEXTURE/UI/PressButton.png" },						//UI プレスボタン
	{ "data/TEXTURE/UI/Title.png" },							//UI タイトル
	{ "data/TEXTURE/UI/Result.png" },							//UI リザルト

	{ "data/TEXTURE/UI/Paint/Pen.png" },						// ブラシカーソル
	{ "data/TEXTURE/UI/Paint/Eraser.png" },						// 消しゴムカーソル
	{ "data/TEXTURE/UI/Paint/Pen_Button.png" },					// ブラシ
	{ "data/TEXTURE/UI/Paint/Eraser_Button.png" },				// 消しゴム
	{ "data/TEXTURE/UI/Paint/send.png" },						// 送信

	{ "data/TEXTURE/UI/DecodingBook/T00.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/T01.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/T02.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/T03.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/T04.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/T05.png" },					// 解読UI
	{ "data/TEXTURE/UI/DecodingBook/backLight.png" },			// 解読UIバックライト

	{ "data/TEXTURE/UI/DecodingBook/D00.png" },					// 解読書(Document)
	{ "data/TEXTURE/UI/DecodingBook/D01.png" },					// 解読書(Document)
	{ "data/TEXTURE/UI/DecodingBook/D02.png" },					// 解読書(Document)
	{ "data/TEXTURE/UI/DecodingBook/D03.png" },					// 解読書(Document)
	{ "data/TEXTURE/UI/DecodingBook/D04.png" },					// 解読書(Document)
	{ "data/TEXTURE/UI/DecodingBook/D05.png" },					// 解読書(Document)

	{ "data/TEXTURE/UI/DecodingBook/close.png" },				// 閉じるボタン
	{ "data/TEXTURE/UI/DecodingBook/ScrollBar.png" },			// スクロールバー
	{ "data/TEXTURE/UI/DecodingBook/ScrollHandle.png" },		// スクロールハンドル
};

//テクスチャ名の設定
FILENAME_LIST CTexture::m_aSeparateTexFileNameList =
{
	{ "data/TEXTURE/other/TexNone.png"},						//白紙

	{ "data/TEXTURE/other/Number.png" },						//ナンバー
	{ "data/TEXTURE/Effect/Explosion00.png" },					//爆発

	{ "data/TEXTURE/ModuleParts/Icon_Module00.png" },					//爆発
	{ "data/TEXTURE/ModuleParts/Icon_Module01.png" },					//爆発
	{ "data/TEXTURE/ModuleParts/Icon_Module03.png" },					//爆発

};


//テクスチャ名の設定
std::vector<CTexture::SEPARATE_TEX_INFO> CTexture::m_apSeparateTexInfoList =
{
	//テクスチャのポインタ、テクスチャのパス、テクスチャのタイプ、テクスチャの分割数、テクスチャのUVサイズ
	//ポインタとUVサイズに関してはLoad関数で読み込み＆計算する
	{ nullptr, CTexture::SEPARATE_TEX_NONE,								D3DXVECTOR2(1,1),	ZeroVector2 },		//白紙
	{ nullptr, CTexture::SEPARATE_TEX_EFFECT_EXPLOSION00,				D3DXVECTOR2(4,2),	ZeroVector2 },		//爆発
	{ nullptr, CTexture::SEPARATE_TEX_UI_NUMBER,						D3DXVECTOR2(10,1),	ZeroVector2 },		//爆発
	{ nullptr, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE00,				D3DXVECTOR2(12,2),	ZeroVector2 },		//爆発
	{ nullptr, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01,				D3DXVECTOR2(4,1),	ZeroVector2 },		//爆発
	{ nullptr, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE03,				D3DXVECTOR2(6,1),	ZeroVector2 },		//爆発

};

//-----------------------------------------------------------------------------
//テクスチャ一括ロード
//-----------------------------------------------------------------------------
void CTexture::TexLoad(HWND hwnd)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャ名のリストのサイズ分
	for (size_t nCnt = 0; nCnt < m_aTexFileName.size(); nCnt++)
	{
		//null
		pTexture = nullptr;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			m_aTexFileName[nCnt].data(),
			&pTexture);

		//debug
		std::cout << "Tex Load - " << nCnt << m_aTexFileName[nCnt].data() << NEWLINE;

		//nullcheck
		if (!pTexture)
		{
			//debug
			std::cout << "LOADFAILED!!! Tex - " << nCnt << m_aTexFileName[nCnt].data() << NEWLINE;

			//失敗
			MessageBox(hwnd, "テクスチャ読み込み失敗", m_aTexFileName[nCnt].data(), MB_OK | MB_ICONHAND);
		}

		//リストに追加
		m_apTextureList.emplace_back(pTexture);
	}

	//テクスチャ名のリストのサイズ分
	for (size_t nCnt = 0; nCnt < m_apSeparateTexInfoList.size(); nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			m_aSeparateTexFileNameList[nCnt].data(),
			&m_apSeparateTexInfoList[nCnt].m_apSeparateTexture);

		//debug
		std::cout << "SeparateTex Load - " << nCnt << m_aSeparateTexFileNameList[nCnt].data() << NEWLINE;

		//nullcheck
		if (!m_apSeparateTexInfoList[nCnt].m_apSeparateTexture)
		{
			//debug
			std::cout << "LOADFAILED!!! SeparateTex - " << nCnt << m_aSeparateTexFileNameList[nCnt].data() << NEWLINE;

			//失敗
			MessageBox(hwnd, "テクスチャ読み込み失敗", m_aSeparateTexFileNameList[nCnt].data(), MB_OK | MB_ICONHAND);
		}

		//どちらかの値が0だった場合は計算しない
		//0除算防止
		if (m_apSeparateTexInfoList[nCnt].m_UVCnt.x * m_apSeparateTexInfoList[nCnt].m_UVCnt.y)
		{
			//UVサイズ計算
			m_apSeparateTexInfoList[nCnt].m_UVSize = D3DXVECTOR2(1.0f / m_apSeparateTexInfoList[nCnt].m_UVCnt.x, 1.0f / m_apSeparateTexInfoList[nCnt].m_UVCnt.y);
		}
		else
		{
			m_apSeparateTexInfoList[nCnt].m_UVSize = ZeroVector2;
			//警告文
			MessageBox(hwnd, "0除算をしようとしました　値を確認してください", m_aSeparateTexFileNameList[nCnt].data(), MB_OK | MB_ICONHAND);
		}
	}
}

//-----------------------------------------------------------------------------
//テクスチャ一括破棄
//-----------------------------------------------------------------------------
void CTexture::TexUnload()
{
	//サイズ分
	for (size_t nCnt = 0; nCnt < m_apTextureList.size(); nCnt++)
	{
		//nullcheck
		if (m_apTextureList[nCnt])
		{
			//テクスチャの開放
			m_apTextureList[nCnt]->Release();
			m_apTextureList[nCnt] = nullptr;
		}
	}

	//サイズ分
	for (size_t nCnt = 0; nCnt < m_apSeparateTexInfoList.size(); nCnt++)
	{
		//nullcheck
		if (m_apSeparateTexInfoList[nCnt].m_apSeparateTexture)
		{
			//テクスチャの開放
			m_apSeparateTexInfoList[nCnt].m_apSeparateTexture->Release();
			m_apSeparateTexInfoList[nCnt].m_apSeparateTexture = nullptr;
		}
	}

	//配列の開放
	m_apTextureList.clear();
	m_apSeparateTexInfoList.clear();
}

//-----------------------------------------------------------------------------
//テクスチャ取得　通常
//----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEX_TYPE textype)
{
	//配列より小さい値か
	if (textype < (int)m_apTextureList.size())
	{
		//nullcheck
		if (m_apTextureList[textype])
		{
			return m_apTextureList[textype];
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
//テクスチャ取得　分割されてるやつ
//----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetSeparateTexture(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		//nullcheck
		if (m_apSeparateTexInfoList[textype].m_apSeparateTexture)
		{
			return m_apSeparateTexInfoList[textype].m_apSeparateTexture;
		}
	}
	return nullptr;
}
//-----------------------------------------------------------------------------
//テクスチャのUVサイズ取得
//----------------------------------------------------------------------------
D3DXVECTOR2 CTexture::GetSparateTex_UVSize(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	//配列外のモノを指定されていた場合はアクセス違反なるので注意
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		return m_apSeparateTexInfoList[textype].m_UVSize;
	}
	return ZeroVector2;
}

//-----------------------------------------------------------------------------
//テクスチャのUV枚数取得
//----------------------------------------------------------------------------
D3DXVECTOR2 CTexture::GetSparateTex_UVCnt(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	//配列外のモノを指定されていた場合はアクセス違反なるので注意
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		return m_apSeparateTexInfoList[textype].m_UVCnt;
	}
	return ZeroVector2;
}
