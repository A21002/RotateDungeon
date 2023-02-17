//=============================================================================
//		�Q�c�A�N�V�����Q�[���v���O����
//		�b�Q�c�`�����P�P�R�@�@�@�@�@�@           ver 3.0        2021.1.11
//
//		�}�b�v�̏���
//																Map.cpp
//=============================================================================
#include "Map.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Effect.h"
#include "Weapon.h"


// CMap�N���X   --------------------------------------------------------------------------------------------
// �R���X�g���N�^
CMap::CMap(CGameMain* pGMain, CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight)
{
	m_pGMain = pGMain;
	m_pMapImage = pMapImage;
	m_nMapX = imapX;
	m_nMapY = imapY;
	m_nMapchipWidth = imapchipWidth;
	m_nMapchipHeight = imapchipHeight;
	m_nMapLnLength = 0;
	m_nEvtMapLength = 0;
	m_MapBakTbl = NULL;
	m_MapForTbl = NULL;
	m_MapLn     = NULL;
	m_EvtMap    = NULL;
}
//  �f�X�g���N�^
CMap::~CMap()
{
	SAFE_DELETE_ARRAY(m_MapBakTbl);
	SAFE_DELETE_ARRAY(m_MapForTbl);
	SAFE_DELETE_ARRAY(m_MapLn);
	SAFE_DELETE_ARRAY(m_EvtMap);
}

//  CMapProc�N���X  ---------------------------------------------------------------------------------------
//  �R���X�g���N�^
CMapProc::CMapProc(CGameMain*	pGMain)
{
	m_pGMain = pGMain;
	m_nMapNo       = 0;
	m_bDrawMapLine = FALSE;
	m_OneSet = FALSE;
	for (DWORD i = 0; i < MAP_MAX; i++)
	{
		m_pMapArray[i] = NULL;
	}
	//m_pMapImage1  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/mapchip3.png"));
	m_pMapImage1  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/Map1.png"));
	m_pMapImage2 = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/Map2.png"));
	m_pBakImage   = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/aozora.png"));
	//m_pBakImage = new CSpriteImage(m_pGMain->m_pShader, _T("Data/image/hoshizora2.png"));
	m_pSpriteMap  = new CSprite(m_pGMain->m_pShader); // �X�v���C�g�I�u�W�F�N�g�̐���
	m_pSpriteBak  = new CSprite(m_pBakImage);
	m_pSpriteLine = new CSprite(m_pGMain->m_pShader);
	m_RotateAnim = 0;

	InitMap();
}

