//=============================================================================
//		コウモリの処理
//																EnamySlime.cpp
//=============================================================================
#include "EnemyBat.h"
#include "Enemy.h"
#include "Map.h"
#include "Effect.h"
#include "Playchar.h"

//=============================================================================
// コウモリプロシージャ
//=============================================================================
//------------------------------------------------------------------------
//	コウモリの敵プロシージャのコンストラクタ	
//------------------------------------------------------------------------
CEnmBatProc::CEnmBatProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_BAT_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmBatObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
		((CEnmBatObj*)(m_pObjArray[i]))->CreateChildObj();
	}
}

// ---------------------------------------------------------------------------
// コウモリの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmBatProc::Update()
{
	VECTOR2 vPos;
	int  nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップの探索　 ( EvtID:3　敵の出現位置,  EvtNo:0x04	コウモリの敵  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x04, vPos, nNext))
			{
				for (DWORD j = 0; j < m_pObjArray.size(); j++)
				{
					if (!m_pObjArray[j]->GetActive())
					{
						m_pObjArray[j]->Start(vPos);
						break;
					}
				}
			}
		}

		m_nStartFlag = 1;  // 一回発させたら、もう発生しない
	}
}

//------------------------------------------------------------------------
// 全てのオブジェクトを回転後の座標にセットする
//------------------------------------------------------------------------
void CEnmBatProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
		}
	}
}

//------------------------------------------------------------------------
// 全てのオブジェクトを回転させる
// 
// 引数
// DWORD	flag
// UP	: 右回転
// DOWN	: 左回転
//------------------------------------------------------------------------
void CEnmBatProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			if (flag == UP) {
				m_pObjArray[j]->RotateDrawUp();
			}
			else if (flag == DOWN) {
				m_pObjArray[j]->RotateDrawDown();
			}
		}
	}
}

//=============================================================================
// コウモリオブジェクト
//=============================================================================
//------------------------------------------------------------------------
//	コウモリの敵オブジェクトのコンストラクタ	
//------------------------------------------------------------------------
CEnmBatObj::CEnmBatObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 96, 
		BatConstruct::IMAGE_WIDTH, BatConstruct::IMAGE_HEIGHT);
	m_pHpBarObj = NULL;
	m_nAnimNum = BatConstruct::ANIM_NUM;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = BatConstruct::HP;
	m_nAtc = BatConstruct::ATK;
}

// ---------------------------------------------------------------------------
// コウモリの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmBatObj::~CEnmBatObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// ---------------------------------------------------------------------------
// 子オブジェクト：HPバーの生成
// ---------------------------------------------------------------------------
void CEnmBatObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmBatObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// コウモリの敵オブジェクトの開始
//-----------------------------------------------------------------------------
BOOL	CEnmBatObj::Start(VECTOR2 vPos)
{

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);
	moveNum = 0;

	m_nHp = m_nMaxHp;

	// Hpバー
	m_pHpBarObj->m_bActive = TRUE;
	m_pHpBarObj->ResetStatus();
	m_pHpBarObj->ResetAnim();
	m_pHpBarObj->m_vPos = m_vPos;
	m_pHpBarObj->m_vPosUp = VECTOR2(0, 0);
	m_pHpBarObj->m_vOf = VECTOR2(0, HpBarConstruct::DIFF);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 
		HpBarConstruct::IMAGE_WIDTH, HpBarConstruct::IMAGE_HEIGHT);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;
}

//-----------------------------------------------------------------------------
// コウモリの敵オブジェクトの更新
//-----------------------------------------------------------------------------
void	CEnmBatObj::Update()
{
	CMapLine* pHitmapline = NULL;
	BOOL bRet;

	if (m_pGMain->m_moveFlag) {
		if (m_bActive)
		{
			m_vPosUp = VECTOR2(0, 0);

			switch (m_dwStatus)
			{
			case  FLASH:
				// ダメージを受けた際の無敵時間
				m_nCnt1--;
				if (m_nCnt1 <= 0) {
					ResetStatus();
					ResetAnim();
					m_pHpBarObj->ResetStatus();
					m_pHpBarObj->ResetAnim();
				}
				else {
					FlashCountup();
					m_pHpBarObj->FlashCountup();
				}		// break;を入れずそのまま　NORMAL　の処理を行う
			case  NORMAL:
				switch (m_dwStatusSub)
				{
				case  WALK:
					// 自由移動の処理
					if (m_nDirIdx == RIGHT)
					{
						m_vPosUp.x = BatConstruct::SPEED;
						moveNum += BatConstruct::SPEED;
					}
					else {
						m_vPosUp.x = -BatConstruct::SPEED;
						moveNum += -BatConstruct::SPEED;
					}

					// マップ線との接触判定と適切な位置への移動
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) || (moveNum > 200 || moveNum < 0))
					{
						// マップ線の端に来たので左右反転
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -BatConstruct::SPEED;
							m_vPosUp.y = 0;
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = BatConstruct::SPEED;
							m_vPosUp.y = 0;
						}
					}
					break;
				}
				// 増分
				m_vPos += m_vPosUp;
				m_pHpBarObj->m_vPos = m_vPos;
				// ＰＣとのあたり判定
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case  DAMAGE:
				m_nHp -= m_pOtherObj->GetAtc();
				if (m_nHp <= 0) {
					// HPが0になった時
					m_dwStatus = DEAD;
					m_pHpBarObj->m_bActive = FALSE;
					m_nCnt1 = BatConstruct::FLASHTIME_DEAD * 60;
				}
				else {
					m_dwStatus = FLASH;
					m_pHpBarObj->m_dwStatus = FLASH;
					m_nCnt1 = BatConstruct::FLASHTIME_DAMAGE * 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				// 倒れた際の処理
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pHpBarObj->m_bActive = FALSE;

					// 回復パーティクルの出現
					for (int i = 0; i < 10; i++) {
						m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos, ITEMRECOVER);
					}
				}
				else {
					FlashCountup();
				}
				break;
			}

			AnimCountup();

			// HPバーの表示
			FLOAT h = (float)m_nHp / (float)m_nMaxHp;
			m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(HpBarConstruct::IMAGE_WIDTH * h), HpBarConstruct::IMAGE_HEIGHT);
			m_pHpBarObj->Draw();
		}
		Draw();

	}
}

// ---------------------------------------------------------------------------
// 回転角に応じて、回転後の座標をセットする
// ---------------------------------------------------------------------------
void CEnmBatObj::RotatePos(DWORD rotate) {
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
	}
	m_fRotate = 0;
}
