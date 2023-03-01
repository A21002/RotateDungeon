//=============================================================================
//      武器の処理
//																WeaponRock.cpp
//=============================================================================

#include "WeaponRock.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//------------------------------------------------------------------------
//
//	岩プロシージャのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CWeaponRockProc::CWeaponRockProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 武器・ショット
	for (DWORD i = 0; i < WEAPON_ROCK_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponRockObj(m_pGMain));	// m_pObjArrayに武器・ショットオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = 8;

}

//-----------------------------------------------------------------------------
// 岩プロシージャの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　正常に発生 : TRUE    発生しなかった : FALSE
//-----------------------------------------------------------------------------
BOOL	CWeaponRockProc::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
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
//	岩オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CWeaponRockObj::CWeaponRockObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageWeapon, 0, 34, 64, 63);
	m_nAnimNum = 2;

	m_nAtc = WEAPON_ROCK_ATK_PC;
	m_nEnmAtc = WEAPON_ROCK_ATK_ENM;
}

// ---------------------------------------------------------------------------
//
// 岩オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponRockObj::~CWeaponRockObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// 岩オブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//   CBaseObj*   pObj    発射元のオブジェクト
//   DWORD       dwOwner 発射元のオブジェクト区分
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL	CWeaponRockObj::Start(VECTOR2 vPos, CBaseObj* pObj, DWORD dwOwner)
{
	//float fSpeed = 8;	// 弾の速さ

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_dwOwner = dwOwner;

	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 10);
	if (Random(0, 9) > 5) {
		m_hp = 2;
	}
	else {
		m_hp = 1;
	}

	m_pGMain->m_pFall->Play();

	return TRUE;

}

//-----------------------------------------------------------------------------
// 岩オブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CWeaponRockObj::Update()
{
	CMapLine* pHitmapline = NULL;
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			// マップ線との接触判定と適切な位置への移動
			if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
			{
				if (pHitmapline && pHitmapline->m_vNormal.y <= 0)	// マップ線が垂直まではＯＫ
				{
					m_dwStatus = STOP;	// マップ線に接触しているときは、ジャンプを歩行に戻す
					m_vPos += m_vPosUp;
					m_vPosUp = VECTOR2(0, 0);
					m_hp--;
					m_fJumpTime = 0;
				}
			}


			// 破壊するか判定
			if (m_hp <= 0) {
				m_dwStatus = DAMAGE;
			}
			m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);
			m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);
			break;

		case  DAMAGE:
			m_pGMain->m_pCrash->Play();
			m_bActive = FALSE;
			break;

		case  STOP:
			m_nAnimNum = 1;
			if (m_hp <= 0) {
				m_dwStatus = DAMAGE;
			}
			break;
		}

		m_vPos += m_vPosUp;

		AnimCountup();
		Draw();

	}
}

void CWeaponRockProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			if (flag == 1) {
				m_pObjArray[j]->RotateDrawUp();
			}
			else if (flag == 2) {
				m_pObjArray[j]->RotateDrawDown();
			}
			//break;
		}
	}
}

void CWeaponRockObj::RotateDrawUp() {
	// m_vCenterの代入
	m_vCenter.x = 683 - m_vPos.x;
	m_vCenter.y = 384 - m_vPos.y;
	m_fRotate++;
}

void CWeaponRockObj::RotateDrawDown() {
	// m_vCenterの代入
	m_vCenter.x = 683 - m_vPos.x;
	m_vCenter.y = 384 - m_vPos.y;
	m_fRotate--;
}

void CWeaponRockProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CWeaponRockObj::RotatePos(DWORD rotate) {
	DWORD x = m_vPos.x;
	switch (rotate) {
	case 90:
		m_vPos.x = m_vPos.y;
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - x;
		break;

	case 180:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.x - m_pSprite->GetSrcWidth();
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - m_vPos.y - m_pSprite->GetSrcHeight();
		break;

	case 270:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.y - m_pSprite->GetSrcHeight();
		m_vPos.y = x;
	}
	m_fRotate = 0;
	m_dwStatus = NORMAL;
	m_vPosUp = VECTOR2(0, 10);
	m_pGMain->m_pFall->Play();
}