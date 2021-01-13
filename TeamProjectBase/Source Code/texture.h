//------------------------------------------------------------------------------
//
//テクスチャ管理  [texture.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

#ifndef _TEXTURE_H_
#define _TEXTURE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
//前方宣言
class CTexture
{
public:
	CTexture() {};
	~CTexture() {};

	//通常のテクスチャ
	enum TEX_TYPE
	{
		TEX_NONE = 0,

		TEX_EFFECT_PARTICLE,					//パーティクル

		//マテリアル
		TEX_MATERIAL_FIELD000,					//マテリアル
		TEX_MATERIAL_FIELD001,					//マテリアル
		TEX_MATERIAL_FIELD002,					//マテリアル
		TEX_MATERIAL_FIELD003,					//マテリアル
		TEX_MATERIAL_FIELD004,					//マテリアル

		TEX_MODULEPARTS_MODULE03_SENDBUTTON,	//送信ボタン

		TEX_CHAT_BACK,							// チャットタブの背景
		TEX_CHAT_TITLE,							// チャットタブのタイトル
		TEX_CHAT_WRITEWINDOW,					// チャットタブの書き込みウィンドウ
		TEX_CHAT_TABOPEN,						// チャットタブ (開く)
		TEX_CHAT_TABCLOSE,						// チャットタブ (閉じる)
		TEX_CHAT_TABLETOPEN,					// タブレット (開く)
		TEX_CHAT_TABLETCLOSE,					// タブレット (閉じる)
		TEX_CHAT_BOX_00,						// チャットボックス00
		TEX_CHAT_BOX_01,						// チャットボックス01
		TEX_CHAT_BOX_02,						// チャットボックス02
		TEX_CHAT_BOX_03,						// チャットボックス03

		TEX_CONNECT_LOAD,						// 接続中のロードアイコン
		TEX_CONNECT_BACK_00,					// 接続中の背景(1P)
		TEX_CONNECT_BACK_01,					// 接続中の背景(2P)
		TEX_CONNECT_CONNECTING,					// 接続中の表記
		TEX_CONNECT_CONNECTED,					// 接続完了の表記
		TEX_CONNECT_SELECT_MODE,				// モード選択
		TEX_CONNECT_SELECT_LEVEL,				// レベル選択
		TEX_CONNECT_SELECT_REMOVE,				// モード(解除)
		TEX_CONNECT_SELECT_SOLVE,				// モード(解読)
		TEX_CONNECT_SELECT_EASY,				// イージー
		TEX_CONNECT_SELECT_NORMAL,				// ノーマル
		TEX_CONNECT_SELECT_HARD,				// ハード
		TEX_CONNECT_GUEST_SELECTING,			// ゲストの選択中
		TEX_CONNECT_GUEST_SELECTED,				// ゲストの選択
		TEX_CONNECT_DECIDE,						// 決定
		TEX_CONNECT_COMMENT_REMOVE,				// 解除の説明
		TEX_CONNECT_COMMENT_SOLVE,				// 解読の説明
		TEX_CONNECT_COMMENT_EASY,				// イージーの説明
		TEX_CONNECT_COMMENT_NORMAL,				// ノーマル野説明
		TEX_CONNECT_COMMENT_HARD,				// ハードの説明
		TEX_CONNECT_CAUTION_MODE,				// モード選択時の注意
		TEX_CONNECT_CAUTION_LEVEL,				// レベル選択時の注意

		TEX_UI_PRESSBUTTON,						//UI プレス

		TEX_UI_TITLE_LOGO,						//UI タイトルロゴ
		TEX_UI_TITLE_START,						//UI タイトル　スタート
		TEX_UI_TITLE_END,						//UI タイトル　エンド

		TEX_UI_RESULT_LOGO,						//UI リザルトロゴ
		TEX_UI_RESULT_TIMER,					//UI リザルト　タイム
		TEX_UI_RESULT_CLEAR,					//UI リザルト　クリア
		TEX_UI_RESULT_FAILED,					//UI リザルト　失敗
		TEX_UI_RESULT_BAR,						//UI リザルト　バー
		TEX_UI_RESULT_WINDOW,					//UI リザルト　バー
		TEX_UI_RESULT_RETRY,					//UI リザルト　リトライ
		TEX_UI_RESULT_END,						//UI リザルト　終了
		TEX_UI_RESULT_COMMA,					//UI リザルト　コンマ
		TEX_UI_RESULT_SEMICOLON,				//UI リザルト　セミコロン