//  �f�X�g���N�^
CMapProc::~CMapProc()
{
	for (DWORD i = 0; i < MAP_MAX; i++)
	{
		SAFE_DELETE(m_pMapArray[i]);
	}
	SAFE_DELETE(m_pSpriteMap);
	SAFE_DELETE(m_pSpriteBak);
	SAFE_DELETE(m_pSpriteLine);
	SAFE_DELETE(m_pBakImage);
	SAFE_DELETE(m_pMapImage1);
	SAFE_DELETE(m_pMapImage2);
}
// ----------------------------------------------------------------------
//
// �}�b�v�z�񂩂�}�b�v�I�u�W�F�N�g�̍쐬
//
//   mapBakTbl �w�i�}�b�v�z��
//             (��)�}�b�v�̃`�b�v�ԍ�  ��R���͂x�ʒu�A���R���͂w�ʒu
//             (��)�}�b�v�̃`�b�v�ԍ�-1�Ԃ́A�}�b�v�`�b�v��u���Ȃ��ꏊ
//
//   mapForTbl �O�i�}�b�v�z��
//             (��)�}�b�v�̃`�b�v�ԍ�  ��R���͂x�ʒu�A���R���͂w�ʒu
//             (��)�}�b�v�̃`�b�v�ԍ�-1�Ԃ́A�}�b�v�`�b�v��u���Ȃ��ꏊ
//
//   mapLine   �}�b�v���z��z��
//
//   EvtMap    �x���g�}�b�v
//
// ----------------------------------------------------------------------
void CMapProc::InitMap()
{
	// -----------------------------------------------------------------------------------------------
	//
	// �X�e�[�W	1�@��̃X�e�[�W
	//
	// -----------------------------------------------------------------------------------------------

	// �w�i�}�b�v�@-----------------------------------------------------------
	int  mapBakTbl0[] = {
		0
	};

	int mapBakTbl1[] = {
		0
	};

	// �O�i�}�b�v�@-----------------------------------------------------------
	int  mapForTbl0[] = {
		-1
	};

	int mapForTbl1[] = {
		-1
	};


	//----------------------------------------------------------------------------
	// �}�b�v���z��@
	// 		�J�n�ʒu�o�w���W�A�x���W�p�A�r���E�I���ʒu�o�w���W�A�x���W�p�E�E�E�E
	// 		��̋L�q�̏I��{999999},
	//
	//----------------------------------------------------------------------------
	VECTOR2  mapLine0[] = {
		// �n�ʂ̐ݒ�
		/*
		VECTOR2(0, 0),			// ����
		VECTOR2(-48, 385 + 192),			// ���ǉ�
		VECTOR2(240, 385 + 192),			// �E�݂P�@����
		VECTOR2(336, 433 + 192),			// �E�݂P�@����
		VECTOR2(432, 433 + 192),			// �E�݂P�@�E��
		VECTOR2(528, 385 + 192),			// �E�݂P�@�E��

		VECTOR2(720, 385 + 192),			// �R�P�@����
		VECTOR2(720, 481 + 192),			// �R�P�@����
		VECTOR2(816, 481 + 192),			// �R�P�@�E��
		VECTOR2(816, 385 + 192),			// �R�P�@�E��


		VECTOR2(960, 385 + 192),			// �E�݂Q�@����
		VECTOR2(1104, 529 + 192),		// �E�݂Q�@����
		VECTOR2(1200, 529 + 192),		// �E�݂Q�@�E��
		VECTOR2(1344, 385 + 192),		// �E�݂Q�@�E��

		VECTOR2(1584, 385 + 192),		// �R�Q�@����
		VECTOR2(1584, 529 + 192),		// �R�Q�@����
		VECTOR2(1728, 529 + 192),		// �R�Q�@�E��
		VECTOR2(1728, 385 + 192),		// �R�Q�@�E��

		VECTOR2(1968, 385 + 192),		// �R�R�@����
		VECTOR2(1968, 529 + 192),		// �R�R�@����
		VECTOR2(2112, 529 + 192),		// �R�R�@�E��
		VECTOR2(2112, 385 + 192),		// �R�R�@�E��

		VECTOR2(2208, 385 + 192),		// �R�S�@����
		VECTOR2(2208, 529 + 192),		// �R�S�@����
		VECTOR2(2352, 529 + 192),		// �R�S�@�E��
		VECTOR2(2352, 385 + 192),		// �R�S�@�E��

		VECTOR2(2448, 385 + 192),		// �E�݂R�@����
		VECTOR2(2544, 481 + 192),		// �E�݂R�@����
		VECTOR2(2688, 481 + 192),		// �E�݂R�@�E��
		VECTOR2(2784, 385 + 192),		// �E�݂R�@�E��

		VECTOR2(2880, 385 + 192),		// �E�ǉ�
		VECTOR2(2820, 0),			// �E�Ǐ�
		VECTOR2(0, 0),			// �V��
		*/
		// �@
		VECTOR2(160, 2112),
		VECTOR2(160, 2432),

		VECTOR2(1408, 2432),

		VECTOR2(1408, 2592),
		VECTOR2(1568, 2592),
		VECTOR2(1568, 2624),
		VECTOR2(1408, 2624),

		VECTOR2(1408, 2720),
		VECTOR2(160, 2720),
		VECTOR2(160, 3040),
		VECTOR2(1760, 3040),

		VECTOR2(1760, 2816),
		VECTOR2(1600, 2816),
		VECTOR2(1600, 2784),
		VECTOR2(1760, 2784),

		VECTOR2(1760, 2432),
		VECTOR2(1600, 2432),
		VECTOR2(1600, 2400),
		VECTOR2(1760, 2400),

		VECTOR2(1760, 2112),
		VECTOR2(160, 2112),

		VECTOR2(999999, 999999),	// �n�ʂ̏I��


		// �A
		VECTOR2(1888, 160),
		VECTOR2(1888, 480),
		VECTOR2(2720, 480),
		VECTOR2(2720, 544),
		VECTOR2(1888, 544),
		VECTOR2(1888, 976),
		VECTOR2(2047, 976),
		VECTOR2(2047, 1008),
		VECTOR2(1888, 1008),
		VECTOR2(1888, 1216),
		VECTOR2(3040, 1216),
		VECTOR2(3040, 896),
		VECTOR2(2240, 896),
		VECTOR2(2240, 864),
		VECTOR2(3040, 864),
		VECTOR2(3040, 640),
		VECTOR2(2880, 640),
		VECTOR2(2880, 608),
		VECTOR2(3040, 608),
		VECTOR2(3040, 160),
		VECTOR2(1888, 160),

		VECTOR2(999999, 999999),	// �n�ʂ̏I��

		// �B
		VECTOR2(160, 160),
		VECTOR2(160, 1280),
		VECTOR2(480, 1280),
		VECTOR2(480, 480),
		VECTOR2(544, 480),
		VECTOR2(544, 1312),
		VECTOR2(160, 1312),
		VECTOR2(160, 2048),
		VECTOR2(512, 2048),
		VECTOR2(512, 1504),
		VECTOR2(864, 1504),

		// ��������ύX�̉\����

		VECTOR2(1120, 1504),
		VECTOR2(1120, 320),
		VECTOR2(1152, 320),
		VECTOR2(1152, 352),
		VECTOR2(1184, 352),
		VECTOR2(1184, 384),
		VECTOR2(1216, 384),
		VECTOR2(1216, 416),
		VECTOR2(1248, 416),
		VECTOR2(1248, 448),
		VECTOR2(1280, 448),
		VECTOR2(1280, 480),
		VECTOR2(1312, 480),
		VECTOR2(1312, 512),
		VECTOR2(1344, 512),
		VECTOR2(1344, 544),
		VECTOR2(1376, 544),
		VECTOR2(1376, 576),
		VECTOR2(1408, 576),
		VECTOR2(1408, 1824),
		VECTOR2(1376, 1824),
		VECTOR2(1376, 576),
		VECTOR2(1344, 576),
		VECTOR2(1344, 544),
		VECTOR2(1312, 544),
		VECTOR2(1312, 512),
		VECTOR2(1280, 512),
		VECTOR2(1280, 480),
		VECTOR2(1248, 480),
		VECTOR2(1248, 448),
		VECTOR2(1216, 448),
		VECTOR2(1216, 416),
		VECTOR2(1152, 416),
		VECTOR2(1152, 2048),

		VECTOR2(1280, 2048),
		VECTOR2(1280, 1888),
		VECTOR2(1312, 1888),
		VECTOR2(1312, 2048),
		VECTOR2(1728, 2048),
		VECTOR2(1728, 160),
		VECTOR2(896, 160),
		VECTOR2(896, 1120),
		VECTOR2(864, 1120),

		VECTOR2(864, 160),
		VECTOR2(384, 160),
		VECTOR2(384, 320),
		VECTOR2(352, 320),
		VECTOR2(352, 160),
		VECTOR2(160, 160),

		VECTOR2(999999, 999999),

		// �C
		VECTOR2(1760, 1248),
		VECTOR2(1760, 2080),
		VECTOR2(1952, 2080),
		VECTOR2(1952, 1504),
		VECTOR2(1984, 1504),
		VECTOR2(1984, 2112),
		VECTOR2(1824, 2112),
		VECTOR2(1824, 3040),
		VECTOR2(3040, 3040),
		VECTOR2(3040, 2752),
		VECTOR2(1952, 2752),
		VECTOR2(1952, 2720),
		VECTOR2(1984, 2720),
		VECTOR2(1984, 2688),
		VECTOR2(2016, 2688),
		VECTOR2(2016, 2656),
		VECTOR2(2048, 2656),
		VECTOR2(2048, 2624),
		VECTOR2(2080, 2624),
		VECTOR2(2080, 2592),
		VECTOR2(2112, 2592),
		VECTOR2(2112, 2560),
		VECTOR2(2144, 2560),
		VECTOR2(2144, 2528),
		VECTOR2(2176, 2528),
		VECTOR2(2176, 2496),
		VECTOR2(2208, 2496),
		VECTOR2(2208, 2464),
		VECTOR2(2240, 2464),
		VECTOR2(2240, 2432),
		VECTOR2(2272, 2432),
		VECTOR2(2272, 2400),
		VECTOR2(2304, 2400),
		VECTOR2(2304, 2368),
		VECTOR2(2336, 2368),
		VECTOR2(2336, 2336),
		VECTOR2(2368, 2336),
		VECTOR2(2368, 2304),
		VECTOR2(2400, 2304),
		VECTOR2(2400, 2272),
		VECTOR2(2432, 2272),
		VECTOR2(2432, 2240),
		VECTOR2(2464, 2240),
		VECTOR2(2464, 2208),
		VECTOR2(2496, 2208),
		VECTOR2(2496, 2176),
		VECTOR2(2528, 2176),
		VECTOR2(2528, 2144),
		VECTOR2(2560, 2144),
		VECTOR2(2560, 2112),
		VECTOR2(2592, 2112),
		VECTOR2(2592, 2080),
		VECTOR2(2624, 2080),
		VECTOR2(2624, 2720),
		VECTOR2(2880, 2720),
		VECTOR2(2880, 2624),
		VECTOR2(2912, 2624),
		VECTOR2(2912, 2720),
		VECTOR2(3040, 2720),
		VECTOR2(3040, 1248),
		VECTOR2(2816, 1248),
		VECTOR2(2816, 2528),
		VECTOR2(2784, 2528),
		VECTOR2(2784, 1248),
		VECTOR2(2528, 1248),
		VECTOR2(2528, 1920),
		VECTOR2(2496, 1920),
		VECTOR2(2496, 1792),
		VECTOR2(2464, 1792),
		VECTOR2(2464, 1760),
		VECTOR2(2432, 1760),
		VECTOR2(2432, 1728),
		VECTOR2(2400, 1728),
		VECTOR2(2400, 1696),
		VECTOR2(2368, 1696),
		VECTOR2(2368, 1664),
		VECTOR2(2336, 1664),
		VECTOR2(2336, 1632),
		VECTOR2(2304, 1632),
		VECTOR2(2304, 1600),
		VECTOR2(2272, 1600),
		VECTOR2(2272, 1568),
		VECTOR2(2240, 1568),
		VECTOR2(2240, 1536),
		VECTOR2(2208, 1536),
		VECTOR2(2208, 1504),
		VECTOR2(2176, 1504),
		VECTOR2(2176, 1472),
		VECTOR2(2144, 1472),
		VECTOR2(2144, 1440),
		VECTOR2(2112, 1440),
		VECTOR2(2112, 1408),
		VECTOR2(2080, 1408),
		VECTOR2(2080, 1376),
		VECTOR2(2048, 1376),
		VECTOR2(2048, 1344),
		VECTOR2(2026, 1344),
		VECTOR2(2026, 1312),
		VECTOR2(1984, 1312),
		VECTOR2(1984, 1280),
		VECTOR2(1952, 1280),
		VECTOR2(1952, 1248),
		VECTOR2(1760, 1248),

		VECTOR2(999999, 999999),

		VECTOR2(160, 2496),
		VECTOR2(160, 2656),
		VECTOR2(1344, 2656),
		VECTOR2(1344, 2496),
		VECTOR2(160, 2496),

		VECTOR2(999999, 999999),
	};

	VECTOR2 mapLine1[] = {
		VECTOR2(128, 96),
		VECTOR2(128, 672),
		VECTOR2(1237, 672),
		VECTOR2(1237, 96),
		VECTOR2(128, 96),

		VECTOR2(999999, 999999),	// �ŉ��w
	};
	
	//----------------------------------------------------------------------------
	// �C�x���g�}�b�v�z��@
	// 		�ʒu�o�w���W�A�x���W�p�A�C�x���g�h�c�A�C�x���g���(��ނ�_���a��)�A�ėp�J�E���^
	//		�C�x���g�h�c�@1:�o�b�̃X�^�[�g�ʒu�@2:�A�C�e���@3:�G
	//		�C�x���g��ށ@0x01 ��̐�    0x02 ���܂̓G    0x04 �I�I�J�~�̓G
	//					�@0x10 �~�}��    0x20 ��
	//
	//----------------------------------------------------------------------------
	CEvtMap  EvtMap0[] = {
		/*
		{ VECTOR2(1584, 484 + 192),      3, 0x01, 0 },		// ��̐�
		{ VECTOR2(1584 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(1584 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(1968, 484 + 192),      3, 0x01, 0 },		// ��̐�
		{ VECTOR2(1968 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(1968 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(2208, 484 + 192),      3, 0x01, 0 },		// ��̐�
		{ VECTOR2(2208 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(2208 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(600, 337 + 192),  3, 0x02, 0 },			// ���܂̓G
		{ VECTOR2(856, 337 + 192),  3, 0x02, 0 },
		{ VECTOR2(1448, 337 + 192), 3, 0x02, 0 },
		{ VECTOR2(2400, 337 + 192), 3, 0x02, 0 },
		{ VECTOR2(2600, 433 + 192), 3, 0x02, 0 },

		{ VECTOR2(1824, 288 + 192), 2, 0x10, 0 },			// �~�}��
		{ VECTOR2(2832, 336 + 192), 2, 0x20, 0 },			// ��
		*/
		{ VECTOR2(288,   2944), 1, 0x00, 0 },			// �X�^�[�g�ʒu

		{VECTOR2(320, 2250),         2, 0x10, 0},			// ��]�A�C�e��

		{VECTOR2(3020, 2944),        2, 0x20, 0},			// �}�b�v�ړ�

		{VECTOR2(1984, 300),         2, 0x400, 0},

		{VECTOR2(240, 1850),         2, 0x10, 0},

		{VECTOR2(1856, 1856),        2, 0x10, 0},

		{VECTOR2(1184, 512),         2, 0x800, 0},

		{VECTOR2(700, 2300),         3, 0x02, 0},			// �X���C���̃X�|�[���ʒu
		{VECTOR2(2300, 800),         3, 0x02, 0},
		{VECTOR2(2000, 400),         3, 0x02, 0},
		//{VECTOR2(1375, 2990),        3, 0x02, 0},
		{VECTOR2(475, 407),          3, 0x02, 0},
		{VECTOR2(250, 400),          3, 0x02, 0},
		{VECTOR2(620, 620),          3, 0x02, 0},
		{VECTOR2(224, 1450),         3, 0x02, 0},
		{VECTOR2(2855, 1600),        3, 0x02, 0},
		{VECTOR2(2974, 1853),        3, 0x02, 0},
		{VECTOR2(2560, 1633),        3, 0x02, 0},
		{VECTOR2(2263, 2310),        3, 0x02, 0},
		{VECTOR2(1800, 2024),        3, 0x02, 0},
		{VECTOR2(1200, 700),         3, 0x02, 0},
		{VECTOR2(1369, 1996),        3, 0x02, 0},
		{VECTOR2(960, 550),          3, 0x02, 0},
		{VECTOR2(1048, 1000),        3, 0x02, 0},

		{VECTOR2(2735, 560),         3, 0x04, 0},			// �R�E�����̃X�|�[���ʒu
		//{VECTOR2(1935, 918),         3, 0x04, 0},
		{VECTOR2(730, 400),          3, 0x04, 0},
		{VECTOR2(816, 1243),         3, 0x04, 0},
		{VECTOR2(390, 1400),         3, 0x04, 0},
		{VECTOR2(2893, 2122),        3, 0x04, 0},
		//{VECTOR2(2446, 2160),        3, 0x04, 0},
		
		// �Ŕ�
		{VECTOR2(570, 2745),         2, 0x1000, 0},
		{VECTOR2(1200, 2135),        2, 0x2000, 0},
		{VECTOR2(1140, 2745),        2, 0x4000, 0},
	};
	
	CEvtMap EvtMap1[] = {

		{VECTOR2(860, 600), 3, 0x08, 0},		// ��](180)

		{ VECTOR2(242,   98), 1, 0x00, 0 },			// �X�^�[�g�ʒu
	};


	// -------------------------------------------------------------------------------------------------
	//
	// �e�}�b�v�z��f�[�^����}�b�v�I�u�W�F�N�g�̍쐬
	//
	// -------------------------------------------------------------------------------------------------


	// �X�e�[�W1
	m_nMapNo = 0;
	MakeMap(m_pMapImage1, 1, 1, 3200, 3200,
		mapBakTbl0, sizeof(mapBakTbl0) / sizeof(int), mapForTbl0, sizeof(mapForTbl0) / sizeof(int), 
		mapLine0, (int)(sizeof(mapLine0) / sizeof(VECTOR2)), EvtMap0, sizeof(EvtMap0) / sizeof(CEvtMap));
	m_nMapSize[m_nMapNo] = VECTOR2(3200, 3200);
	m_RotateMap[m_nMapNo] = 0;

	// �X�e�[�W2
	m_nMapNo = 1;
	MakeMap(m_pMapImage2, 1, 1, 1366, 768,
		mapBakTbl1, sizeof(mapBakTbl1) / sizeof(int), mapForTbl1, sizeof(mapForTbl1) / sizeof(int),
		mapLine1, (int)(sizeof(mapLine1) / sizeof(VECTOR2)), EvtMap1, sizeof(EvtMap1) / sizeof(CEvtMap));
	m_nMapSize[m_nMapNo] = VECTOR2(1366, 768);
	m_RotateMap[m_nMapNo] = 0;

	// �J�n�}�b�v�̐ݒ�
	SetMap(0, VECTOR2(0, 0));

}

