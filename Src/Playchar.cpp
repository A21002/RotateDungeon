//=============================================================================
//		プレイキャラクターの処理
//																Playchar.cpp
//=============================================================================

#include "Playchar.h"
#include "Weapon.h"
#include "Effect.h"
#include "Map.h"

// ===========================================================================
// PC定数定義
// ===========================================================================
float const PcConstruct::SHOT_WAIT_SECOND = 0.2f;

// ===========================================================================
// PCプロシージャ
// ===========================================================================
//------------------------------------------------------------------------
//
//	ＰＣプロシージャのコンストラクタ	
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	
	m_pObjArray.push_back(new CPcObj(m_pGMain));  // m_pObjArrayにオブジェクトを登録する

	m_nPcNum = 1;
}

// ===========================================================================
// PCオブジェクト
// ===========================================================================
//------------------------------------------------------------------------
//
//	ＰＣオブジェクトのコンストラクタ	
//
//  引数　CGameMain* pGMain
//
//------------------------------------------------------------------------
CPcObj::CPcObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageChar, 0, 0, PcConstruct::IMAGE_WIDTH, PcConstruct::IMAGE_HEIGHT);
	m_vPos = VECTOR2(0.0f, 0.0f);
	ResetStatus();
	ResetAnim();
	m_bActive = TRUE;
	m_nDirIdx = RIGHT;
	m_nHp = m_nMaxHp = PcConstruct::MAX_HP;
	m_nAtc = PcConstruct::ATC;

}
// ---------------------------------------------------------------------------
//
// ＰＣオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CPcObj::~CPcObj()
{
	SAFE_DELETE(m_pSprite);
}
//-----------------------------------------------------------------------------
// ＰＣオブジェクトの更新
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;
	CMapLine*  pHitmapline = NULL;
	float fSpeed     = PcConstruct::SPEED_MOVE;
	float fJumpPlus  = PcConstruct::SPEED_JUMP;
	BOOL fMoveFlag = FALSE;

	if (!m_bActive) return;

	// 移動増分の初期化
	m_vPosUp = VECTOR2(0, 0);

	// 回転処理中、メッセージダイアログでなければ移動処理を行う
	if (m_pGMain->m_moveFlag) {
		switch (m_dwStatus)
		{
		case  FLASH:			// 無敵時間
			m_nCnt1--;
			if (m_nCnt1 <= 0) {
				ResetStatus();
				ResetAnim();
			}
			else {
				FlashCountup();
			}		// break;を入れずそのまま　NORMAL　の処理を行う
		case  NORMAL:

			break;

		case DAMAGE:			// ダメージ処理
			if (m_pOtherObj->GetAtc() > 0)
			{
				m_nHp -= m_pOtherObj->GetAtc();	// 攻撃を受けたダメージ
				if (m_nHp <= 0)
				{
					m_nHp = 0;
					m_dwStatus = DEAD;		// HPが０なので死亡へ
					m_nCnt1 = PcConstruct::DEAD_SECOND * 60;			// 死亡フラッシュ中の時間設定
				}
				else {
					m_dwStatus = FLASH;
					m_nCnt1 = PcConstruct::FLASH_SECOND * 60;
				}
			}
			else {
				m_dwStatus = NORMAL;  // 攻撃力なしのときはすぐにNORMALに戻る
			}
			break;

		case DEAD:
			m_pGMain->m_dwGameStatus = GAMEOVER;
			
			break;

		}

		switch (m_dwStatusSub)
		{
		case  WALK:		// 歩行中の処理
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//→キー
			{
				fMoveFlag = TRUE;
				m_vPosUp.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//←キー
			{
				fMoveFlag = TRUE;
				m_vPosUp.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_SPACE) || pDI->CheckJoy(KD_DAT, DIJ_UP)) //	SPACEキー
			{	// ジャンプ開始
				fMoveFlag = TRUE;
				m_dwStatusSub = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = -(fSpeed*2.5 + fJumpPlus);
				m_fJumpTime = 0;
				m_vPosUp.y = m_vJumpSpeed.y;
			}
			else {// 自然落下
				m_dwStatusSub = JUMP;
				m_vJumpSpeed.x = m_vPosUp.x;
				m_vJumpSpeed.y = fSpeed / 2;
				m_fJumpTime = 0;
				m_vPosUp.y = m_vJumpSpeed.y;
			}
			break;

		case  JUMP:		// ジャンプ中の処理
			if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//→キー
			{
				m_vJumpSpeed.x = fSpeed;
				m_nDirIdx = RIGHT;
			}
			if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//←キー
			{
				m_vJumpSpeed.x = -fSpeed;
				m_nDirIdx = LEFT;
			}
			fMoveFlag = TRUE;
			m_fJumpTime++;
			m_vPosUp.x = m_vJumpSpeed.x;
			m_vPosUp.y = round(m_vJumpSpeed.y + GRAVITY * m_fJumpTime);
			break;
		}

		if (pDI->CheckKey(KD_DAT, DIK_X) || pDI->CheckJoy(KD_DAT, DIJ_A))			// 攻撃
		{
			if (m_nShotWait <= 0)
			{
				// ショットの発射
				m_pGMain->m_pWeaponProc->m_pWeaponShotProc->Start(m_vPos + VECTOR2(16, 16), this);
				m_nShotWait = PcConstruct::SHOT_WAIT_SECOND * 60;    // 武器発射のウェイトを掛ける
			}
		}

		if (m_nShotWait > 0) m_nShotWait--;    // 武器発射のウェイトをカウントダウン

		// マップ線との接触判定と適切な位置への移動
		if (m_pGMain->m_pMapProc->isCollisionMoveMap(this, pHitmapline))
		{
			if (pHitmapline && pHitmapline->m_vNormal.y <= 0)	// マップ線が垂直まではＯＫ
			{
				m_dwStatusSub = WALK;	// マップ線に接触しているときは、ジャンプを歩行に戻す
				m_fJumpTime = 0;
				m_vJumpSpeed.x = m_vJumpSpeed.y = 0;
			}
		}
		// 増分計算
		m_vPos += m_vPosUp;

		// 移動していればアニメーション再生
		if (fMoveFlag) {
			AnimCountup();
		}
		
	}
	Draw();
	

}
