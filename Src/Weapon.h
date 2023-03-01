//=============================================================================
//      ����̏���
//																Weapon.h
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

#include "WeaponShot.h"
#include "WeaponRock.h"

//======================================================================
// ����S�̂̃��C���v���V�[�W���N���X
//======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponShotProc*	m_pWeaponShotProc;
	CWeaponRockProc*    m_pWeaponRockProc;

public:
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	CWeaponProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponProc() { ; }
};
