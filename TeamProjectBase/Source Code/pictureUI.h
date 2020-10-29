//*************************************************************************************************************
//
// 絵のUI処理 [pictureUI.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _PICTUREUI_H_
#define _PICTUREUI_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene2D;
class CPictureUI : public CScene
{
public:
	/* 列挙型定義 */
	typedef enum 
	{
		TYPE_NONE = -1,
		TYPE_BRUSH,			// ブラシ
		TYPE_ERASER,		// 消しゴム
		TYPE_MAX
	} TYPE;

	/* 構造体 */
	typedef struct CREATEINFO
	{
		D3DXVECTOR3        pos;			// 位置
		D3DXVECTOR3        size;		// サイズ
		D3DXCOLOR          col;			// 色
		LPDIRECT3DTEXTURE9 tex;			// テクスチャポインタ
		CScene::OBJTYPE    objtype;		// オブジェクトタイプ
	}CREATEINFO;

	CPictureUI();				// コンストラクタ
	~CPictureUI();				// デストラクタ

	static std::shared_ptr<CPictureUI>Create(void);	// 生成
	
	static void Load(void);		// 読み込み
	static void Unload(void) {}	// 開放


	HRESULT Init();				//初期化
	inline void Uninit() {}		//終了
	void Update();				//更新
	void Draw();				//描画
	void ShowDebugInfo();		//デバッグ情報表記

	int GetPressedType(void) { return m_PressedType; }

private:
	bool Collision(CScene2D *pScene2D,int& nCntType);
	static CREATEINFO m_aCreateInfo[TYPE_MAX];
	std::shared_ptr<CScene2D> m_pScene2D[TYPE_MAX];		// シーン2D
	bool m_bIsOn[TYPE_MAX];
	TYPE m_PressedType;
};
#endif