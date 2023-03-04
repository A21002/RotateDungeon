//=============================================================================
//		�O�i�E��i����
//																BackFore.cpp
//=============================================================================

#include "GameMain.h"
#include "Playchar.h"
#include "BackFore.h"
#include "Map.h"
#include "Enemy.h"

// ========================================================================================
// �O�i�̏���
// ========================================================================================
//------------------------------------------------------------------------
//
//	�O�i�v���V�[�W���̃R���X�g���N�^	
//
//  �����@CGameMain* pGMain
//
//------------------------------------------------------------------------
CForeProc::CForeProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pForegrd = new CSprite(m_pGMain->m_pShader);
}

// ---------------------------------------------------------------------------
// �O�i�v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CForeProc::~CForeProc()
{
	SAFE_DELETE(m_pForegrd);
}

//-----------------------------------------------------------------------------
// �O�i�v���V�[�W���̍X�V
//-----------------------------------------------------------------------------
void	CForeProc::Update()
{
	CSpriteImage* pImageFore = m_pGMain->m_pImageSprite; // ���łɓǂݍ���ł���X�v���C�g�C���[�W���g�p����
	float h = 0, m = 0;
	TCHAR str[256];
	int   DestX, DestY;

	// �X�e�[�^�X�o�[�̕\��
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();
	if (h < 0) h = 0;
	m = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp();
	if (m < 0) m = 0;

	// �g�o�Ƃo�b�c���̕\��
	DestX = 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 0, 213, 31);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 32, (DWORD)(144 * h), 6);
	_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetPcNum());
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	if (m_pGMain->m_pMapProc->m_nMapNo == 0) {
		// �~�j�}�b�v�̕\��
		DestX = WINDOW_WIDTH - 180 - 10;
		DestY = 10;
		m_pForegrd->Draw(pImageFore, DestX, DestY, 280, 0, 180, 160);

		// �~�j�}�b�v���PC�̈ʒu�̕\��
		DWORD PcPosX = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x / 20;
		DWORD PcPosY = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y / 20;
		// ��]�ɂ����W�C��
		DWORD x = PcPosX;
		switch (m_pGMain->m_pMapProc->GetRotate() / 90) {
		case 0:
			break;

		case 1:
			PcPosX = PcPosY;
			PcPosY = 160 - x;
			break;

		case 2:
			PcPosX = 160 - PcPosX;
			PcPosY = 160 - PcPosY;
			break;

		case 3:
			PcPosX = 160 - PcPosY;
			PcPosY = x;
			break;
		}

		m_pForegrd->Draw(pImageFore, DestX + PcPosX + 7, DestY + PcPosY - 2, 280, 160, 5, 5);
	}

	// �ړI���X�g�̕`��
	DestX = WINDOW_WIDTH - 297 - 10;
	DestY += 160 + 32;
	if (m_pGMain->m_pMapProc->m_nMapNo == 0) {
		if (!m_pGMain->m_DoorTachFlag) {
			m_pForegrd->Draw(pImageFore, DestX, DestY, 1249, 352, 297, 32);
		}
		else {
			if (m_pGMain->m_KeyFlag) {
				m_pForegrd->Draw(pImageFore, DestX, DestY, 1249, 384, 32, 32);
			}
			else {
				m_pForegrd->Draw(pImageFore, DestX, DestY, 1249, 352, 32, 32);
			}
			DestX += 32;
			m_pForegrd->Draw(pImageFore, DestX, DestY, 1281, 384, 175, 32);
			DestX -= 32;
			DestY += 32;
			m_pForegrd->Draw(pImageFore, DestX, DestY, 1249, 352, 297, 32);
		}
	}

	// ���b�Z�[�W�E�B���h�E�̕`�揈��
	if (m_pGMain->m_TextUIFlag) {
		m_pGMain->m_moveFlag = FALSE;
		if (m_pGMain->m_KeyFlag) {
			m_pForegrd->Draw(pImageFore, 183, 500, 470, 170, 1000, 160);
			CDirectInput* pDI = m_pGMain->m_pDI;
			if (pDI->CheckKey(KD_DAT, DIK_RETURN)) {
				m_pGMain->m_TextUIFlag = FALSE;
			}
		}
		else {
			m_pForegrd->Draw(pImageFore, 183, 500, 470, 0, 1000, 160);
		}
		CDirectInput* pDI = m_pGMain->m_pDI;
		if (pDI->CheckKey(KD_DAT, DIK_RETURN)) {
			m_pGMain->m_TextUIFlag = FALSE;
		}
	}

	// �{�X��HP�o�[�̕`��
	if (m_pGMain->m_BossSpownFlag) {
		int H = m_pGMain->m_pEnmProc->m_pEnmMoleProc->GetMoleObj()->GetHp();
		int maxH = m_pGMain->m_pEnmProc->m_pEnmMoleProc->GetMoleObj()->GetMaxHp();
		FLOAT h = (FLOAT)H / (FLOAT)maxH;
		if (H <= 0) {
			h = 0;
		}

		DestX = WINDOW_WIDTH - 608 - 100;
		DestY = WINDOW_HEIGHT - 48 - 20;
		// �g�̕`��
		m_pForegrd->Draw(pImageFore, DestX, DestY, 608, 416, 608, 48);

		DestX += 128 + 20;
		DestY += 8;
		// �o�[�̕`��
		m_pForegrd->Draw(pImageFore, DestX, DestY, 756, 480, (DWORD)(428 * h), 32);
	}	
}