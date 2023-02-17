//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		映像効果の処理
//																Effect.h
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

#define  ITEMROTATION180   1           // 180度
#define  ITEMMOVEDOOR     2           // 進むドア
#define  ITEMBACKDOOR     3           // 戻るドア

#define  ITEMMOVEDOOR_VER 4			// スプライトの範囲識別用
#define  ITEMBACKDOOR_VER 5

#define  ITEMMOVEDOOR_WID 6
#define  ITEMBACKDOOR_WID 7

#define  ITEMROTATION90    8
#define  ITEMROTATION270   9

#define  ITEMKEY 10

#define  ITEMRECOVER 11

#define  ITEMROTATEDOWN180 12

#define  ITEMMOVESIGN 13
#define  ITEMSHOTSIGN 14
#define  ITEMJUMPSIGN 15



#define  ITEM_MAX  50

#define  EFFECT_BOM_MAX  50

//======================================================================
// 映像効果・爆発　オブジェクトクラス
class CEffectBomObj : public CBaseObj
{
protected:
	;

public:
	BOOL Start(VECTOR2) override;

	void Update() override;

	// コンストラクタ
	CEffectBomObj(CGameMain* pGMain);
	~CEffectBomObj();

};
//======================================================================
// 映像効果・爆発　プロシージャクラス
class CEffectBomProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2);

	CEffectBomProc(CGameMain* pGMain);	// コンストラクタ
	~CEffectBomProc() { ; }	// デストラクタ

};
//=================================================================================================================
//======================================================================
// アイテム　オブジェクトクラス
class CItemObj : public CBaseObj
{
protected:
	;

public:
	BOOL Start(VECTOR2, DWORD) override;

	void Update() override;

	void RotatePos(DWORD) override;

	float waitTime;
	DWORD flag;
	BOOL  RotateFlag;
	BOOL  SignFlag;
	// コンストラクタ
	CItemObj(CGameMain* pGMain);
	~CItemObj();

};
//======================================================================
// アイテム　プロシージャクラス
class CItemProc : public CBaseProc
{
public:
	void Update() override;
	void Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;
	void Start(VECTOR2, DWORD);
	CItemProc(CGameMain* pGMain);	// コンストラクタ
	~CItemProc() { ; }	// デストラクタ

};

//=================================================================================================================


//======================================================================
// 映像効果　プロシージャクラス
class CEffectProc : public CBaseProc
{
public:
	CEffectBomProc*		m_pEffectBomProc;

	CItemProc*			m_pItemProc;

public:
	void  SetNonActive();

	CEffectProc(CGameMain* pGMain);	// コンストラクタ
	~CEffectProc() { ; }	// デストラクタ
};
