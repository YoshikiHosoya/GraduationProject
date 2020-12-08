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
#include "game.h"
#include "Decoding.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define TABLETBUTTON_NOTPRESS_POS_Z	(-8.0f)		// 押されていない時の位置のZ値
#define TABLETBUTTON_PRESS_POS_Z	(-1.0f)		// 押されている時の位置Z値

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTabletButton::m_aTexture[CTabletButton::TYPE_MAX]   = Mlf_array(NULL);	// テクスチャ情報
float              CTabletButton::m_aPressPos[CTabletButton::PRESS_MAX] = Mlf_array(0.0f);
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
// エラーの検出
//-------------------------------------------------------------------------------------------------------------
HRESULT CTabletButton::LoadError(void)
{
	return S_OK;
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
	// 変数宣言
	CMouse*       pMouse   = CManager::GetMouse();					// マウスの取得
	CPaintingPen* pPen     = CTabletButton::GetPen();				// ペンの取得
	CPicture*     pPicture = CTabletButton::GetPicture();			// ピクチャの取得

	if (pPen == nullptr || pPicture == nullptr)
	{
		return;
	}

	// フラグが立っている時かつマウスが押されていない時
	if (m_bPress && !pMouse->GetPress(0) && m_bChange)
	{
		// タイプ分岐処理
		switch (m_Type)
		{
			ML_CASE(TYPE_PEN)   pPen->SetMode(CPaintingPen::MODE_BRUSH);	// ペン
			ML_CASE(TYPE_ERASER)pPen->SetMode(CPaintingPen::MODE_ERASER);	// 消しゴム
			ML_CASE(TYPE_SEND)  pPicture->Writing();						// 送信
			ML_CASEEND;														// ケース終了
		}
		// 押されたフラグを消す
		m_bPress = false;
	}

	// タイプとモードが一致していた時
	if (pPen->GetMode() == m_Type)
	{
		// 位置の取得
		D3DXVECTOR3 *pos = GetPosPtr();
		// Z値を変更
		pos->z = TABLETBUTTON_PRESS_POS_Z;
	}
	else
	{
		// 位置の取得
		D3DXVECTOR3 *pos = GetPosPtr();
		// Z値を変更
		pos->z = (m_bPress == true) ? m_aPressPos[CTabletButton::PRESSED] : m_aPressPos[CTabletButton::UNPRESSED];
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
// 文字列から設定する
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::SetFromString(CONST_STRING str)
{
	// 変数宣言
	float fData = MYLIB_FLOAT_UNSET;
	// 押した時の位置
	if (sscanf(str, "PressedPos = %f", &fData) == 1)
	{
		m_aPressPos[CTabletButton::PRESSED] = fData;
	}
	//押していない時の位置
	else if (sscanf(str, "UnPressedPos = %f", &fData) == 1)
	{
		m_aPressPos[CTabletButton::UNPRESSED] = fData;
	}
}

//-------------------------------------------------------------------------------------------------------------
// モードの設定
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::FlagProcToChangeMode(void)
{
	// 変数宣言
	CMouse *pMouse = CManager::GetMouse();// マウスの取得

	// マウスが押されていない時
	if (!pMouse->GetPress(0))
	{// 変更フラグを立てる
		SetToSetChangeFlag();
		// 内部で押されたフラグを消す
		SetToOffInPressFlag();
	}
	else
	{
	}

	// 押されているフラグが立っている時又はマウスが押されていない時
	if (m_bPress || !pMouse->GetPress(0) || !m_bChange)
	{// 処理を抜ける
		return;
	}
	// 内部で押されたフラグを立てる
	SetToSetInPressFlag();
	// フラグを立てる
	m_bPress = true;
}

//-------------------------------------------------------------------------------------------------------------
// 変更フラグの設定
//-------------------------------------------------------------------------------------------------------------
void CTabletButton::SetChangeFlagProc(void)
{
	// マウスが押されている時かつ内部で押されたフラグが立っていない時
	if(CManager::GetMouse()->GetPress(0) && !m_bInPress)
	{// 変更フラグを消す
		SetToOffChangeFlag();
		return;
	}
	if (!CManager::GetMouse()->GetPress(0))
	{
		// 内部で押されたフラグを消す
		SetToOffInPressFlag();
	}

	// 変更フラグを立てる
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

//-------------------------------------------------------------------------------------------------------------
// ピクチャの取得
//-------------------------------------------------------------------------------------------------------------
CPicture * CTabletButton::GetPicture(void)
{
	// 変数宣言
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CDecoding *pDecoding = CManager::GetDecoding();	// 解読の取得
	if (pGame != nullptr)
	{
		// ピクチャの取得
		return pGame->GetPicture();
	}
	else if (pDecoding != nullptr)
	{
		// ピクチャの取得
		return pDecoding->GetPicture();
	}
	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// ペンの取得
//-------------------------------------------------------------------------------------------------------------
CPaintingPen * CTabletButton::GetPen(void)
{
	// 変数宣言
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CDecoding *pDecoding = CManager::GetDecoding();	// 解読の取得
	if (pGame != nullptr)
	{
		// ペンの取得
		return pGame->GetPicture()->GetPaintPen();
	}
	else if (pDecoding != nullptr)
	{
		// ペンの取得
		return pDecoding->GetPicture()->GetPaintPen();
	}
	return nullptr;
}

