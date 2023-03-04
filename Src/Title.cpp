//=============================================================================
//      �I�[�v�j���O�^�C�g���E�Q�[���N�����[�E�Q�[���I�[�o�[�̏���
//																Title.cpp
//=============================================================================
#include "Title.h"
#include "Playchar.h"

// ---------------------------------------------------------------------------
// �^�C�g���v���V�[�W���̃R���X�g���N�^
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/title.png")); // �C���[�W�̃��[�h
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/clear.png")); // �C���[�W�̃��[�h
	m_pImageOver = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/over.png")); // �C���[�W�̃��[�h
	m_pSprite = new CSprite(m_pGMain->m_pShader ); // �X�v���C�g�̐���

	m_dwCntTime = 0;
}

// ---------------------------------------------------------------------------
// �^�C�g���^�C�g���v���V�[�W���̃f�X�g���N�^
// ---------------------------------------------------------------------------
CTitleProc::~CTitleProc()
{
	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pSprite);
}

//------------------------------------------------------------------------
//	�^�C�g���̕\��	
//------------------------------------------------------------------------
void CTitleProc::Title()
{
	// �\��
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		// �T�E���h�̍Đ�
		m_pGMain->m_pBgm1->Play(AUDIO_LOOP);

		m_pGMain->m_dwGameStatus = GAMEMAIN;
		m_pGMain->m_StartTime = clock();

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	�Q�[���N�����[�̕\��	
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{
	// �\��
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("�Q�[���N�����["), 90, RGB(0, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(350, 80, _T("�Q�[���N�����["), 90, RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	�Q�[���I�[�o�[�̕\��	
//------------------------------------------------------------------------
void CTitleProc::GameOver()
{

	// �\��
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("�Q�[���I�[�o�["), 90, RGB(0, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(350, 80, _T("�Q�[���I�[�o�["), 90, RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) ||
		m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}