//=============================================================================
//		敵の処理
//																Enamy.h
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

#include "EnemySlime.h"
#include "EnemyBat.h"
#include "EnemyMole.h"

//======================================================================
// 敵キャラクター全体のメインプロシージャクラス
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	CEnmSlimeProc*	m_pEnmSlimeProc;
	CEnmBatProc*	m_pEnmBatProc;
	CEnmMoleProc*   m_pEnmMoleProc;

public:
	void  SetNonActive();
	void  Rotate(DWORD) override;
	void  RotateCenter(DWORD);
	CEnmProc(CGameMain* pGMain);	// コンストラクタ
	~CEnmProc() { ; }

};

