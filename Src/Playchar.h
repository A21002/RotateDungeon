//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�v���C�L�����N�^�[�̏���
//																Playchar.h
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

#define PC_IMAGE_WIDTH 48
#define PC_IMAGE_HEIGHT 57

#define PC_MAXHP 1000
#define PC_ATC   10
#define PC_MOVESPEED 5
#define PC_JUMPSPEED 9

//======================================================================
// �v���C�L�����N�^�[�@�I�u�W�F�N�g�N���X
class CPcObj : public CBaseObj
{
protected:
	int  m_nShotWait;

public:
	void Update() override;

	// �R���X�g���N�^�i�I�u�W�F�N�g�̏������j
	CPcObj(CGameMain* pGMain);
	~CPcObj();

};
//======================================================================
// �v���C�L�����N�^�[�@�v���V�[�W���N���X
class CPcProc : public CBaseProc
{
protected:
	int             m_nPcNum;

public:
	// �A�N�Z�X�֐�
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	int				    GetPcNum() { return m_nPcNum; }
	void				AddPcNum(int Num) { m_nPcNum += Num; }

	CPcProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CPcProc() { ; }

};