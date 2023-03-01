//=============================================================================
//      ����̏���
//																Weapon.cpp
//=============================================================================

#include "Weapon.h"
#include "Playchar.h"
#include "Enemy.h"
#include "Map.h"

//=============================================================================================================================
//
// ���탁�C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// ----------------------------------------------------------------------------------------------------------------------------
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pWeaponShotProc = new CWeaponShotProc(m_pGMain);	// ����E�V���b�g�v���V�[�W��
	m_pWeaponRockProc = new CWeaponRockProc(m_pGMain);	// ���e�v���V�[�W��
	m_pProcArray.push_back(m_pWeaponShotProc);     // �v���V�[�W�����v���V�[�W���z��ɓo�^����
	m_pProcArray.push_back(m_pWeaponRockProc);
} 

void CWeaponProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}

void CWeaponProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}
