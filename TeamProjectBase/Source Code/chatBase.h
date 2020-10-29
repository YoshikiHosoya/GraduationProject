// ===================================================================
//
// チャットの基盤処理 [ chatBase.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHATBASE_H_
#define _CHATBASE_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "scene.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CChatTab;

class CChatBase : public CScene
{
public:
	CChatBase();
	~CChatBase();

	virtual HRESULT Init(void);			// 初期化
	virtual void Update(void);			// 更新
	virtual void Draw(void);			// 描画
	virtual void ShowDebugInfo() {};		//デバッグ情報表記

	static std::shared_ptr<CChatBase> Create(void);	// 生成

private:
	CChatTab *m_pTab;	// タブのポインタ
};

#endif
