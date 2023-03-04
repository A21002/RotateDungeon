//=============================================================================
//		スライムの処理
//																EnamySlime.cpp
//=============================================================================
#include "EnemySlime.h"
#include "Enemy.h"
#include "Map.h"
#include "Effect.h"
#include "Playchar.h"

//=============================================================================
// スライムプロシージャ
//=============================================================================
//------------------------------------------------------------------------
//	スライムの敵プロシージャのコンストラクタ	
//------------------------------------------------------------------------
CEnmSlimeProc::CEnmSlimeProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_SLIME_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmSlimeObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
		((CEnmSlimeObj*)(m_pObjArray[i]))->CreateChildObj();
	}
}

// ---------------------------------------------------------------------------
// スライムの敵プロシージャの自動発生処理
// ---------------------------------------------------------------------------
void CEnmSlimeProc::Update()
{
	VECTOR2 vPos;
	int nNext;

	// まだ発生できる場合のみ発生させる
	if (m_nStartFlag == 0)
	{
		// イベントマップを探索し、敵の出現位置を設定する
		nNext = 0;
		while (nNext != -1)
		{
			// イベントマップ　 ( EvtID:3　敵の出現位置,     　EvtNo:0x02　スライムの敵  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x02, vPos, nNext))
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
void CEnmSlimeProc::Rotate(DWORD rotate) {
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
void CEnmSlimeProc::RotateCenter(DWORD flag) {
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
// スライムオブジェクト
//=============================================================================
//------------------------------------------------------------------------
//	スライムの敵オブジェクトのコンストラクタ	
//------------------------------------------------------------------------
CEnmSlimeObj::CEnmSlimeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 0, 
		SlimeConstruct::IMAGE_WIDTH, SlimeConstruct::IMAGE_HEIGHT);
	m_pHpBarObj = NULL;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = SlimeConstruct::HP;
	m_nAtc = SlimeConstruct::ATK;
	m_nAnimNum = SlimeConstruct::ANIM_NUM;
}

// ---------------------------------------------------------------------------
// スライムの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmSlimeObj::~CEnmSlimeObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// ---------------------------------------------------------------------------
// 子オブジェクト：HPバーの生成
// ---------------------------------------------------------------------------
void CEnmSlimeObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmSlimeObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// スライムの敵オブジェクトの開始
//-----------------------------------------------------------------------------
BOOL	CEnmSlimeObj::Start(VECTOR2 vPos)
{
	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// 発生位置

	m_vPosUp = VECTOR2(0, 0);

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
// スライムの敵オブジェクトの更新
//-----------------------------------------------------------------------------
void	CEnmSlimeObj::Update()
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
				// ダメージを受けた際の処理
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
						m_vPosUp.x = SlimeConstruct::SPEED;
					}
					else {
						m_vPosUp.x = -SlimeConstruct::SPEED;
					}

					// ジャンプ処理
					if (m_nAnimIdx == SlimeConstruct::JUMP_FLAME)
					{
						m_vPosUp.y += -SlimeConstruct::JUMP_POWER;
						m_vJumpSpeed.y = m_vPosUp.y;
					}

					// マップ線との接触判定と適切な位置への移動
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (!bRet || (pHitmapline->m_vNormal.y != -1))
					{
						m_fJumpTime++;
						m_vPosUp.y += round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
						//m_vJumpSpeed.y = m_vPosUp.y;
					}
					else {
						m_fJumpTime = 0;
						m_vJumpSpeed.y = 0;
					}

					if (pHitmapline && (pHitmapline->m_vNormal.x == -1 || pHitmapline->m_vNormal.x == 1))
					{
						// マップ線の端に来たので左右反転
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							//m_vPosUp.x = -SlimeConstruct::SPEED;
						}
						else {
							m_nDirIdx = RIGHT;
							//m_vPosUp.x = SlimeConstruct::SPEED;
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
					m_nCnt1 = SlimeConstruct::FLASHTIME_DEAD * 60;
				}
				else {
					m_dwStatus = FLASH;
					m_pHpBarObj->m_dwStatus = FLASH;
					m_nCnt1 = SlimeConstruct::FLASHTIME_DAMAGE * 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				// 倒れた際の最後の処理
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
			m_pHpBarObj->AnimCountup();

		}

		// HPバーの描画
		FLOAT h = (float)m_nHp / (float)m_nMaxHp;
		m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(50.0 * h), 6);
		m_pHpBarObj->Draw();
	}
	Draw();
}

// ---------------------------------------------------------------------------
// 回転角に応じて、回転後の座標をセットする
// ---------------------------------------------------------------------------
void CEnmSlimeObj::RotatePos(DWORD rotate) {
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