// ----------------------------------------------------------------------
//
// �}�b�v�̍쐬
//
//  CSpriteImage* pMapImage  �}�b�v�̃X�v���C�g�C���[�W
//  int imapX                ��ʂ̕��@�i�}�b�v�`�b�v���������j
//  int imapY                ��ʂ̍����i�}�b�v�`�b�v���������j
//  int imapchipWidth        ��̃}�b�v�`�b�v�̕�
//  int imapchipHeight       ��̃}�b�v�`�b�v�̍���
//  int mapBakTbl[]          �w�i�}�b�v�z��
//  int mapBakLen            �w�i�}�b�v�z��̗v�f��
//  int mapForTbl[]          �O�i�}�b�v�z��
//  int mapForLen            �O�i�}�b�v�z��̗v�f��
//  VECTOR2 mapLine[]    �}�b�v���z��
//  int mapLineLen           �}�b�v���z��̗v�f��
//  CEvtMap EvtMap[]         �C�x���g�}�b�v�z��
//  int EvtMapLen            �C�x���g�}�b�v�z��̗v�f��
//
// ----------------------------------------------------------------------
void  CMapProc::MakeMap(CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight,
	                    int  mapBakTbl[], int mapBakLen, int  mapForTbl[], int mapForLen, 
	                    VECTOR2 mapLine[], int mapLineLen, CEvtMap EvtMap[], int EvtMapLen )
{

	int i, j;
	VECTOR2 vSave;

	// �}�b�v�I�u�W�F�N�g�̐���
	m_pMapArray[m_nMapNo] = new CMap(m_pGMain, pMapImage, imapX, imapY, imapchipWidth, imapchipHeight);

	if (imapX*imapY != mapForLen)
	{
		MessageBoxA(0, "�O�i�}�b�v�̗v�f�����w��l�ƈقȂ�܂�", NULL, MB_OK);
		return;
	}
	if (imapX*imapY != mapBakLen)
	{
		MessageBoxA(0, "�w�i�}�b�v�̗v�f�����w��l�ƈقȂ�܂�", NULL, MB_OK);
		return;
	}

	// ��i�}�b�v�̍쐬
	m_pMapArray[m_nMapNo]->m_MapBakTbl = new int[mapBakLen];	// �v�f�����̔z����m��
	for (i = 0; i < mapBakLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_MapBakTbl[i] = mapBakTbl[i];
	}
	// �O�i�}�b�v�̍쐬
	m_pMapArray[m_nMapNo]->m_MapForTbl = new int[mapForLen];	// �v�f�����̔z����m��
	for (i = 0; i < mapForLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_MapForTbl[i] = mapForTbl[i];
	}

	// �}�b�v���I�u�W�F�N�g�̍쐬
	m_pMapArray[m_nMapNo]->m_nMapLnLength = 0;	// �܂��}�b�v���I�u�W�F�N�g�̗v�f�����v�Z
	for (i = 0; i < mapLineLen; i++)
	{
		for (i++; mapLine[i].x != 999999; i++)
		{
			m_pMapArray[m_nMapNo]->m_nMapLnLength++;
		}
	}
	m_pMapArray[m_nMapNo]->m_MapLn = new CMapLine[m_pMapArray[m_nMapNo]->m_nMapLnLength];	// �v�f�����̔z����m��
	for (i = 0, j=0; i < mapLineLen; i++)
	{
		vSave = mapLine[i];
		for (i++; mapLine[i].x != 999999; i++)
		{
			m_pMapArray[m_nMapNo]->m_MapLn[j].m_vStart = vSave;
			m_pMapArray[m_nMapNo]->m_MapLn[j].m_vEnd   = mapLine[i];
			j++;
			vSave = mapLine[i];
		}
	}

	// �@���̐ݒ� (�J�n�ʒu����I���ʒu�Ɍ������āA��������ɖ@�����o����)
	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++)
	{
		float ax, ay, nx, ny, len;
		ax = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x;
		ay = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y;
		nx = ay;
		ny = ax * -1;
		len = magnitude(VECTOR2(nx, ny));
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x = nx / len;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y = ny / len;
	}

	// �C�x���g�}�b�v�I�u�W�F�N�g�̍쐬
	m_pMapArray[m_nMapNo]->m_nEvtMapLength = EvtMapLen;
	m_pMapArray[m_nMapNo]->m_EvtMap = new CEvtMap[m_pMapArray[m_nMapNo]->m_nEvtMapLength];
	for (i = 0; i < EvtMapLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_EvtMap[i] = EvtMap[i];
	}
}
// ----------------------------------------------------------------------
//
// �}�b�v�ړ��̏����@�@���ɐڐG�����Ƃ��Ă΂��
//
// ----------------------------------------------------------------------
void CMapProc::MoveMap()
{
	VECTOR2 vPos = VECTOR2(0, 0);
	
	// ���̃}�b�v�̊J�n�ʒu
	vPos.x = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x;
	vPos.y = m_nMapSize[m_nMapNo].y - m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y;

		SetMap(++m_nMapNo, vPos);	// ���̃}�b�v�͂P
		m_pGMain->m_pEnmProc->SetNonActive();	// �G��S�ď����B�G�̔����t���O�����Z�b�g����
		m_pGMain->m_pEffectProc->SetNonActive();	// �A�C�e����S�ď����B�A�C�e���̔����t���O�����Z�b�g����
	
	
}

