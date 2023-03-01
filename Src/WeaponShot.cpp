//=============================================================================
//      武器の処理
//																WeaponShot.cpp
//=============================================================================

#include "WeaponShot.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//------------------------------------------------------------------------
//
//	武器・ショットプロシージャのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CWeaponShotProc::CWeaponShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 武器・ショット
	for (DWORD i = 0; i < WEAPON_SHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotObj(m_pGMain));	// m_pObjArrayに武器・ショットオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = 8;

}

//-----------------------------------------------------------------------------
// 武器・ショットプロシージャの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　正常に発生 : TRUE    発生しなかった : FALSE
//-----------------------------------------------------------------------------
BOOL	CWeaponShotProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj, dwOwner);	// 武器の発生
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	if (bRet) m_pGMain->m_pSeShot->Play();

	//  (注意)m_nWaitTimeのカウントダウンは、CBaseProcで行っている

	return bRet;
}


//------------------------------------------------------------------------
//
//	武器・ショットオブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CWeaponShotObj::CWeaponShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageWeapon, 1, 1, 38, 16);
	m_nAnimNum = 2;

	m_nAtc = WEAPON_SHOT_ATK;
}
// ---------------------------------------------------------------------------
//
// 武器・ショットオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponShotObj::~CWeaponShotObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// 武器・ショットオブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL	CWeaponShotObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	float fSpeed = WEAPON_SHOT_SPEED;	// 弾の速さ

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

	switch (pObj->GetDirIdx())
	{
	case UP:
		m_vPosUp = VECTOR2(0, -fSpeed);
		break;
	case DOWN:
		m_vPosUp = VECTOR2(0, fSpeed);
		break;
	case RIGHT:
		m_vPosUp = VECTOR2(fSpeed, 0);
		m_nDirIdx = LEFT;
		break;
	case LEFT:
		m_vPosUp = VECTOR2(-fSpeed, 0);
		m_nDirIdx = RIGHT;
		break;
	}
	//m_nDirIdx = m_pGMain->m_pPcProc->GetPcObjPtr()->GetDirIdx();

	return TRUE;

}
//-----------------------------------------------------------------------------
// 武器・ショットオブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CWeaponShotObj::Update()
{
	CMapLine* pHitmapline = NULL;
	BOOL bRet;
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			if (m_dwOwner == PC) // PCが発射した弾
			{
				m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			}
			else { //  敵が発射した弾
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			}

			bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT ||
				bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f)) {	// 画面外へ出たので消去
				m_bActive = FALSE;
			}
			break;

		case  DAMAGE:
			m_bActive = FALSE;
			break;
		}

		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();

	}
}