		TEX_UI_BRUSHCURSOR,						// ブラシカーソル
		TEX_UI_ERASERCURSOR,					// 消しゴムカーソル
		TEX_UI_BRUSHBUTTON,						// ブラシ
		TEX_UI_ERASERBUTTON,					// 消しゴム
		TEX_UI_SENDBUTTON,						// 送信

		TEX_UI_DECODING_T00,					// 解読UI
		TEX_UI_DECODING_T01,					// 解読UI
		TEX_UI_DECODING_T02,					// 解読UI
		TEX_UI_DECODING_T03,					// 解読UI
		TEX_UI_DECODING_T04,					// 解読UI
		TEX_UI_DECODING_T05,					// 解読UI
		TEX_UI_DECODING_BACKLIGHT,				// 解読UIバックライト

		TEX_UI_DECODING_D00,					// 解読書(Document)
		TEX_UI_DECODING_D01,					// 解読書(Document)
		TEX_UI_DECODING_D02,					// 解読書(Document)
		TEX_UI_DECODING_D03,					// 解読書(Document)
		TEX_UI_DECODING_D04,					// 解読書(Document)
		TEX_UI_DECODING_D05,					// 解読書(Document)

		TEX_UI_DECODING_CLOSE,					// 閉じるボタン
		TEX_UI_DECODING_S_BAR,					// スクロールバー
		TEX_UI_DECODING_S_HANDLE,				// スクロールハンドル

		TEX_MAX									//MAX
	};

	//アニメーション用のテクスチャ
	//分割されているテクスチャ
	enum SEPARATE_TEX_TYPE
	{
		SEPARATE_TEX_NONE = 0,

		//UI
		SEPARATE_TEX_UI_NUMBER,								//ナンバー
		SEPARATE_TEX_UI_RESULT_NUMBER,						//ナンバー リザルト用

		//エフェクト
		SEPARATE_TEX_EFFECT_EXPLOSION00,					//爆発

		//モジュールのパーツ
		SEPARATE_TEX_MODULEPARTS_MODULE00,					//モジュールナンバー0 暗号みたいなやつ
		SEPARATE_TEX_MODULEPARTS_MODULE01,					//モジュールナンバー1 〇×△□
		SEPARATE_TEX_MODULEPARTS_MODULE03,					//モジュールナンバー3 パスワードの模様


		SEPARATE_TEX_MAX									//MAX
	};

	typedef struct
	{
		LPDIRECT3DTEXTURE9 m_apSeparateTexture;		//分割テクスチャへのポインタ
		SEPARATE_TEX_TYPE type;						//テクスチャの種類
		D3DXVECTOR2 m_UVCnt;						//UVの枚数
		D3DXVECTOR2 m_UVSize;						//UVの大きさ

	}SEPARATE_TEX_INFO;

	static void TexLoad(HWND hwnd);						//テクスチャ生成
	static void TexUnload();							//テクスチャ破棄

	static LPDIRECT3DTEXTURE9 GetTexture(TEX_TYPE textype);							//通常テクスチャの取得

	static std::vector<std::string> &GetTexFileName() { return m_aTexFileName; };						//テクスチャ名取得
	static std::vector<std::string> &GetSeparateFileName() { return m_aSeparateTexFileNameList; };		//分割テクスチャ名取得

	static LPDIRECT3DTEXTURE9 GetSeparateTexture(SEPARATE_TEX_TYPE textype);		//分割テクスチャ取得
	static D3DXVECTOR2 GetSparateTex_UVSize(SEPARATE_TEX_TYPE textype);				//分割テクスチャ　UVのサイズ取得
	static D3DXVECTOR2 GetSparateTex_UVCnt(SEPARATE_TEX_TYPE textype);				//分割テクスチャ　UVの枚数取得


private:
	static std::vector<LPDIRECT3DTEXTURE9> m_apTextureList;							//通常テクスチャへのポインタのリスト
	static std::vector<std::string> m_aTexFileName;											//通常テクスチャのファイル名


	static std::vector<std::string> m_aSeparateTexFileNameList;								//分割テクスチャのファイル名
	static std::vector<SEPARATE_TEX_INFO> m_apSeparateTexInfoList;					//分割テクスチャに関する情報のポインタ

};

#endif