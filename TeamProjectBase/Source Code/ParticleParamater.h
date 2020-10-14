//------------------------------------------------------------------------------
//
//パーティクルのパラメータクラス  [particleparamater.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _PARTICLEPARAMATER_H_
#define _PARTICLEPARAMATER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DEFAULT_TEXTURE (CTexture::TEX_EFFECT_PARTICLE)			//デフォルトで使用するテクスチャ
#define DEFAULT_DAMPING (0.95f)									//デフォルトの減衰値
#define DEFAULT_GRAVITY_POWER (0.8f)							//デフォルトの重力の大きさ
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CParticleParam
{
public:
	//パーティクルのテキスト
	enum PARTICLE_TYPE
	{
		EFFECT_DEFAULT = 0,						//
		PARTICLE_MAX,
	};

	//パーティクルの向き
	enum PARTICLE_SHAPE
	{
		SHAPE_SPHERE = 0 ,
		SHAPE_CONE,
		SHAPE_CIRCLE_XY,
		SHAPE_LINE,
	};

	//コンストラクタ
	CParticleParam();
	~CParticleParam();

	void SetParamater(int nLife, D3DXVECTOR3 size, D3DXCOLOR col,int nNumber,float fSpeed);
	void UpdateParam();


	static HRESULT LoadParticleDefaultParam();
	static HRESULT SaveParticleDefaultParam(CParticleParam *pSaveParam,std::string aSaveFileName);

	//Get関数
	static std::vector<std::string> &GetFileNameList() {	return m_aFileNameList;};

	bool&GetSpeedRandom()								{ return m_bSpeedRandom; };					//パーティクルの速度がランダムかどうか
	bool &GetGravity()									{ return m_bGravity; };						//重力
	bool &GetFollowing()								{ return m_bFollowing; };					//追従するか

	bool &GetAlphaBlend_Add()							{ return m_bAlphaBlend_Add;};				//加算合成するか
	bool &GetAlphaBlend_Sub()							{ return m_bAlphaBlend_Sub;};				//減算合成するか
	bool &GetZTest()									{ return m_bZtest;};						//Zテストするか
	bool &GetZWrite()									{ return m_bZWrite;};						//Zライティングするか
	bool &GetBillboard()								{ return m_bBillboard; };					//ビルボード

	int &GetLife()										{ return m_nLife; };						//ライフ
	int &GetNumber()									{ return m_nNumber; };						//個数
	float &GetSpeed()									{ return m_fSpeed; };						//速度
	float &GetRange()									{ return m_fConeRange; };						//角度
	float &GetAlphaDamping()							{ return m_fAlphaDamping; };				//アルファ値の減衰値
	float &GetGravityPower()							{ return m_fGravityPower; };				//重力の大きさ

	D3DXVECTOR3 &GetLocalRandomPosRange()				{ return m_LocalPosRandomRange; };			//ローカル座標からランダムで出る時の値
	D3DXVECTOR3 &GetLocalPos()							{ return m_LocalPos; };						//ローカル座標
	D3DXVECTOR3 &GetSize()								{ return m_Size; };							//サイズ
	D3DXVECTOR3 &GetSizeDamping()						{ return m_SizeDamping; };					//サイズの減衰地
	D3DXCOLOR &GetCol()									{ return m_col; };							//色
	D3DXVECTOR3 &GetRot()								{ return m_rot;};							//角度

	bool &GetAnimation()								{ return m_bAnimation;};					//アニメーションするかどうか
	bool &GetAnimationLoop()							{ return m_bAnimationLoop; };				//アニメーションループするかどうか
	int &GetAnimationCntSwitch()						{ return m_nAnimationCntSwitch; };			//アニメーション切替のカウント

	D3DXVECTOR3 &GetCollisionSize()						{ return m_CollisionSize; };				//ランダム生成時の範囲
	bool &GetCollision()								{ return m_bCollision; };					//当たり判定あるか
	bool &GetPenetration()								{ return m_bPenetration; };					//貫通するかどうか
	bool &GetCollisionSizeCalc()						{ return m_bCollisionSizeCalc; };			//当たり判定生成時にサイズを計算するかどうか　ShotGunとかに必要
	int &GetCollisionAttackValue()						{ return m_nCollisionAttackValue; };		//攻撃力
	int &GetCollisionCnt()								{ return m_nCollisionCnt; };				//判定をする時間

	PARTICLE_SHAPE &GetShape()							{ return m_shape; };						//パーティクルの方向
	CTexture::TEX_TYPE &GetTex()						{ return m_Textype; };						//テクスチャ
	CTexture::SEPARATE_TEX_TYPE &GetSeparateTex()		{ return m_SeparateTex; };					//分割テクスチャ
	PARTICLE_TYPE &GetType()							{ return m_ParticleType; };					//パーティクルのタイプ

	static CParticleParam *GetDefaultParam(CParticleParam::PARTICLE_TYPE type) { return m_pParticleDefaultParamList[type].get(); };
	static bool ShowParamConboBox(CParticleParam::PARTICLE_TYPE & rType);

	//オペレータ
	void *operator = (const CParticleParam *pParam);

private:
	static std::vector<std::unique_ptr<CParticleParam>> m_pParticleDefaultParamList;	//パーティクルの初期パラメータのリスト
	static FILENAME_LIST m_aFileNameList;												//読み込むファイルのリスト

	bool m_bSpeedRandom;							//速度がランダムかどうか
	bool m_bGravity;								//重力をかけるか
	bool m_bFollowing;								//追従するか
	bool m_bAlphaBlend_Sub;							//加算合成するか
	bool m_bAlphaBlend_Add;							//減算合成するか
	bool m_bZtest;									//Zテストするか
	bool m_bZWrite;									//Zライティングするか
	bool m_bBillboard;								//ビルボードするかどうか

	int m_nLife;									//ライフ
	int m_nNumber;									//個数
	float m_fSpeed;									//速度
	float m_fConeRange;								//範囲
	float m_fAlphaDamping;							//アルファ値の減衰値
	float m_fGravityPower;							//重力の大きさ

	bool m_bAnimation;								//アニメーションするか
	bool m_bAnimationLoop;							//アニメーションがループするか
	int m_nAnimationCntSwitch;						//アニメーションが切り替わるカウント

	D3DXVECTOR3 m_CollisionSize;					//当たり判定の大きさ
	bool m_bCollision;								//当たり判定あるか
	bool m_bCollisionSizeCalc;						//当たり判定生成時にサイズを計算するかどうか　ShotGunとかに必要
	bool m_bPenetration;							//貫通するかどうか
	int m_nCollisionAttackValue;					//攻撃力
	int m_nCollisionCnt;							//判定をする時間

	D3DXVECTOR3 m_LocalPos;							//生成される座標
	D3DXVECTOR3 m_LocalPosRandomRange;				//ランダム生成時の範囲
	D3DXVECTOR3 m_Size;								//サイズ
	D3DXVECTOR3 m_SizeDamping;						//サイズの減衰値
	D3DXCOLOR m_col;								//色
	D3DXVECTOR3 m_rot;								//角度

	PARTICLE_SHAPE m_shape;							//パーティクルの出方
	CTexture::TEX_TYPE m_Textype;					//テクスチャ
	CTexture::SEPARATE_TEX_TYPE m_SeparateTex;		//分割テクスチャ
	PARTICLE_TYPE m_ParticleType;					//パーティクルのタイプ

};


