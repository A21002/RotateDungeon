//=============================================================================
//      ����̏���
//																WeaponRock.cpp
//=============================================================================

#include "WeaponRock.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//------------------------------------------------------------------------
//	��v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CWeaponRockProc::CWeaponRockProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ����E�V���b�g
	for (DWORD i = 0; i < WEAPON_ROCK_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponRockObj(m_pGMain));	// m_pObjArray�ɕ���E�V���b�g�I�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = 8;
}

//-----------------------------------------------------------------------------
// ��v���V�[�W���̊J�n
//-----------------------------------------------------------------------------
BOOL	CWeaponRockProc::Start(VECTOR2 vPos, CBaseObj* pObj)
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
// �S�ẴI�u�W�F�N�g����]��̍��W�ɃZ�b�g����
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
//	��I�u�W�F�N�g�̃R���X�g���N�^	
//------------------------------------------------------------------------
CWeaponRockObj::CWeaponRockObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageWeapon, 0, 34, 64, 63);
	m_nAnimNum = 2;

	m_nAtc = WEAPON_ROCK_ATK_PC;
	m_nEnmAtc = WEAPON_ROCK_ATK_ENM;
	m_nHp = 0;
}

// ---------------------------------------------------------------------------
// ��I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CWeaponRockObj::~CWeaponRockObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ��I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   CBaseObj*   pObj    ���ˌ��̃I�u�W�F�N�g
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL	CWeaponRockObj::Start(VECTOR2 vPos, CBaseObj* pObj)
{
	//float fSpeed = 8;	// �e�̑���

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();

	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 10);
	if (Random(0, 9) > 5) {
		m_nHp = 2;
	}
	else {
		m_nHp = 1;
	}

	m_pGMain->m_pFall->Play();

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��I�u�W�F�N�g�̍X�V
//-----------------------------------------------------------------------------
void	CWeaponRockObj::Update()
{
	CMapLine* pHitmapline = NULL;
	if (m_bActive)
	{
		switch (m_dwStatus)
		{
		case  NORMAL:
			// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
			if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
			{
				if (pHitmapline && pHitmapline->m_vNormal.y <= 0)	// �}�b�v���������܂ł͂n�j
				{
					m_dwStatus = STOP;	// �}�b�v���ɐڐG���Ă���Ƃ��́A�W�����v����s�ɖ߂�
					m_vPos += m_vPosUp;
					m_vPosUp = VECTOR2(0, 0);
					m_nHp--;
					m_fJumpTime = 0;
				}
			}


			// �j�󂷂邩����
			if (m_nHp <= 0) {
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
			if (m_nHp <= 0) {
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
			if (flag == UP) {
				m_pObjArray[j]->RotateDrawUp();
			}
			else if (flag == DOWN) {
				m_pObjArray[j]->RotateDrawDown();
			}
		}
	}
}

void CWeaponRockObj::RotateDrawUp() {
	// m_vCenter�̑��
	m_vCenter.x = 683 - m_vPos.x;
	m_vCenter.y = 384 - m_vPos.y;
	m_fRotate++;
}

void CWeaponRockObj::RotateDrawDown() {
	// m_vCenter�̑��
	m_vCenter.x = 683 - m_vPos.x;
	m_vCenter.y = 384 - m_vPos.y;
	m_fRotate--;
}


// ---------------------------------------------------------------------------
// ��]�p�ɉ����āA��]��̍��W���Z�b�g����
// ---------------------------------------------------------------------------
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