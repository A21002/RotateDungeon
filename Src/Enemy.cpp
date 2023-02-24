//=============================================================================
//		敵の処理
//																Enamy.cpp
//=============================================================================

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//============================================================================
// 敵キャラクター全体のプロシージャ
//============================================================================
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
	m_pProcArray.push_back(m_pEnmBatProc);   

	m_pEnmMoleProc = new CEnmMoleProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmMoleProc);
}

//------------------------------------------------------------------------
//   敵プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
//   敵プロシージャの開始フラグをリセットする
//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
// 敵プロシージャとオブジェクトを探索し
// 全てのオブジェクトを回転後の座標にセットする
//------------------------------------------------------------------------
void CEnmProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}

//------------------------------------------------------------------------
// 敵プロシージャとオブジェクトを探索し
// 全てのオブジェクトを回転させる
// 引数
// DWORD	flag
// UP	: 右回転
// DOWN	: 左回転
//------------------------------------------------------------------------
void CEnmProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}
