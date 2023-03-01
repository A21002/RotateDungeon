//=============================================================================
//		���O���̏���
//																Enamy.h
//=============================================================================

#pragma once

//�x����\��
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define  ENM_MOLE_MAX 1

//======================================================================
// ���O���̓G�@�v���V�[�W���N���X
//======================================================================
class CEnmMoleProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	CBaseObj* GetMoleObj();
	// �R���X�g���N�^
	CEnmMoleProc(CGameMain* pGMain);
	~CEnmMoleProc() { ; }

};

//======================================================================
// ���O���̓G�@�I�u�W�F�N�g�N���X
//======================================================================
class CEnmMoleObj : public CBaseObj
{
protected:
	;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;

	DWORD m_AnimCount;
	DWORD m_SHotPosX[15];
	DWORD m_RotateItemPos;
	DWORD m_ShotNum;

	// �R���X�g���N�^
	CEnmMoleObj(CGameMain* pGMain);
	~CEnmMoleObj();

};

class MoleConstruct
{
public:
	static const int HP = 3000;
	static const int ATK = 200;
	static const int JUMP_SPEED = 4;
	static const int SHOT_SPEED = 3;

	static const int IMAGE_WIDTH = 318;
	static const int IMAGE_HEIGHT = 168;

	static const int ANIM_NUM_SMOKE = 3;
	static const int ANIM_NUM_APPEAR = 7;
	static const int ANIM_NUM_IDLE = 2;
	static const int ANIM_NUM_DIVE = 7;
	static const int ANIM_NUM_SHOT = 3;
	static const int ANIM_NUM_JUMP = 10;

	static const int SHOT_NUM_MIN = 10;
	static const int SHOT_NUM_MAX = 15;

	static const int AREA_X_MIN = 128;
	static const int AREA_X_MAX = 919;
	static const int AREA_Y_MIN = 96;
	static const int AREA_Y_MAX = 503;

	static const int JUMP_COUNT = 45;
	static const int SMOKE_COUNT = 60;
};