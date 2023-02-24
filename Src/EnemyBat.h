//=============================================================================
//		�R�E�����̏���
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

#define  ENM_BAT_MAX  20

#define  ENM_BAT_WIDTH 60
#define  ENM_BAT_HEIGHT 51
#define  ENM_BAT_HP 300
#define  ENM_BAT_ATK 200

//======================================================================
// �R�E�����̓G�@�I�u�W�F�N�g�N���X
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

	// �R���X�g���N�^
	CEnmBatObj(CGameMain* pGMain);
	~CEnmBatObj();

};
//======================================================================
// �R�E�����̓G�@�v���V�[�W���N���X
class CEnmBatProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	// �R���X�g���N�^
	CEnmBatProc(CGameMain* pGMain);
	~CEnmBatProc() { ; }

};
