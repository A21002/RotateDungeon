//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		マップの処理
//																Map.h
//=============================================================================
#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

//ヘッダーファイルのインクルード
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
// マップ線クラスの定義
// 		始点座標｛Ｘ座標、Ｙ座標｝、終点座標｛Ｘ座標、Ｙ座標｝・・・・
// 		法線座標 (開始位置から終了位置に向かって、左手方向に法線が出来る)
//----------------------------------------------------------------------------
class  CMapLine
{
public:
	VECTOR2 m_vStart;		// 始点座標
	VECTOR2 m_vEnd;			// 終点座標
	VECTOR2 m_vNormal;		// 法線座標 (開始位置から終了位置に向かって、左手方向に法線が出来る)

public:
	// コンストラクタ
	CMapLine()
	{
		m_vStart  = VECTOR2(0, 0);
		m_vEnd    = VECTOR2(0, 0);
		m_vNormal = VECTOR2(0, 0);
	}
};
//----------------------------------------------------------------------------
// イベントマップクラスの定義
// 		座標｛Ｘ座標、Ｙ座標｝、イベントＩＤ、イベントＮｏ(種類を論理和で)、汎用カウンタ
//		イベントＩＤ　1:ＰＣのスタート位置　2:アイテム　3:敵
//		イベントＮｏ　0x01 泉の水    0x02 がまの敵    0x04 オオカミの敵
//					　0x10 救急箱    0x20 扉
//----------------------------------------------------------------------------
class  CEvtMap
{
public:
	VECTOR2 m_vStart;		// 座標
	int         m_nEvtID;		// イベントＩＤ
	DWORD       m_dwEvtNo;		// イベントＮｏ
	int         m_nCnt1;		// 汎用カウンタ
public:
	// コンストラクタ
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
// マップ配列クラスの定義
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
	int*                     m_MapBakTbl;	// マップ配列　背景
	int*                     m_MapForTbl;	// マップ配列　前景
	CMapLine*                m_MapLn;		// マップライン配列
	CEvtMap*                 m_EvtMap;		// イベントマップ配列

public:
	// コンストラクタ
	CMap(CGameMain*	pGMain, CSpriteImage* pMapImage, int imapX, int imapY, int imapchipWidth, int imapchipHeight);
	~CMap();
};

//----------------------------------------------------------------------------
// マッププロシージャクラスの定義
//----------------------------------------------------------------------------
class  CMapProc
{
protected:
	CGameMain*	m_pGMain;

	
	
	// マップ線の描画フラグ
	BOOL  m_bDrawMapLine;

	
	// マップイメージ
	CSpriteImage* m_pMapImage1;
	CSpriteImage* m_pMapImage2;
	CSpriteImage* m_pBakImage;
	// マップスプライト
	CSprite*      m_pSpriteMap;
	CSprite*      m_pSpriteBak;
	CSprite*      m_pSpriteLine;

	DWORD m_RotateAnim;

public:
	// コンストラクタ
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

	// マップ配列
	CMap* m_pMapArray[MAP_MAX];
	// マップ名
	int   m_nMapNo;
	// 回転数
	
	// マップサイズ配列
	VECTOR2 m_nMapSize[10];
	// 法線の回転状態
	DWORD m_RotateMap[10];

	BOOL m_OneSet;
	int  Rota;
};
