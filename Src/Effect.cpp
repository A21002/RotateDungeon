//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�f�����ʂ̏���
//																Effect.cpp
//=============================================================================

#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
//
//	�f�����ʃ��C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CEffectProc::CEffectProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pEffectBomProc = new CEffectBomProc(m_pGMain);	// �f�����ʁE�����v���V�[�W��
	m_pProcArray.push_back(m_pEffectBomProc);     // �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pItemProc = new CItemProc(m_pGMain);				// �A�C�e���v���V�[�W��
	m_pProcArray.push_back(m_pItemProc);		// �v���V�[�W�����v���V�[�W���z��ɓo�^����

}
//============================================================================
//   �v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
//   �v���V�[�W���̊J�n�t���O�����Z�b�g����
//============================================================================
void  CEffectProc::SetNonActive()
{

	//�I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag(0);				// �J�n�t���O�̃��Z�b�g
		((CEffectProc*)m_pProcArray[i])->SetNonActive();      // ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}


// ================================================================================================================================
// 
// �e�f�����ʃv���V�[�W�����̏���
// 
// ================================================================================================================================


//------------------------------------------------------------------------
//
//	�f�����ʁE�����v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CEffectBomProc::CEffectBomProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < EFFECT_BOM_MAX; i++)
	{
		m_pObjArray.push_back(new CEffectBomObj(m_pGMain));	// m_pObjArray�ɉf�����ʁE�����I�u�W�F�N�g�𐶐�����
	}

}

//-----------------------------------------------------------------------------
// �f�����ʁE�����v���V�[�W���̊J�n
//
//   VECTOR2 vPos    �����ʒu
//
//   �߂�l�@�@����ɔ��� : TRUE    �������Ȃ����� : FALSE
//-----------------------------------------------------------------------------
BOOL	CEffectBomProc::Start(VECTOR2 vPos)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive() )
		{
			m_pObjArray[i]->Start(vPos);	// �f�����ʁE�����̔���
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------
//
//	�f�����ʁE�����I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CEffectBomObj::CEffectBomObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 144, 480, 64, 64);
	m_nAnimNum = 7;

}
// ---------------------------------------------------------------------------
//
// �f�����ʁE�����I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CEffectBomObj::~CEffectBomObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos    �����ʒu
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL	CEffectBomObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	m_dwStatus = NORMAL;
	m_dwStatusSub = STOP;
	m_dwStatusPhase = 0;

	m_vPos = vPos;	// �����ʒu

	m_nAnimIdx = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// �f�����ʁE�����I�u�W�F�N�g�̍X�V
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEffectBomObj::Update()
{
	if (m_bActive)
	{
		if (AnimCountup() == 1)
		{
			m_bActive = FALSE;
		}
		Draw();

	}
}
// ==============================================================================================================

// =================================================================================
//------------------------------------------------------------------------
//
//	�A�C�e���v���V�[�W���̃R���X�g���N�^	
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CItemProc::CItemProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CItemObj(m_pGMain));	// m_pObjArray�ɃA�C�e���𐶐�����
	}
	m_nStartFlag = 0;
}

