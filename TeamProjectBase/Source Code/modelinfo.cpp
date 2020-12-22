//-----------------------------------------------------------------------------
//
//モデル情報管理  [modelinfo.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "modelinfo.h"
#include "manager.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
//静的メンバ変数の初期化
//-----------------------------------------------------------------------------
Vec<S_ptr<CModelInfo>> CModelInfo::m_apModelList = {};

//テクスチャ名の設定
Vec<CModelInfo::MODELNAME> CModelInfo::m_aModelFileNameList =
{
	//爆弾関係
	{ "data/MODEL/BombBox/bombbox.x" ,										MODEL_BOMBBOX },									//爆弾
	{ "data/MODEL/Battery/Battery_1.x" ,									MODEL_BATTERY_SMALL },								//小バッテリー
	{ "data/MODEL/Battery/Battery_2.x" ,									MODEL_BATTERY_BIG },								//大バッテリー
	{ "data/MODEL/ConnectionPort/Rca.x" ,									MODEL_RCA_PORT },									//LANポート
	{ "data/MODEL/ConnectionPort/Usb.x" ,									MODEL_USB_PORT },									//USBポート

	//モジュール
	{ "data/MODEL/test/test_module.x" ,										MODEL_MODULE_DEFAULT },								//モジュール
	{ "data/MODEL/test/test_module_none.x" ,								MODEL_MODULE_NONE },								//モジュール　何もないやつ　茶色の
	{ "data/MODEL/BombTimer/BombTimer.x" ,									MODEL_MODULE_TIMER },								//タイマー
	{ "data/MODEL/Module0/ModuleNumber0.x" ,								MODEL_MODULE_NO0 },									//キーパッド
	{ "data/MODEL/Module1/ModuleNumber1.x" ,								MODEL_MODULE_NO1 },									//キーパッド
	{ "data/MODEL/Module2/ModuleNumber2.x" ,								MODEL_MODULE_NO2 },									//キーパッド
	{ "data/MODEL/Module3/ModuleNumber3.x" ,								MODEL_MODULE_NO3 },									//キーパッド
	{ "data/MODEL/Module4/ModuleNumber4.x" ,								MODEL_MODULE_NO4 },									//キーパッド

	//モジュールのパーツ
	{ "data/MODEL/Lamp/Lamp.x" ,											MODEL_MODULEPARTS_CLEARLAMP },						//ランプ
	{ "data/MODEL/ProgressLamp/ProgressLamp.x" ,							MODEL_MODULEPARTS_PROGRESSLAMP },					//進捗度ランプ

	{ "data/MODEL/Module0/ModuleNumber0_Button.x" ,							MODEL_MODULEPARTS_NO0_KEYPAD },						//キーパッド
	{ "data/MODEL/Module1/ModuleNumber1_Button.x" ,							MODEL_MODULEPARTS_NO1_KEYPAD },						//キーパッド
	{ "data/MODEL/Module2/ModuleNumber2_Cord.x" ,							MODEL_MODULEPARTS_NO2_WIRE },						//ワイヤー
	{ "data/MODEL/Module2/ModuleNumber2_CordCutting.x" ,					MODEL_MODULEPARTS_NO2_WIRE_CUT },					//ワイヤー　切れてる状態
	{ "data/MODEL/Module3/ModuleNumber3_Button.x" ,							MODEL_MODULEPARTS_NO3_BUTTON },						//キーパッド
	{ "data/MODEL/Module3/ModuleNumber3_Button2.x" ,						MODEL_MODULEPARTS_NO3_DECITION_BUTTON },			//キーパッド　送信ボタン

	{ "data/MODEL/Module4/ModuleNumber4_Lamp.x" ,							MODEL_MODULEPARTS_NO4_BUTTON },						//キーパッド

	// タブレット
	{ "data/MODEL/Tablet/Tablet.x" ,										MODEL_TABLET },										// タブレット
	{ "data/MODEL/Tablet/TabletButton.x" ,									MODEL_TABLET_BUTTON },								// タブレットボタン

	//背景
	{ "data/MODEL/BackGround/Bg.x" ,										MODEL_BG },									// タブレットボタン
	{ "data/MODEL/BackGround/Bg_Desk.x" ,										MODEL_BG_TABLE },									// タブレットボタン

};

//-----------------------------------------------------------------------------
//コンストラクタ
//-----------------------------------------------------------------------------
CModelInfo::CModelInfo()
{
	//初期化
	m_aName = {};
	m_matBuff = nullptr;
	m_mesh = nullptr;
	m_matNum = 0;
};

//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CModelInfo::~CModelInfo()
{
	//バッファ開放
	if (m_matBuff)
	{
		m_matBuff->Release();
		m_matBuff = nullptr;
	}
	//メッシュ開放
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = nullptr;
	}
}

//-----------------------------------------------------------------------------
//モデル読み込み
//----------------------------------------------------------------------------
void CModelInfo::ModelLoad(HWND hwnd)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (size_t nCnt = 0; nCnt < m_aModelFileNameList.size(); nCnt++)
	{
		//debug
		std::cout << "Model Load - " << nCnt << m_aModelFileNameList[nCnt].modelname.data() << NEWLINE;

		//メモリ確保
		m_apModelList.emplace_back(std::make_shared<CModelInfo>());
		m_apModelList[nCnt]->m_modeltype = m_aModelFileNameList[nCnt].modeltype;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(m_aModelFileNameList[nCnt].modelname.data(),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_apModelList[nCnt]->m_matBuff,
			NULL,
			&m_apModelList[nCnt]->m_matNum,
			&m_apModelList[nCnt]->m_mesh)))
		{
			//失敗したとき
			std::cout << "LOAD FAILED!!!! >> " << m_aModelFileNameList[nCnt].modelname.data() << NEWLINE;
		}
	}
}

//-----------------------------------------------------------------------------
//モデル破棄
//----------------------------------------------------------------------------
void CModelInfo::ModelUnload()
{
	m_apModelList.clear();
}

//-----------------------------------------------------------------------------
//モデル取得
//----------------------------------------------------------------------------
S_ptr<CModelInfo> CModelInfo::GetModelInfo(CModelInfo::MODEL_TYPE modeltype)
{
	//モデルタイプと一致するやつのイテレータ取得
	auto itr = std::find_if(m_apModelList.begin(), m_apModelList.end(),
		[modeltype](S_ptr<CModelInfo>const &pModel) { return pModel->GetModelType() == modeltype; });

	//イテレータ取得できたかチェック
	if (itr != m_apModelList.end())
	{
		//return
		return *itr;
	}

	return nullptr;
}
