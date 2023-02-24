//=============================================================================
//		�X���C���̏���
//																EnamySlime.cpp
//=============================================================================
#include "EnemySlime.h"
#include "Map.h"
#include "Effect.h"
#include "Playchar.h"

//=============================================================================
// �X���C���v���V�[�W��
//=============================================================================
//------------------------------------------------------------------------
//	�X���C���̓G�v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmSlimeProc::CEnmSlimeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_SLIME_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmSlimeObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
		((CEnmSlimeObj*)(m_pObjArray[i]))->CreateChildObj();
	}

}

// ---------------------------------------------------------------------------
// �X���C���̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmSlimeProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�@ ( EvtID:3�@�G�̏o���ʒu,     �@EvtNo:0x02�@�X���C���̓G  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x02, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}
		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}
}

//------------------------------------------------------------------------
// �S�ẴI�u�W�F�N�g����]��̍��W�ɃZ�b�g����
//------------------------------------------------------------------------
void CEnmSlimeProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
		}
	}
}

//------------------------------------------------------------------------
// �S�ẴI�u�W�F�N�g����]������
// 
// ����
// DWORD	flag
// UP	: �E��]
// DOWN	: ����]
//------------------------------------------------------------------------
void CEnmSlimeProc::RotateCenter(DWORD flag) {
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

//=============================================================================
// �X���C���I�u�W�F�N�g
//=============================================================================
//------------------------------------------------------------------------
//	�X���C���̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmSlimeObj::CEnmSlimeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 0, ENM_SLIME_WIDTH, ENM_SLIME_HEIGHT);
	m_pHpBarObj = NULL;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_SLIME_HP;
	m_nAtc = ENM_SLIME_ATK;
	m_nAnimNum = 7;
}

// ---------------------------------------------------------------------------
// �X���C���̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmSlimeObj::~CEnmSlimeObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// ---------------------------------------------------------------------------
// �q�I�u�W�F�N�g�FHP�o�[�̐���
// ---------------------------------------------------------------------------
void CEnmSlimeObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmSlimeObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// �X���C���̓G�I�u�W�F�N�g�̊J�n
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
BOOL	CEnmSlimeObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);

	m_nHp = m_nMaxHp;

	// Hp�o�[
	m_pHpBarObj->m_bActive = TRUE;
	m_pHpBarObj->ResetStatus();
	m_pHpBarObj->ResetAnim();
	m_pHpBarObj->m_vPos = m_vPos;
	m_pHpBarObj->m_vPosUp = VECTOR2(0, 0);
	m_pHpBarObj->m_vOf = VECTOR2(0, -20);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 50, 6);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;
}

//-----------------------------------------------------------------------------
// �X���C���̓G�I�u�W�F�N�g�̍X�V
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmSlimeObj::Update()
{
	CMapLine* pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	if (m_pGMain->m_moveFlag) {
		if (m_bActive)
		{
			m_vPosUp = VECTOR2(0, 0);

			switch (m_dwStatus)
			{
			case  FLASH:
				m_nCnt1--;
				if (m_nCnt1 <= 0) {
					ResetStatus();
					ResetAnim();
					m_pHpBarObj->ResetStatus();
					m_pHpBarObj->ResetAnim();
				}
				else {
					FlashCountup();
					m_pHpBarObj->FlashCountup();
				}		// break;����ꂸ���̂܂܁@NORMAL�@�̏������s��
			case  NORMAL:
				switch (m_dwStatusSub)
				{
				case  WALK:
					// ���R�ړ��̏���
					if (m_nDirIdx == RIGHT)
					{
						m_vPosUp.x = fSpeed;
					}
					else {
						m_vPosUp.x = -fSpeed;
					}

					// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if ((!bRet && (m_nAnimIdx != 2 && m_nAnimIdx != 3 && m_nAnimIdx != 4)) || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
					{
						m_fJumpTime++;
						// �}�b�v���̒[�ɗ����̂ō��E���]
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -fSpeed;
							m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = fSpeed;
							m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
						}
					}
					else {
						m_fJumpTime = 0;
					}

					// �W�����v����
					if (m_nAnimIdx == ENM_SLIME_JUMP_ANIM_START) {
						m_vPosUp.y = -1;
					}
					else if (m_nAnimIdx == ENM_SLIME_JUMP_ANIM_FIN) {
						m_vPosUp.y = 1;
					}
					break;
				}
				// ����
				m_vPos += m_vPosUp;
				m_pHpBarObj->m_vPos = m_vPos;
				// �o�b�Ƃ̂����蔻��
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case  DAMAGE:
				m_nHp -= m_pOtherObj->GetAtc();
				if (m_nHp <= 0) {
					m_dwStatus = DEAD;
					m_pHpBarObj->m_bActive = FALSE;
					m_nCnt1 = 180;
				}
				else {
					m_dwStatus = FLASH;
					m_pHpBarObj->m_dwStatus = FLASH;
					m_nCnt1 = 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pHpBarObj->m_bActive = FALSE;
					if (0 == Random(0, 2)) {
						m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos, ITEMRECOVER);
					}
				}
				else {
					FlashCountup();
				}
				break;
			}

			AnimCountup();
			m_pHpBarObj->AnimCountup();

		}
		FLOAT h = (float)m_nHp / (float)m_nMaxHp;
		m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(50.0 * h), 6);
		m_pHpBarObj->Draw();
	}

	Draw();
}

// ---------------------------------------------------------------------------
// ��]�p�ɉ����āA��]��̍��W���Z�b�g����
// ---------------------------------------------------------------------------
void CEnmSlimeObj::RotatePos(DWORD rotate) {
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
}
