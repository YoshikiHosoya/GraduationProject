//------------------------------------------------------------------------------
//
//レンダリング処理  [renderer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _RENDERER_H_
#define _RENDERER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CFade;
class CDebugProc;
class CCamera;
class CLight;

class CRenderer
{
public:

	//レンダラー周りのコマンド
	enum RENDERER_COMMAND
	{
		RENDERER_LIGHTING_ON,				//ライティングON
		RENDERER_LIGHTING_OFF,				//ライティングOFF
		RENDERER_CULLING_NONE,				//カリングしない
		RENDERER_CULLING_CCW,				//裏をカリング
		RENDERER_CULLING_CW,				//表をカリング
		RENDERER_ALPHABLEND_ADD,			//加算合成
		RENDERER_ALPHABLEND_SUB,			//減算合成
		REDNERER_ALPHABLEND_DEFAULT,		//通常状態
		RENDERER_ZTEST_OFF,					//Zテスト無効
		RENDERER_ZTEST_OFF_ZWRITING_ON,		//Zテスト無効でZライティング有効
		RENDERER_ZTEST_ON_ZWRITING_OFF,		//Zテスト有効でZライティング無効
		RENDERER_ZTEST_DEFAULT,				//Zテスト通常状態
		RENDERER_FOG_ON,					//フォグON
		RENDERER_FOG_OFF,					//フォグOFF
		RENDERER_WIRE_ON,					//ワイヤーON
		RENDERER_WIRE_OFF,					//ワイヤーOFF
	};

	CRenderer();			//コンストラクタ
	~CRenderer();			//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//初期化
	void Uninit();													//終了
	void Update();													//更新
	void Draw();													//描画
	void SetRendererCommand(RENDERER_COMMAND Command);				//レンダラー設定　ショートカット用
	void ResetRenderer();											//モード遷移時のレンダラー情報のリセット

	void SetDispImGuiInfo(bool ImGuiInfo)		{ m_bDispImGuiInfo = ImGuiInfo; };			//Imgui表示非表示設定
	void SetDispDebugProcInfo(bool bDebugInfo) { m_bDispDebugProcInfo = bDebugInfo; };		//DebugProc表示非表示設定

	bool CheckShowImGuiInfo()		{ return m_bDispImGuiInfo; };							//Imgui情報表示してるか確認
	bool CheckShowDebugProcInfo()	{ return m_bDispDebugProcInfo; };						//DebugProc情報表示してるか確認

	LPDIRECT3DDEVICE9 GetDevice()	{ return m_pD3DDevice; };		//デバイス情報取得
	CFade *GetFade()				{ return m_pFade.get();};		//フェード取得
	CCamera *GetCamera()			{ return m_pCamera.get(); };	//カメラ取得
	CLight *GetLight()				{ return m_pLight.get(); };		//ライトの情報取得

private:
	static D3DXCOLOR m_BackColor;					//背景色

	LPDIRECT3D9			m_pD3D;						//ウィンドウ生成に必要
	LPDIRECT3DDEVICE9	m_pD3DDevice;				//ウィンドウ生成に必要
	D3DPRESENT_PARAMETERS m_d3dpp;					//プレゼンテーションパラメータ

	std::unique_ptr<CFade>		m_pFade;			//フェードのポインタ
	std::unique_ptr<CDebugProc>	m_pDebugProc;		//デバッグテキストへのポインタ
	std::unique_ptr<CCamera>	m_pCamera;			//カメラへのポインタ
	std::unique_ptr<CLight>		m_pLight;			//ライトへのポインタ

	bool				m_bDispImGuiInfo;			//Imgui描画するかどうか
	bool				m_bDispDebugProcInfo;		//デバッグテキスト表示するか


	void ResetDevice();								//Imguiのリセット
	void RendererDebugInfo();						//レンダラーのデバッグ情報表記
};

#endif