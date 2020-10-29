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
class CPaintingPen;
class CPictureUI;
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
	typedef enum
	{
		MODE_EDIT = 0,	// 編集
		MODE_EXHI,		// 展示
		MODE_MAX		// 最大数
	} MODE;

	/* メンバ関数 */
	inline           CPicture() :CScene(){}																	// コンストラクタ
	inline           ~CPicture();																			// デストラクタ
	HRESULT          Init();																				// 初期化
	void             Update();																				// 更新
	void             Draw();																				// 描画
	inline void      ShowDebugInfo() {};																	// デバッグ情報表記

	static HRESULT   Load(void);																			// 読み込み
	static void      InitStaticMember(void);																// 静的メンバの初期化
	static void      UninitStaticMember(void);																// 静的メンバの終了

	void             MatrixCal(void);																		// マトリックスの計算

	// 設定関数
	inline void      SetPos(CONST D3DXVECTOR3 &pos) { m_trans.pos = pos; }									// 位置の設定
	inline void      SetRot(CONST D3DXVECTOR3 &rot) { m_trans.rot = rot; }									// 向きの設定
	inline void      SetScal(CONST D3DXVECTOR3 &scal) { m_trans.scal = scal; }								// スケールの設定
	inline void      SetFlag(const int nMask) { m_Flags.cValue = nMask; }									// フラグの設定
	inline void      SetFileName(CONST_STRING FileName) { m_FileName.Set(FileName); }						// ファイル名の設定
	inline void      SetMode(MODE mode) {m_mode = mode; }													// モードの設定
	inline void      BuildFlag(const int nFlagIndex) { Mybfunc_bit_set(m_Flags.cValue, nFlagIndex); }		// フラグを建てる
	inline void      BreakFlag(const int nFlagIndex) { Mybfunc_bit_clear(m_Flags.cValue, nFlagIndex); }		// フラグを壊す

	// 取得関数
	inline UVSHORT*  GetFlag(void) { return &m_Flags.cValue; }												// フラグの取得

	// クリエイト関数
	static std::shared_ptr<CPicture>Create(CONST D3DXVECTOR3 &pos, CONST D3DXVECTOR3 &rot, CONST MODE mode = MODE_EDIT, CONST_STRING FileName = NULL);	// 生成
private:
	/* 内部で使う関数 */
	void             MakeTexture(LPDIRECT3DDEVICE9 pDevice);					// テクスチャの作成
	void             MakeVertex(LPDIRECT3DDEVICE9 pDevice);						// 頂点情報の作成
	void             InitTextureColor(void);									// テクスチャカラーの初期化
	static void      ReadFromLine(CONST_STRING Line);							// 1行から情報を読み取る
	static void      CreatePixelPos(void);										// ピクセル位置の生成
	static void      ReleasePixelPos(void);										// ピクセル位置の開放
	static void      ReleasePen(void);											// ペンの開放
	void             PaintProc(void);											// 塗る処理
	bool             GetMousePosOnPicture(void);								// 絵上のマウス位置の取得
	static void      Writing(void);												// 書き込み処理
	static void      Reading(CString& file);									// 読み込み
	static void      TexterReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);// テクスチャ情報の1行から情報を読み取る

	/* メンバ変数 */
	static INTEGER2             m_nNumPixelBlock;			// ピクセル数
	static FLOAT2               m_size;					// 大きさ
	static D3DXVECTOR3          m_PlaneNor;				// 平面の法線
	static D3DXVECTOR2          m_PixelSize;				// ピクセルサイズ
	static UINT                 m_nNumDataMax;				// 最大データ数
	static CPaintingPen*        m_pPen;					// ペンのポインタ
	static D3DXVECTOR2          m_PixelSizehalf;			// ピクセルサイズの半分
	static D3DXVECTOR2*         m_pPixelPos;				// ピクセル位置のポインタ
	static UINT                 m_nNumMakeFile;			// ファイルを作った回数
	static std::shared_ptr<CPictureUI> m_pUi;						// UI
	LPDIRECT3DVERTEXBUFFER9     m_pVtexBuff;				// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9          m_pTexture;				// テクスチャポインタ
	UBITS_4                     m_Flags;					// フラグ
	TRANSFORM                   m_trans;					// トランス情報
	MODE                        m_mode;					// モード
	CString                     m_FileName;				// ファイル名
};


#endif // !_PICTURE_H_