void CMapProc::BackMap()
{
	// ���̃}�b�v�̊J�n�ʒu
	VECTOR2 vPos = VECTOR2(0, 0);
	vPos.x = m_nMapSize[m_nMapNo - 1].x - m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x - 80;
	vPos.y = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y;

	SetMap(--m_nMapNo, vPos);	// ���̃}�b�v�͂P
	m_pGMain->m_pEnmProc->SetNonActive();	// �G��S�ď����B�G�̔����t���O�����Z�b�g����
	m_pGMain->m_pEffectProc->SetNonActive();	// �A�C�e����S�ď����B�A�C�e���̔����t���O�����Z�b�g����
	

}
// ----------------------------------------------------------------------
//
// �J�n�}�b�v�̐ݒ�
//
// �����@�F�@int no�@�}�b�v�ԍ�
//
// ----------------------------------------------------------------------
void CMapProc::SetMap(int no, VECTOR2 vPos)
{
	// �J�n�}�b�v�m�n
	m_nMapNo = no;

	// �}�b�v�p�X�v���C�g�̐ݒ�
	m_pSpriteMap->SetSrc( m_pMapArray[m_nMapNo]->m_pMapImage, 0, 0, m_pMapArray[m_nMapNo]->m_nMapchipWidth, m_pMapArray[m_nMapNo]->m_nMapchipHeight);

	m_pGMain->m_moveFlag = TRUE;
	m_RotateAnim = 0;
	m_pGMain->m_Rotate = 0;
	/*
	// �o�b�J�n�ʒu�̐ݒ�
	VECTOR2 vPos = VECTOR2(0, 0);
	*/
	// �Q�[���J�n���̂݁A�X�|�[���C�x���g�擾
	if (m_pGMain->m_StartSpownFlag) {
		for (DWORD i = 0; i < m_pMapArray[m_nMapNo]->m_nEvtMapLength; i++)
		{
			if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == 1)  // �o�b�̃X�^�[�g�ʒu
			{
				vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;  // �o�b�̃X�^�[�g���W
				m_pGMain->m_StartSpownFlag = FALSE;
				break;
			}
		}
	}
	
	// �o�b�J�n�ʒu��ݒ肵�A�g�o���񕜂���
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetPos(vPos);
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
}


