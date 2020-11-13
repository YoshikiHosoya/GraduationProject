//------------------------------------------------------------------------------
//
//モジュールパーツのキーパッド  [module_parts_No1_ShapeKey.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_NO2_SHAPEKEY_H_
#define _MODULE_PARTS_NO2_SHAPEKEY_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"
#include "module_No1_ShapeKeypad.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;
class CScene3D;

class CModule_Parts_No1_ShapeKey : public CModule_Parts_Base
{
public:

	CModule_Parts_No1_ShapeKey();
	virtual ~CModule_Parts_No1_ShapeKey();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetShape(CModule_No1_ShapeKeyPad::SHAPE shape);

	CModule_No1_ShapeKeyPad::SHAPE GetShape() { return m_Shape; };
protected:

private:
	S_ptr<CScene3D> m_pShape;			//シンボル
	CModule_No1_ShapeKeyPad::SHAPE m_Shape;							//シンボルの番号
};
#endif