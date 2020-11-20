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
#define SIZE_CHATTEXT			(64)	// チャットテキストの最大サイズ

// ===================================================================
// クラス定義
// ===================================================================
class CChatTab;

class CChatBase : public CScene
{
public:
	typedef enum
	{	// チャット所有者
		OWNER_OWN,		// 自身
		OWNER_GUEST,	// ゲスト
		OWNER_MAX
	} TEXTOWNER;

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
