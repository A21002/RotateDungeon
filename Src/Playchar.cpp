//=============================================================================
//		�v���C�L�����N�^�[�̏���
//																Playchar.cpp
//=============================================================================

#include "Playchar.h"
#include "Weapon.h"
#include "Effect.h"
#include "Map.h"

// ===========================================================================
// PC�萔��`
// ===========================================================================
float const PcConstruct::SHOT_WAIT_SECOND = 0.2f;

// ===========================================================================
// PC�v���V�[�W��
// ===========================================================================
//------------------------------------------------------------------------
//
//	�o�b�v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	
	m_pObjArray.push_back(new CPcObj(m_pGMain));  // m_pObjArray�ɃI�u�W�F�N�g��o�^����

	m_nPcNum = 1;
}

// ===========================================================================
// PC�I�u�W�F�N�g
// ===========================================================================
//------------------------------------------------------------------------
//
//	�o�b�I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@CGameMain* pGMain
//
//------------------------------------------------------------------------
CPcObj::CPcObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 0, 0, PcConstruct::IMAGE_WIDTH, PcConstruct::IMAGE_HEIGHT);
	m_vPos = VECTOR2(0.0f, 0.0f);
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_nDirIdx = RIGHT;
	m_nHp = m_nMaxHp = PcConstruct::MAX_HP;
	m_nAtc = PcConstruct::ATC;

}
// ---------------------------------------------------------------------------
//
// �o�b�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CPcObj::~CPcObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �o�b�I�u�W�F�N�g�̍X�V
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CMapLine*  pHitmapline = NULL;
	float fSpeed     = PcConstruct::SPEED_MOVE;
	float fJumpPlus  = PcConstruct::SPEED_JUMP;
	BOOL fMoveFlag = FALSE;

	if (!m_bActive) return;

	// �ړ������̏�����
	m_vPosUp = VECTOR2(0, 0);

	// ��]�������A���b�Z�[�W�_�C�A���O�łȂ���Έړ��������s��
	if (m_pGMain->m_moveFlag) {
		switch (m_dwStatus)
		{
		case  FLASH:			// ���G����
			m_nCnt1--;
			if (m_nCnt1 <= 0) {
				ResetStatus();
				ResetAnim();
			}
			else {
				FlashCountup();
			}		// break;����ꂸ���̂܂܁@NORMAL�@�̏������s��
		case  NORMAL:

			break;

		case DAMAGE:			// �_���[�W����
			if (m_pOtherObj->GetAtc() > 0)
			{
				m_nHp -= m_pOtherObj->GetAtc();	// �U�����󂯂��_���[�W
				if (m_nHp <= 0)
				{
					m_nHp = 0;
					m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
					m_nCnt1 = PcConstruct::DEAD_SECOND * 60;			// ���S�t���b�V�����̎��Ԑݒ�
				}
				else {
					m_dwStatus = FLASH;
					m_nCnt1 = PcConstruct::FLASH_SECOND * 60;
				}
			}
			else {
				m_dwStatus = NORMAL;  // �U���͂Ȃ��̂Ƃ��͂�����NORMAL�ɖ߂�
			}
			break;

		case DEAD:
			m_pGMain->m_dwGameStatus = GAMEOVER;
			
			break;

		}

		switch (m_dwStatusSub)
		{
		case  WALK:		// ���s���̏���
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//���L�[
			{
				fMoveFlag = TRUE;
				m_vPosUp.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//���L�[
			{
				fMoveFlag = TRUE;
				m_vPosUp.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_SPACE) || pDI->CheckJoy(KD_DAT, DIJ_UP)) //	SPACE�L�[
			{	// �W�����v�J�n
				fMoveFlag = TRUE;
				m_dwStatusSub = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_fJumpTime = 0;
				m_vPosUp.y = m_vJumpSpeed.y;
			}
			else {// ���R����
				m_dwStatusSub = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = fSpeed / 2;
				m_fJumpTime = 0;
				m_vPosUp.y = m_vJumpSpeed.y;
			}
			break;

		case  JUMP:		// �W�����v���̏���
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//���L�[
			{
				m_vJumpSpeed.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//���L�[
			{
				m_vJumpSpeed.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			fMoveFlag = TRUE;
			m_fJumpTime++;
			m_vPosUp.x = m_vJumpSpeed.x;
			m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
			break;
		}

		if (pDI->CheckKey(KD_DAT, DIK_X) || pDI->CheckJoy(KD_DAT, DIJ_A))			// �U��
		{
			if (m_nShotWait <= 0)
			{
				// �V���b�g�̔���
				m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos + VECTOR2(16, 16), this);
				m_nShotWait = PcConstruct::SHOT_WAIT_SECOND * 60;    // ���픭�˂̃E�F�C�g���|����
			}
		}

		if (m_nShotWait > 0) m_nShotWait--;    // ���픭�˂̃E�F�C�g���J�E���g�_�E��

		// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
		if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
		{
			if (pHitmapline && pHitmapline->m_vNormal.y <= 0)	// �}�b�v���������܂ł͂n�j
			{
				m_dwStatusSub = WALK;	// �}�b�v���ɐڐG���Ă���Ƃ��́A�W�����v����s�ɖ߂�
				m_fJumpTime = 0;
				m_vJumpSpeed.x = m_vJumpSpeed.y = 0;
			}
		}
		// �����v�Z
		m_vPos += m_vPosUp;

		// �ړ����Ă���΃A�j���[�V�����Đ�
		if (fMoveFlag) {
			AnimCountup();
		}
		
	}
	Draw();
	

}