// ----------------------------------------------------------------------
//
// �}�b�v�̍X�V
//
// ----------------------------------------------------------------------
void  CMapProc::Update()
{
	VECTOR2 vScr;

	// �o�b�̈ʒu����X�N���[�����W��ݒ肷��
	vScr.x = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().x + m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcWidth() / 2 - WINDOW_WIDTH / 2;
	if (vScr.x > m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH)
		vScr.x = m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH;
	if (vScr.x < 0) vScr.x = 0;

	vScr.y = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcHeight() / 2 - WINDOW_HEIGHT / 2;
	if (vScr.y > m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT)
		vScr.y = m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT;
	if (vScr.y < 0) vScr.y = 0;

	m_pGMain->m_vScroll = vScr;			// �X�N���[�����W��ݒ�

	// �}�b�v���̕`������邩�ǂ���
	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_P))
	{
		if (m_bDrawMapLine)
		{
			m_bDrawMapLine = FALSE;
		}
		else {
			m_bDrawMapLine = TRUE;
		}
	}
	DrawBack();  // �w�i�}�b�v�̕`��
}

// ----------------------------------------------------------------------
//
// �}�b�v�̍X�V�Q
//
// ----------------------------------------------------------------------
void  CMapProc::Update2()
{
	DrawFore();
	if (m_bDrawMapLine) DrawMapLine();
}

// ----------------------------------------------------------------------
//
// �C�x���g�}�b�v�̒T������
//
// ����
// �@�@int         nStart;		// �T���J�n�ʒu
// �@�@int         nEvtID;		// �C�x���g�h�c
// �@�@DWORD       dwEvtNo;		// �C�x���g�m��
// �@�@VECTOR2 vPos;		// ���W(OUT)
//     int         nNext;       // ���J�n�ʒu(OUT) �ŏI�ɒB������@-1 
//
// �߂�l
//�@�@TRUE:��������  FALSE:������Ȃ�
//
// ----------------------------------------------------------------------
BOOL  CMapProc::SearchEvt(int nStart, int nEvtID, DWORD dwEvtNo, VECTOR2& vPos, int& nNext)
{
	BOOL bRet = FALSE;

	if (nStart < 0 || nStart >= m_pMapArray[m_nMapNo]->m_nEvtMapLength)
	{
		nNext = -1;
		return bRet;
	}

	for (DWORD i = nStart; i < m_pMapArray[m_nMapNo]->m_nEvtMapLength; i++)
	{
		if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == nEvtID &&	    // �C�x���g�}�b�v�@ID�@�G�̏o���ʒu
			m_pMapArray[m_nMapNo]->m_EvtMap[i].m_dwEvtNo & dwEvtNo)			// �C�x���g�}�b�v�@NO
		{
			vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;	// �����ʒu
			nNext = i + 1;  // ���̊J�n�ʒu��ݒ�
			bRet = TRUE;
			break;
		}
	}
	if (!bRet)
	{
		nNext = -1;
	}

	return bRet;
}

