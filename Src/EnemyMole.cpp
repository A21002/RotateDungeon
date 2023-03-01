//=============================================================================
//		モグラの処理
//																EnamyMole.cpp
//=============================================================================
#include "EnemyMole.h"
#include "Map.h"
#include "Effect.h"
#include "Weapon.h"
#include "Playchar.h"

//=============================================================================
// モグラプロシージャ
//=============================================================================
//------------------------------------------------------------------------
//	モグラの敵プロシージャのコンストラクタ	
//------------------------------------------------------------------------
CEnmMoleProc::CEnmMoleProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (DWORD i = 0; i < ENM_MOLE_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmMoleObj(m_pGMain));  // m_pObjArrayにオブジェクトを生成する
	}
}

// ---------------------------------------------------------------------------
// モグラの敵プロシージャの自動発生処理
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
			// イベントマップ　 ( EvtID:3　敵の出現位置,     　EvtNo:0x08　モグラの敵  )
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
}

//=============================================================================
// モグラオブジェクト
//=============================================================================
//------------------------------------------------------------------------
//	モグラの敵オブジェクトのコンストラクタ	
//------------------------------------------------------------------------
CEnmMoleObj::CEnmMoleObj(CGameMain* pGMain) : CBaseObj(pGMain)
{
	m_pSprite = new CSprite(m_pGMain->m_pImageBoss, 0, 0, 
		MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
	m_vPos = VECTOR2(0, 0);
	m_nHp = m_nMaxHp = MoleConstruct::HP;
	m_nAtc = MoleConstruct::ATK;
	m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
}

// ---------------------------------------------------------------------------
// モグラの敵オブジェクトのデストラクタ
// ---------------------------------------------------------------------------
CEnmMoleObj::~CEnmMoleObj()
{
	SAFE_DELETE(m_pSprite);
}

//-----------------------------------------------------------------------------
// モグラの敵オブジェクトの開始
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
	m_AnimCount = MoleConstruct::SMOKE_COUNT;

	m_nHp = m_nMaxHp;
	m_pGMain->m_pDig->Play();
	m_pGMain->m_BossSpownFlag = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// モグラの敵オブジェクトの更新
//-----------------------------------------------------------------------------
void	CEnmMoleObj::Update()
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
				// ダメージを受けたときの無敵時間の点滅演出
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
					// 地面から出現アニメーション
				case  APPEAR:
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// アニメーション終了後、IDLEへ遷移
						m_pSprite->SetSrc(0, 336, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_dwStatusSub = IDLE;
						m_nAnimNum = MoleConstruct::ANIM_NUM_IDLE;
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
					// 次の攻撃へ遷移までの待機アニメーション
					if (Random(0, 100) == 0) {
						// 1%の確率で次の攻撃を開始
						// DIVEへ遷移
						m_dwStatusSub = DIVE;
						m_pSprite->SetSrc(0, 1008, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_nAnimNum = MoleConstruct::ANIM_NUM_DIVE;
						m_nAnimIdx = 0;
					}
					break;



				case  DIVE:
					// 穴掘りアニメーション
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// アニメーション終了後、行う攻撃をランダムで決定する
						if (Random(0, 1) == 0) {
							// 天井から岩を落とす攻撃
							m_pSprite->SetSrc(0, 168, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_dwStatus = SHOT;
							m_nAnimNum = MoleConstruct::ANIM_NUM_SHOT;
							m_nAnimIdx = 0;
							m_nDirIdx = LEFT;

							// 落とす岩の数を決める
							m_ShotNum = Random(MoleConstruct::SHOT_NUM_MIN, MoleConstruct::SHOT_NUM_MAX);
							// 落とす位置を決める
							for (int i = 0; i < m_ShotNum; i++) {
								m_SHotPosX[i] = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							}
							// 落とす岩の1つをアイテムに置き換える
							m_RotateItemPos = Random(0, m_ShotNum - 1);
							// 右上に移動
							m_vPos = VECTOR2(MoleConstruct::AREA_X_MAX, MoleConstruct::AREA_Y_MIN);
						}
						else {
							// 波上に突進して攻撃
							m_pSprite->SetSrc(0, 1344, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_dwStatusSub = JUMP;
							m_nAnimNum = MoleConstruct::ANIM_NUM_JUMP;
							m_nAnimIdx = 0;

							if (m_nDirIdx == LEFT) {
								// IDLE時、左を向いていれば右からスタート
								m_vPos.x = MoleConstruct::AREA_X_MAX;
							}
							else {
								// 右を向いていれば、左からスタート
								m_vPos.x = MoleConstruct::AREA_X_MIN;
							}
						}
					}
					break;

				case JUMP:
					// 波上に突進して攻撃
					if (m_nAnimIdx >= m_nAnimNum - 1) {
						// 1度ジャンプするとしばらく地中にいる
						m_pSprite->SetSrc(0, 0, 0, 0);		// 当たり判定をなくす
						m_vPos.y = 0;
						m_AnimCount = MoleConstruct::JUMP_COUNT;	// 潜っている間のフレーム数
					}
					if (m_AnimCount <= 0 && m_pSprite->GetSrcWidth() == 0) {
						// 潜っている間に待機フレーム数が0になった
						m_pSprite->SetSrc(0, 1344, 
							MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
						m_vPos.y = MoleConstruct::AREA_Y_MAX;
						m_nAnimIdx = 0;
					}
					else {
						// 待機フレームを減らす
						if (m_AnimCount > 0) {
							m_AnimCount--;
						}
					}
					// エリアの端についているかチェック
					if (m_nDirIdx == LEFT) {
						m_vPosUp = VECTOR2(-MoleConstruct::JUMP_SPEED, 0);
						if (m_vPos.x <= MoleConstruct::AREA_X_MIN) {
							// 左端についた場合、煙状態へ遷移
							m_dwStatus = SMOKE;
							m_pSprite->SetSrc(0, 0, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
							m_AnimCount = MoleConstruct::SMOKE_COUNT;

							// 位置をランダムにする
							m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							m_vPos.y = MoleConstruct::AREA_Y_MAX;
							m_vPosUp.x = 0;
							m_pGMain->m_pDig->Play();
						}
					}
					else {
						m_vPosUp = VECTOR2(MoleConstruct::JUMP_SPEED, 0);
						if (m_vPos.x >= MoleConstruct::AREA_X_MAX) {
							// 右端についた場合、煙状態へ遷移
							m_dwStatus = SMOKE;
							m_nDirIdx = LEFT;
							m_pSprite->SetSrc(0, 0, 
								MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
							m_nAnimNum = MoleConstruct::ANIM_NUM_SMOKE;
							m_AnimCount = MoleConstruct::SMOKE_COUNT;

							// 位置をランダムにする
							m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
							m_vPos.y = MoleConstruct::AREA_Y_MAX;
							m_vPosUp.x = 0;
							m_pGMain->m_pDig->Play();
						}
					}
				}
				// ＰＣとのあたり判定
				m_pGMain->m_pPcProc->Hitcheck((CBaseObj*)this);

				break;

			case SMOKE:
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
					// 出現状態へ遷移
					m_pSprite->SetSrc(0, 672, 
						MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
					m_nAnimNum = MoleConstruct::ANIM_NUM_APPEAR;
					m_nAnimIdx = 0;
					m_dwStatus = NORMAL;
					m_dwStatusSub = APPEAR;

					// 現在の位置からプレイヤーがいる方を向く
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
				// 上から岩を落とす攻撃
				m_vPosUp.x = -MoleConstruct::SHOT_SPEED;

				for (int i = 0; i < m_ShotNum; i++) {
					// 現在の座標の前後に攻撃を行う座標が設定されているか
					if (m_SHotPosX[i] == m_vPos.x || m_SHotPosX[i] == m_vPos.x - 1 || m_SHotPosX[i] == m_vPos.x + 1) {
						// 落とすのは岩か回転アイテムか
						if (m_RotateItemPos == i) {
							m_pGMain->m_pEffectProc->m_pItemProc->Start(m_vPos + VECTOR2(0, 100), ITEMROTATEDOWN180);
						}
						else {
							m_pGMain->m_pWeaponProc->m_pWeaponRockProc->Start(m_vPos + VECTOR2(0, 100), this, ENM);
						}
					}
				}

				if (m_vPos.x <= MoleConstruct::AREA_X_MIN) {
					// 左上に到達したら煙状態へ遷移
					m_dwStatus = SMOKE;
					m_pSprite->SetSrc(0, 0, 
						MoleConstruct::IMAGE_WIDTH, MoleConstruct::IMAGE_HEIGHT);
					m_vPos.x = Random(MoleConstruct::AREA_X_MIN, MoleConstruct::AREA_X_MAX);
					m_vPos.y = MoleConstruct::AREA_Y_MAX;
					m_vPosUp.x = 0;
					m_AnimCount = MoleConstruct::SMOKE_COUNT;
					m_pGMain->m_pDig->Play();
				}
				break;


			case  DAMAGE:
				if (m_pOtherObj->GetEnmAtc() == 1000) {
					// 岩が当たれば、専用の値を参照する
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
			// 増分
			m_vPos += m_vPosUp;
			AnimCountup();

		}
	}
	Draw();
}

//-----------------------------------------------------------------------------
// モグラオブジェクトのゲッター
//-----------------------------------------------------------------------------
CBaseObj* CEnmMoleProc::GetMoleObj() {
	for (DWORD j = 0; j < m_pObjArray.size(); j++)
	{
		if (m_pObjArray[j]->GetActive())
		{
			return m_pObjArray[j];
		}
	}
}