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

#define  ENM_MOLE_WIDTH 318
#define  ENM_MOLE_HEIGHT 168
#define  ENM_MOLE_HP 3000
#define  ENM_MOLE_ATK 200

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
	DWORD m_waitTime;
	DWORD m_SHotPosX[15];
	DWORD m_RotateItemPos;
	DWORD m_ShotNum;

	// �R���X�g���N�^
	CEnmMoleObj(CGameMain* pGMain);
	~CEnmMoleObj();

};