// ----------------------------------------------------------------------
//
// �}�b�v�̔w�i�̕`��
//
// ----------------------------------------------------------------------
void  CMapProc::DrawBack()
{

	// �X�e�[�W�w�i�̕`��i�S�����X�N���[���j�@�|�|�|�|�|�|�|�|�|�|
	VECTOR2 scr;

	scr.x = WINDOW_WIDTH  - ((int)(m_pGMain->m_vScroll.x / 2)) % WINDOW_WIDTH;
	scr.y = WINDOW_HEIGHT - ((int)(m_pGMain->m_vScroll.y / 2)) % WINDOW_HEIGHT;

	m_pSpriteBak->Draw(m_pBakImage, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y, scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, 0, 0, WINDOW_HEIGHT - scr.y, WINDOW_WIDTH - scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, 0, scr.y, WINDOW_WIDTH - scr.x, 0, scr.x, WINDOW_HEIGHT - scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, scr.y, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y);

	// �}�b�v�w�i�̕`��@�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
	int x, y, no;

	for (y = 0; y < m_pMapArray[m_nMapNo]->m_nMapY; y++)
	{
		for (x = 0; x < m_pMapArray[m_nMapNo]->m_nMapX; x++)
		{
			no = m_pMapArray[m_nMapNo]->m_MapBakTbl[y*m_pMapArray[m_nMapNo]->m_nMapX + x];
			if ( no == -1)
			{
				;  		// �}�b�v�̖������͕`�悵�Ȃ�
			}else{
				m_pSpriteMap->m_ofX = no % 1000 * m_pMapArray[m_nMapNo]->m_nMapchipWidth;
				m_pSpriteMap->m_ofY = no / 1000 * m_pMapArray[m_nMapNo]->m_nMapchipHeight;

				int cx, cy;

				cx = m_pSpriteMap->GetDestWidth() / 2;
				cy = m_pSpriteMap->GetDestHeight() / 2;

				if (m_pGMain->m_Rotate > m_RotateAnim) {	// ��]���I�����ĂȂ��ꍇ
					m_RotateAnim++;
					m_pGMain->m_moveFlag = FALSE;
					m_pGMain->m_pEnmProc->RotateCenter(1);
					m_pGMain->m_pWeaponProc->RotateCenter(1);
					m_pGMain->m_pEffectProc->m_pItemProc->RotateCenter(1);
				}
				else if (m_pGMain->m_Rotate < m_RotateAnim) {	// ��]���I�����ĂȂ��ꍇ
					m_RotateAnim--;
					m_pGMain->m_moveFlag = FALSE;
					m_pGMain->m_pEnmProc->RotateCenter(2);
					m_pGMain->m_pWeaponProc->RotateCenter(2);
					m_pGMain->m_pEffectProc->m_pItemProc->RotateCenter(2);
				}
				else {
					if (m_OneSet) {						// ��]�����ׂďI��(1��]�ɂ�1����s)
						m_pGMain->m_moveFlag = TRUE;
						m_OneSet = FALSE;
						m_pGMain->m_pEnmProc->Rotate(Rota);
						m_pGMain->m_pWeaponProc->Rotate(Rota);
						m_pGMain->m_pEffectProc->m_pItemProc->Rotate(Rota);
					}
				}

				// ��]�}�g���b�N�X�̍쐬
				MATRIX4X4 mWorld, mCenter, mRot, mScale;
				mWorld = XMMatrixTranslation((x*m_pMapArray[m_nMapNo]->m_nMapchipWidth - m_pGMain->m_vScroll.x), (y*m_pMapArray[m_nMapNo]->m_nMapchipHeight - m_pGMain->m_vScroll.y), 0);
				mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
				mCenter = XMMatrixTranslation(cx, cy, 0);
				mRot = XMMatrixRotationZ(XMConvertToRadians(m_RotateAnim));
				mWorld = mRot * mCenter * mScale * mWorld;
				mCenter = XMMatrixTranslation(-cx, -cy, 0);
				mWorld = mCenter * mWorld;

				m_pSpriteMap->Draw(mWorld);
				//m_pSpriteMap->Draw(x*m_pMapArray[m_nMapNo]->m_nMapchipWidth - m_pGMain->m_vScroll.x, y*m_pMapArray[m_nMapNo]->m_nMapchipHeight - m_pGMain->m_vScroll.y);
			}
		}
	}
}
// ----------------------------------------------------------------------
//
// �}�b�v�̑O�i�̕`��
//
// ----------------------------------------------------------------------
void  CMapProc::DrawFore()
{
	int x, y, no;

	for (y = 0; y < m_pMapArray[m_nMapNo]->m_nMapY; y++)
	{
		for (x = 0; x < m_pMapArray[m_nMapNo]->m_nMapX; x++)
		{
			no = m_pMapArray[m_nMapNo]->m_MapForTbl[y*m_pMapArray[m_nMapNo]->m_nMapX + x];
			if (no == -1)
			{
				;		// �}�b�v�̖������͕`�悵�Ȃ�
			}else {
				m_pSpriteMap->m_ofX = no % 1000 * m_pMapArray[m_nMapNo]->m_nMapchipWidth;
				m_pSpriteMap->m_ofY = no / 1000 * m_pMapArray[m_nMapNo]->m_nMapchipHeight;
				m_pSpriteMap->Draw(x*m_pMapArray[m_nMapNo]->m_nMapchipWidth - m_pGMain->m_vScroll.x, y*m_pMapArray[m_nMapNo]->m_nMapchipHeight - m_pGMain->m_vScroll.y);
			}
		}
	}
}

