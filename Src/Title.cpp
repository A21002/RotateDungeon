//=============================================================================
//      オープニングタイトル・ゲームクリヤー・ゲームオーバーの処理
//																Title.cpp
//=============================================================================
#include "Title.h"
#include "Playchar.h"

// ---------------------------------------------------------------------------
// タイトルプロシージャのコンストラクタ
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/title.png")); // イメージのロード
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/clear.png")); // イメージのロード
	m_pImageOver = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/over.png")); // イメージのロード
	m_pSprite = new CSprite(m_pGMain->m_pShader ); // スプライトの生成

	m_dwCntTime = 0;
}

// ---------------------------------------------------------------------------
// タイトルタイトルプロシージャのデストラクタ
// ---------------------------------------------------------------------------
CTitleProc::~CTitleProc()
{
	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pSprite);
}

//------------------------------------------------------------------------
//	タイトルの表示	
//------------------------------------------------------------------------
void CTitleProc::Title()
{
	// 表示
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		// サウンドの再生
		m_pGMain->m_pBgm1->Play(AUDIO_LOOP);

		m_pGMain->m_dwGameStatus = GAMEMAIN;
		m_pGMain->m_StartTime = clock();

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	ゲームクリヤーの表示	
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{
	// 表示
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ゲームクリヤー"), 90, RGB(0, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ゲームクリヤー"), 90, RGB(255, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}

//------------------------------------------------------------------------
//	ゲームオーバーの表示	
//------------------------------------------------------------------------
void CTitleProc::GameOver()
{

	// 表示
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ゲームオーバー"), 90, RGB(0, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ゲームオーバー"), 90, RGB(255, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 100, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_RETURN) ||
		m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) || m_dwCntTime > 600)	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;
	}
}