//=============================================================================
//		前景・後景処理
//																BackFore.h
//=============================================================================

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)


//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

//======================================================================
// フォアクラウンド　プロシージャクラス
//======================================================================
class CForeProc
{
protected:
	CGameMain*		m_pGMain;
	CSprite*		m_pForegrd;

public:
	void Update();

	CForeProc(CGameMain* pGMain);	// コンストラクタ
	~CForeProc();
};
