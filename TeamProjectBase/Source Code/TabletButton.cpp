//*************************************************************************************************************
//
// タブレットボタンの基底処理 [TabletButton_Base.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "TabletButton.h"
#include "modelinfo.h"
#include "scene3D.h"
#include "texture.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTabletButton::m_aTexture[CTabletButton::TYPE_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CTabletButton::CTabletButton()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CTabletButton::~CTabletButton()
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Load(void)
{
	m_aTexture[TYPE_PEN] = CTexture::GetTexture(CTexture::TEX_UI_BRUSHCURSOR);
	m_aTexture[TYPE_ERASER] = CTexture::GetTexture(CTexture::TEX_UI_ERASERCURSOR);
	m_aTexture[TYPE_SEND] = CTexture::GetTexture(CTexture::TEX_UI_SENDBUTTON);
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		m_aTexture[nCntTex] = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CTabletButton::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_TABLET_BUTTON));
	CSceneX::Init();
	
	// 3D画像の生成
	m_pImage = CScene3D::ScenePolygonCreateShared<CScene3D>(D3DXVECTOR3(0.0f, 0.0f, -4.8f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), MYLIB_D3DXCOR_SET,
		m_aTexture[m_Type], CScene::OBJTYPE_PICTURE_SYMBOL);
	m_pImage->SetParentMtxPtr(this->GetMtxWorldPtr());
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Update()
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Draw()
{
	CSceneX::Draw();
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CTabletButton> CTabletButton::Create(D3DXMATRIX *pMtxParent, CONST D3DXVECTOR3 &pos, TYPE type)
{
	std::shared_ptr<CTabletButton> pTabletButton_Base = std::make_shared<CTabletButton>();
	pTabletButton_Base->SetParentMtxPtr(pMtxParent);
	pTabletButton_Base->SetPos(pos);
	pTabletButton_Base->SetType(type);
	pTabletButton_Base->Init();

	//Sceneにポインタを渡す
	pTabletButton_Base->SetObjType(OBJTYPE_PICTURE);
	pTabletButton_Base->AddSharedList(pTabletButton_Base);

	return pTabletButton_Base;
}

