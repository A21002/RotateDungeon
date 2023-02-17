//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�}�b�v�̏���
//																Map.h
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

#include "Main.h"
#include "Direct3D.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite.h"

#define   MAP_IMAGE_PATH  "Data/"
#define   MAP_MAX  11

//----------------------------------------------------------------------------
// �}�b�v���N���X�̒�`
// 		�n�_���W�o�w���W�A�x���W�p�A�I�_���W�o�w���W�A�x���W�p�E�E�E�E
// 		�@�����W (�J�n�ʒu����I���ʒu�Ɍ������āA��������ɖ@�����o����)
//----------------------------------------------------------------------------
class  CMapLine
{
public:
	VECTOR2 m_vStart;		// �n�_���W
	VECTOR2 m_vEnd;			// �I�_���W
	VECTOR2 m_vNormal;		// �@�����W (�J�n�ʒu����I���ʒu�Ɍ������āA��������ɖ@�����o����)

public:
	// �R���X�g���N�^
	CMapLine()
	{
		m_vStart  = VECTOR2(0, 0);
		m_vEnd    = VECTOR2(0, 0);
		m_vNormal = VECTOR2(0, 0);
	}
};
//----------------------------------------------------------------------------
// �C�x���g�}�b�v�N���X�̒�`
// 		���W�o�w���W�A�x���W�p�A�C�x���g�h�c�A�C�x���g�m��(��ނ�_���a��)�A�ėp�J�E���^
//		�C�x���g�h�c�@1:�o�b�̃X�^�[�g�ʒu�@2:�A�C�e���@3:�G
//		�C�x���g�m���@0x01 ��̐�    0x02 ���܂̓G    0x04 �I�I�J�~�̓G
//					�@0x10 �~�}��    0x20 ��
//----------------------------------------------------------------------------
class  CEvtMap
{
public:
	VECTOR2 m_vStart;		// ���W
	int         m_nEvtID;		// �C�x���g�h�c
	DWORD       m_dwEvtNo;		// �C�x���g�m��
	int         m_nCnt1;		// �ėp�J�E���^
public:
	// �R���X�g���N�^
	CEvtMap()
	{
		m_vStart     = VECTOR2(0, 0);
		m_nEvtID     = 0;
		m_dwEvtNo    = 0;
		m_nCnt1      = 0;
	}
	CEvtMap(VECTOR2 vStart, int nEvtID, DWORD dwEvtNo, int nCnt1)
	{
		m_vStart      = vStart;
		m_nEvtID      = nEvtID;
		m_dwEvtNo     = dwEvtNo;
		m_nCnt1       = nCnt1;
	}
};

//----------------------------------------------------------------------------
// �}�b�v�z��N���X�̒�`
//----------------------------------------------------------------------------
class  CMap
{ 
public:
	CGameMain*				 m_pGMain;
	CSpriteImage*            m_pMapImage;
	int                      m_nMapX;
	int                      m_nMapY;
	int                      m_nMapchipWidth;
	int                      m_nMapchipHeight;
	int                      m_nMapLnLength;
	int                      m_nEvtMapLength;
	int*                     m_MapBakTbl;	// �}�b�v�z��@�w�i
	int*                     m_MapForTbl;	// �}�b�v�z��@�O�i
	CMapLine*                m_MapLn;		// �}�b�v���C���z��
	CEvtMap*                 m_EvtMap;		// �C�x���g�}�b�v�z��

public:
	// �R���X�g���N�^
	CMap(CGameMain*	pGMain, CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight);
	~CMap();
};

//----------------------------------------------------------------------------
// �}�b�v�v���V�[�W���N���X�̒�`
//----------------------------------------------------------------------------
class  CMapProc
{
protected:
	CGameMain*	m_pGMain;

	
	
	// �}�b�v���̕`��t���O
	BOOL  m_bDrawMapLine;

	
	// �}�b�v�C���[�W
	CSpriteImage* m_pMapImage1;
	CSpriteImage* m_pMapImage2;
	CSpriteImage* m_pBakImage;
	// �}�b�v�X�v���C�g
	CSprite*      m_pSpriteMap;
	CSprite*      m_pSpriteBak;
	CSprite*      m_pSpriteLine;

	DWORD m_RotateAnim;

public:
	// �R���X�g���N�^
	CMapProc(CGameMain*	pGMain);
	~CMapProc();
	void InitMap();
	void MakeMap(CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight,
				int  mapBakTbl[], int mapBakLen, int  mapForTbl[], int mapForLen, 
				VECTOR2 mapLine[], int mapLineLen, CEvtMap EvtMap[], int EvtMapLen);
	void MoveMap();
	void BackMap();
	void SetMap(int, VECTOR2);
	void Update();
	void Update2();
	void DrawBack();
	void DrawFore();
	void DrawMapLine();
	void Rotate90(DWORD);
	void Rotate180(DWORD);
	void Rotate270(DWORD);
	BOOL isCollisionMoveMap(CBaseObj* obj, CMapLine* &pHitmapline);
	BOOL CheckMapcross(CMapLine map, CBaseObj* obj, VECTOR2 &hitpos);
	BOOL CheckLinecross(VECTOR2 a1, VECTOR2 a2, VECTOR2 b1, VECTOR2 b2, VECTOR2 &hitpos);
	BOOL CheckRange(float l, float r, float pt); 
	BOOL CheckMapnear(CMapLine MapLn, CBaseObj* obj, VECTOR2 &vHitpos);
	float GetDistance(CMapLine MapLn, CBaseObj* obj);
	float GetLength(VECTOR2 p1, VECTOR2 p2);
	float GetCross(VECTOR2 a, VECTOR2 b);
	float GetDot(VECTOR2 a, VECTOR2 b);

	BOOL SearchEvt(int nStart, int nEvtID, DWORD dwEvtNo, VECTOR2& vPos, int& nNext);

	// �}�b�v�z��
	CMap* m_pMapArray[MAP_MAX];
	// �}�b�v��
	int   m_nMapNo;
	// ��]��
	
	// �}�b�v�T�C�Y�z��
	VECTOR2 m_nMapSize[10];
	// �@���̉�]���
	DWORD m_RotateMap[10];

	BOOL m_OneSet;
	int  Rota;
};
