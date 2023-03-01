//=============================================================================
//      ����̏���
//																WeaponShot.h
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

#define  WEAPON_SHOT_MAX  200
#define  WEAPON_SHOT_ATK 100
#define  WEAPON_SHOT_SPEED 8

//======================================================================
// ����E�V���b�g�@�I�u�W�F�N�g�N���X
//======================================================================
class CWeaponShotObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	// �R���X�g���N�^
	CWeaponShotObj(CGameMain* pGMain);
	~CWeaponShotObj();

};

//======================================================================
// ����E�V���b�g�@�v���V�[�W���N���X
//======================================================================
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	CWeaponShotProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponShotProc() { ; }

};