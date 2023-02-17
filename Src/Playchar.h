//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
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

#define PC_IMAGE_WIDTH 48
#define PC_IMAGE_HEIGHT 57

#define PC_MAXHP 1000
#define PC_ATC   10
#define PC_MOVESPEED 5
#define PC_JUMPSPEED 9

//======================================================================
// プレイキャラクター　オブジェクトクラス
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
class CPcProc : public CBaseProc
{
protected:
	int             m_nPcNum;

public:
	// アクセス関数
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	int				    GetPcNum() { return m_nPcNum; }
	void				AddPcNum(int Num) { m_nPcNum += Num; }

	CPcProc(CGameMain* pGMain);	// コンストラクタ
	~CPcProc() { ; }

};