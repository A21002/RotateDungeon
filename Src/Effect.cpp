//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		映像効果の処理
//																Effect.cpp
//=============================================================================

#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
//
//	映像効果メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CEffectProc::CEffectProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pEffectBomProc = new CEffectBomProc(m_pGMain);	// 映像効果・爆発プロシージャ
	m_pProcArray.push_back(m_pEffectBomProc);     // プロシージャをプロシージャ配列に登録する

	m_pItemProc = new CItemProc(m_pGMain);				// アイテムプロシージャ
	m_pProcArray.push_back(m_pItemProc);		// プロシージャをプロシージャ配列に登録する

}
//============================================================================
//   プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
//   プロシージャの開始フラグをリセットする
//============================================================================
void  CEffectProc::SetNonActive()
{

	//オブジェクトポインタ配列の探索
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// 下位のプロシージャポインタ配列の探索
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag(0);				// 開始フラグのリセット
		((CEffectProc*)m_pProcArray[i])->SetNonActive();      // 下位のプロシージャの処理（再帰処理）
	}
}


// ================================================================================================================================
// 
// 各映像効果プロシージャ毎の処理
// 
// ================================================================================================================================


//------------------------------------------------------------------------
//
//	映像効果・爆発プロシージャのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CEffectBomProc::CEffectBomProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < EFFECT_BOM_MAX; i++)
	{
		m_pObjArray.push_back(new CEffectBomObj(m_pGMain));	// m_pObjArrayに映像効果・爆発オブジェクトを生成する
	}

}

//-----------------------------------------------------------------------------
// 映像効果・爆発プロシージャの開始
//
//   VECTOR2 vPos    発生位置
//
//   戻り値　　正常に発生 : TRUE    発生しなかった : FALSE
//-----------------------------------------------------------------------------
BOOL	CEffectBomProc::Start(VECTOR2 vPos)
{
	BOOL bRet = FALSE;

	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive() )
		{
			m_pObjArray[i]->Start(vPos);	// 映像効果・爆発の発生
			m_pGMain->m_pSeHit->Play();
			bRet = TRUE;
			break;
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------
//
//	映像効果・爆発オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CEffectBomObj::CEffectBomObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageSprite, 144, 480, 64, 64);
	m_nAnimNum = 7;

}
// ---------------------------------------------------------------------------
//
// 映像効果・爆発オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CEffectBomObj::~CEffectBomObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// 映像効果・爆発オブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL	CEffectBomObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	m_dwStatus = NORMAL;
	m_dwStatusSub = STOP;
	m_dwStatusPhase = 0;

	m_vPos = vPos;	// 発生位置

	m_nAnimIdx = 0;

	return TRUE;
}
//-----------------------------------------------------------------------------
// 映像効果・爆発オブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEffectBomObj::Update()
{
	if (m_bActive)
	{
		if (AnimCountup() == 1)
		{
			m_bActive = FALSE;
		}
		Draw();

	}
}
// ==============================================================================================================

// =================================================================================
//------------------------------------------------------------------------
//
//	アイテムプロシージャのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CItemProc::CItemProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CItemObj(m_pGMain));	// m_pObjArrayにアイテムを生成する
	}
	m_nStartFlag = 0;
}

void CItemProc::Start(VECTOR2 vPos, DWORD dwNo) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (!m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->Start(vPos, dwNo);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// アイテムプロシージャの開始
//
//   戻り値　　なし
//-----------------------------------------------------------------------------
void	CItemProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、救急箱を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x10 救急箱	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x10, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION180);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、ドア縦（Move）を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x20 ドア	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x20, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVEDOOR_VER);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、ドア縦（Back）を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x40 ドア	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x40, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBACKDOOR_VER);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、ドア横（Move）を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x80 ドア	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x80, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVEDOOR_WID);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、ドア横（Move）を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x100 ドア	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x100, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMBACKDOOR_WID);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、90度を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x200 90度	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x200, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION90);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、270度を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x400 270度	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x400, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMROTATION270);
						break;
					}
				}
			}
		}

		// イベントマップを探索し、270度を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x800 鍵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x800, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMKEY);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x1000 移動看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x1000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMMOVESIGN);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x2000 攻撃看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x2000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMSHOTSIGN);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x4000 ジャンプ看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x4000, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEMJUMPSIGN);
						break;
					}
				}
			}
		}

		for (DWORD j = 0; j < m_pObjArray.size(); j++)
		{
			
			if (m_pObjArray[j]->GetActive())
			{
				for (DWORD i = 0; i < m_pGMain->m_Rotate / 90; i++) {
					m_pObjArray[j]->RotatePos(90);
					//break;
				}
				if (m_pGMain->m_Rotate / 90 != 0) {
					m_pObjArray[j]->SetPos(VECTOR2(m_pObjArray[j]->GetPos().x - m_pObjArray[j]->GetSprite()->GetSrcWidth(),
						m_pObjArray[j]->GetPos().y - m_pObjArray[j]->GetSprite()->GetSrcHeight()));
				}
				
			}
			
		}
		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}

	
}

