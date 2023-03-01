//=============================================================================
//      ����̏���
//																WeaponRock.h
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

#define  WEAPON_ROCK_MAX  100
#define  WEAPON_ROCK_ATK_PC 300
#define  WEAPON_ROCK_ATK_ENM 1000

//======================================================================
// ��@�I�u�W�F�N�g�N���X
//======================================================================
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
//======================================================================
class CWeaponRockProc : public CBaseProc
{
public:
	BOOL Start(VECTOR2, CBaseObj* pObj, DWORD dwOwner);

	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;

	CWeaponRockProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CWeaponRockProc() { ; }

};
