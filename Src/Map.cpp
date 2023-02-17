//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		マップの処理
//																Map.cpp
//=============================================================================
#include "Map.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Effect.h"
#include "Weapon.h"


// CMapクラス   --------------------------------------------------------------------------------------------
// コンストラクタ
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
//  デストラクタ
CMap::~CMap()
{
	SAFE_DELETE_ARRAY(m_MapBakTbl);
	SAFE_DELETE_ARRAY(m_MapForTbl);
	SAFE_DELETE_ARRAY(m_MapLn);
	SAFE_DELETE_ARRAY(m_EvtMap);
}

//  CMapProcクラス  ---------------------------------------------------------------------------------------
//  コンストラクタ
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
	m_pSpriteMap  = new CSprite(m_pGMain->m_pShader); // スプライトオブジェクトの生成
	m_pSpriteBak  = new CSprite(m_pBakImage);
	m_pSpriteLine = new CSprite(m_pGMain->m_pShader);
	m_RotateAnim = 0;

	InitMap();
}

//  デストラクタ
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
// マップ配列からマップオブジェクトの作成
//
//   mapBakTbl 背景マップ配列
//             (注)マップのチップ番号  上３桁はＹ位置、下３桁はＸ位置
//             (注)マップのチップ番号-1番は、マップチップを置かない場所
//
//   mapForTbl 前景マップ配列
//             (注)マップのチップ番号  上３桁はＹ位置、下３桁はＸ位置
//             (注)マップのチップ番号-1番は、マップチップを置かない場所
//
//   mapLine   マップ線配列配列
//
//   EvtMap    ベントマップ
//
// ----------------------------------------------------------------------
void CMapProc::InitMap()
{
	// -----------------------------------------------------------------------------------------------
	//
	// ステージ	1　泉のステージ
	//
	// -----------------------------------------------------------------------------------------------

	// 背景マップ　-----------------------------------------------------------
	int  mapBakTbl0[] = {
		0
	};

	int mapBakTbl1[] = {
		0
	};

	// 前景マップ　-----------------------------------------------------------
	int  mapForTbl0[] = {
		-1
	};

	int mapForTbl1[] = {
		-1
	};


	//----------------------------------------------------------------------------
	// マップ線配列　
	// 		開始位置｛Ｘ座標、Ｙ座標｝、途中・終了位置｛Ｘ座標、Ｙ座標｝・・・・
	// 		一つの記述の終了{999999},
	//
	//----------------------------------------------------------------------------
	VECTOR2  mapLine0[] = {
		// 地面の設定
		/*
		VECTOR2(0, 0),			// 左壁
		VECTOR2(-48, 385 + 192),			// 左壁下
		VECTOR2(240, 385 + 192),			// 窪み１　左上
		VECTOR2(336, 433 + 192),			// 窪み１　左下
		VECTOR2(432, 433 + 192),			// 窪み１　右下
		VECTOR2(528, 385 + 192),			// 窪み１　右上

		VECTOR2(720, 385 + 192),			// 崖１　左上
		VECTOR2(720, 481 + 192),			// 崖１　左下
		VECTOR2(816, 481 + 192),			// 崖１　右下
		VECTOR2(816, 385 + 192),			// 崖１　右上


		VECTOR2(960, 385 + 192),			// 窪み２　左上
		VECTOR2(1104, 529 + 192),		// 窪み２　左下
		VECTOR2(1200, 529 + 192),		// 窪み２　右下
		VECTOR2(1344, 385 + 192),		// 窪み２　右上

		VECTOR2(1584, 385 + 192),		// 崖２　左上
		VECTOR2(1584, 529 + 192),		// 崖２　左下
		VECTOR2(1728, 529 + 192),		// 崖２　右下
		VECTOR2(1728, 385 + 192),		// 崖２　右上

		VECTOR2(1968, 385 + 192),		// 崖３　左上
		VECTOR2(1968, 529 + 192),		// 崖３　左下
		VECTOR2(2112, 529 + 192),		// 崖３　右下
		VECTOR2(2112, 385 + 192),		// 崖３　右上

		VECTOR2(2208, 385 + 192),		// 崖４　左上
		VECTOR2(2208, 529 + 192),		// 崖４　左下
		VECTOR2(2352, 529 + 192),		// 崖４　右下
		VECTOR2(2352, 385 + 192),		// 崖４　右上

		VECTOR2(2448, 385 + 192),		// 窪み３　左上
		VECTOR2(2544, 481 + 192),		// 窪み３　左下
		VECTOR2(2688, 481 + 192),		// 窪み３　右下
		VECTOR2(2784, 385 + 192),		// 窪み３　右上

		VECTOR2(2880, 385 + 192),		// 右壁下
		VECTOR2(2820, 0),			// 右壁上
		VECTOR2(0, 0),			// 天井
		*/
		// ①
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

		VECTOR2(999999, 999999),	// 地面の終了


		// ②
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

		VECTOR2(999999, 999999),	// 地面の終了

		// ③
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

		// ここから変更の可能性大

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

		// ④
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

		VECTOR2(999999, 999999),	// 最下層
	};
	
	//----------------------------------------------------------------------------
	// イベントマップ配列　
	// 		位置｛Ｘ座標、Ｙ座標｝、イベントＩＤ、イベント種類(種類を論理和で)、汎用カウンタ
	//		イベントＩＤ　1:ＰＣのスタート位置　2:アイテム　3:敵
	//		イベント種類　0x01 泉の水    0x02 がまの敵    0x04 オオカミの敵
	//					　0x10 救急箱    0x20 扉
	//
	//----------------------------------------------------------------------------
	CEvtMap  EvtMap0[] = {
		/*
		{ VECTOR2(1584, 484 + 192),      3, 0x01, 0 },		// 泉の水
		{ VECTOR2(1584 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(1584 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(1968, 484 + 192),      3, 0x01, 0 },		// 泉の水
		{ VECTOR2(1968 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(1968 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(2208, 484 + 192),      3, 0x01, 0 },		// 泉の水
		{ VECTOR2(2208 + 48, 484 + 192), 3, 0x01, 0 },
		{ VECTOR2(2208 + 96, 484 + 192), 3, 0x01, 0 },

		{ VECTOR2(600, 337 + 192),  3, 0x02, 0 },			// がまの敵
		{ VECTOR2(856, 337 + 192),  3, 0x02, 0 },
		{ VECTOR2(1448, 337 + 192), 3, 0x02, 0 },
		{ VECTOR2(2400, 337 + 192), 3, 0x02, 0 },
		{ VECTOR2(2600, 433 + 192), 3, 0x02, 0 },

		{ VECTOR2(1824, 288 + 192), 2, 0x10, 0 },			// 救急箱
		{ VECTOR2(2832, 336 + 192), 2, 0x20, 0 },			// 扉
		*/
		{ VECTOR2(288,   2944), 1, 0x00, 0 },			// スタート位置

		{VECTOR2(320, 2250),         2, 0x10, 0},			// 回転アイテム

		{VECTOR2(3020, 2944),        2, 0x20, 0},			// マップ移動

		{VECTOR2(1984, 300),         2, 0x400, 0},

		{VECTOR2(240, 1850),         2, 0x10, 0},

		{VECTOR2(1856, 1856),        2, 0x10, 0},

		{VECTOR2(1184, 512),         2, 0x800, 0},

		{VECTOR2(700, 2300),         3, 0x02, 0},			// スライムのスポーン位置
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

		{VECTOR2(2735, 560),         3, 0x04, 0},			// コウモリのスポーン位置
		//{VECTOR2(1935, 918),         3, 0x04, 0},
		{VECTOR2(730, 400),          3, 0x04, 0},
		{VECTOR2(816, 1243),         3, 0x04, 0},
		{VECTOR2(390, 1400),         3, 0x04, 0},
		{VECTOR2(2893, 2122),        3, 0x04, 0},
		//{VECTOR2(2446, 2160),        3, 0x04, 0},
		
		// 看板
		{VECTOR2(570, 2745),         2, 0x1000, 0},
		{VECTOR2(1200, 2135),        2, 0x2000, 0},
		{VECTOR2(1140, 2745),        2, 0x4000, 0},
	};
	
	CEvtMap EvtMap1[] = {

		{VECTOR2(860, 600), 3, 0x08, 0},		// 回転(180)

		{ VECTOR2(242,   98), 1, 0x00, 0 },			// スタート位置
	};


	// -------------------------------------------------------------------------------------------------
	//
	// 各マップ配列データからマップオブジェクトの作成
	//
	// -------------------------------------------------------------------------------------------------


	// ステージ1
	m_nMapNo = 0;
	MakeMap(m_pMapImage1, 1, 1, 3200, 3200,
		mapBakTbl0, sizeof(mapBakTbl0) / sizeof(int), mapForTbl0, sizeof(mapForTbl0) / sizeof(int), 
		mapLine0, (int)(sizeof(mapLine0) / sizeof(VECTOR2)), EvtMap0, sizeof(EvtMap0) / sizeof(CEvtMap));
	m_nMapSize[m_nMapNo] = VECTOR2(3200, 3200);
	m_RotateMap[m_nMapNo] = 0;

	// ステージ2
	m_nMapNo = 1;
	MakeMap(m_pMapImage2, 1, 1, 1366, 768,
		mapBakTbl1, sizeof(mapBakTbl1) / sizeof(int), mapForTbl1, sizeof(mapForTbl1) / sizeof(int),
		mapLine1, (int)(sizeof(mapLine1) / sizeof(VECTOR2)), EvtMap1, sizeof(EvtMap1) / sizeof(CEvtMap));
	m_nMapSize[m_nMapNo] = VECTOR2(1366, 768);
	m_RotateMap[m_nMapNo] = 0;

	// 開始マップの設定
	SetMap(0, VECTOR2(0, 0));

}