//------------------------------------------------------------------------
//
//	アイテムオブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CItemObj::CItemObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageItem);
	m_nAnimNum = 2;

}
// ---------------------------------------------------------------------------
//
// アイテムオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CItemObj::~CItemObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// アイテムオブジェクトの開始
//
//   VECTOR2 vPos    発生位置
//   DWORD       dwNo    アイテム種別
//
//   戻り値　　TRUE
//-----------------------------------------------------------------------------
BOOL	CItemObj::Start(VECTOR2 vPos, DWORD dwNo)
{
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_dwStatusSub = dwNo;
	waitTime = 0;
	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);
	RotateFlag = FALSE;
	SignFlag = FALSE;

	switch (m_dwStatusSub)
	{
	case  ITEMROTATION180:     // 救急箱
		m_pSprite->SetSrc(96, 64, 64, 64);
		flag = 0;
		break;

	case  ITEMMOVEDOOR_VER:      // ドア
		m_pSprite->SetSrc(0, 0, 64, 96);
		m_dwStatusSub = ITEMMOVEDOOR;
		m_nAnimNum = 1;
		flag = 1;
		break;

	case  ITEMBACKDOOR_VER:		 // ドア(Back)
		m_pSprite->SetSrc(0, 0, 64, 96);
		m_dwStatusSub = ITEMBACKDOOR;
		flag = 1;
		break;

	case  ITEMMOVEDOOR_WID:
		m_pSprite->SetSrc(0, 96, 96, 64);
		m_dwStatusSub = ITEMMOVEDOOR;
		flag = 2;
		break;

	case  ITEMBACKDOOR_WID:
		m_pSprite->SetSrc(0, 96, 96, 64);
		m_dwStatusSub = ITEMBACKDOOR;
		flag = 2;
		break;

	case  ITEMROTATION90:
		m_pSprite->SetSrc(96, 0, 64, 64);
		flag = 0;
		break;

	case  ITEMROTATION270:
		m_pSprite->SetSrc(96, 128, 64, 64);
		flag = 0;
		break;

	case  ITEMKEY:
		m_pSprite->SetSrc(224, 0, 64, 64);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMRECOVER:
		m_pSprite->SetSrc(0, 192, 48, 48);
		flag = 0;
		m_nAnimNum = 2;
		break;

	case  ITEMROTATEDOWN180:
		m_pSprite->SetSrc(96, 64, 64, 64);
		flag = 0;
		m_nAnimNum = 2;
		m_vPosUp = VECTOR2(0, 4);
		break;

	case  ITEMMOVESIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMSHOTSIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMJUMPSIGN:
		m_pSprite->SetSrc(0, 285, 285, 285);
		flag = 0;
		m_nAnimNum = 1;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// アイテムオブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CItemObj::Update()
{
	if (!m_bActive) return;

	if (m_pGMain->m_moveFlag) {
		if (isCollision(m_pGMain->m_pPcProc->GetPcObjPtr()) && m_pGMain->m_MoveItemWaitTime <= 0) // PCとの接触判定
		{
			switch (m_dwStatusSub)
			{
			case  ITEMMOVEDOOR:      // ドア
				m_pGMain->m_DoorTachFlag = TRUE;
				m_pGMain->m_pSePower->Play();
				if (m_pGMain->m_KeyFlag) {
					m_pGMain->m_TextUIFlag = TRUE;
					RotateFlag = TRUE;
				}
				else {
					m_pGMain->m_TextUIFlag = TRUE;
					RotateFlag = TRUE;
				}
				break;

			case  ITEMBACKDOOR:
				m_pGMain->m_pSePower->Play();
				m_pGMain->m_pMapProc->BackMap();
				break;

			case  ITEMROTATION180:     
			/*
			m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp( m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
			m_pGMain->m_pSePower->Play();
			break;
			*/
				m_pGMain->m_pMapProc->Rotate180(180);
				break;

			case  ITEMROTATION90:
				m_pGMain->m_pMapProc->Rotate90(270);
				break;

			case ITEMROTATION270:
				m_pGMain->m_pMapProc->Rotate270(90);
				break;

			case  ITEMKEY:
				m_pGMain->m_KeyFlag = TRUE;
				m_bActive = FALSE;
				m_pGMain->m_pKey->Play();
				break;

			case  ITEMRECOVER:
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp());
				m_pGMain->m_pSePower->Play();
				m_bActive = FALSE;
				break;

			case  ITEMROTATEDOWN180:
				m_pGMain->m_pMapProc->Rotate180(180);
				m_bActive = FALSE;
				break;

			case  ITEMMOVESIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;

			case  ITEMSHOTSIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;

			case  ITEMJUMPSIGN:
				if (!SignFlag) {
					m_nAnimNum = 6;
				}
				break;
			}
			m_pGMain->m_MoveItemWaitTime = 60;

		}

		if (m_pGMain->m_MoveItemWaitTime > 0) {
			m_pGMain->m_MoveItemWaitTime--;
		}

		if (m_dwStatusSub == ITEMROTATEDOWN180) {
			BOOL bRet;
			CMapLine*  pHitmapline = NULL;
			// マップ線との接触判定と適切な位置への移動
			bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
			if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
			{
				m_vPos += m_vPosUp;
			}
		}
		
		if (m_nAnimIdx >= 5 && !SignFlag) {
			switch (m_dwStatusSub) {
			case  ITEMMOVESIGN:
				m_pSprite->SetSrc(0, 570, 285, 285);
				m_nAnimNum = 14;
				break;

			case  ITEMSHOTSIGN:
				m_pSprite->SetSrc(0, 855, 285, 285);
				m_nAnimNum = 8;
				break;

			case  ITEMJUMPSIGN:
				m_pSprite->SetSrc(0, 1140, 285, 285);
				m_nAnimNum = 7;
				break;
			}
			m_nAnimIdx = 0;
			SignFlag = TRUE;
		}

		AnimCountup();
	}
	if (!m_pGMain->m_TextUIFlag && RotateFlag) {
		if (m_pGMain->m_KeyFlag) {
			m_pGMain->m_pMapProc->MoveMap();
			RotateFlag = FALSE;
		}
		else {
			m_pGMain->m_pMapProc->Rotate180(180);
			RotateFlag = FALSE;
		}
		
	}
	Draw();

}

void CItemProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CItemObj::RotatePos(DWORD rotate) {
	if (m_dwStatusSub == ITEMMOVESIGN || m_dwStatusSub == ITEMJUMPSIGN || m_dwStatusSub == ITEMSHOTSIGN) {
		m_bActive = FALSE;
	}
	DWORD x = m_vPos.x;
	switch (rotate) {
	case 90:
		
		m_vPos.x = m_vPos.y;
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - x;
		break;

	case 180:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.x - m_pSprite->GetSrcWidth();
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - m_vPos.y - m_pSprite->GetSrcHeight();
		break;

	case 270:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.y - m_pSprite->GetSrcHeight();
		m_vPos.y = x;

		if (flag == 1) {
			m_pSprite->SetSrc(0, 96, 96, 64);
			flag = 2;
		}
		else if (flag == 2) {
			m_pSprite->SetSrc(0, 0, 64, 96);
			flag = 1;
		}
		break;
	}
	m_fRotate = 0;
}

void CItemProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			if (m_pGMain->m_pMapProc->m_nMapNo != 1) {
				if (flag == 1) {
					m_pObjArray[j]->RotateDrawUp();
				}
				else if (flag == 2) {
					m_pObjArray[j]->RotateDrawDown();
				}
				//break;
			}
		}
	}
}
