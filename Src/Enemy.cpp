//=============================================================================
//		２Ｄアクションゲームプログラム
//		Ｃ２ＤＡｃｔ１１３　　　　　　           ver 3.0        2021.1.11
//
//		敵の処理
//																Enamy.cpp
//=============================================================================

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//------------------------------------------------------------------------
//
// 敵メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pEnmSlimeProc = new CEnmSlimeProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmSlimeProc);   // プロシージャをプロシージャ配列に登録する

	m_pEnmBatProc = new CEnmBatProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmBatProc);   // プロシージャをプロシージャ配列に登録する

	m_pEnmMoleProc = new CEnmMoleProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmMoleProc);


}

//============================================================================
//   敵プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
//   敵プロシージャの開始フラグをリセットする
//============================================================================
void  CEnmProc::SetNonActive()
{

	// 敵のオブジェクトポインタ配列の探索
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// 下位のプロシージャポインタ配列の探索
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag( 0 );				// 開始フラグのリセット
		((CEnmProc*)m_pProcArray[i])->SetNonActive();   // 下位のプロシージャの処理（再帰処理）
	}
}

void CEnmProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}

void CEnmProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}

// =====================================================================================================
// 
// 各敵プロシージャ毎の処理
// 
// =====================================================================================================

// ===================================================================================================

//------------------------------------------------------------------------
//
//	スライムの敵プロシージャのコンストラクタ	
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
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
//
// スライムの敵プロシージャの自動発生処理
//
//-
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
		while ( nNext != -1 )
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

	//  (注意)m_nWaitTimeのカウントダウンは、CBaseProcで行っている

}

void CEnmSlimeProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CEnmSlimeProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
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

//------------------------------------------------------------------------
//
//	スライムの敵オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CEnmSlimeObj::CEnmSlimeObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 0, ENM_SLIME_WIDTH, ENM_SLIME_HEIGHT);
	m_pHpBarObj = NULL;
	m_vPos = VECTOR2(0,0);
	m_nHp = m_nMaxHp = ENM_SLIME_HP;
	m_nAtc = ENM_SLIME_ATK;
	m_nAnimNum = 7;
}
// ---------------------------------------------------------------------------
//
// スライムの敵オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CEnmSlimeObj::~CEnmSlimeObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// 子オブジェクト：HPバー
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
	CMapLine*  pHitmapline = NULL;
	float fSpeed = 1;
	BOOL bRet;
	
	if(m_pGMain->m_moveFlag){
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
					if ((!bRet && (m_nAnimIdx != 2 && m_nAnimIdx != 3 && m_nAnimIdx != 4)) || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) )
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
					m_nCnt1 = 180;				}
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

// ===================================================================================================

//------------------------------------------------------------------------
//
//	コウモリの敵プロシージャのコンストラクタ	
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
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
//
// コウモリの敵プロシージャの自動発生処理
//
//
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

	//  (注意)m_nWaitTimeのカウントダウンは、CBaseProcで行っている

}

void CEnmBatProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			m_pObjArray[j]->RotatePos(rotate);
			//break;
		}
	}
}

void CEnmBatProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
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

//------------------------------------------------------------------------
//
//	コウモリの敵オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CEnmBatObj::CEnmBatObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageEnemy, 0, 96, ENM_BAT_WIDTH, ENM_BAT_HEIGHT);
	m_pHpBarObj = NULL;
	m_nAnimNum = 2;
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_BAT_HP;
	m_nAtc = ENM_BAT_ATK;

}
// ---------------------------------------------------------------------------
//
// コウモリの敵オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CEnmBatObj::~CEnmBatObj()
{
	SAFE_DELETE(m_pHpBarObj);
	SAFE_DELETE(m_pSprite);
}

// 子オブジェクト
void CEnmBatObj::CreateChildObj()
{
	m_pHpBarObj = new CEnmBatObj(m_pGMain);
}

//-----------------------------------------------------------------------------
// コウモリの敵オブジェクトの開始
//
//   引数　　　なし
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
	m_pHpBarObj->m_vOf = VECTOR2(0, -20);
	m_pHpBarObj->m_pSprite->SetSrc(144, 144, 50, 6);
	m_pHpBarObj->m_nAnimNum = 1;
	m_pHpBarObj->m_nHp = m_nHp;

	return TRUE;

}
//-----------------------------------------------------------------------------
// コウモリの敵オブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmBatObj::Update()
{
	CMapLine*  pHitmapline = NULL;
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
						moveNum += fSpeed;
					}
					else {
						m_vPosUp.x = -fSpeed;
						moveNum += -fSpeed;
					}

					// マップ線との接触判定と適切な位置への移動
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f) || (moveNum > 200 || moveNum < 0))
					{
						// マップ線の端に来たので左右反転
						if (m_nDirIdx == RIGHT)
						{
							m_nDirIdx = LEFT;
							m_vPosUp.x = -fSpeed;
							m_vPosUp.y = 0;
						}
						else {
							m_nDirIdx = RIGHT;
							m_vPosUp.x = fSpeed;
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
			FLOAT h = (float)m_nHp / (float)m_nMaxHp;
			m_pHpBarObj->m_pSprite->SetSrc(144, 144, (DWORD)(50.0 * h), 6);
			m_pHpBarObj->Draw();
		}
	Draw();

	}
}