class CreaterParam
{
public:
	//コンストラクタ
	CreaterParam()
	{
		m_bLoop = false;
		m_nLoopNum = 0;
		m_nLoopInterval = 0;
		m_text = CParticleParam::PARTICLE_TYPE::EFFECT_DEFAULT;
	}
	~CreaterParam() {};

private:
	bool m_bLoop;								//ループしたかどうか
	int m_nLoopNum;								//ループの回数
	int m_nLoopInterval;						//ループのインターバル
	CParticleParam::PARTICLE_TYPE m_text;		//読み込むテキスト
};

class CreatorWave
{
public:
	//コンストラクタ
	CreatorWave() { m_ParamList.clear(); };
	~CreatorWave() { m_ParamList.clear(); };
private:
	std::vector<std::unique_ptr<CreaterParam>> m_ParamList;
};

//
////パーティクルの生成口
//class CParticleCreators
//{
//public:
//	CParticleCreators() {};				//コンストラクタ
//	~CParticleCreators() {};				//デストラクタ
//
//	static void LoadParticleCreators();			//パーティクル生成口のロード
//	static CParticleCreators *Create();
//private:
//	int m_nCntTime;		//時間計測
//	int m_nCntLoop;		//ループした回数計測
//
//	static std::vector<std::unique_ptr<CreatorWave>> m_pCreatorParamList;
//	static FILENAME_LIST m_aFileNameList;
//};
#endif
