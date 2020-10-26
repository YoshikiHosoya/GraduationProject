//------------------------------------------------------------------------------
//
//複数桁の数値の処理  [multinumber.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "multinumber.h"
#include "scene2D.h"
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DISAPPEAR_COUNT			(60)
#define DISAPPEAR_SCALE_VALUE	(1.0f)
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CMultiNumber::CMultiNumber()
{
	//初期化
	m_pNumberList.clear();
	m_nCnt = 0;
	m_nValue = 0;
	m_NumberEffect = NUMBER_EFFECT::NONE;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CMultiNumber::~CMultiNumber()
{
	m_pNumberList.clear();
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CMultiNumber::Init()
{
	//初期化
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CMultiNumber::Uninit()
{

}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CMultiNumber::Update()
{
	//カウント加算
	m_nCnt++;

	// タイプによって処理を変える
	switch (m_NumberEffect)
	{
		//点滅　赤と黄色
	case NUMBER_EFFECT::FLASHING_RED_YELLOW:
		//2Fごとに色を変える
		if (m_nCnt % 4 == 0)
		{
			//赤
			SetColor(RedColor);
		}
		else if (m_nCnt % 4 == 2)
		{
			//黄
			SetColor(YellowColor);
		}
		break;

		//点滅
	case NUMBER_EFFECT::FLASHING:
		//2Fごとに点滅
		if (m_nCnt % 4 == 0)
		{
			SetDisp(true);
		}
		else if (m_nCnt % 4 == 2)
		{
			SetDisp(false);
		}
		break;
	}

	//nullcheck
	if (!m_pNumberList.empty())
	{
		//サイズ分
		for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
		{
			//nullcheck
			if (m_pNumberList[nCnt])
			{
				//更新
				m_pNumberList[nCnt]->Update();
			}
		}
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CMultiNumber::Draw()
{
	//描画しない時
	if (!GetDisp())
	{
		return;
	}

	//ワールドマトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot(), OneVector3);

	//nullcheck
	if (GetParentMtxPtr())
	{
		//親のマトリックスを掛け合わせる
		*GetMtxWorldPtr() *= *GetParentMtxPtr();
	}

	//nullcheck
	if (!m_pNumberList.empty())
	{
		//サイズ分
		for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
		{
			//nullcheck
			if (m_pNumberList[nCnt])
			{
				//描画
				m_pNumberList[nCnt]->Draw();
			}
		}
	}
}

//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
void CMultiNumber::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //_DEBUG
}

//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CMultiNumber> CMultiNumber::Create(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & onesize, int const nValue, int const nDigits, NUMBER_TYPE const type, CScene::OBJTYPE const objtype)
{
	//メモリ確保
	std::shared_ptr<CMultiNumber> pMultiNumber(new CMultiNumber);

	//nullcheck
	if (pMultiNumber)
	{
		//初期化処理
		pMultiNumber->Init();

		//Numberの生成
		for (int nCnt = 0; nCnt < nDigits; nCnt++)
		{
			if (type == CMultiNumber::NUMBER_TYPE::NUMBER_2D)
			{
				//配列に追加
				//数値の並ぶ中心をposとする
				//pMultiNumber->m_pNumberList.emplace_back(CScene2D::Create_SelfManagement(pos + D3DXVECTOR3(-onesize.x * (nDigits - 1) * 0.5f + onesize.x * nCnt, 0.0f, 0.0f), onesize, WhiteColor));
			}
			else if (type == CMultiNumber::NUMBER_TYPE::NUMBER_3D)
			{
				//配列に追加
				//数値の並ぶ中心をposとする
				pMultiNumber->m_pNumberList.emplace_back(CScene3D::ScenePolygonCreateSelfManagement<CScene3D>(D3DXVECTOR3(-onesize.x * (nDigits - 1) * 0.5f + onesize.x * nCnt, 0.0f, 0.0f), onesize, WhiteColor, CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_UI_NUMBER)));
				pMultiNumber->m_pNumberList[pMultiNumber->m_pNumberList.size() - 1]->SetParentMtxPtr(pMultiNumber->GetMtxWorldPtr());
				pMultiNumber->m_pNumberList[pMultiNumber->m_pNumberList.size() - 1]->SetLighting(false);

			}
		}

		pMultiNumber->SetPos(pos);
		pMultiNumber->SetSize(onesize);

		//数値の初期化
		pMultiNumber->SetMultiNumber(nValue);

		//Sceneのリストで管理
		pMultiNumber->SetObjType(objtype);
		pMultiNumber->AddSharedList(pMultiNumber);

		//リターン
		return pMultiNumber;
	}
	return nullptr;
}
//------------------------------------------------------------------------------
//スコア加算処理
//------------------------------------------------------------------------------
void CMultiNumber::SetMultiNumber(int nValue)
{
	//数字が入っている桁数
	int nDigits = 0;

	//値を加算
	m_nValue = nValue;

	//カウントストップ　桁数を基に計算
	if (m_nValue >= (int)powf(10.0f, (float)m_pNumberList.size()))
	{
		m_nValue = (int)powf(10.0f, (float)m_pNumberList.size()) - 1;
	}
	//0以下にしない
	else if (m_nValue < 0)
	{
		m_nValue = 0;
	}

	//各桁の計算
	for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
	{
		//数値を出す
		int nMultiNumber = m_nValue % (int)powf(10.0f, m_pNumberList.size() - (float)nCnt) / (int)powf(10.0f, m_pNumberList.size() - (float)nCnt - 1.0f);
		float fMultiNumber = (float)nMultiNumber / 10.0f;

		//UV座標設定
		m_pNumberList[nCnt]->SetAnimation(D3DXVECTOR2(fMultiNumber, 0.0f), D3DXVECTOR2(0.1f, 1.0f));

		//現在の桁で切り捨てた時に値が0以上場合
		if (fabs(m_nValue / (int)powf(10.0f, m_pNumberList.size() - (float)nCnt - 1.0f)) > 0)
		{
			//桁数カウント
			nDigits++;
		}
	}
	//桁数0のときは1に設定
	if (nDigits == 0)
	{
		nDigits = 1;
	}

	//桁数に応じて表示非表示の設定
	for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
	{
		//最大桁数 - 現在の桁数がカウントより小さい場合はtrue
		m_pNumberList[nCnt]->SetDisp(nCnt >= m_pNumberList.size() - nDigits ? true : false);
	}
}

//------------------------------------------------------------------------------
//座標設定
//------------------------------------------------------------------------------
void CMultiNumber::SetPos(D3DXVECTOR3 const &pos)
{
	//座標代入
	CSceneBase::SetPos(pos);

	//桁数分繰り返す
	for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
	{
		//nullcheck
		if (m_pNumberList[nCnt])
		{
			//座標設定
			m_pNumberList[nCnt]->SetPos(pos + D3DXVECTOR3(-m_pNumberList[nCnt]->GetSize().x * (m_pNumberList.size() - 1) * 0.5f + m_pNumberList[nCnt]->GetSize().x * nCnt, 0.0f, 0.0f));
		}
	}
}

//------------------------------------------------------------------------------
//サイズ設定
//------------------------------------------------------------------------------
void CMultiNumber::SetSize(D3DXVECTOR3 const &size)
{
	//座標代入
	CSceneBase::SetSize(size);

	//桁数分繰り返す
	for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
	{
		//nullcheck
		if (m_pNumberList[nCnt])
		{
			//座標設定
			m_pNumberList[nCnt]->SetSize(size);
		}
	}
	SetPos(GetPos());
}

//------------------------------------------------------------------------------
//色設定
//------------------------------------------------------------------------------
void CMultiNumber::SetColor(D3DXCOLOR const &col)
{
	CSceneBase::SetColor(col);

	if (!m_pNumberList.empty())
	{
		//桁数分繰り返す
		for (size_t nCnt = 0; nCnt < m_pNumberList.size(); nCnt++)
		{
			//nullcheck
			if (m_pNumberList[nCnt])
			{
				//色設定
				m_pNumberList[nCnt]->SetColor(col);
			}
		}
	}
}

//------------------------------------------------------------------------------
//タイプ設定
//------------------------------------------------------------------------------
void CMultiNumber::Settype(NUMBER_EFFECT effect)
{
	// タイプによって処理を変える
	switch (m_NumberEffect)
	{
		case NUMBER_EFFECT::NONE:
			SetDisp(false);
			break;

		case NUMBER_EFFECT::FLASHING:

			break;

		case NUMBER_EFFECT::FLASHING_RED_YELLOW:

			break;

	}
}

