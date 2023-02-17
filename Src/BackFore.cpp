//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		前景・後景処理
//																BackFore.cpp
//=============================================================================

#include "GameMain.h"
#include "Playchar.h"
#include "BackFore.h"
#include "Map.h"
#include "Enemy.h"


// ========================================================================================
//
// 前景の処理
//
//
// ========================================================================================
//------------------------------------------------------------------------
//
//	前景プロシージャのコンストラクタ	
//
//  引数　CGameMain* pGMain
//
//------------------------------------------------------------------------
CForeProc::CForeProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pForegrd = new CSprite(m_pGMain->m_pShader);
}
// ---------------------------------------------------------------------------
//
// 前景プロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CForeProc::~CForeProc()
{
	SAFE_DELETE(m_pForegrd);
}
//-----------------------------------------------------------------------------
// 前景プロシージャの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CForeProc::Update()
{
	CSpriteImage* pImageFore = m_pGMain->m_pImageSprite; // すでに読み込んであるスプライトイメージを使用する
	float h = 0, m = 0;
	TCHAR str[256];
	int   DestX, DestY;

	// ステータスバーの表示
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();
	if (h < 0) h = 0;
	m = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxMp();
	if (m < 0) m = 0;

	// ＨＰとＰＣ残数の表示
	DestX = 10;
	DestY = 10;
	m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 0, 213, 31);
	m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 32, (DWORD)(144 * h), 6);
	_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetPcNum());
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	if (m_pGMain->m_pMapProc->m_nMapNo == 0) {
		// ミニマップの表示
		DestX = WINDOW_WIDTH - 180 - 10;
		DestY = 10;
		m_pForegrd->Draw(pImageFore, DestX, DestY, 280, 0, 180, 160);
		/*
		m_pForegrd->Draw(pImageFore, DestX, DestY, 0, 156, 213, 28);
		m_pForegrd->Draw(pImageFore, DestX + 59, DestY + 6, 59, 186, (int)(144 * m), 6);
		_stprintf(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetMp());
		m_pGMain->m_pFont->Draw(DestX + 8, DestY + 16, str, 12, RGB(0, 0, 0));
		*/

		// ミニマップ上のPCの位置の表示
		DWORD PcPosX = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x / 20;
		DWORD PcPosY = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y / 20;
		// 回転による座標修正
		DWORD x = PcPosX;
		switch (m_pGMain->m_Rotate / 90) {
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

	// 目的リストの描画
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

	// メッセージウィンドウの描画処理
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

	/*
	// タイマーの表示処理

	// 現在のクロック数を取得
	clock_t EndTime = clock();

	// スタートから今までの時間
	DWORD NowTime = (EndTime - m_pGMain->m_StartTime) / CLOCKS_PER_SEC;

	// 時間が過ぎたらゲームオーバー
	if (NowTime >= MAXTIME) {
		m_pGMain->m_dwGameStatus = GAMEOVER;
	}

	// 残り時間
	DWORD ResidueTime = MAXTIME - NowTime;
	
	// 描画
	DestX = WINDOW_WIDTH - 48 * 3 - 10;
	DestY = 180;
	_stprintf(str, _T("%03d"), ResidueTime);
	m_pGMain->m_pFont->Draw(DestX , DestY, str, 48, RGB(255, 0, 0));

	*/
	/*
	if (m_pGMain->m_pMapProc->m_nMapNo == 0) {
		// 鍵の取得アイコンを描画
		DestX = WINDOW_WIDTH - 230;
		DestY = 30;

		if (m_pGMain->m_KeyFlag) {
			m_pForegrd->Draw(pImageFore, DestX, DestY, 240, 0, 30, 30);
		}
	}
	*/

	// ボスのHPバーの描画
	if (m_pGMain->m_BossSpownFlag) {
		int H = m_pGMain->m_pEnmProc->m_pEnmMoleProc->GetMoleObj()->GetHp();
		int maxH = m_pGMain->m_pEnmProc->m_pEnmMoleProc->GetMoleObj()->GetMaxHp();
		FLOAT h = (FLOAT)H / (FLOAT)maxH;
		if (H <= 0) {
			h = 0;
		}

		DestX = WINDOW_WIDTH - 608 - 100;
		DestY = WINDOW_HEIGHT - 48 - 20;
		// 枠の描画
		m_pForegrd->Draw(pImageFore, DestX, DestY, 608, 416, 608, 48);

		DestX += 128 + 20;
		DestY += 8;
		// バーの描画
		m_pForegrd->Draw(pImageFore, DestX, DestY, 756, 480, (DWORD)(428 * h), 32);
	}

	
}


