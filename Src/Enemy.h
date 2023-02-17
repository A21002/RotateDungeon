//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
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


#define  ENM_SLIME_MAX  20
#define  ENM_BAT_MAX  20
#define  ENM_MOLE_MAX 1

#define  ENM_SLIME_WIDTH 72
#define  ENM_SLIME_HEIGHT 48
#define  ENM_SLIME_HP 200
#define  ENM_SLIME_ATK 300
#define  ENM_SLIME_JUMP_ANIM_START 2
#define  ENM_SLIME_JUMP_ANIM_FIN 4

#define  ENM_BAT_WIDTH 60
#define  ENM_BAT_HEIGHT 51
#define  ENM_BAT_HP 300
#define  ENM_BAT_ATK 200

#define  ENM_MOLE_WIDTH 318
#define  ENM_MOLE_HEIGHT 168
#define  ENM_MOLE_HP 3000
#define  ENM_MOLE_ATK 200


//======================================================================
// スライムの敵　オブジェクトクラス
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

//======================================================================
// コウモリの敵　オブジェクトクラス
class CEnmBatObj : public CBaseObj
{
protected:
	CEnmBatObj* m_pHpBarObj;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;
	void RotatePos(DWORD) override;
	void CreateChildObj();

	FLOAT moveNum;

	// コンストラクタ
	CEnmBatObj(CGameMain* pGMain);
	~CEnmBatObj();

};
//======================================================================
// コウモリの敵　プロシージャクラス
class CEnmBatProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	// コンストラクタ
	CEnmBatProc(CGameMain* pGMain);
	~CEnmBatProc() { ; }

};

//======================================================================
// モグラの敵　プロシージャクラス
class CEnmMoleProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	CBaseObj* GetMoleObj();
	// コンストラクタ
	CEnmMoleProc(CGameMain* pGMain);
	~CEnmMoleProc() { ; }

};

//======================================================================
// モグラの敵　オブジェクトクラス
class CEnmMoleObj : public CBaseObj
{
protected:
	;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	DWORD m_AnimCount;
	DWORD m_waitTime;
	DWORD m_SHotPosX[15];
	DWORD m_RotateItemPos;
	DWORD m_ShotNum;

	// コンストラクタ
	CEnmMoleObj(CGameMain* pGMain);
	~CEnmMoleObj();

};

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

