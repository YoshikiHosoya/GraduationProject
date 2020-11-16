//------------------------------------------------------------------------------
//
//モジュールパーツのキーパッド  [module_parts_No0_SymbolKey.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_NO1_SYMBOLKEY_H_
#define _MODULE_PARTS_NO1_SYMBOLKEY_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;
class CScene3D;

class CModule_Parts_No0_SymbolKey : public CModule_Parts_Base
{
public:

	enum class KEYPAD_STATE
	{
		NORMAL,
		FAILED,
		CLEAR,
	};

	CModule_Parts_No0_SymbolKey();
	virtual ~CModule_Parts_No0_SymbolKey();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetSymbol(int nSymbol);
	void SetKeypadState(KEYPAD_STATE keypadstate);


	KEYPAD_STATE GetKeyPadState()				{ return m_KeyPadState; };
	int GetSymbolNum()							{ return m_nSymbolNum; };
protected:

private:
	U_ptr<CScene3D> m_pLight;			//ライト
	S_ptr<CScene3D> m_pSymbol;		//シンボル

	KEYPAD_STATE m_KeyPadState;					//ライトの状態
	int m_nCntLampCnt;							//ライト用のカウント
	int m_nSymbolNum;							//シンボルの番号

};
#endif