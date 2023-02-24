//=============================================================================
//		スライムの処理
//																EnamySlime.cpp
//=============================================================================
#include "EnemySlime.h"
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
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 0, ENM_SLIME_WIDTH, ENM_SLIME_HEIGHT);
	m_pHpBarObj = NULL;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_SLIME_HP;
	m_nAtc = ENM_SLIME_ATK;
	m_nAnimNum = 7;
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
//
//   引数　　　なし
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
	m_pHpBarObj->m_vOf = VECTOR2(0, -20);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 50, 6);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;
}

//-----------------------------------------------------------------------------
// スライムの敵オブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmSlimeObj::Update()
{
	CMapLine* pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;

	if (m_pGMain->m_moveFlag) {
		if (m_bActive)
		{
			m_vPosUp = VECTOR2(0, 0);

			switch (m_dwStatus)
			{
			case  FLASH:
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
						m_vPosUp.x = fSpeed;
					}
					else {
						m_vPosUp.x = -fSpeed;
					}

					// マップ線との接触判定と適切な位置への移動
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if ((!bRet && (m_nAnimIdx != 2 && m_nAnimIdx != 3 && m_nAnimIdx != 4)) || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
					{
						m_fJumpTime++;
						// マップ線の端に来たので左右反転
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -fSpeed;
							m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = fSpeed;
							m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
						}
					}
					else {
						m_fJumpTime = 0;
					}

					// ジャンプ処理
					if (m_nAnimIdx == ENM_SLIME_JUMP_ANIM_START) {
						m_vPosUp.y = -1;
					}
					else if (m_nAnimIdx == ENM_SLIME_JUMP_ANIM_FIN) {
						m_vPosUp.y = 1;
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
					m_dwStatus = DEAD;
					m_pHpBarObj->m_bActive = FALSE;
					m_nCnt1 = 180;
				}
				else {
					m_dwStatus = FLASH;
					m_pHpBarObj->m_dwStatus = FLASH;
					m_nCnt1 = 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pHpBarObj->m_bActive = FALSE;
					if (0 == Random(0, 2)) {
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
	}
	m_fRotate = 0;
}
