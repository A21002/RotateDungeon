//=============================================================================
//      武器の処理
//																Weapon.cpp
//=============================================================================

#include "Weapon.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//=============================================================================================================================
//
// 武器メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
// ----------------------------------------------------------------------------------------------------------------------------
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pWeaponShotProc = new CWeaponShotProc(m_pGMain);	// 武器・ショットプロシージャ
	m_pWeaponRockProc = new CWeaponRockProc(m_pGMain);	// 爆弾プロシージャ
	m_pProcArray.push_back(m_pWeaponShotProc);     // プロシージャをプロシージャ配列に登録する
	m_pProcArray.push_back(m_pWeaponRockProc);
} 

void CWeaponProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}

void CWeaponProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}