void CItemProc::Start(VECTOR2 vPos, DWORD dwNo) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (!m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->Start(vPos, dwNo);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// �A�C�e���v���V�[�W���̊J�n
//
//   �߂�l�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CItemProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	if (m_nStartFlag == 0)
	{
		// �C�x���g�}�b�v��T�����A�~�}����ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x10 �~�}��	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x10, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION180);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A�h�A�c�iMove�j��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x20 �h�A	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x20, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVEDOOR_VER);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A�h�A�c�iBack�j��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x40 �h�A	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x40, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBACKDOOR_VER);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A�h�A���iMove�j��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x80 �h�A	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x80, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVEDOOR_WID);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A�h�A���iMove�j��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x100 �h�A	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x100, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBACKDOOR_WID);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A90�x��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x200 90�x	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x200, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION90);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A270�x��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x400 270�x	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x400, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION270);
						break;
					}
				}
			}
		}

		// �C�x���g�}�b�v��T�����A270�x��ݒ肷��
		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x800 ��)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x800, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMKEY);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x1000 �ړ��Ŕ�)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x1000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVESIGN);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x2000 �U���Ŕ�)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x2000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMSHOTSIGN);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// �C�x���g�}�b�v�̒T���@ ( EvtID:2�@�A�C�e���̏o���ʒu,  EvtNo:0x4000 �W�����v�Ŕ�)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x4000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMJUMPSIGN);
						break;
					}
				}
			}
		}

		for (DWORD j = 0; j < m_pObjArray.size(); j++)
		{
			
			if (m_pObjArray[j]->GetActive())
			{
				for (DWORD i = 0; i < m_pGMain->m_Rotate / 90; i++) {
					m_pObjArray[j]->RotatePos(90);
					//break;
				}
				if (m_pGMain->m_Rotate / 90 != 0) {
					m_pObjArray[j]->SetPos(VECTOR2(m_pObjArray[j]->GetPos().x - m_pObjArray[j]->GetSprite()->GetSrcWidth(),
						m_pObjArray[j]->GetPos().y - m_pObjArray[j]->GetSprite()->GetSrcHeight()));
				}
				
			}
			
		}
		m_nStartFlag = 1;  // ��񔭂�������A�����������Ȃ�
	}

	
}

