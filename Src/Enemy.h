//=============================================================================
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

#include "EnemySlime.h"
#include "EnemyBat.h"
#include "EnemyMole.h"

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

