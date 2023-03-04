//=============================================================================
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

//======================================================================
// �v���C�L�����N�^�[�@�I�u�W�F�N�g�N���X
//======================================================================
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
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	int             m_nPcNum;

public:
	// �A�N�Z�X�֐�
	CPcObj*			    GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }
	int				    GetPcNum() { return m_nPcNum; }

	CPcProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CPcProc() { ; }

};

//======================================================================
// �v���C���[�萔�N���X
//======================================================================
class PcConstruct
{
public:
	static const int IMAGE_WIDTH = 48;
	static const int IMAGE_HEIGHT = 57;

	static const int MAX_HP = 1000;
	static const int ATC = 10;
	static const int SPEED_MOVE = 5;
	static const int SPEED_JUMP = 5;

	static const int FLASH_SECOND = 1;
	static const int DEAD_SECOND = 3;
	static const float SHOT_WAIT_SECOND;
};