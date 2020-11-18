//------------------------------------------------------------------------------
//
//シーン処理  [scene.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

class CScene
{
public:

	enum OBJTYPE
	{
		OBJTYPE_NONE = 0,					//未設定
		OBJTYPE_BACK,						//後ろ
		OBJTYPE_MESHFIELD,					//メッシュ
		OBJTYPE_MAPOBJECT,					//マップに配置されるオブジェクト
		OBJTYPE_PICTURE,					// ピクチャ
		OBJTYPE_PICTURE_SYMBOL,				// ピクチャ3Dポリゴン
		OBJTYPE_BOMB,						//爆弾
		OBJTYPE_BOMB_EXTERIOR,				//爆弾の装飾品
		OBJTYPE_MODULE,						//モジュール
		OBJTYPE_MODULE_PARTS,				//モジュールのパーツ
		OBJTYPE_MODULE_PARTS_SYMBOL,		//モジュールのパーツ上のポリゴン
		OBJTYPE_EFFECT,						//エフェクト
		OBJTYPE_UI,							//UI
		OBJTYPE_NUMBER,						//数字
		OBJTYPE_PAUSE,						//ポーズ
		OBJTYPE_OPTION,						//オプション
		OBJTYPE_FRONT,						//前面
		OBJTYPE_DEBUG,						//デバッグ系
		OBJTYPE_MAX							//最大値
	};

	CScene();

	//純粋仮想関数　オーバーライド用
	virtual ~CScene();
	virtual HRESULT Init() = 0;														//初期化
	virtual void Update() = 0;														//更新
	virtual void Draw() = 0;														//描画
	virtual void ShowDebugInfo() = 0;												//デバッグ情報表記

	static void ShowNumAll();

	static void ReleaseAll();															//メモリ開放
	static void UpdateAll();															//更新処理
	static void DrawAll();																//描画処理
	static void ShowDebugInfoAll();														//全てのデバッグ情報表記処理

	static void ReleaseSpecificObject(OBJTYPE objtype);									//選択したオブジェクトタイプを全てリリース


	void Release()								{ m_bDeadFlag = true; };				//メモリ開放
	void SetObjType(OBJTYPE objtype)			{ m_objtype = objtype; };				//オブジェクトタイプ設定関数

	static void SetStop(bool bStop)				{ m_bStop = bStop; };					//ストップ処理
	static void Set1FAction()					{ m_b1FAction = true; };				//１Fだけ進める処理

	bool GetDeleteFlag()						{ return m_bDeadFlag; };				//死亡フラグ取得
	CScene::OBJTYPE GetObjType()				{ return m_objtype; };					//オブジェクトの種類取得

	static int GetNumAll()						{ return m_nNumAll; };					//総数取得
	static bool GetStop()						{ return m_bStop; };					//ストップ状態か取得
	static void GetSceneList(OBJTYPE objtype, Vec<S_ptr<CScene>> &SceneList);			//シーン情報取得（配列ごと）

	//Unique_ptrのリストに追加
	template <class T> void AddUniqueList(T ptr)
	{
		//ユニークポインタのリストに自分を追加
		m_UniqueSceneList[ptr->m_objtype].emplace_back(std::move(ptr));
	};

	//Shared_ptrのリストに追加
	template <class T> void AddSharedList(T ptr)
	{
		//共有ポインタのリストに自分を追加
		m_SharedSceneList[m_objtype].emplace_back(ptr);
	};

protected:

private:
	static Vec<U_ptr<CScene>> m_UniqueSceneList[OBJTYPE_MAX];		//ユニークポインタ　独占すべきポインタ
	static Vec<S_ptr<CScene>> m_SharedSceneList[OBJTYPE_MAX];		//シェアポインタ	共有される事があると思われるポインタ
	static int m_nNumAll;					//オブジェクト総数
	static int m_nCntStop;					//ストップのカウント
	static bool m_bStop;					//画面ストップするかどうか
	static bool m_b1FAction;				//1Fだけアクションするか

	bool m_bNonStop;						//ストップしない
	bool m_bDeadFlag;						//死亡フラグ
	OBJTYPE	m_objtype;						//タイプ

#ifdef _DEBUG
	static Vec<int> m_fUpdateTimeList;
	static Vec<int> m_fDrawTimeList;
#endif //_DEBUG
};

#endif

