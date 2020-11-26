//------------------------------------------------------------------------------
//
//爆弾の処理  [Bomb.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BOMB_H_
#define _BOMB_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "SceneX.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MODULE_INTERVAL (D3DXVECTOR3(125.0f,100.0f,80.0f))
#define MAX_MODULE_NUM	 (12)
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Base;
class CBomb_Exterior;

class CBomb : public CSceneX , public std::enable_shared_from_this<CBomb>
{
public:

	//難易度
	enum DIFFICULTY
	{
		EASY,
		NORMAL,
		HARD,
	};

	CBomb();
	virtual ~CBomb();

	virtual HRESULT Init()			override;									//初期化
	virtual void Update()			override;									//更新
	virtual void Draw()				override;									//描画
	virtual void ShowDebugInfo()	override;									//デバッグ情報表記

	static S_ptr<CBomb> CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, DIFFICULTY const difficulty);
	void Operation_Keyboard();													//操作　キーボード
	void Operation_Mouse();														//操作　マウス
	void Operation_Camera();													//カメラ操作

	void ModuleClearCheck();													//クリアしたかチェック
	void ModuleMiss();															//モジュールミスった

	CBomb_Exterior *GetBombExterior() { return m_pBombExterior.get(); };		//モジュール外装のポインタ取得
private:
	D3DXVECTOR3 m_RotDest;														//回転の決定先
	int m_nModuleNum;															//モジュール数
	int m_nSelectModuleNum;														//選択しているモジュール数
	bool m_bCameraDir;															//カメラが正面向いているか
	DIFFICULTY m_difficulty;													//難易度
	Vec<S_ptr<CModule_Base>> m_pModuleList;										//モジュールのリスト
	U_ptr<CBomb_Exterior> m_pBombExterior;										//爆弾の外装のポインタ

	void CreateModule();								//モジュール生成
	void CreateModule_Random();													//モジュール生成　ランダム配置
	void ModuleSelect();														//モジュールの洗濯処理
	void SearchHeadCanSelectNum(int nStartNum);									//一番最初の選択可能番号検索
#ifdef _DEBUG
	void CreateModuleDebug();
	static bool m_bCanExplosion;
#endif

	//------------------------------------------------------------------------------
	//モジュール生成関数
	//------------------------------------------------------------------------------
	template<class Module> bool CreateModuleOne()
	{
		//モジュールが最大数に達したとき
		if (m_pModuleList.size() >= MAX_MODULE_NUM)
		{
			return false;
		}

		//変数宣言
		int nX = m_pModuleList.size() % 3;
		int nY = (m_pModuleList.size() / 3) % 2;
		int nZ = m_pModuleList.size() / 6;

		if (nZ)
		{
			nX = 2 - nX;
		}

		m_pModuleList.emplace_back(CModule_Base::Create_Module<Module>
					(D3DXVECTOR3(CHossoLibrary::CalcEvenPosition(3, nX, MODULE_INTERVAL.x),
									CHossoLibrary::CalcEvenPosition(2, nY, -MODULE_INTERVAL.y),
									CHossoLibrary::CalcEvenPosition(2, nZ, MODULE_INTERVAL.z)),
				D3DXVECTOR3(0.0f, (D3DX_PI)* nZ, 0.0f), GetMtxWorldPtr()));

		return true;
	}
};
#endif