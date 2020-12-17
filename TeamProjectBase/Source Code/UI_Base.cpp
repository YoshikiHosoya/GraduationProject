//------------------------------------------------------------------------------
//
//UIのベース  [UI_Base.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "UI_Base.h"
#include "manager.h"
#include "renderer.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CUI_Base::CUI_Base()
{
	//初期化
	m_fChangeValue = 0.0f;
	m_nCnt = 0;
	m_nTotalTime = 0;

}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CUI_Base::~CUI_Base()
{

}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CUI_Base::Init()
{
	CScene2D::Init();

	//初期化
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CUI_Base::Update()
{
	m_nCnt++;

	m_fChangeValue;

	if (m_nCnt < m_nTotalTime)
	{
		switch (m_pattern)
		{
		case CUI_Base::RIGHT_TO_CENTER:
			GetPos().x -= m_fChangeValue;

			break;
		case CUI_Base::TRANSPARENT_TO_APPEAR:
			GetColor().a += m_fChangeValue;

			break;
		default:
			break;
		}
	}

	CScene2D::Update();

}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CUI_Base::Draw()
{

	CScene2D::Draw();

}


//------------------------------------------------------------------------------
//生成
//------------------------------------------------------------------------------
S_ptr<CUI_Base> CUI_Base::Create(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & size, D3DXCOLOR const & col,LPDIRECT3DTEXTURE9 const tex, CScene::OBJTYPE const objtype, APPEAR_PATTERN pattern, int nTotalTime)
{
	//メモリ確保
	S_ptr<CUI_Base> ptr = std::make_shared<CUI_Base>();

	//情報設定
	ptr->SetPos(pos);
	ptr->SetSize(size);
	ptr->SetColor(col);
	ptr->BindTexture(tex);

	//初期化処理
	ptr->Init();


	ptr->SetAppearPattern(pattern, nTotalTime);

	//Sceneにポインタを渡す
	ptr->SetObjType(objtype);
	ptr->AddSharedList(ptr);

	return ptr;
}

//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
void CUI_Base::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //_DEBUG
}

//------------------------------------------------------------------------------
//出現パターン設定
//------------------------------------------------------------------------------
void CUI_Base::SetAppearPattern(APPEAR_PATTERN pattern, int nTotalTime)
{
	m_pattern = pattern;
	m_nTotalTime = nTotalTime;
	float fDefaultValue, fAfterValue;


	switch (pattern)
	{
	case CUI_Base::RIGHT_TO_CENTER:

		fDefaultValue = GetPos().x;

		GetPos().x += GetSize().x;

		fAfterValue = GetPos().x;

		m_fChangeValue = fabsf(fAfterValue - fDefaultValue) / m_nTotalTime;



		break;
	case CUI_Base::TRANSPARENT_TO_APPEAR:
		GetColor().a = 0.0f;

		m_fChangeValue = 1.0f / m_nTotalTime;



		break;
	default:
		break;
	}

}