// ----------------------------------------------------------------------
//
// マップの作成
//
//  CSpriteImage* pMapImage  マップのスプライトイメージ
//  int imapX                画面の幅　（マップチップが何個分か）
//  int imapY                画面の高さ（マップチップが何個分か）
//  int imapchipWidth        一つのマップチップの幅
//  int imapchipHeight       一つのマップチップの高さ
//  int mapBakTbl[]          背景マップ配列
//  int mapBakLen            背景マップ配列の要素数
//  int mapForTbl[]          前景マップ配列
//  int mapForLen            前景マップ配列の要素数
//  VECTOR2 mapLine[]    マップ線配列
//  int mapLineLen           マップ線配列の要素数
//  CEvtMap EvtMap[]         イベントマップ配列
//  int EvtMapLen            イベントマップ配列の要素数
//
// ----------------------------------------------------------------------
void  CMapProc::MakeMap(CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight,
	                    int  mapBakTbl[], int mapBakLen, int  mapForTbl[], int mapForLen, 
	                    VECTOR2 mapLine[], int mapLineLen, CEvtMap EvtMap[], int EvtMapLen )
{

	int i, j;
	VECTOR2 vSave;

	// マップオブジェクトの生成
	m_pMapArray[m_nMapNo] = new CMap(m_pGMain, pMapImage, imapX, imapY, imapchipWidth, imapchipHeight);

	if (imapX*imapY != mapForLen)
	{
		MessageBoxA(0, "前景マップの要素数が指定値と異なります", NULL, MB_OK);
		return;
	}
	if (imapX*imapY != mapBakLen)
	{
		MessageBoxA(0, "背景マップの要素数が指定値と異なります", NULL, MB_OK);
		return;
	}

	// 後景マップの作成
	m_pMapArray[m_nMapNo]->m_MapBakTbl = new int[mapBakLen];	// 要素数分の配列を確保
	for (i = 0; i < mapBakLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_MapBakTbl[i] = mapBakTbl[i];
	}
	// 前景マップの作成
	m_pMapArray[m_nMapNo]->m_MapForTbl = new int[mapForLen];	// 要素数分の配列を確保
	for (i = 0; i < mapForLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_MapForTbl[i] = mapForTbl[i];
	}

	// マップ線オブジェクトの作成
	m_pMapArray[m_nMapNo]->m_nMapLnLength = 0;	// まずマップ線オブジェクトの要素数を計算
	for (i = 0; i < mapLineLen; i++)
	{
		for (i++; mapLine[i].x != 999999; i++)
		{
			m_pMapArray[m_nMapNo]->m_nMapLnLength++;
		}
	}
	m_pMapArray[m_nMapNo]->m_MapLn = new CMapLine[m_pMapArray[m_nMapNo]->m_nMapLnLength];	// 要素数分の配列を確保
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

	// 法線の設定 (開始位置から終了位置に向かって、左手方向に法線が出来る)
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

	// イベントマップオブジェクトの作成
	m_pMapArray[m_nMapNo]->m_nEvtMapLength = EvtMapLen;
	m_pMapArray[m_nMapNo]->m_EvtMap = new CEvtMap[m_pMapArray[m_nMapNo]->m_nEvtMapLength];
	for (i = 0; i < EvtMapLen; i++)
	{
		m_pMapArray[m_nMapNo]->m_EvtMap[i] = EvtMap[i];
	}
}
// ----------------------------------------------------------------------
//
// マップ移動の処理　　扉に接触したとき呼ばれる
//
// ----------------------------------------------------------------------
void CMapProc::MoveMap()
{
	VECTOR2 vPos = VECTOR2(0, 0);
	
	// 次のマップの開始位置
	vPos.x = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x;
	vPos.y = m_nMapSize[m_nMapNo].y - m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y;

		SetMap(++m_nMapNo, vPos);	// 次のマップは１
		m_pGMain->m_pEnmProc->SetNonActive();	// 敵を全て消す。敵の発生フラグをリセットする
		m_pGMain->m_pEffectProc->SetNonActive();	// アイテムを全て消す。アイテムの発生フラグをリセットする
	
	
}

