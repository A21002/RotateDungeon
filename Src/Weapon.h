//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
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


#define  WEAPON_SHOT_MAX  200
#define  WEAPON_ROCK_MAX  100
#define  WEAPON_TEXTUI_MAX  1

#define  WEAPON_SHOT_ATK 100
#define  WEAPON_SHOT_SPEED 8

#define  WEAPON_ROCK_ATK_PC 300
#define  WEAPON_ROCK_ATK_ENM 1000



//======================================================================
// ����E�V���b�g�@�I�u�W�F�N�g�N���X
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
class CWeaponShotProc : public CBaseProc
{
public:
	BOOL Start( VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	CWeaponShotProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponShotProc() { ; }

};

//======================================================================
// ��@�I�u�W�F�N�g�N���X
class CWeaponRockObj : public CBaseObj
{
protected:
	DWORD  m_dwOwner;
	DWORD  m_hp;
	VECTOR2 m_top;

public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner) override;
	void Update() override;

	void RotatePos(DWORD) override;
	void RotateDrawUp() override;
	void RotateDrawDown() override;

	// �R���X�g���N�^
	CWeaponRockObj(CGameMain* pGMain);
	~CWeaponRockObj();

};

//======================================================================
// ��@�v���V�[�W���N���X
class CWeaponRockProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	CWeaponRockProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponRockProc() { ; }

};

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
