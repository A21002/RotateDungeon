//=============================================================================
//		コウモリの処理
//																Enamy.h
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

#define  ENM_BAT_MAX  20

#define  ENM_BAT_WIDTH 60
#define  ENM_BAT_HEIGHT 51
#define  ENM_BAT_HP 300
#define  ENM_BAT_ATK 200

//======================================================================
// コウモリの敵　オブジェクトクラス
//======================================================================
class CEnmBatObj : public CBaseObj
{
protected:
	CEnmBatObj* m_pHpBarObj;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;
	void RotatePos(DWORD) override;
	void CreateChildObj();

	FLOAT moveNum;

	// コンストラクタ
	CEnmBatObj(CGameMain* pGMain);
	~CEnmBatObj();
};

//======================================================================
// コウモリの敵　プロシージャクラス
//======================================================================
class CEnmBatProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	// コンストラクタ
	CEnmBatProc(CGameMain* pGMain);
	~CEnmBatProc() { ; }
};

class BatConstruct
{
public:
	static const int HP = 300;
	static const int ATK = 200;
	static const int SPEED = 1;

	static const int IMAGE_WIDTH = 60;
	static const int IMAGE_HEIGHT = 51;
	static const int ANIM_NUM = 2;

	static const int FLASHTIME_DEAD = 3;
	static const int FLASHTIME_DAMAGE = 1;
};