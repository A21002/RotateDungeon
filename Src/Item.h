//=============================================================================
//		映像効果の処理
//																Item.h
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

#define  ITEMRECOVER	1
#define  ITEM_DOOR      2  
#define  ITEMKEY		3

#define  ITEMROTATION90		4
#define  ITEMROTATION180    5
#define  ITEMROTATEDOWN180  6
#define  ITEMROTATION270    7

#define  ITEMMOVESIGN	8
#define  ITEMSHOTSIGN	9
#define  ITEMJUMPSIGN  10

#define  ITEM_MAX  50

//======================================================================
// アイテム　オブジェクトクラス
//======================================================================
class CItemObj : public CBaseObj
{
protected:
	;

public:
	BOOL Start(VECTOR2, DWORD) override;

	void Update() override;

	void RotatePos(DWORD) override;

	DWORD m_bIsRotateChengeFlag;		// 回転後、角度に合わせた絵に変更するか　0：なし	1:縦	2：横
	BOOL  m_bIsCheckKeyFlag;			// テキストダイアログ後、鍵があるかのチェックを行うためのフラグ
	BOOL  SignFlag;						// 看板に触れ、チュートリアルを表示するフラグ

	// コンストラクタ
	CItemObj(CGameMain* pGMain);
	~CItemObj();

};

//======================================================================
// アイテム　プロシージャクラス
//======================================================================
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

class ItemConstruct
{
public:
	static const int RECOVER_IMAGE_WIDTH = 7;
	static const int RECOVER_IMAGE_HEIGHT = 7;
	static const int RECOVER_DIFF = 20;
	static const int RECOVER_NUM = 10;
	static const int RECOVER_PARTICLE_SPEED = 3;

	static const int DOOR_IMAGE_WIDTH = 64;
	static const int DOOR_IMAGE_HEIGHT = 96;

	static const int KEY_IMAGE_WIDTH = 64;
	static const int KEY_IMAGE_HEIGHT = 64;

	static const int ROTATION_IMAGE_WIDTH = 64;
	static const int ROTATION_IMAGE_HEIGHT = 64;

	static const int SIGN_IMAGE_WIDTH = 285;
	static const int SIGN_IMAGE_HEIGHT = 285;
	static const int SIGN_TURN_ANIM_NUM = 6;
	static const int SIGN_MOVE_ANIM_NUM = 14;
	static const int SIGN_SHOT_ANIM_NUM = 8;
	static const int SIGN_JUMP_ANIM_NUM = 7;
};