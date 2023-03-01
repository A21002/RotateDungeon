//=============================================================================
//		���O���̏���
//																EnamyMole.cpp
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
			// �C�x���g�}�b�v�@ ( EvtID:3�@�G�̏o���ʒu,     �@EvtNo:0x08�@���O���̓G  )
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
	m_pSprite = new CSprite(m_pGMain->m_pImageBoss, 0, 0, 
		MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = MoleConstruct::HP;
	m_nAtc = MoleConstruct::ATK;
	m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
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
	m_AnimCount = MoleConstruct::SMOKE_COUNT;

	m_nHp = m_nMaxHp;
	m_pGMain->m_pDig->Play();
	m_pGMain->m_BossSpownFlag = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���O���̓G�I�u�W�F�N�g�̍X�V
//-----------------------------------------------------------------------------
void	CEnmMoleObj::Update()
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
				// �_���[�W���󂯂��Ƃ��̖��G���Ԃ̓_�ŉ��o
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
					// �n�ʂ���o���A�j���[�V����
				case  APPEAR:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// �A�j���[�V�����I����AIDLE�֑J��
						m_pSprite->SetSrc(0, 336, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_dwStatusSub = IDLE;
						m_nAnimNum = MoleConstruct::ANIM_NUM_IDLE;
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
					// ���̍U���֑J�ڂ܂ł̑ҋ@�A�j���[�V����
					if (Random(0, 100) == 0) {
						// 1%�̊m���Ŏ��̍U�����J�n
						// DIVE�֑J��
						m_dwStatusSub = DIVE;
						m_pSprite->SetSrc(0, 1008, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_nAnimNum = MoleConstruct::ANIM_NUM_DIVE;
						m_nAnimIdx = 0;
					}
					break;



				case  DIVE:
					// ���@��A�j���[�V����
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// �A�j���[�V�����I����A�s���U���������_���Ō��肷��
						if (Random(0, 1) == 0) {
							// �V�䂩���𗎂Ƃ��U��
							m_pSprite->SetSrc(0, 168, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_dwStatus = SHOT;
							m_nAnimNum = MoleConstruct::ANIM_NUM_SHOT;
							m_nAnimIdx = 0;
							m_nDirIdx = LEFT;

							// ���Ƃ���̐������߂�
							m_ShotNum = Random(MoleConstruct::SHOT_NUM_MIN, MoleConstruct::SHOT_NUM_MAX);
							// ���Ƃ��ʒu�����߂�
							for (int i = 0; i < m_ShotNum; i++) {
								m_SHotPosX[i] = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							}
							// ���Ƃ����1���A�C�e���ɒu��������
							m_RotateItemPos = Random(0, m_ShotNum - 1);
							// �E��Ɉړ�
							m_vPos = VECTOR2(MoleConstruct::AREA_X_MAX, MoleConstruct::AREA_Y_MIN);
						}
						else {
							// �g��ɓːi���čU��
							m_pSprite->SetSrc(0, 1344, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_dwStatusSub = JUMP;
							m_nAnimNum = MoleConstruct::ANIM_NUM_JUMP;
							m_nAnimIdx = 0;

							if (m_nDirIdx == LEFT) {
								// IDLE���A���������Ă���ΉE����X�^�[�g
								m_vPos.x = MoleConstruct::AREA_X_MAX;
							}
							else {
								// �E�������Ă���΁A������X�^�[�g
								m_vPos.x = MoleConstruct::AREA_X_MIN;
							}
						}
					}
					break;

				case JUMP:
					// �g��ɓːi���čU��
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// 1�x�W�����v����Ƃ��΂炭�n���ɂ���
						m_pSprite->SetSrc(0, 0, 0, 0);		// �����蔻����Ȃ���
						m_vPos.y = 0;
						m_AnimCount = MoleConstruct::JUMP_COUNT;	// �����Ă���Ԃ̃t���[����
					}
					if (m_AnimCount <= 0 && m_pSprite->GetSrcWidth() == 0) {
						// �����Ă���Ԃɑҋ@�t���[������0�ɂȂ���
						m_pSprite->SetSrc(0, 1344, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_vPos.y = MoleConstruct::AREA_Y_MAX;
						m_nAnimIdx = 0;
					}
					else {
						// �ҋ@�t���[�������炷
						if (m_AnimCount > 0) {
							m_AnimCount--;
						}
					}
					// �G���A�̒[�ɂ��Ă��邩�`�F�b�N
					if (m_nDirIdx == LEFT) {
						m_vPosUp = VECTOR2(-MoleConstruct::JUMP_SPEED, 0);
						if (m_vPos.x <= MoleConstruct::AREA_X_MIN) {
							// ���[�ɂ����ꍇ�A����Ԃ֑J��
							m_dwStatus = SMOKE;
							m_pSprite->SetSrc(0, 0, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
							m_AnimCount = MoleConstruct::SMOKE_COUNT;

							// �ʒu�������_���ɂ���
							m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							m_vPos.y = MoleConstruct::AREA_Y_MAX;
							m_vPosUp.x = 0;
							m_pGMain->m_pDig->Play();
						}
					}
					else {
						m_vPosUp = VECTOR2(MoleConstruct::JUMP_SPEED, 0);
						if (m_vPos.x >= MoleConstruct::AREA_X_MAX) {
							// �E�[�ɂ����ꍇ�A����Ԃ֑J��
							m_dwStatus = SMOKE;
							m_nDirIdx = LEFT;
							m_pSprite->SetSrc(0, 0, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
							m_AnimCount = MoleConstruct::SMOKE_COUNT;

							// �ʒu�������_���ɂ���
							m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							m_vPos.y = MoleConstruct::AREA_Y_MAX;
							m_vPosUp.x = 0;
							m_pGMain->m_pDig->Play();
						}
					}
				}
				// �o�b�Ƃ̂����蔻��
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case SMOKE:
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
					// �o����Ԃ֑J��
					m_pSprite->SetSrc(0, 672, 
						MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
					m_nAnimNum = MoleConstruct::ANIM_NUM_APPEAR;
					m_nAnimIdx = 0;
					m_dwStatus = NORMAL;
					m_dwStatusSub = APPEAR;

					// ���݂̈ʒu����v���C���[�������������
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
				// �ォ���𗎂Ƃ��U��
				m_vPosUp.x = -MoleConstruct::SHOT_SPEED;

				for (int i = 0; i < m_ShotNum; i++) {
					// ���݂̍��W�̑O��ɍU�����s�����W���ݒ肳��Ă��邩
					if (m_SHotPosX[i] == m_vPos.x || m_SHotPosX[i] == m_vPos.x - 1 || m_SHotPosX[i] == m_vPos.x + 1) {
						// ���Ƃ��̂͊₩��]�A�C�e����
						if (m_RotateItemPos == i) {
							m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos + VECTOR2(0, 100), ITEMROTATEDOWN180);
						}
						else {
							m_pGMain->m_pWeaponProc->m_pWeaponRockProc->Start(m_vPos + VECTOR2(0, 100), this, ENM);
						}
					}
				}

				if (m_vPos.x <= MoleConstruct::AREA_X_MIN) {
					// ����ɓ��B�����牌��Ԃ֑J��
					m_dwStatus = SMOKE;
					m_pSprite->SetSrc(0, 0, 
						MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
					m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
					m_vPos.y = MoleConstruct::AREA_Y_MAX;
					m_vPosUp.x = 0;
					m_AnimCount = MoleConstruct::SMOKE_COUNT;
					m_pGMain->m_pDig->Play();
				}
				break;


			case  DAMAGE:
				if (m_pOtherObj->GetEnmAtc() == 1000) {
					// �₪������΁A��p�̒l���Q�Ƃ���
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