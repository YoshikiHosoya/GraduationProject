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
#include "picture.h"
#include "PaintingPen.h"
#include "manager.h"
#include "mouse.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define TABLETBUTTON_NOTPRESS_POS_Z	(-8.0f)		// 押されていない時の位置のZ値
#define TABLETBUTTON_PRESS_POS_Z	(-1.0f)		// 押されている時の位置Z値

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
	m_bPress = false;
	m_bChange = false;
	m_bInPress = false;
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Update()
{
	// マウスの取得a
	CMouse *pMouse = CManager::GetMouse();
	// ペンの取得
	CPaintingPen *pPen = CPicture::GetPaintPen();
	// フラグが立っている時かつマウスが押されていない時
	if (m_bPress && !pMouse->GetPress(0) && m_bChange)
	{
		m_bPress = false;
		// タイプ分岐処理
		switch (m_Type)
		{
			MLB_CASE(TYPE_PEN)   pPen->SetMode(CPaintingPen::MODE_BRUSH);	// ペン
			MLB_CASE(TYPE_ERASER)pPen->SetMode(CPaintingPen::MODE_ERASER);	// 消しゴム
			MLB_CASE(TYPE_SEND);											// 送信
			MLB_CASEEND;													// ケース終了
		}
	}

	if (pPen->GetMode() == m_Type)
	{
		// 位置の取得
		D3DXVECTOR3 *pos = GetPosPtr();
		// z値を変更
		pos->z = TABLETBUTTON_PRESS_POS_Z;
	}
	else
	{
		// 位置の取得
		D3DXVECTOR3 *pos = GetPosPtr();
		// z値を変更
		pos->z = (m_bPress == true) ? TABLETBUTTON_PRESS_POS_Z : TABLETBUTTON_NOTPRESS_POS_Z;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::Draw()
{
	CSceneX::Draw();
}

//-------------------------------------------------------------------------------------------------------------
// モードの設定
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::SetMode(void)
{
	// マウスの取得
	CMouse *pMouse = CManager::GetMouse();

	// マウスが押されていない時
	if (!pMouse->GetPress(0))
	{
		SetToSetChangeFlag();
		m_bInPress = false;
	}
	else
	{
		m_bInPress = true;
	}
	// 押されているフラグが立っている時又はマウスが押されていない時
	if (m_bPress || !pMouse->GetPress(0) || !m_bChange)
	{
		// 処理を抜ける
		return;
	}

	// フラグを立てる
	m_bPress = true;
}

//-------------------------------------------------------------------------------------------------------------
// 変更フラグの設定
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::SetChangeFlag(void)
{
	if(CManager::GetMouse()->GetPress(0) && !m_bInPress)
	{
		SetToOffChangeFlag();
		return;
	}
	SetToSetChangeFlag();
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