//------------------------------------------------------------------------
//
//	�A�C�e���I�u�W�F�N�g�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CItemObj::CItemObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageItem);
	m_nAnimNum = 2;

}
// ---------------------------------------------------------------------------
//
// �A�C�e���I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CItemObj::~CItemObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̊J�n
//
//   VECTOR2 vPos    �����ʒu
//   DWORD       dwNo    �A�C�e�����
//
//   �߂�l�@�@TRUE
//-----------------------------------------------------------------------------
BOOL	CItemObj::Start(VECTOR2 vPos, DWORD dwNo)
{
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_dwStatusSub = dwNo;
	waitTime = 0;
	m_vPos = vPos;	// �����ʒu
	m_vPosUp = VECTOR2(0, 0);
	RotateFlag = FALSE;
	SignFlag = FALSE;

	switch (m_dwStatusSub)
	{
	case  ITEMROTATION180:     // �~�}��
		m_pSprite->SetSrc(96, 64, 64, 64);
		flag = 0;
		break;

	case  ITEMMOVEDOOR_VER:      // �h�A
		m_pSprite->SetSrc(0, 0, 64, 96);
		m_dwStatusSub = ITEMMOVEDOOR;
		m_nAnimNum = 1;
		flag = 1;
		break;

	case  ITEMBACKDOOR_VER:		 // �h�A(Back)
		m_pSprite->SetSrc(0, 0, 64, 96);
		m_dwStatusSub = ITEMBACKDOOR;
		flag = 1;
		break;

	case  ITEMMOVEDOOR_WID:
		m_pSprite->SetSrc(0, 96, 96, 64);
		m_dwStatusSub = ITEMMOVEDOOR;
		flag = 2;
		break;

	case  ITEMBACKDOOR_WID:
		m_pSprite->SetSrc(0, 96, 96, 64);
		m_dwStatusSub = ITEMBACKDOOR;
		flag = 2;
		break;

	case  ITEMROTATION90:
		m_pSprite->SetSrc(96, 0, 64, 64);
		flag = 0;
		break;

	case  ITEMROTATION270:
		m_pSprite->SetSrc(96, 128, 64, 64);
		flag = 0;
		break;

	case  ITEMKEY:
		m_pSprite->SetSrc(224, 0, 64, 64);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMRECOVER:
		m_pSprite->SetSrc(0, 192, 48, 48);
		flag = 0;
		m_nAnimNum = 2;
		break;

	case  ITEMROTATEDOWN180:
		m_pSprite->SetSrc(96, 64, 64, 64);
		flag = 0;
		m_nAnimNum = 2;
		m_vPosUp = VECTOR2(0, 4);
		break;

	case  ITEMMOVESIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMSHOTSIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMJUMPSIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// �A�C�e���I�u�W�F�N�g�̍X�V
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CItemObj::Update()
{
	if (!m_bActive) return;

	if (m_pGMain->m_moveFlag) {
		if (isCollision(m_pGMain->m_pPcProc->GetPcObjPtr()) && m_pGMain->m_MoveItemWaitTime <= 0) // PC�Ƃ̐ڐG����
		{
			switch (m_dwStatusSub)
			{
			case  ITEMMOVEDOOR:      // �h�A
				m_pGMain->m_DoorTachFlag = TRUE;
				m_pGMain->m_pSePower->Play();
				if (m_pGMain->m_KeyFlag) {
					m_pGMain->m_TextUIFlag = TRUE;
					RotateFlag = TRUE;
				}
				else {
					m_pGMain->m_TextUIFlag = TRUE;
					RotateFlag = TRUE;
				}
				break;

			case  ITEMBACKDOOR:
				m_pGMain->m_pSePower->Play();
				m_pGMain->m_pMapProc->BackMap();
				break;

			case  ITEMROTATION180:     
			/*
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp( m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
			m_pGMain->m_pSePower->Play();
			break;
			*/
				m_pGMain->m_pMapProc->Rotate180(180);
				break;

			case  ITEMROTATION90:
				m_pGMain->m_pMapProc->Rotate90(270);
				break;

			case ITEMROTATION270:
				m_pGMain->m_pMapProc->Rotate270(90);
				break;

			case  ITEMKEY:
				m_pGMain->m_KeyFlag = TRUE;
				m_bActive = FALSE;
				m_pGMain->m_pKey->Play();
				break;

			case  ITEMRECOVER:
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
				m_pGMain->m_pSePower->Play();
				m_bActive = FALSE;
				break;

			case  ITEMROTATEDOWN180:
				m_pGMain->m_pMapProc->Rotate180(180);
				m_bActive = FALSE;
				break;

			case  ITEMMOVESIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;

			case  ITEMSHOTSIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;

			case  ITEMJUMPSIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;
			}
			m_pGMain->m_MoveItemWaitTime = 60;

		}

		if (m_pGMain->m_MoveItemWaitTime > 0) {
			m_pGMain->m_MoveItemWaitTime--;
		}

		if (m_dwStatusSub == ITEMROTATEDOWN180) {
			BOOL bRet;
			CMapLine*  pHitmapline = NULL;
			// �}�b�v���Ƃ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
			bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
			if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
			{
				m_vPos += m_vPosUp;
			}
		}
		
		if (m_nAnimIdx >= 5 && !SignFlag) {
			switch (m_dwStatusSub) {
			case  ITEMMOVESIGN:
				m_pSprite->SetSrc(0, 570, 285, 285);
				m_nAnimNum = 14;
				break;

			case  ITEMSHOTSIGN:
				m_pSprite->SetSrc(0, 855, 285, 285);
				m_nAnimNum = 8;
				break;

			case  ITEMJUMPSIGN:
				m_pSprite->SetSrc(0, 1140, 285, 285);
				m_nAnimNum = 7;
				break;
			}
			m_nAnimIdx = 0;
			SignFlag = TRUE;
		}

		AnimCountup();
	}
	if (!m_pGMain->m_TextUIFlag && RotateFlag) {
		if (m_pGMain->m_KeyFlag) {
			m_pGMain->m_pMapProc->MoveMap();
			RotateFlag = FALSE;
		}
		else {
			m_pGMain->m_pMapProc->Rotate180(180);
			RotateFlag = FALSE;
		}
		
	}
	Draw();

}

void CItemProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CItemObj::RotatePos(DWORD rotate) {
	if (m_dwStatusSub == ITEMMOVESIGN || m_dwStatusSub == ITEMJUMPSIGN || m_dwStatusSub == ITEMSHOTSIGN) {
		m_bActive = FALSE;
	}
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

		if (flag == 1) {
			m_pSprite->SetSrc(0, 96, 96, 64);
			flag = 2;
		}
		else if (flag == 2) {
			m_pSprite->SetSrc(0, 0, 64, 96);
			flag = 1;
		}
		break;
	}
	m_fRotate = 0;
}

void CItemProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			if (m_pGMain->m_pMapProc->m_nMapNo != 1) {
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
}
