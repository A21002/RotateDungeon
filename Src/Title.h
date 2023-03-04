//=============================================================================
//      オープニングタイトル・ゲームクリヤー・ゲームオーバーの処理
//																Title.h
//=============================================================================
#pragma once

#include "GameMain.h"

// --------------------------------------------------------------------
// タイトル　プロシージャクラス
// --------------------------------------------------------------------
class CTitleProc : public CBaseProc
{
protected:
	CGameMain*		m_pGMain;

	CSpriteImage*   m_pImageTitle;
	CSpriteImage*   m_pImageClear;
	CSpriteImage*   m_pImageOver;
	CSprite*        m_pSprite;

	DWORD           m_dwCntTime;

public:
	void Title();
	void GameClear();
	void GameOver();
	CTitleProc(CGameMain* pGMain);	// コンストラクタ
	~CTitleProc();
};