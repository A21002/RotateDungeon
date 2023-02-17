//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�G�̏���
//																Enamy.cpp
//=============================================================================

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
//
// �G���C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pEnmSlimeProc = new CEnmSlimeProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmSlimeProc);   // �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pEnmBatProc = new CEnmBatProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmBatProc);   // �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pEnmMoleProc = new CEnmMoleProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmMoleProc);


}

//============================================================================
//   �G�v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
//   �G�v���V�[�W���̊J�n�t���O�����Z�b�g����
//============================================================================
void  CEnmProc::SetNonActive()
{

	// �G�̃I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag( 0 );				// �J�n�t���O�̃��Z�b�g
		((CEnmProc*)m_pProcArray[i])->SetNonActive();   // ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}

void CEnmProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}

void CEnmProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}

// =====================================================================================================
// 
// �e�G�v���V�[�W�����̏���
// 
// =====================================================================================================

// ===================================================================================================

//------------------------------------------------------------------------
//
//	�X���C���̓G�v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
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
//
// �X���C���̓G�v���V�[�W���̎�����������
//
//-
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
		while ( nNext != -1 )
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

	//  (����)m_nWaitTime�̃J�E���g�_�E���́ACBaseProc�ōs���Ă���

}

void CEnmSlimeProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CEnmSlimeProc::RotateCenter(DWORD flag) {
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

//------------------------------------------------------------------------
//
//	�X���C���̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CEnmSlimeObj::CEnmSlimeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 0, ENM_SLIME_WIDTH, ENM_SLIME_HEIGHT);
	m_pHpBarObj = NULL;
	m_vPos = VECTOR2(0,0);
	m_nHp = m_nMaxHp = ENM_SLIME_HP;
	m_nAtc = ENM_SLIME_ATK;
	m_nAnimNum = 7;
}
// ---------------------------------------------------------------------------
//
// �X���C���̓G�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CEnmSlimeObj::~CEnmSlimeObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// �q�I�u�W�F�N�g�FHP�o�[
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
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;
	
	if(m_pGMain->m_moveFlag){
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
					if ((!bRet && (m_nAnimIdx != 2 && m_nAnimIdx != 3 && m_nAnimIdx != 4)) || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) )
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
					m_nCnt1 = 180;				}
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

// ===================================================================================================

//------------------------------------------------------------------------
//
//	�R�E�����̓G�v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
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
//
// �R�E�����̓G�v���V�[�W���̎�����������
//
//
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

	//  (����)m_nWaitTime�̃J�E���g�_�E���́ACBaseProc�ōs���Ă���

}

void CEnmBatProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CEnmBatProc::RotateCenter(DWORD flag) {
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

//------------------------------------------------------------------------
//
//	�R�E�����̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CEnmBatObj::CEnmBatObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 96, ENM_BAT_WIDTH, ENM_BAT_HEIGHT);
	m_pHpBarObj = NULL;
	m_nAnimNum = 2;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_BAT_HP;
	m_nAtc = ENM_BAT_ATK;

}
// ---------------------------------------------------------------------------
//
// �R�E�����̓G�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CEnmBatObj::~CEnmBatObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// �q�I�u�W�F�N�g
void CEnmBatObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmBatObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// �R�E�����̓G�I�u�W�F�N�g�̊J�n
//
//   �����@�@�@�Ȃ�
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
	m_pHpBarObj->m_vOf = VECTOR2(0, -20);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 50, 6);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;

}
//-----------------------------------------------------------------------------
// �R�E�����̓G�I�u�W�F�N�g�̍X�V
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmBatObj::Update()
{
	CMapLine*  pHitmapline = NULL;
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
						moveNum += fSpeed;
					}
					else {
						m_vPosUp.x = -fSpeed;
						moveNum += -fSpeed;
					}

					// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) || (moveNum > 200 || moveNum < 0))
					{
						// �}�b�v���̒[�ɗ����̂ō��E���]
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -fSpeed;
							m_vPosUp.y = 0;
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = fSpeed;
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
			FLOAT h = (float)m_nHp / (float)m_nMaxHp;
			m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(50.0 * h), 6);
			m_pHpBarObj->Draw();
		}
	Draw();

	}
}

void CEnmBatObj::RotatePos(DWORD rotate) {
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

//------------------------------------------------------------------------
//
//	���O���̓G�v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CEnmMoleProc::CEnmMoleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_MOLE_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmMoleObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}

}

// ---------------------------------------------------------------------------
//
// ���O���̓G�v���V�[�W���̎�����������
//
//-
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

	//  (����)m_nWaitTime�̃J�E���g�_�E���́ACBaseProc�ōs���Ă���

}

//------------------------------------------------------------------------
//
//	���O���̓G�I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
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
//
// ���O���̓G�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CEnmMoleObj::~CEnmMoleObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// ���O���̓G�I�u�W�F�N�g�̊J�n
//
//   �����@�@�@�Ȃ�
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
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmMoleObj::Update()
{
	CMapLine*  pHitmapline = NULL;
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

CBaseObj* CEnmMoleProc::GetMoleObj() {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			return m_pObjArray[j];
		}
	}
}