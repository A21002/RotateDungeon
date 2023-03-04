//=============================================================================
//		スライムの処理
//																EnamySlime.h
//=============================================================================
#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define  ENM_SLIME_MAX  20

//======================================================================
// スライムの敵　オブジェクトクラス
//======================================================================
class CEnmSlimeObj : public CBaseObj
{
protected:
	CEnmSlimeObj* m_pHpBarObj;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;
	void RotatePos(DWORD) override;

	void CreateChildObj();

	// コンストラクタ
	CEnmSlimeObj(CGameMain* pGMain);
	~CEnmSlimeObj();

};

//======================================================================
// スライムの敵　プロシージャクラス
//======================================================================
class CEnmSlimeProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;
	// コンストラクタ
	CEnmSlimeProc(CGameMain* pGMain);
	~CEnmSlimeProc() { ; }

};

class SlimeConstruct {
public:
	static const int HP = 200;
	static const int ATK = 300;
	static const int SPEED = 1;
	static const int JUMP_POWER = 4;
	static const int JUMP_FLAME = 2;

	static const int IMAGE_WIDTH = 72;
	static const int IMAGE_HEIGHT = 48;

	static const int ANIM_NUM = 7;

	static const int FLASHTIME_DEAD = 3;
	static const int FLASHTIME_DAMAGE = 1;
};