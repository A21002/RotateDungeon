//=============================================================================
//		���O���̏���
//																EnamySlime.cpp
//=============================================================================
#include "EnemyMole.h"
#include "Map.h"
#include "Effect.h"
#include "Weapon.h"
#include "Playchar.h"

//=============================================================================
// ���O���v���V�[�W��
//=============================================================================
//------------------------------------------------------------------------
//	���O���̓G�v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmMoleProc::CEnmMoleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_MOLE_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmMoleObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}
}

// ---------------------------------------------------------------------------
// ���O���̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmMoleProc::Update()
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
			// �C�x���g�}�b�v�@ ( EvtID:3�@�G�̏o���ʒu,     �@EvtNo:0x02�@���O���̓G  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x08, vPos, nNext))
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

//=============================================================================
// ���O���I�u�W�F�N�g
//=============================================================================
//------------------------------------------------------------------------
//	���O���̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmMoleObj::CEnmMoleObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageBoss, 0, 0, ENM_MOLE_WIDTH, ENM_MOLE_HEIGHT);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_MOLE_HP;
	m_nAtc = ENM_MOLE_ATK;
	m_nAnimNum = 3;
}

// ---------------------------------------------------------------------------
// ���O���̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmMoleObj::~CEnmMoleObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ���O���̓G�I�u�W�F�N�g�̊J�n
//-----------------------------------------------------------------------------
BOOL	CEnmMoleObj::Start(VECTOR2 vPos)
{

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// �����ʒu
	m_dwStatus = SMOKE;

	m_vPosUp = VECTOR2(0, 0);
	m_AnimCount = 120;
	m_waitTime = 0;

	m_nHp = m_nMaxHp;
	m_pGMain->m_pDig->Play();
	speed = 8;

	m_pGMain->m_BossSpownFlag = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O���̓G�I�u�W�F�N�g�̍X�V
//-----------------------------------------------------------------------------
void	CEnmMoleObj::Update()
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
					m_dwStatus = NORMAL;
					m_nFlashIdx = 0;		// �t���b�V���A�j���[�V�����C���f�b�N�X��
					m_nFlashFrm = 0;		// �t���b�V���A�j���[�V�����t���[���J�E���g
				}
				else {
					FlashCountup();
				}		// break;����ꂸ���̂܂܁@NORMAL�@�̏������s��
			case  NORMAL:
				switch (m_dwStatusSub)
				{
				case  APPEAR:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						m_pSprite->SetSrc(0, 336, 318, 168);
						m_dwStatusSub = IDLE;
						m_nAnimNum = 2;
						m_nAnimIdx = 0;
					}

					// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
					{
						m_fJumpTime++;
						m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
					}
					else {
						m_fJumpTime = 0;
					}

					break;

				case  IDLE:
					if (Random(0, 100) == 0) {

						m_dwStatusSub = DIVE;
						m_pSprite->SetSrc(0, 1008, 318, 168);
						m_nAnimNum = 7;
						m_nAnimIdx = 0;

					}
					break;



				case  DIVE:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						if (Random(0, 1) == 0) {
							m_pSprite->SetSrc(0, 168, 318, 168);
							m_dwStatus = SHOT;
							m_nAnimNum = 3;
							m_nAnimIdx = 0;
							m_nDirIdx = LEFT;
							m_ShotNum = Random(10, 15);
							for (int i = 0; i < m_ShotNum; i++) {
								m_SHotPosX[i] = Random(128, 1237 - m_pSprite->GetSrcWidth());
							}
							m_RotateItemPos = Random(0, m_ShotNum - 1);
							m_vPos = VECTOR2(1237 - m_pSprite->GetSrcWidth(), 96);
						}
						else {
							m_pSprite->SetSrc(0, 1344, 318, 168);
							m_dwStatusSub = JUMP;
							m_nAnimNum = 10;
							m_nAnimIdx = 0;
							if (m_nDirIdx == LEFT) {
								m_vPos.x = 1237 - m_pSprite->GetSrcWidth();

							}
							else {
								m_vPos.x = 128;

							}
						}
					}

					break;

				case JUMP:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						m_pSprite->SetSrc(0, 0, 0, 0);
						m_vPos.y = 0;
						m_AnimCount = 45;
					}
					if (m_AnimCount <= 0 && m_pSprite->GetSrcWidth() == 0) {
						m_pSprite->SetSrc(0, 1344, 318, 168);
						m_vPos.y = 503;
						m_nAnimIdx = 0;
					}
					else {
						if (m_AnimCount > 0) {
							m_AnimCount--;
						}
					}
					if (m_nDirIdx == LEFT) {
						m_vPosUp = VECTOR2(-4, 0);
						if (m_vPos.x <= 128) {
							m_dwStatus = SMOKE;
							m_pSprite->SetSrc(0, 0, 318, 168);
							m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
							m_vPos.y = 572;
							m_vPosUp.x = 0;
							m_nAnimNum = 3;
							m_AnimCount = 60;
							m_pGMain->m_pDig->Play();
						}
					}
					else {
						m_vPosUp = VECTOR2(4, 0);
						if (m_vPos.x >= 1237 - m_pSprite->GetSrcWidth()) {
							m_dwStatus = SMOKE;
							m_nDirIdx = LEFT;
							m_pSprite->SetSrc(0, 0, 318, 168);
							m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
							m_vPos.y = 572;
							m_vPosUp.x = 0;
							m_nAnimNum = 3;
							m_AnimCount = 60;
							m_pGMain->m_pDig->Play();
						}
					}
				}

				// �o�b�Ƃ̂����蔻��
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case SMOKE:
				m_nFlashIdx = 0;		// �t���b�V���A�j���[�V�����C���f�b�N�X��
				m_nFlashFrm = 0;		// �t���b�V���A�j���[�V�����t���[���J�E���g
				m_nCnt1 = 0;
				// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					m_fJumpTime++;
					m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
				}
				else {
					m_fJumpTime = 0;
				}

				if (m_AnimCount <= 0) {
					m_pSprite->SetSrc(0, 672, 318, 168);
					m_nAnimNum = 7;
					m_nAnimIdx = 0;
					m_dwStatus = NORMAL;
					m_dwStatusSub = APPEAR;
					if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x > m_vPos.x) {
						m_nDirIdx = RIGHT;
					}
					else {
						m_nDirIdx = LEFT;
					}
				}
				else {
					m_AnimCount--;
				}
				break;

			case  SHOT:
				m_nFlashIdx = 0;		// �t���b�V���A�j���[�V�����C���f�b�N�X��
				m_nFlashFrm = 0;		// �t���b�V���A�j���[�V�����t���[���J�E���g
				m_nCnt1 = 0;
				m_vPosUp.x = -3;
				for (int i = 0; i < m_ShotNum; i++) {
					if (m_SHotPosX[i] == m_vPos.x || m_SHotPosX[i] == m_vPos.x - 1 || m_SHotPosX[i] == m_vPos.x + 1) {
						if (m_RotateItemPos == i) {
							m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos + VECTOR2(0, 100), ITEMROTATEDOWN180);
						}
						else {
							m_pGMain->m_pWeaponProc->m_pWeaponRockProc->Start(m_vPos + VECTOR2(0, 100), this, ENM);
						}
					}
				}

				if (m_vPos.x <= 128) {
					m_dwStatus = SMOKE;
					m_pSprite->SetSrc(0, 0, 318, 168);
					m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
					m_vPos.y = 604;
					m_vPosUp.x = 0;
					m_AnimCount = 60;
					m_pGMain->m_pDig->Play();
				}
				break;


			case  DAMAGE:
				if (m_pOtherObj->GetEnmAtc() == 1000) {
					m_nHp -= m_pOtherObj->GetEnmAtc();
				}
				else {
					m_nHp -= m_pOtherObj->GetAtc();
				}
				if (m_nHp <= 0) {
					m_dwStatus = DEAD;
					m_nCnt1 = 180;
				}
				else {
					m_dwStatus = FLASH;
					m_nCnt1 = 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pGMain->m_BossSpownFlag = FALSE;
					m_pGMain->m_dwGameStatus = GAMECLEAR;
				}
				else {
					FlashCountup();
				}
				break;
			}
			if (m_waitTime > 0) {
				m_waitTime--;
			}
			// ����
			m_vPos += m_vPosUp;
			AnimCountup();

		}
	}
	Draw();
}

//-----------------------------------------------------------------------------
// ���O���I�u�W�F�N�g�̃Q�b�^�[
//-----------------------------------------------------------------------------
CBaseObj* CEnmMoleProc::GetMoleObj() {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			return m_pObjArray[j];
		}
	}
}