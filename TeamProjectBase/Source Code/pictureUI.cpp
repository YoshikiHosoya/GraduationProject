//*************************************************************************************************************
//
// 絵のUI処理 [pictureUI.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "pictureUI.h"
#include "scene2D.h"
#include "texture.h"
#include "manager.h"
#include "mouse.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CPictureUI::CREATEINFO CPictureUI::m_aCreateInfo[CPictureUI::TYPE_MAX] = {
	{ D3DXVECTOR3(650.0f,150.0f,0.0f),D3DXVECTOR3(120.0f,120.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.5f),nullptr,CScene::OBJTYPE_UI },
	{ D3DXVECTOR3(650.0f,360.0f,0.0f),D3DXVECTOR3(120.0f,120.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.5f),nullptr,CScene::OBJTYPE_UI },
};

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CPictureUI::CPictureUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CPictureUI::~CPictureUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CPictureUI> CPictureUI::Create(void)
{
	std::shared_ptr<CPictureUI> pPictureUi = std::make_shared<CPictureUI>();
	pPictureUi->Init();
	//Sceneで管理
	pPictureUi->SetObjType(OBJTYPE_UI);
	pPictureUi->AddSharedList(pPictureUi);
	return pPictureUi;
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void CPictureUI::Load(void)
{
	m_aCreateInfo[CPictureUI::TYPE_BRUSH].tex = CTexture::GetTexture(CTexture::TEX_UI_BRUSHBUTTON);
	m_aCreateInfo[CPictureUI::TYPE_ERASER].tex = CTexture::GetTexture(CTexture::TEX_UI_ERASERBUTTON);
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CPictureUI::Init()
{
	// ポインタの初期化
	CREATEINFO *pInfo = &m_aCreateInfo[0];

	for (int nCtntType = 0; nCtntType < CPictureUI::TYPE_MAX; nCtntType++, pInfo++)
	{
		m_pScene2D[nCtntType] = std::make_shared<CScene2D>();
		//情報設定
		m_pScene2D[nCtntType]->SetPos(pInfo->pos);
		m_pScene2D[nCtntType]->SetSize(pInfo->size);
		m_pScene2D[nCtntType]->SetColor(pInfo->col);
		m_pScene2D[nCtntType]->SetObjType(pInfo->objtype);
		m_pScene2D[nCtntType]->BindTexture(pInfo->tex);

		//初期化処理
		m_pScene2D[nCtntType]->Init();

		//Sceneにポインタを渡す
		m_pScene2D[nCtntType]->SetObjType(pInfo->objtype);
		m_pScene2D[nCtntType]->AddSharedList(m_pScene2D[nCtntType]);
		m_bIsOn[nCtntType] = false;
	}
	m_PressedType = TYPE_BRUSH;
	return E_NOTIMPL;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CPictureUI::Update()
{
	for (int nCtntType = 0; nCtntType < CPictureUI::TYPE_MAX; nCtntType++)
	{
		if (Collision(m_pScene2D[nCtntType].get(), nCtntType))
		{
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPictureUI::Draw()
{
}

//-------------------------------------------------------------------------------------------------------------
// デバッグ情報表記
//-------------------------------------------------------------------------------------------------------------
void CPictureUI::ShowDebugInfo()
{
}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CPictureUI::Collision(CScene2D *pScene2D, int& nCntType)
{
	CMouse *pMouse = CManager::GetMouse();
	D3DXVECTOR2 MousePos = D3DXVECTOR2((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY());
	D3DXVECTOR3 &pos = pScene2D->GetPos();
	D3DXVECTOR3 &size = pScene2D->GetSize() * 0.5f;

	if (pos.x + size.x >= MousePos.x && MousePos.x >= pos.x - size.x  &&
		pos.y + size.y >= MousePos.y && MousePos.y >= pos.y - size.y)
	{
		if (pMouse->GetPress(0))
		{
			if (m_bIsOn[nCntType])
			{
				pScene2D->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				m_PressedType = (TYPE)nCntType;
				return true;
			}
			pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			return true;
		}
		pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_bIsOn[nCntType] = true;
		return true;
	}
	else
	{
		m_bIsOn[nCntType] = false;
		pScene2D->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));
		return false;
	}
}
