//=============================================================================
//		�O�i�E��i����
//																BackFore.h
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
// �t�H�A�N���E���h�@�v���V�[�W���N���X
//======================================================================
class CForeProc
{
protected:
	CGameMain*		m_pGMain;
	CSprite*		m_pForegrd;

public:
	void Update();

	CForeProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CForeProc();
};
