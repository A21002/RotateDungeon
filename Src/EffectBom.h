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

#define  EFFECT_BOM_MAX  50

//======================================================================
// 映像効果・爆発　オブジェクトクラス
//======================================================================
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
//======================================================================
class CEffectBomProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2);

	CEffectBomProc(CGameMain* pGMain);	// コンストラクタ
	~CEffectBomProc() { ; }	// デストラクタ
};