void CMapProc::BackMap()
{
	// 次のマップの開始位置
	VECTOR2 vPos = VECTOR2(0, 0);
	vPos.x = m_nMapSize[m_nMapNo - 1].x - m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x - 80;
	vPos.y = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y;

	SetMap(--m_nMapNo, vPos);	// 次のマップは１
	m_pGMain->m_pEnmProc->SetNonActive();	// 敵を全て消す。敵の発生フラグをリセットする
	m_pGMain->m_pEffectProc->SetNonActive();	// アイテムを全て消す。アイテムの発生フラグをリセットする
	

}
// ----------------------------------------------------------------------
//
// 開始マップの設定
//
// 引数　：　int no　マップ番号
//
// ----------------------------------------------------------------------
void CMapProc::SetMap(int no, VECTOR2 vPos)
{
	// 開始マップＮＯ
	m_nMapNo = no;

	// マップ用スプライトの設定
	m_pSpriteMap->SetSrc( m_pMapArray[m_nMapNo]->m_pMapImage, 0, 0, m_pMapArray[m_nMapNo]->m_nMapchipWidth, m_pMapArray[m_nMapNo]->m_nMapchipHeight);

	m_pGMain->m_moveFlag = TRUE;
	m_RotateAnim = 0;
	m_pGMain->m_Rotate = 0;
	/*
	// ＰＣ開始位置の設定
	VECTOR2 vPos = VECTOR2(0, 0);
	*/
	// ゲーム開始時のみ、スポーンイベント取得
	if (m_pGMain->m_StartSpownFlag) {
		for (DWORD i = 0; i < m_pMapArray[m_nMapNo]->m_nEvtMapLength; i++)
		{
			if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == 1)  // ＰＣのスタート位置
			{
				vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;  // ＰＣのスタート座標
				m_pGMain->m_StartSpownFlag = FALSE;
				break;
			}
		}
	}
	
	// ＰＣ開始位置を設定し、ＨＰを回復する
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetPos(vPos);
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
}


