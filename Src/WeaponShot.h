//=============================================================================
//      武器の処理
//																WeaponShot.h
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

#define  WEAPON_SHOT_MAX  200
#define  WEAPON_SHOT_ATK 100
#define  WEAPON_SHOT_SPEED 8

//======================================================================
// 武器・ショット　オブジェクトクラス
//======================================================================
class CWeaponShotObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// コンストラクタ
	CWeaponShotObj(CGameMain* pGMain);
	~CWeaponShotObj();

};

//======================================================================
// 武器・ショット　プロシージャクラス
//======================================================================
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	CWeaponShotProc(CGameMain* pGMain);	// コンストラクタ
	~CWeaponShotProc() { ; }

};