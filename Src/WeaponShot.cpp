//=============================================================================
//      ����̏���
//																WeaponShot.cpp
//=============================================================================

#include "WeaponShot.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//------------------------------------------------------------------------
//	����E�V���b�g�v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CWeaponShotProc::CWeaponShotProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ����E�V���b�g
	for (DWORD i = 0; i < WEAPON_SHOT_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponShotObj(m_pGMain));	// m_pObjArray�ɕ���E�V���b�g�I�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = 8;

}

//-----------------------------------------------------------------------------
// ����E�V���b�g�v���V�[�W���̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   CBaseObj*   pObj    ���ˌ��̃I�u�W�F�N�g
//   DWORD       dwOwner ���ˌ��̃I�u�W�F�N�g�敪
//
//   �߂�l�@�@����ɔ��� : TRUE    �������Ȃ����� : FALSE
//-----------------------------------------------------------------------------
BOOL	CWeaponShotProc::Start(VECTOR2 vPos, CBaseObj* pObj)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos, pObj);	// ����̔���
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}
	if (bRet) m_pGMain->m_pSeShot->Play();

	return bRet;
}

//------------------------------------------------------------------------
//	����E�V���b�g�I�u�W�F�N�g�̃R���X�g���N�^	
//------------------------------------------------------------------------
CWeaponShotObj::CWeaponShotObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageWeapon, 1, 1, 38, 16);
	m_nAnimNum = 2;

	m_nAtc = WEAPON_SHOT_ATK;
}

// ---------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponShotObj::~CWeaponShotObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̊J�n
//-----------------------------------------------------------------------------
BOOL	CWeaponShotObj::Start(VECTOR2 vPos, CBaseObj* pObj)
{
	float fSpeed = WEAPON_SHOT_SPEED;	// �e�̑���

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();

	m_vPos = vPos;	// �����ʒu

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

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����E�V���b�g�I�u�W�F�N�g�̍X�V
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
			m_pGMain->m_pEnmProc->Hitcheck((CBaseObj*)this);

			bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
			if (m_vPos.x - m_pGMain->m_vScroll.x < 0 || m_vPos.x - m_pGMain->m_vScroll.x > WINDOW_WIDTH ||
				m_vPos.y - m_pGMain->m_vScroll.y < 0 || m_vPos.y - m_pGMain->m_vScroll.y > WINDOW_HEIGHT ||
				bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f)) {	// ��ʊO�֏o���̂ŏ���
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