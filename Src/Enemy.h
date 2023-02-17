//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�G�̏���
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


#define  ENM_SLIME_MAX  20
#define  ENM_BAT_MAX  20
#define  ENM_MOLE_MAX 1

#define  ENM_SLIME_WIDTH 72
#define  ENM_SLIME_HEIGHT 48
#define  ENM_SLIME_HP 200
#define  ENM_SLIME_ATK 300
#define  ENM_SLIME_JUMP_ANIM_START 2
#define  ENM_SLIME_JUMP_ANIM_FIN 4

#define  ENM_BAT_WIDTH 60
#define  ENM_BAT_HEIGHT 51
#define  ENM_BAT_HP 300
#define  ENM_BAT_ATK 200

#define  ENM_MOLE_WIDTH 318
#define  ENM_MOLE_HEIGHT 168
#define  ENM_MOLE_HP 3000
#define  ENM_MOLE_ATK 200


//======================================================================
// �X���C���̓G�@�I�u�W�F�N�g�N���X
class CEnmSlimeObj : public CBaseObj
{
protected:
	CEnmSlimeObj* m_pHpBarObj;

public:
	BOOL Start(VECTOR2 vPos) override;
	void Update() override;
	void RotatePos(DWORD) override;

	void CreateChildObj();

	// �R���X�g���N�^
	CEnmSlimeObj(CGameMain* pGMain);
	~CEnmSlimeObj();

};
//======================================================================
// �X���C���̓G�@�v���V�[�W���N���X
class CEnmSlimeProc : public CBaseProc
{
protected:
	;

public:
	void   Update() override;
	void   Rotate(DWORD) override;
	void   RotateCenter(DWORD) override;
	// �R���X�g���N�^
	CEnmSlimeProc(CGameMain* pGMain);
	~CEnmSlimeProc() { ; }

};

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

//======================================================================
// ���O���̓G�@�v���V�[�W���N���X
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

//======================================================================
// �G�L�����N�^�[�S�̂̃��C���v���V�[�W���N���X
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	CEnmSlimeProc*	m_pEnmSlimeProc;
	CEnmBatProc*	m_pEnmBatProc;
	CEnmMoleProc*   m_pEnmMoleProc;

public:
	void  SetNonActive();
	void  Rotate(DWORD) override;
	void  RotateCenter(DWORD);
	CEnmProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CEnmProc() { ; }

};