void CEnmBatObj::RotatePos(DWORD rotate) {
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

//------------------------------------------------------------------------
//
//	モグラの敵プロシージャのコンストラクタ	
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CEnmMoleProc::CEnmMoleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_MOLE_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmMoleObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}

}

// ---------------------------------------------------------------------------
//
// モグラの敵プロシージャの自動発生処理
//
//-
// ---------------------------------------------------------------------------
void CEnmMoleProc::Update()
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
			// イベントマップ　 ( EvtID:3　敵の出現位置,     　EvtNo:0x02　モグラの敵  )
			if (m_pGMain->m_pMapProc->SearchEvt(nNext, 3, 0x08, vPos, nNext))
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

	//  (注意)m_nWaitTimeのカウントダウンは、CBaseProcで行っている

}

//------------------------------------------------------------------------
//
//	モグラの敵オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CEnmMoleObj::CEnmMoleObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageBoss, 0, 0, ENM_MOLE_WIDTH, ENM_MOLE_HEIGHT);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = ENM_MOLE_HP;
	m_nAtc = ENM_MOLE_ATK;
	m_nAnimNum = 3;
}
// ---------------------------------------------------------------------------
//
// モグラの敵オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CEnmMoleObj::~CEnmMoleObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// モグラの敵オブジェクトの開始
//
//   引数　　　なし
//-----------------------------------------------------------------------------
BOOL	CEnmMoleObj::Start(VECTOR2 vPos)
{

	m_bActive = TRUE;
	ResetStatus();
	ResetAnim();
	m_nDirIdx = LEFT;
	m_vPos = vPos;	// 発生位置
	m_dwStatus = SMOKE;

	m_vPosUp = VECTOR2(0, 0);
	m_AnimCount = 120;
	m_waitTime = 0;

	m_nHp = m_nMaxHp;
	m_pGMain->m_pDig->Play();
	speed = 8;

	m_pGMain->m_BossSpownFlag = TRUE;

	return TRUE;

}
//-----------------------------------------------------------------------------
// モグラの敵オブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmMoleObj::Update()
{
	CMapLine*  pHitmapline = NULL;
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
					m_dwStatus = NORMAL;
					m_nFlashIdx = 0;		// フラッシュアニメーションインデックス数
					m_nFlashFrm = 0;		// フラッシュアニメーションフレームカウント
				}
				else {
					FlashCountup();
				}		// break;を入れずそのまま　NORMAL　の処理を行う
			case  NORMAL:
				switch (m_dwStatusSub)
				{
				case  APPEAR:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						m_pSprite->SetSrc(0, 336, 318, 168);
						m_dwStatusSub = IDLE;
						m_nAnimNum = 2;
						m_nAnimIdx = 0;
					}

					// マップ線との接触判定と適切な位置への移動
					bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
					if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
					{
						m_fJumpTime++;
						m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
					}
					else {
						m_fJumpTime = 0;
					}

					break;

				case  IDLE:
					if (Random(0, 100) == 0) {
						
						m_dwStatusSub = DIVE;
						m_pSprite->SetSrc(0, 1008, 318, 168);
						m_nAnimNum = 7;
						m_nAnimIdx = 0;
						
					}
					break;

				

				case  DIVE:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						if (Random(0, 1) == 0) {
							m_pSprite->SetSrc(0, 168, 318, 168);
							m_dwStatus = SHOT;
							m_nAnimNum = 3;
							m_nAnimIdx = 0;
							m_nDirIdx = LEFT;
							m_ShotNum = Random(10, 15);
							for (int i = 0; i < m_ShotNum; i++) {
								m_SHotPosX[i] = Random(128, 1237 - m_pSprite->GetSrcWidth());
							}
							m_RotateItemPos = Random(0, m_ShotNum - 1);
							m_vPos = VECTOR2(1237 - m_pSprite->GetSrcWidth(), 96);
						}
						else {
							m_pSprite->SetSrc(0, 1344, 318, 168);
							m_dwStatusSub = JUMP;
							m_nAnimNum = 10;
							m_nAnimIdx = 0;
							if (m_nDirIdx == LEFT) {
								m_vPos.x = 1237 - m_pSprite->GetSrcWidth();
								
							}
							else {
								m_vPos.x = 128;
								
							}
						}
					}

					break;

				case JUMP:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						m_pSprite->SetSrc(0, 0, 0, 0);
						m_vPos.y = 0;
						m_AnimCount = 45;
					}
					if (m_AnimCount <= 0 && m_pSprite->GetSrcWidth() == 0) {
						m_pSprite->SetSrc(0, 1344, 318, 168);
						m_vPos.y = 503;
						m_nAnimIdx = 0;
					}
					else {
						if (m_AnimCount > 0) {
							m_AnimCount--;
						}
					}
					if (m_nDirIdx == LEFT) {
						m_vPosUp = VECTOR2(-4, 0);
						if (m_vPos.x <= 128) {
							m_dwStatus = SMOKE;
							m_pSprite->SetSrc(0, 0, 318, 168);
							m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
							m_vPos.y = 572;
							m_vPosUp.x = 0;
							m_nAnimNum = 3;
							m_AnimCount = 60;
							m_pGMain->m_pDig->Play();
						}
					}
					else {
						m_vPosUp = VECTOR2(4, 0);
						if (m_vPos.x >= 1237 - m_pSprite->GetSrcWidth()) {
							m_dwStatus = SMOKE;
							m_nDirIdx = LEFT;
							m_pSprite->SetSrc(0, 0, 318, 168);
							m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
							m_vPos.y = 572;
							m_vPosUp.x = 0;
							m_nAnimNum = 3;
							m_AnimCount = 60;
							m_pGMain->m_pDig->Play();
						}
					}
				}
				
				// ＰＣとのあたり判定
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case SMOKE:
				m_nFlashIdx = 0;		// フラッシュアニメーションインデックス数
				m_nFlashFrm = 0;		// フラッシュアニメーションフレームカウント
				m_nCnt1 = 0;
				// マップ線との接触判定と適切な位置への移動
				bRet = m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline);
				if (!bRet || (pHitmapline && pHitmapline->m_vNormal.y > -0.99f))
				{
					m_fJumpTime++;
					m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
				}
				else {
					m_fJumpTime = 0;
				}

				if (m_AnimCount <= 0) {
					m_pSprite->SetSrc(0, 672, 318, 168);
					m_nAnimNum = 7;
					m_nAnimIdx = 0;
					m_dwStatus = NORMAL;
					m_dwStatusSub = APPEAR;
					if (m_pGMain->m_pPcProc->GetPcObjPtr()->GetPos().x > m_vPos.x) {
						m_nDirIdx = RIGHT;
					}
					else {
						m_nDirIdx = LEFT;
					}
				}
				else {
					m_AnimCount--; 
				}
				break;

			case  SHOT:
				m_nFlashIdx = 0;		// フラッシュアニメーションインデックス数
				m_nFlashFrm = 0;		// フラッシュアニメーションフレームカウント
				m_nCnt1 = 0;
				m_vPosUp.x = -3;
				for (int i = 0; i < m_ShotNum; i++) {
					if (m_SHotPosX[i] == m_vPos.x || m_SHotPosX[i] == m_vPos.x - 1 || m_SHotPosX[i] == m_vPos.x + 1) {
						if (m_RotateItemPos == i) {
							m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos + VECTOR2(0, 100), ITEMROTATEDOWN180);
						}
						else {
							m_pGMain->m_pWeaponProc->m_pWeaponRockProc->Start(m_vPos + VECTOR2(0, 100), this, ENM);
						}
					}
				}

				if (m_vPos.x <= 128) {
					m_dwStatus = SMOKE;
					m_pSprite->SetSrc(0, 0, 318, 168);
					m_vPos.x = Random(128, 1237 - m_pSprite->GetSrcWidth());
					m_vPos.y = 604;
					m_vPosUp.x = 0;
					m_AnimCount = 60;
					m_pGMain->m_pDig->Play();
				}
				break;


			case  DAMAGE:
				if (m_pOtherObj->GetEnmAtc() == 1000) {
					m_nHp -= m_pOtherObj->GetEnmAtc();
				}
				else {
					m_nHp -= m_pOtherObj->GetAtc();
				}
				if (m_nHp <= 0) {
					m_dwStatus = DEAD;
					m_nCnt1 = 180;
				}
				else {
					m_dwStatus = FLASH;
					m_nCnt1 = 60;
				}
				m_pGMain->m_pSeHit->Play();
				break;

			case  DEAD:
				m_nCnt1--;
				if (m_nCnt1 <= 0)
				{
					m_bActive = FALSE;
					m_pGMain->m_BossSpownFlag = FALSE;
					m_pGMain->m_dwGameStatus = GAMECLEAR;
				}
				else {
					FlashCountup();
				}
				break;
			}
			if (m_waitTime > 0) {
				m_waitTime--;
			}
			// 増分
			m_vPos += m_vPosUp;
			AnimCountup();

		}
	}
	Draw();
}

CBaseObj* CEnmMoleProc::GetMoleObj() {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			return m_pObjArray[j];
		}
	}
}