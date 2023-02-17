//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
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

#define  MAXTIME 400

//======================================================================
// バッククラウンド　プロシージャクラス
//
// (注意)基本プロシージャクラスは継承していない
//
//======================================================================
class CBackProc
{
protected:
	CGameMain*		m_pGMain;
	CSpriteImage*	m_pImageBk;
	CSprite*		m_pBackgrd;
	VECTOR2			m_vScroll;

public:
	//void Update();

	//CBackProc(CGameMain* pGMain);	// コンストラクタ
	//~CBackProc();

};
//======================================================================
// フォアクラウンド　プロシージャクラス
//
// (注意)基本プロシージャクラスは継承していない
//
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
