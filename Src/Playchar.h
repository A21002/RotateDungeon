//=============================================================================
//		プレイキャラクターの処理
//																Playchar.h
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
// プレイキャラクター　オブジェクトクラス
//======================================================================
class CPcObj : public CBaseObj
{
protected:
	int  m_nShotWait;

public:
	void Update() override;

	// コンストラクタ（オブジェクトの初期化）
	CPcObj(CGameMain* pGMain);
	~CPcObj();

};

//======================================================================
// プレイキャラクター　プロシージャクラス
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	int             m_nPcNum;

public:
	// アクセス関数
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	int				    GetPcNum() { return m_nPcNum; }

	CPcProc(CGameMain* pGMain);	// コンストラクタ
	~CPcProc() { ; }

};

//======================================================================
// プレイヤー定数クラス
//======================================================================
class PcConstruct
{
public:
	static const int IMAGE_WIDTH = 48;
	static const int IMAGE_HEIGHT = 57;

	static const int MAX_HP = 1000;
	static const int ATC = 10;
	static const int SPEED_MOVE = 5;
	static const int SPEED_JUMP = 5;

	static const int FLASH_SECOND = 1;
	static const int DEAD_SECOND = 3;
	static const float SHOT_WAIT_SECOND;
};