// ----------------------------------------------------------------------
//
// マップの更新
//
// ----------------------------------------------------------------------
void  CMapProc::Update()
{
	VECTOR2 vScr;

	// ＰＣの位置からスクロール座標を設定する
	vScr.x = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().x + m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcWidth() / 2 - WINDOW_WIDTH / 2;
	if (vScr.x > m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH)
		vScr.x = m_pMapArray[m_nMapNo]->m_nMapX * m_pMapArray[m_nMapNo]->m_nMapchipWidth - WINDOW_WIDTH;
	if (vScr.x < 0) vScr.x = 0;

	vScr.y = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetOf().y + m_pGMain->m_pPcProc->GetPcObjPtr()->GetSprite()->GetSrcHeight() / 2 - WINDOW_HEIGHT / 2;
	if (vScr.y > m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT)
		vScr.y = m_pMapArray[m_nMapNo]->m_nMapY * m_pMapArray[m_nMapNo]->m_nMapchipHeight - WINDOW_HEIGHT;
	if (vScr.y < 0) vScr.y = 0;

	m_pGMain->m_vScroll = vScr;			// スクロール座標を設定

	// マップ線の描画をするかどうか
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
	DrawBack();  // 背景マップの描画
}

// ----------------------------------------------------------------------
//
// マップの更新２
//
// ----------------------------------------------------------------------
void  CMapProc::Update2()
{
	DrawFore();
	if (m_bDrawMapLine) DrawMapLine();
}

