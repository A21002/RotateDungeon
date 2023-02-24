//=============================================================================
//		スライムの処理
//																EnamySlime.h
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

#define  ENM_SLIME_MAX  20

#define  ENM_SLIME_WIDTH 72
#define  ENM_SLIME_HEIGHT 48
#define  ENM_SLIME_HP 200
#define  ENM_SLIME_ATK 300
#define  ENM_SLIME_JUMP_ANIM_START 2
#define  ENM_SLIME_JUMP_ANIM_FIN 4


//======================================================================
// スライムの敵　オブジェクトクラス
//======================================================================
class CEnmSlimeObj : public CBaseObj
{
protected:
	CEnmSlimeObj* m_pHpBarObj;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;
	void RotatePos(DWORD) override;

	void CreateChildObj();

	// コンストラクタ
	CEnmSlimeObj(CGameMain* pGMain);
	~CEnmSlimeObj();

};

//======================================================================
// スライムの敵　プロシージャクラス
//======================================================================
class CEnmSlimeProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;
	// コンストラクタ
	CEnmSlimeProc(CGameMain* pGMain);
	~CEnmSlimeProc() { ; }

};
