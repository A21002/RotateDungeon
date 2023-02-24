//=============================================================================
//		�G�̏���
//																Enamy.cpp
//=============================================================================

#include "Enemy.h"
#include "Weapon.h"
#include "Effect.h"
#include "Playchar.h"
#include "Map.h"

//============================================================================
// �G�L�����N�^�[�S�̂̃v���V�[�W��
//============================================================================
//------------------------------------------------------------------------
//
// �G���C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
//------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pEnmSlimeProc = new CEnmSlimeProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmSlimeProc);   // �v���V�[�W�����v���V�[�W���z��ɓo�^����

	m_pEnmBatProc = new CEnmBatProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmBatProc);   

	m_pEnmMoleProc = new CEnmMoleProc(m_pGMain);
	m_pProcArray.push_back(m_pEnmMoleProc);
}

//------------------------------------------------------------------------
//   �G�v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
//   �G�v���V�[�W���̊J�n�t���O�����Z�b�g����
//------------------------------------------------------------------------
void  CEnmProc::SetNonActive()
{

	// �G�̃I�u�W�F�N�g�|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive( FALSE);
	}

	// ���ʂ̃v���V�[�W���|�C���^�z��̒T��
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetStartFlag( 0 );				// �J�n�t���O�̃��Z�b�g
		((CEnmProc*)m_pProcArray[i])->SetNonActive();   // ���ʂ̃v���V�[�W���̏����i�ċA�����j
	}
}

//------------------------------------------------------------------------
// �G�v���V�[�W���ƃI�u�W�F�N�g��T����
// �S�ẴI�u�W�F�N�g����]��̍��W�ɃZ�b�g����
//------------------------------------------------------------------------
void CEnmProc::Rotate(DWORD rotate) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->Rotate(rotate);
	}
}

//------------------------------------------------------------------------
// �G�v���V�[�W���ƃI�u�W�F�N�g��T����
// �S�ẴI�u�W�F�N�g����]������
// ����
// DWORD	flag
// UP	: �E��]
// DOWN	: ����]
//------------------------------------------------------------------------
void CEnmProc::RotateCenter(DWORD flag) {
	for (DWORD j = 0; j < m_pProcArray.size(); j++)
	{
		m_pProcArray[j]->RotateCenter(flag);
	}
}
