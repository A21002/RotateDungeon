//=============================================================================
//		映像効果の処理
//																Item.cpp
//=============================================================================
#include "Item.h"
#include "Playchar.h"
#include "Map.h"

//======================================================================
// アイテムプロシージャ
//======================================================================
//------------------------------------------------------------------------
//	アイテムプロシージャのコンストラクタ	
//------------------------------------------------------------------------
CItemProc::CItemProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CItemObj(m_pGMain));	// m_pObjArrayにアイテムを生成する
	}
	m_nStartFlag = 0;
}

//------------------------------------------------------------------------
//	アイテムの手動発生処理	
//------------------------------------------------------------------------
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
// アイテムプロシージャの開始直後の生成処理
//-----------------------------------------------------------------------------
void	CItemProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	if (m_nStartFlag == 0)
	{
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x01 ドア	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x01, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos, ITEM_DOOR);
						break;
					}
				}
			}
		}

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x02 鍵)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x02, vPos, nNext))
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
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x04 90度	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x04, vPos, nNext))
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

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x08 180度回転	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x08, vPos, nNext))
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

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x10 270度	)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x10, vPos, nNext))
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

		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x20 移動看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x20, vPos, nNext))
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
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x40 攻撃看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x40, vPos, nNext))
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
			// イベントマップの探索　 ( EvtID:2　アイテムの出現位置,  EvtNo:0x80 ジャンプ看板)
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 2, 0x80, vPos, nNext))
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
		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
// 全てのオブジェクトを回転させる
// 
// 引数
// DWORD	m_bIsRotateChengeFlag
// UP	: 右回転
// DOWN	: 左回転
//------------------------------------------------------------------------
void CItemProc::RotateCenter(DWORD m_bIsRotateChengeFlag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			// ボス戦でなければ回転する
			if (m_pGMain->m_pMapProc->m_nMapNo != 1) {
				if (m_bIsRotateChengeFlag == UP) {
					m_pObjArray[j]->RotateDrawUp();
				}
				else if (m_bIsRotateChengeFlag == DOWN) {
					m_pObjArray[j]->RotateDrawDown();
				}
			}
		}
	}
}

//------------------------------------------------------------------------
// 全てのオブジェクトを回転後の座標にセットする
//------------------------------------------------------------------------
void CItemProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
		}
	}
}

//======================================================================
// アイテムオブジェクト
//======================================================================
//------------------------------------------------------------------------
//	アイテムオブジェクトのコンストラクタ	
//------------------------------------------------------------------------
CItemObj::CItemObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageItem);
	m_nAnimNum = 2;
	m_bIsCheckKeyFlag = TRUE;
	SignFlag = TRUE;
	m_bIsRotateChengeFlag = TRUE;
}

