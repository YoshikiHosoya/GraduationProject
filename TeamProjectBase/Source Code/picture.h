//*************************************************************************************************************
//
// 絵の処理 [picture.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _PICTURE_H_
#define _PICTURE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CPicture : public CScene
{
public:
	enum
	{
		FLAG_NONE = -1,
		FLAG_DISP,
		FLAG_UPDATE,
		FLAG_MAX
	};

	enum
	{
		MASK_NONE = 0,
		MASK_DISP = 0b0001,
		MASK_UPDATE = 0b0010,
		MASK_MAX = 0b0011,
	};

	/* 構造体定義 */
	typedef struct MESHINFO
	{
		MESHINFO();
		~MESHINFO();
		LPDIRECT3DVERTEXBUFFER9  pVtexBuff;								// 頂点バッファのポインタ
		LPDIRECT3DINDEXBUFFER9   pIdxBuff;								// インデックスのバッファのポインタ
		LPDIRECT3DTEXTURE9       pTexture;								// テクスチャのポインタ
		TRANSFORM                trans;									// トランス情報
	}MESHINFO;

	/* メンバ関数 */
	inline           CPicture() :CScene(){}
	inline           ~CPicture() {}
	HRESULT          Init();																	//初期化
	void             Uninit();																	//終了
	void             Update();																	//更新
	void             Draw();																	//描画
	inline void      ShowDebugInfo() {};														//デバッグ情報表記

	static HRESULT   Load(void);																// 読み込み
	static void      InitStaticMember(void);													// 静的メンバの初期化

	void             MatrixCal(void);															// マトリックスの計算

	// 設定関数
	inline void      SetPos(CONST D3DXVECTOR3 &pos) { m_mesh.trans.pos = pos; }					// 位置の設定
	inline void      SetRot(CONST D3DXVECTOR3 &rot) { m_mesh.trans.rot = rot; }					// 向きの設定
	inline void      SetScal(CONST D3DXVECTOR3 &scal) { m_mesh.trans.scal = scal; }				// スケールの設定
	inline void      SetFlag(const int nMask) { m_Flags.cValue = nMask; }
	// 取得関数
	inline MESHINFO* GetInfo(void) { return &m_mesh; }												// 情報の取得
	inline UVSHORT*  GetFlag(void) { return &m_Flags.cValue; }										// フラグの取得
	// クリエイト関数
	static std::shared_ptr<CPicture> Create(CONST D3DXVECTOR3 &pos, CONST D3DXVECTOR3 &rot);			// 生成
private:
	/* 内部で使う関数 */
	void        MakeVertex(LPDIRECT3DDEVICE9 pDevice);					// 頂点情報の作成
	void        MakeIndex(LPDIRECT3DDEVICE9 pDevice);					// インデックス情報の作成
	static void ReadFromLine(CONST_STRING Line);						// 1行から情報を読み取る

	/* メンバ変数 */
	static INTEGER2    m_nNumPolyBlock;		// ポリゴン数(ずらす大きさの設定後2倍にする)
	static UINT        m_nNumVertex;		// 総頂点数
	static UINT        m_nNumIndex;			// 総インデックス数
	static UINT        m_nNumPolygon;		// 総ポリゴン数
	static FLOAT2      m_size;				// 大きさ
	static FLOAT2      m_sizeShift;			// ずらす大きさ

	MESHINFO           m_mesh;				// メッシュ情報
	UBITS_4            m_Flags;				// フラグ
};


#endif // !_PICTURE_H_