// ----------------------------------------------------------------------
//
// �}�b�v���̕`��
//
// ----------------------------------------------------------------------
void  CMapProc::DrawMapLine()
{
	int i;

	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++)
	{
		m_pSpriteLine->DrawLine(m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x - m_pGMain->m_vScroll.x, m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y - m_pGMain->m_vScroll.y,
										m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x - m_pGMain->m_vScroll.x, m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y - m_pGMain->m_vScroll.y, 3, RGB(255, 0, 0));
		float cx = (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x) / 2;
		float cy = (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y) / 2;
		float nx = cx + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x * 10.0f;
		float ny = cy + m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y * 10.0f;
		m_pSpriteLine->DrawLine( cx - m_pGMain->m_vScroll.x, cy - m_pGMain->m_vScroll.y, nx - m_pGMain->m_vScroll.x, ny - m_pGMain->m_vScroll.y, 3, RGB(255, 0, 0));
	}
}

// �����F�}�b�v�`�b�v�̉�](�ʏ�270)
void CMapProc::Rotate90(DWORD rotate) {
	int i;
	int x;

	// ���ׂĂ̖@���i�����蔻��j���v�Z
	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x;												// �@���̊J�n�n�_
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y = m_nMapSize[m_nMapNo].y - x;

		x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x;													// �@���̏I���n�_
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y = m_nMapSize[m_nMapNo].y - x;

		if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x > 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x--;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y--;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x < 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x++;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y++;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y > 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x++;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y--;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y < 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x--;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y++;
		}
	}
	m_OneSet = TRUE;
	m_pGMain->m_Rotate += rotate;

	if (m_pGMain->m_Rotate >= 360) {
		m_pGMain->m_Rotate -= 360;
	}
	m_RotateMap[m_nMapNo] = m_pGMain->m_Rotate;

	
	Rota = 90;
	//m_pGMain->m_pEnmProc->Rotate(90);
}

// �����F�}�b�v�`�b�v�̉�](�ʏ�180)
void CMapProc::Rotate180(DWORD rotate) {
	int i;
	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x  = m_nMapSize[m_nMapNo].x - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y = m_nMapSize[m_nMapNo].y - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x = m_nMapSize[m_nMapNo].x - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y = m_nMapSize[m_nMapNo].y - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal *= -1;

	}
	m_OneSet = TRUE;
	m_pGMain->m_Rotate += rotate;

	if (m_pGMain->m_Rotate >= 360) {
		m_pGMain->m_Rotate -= 360;
	}
	m_RotateMap[m_nMapNo] = m_pGMain->m_Rotate;

	Rota = 180;
	//m_pGMain->m_pEnmProc->Rotate(180);
}

// �����F�}�b�v�`�b�v�̉�](�ʏ�90)
void CMapProc::Rotate270(DWORD rotate) {
	int i;
	int y;
	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		y = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x = m_nMapSize[m_nMapNo].x - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y = y;
		y = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x = m_nMapSize[m_nMapNo].x - m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.y = y;

		if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x > 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x--;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y++;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x < 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x++;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y--;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y > 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x--;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y--;
		}
		else if (m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y < 0) {
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x++;
			m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y++;
		}
	}
	m_OneSet = TRUE;
	m_pGMain->m_Rotate += rotate;

	if (m_pGMain->m_Rotate >= 360) {
		m_pGMain->m_Rotate -= 360;
	}
	m_RotateMap[m_nMapNo] = m_pGMain->m_Rotate;

	Rota = 270;
	//m_pGMain->m_pEnmProc->Rotate(270);
}