// ----------------------------------------------------------------------
//
// イベントマップの探索処理
//
// 引数
// 　　int         nStart;		// 探索開始位置
// 　　int         nEvtID;		// イベントＩＤ
// 　　DWORD       dwEvtNo;		// イベントＮｏ
// 　　VECTOR2 vPos;		// 座標(OUT)
//     int         nNext;       // 次開始位置(OUT) 最終に達したら　-1 
//
// 戻り値
//　　TRUE:見つかった  FALSE:見つからない
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
		if (m_pMapArray[m_nMapNo]->m_EvtMap[i].m_nEvtID == nEvtID &&	    // イベントマップ　ID　敵の出現位置
			m_pMapArray[m_nMapNo]->m_EvtMap[i].m_dwEvtNo & dwEvtNo)			// イベントマップ　NO
		{
			vPos = m_pMapArray[m_nMapNo]->m_EvtMap[i].m_vStart;	// 発生位置
			nNext = i + 1;  // 次の開始位置を設定
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
// マップの背景の描画
//
// ----------------------------------------------------------------------
void  CMapProc::DrawBack()
{

	// ステージ背景の描画（４方向スクロール）　－－－－－－－－－－
	VECTOR2 scr;

	scr.x = WINDOW_WIDTH  - ((int)(m_pGMain->m_vScroll.x / 2)) % WINDOW_WIDTH;
	scr.y = WINDOW_HEIGHT - ((int)(m_pGMain->m_vScroll.y / 2)) % WINDOW_HEIGHT;

	m_pSpriteBak->Draw(m_pBakImage, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y, scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, 0, 0, WINDOW_HEIGHT - scr.y, WINDOW_WIDTH - scr.x, scr.y);
	m_pSpriteBak->Draw(m_pBakImage, 0, scr.y, WINDOW_WIDTH - scr.x, 0, scr.x, WINDOW_HEIGHT - scr.y);
	m_pSpriteBak->Draw(m_pBakImage, scr.x, scr.y, 0, 0, WINDOW_WIDTH - scr.x, WINDOW_HEIGHT - scr.y);

	// マップ背景の描画　－－－－－－－－－－－－－－－－－－－－－－
	int x, y, no;

	for (y = 0; y < m_pMapArray[m_nMapNo]->m_nMapY; y++)
	{
		for (x = 0; x < m_pMapArray[m_nMapNo]->m_nMapX; x++)
		{
			no = m_pMapArray[m_nMapNo]->m_MapBakTbl[y*m_pMapArray[m_nMapNo]->m_nMapX + x];
			if ( no == -1)
			{
				;  		// マップの無い所は描画しない
			}else{
				m_pSpriteMap->m_ofX = no % 1000 * m_pMapArray[m_nMapNo]->m_nMapchipWidth;
				m_pSpriteMap->m_ofY = no / 1000 * m_pMapArray[m_nMapNo]->m_nMapchipHeight;

				int cx, cy;

				cx = m_pSpriteMap->GetDestWidth() / 2;
				cy = m_pSpriteMap->GetDestHeight() / 2;

				if (m_pGMain->m_Rotate > m_RotateAnim) {	// 回転が終了してない場合
					m_RotateAnim++;
					m_pGMain->m_moveFlag = FALSE;
					m_pGMain->m_pEnmProc->RotateCenter(1);
					m_pGMain->m_pWeaponProc->RotateCenter(1);
					m_pGMain->m_pEffectProc->m_pItemProc->RotateCenter(1);
				}
				else if (m_pGMain->m_Rotate < m_RotateAnim) {	// 回転が終了してない場合
					m_RotateAnim--;
					m_pGMain->m_moveFlag = FALSE;
					m_pGMain->m_pEnmProc->RotateCenter(2);
					m_pGMain->m_pWeaponProc->RotateCenter(2);
					m_pGMain->m_pEffectProc->m_pItemProc->RotateCenter(2);
				}
				else {
					if (m_OneSet) {						// 回転がすべて終了(1回転につき1回実行)
						m_pGMain->m_moveFlag = TRUE;
						m_OneSet = FALSE;
						m_pGMain->m_pEnmProc->Rotate(Rota);
						m_pGMain->m_pWeaponProc->Rotate(Rota);
						m_pGMain->m_pEffectProc->m_pItemProc->Rotate(Rota);
					}
				}

				// 回転マトリックスの作成
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
// マップの前景の描画
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
				;		// マップの無い所は描画しない
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
// マップ線の描画
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

// 引数：マップチップの回転(通常270)
void CMapProc::Rotate90(DWORD rotate) {
	int i;
	int x;

	// すべての法線（当たり判定）を計算
	for (i = 0; i < m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
		x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x;												// 法線の開始地点
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y;
		m_pMapArray[m_nMapNo]->m_MapLn[i].m_vStart.y = m_nMapSize[m_nMapNo].y - x;

		x = m_pMapArray[m_nMapNo]->m_MapLn[i].m_vEnd.x;													// 法線の終了地点
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

// 引数：マップチップの回転(通常180)
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

// 引数：マップチップの回転(通常90)
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
// マップの接触判定と適切な位置への移動
//
//   ①　マップ線を突き抜けているかチェックする
//   ②　マップ線に近接しているか（キャラが食い込んでいるか）チェックする
//
//   突き抜けているか近接しているとき、増分値m_vPosUpを法線方向に食い込み分だけ、戻してやる
//
//   　　戻り値：突き抜けているか近接しているとき真。pHitmapline:接触したマップ線のアドレスが返る
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
		rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// ①　マップ線との突き抜け判定
		if (rw) {	// 突き抜けているとき
			bRet = rw;
			dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
			n = i;
			vHitpos = vHpw;
			break;
		}
		else {
			rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// ②　マップ線との近接判定（キャラがマップ線に食い込んでいるか）
			if (rw) {
				bRet = rw;
				dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				if (dist > dw) {						// 一番近いマップ線を探す
					n = i;
					dist = dw;
					vHitpos = vHpw;
				}
			}
		}
	}

	i = n;
	if (bRet) {	// 突き抜けているか近接しているとき、法線方向に食い込み分だけ、戻してやる

		pHitmapline = &(m_pMapArray[m_nMapNo]->m_MapLn[i]);  // 接触したマップ線のアドレス
		obj->SetPosUp(obj->GetPosUp() + VECTOR2(
			round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
			round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
	}

	// 接触しているときのみ２回目のチェックを行う
	if (bRet) {
		bRet = FALSE;	// 一旦クリヤーする
		dist = 999999;
		for (n = 0, i = 0; i<m_pMapArray[m_nMapNo]->m_nMapLnLength; i++) {
			rw = CheckMapcross(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);		// マップ線との突き抜け判定
			if (rw) {	// 突き抜けているとき
				bRet = rw;
				dist = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
				n = i;
				vHitpos = vHpw;
				break;
			}
			else {
				rw = CheckMapnear(m_pMapArray[m_nMapNo]->m_MapLn[i], obj, vHpw);	// マップ線との近接判定（キャラがマップ線に食い込んでいるか）
				if (rw) {
					bRet = rw;
					dw = GetDistance(m_pMapArray[m_nMapNo]->m_MapLn[i], obj);
					if (dist > dw) {						// 一番近いマップ線を探す
						n = i;
						dist = dw;
						vHitpos = vHpw;
					}
				}
			}
		}

		i = n;
		if (bRet) {	// 突き抜けているか近接しているとき、m_vPosUpを法線方向に食い込み分だけ、戻してやる
			obj->SetPosUp(obj->GetPosUp() + VECTOR2(
				round(-(dist - obj->GetSprite()->GetSrcWidth() / 2)   * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.x),
				round(-(dist - obj->GetSprite()->GetSrcHeight() / 2)  * m_pMapArray[m_nMapNo]->m_MapLn[i].m_vNormal.y)));
		}
		bRet = TRUE;	// 再度セットする
	}

	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// マップ線との突き抜け判定
//
// 　戻り値：交差しているとき真。交点座標がhitposに返る
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapcross(CMapLine map, CBaseObj* obj, VECTOR2 &hitpos)
{

	BOOL bRet = FALSE;

	// 直線１　ｍ１，ｍ２   ・・・・　マップ線
	VECTOR2  m1 = map.m_vStart, m2 = map.m_vEnd;
	// 直線２　ｏｊ１，ｏｊ２ ・・・　オブジェクト移動
	VECTOR2  oj1 = obj->GetCenterPos();
	VECTOR2  oj2 = obj->GetCenterPos() + obj->GetPosUp();

	// ２直線の交差チェック
	bRet = CheckLinecross(m1, m2, oj1, oj2, hitpos);

	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// ２直線の交差チェック
// 　直線ａと直線ｂの交差チェック。
//
// 　戻り値：交差しているとき真。交点座標がhitposに返る
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckLinecross(VECTOR2 a1, VECTOR2 a2, VECTOR2 b1, VECTOR2 b2, VECTOR2 &hitpos)
{

	BOOL bRet = FALSE;

	// 交点　ａｐ
	VECTOR2  ap = VECTOR2(0, 0);
	float d1, d2;

	// 直線の長さが０のとき
	if ((a1.x == a2.x && a1.y == a2.y) ||
		(b1.x == b2.x && b1.x == b2.y)) {
		return bRet;
	}

	// 交点があるか
	float dev = (a2.y - a1.y) * (b2.x - b1.x) - (a2.x - a1.x) * (b2.y - b1.y);
	if (dev == 0) {// 平行線のとき
		return bRet;
	}

	// 交点を求める
	d1 = (b1.y*b2.x - b1.x*b2.y);
	d2 = (a1.y*a2.x - a1.x*a2.y);

	ap.x = d1*(a2.x - a1.x) - d2*(b2.x - b1.x);
	ap.x /= dev;
	ap.y = d1*(a2.y - a1.y) - d2*(b2.y - b1.y);
	ap.y /= dev;

	// 交点が直線の範囲の中にあるか
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
// 範囲チェック
//
// 　戻り値：ptがｌとｒの間に入っているとき真。
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckRange(float l, float r, float pt)
{
	float low, hi;
	float mgn = 0.05f;	// 誤差

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
// マップ線との近接チェック
//
// 　キャラがマップ線に食い込んでいるかチェック。
//
// 　戻り値：食い込んでいるとき真。接触位置がvHitposに返る
//
// ----------------------------------------------------------------------------------------
BOOL CMapProc::CheckMapnear(CMapLine MapLn, CBaseObj* obj, VECTOR2 &vHitpos)
{
	BOOL bRet = FALSE;

	// ①　キャラとマップ線の法線方向との食い込みチェックを行う --------------------------------
	// キャラの中心から、マップ線の法線方向にキャラの大きさ分の直線を引く
	VECTOR2  p0, p1, p2;
	p0 = obj->GetCenterPos() + obj->GetPosUp();
	p1.x = p0.x - MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p1.y = p0.y - MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;
	p2.x = p0.x + MapLn.m_vNormal.x * obj->GetSprite()->GetSrcWidth() / 2;
	p2.y = p0.y + MapLn.m_vNormal.y * obj->GetSprite()->GetSrcHeight() / 2;

	// 直線とマップ線との交差チェック
	bRet = CheckLinecross( MapLn.m_vStart,  MapLn.m_vEnd, p1, p2, vHitpos);

	// ②　交差していない場合、キャラと垂直下方向との食い込みチェックを行う---------------------
	if (!bRet) {
		// キャラの中心から、垂直下方向にキャラの大きさ分の直線を引く
		p0 = obj->GetCenterPos() + obj->GetPosUp();

		p1.x = p0.x - 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p1.y = p0.y - 1 * obj->GetSprite()->GetSrcHeight() / 2;
		p2.x = p0.x + 0 * obj->GetSprite()->GetSrcWidth() / 2;
		p2.y = p0.y + 1 * obj->GetSprite()->GetSrcHeight() / 2;

		// 直線とマップ線との交差チェック
		bRet = CheckLinecross(MapLn.m_vStart, MapLn.m_vEnd, p1, p2, vHitpos);
	}
	return bRet;
}

// ----------------------------------------------------------------------------------------
//
// マップ線との距離を求める
//
// 　戻り値： マップ線との距離。ただし、法線方向がプラスとなる。
//
// ----------------------------------------------------------------------------------------
float  CMapProc::GetDistance(CMapLine MapLn, CBaseObj* obj)
{
	VECTOR2  b = obj->GetCenterPos() + obj->GetPosUp() - MapLn.m_vStart;
	float len;

	// 内積は、ベクトルの法線方向の距離になる
	// (法線の長さが１のため)
	len = GetDot( MapLn.m_vNormal, b );

	return len;
}

// ----------------------------------------------------------------------------------------
//
// ２点間の距離を求める
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetLength(VECTOR2 p1, VECTOR2 p2)
{
	return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

// ----------------------------------------------------------------------------------------
//
// ベクトルの外積を求める
// （２次元の場合、外積はスカラー値となる）
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetCross(VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.y - a.y * b.x;
}

// ----------------------------------------------------------------------------------------
//
// ベクトルの内積を求める
//
// ----------------------------------------------------------------------------------------
float CMapProc::GetDot( VECTOR2 a, VECTOR2 b)
{
	return  a.x * b.x + a.y * b.y;
}


