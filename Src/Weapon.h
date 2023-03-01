//=============================================================================
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

#include "WeaponShot.h"
#include "WeaponRock.h"

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
