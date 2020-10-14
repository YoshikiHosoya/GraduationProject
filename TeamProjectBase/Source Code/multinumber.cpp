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
	m_type = CMultiNumber::TYPE_NORMAL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	switch (m_type)
	{
		//点滅タイプ
	case CMultiNumber::TYPE_FLASHING:
		//2Fごとに色を変える
		if (m_nCnt % 4 == 0)
		{
			//赤
			SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
		}
		else if (m_nCnt % 4 == 2)
		{
			//黄
			SetCol(D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f));
		}
		break;

		//中央に出てきて徐々に消えてく
	case CMultiNumber::TYPE_CENTER_DISAPPEAR:
		//カウントが一定以上になった時
		if (m_nCnt >= DISAPPEAR_COUNT)
		{
			//開放
			Release();
			return;
		}

		//色設定 徐々に透明になる感じ
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, (DISAPPEAR_COUNT - m_nCnt) * 0.01f));
		SetSize(GetSize() + D3DXVECTOR3(DISAPPEAR_SCALE_VALUE, DISAPPEAR_SCALE_VALUE, 0.0f));
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
std::shared_ptr<CMultiNumber> CMultiNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 onesize, int nValue, int nDigits, CScene::OBJTYPE objtype)
{
	//メモリ確保
	std::shared_ptr<CMultiNumber> pMultiNumber(new CMultiNumber);

	//nullcheck
	if (pMultiNumber)
	{
		//初期化処理
		pMultiNumber->Init();
		pMultiNumber->m_pos = pos;
		pMultiNumber->m_onesize = onesize;

		//Numberの生成
		for (int nCnt = 0; nCnt < nDigits; nCnt++)
		{
			//配列に追加
			//数値の並ぶ中心をposとする
			pMultiNumber->m_pNumberList.emplace_back(CScene2D::Create_SelfManagement(pos + D3DXVECTOR3(-onesize.x * (nDigits - 1) * 0.5f + onesize.x * nCnt, 0.0f, 0.0f), onesize, WhiteColor));
			pMultiNumber->m_pNumberList[pMultiNumber->m_pNumberList.size() - 1]->BindTexture(CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_UI_NUMBER));
		}

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
//色設定
//------------------------------------------------------------------------------
void CMultiNumber::SetCol(D3DXCOLOR col)
{
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
//座標設定
//------------------------------------------------------------------------------
void CMultiNumber::SetPos(D3DXVECTOR3 pos)
{
	//座標代入
	m_pos = pos;

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
void CMultiNumber::SetSize(D3DXVECTOR3 size)
{
	//座標代入
	m_onesize = size;

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
