//------------------------------------------------------------------------------
//
//複数桁の数字の処理  [multinumber.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

class CMultiNumber : public CScene
{
public:
	//種類
	enum TYPE
	{
		TYPE_NORMAL,							//通常
		TYPE_RED,								//赤字
		TYPE_FLASHING,							//点滅
		TYPE_CENTER_DISAPPEAR,					//中央に出てきて自然に消えてく
	};

	CMultiNumber();
	~CMultiNumber();
	HRESULT Init();													//初期化
	void Uninit();													//終了
	void Update();													//更新
	void Draw();													//描画
	void ShowDebugInfo();											//デバッグ情報
	static std::shared_ptr<CMultiNumber> Create(D3DXVECTOR3 pos, D3DXVECTOR3 onesize, int nValue, int nDigits, CScene::OBJTYPE objtype);	//生成処理
	void SetMultiNumber(int nValue);								//スコア設定処理

	void SetCol(D3DXCOLOR col);										//色設定
	void SetPos(D3DXVECTOR3 pos);									//座標設定
	void SetSize(D3DXVECTOR3 size);									//サイズ設定
	void Settype(CMultiNumber::TYPE type) { m_type = type; };		//種類設定

	D3DXVECTOR3 GetPos() { return m_pos; };							//座標取得
	D3DXVECTOR3 GetSize() { return m_onesize; };					//サイズ取得

private:
	std::vector<std::unique_ptr<CScene2D>> m_pNumberList;	//ナンバーのポインタのリスト
	D3DXVECTOR3 m_pos;										//座標
	D3DXVECTOR3 m_onesize;									//1つ当たりのサイズ
	int m_nValue;											//値
	int m_nCnt;												//点滅カウント
	TYPE m_type;											//種類

};

#endif