//=============================================================================
//		�R�E�����̏���
//																EnamySlime.cpp
//=============================================================================
#include "EnemyBat.h"
#include "Enemy.h"
#include "Map.h"
#include "Effect.h"
#include "Playchar.h"

//=============================================================================
// �R�E�����v���V�[�W��
//=============================================================================
//------------------------------------------------------------------------
//	�R�E�����̓G�v���V�[�W���̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmBatProc::CEnmBatProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_BAT_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBatObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
		((CEnmBatObj*)(m_pObjArray[i]))->CreateChildObj();
	}
}

// ---------------------------------------------------------------------------
// �R�E�����̓G�v���V�[�W���̎�����������
// ---------------------------------------------------------------------------
void CEnmBatProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// �܂������ł���ꍇ�̂ݔ���������
	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�G�̏o���ʒu��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:3�@�G�̏o���ʒu,  EvtNo:0x04	�R�E�����̓G  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x04, vPos, nNext))
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
void CEnmBatProc::Rotate(DWORD rotate) {
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
void CEnmBatProc::RotateCenter(DWORD flag) {
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
// �R�E�����I�u�W�F�N�g
//=============================================================================
//------------------------------------------------------------------------
//	�R�E�����̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//------------------------------------------------------------------------
CEnmBatObj::CEnmBatObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 96, 
		BatConstruct::IMAGE_WIDTH, BatConstruct::IMAGE_HEIGHT);
	m_pHpBarObj = NULL;
	m_nAnimNum = BatConstruct::ANIM_NUM;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = BatConstruct::HP;
	m_nAtc = BatConstruct::ATK;
}

// ---------------------------------------------------------------------------
// �R�E�����̓G�I�u�W�F�N�g�̃f�X�g���N�^
// ---------------------------------------------------------------------------
CEnmBatObj::~CEnmBatObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// ---------------------------------------------------------------------------
// �q�I�u�W�F�N�g�FHP�o�[�̐���
// ---------------------------------------------------------------------------
void CEnmBatObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmBatObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// �R�E�����̓G�I�u�W�F�N�g�̊J�n
//-----------------------------------------------------------------------------
BOOL	CEnmBatObj::Start(VECTOR2 vPos)
{

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// �����ʒu

	m_vPosUp = VECTOR2(0, 0);
	moveNum = 0;

	m_nHp = m_nMaxHp;

	// Hp�o�[
	m_pHpBarObj->m_bActive = TRUE;
	m_pHpBarObj->ResetStatus();
	m_pHpBarObj->ResetAnim();
	m_pHpBarObj->m_vPos = m_vPos;
	m_pHpBarObj->m_vPosUp = VECTOR2(0, 0);
	m_pHpBarObj->m_vOf = VECTOR2(0, HpBarConstruct::DIFF);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 
		HpBarConstruct::IMAGE_WIDTH, HpBarConstruct::IMAGE_HEIGHT);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;
}

//-----------------------------------------------------------------------------
// �R�E�����̓G�I�u�W�F�N�g�̍X�V
//-----------------------------------------------------------------------------
void	CEnmBatObj::Update()
{
	CMapLine* pHitmapline = NULL;
	BOOL bRet;

	if (m_pGMain->m_moveFlag) {
		if (m_bActive)
		{
			m_vPosUp = VECTOR2(0, 0);

			switch (m_dwStatus)
			{
			case  FLASH:
				// �_���[�W���󂯂��ۂ̖��G����
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
						m_vPosUp.x = BatConstruct::SPEED;
						moveNum += BatConstruct::SPEED;
					}
					else {
						m_vPosUp.x = -BatConstruct::SPEED;
						moveNum += -BatConstruct::SPEED;
					}

					// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) || (moveNum > 200 || moveNum < 0))
					{
						// �}�b�v���̒[�ɗ����̂ō��E���]
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -BatConstruct::SPEED;
							m_vPosUp.y = 0;
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = BatConstruct::SPEED;
							m_vPosUp.y = 0;
						}
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
					// HP��0�ɂȂ�����
					m_dwStatus = DEAD;
					m_pHpBarObj->m_bActive = FALSE;
					m_nCnt1 = BatConstruct::FLASHTIME_DEAD * 60;
				}
				else {
					m_dwStatus = FLASH;
					m_pHpBarObj->m_dwStatus = FLASH;
					m_nCnt1 = BatConstruct::FLASHTIME_DAMAGE * 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				// �|�ꂽ�ۂ̏���
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pHpBarObj->m_bActive = FALSE;

					// �񕜃p�[�e�B�N���̏o��
					for (int i = 0; i < 10; i++) {
						m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos, ITEMRECOVER);
					}
				}
				else {
					FlashCountup();
				}
				break;
			}

			AnimCountup();

			// HP�o�[�̕\��
			FLOAT h = (float)m_nHp / (float)m_nMaxHp;
			m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(HpBarConstruct::IMAGE_WIDTH * h), HpBarConstruct::IMAGE_HEIGHT);
			m_pHpBarObj->Draw();
		}
		Draw();

	}
}

// ---------------------------------------------------------------------------
// ��]�p�ɉ����āA��]��̍��W���Z�b�g����
// ---------------------------------------------------------------------------
void CEnmBatObj::RotatePos(DWORD rotate) {
	DWORD x = m_vPos.x;
	switch (rotate) {
	case 90:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.y - m_pSprite->GetSrcHeight();
		m_vPos.y = x;
		break;

	case 180:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.x - m_pSprite->GetSrcWidth();
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - m_vPos.y - m_pSprite->GetSrcHeight();
		break;

	case 270:
		m_vPos.x = m_vPos.y;
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - x;
		break;
	}
	m_fRotate = 0;
}
