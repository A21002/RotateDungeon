//=============================================================================
//		�Q�[�����C������
//																GameMain.h
//=============================================================================
#pragma once

//�x����\��
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <time.h>

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Shader.h"
#include "Sprite.h"
#include "BaseProc.h"

// �X�e�[�^�X�^�C�v
#define  NONE				0x00000000
#define  TITLE				0x00000001
#define  STAGEDEMO  		0x00000002
#define  GAMEMAIN			0x00000004
#define  GAMEOVER			0x00000008
#define  GAMECLEAR			0x00000010
#define  GAMEEND			0x00000020

#define  NORMAL				0x00000001
#define  DAMAGE				0x00000010
#define  DEAD				0x00000020
#define  FLASH				0x00000040
#define  WALK				0x00000080
#define  JUMP				0x00000100
#define  STOP				0x00000400
#define  SMOKE              0x00000800
#define  APPEAR             0x00001000
#define  IDLE               0x00002000
#define  SHOT               0x00004000
#define  DIVE               0x00008000

// �����iDirIdx�j
#define  UP					3
#define  RIGHT				1
#define  DOWN				2
#define  LEFT				0

#define  GRAVITY  0.5

class CMain;
class CShader;
class CSpriteImage;
class CSprite;
class CFontTexture;
class CBaseObj;
class CBaseProc;
class CPcProc;
class CEnmProc;
class CWeaponProc;
class CEffectProc;
class CForeProc;
class CMapProc;
class CTitleProc;
class CBaseProc;

// --------------------------------------------------------------------------------------------------
//  �Q�[�����C���N���X
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// �����o�ϐ�
	CMain*			m_pMain;		// ���C���N���X
	CDirect3D*		m_pD3D;			//	Direct3D�I�u�W�F�N�g
	CXAudio*		m_pXAudio;		//	XAudio�I�u�W�F�N�g
	CDirectInput*	m_pDI;			//	DirectInputD�I�u�W�F�N�g
	CShader*		m_pShader;		//	�V�F�[�_�[
	CFontTexture*	m_pFont;		//	�t�H���g�e�N�X�`���[

	DWORD			m_dwGameStatus;
	DWORD           m_Rotate;
	VECTOR2			m_vScroll;
	BOOL            m_StartSpownFlag;
	DWORD           m_MoveItemWaitTime;
	BOOL            m_KeyFlag;
	BOOL            m_moveFlag;
	BOOL            m_TextUIFlag;
	BOOL            m_BossSpownFlag;
	BOOL            m_DoorTachFlag;
	clock_t         m_StartTime;

	// �v���V�[�W��
	CPcProc*		m_pPcProc;
	CEnmProc*		m_pEnmProc;
	CWeaponProc*	m_pWeaponProc;
	CEffectProc*	m_pEffectProc;
	CForeProc*		m_pForeProc;
	CMapProc*		m_pMapProc;
	CTitleProc*		m_pTitleProc;


	// �T�E���h
	CXAudioSource*  m_pSeShot;
	CXAudioSource*  m_pSeHit;
	CXAudioSource*  m_pSePower;
	CXAudioSource*  m_pBgm1;
	CXAudioSource*  m_pFall;
	CXAudioSource*  m_pCrash;
	CXAudioSource*  m_pDig;
	CXAudioSource*  m_pKey;

	// �X�v���C�g
	CSpriteImage*   m_pImageChar;      
	CSpriteImage*   m_pImageSprite;  
	CSpriteImage*   m_pImageWeapon;
	CSpriteImage*   m_pImageEnemy;
	CSpriteImage*   m_pImageBoss;
	CSpriteImage*   m_pImageItem;
	

	// ���\�b�h
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Loop();
	void    GameMain();
	void    Quit();

	HRESULT ChangeScreenMode(int nMode=-1); 
};