// ---------------------------------------------------------------------------
// アイテムオブジェクトのデストラクタ
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
	m_vPos = vPos;	// 発生位置
	m_vPosUp = VECTOR2(0, 0);
	m_bIsCheckKeyFlag = FALSE;
	SignFlag = FALSE;

	switch (m_dwStatusSub)
	{
	case  ITEMRECOVER:			 // 救急箱
		m_pSprite->SetSrc(0, 192, 
			ItemConstruct::RECOVER_IMAGE_WIDTH, ItemConstruct::RECOVER_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		m_nAnimNum = 1;

		// 位置のランダム化
		m_vPos.x += Random(-ItemConstruct::RECOVER_DIFF, ItemConstruct::RECOVER_DIFF);
		m_vPos.y += Random(-ItemConstruct::RECOVER_DIFF, ItemConstruct::RECOVER_DIFF);
		break;

	case  ITEM_DOOR:      // ドア
		m_pSprite->SetSrc(0, 0, 
			ItemConstruct::DOOR_IMAGE_WIDTH, ItemConstruct::DOOR_IMAGE_HEIGHT);
		m_nAnimNum = 1;
		m_bIsRotateChengeFlag = 1;
		break;

	case  ITEMKEY:
		m_pSprite->SetSrc(224, 0, 
			ItemConstruct::KEY_IMAGE_WIDTH, ItemConstruct::KEY_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		m_nAnimNum = 1;
		break;

	case  ITEMROTATION90:
		m_pSprite->SetSrc(96, 0, 
			ItemConstruct::ROTATION_IMAGE_WIDTH, ItemConstruct::ROTATION_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		break;

	case  ITEMROTATION180:    
		m_pSprite->SetSrc(96, 64,
			ItemConstruct::ROTATION_IMAGE_WIDTH, ItemConstruct::ROTATION_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		break;

	case  ITEMROTATEDOWN180:
		m_pSprite->SetSrc(96, 64,
			ItemConstruct::ROTATION_IMAGE_WIDTH, ItemConstruct::ROTATION_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		m_vPosUp = VECTOR2(0, 4);
		break;

	case  ITEMROTATION270:
		m_pSprite->SetSrc(96, 128,
			ItemConstruct::ROTATION_IMAGE_WIDTH, ItemConstruct::ROTATION_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		break;

	case  ITEMMOVESIGN:
	case  ITEMSHOTSIGN:
	case  ITEMJUMPSIGN:
		m_pSprite->SetSrc(0, 285,
			ItemConstruct::SIGN_IMAGE_WIDTH, ItemConstruct::SIGN_IMAGE_HEIGHT);
		m_bIsRotateChengeFlag = 0;
		m_nAnimNum = 1;
		break;
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

	// 回転中、ダイアログ表示中は更新処理を行わない
	if (m_pGMain->m_moveFlag) {
		// PCとの接触判定
		if (isCollision(m_pGMain->m_pPcProc->GetPcObjPtr()))
		{
			switch (m_dwStatusSub)
			{
			case  ITEM_DOOR:      // ドア
				m_pGMain->m_DoorTachFlag = TRUE;
				m_pGMain->m_pSePower->Play();
				if (m_pGMain->m_KeyFlag) {
					// 鍵取得
					m_pGMain->m_TextUIFlag = TRUE;
					m_bIsCheckKeyFlag = TRUE;
				}
				else {
					// 鍵未取得
					m_pGMain->m_TextUIFlag = TRUE;
					m_bIsCheckKeyFlag = TRUE;
				}
				break;

			case  ITEMROTATEDOWN180:
			case  ITEMROTATION180:	// 180度回転
				m_pGMain->m_pMapProc->Rotate180();
				break;

			case  ITEMROTATION90:	// 90度回転
				m_pGMain->m_pMapProc->Rotate90();
				break;

			case ITEMROTATION270:	// 270度回転
				m_pGMain->m_pMapProc->Rotate270();
				break;

			case  ITEMKEY:			// カギ
				m_pGMain->m_KeyFlag = TRUE;
				m_bActive = FALSE;
				m_pGMain->m_pKey->Play();
				break;

			case  ITEMRECOVER:		// 回復パーティクル
				m_pGMain->m_pPcProc->GetPcObjPtr()->SetHp(m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() + ItemConstruct::RECOVER_NUM);
				m_pGMain->m_pSePower->Play();
				m_bActive = FALSE;
				break;

			case  ITEMMOVESIGN:
			case  ITEMSHOTSIGN:
			case  ITEMJUMPSIGN:
				if (!SignFlag) {
					m_nAnimNum = ItemConstruct::SIGN_TURN_ANIM_NUM;
				}
				break;
			}
		}

		if (m_dwStatusSub == ITEMROTATEDOWN180) {	//落下する回転アイテムの落下処理
			BOOL bRet;
			CMapLine* pHitmapline = NULL;
			// マップ線との接触判定と適切な位置への移動
			bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
			if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
			{
				m_vPos += m_vPosUp;
			}
		}

		// チュートリアル看板の裏返った後のスプライト位置やアニメーション数
		if (m_nAnimIdx >= 5 && !SignFlag) {
			switch (m_dwStatusSub) {
			case  ITEMMOVESIGN:
				m_pSprite->SetSrc(0, 570,
					ItemConstruct::SIGN_IMAGE_WIDTH, ItemConstruct::SIGN_IMAGE_HEIGHT);
				m_nAnimNum = ItemConstruct::SIGN_MOVE_ANIM_NUM;
				break;

			case  ITEMSHOTSIGN:
				m_pSprite->SetSrc(0, 855,
					ItemConstruct::SIGN_IMAGE_WIDTH, ItemConstruct::SIGN_IMAGE_HEIGHT);
				m_nAnimNum = ItemConstruct::SIGN_SHOT_ANIM_NUM;
				break;

			case  ITEMJUMPSIGN:
				m_pSprite->SetSrc(0, 1140,
					ItemConstruct::SIGN_IMAGE_WIDTH, ItemConstruct::SIGN_IMAGE_HEIGHT);
				m_nAnimNum = ItemConstruct::SIGN_JUMP_ANIM_NUM;
				break;
			}
			m_nAnimIdx = 0;
			SignFlag = TRUE;
		}

		AnimCountup();
	}

	// 回復パーティクルの移動
	if (m_dwStatusSub == ITEMRECOVER)
	{
		m_vTarget = m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos() + VECTOR2(PcConstruct::IMAGE_WIDTH / 2, PcConstruct::IMAGE_HEIGHT / 2);
		if (!TargetMove(ItemConstruct::RECOVER_PARTICLE_SPEED)) {
			m_vPos += m_vPosUp;
		}
	}

	// ダイアログを閉じた後の処理遷移
	if (!m_pGMain->m_TextUIFlag && m_bIsCheckKeyFlag) {
		if (m_pGMain->m_KeyFlag) {
			// 鍵取得
			m_pGMain->m_pMapProc->MoveMap();
			m_bIsCheckKeyFlag = FALSE;
		}
		else {
			// 鍵未取得
			m_pGMain->m_pMapProc->Rotate180();
			m_bIsCheckKeyFlag = FALSE;
		}

	}
	Draw();

}

// ---------------------------------------------------------------------------
// 回転角に応じて、回転後の座標をセットする
// ---------------------------------------------------------------------------
void CItemObj::RotatePos(DWORD rotate) {
	if (m_dwStatusSub == ITEMMOVESIGN || m_dwStatusSub == ITEMJUMPSIGN || m_dwStatusSub == ITEMSHOTSIGN) {
		m_bActive = FALSE;
	}
	DWORD x = m_vPos.x;
	switch (rotate) {
	case 90:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.y - m_pSprite->GetSrcHeight();
		m_vPos.y = x;
		break;

	case 180:
		m_vPos.x = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].x - m_vPos.x - m_pSprite->GetSrcWidth();
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - m_vPos.y - m_pSprite->GetSrcHeight();
		break;

	case 270:
		m_vPos.x = m_vPos.y;
		m_vPos.y = m_pGMain->m_pMapProc->m_nMapSize[m_pGMain->m_pMapProc->m_nMapNo].y - x;
		break;

		if (m_bIsRotateChengeFlag == 1) {
			m_pSprite->SetSrc(0, 96, ItemConstruct::DOOR_IMAGE_HEIGHT, ItemConstruct::DOOR_IMAGE_WIDTH);
			m_bIsRotateChengeFlag = 2;
		}
		else if (m_bIsRotateChengeFlag == 2) {
			m_pSprite->SetSrc(0, 0, ItemConstruct::DOOR_IMAGE_WIDTH, ItemConstruct::DOOR_IMAGE_HEIGHT);
			m_bIsRotateChengeFlag = 1;
		}
		break;
	}
	m_fRotate = 0;
}