// ----------------------------------------------------------------------------------------
//
// �}�b�v�̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
//
//   �@�@�}�b�v����˂������Ă��邩�`�F�b�N����
//   �A�@�}�b�v���ɋߐڂ��Ă��邩�i�L�������H������ł��邩�j�`�F�b�N����
//
//   �˂������Ă��邩�ߐڂ��Ă���Ƃ��A�����lm_vPosUp��@�������ɐH�����ݕ������A�߂��Ă��
//
//   �@�@�߂�l�F�˂������Ă��邩�ߐڂ��Ă���Ƃ��^�BpHitmapline:�ڐG�����}�b�v���̃A�h���X���Ԃ�
// ----------------------------------------------------------------------------------------
BOOL CMapProc::isCollisionMoveMap( CBaseObj* obj, CMapLine* &pHitmapline)
{
	int i, n, rw;
	BOOL bRet = FALSE;
	VECTOR2 vHitpos = VECTOR2(0,0);
	VECTOR2 vHpw = VECTOR2(0,0);
	float dist, dw;

	pHitmapline = NULL;
	dist = 999999;
	for (n = 0, i = 0; i< m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// �@�@�}�b�v���Ƃ̓˂���������
		if (rw) {	// �˂������Ă���Ƃ�
			bRet = rw;
			dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
			n = i;
			vHitpos = vHpw;
			break;
		}
		else {
			rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// �A�@�}�b�v���Ƃ̋ߐڔ���i�L�������}�b�v���ɐH������ł��邩�j
			if (rw) {
				bRet = rw;
				dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				if (dist > dw) {						// ��ԋ߂��}�b�v����T��
					n = i;
					dist = dw;
					vHitpos = vHpw;
				}
			}
		}
	}

	i = n;
	if (bRet) {	// �˂������Ă��邩�ߐڂ��Ă���Ƃ��A�@�������ɐH�����ݕ������A�߂��Ă��

		pHitmapline = &(m_pMapArray[m_nMapNo]->m_MapLn[i]);  // �ڐG�����}�b�v���̃A�h���X
		obj->SetPosUp(obj->GetPosUp() + VECTOR2(
			round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
			round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
	}

	// �ڐG���Ă���Ƃ��݂̂Q��ڂ̃`�F�b�N���s��
	if (bRet) {
		bRet = FALSE;	// ��U�N�����[����
		dist = 999999;
		for (n = 0, i = 0; i<m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
			rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// �}�b�v���Ƃ̓˂���������
			if (rw) {	// �˂������Ă���Ƃ�
				bRet = rw;
				dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				n = i;
				vHitpos = vHpw;
				break;
			}
			else {
				rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// �}�b�v���Ƃ̋ߐڔ���i�L�������}�b�v���ɐH������ł��邩�j
				if (rw) {
					bRet = rw;
					dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
					if (dist > dw) {						// ��ԋ߂��}�b�v����T��
						n = i;
						dist = dw;
						vHitpos = vHpw;
					}
				}
			}
		}

		i = n;
		if (bRet) {	// �˂������Ă��邩�ߐڂ��Ă���Ƃ��Am_vPosUp��@�������ɐH�����ݕ������A�߂��Ă��
			obj->SetPosUp(obj->GetPosUp() + VECTOR2(
				round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
				round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
		}
		bRet = TRUE;	// �ēx�Z�b�g����
	}

	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// �}�b�v���Ƃ̓˂���������
//
// �@�߂�l�F�������Ă���Ƃ��^�B��_���W��hitpos�ɕԂ�
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapcross(CMapLine map, CBaseObj* obj, VECTOR2 &hitpos)
{

	BOOL bRet = FALSE;

	// �����P�@���P�C���Q   �E�E�E�E�@�}�b�v��
	VECTOR2  m1 = map.m_vStart, m2 = map.m_vEnd;
	// �����Q�@�����P�C�����Q �E�E�E�@�I�u�W�F�N�g�ړ�
	VECTOR2  oj1 = obj->GetCenterPos();
	VECTOR2  oj2 = obj->GetCenterPos() + obj->GetPosUp();

	// �Q�����̌����`�F�b�N
	bRet = CheckLinecross(m1, m2, oj1, oj2, hitpos);

	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// �Q�����̌����`�F�b�N
// �@�������ƒ������̌����`�F�b�N�B
//
// �@�߂�l�F�������Ă���Ƃ��^�B��_���W��hitpos�ɕԂ�
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckLinecross(VECTOR2 a1, VECTOR2 a2, VECTOR2 b1, VECTOR2 b2, VECTOR2 &hitpos)
{

	BOOL bRet = FALSE;

	// ��_�@����
	VECTOR2  ap = VECTOR2(0, 0);
	float d1, d2;

	// �����̒������O�̂Ƃ�
	if ((a1.x == a2.x && a1.y == a2.y) ||
		(b1.x == b2.x && b1.x == b2.y)) {
		return bRet;
	}

	// ��_�����邩
	float dev = (a2.y - a1.y) * (b2.x - b1.x) - (a2.x - a1.x) * (b2.y - b1.y);
	if (dev == 0) {// ���s���̂Ƃ�
		return bRet;
	}

	// ��_�����߂�
	d1 = (b1.y*b2.x - b1.x*b2.y);
	d2 = (a1.y*a2.x - a1.x*a2.y);

	ap.x = d1*(a2.x - a1.x) - d2*(b2.x - b1.x);
	ap.x /= dev;
	ap.y = d1*(a2.y - a1.y) - d2*(b2.y - b1.y);
	ap.y /= dev;

	// ��_�������͈̔͂̒��ɂ��邩
	if ((CheckRange(a1.x, a2.x, ap.x) && CheckRange(a1.y, a2.y, ap.y)) &&
		(CheckRange(b1.x, b2.x, ap.x) && CheckRange(b1.y, b2.y, ap.y))) {
		bRet = TRUE;
	}

	if (bRet) {
		hitpos = ap;
	}

	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// �͈̓`�F�b�N
//
// �@�߂�l�Fpt�����Ƃ��̊Ԃɓ����Ă���Ƃ��^�B
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckRange(float l, float r, float pt)
{
	float low, hi;
	float mgn = 0.05f;	// �덷

	if (l <= r) {
		low = l;
		hi = r;
	}
	else {
		low = r;
		hi = l;
	}
	low -= mgn;
	hi += mgn;

	if (low <= pt && pt <= hi) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


// ----------------------------------------------------------------------------------------
//
// �}�b�v���Ƃ̋ߐڃ`�F�b�N
//
// �@�L�������}�b�v���ɐH������ł��邩�`�F�b�N�B
//
// �@�߂�l�F�H������ł���Ƃ��^�B�ڐG�ʒu��vHitpos�ɕԂ�
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapnear(CMapLine MapLn, CBaseObj* obj, VECTOR2 &vHitpos)
{
	BOOL bRet = FALSE;

	// �@�@�L�����ƃ}�b�v���̖@�������Ƃ̐H�����݃`�F�b�N���s�� --------------------------------
	// �L�����̒��S����A�}�b�v���̖@�������ɃL�����̑傫�����̒���������
	VECTOR2  p0, p1, p2;
	p0 = obj->GetCenterPos() + obj->GetPosUp();
	p1.x = p0.x - MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p1.y = p0.y - MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;
	p2.x = p0.x + MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p2.y = p0.y + MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;

	// �����ƃ}�b�v���Ƃ̌����`�F�b�N
	bRet = CheckLinecross( MapLn.m_vStart,  MapLn.m_vEnd, p1, p2, vHitpos);

	// �A�@�������Ă��Ȃ��ꍇ�A�L�����Ɛ����������Ƃ̐H�����݃`�F�b�N���s��---------------------
	if (!bRet) {
		// �L�����̒��S����A�����������ɃL�����̑傫�����̒���������
		p0 = obj->GetCenterPos() + obj->GetPosUp();

		p1.x = p0.x - 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p1.y = p0.y - 1 * obj->GetSprite()->GetSrcHeight() / 2;
		p2.x = p0.x + 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p2.y = p0.y + 1 * obj->GetSprite()->GetSrcHeight() / 2;

		// �����ƃ}�b�v���Ƃ̌����`�F�b�N
		bRet = CheckLinecross(MapLn.m_vStart, MapLn.m_vEnd, p1, p2, vHitpos);
	}
	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// �}�b�v���Ƃ̋��������߂�
//
// �@�߂�l�F �}�b�v���Ƃ̋����B�������A�@���������v���X�ƂȂ�B
//
// ----------------------------------------------------------------------------------------
float  CMapProc::GetDistance(CMapLine MapLn, CBaseObj* obj)
{
	VECTOR2  b = obj->GetCenterPos() + obj->GetPosUp() - MapLn.m_vStart;
	float len;

	// ���ς́A�x�N�g���̖@�������̋����ɂȂ�
	// (�@���̒������P�̂���)
	len = GetDot( MapLn.m_vNormal, b );

	return len;
}

// ----------------------------------------------------------------------------------------
//
// �Q�_�Ԃ̋��������߂�
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetLength(VECTOR2 p1, VECTOR2 p2)
{
	return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

// ----------------------------------------------------------------------------------------
//
// �x�N�g���̊O�ς����߂�
// �i�Q�����̏ꍇ�A�O�ς̓X�J���[�l�ƂȂ�j
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetCross(VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.y - a.y * b.x;
}

// ----------------------------------------------------------------------------------------
//
// �x�N�g���̓��ς����߂�
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetDot( VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.x + a.y * b.y;
}


