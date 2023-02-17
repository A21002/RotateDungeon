//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//      武器の処理
//																Weapon.h
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
#define  WEAPON_ROCK_MAX  100
#define  WEAPON_TEXTUI_MAX  1

#define  WEAPON_SHOT_ATK 100
#define  WEAPON_SHOT_SPEED 8

#define  WEAPON_ROCK_ATK_PC 300
#define  WEAPON_ROCK_ATK_ENM 1000



//======================================================================
// 武器・ショット　オブジェクトクラス
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
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start( VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	CWeaponShotProc(CGameMain* pGMain);	// コンストラクタ
	~CWeaponShotProc() { ; }

};

//======================================================================
// 岩　オブジェクトクラス
class CWeaponRockObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;
	DWORD  m_hp;
	VECTOR2 m_top;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	void RotatePos(DWORD) override;
	void RotateDrawUp() override;
	void RotateDrawDown() override;

	// コンストラクタ
	CWeaponRockObj(CGameMain* pGMain);
	~CWeaponRockObj();

};

//======================================================================
// 岩　プロシージャクラス
class CWeaponRockProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	CWeaponRockProc(CGameMain* pGMain);	// コンストラクタ
	~CWeaponRockProc() { ; }

};

//======================================================================
// 武器全体のメインプロシージャクラス
//======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponShotProc*	m_pWeaponShotProc;
	CWeaponRockProc*    m_pWeaponRockProc;

public:
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	CWeaponProc(CGameMain* pGMain);	// コンストラクタ
	~CWeaponProc() { ; }
};
