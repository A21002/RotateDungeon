//-----------------------------------------------------------------------------
// DInput�֐� : DirectInput���ȒP�ɗ��p���邽�߂̃��C�u����(�{��)
// 
//												       ver 2.8        2020.2.15
//
// Copyright (c) 2019 �É��Y�ƋZ�p���w�Z �Q�[���N���G�C�g�� All rights reserved.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "DInput.h"


// �O���[�o���ϐ�(�R�[���o�b�N�֐����g�p���邽�ߕK�v)
static CDirectInput*	g_pDI = NULL;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CDirectInput::CDirectInput(void)
{
	ZeroMemory(this, sizeof(CDirectInput));
	m_bInputActive = TRUE;

	g_pDI = this;
}
//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CDirectInput::~CDirectInput()
{
	EndDirectInput();
}
//-----------------------------------------------------------------------------
// DirectInput�̊J�n
// 
// ����		HINSTANCE	hInst	�C���X�^���X�n���h��
//			HWND		hWnd	�E�B���h�E�n���h��
//			int			flag	����������f�B�o�C�X�̑I���iINIT_KEYBOARD|INIT_MOUSE|INIT_JOYSTICK�j
//			DWORD		dwWidth	�E�B���h�̕�
//			DWORD		dwHeight�E�B���h�̍���
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::StartDirectInput(HINSTANCE hInst, HWND hWnd, int flag, DWORD dwWidth, DWORD dwHeight)
{
	// DirectInput8�I�u�W�F�N�g�̍쐬
	m_hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI8, NULL);
	if (DI_OK != m_hr) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : DirectInput�����������邱�Ƃ��ł��܂���B"), NULL, MB_OK);
		return FALSE;
	}
	m_hWnd = hWnd;

	if (flag & INIT_KEYBOARD) InitKey(hWnd);
	if (flag & INIT_MOUSE)    InitMouse(hWnd);
	if (flag & INIT_JOYSTICK) InitJoy(hWnd);

	m_ViewWidth  = (float)dwWidth;
	m_ViewHeight = (float)dwHeight;

	return TRUE;
}
//-----------------------------------------------------------------------------
// DirectInput�̏I��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::EndDirectInput(void)
{
	// DirectInputDevice(keyboard)�����
	if( m_pBufferKey ){
		delete []m_pBufferKey;	// �o�b�t�@�����
		m_pBufferKey = NULL;
	}
	if (m_pKey) {
		m_pKey->Unacquire();
		m_pKey->Release();
		m_pKey = NULL;
	}

	// DirectInputDevice(mouse)�����
	if( m_pBufferMouse ){
		delete []m_pBufferMouse;	// �o�b�t�@�����
		m_pBufferMouse = NULL;
	}
	if (m_pMouse) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}

	int i, j;
	for (i=0; i<m_nJoySum; i++) {
		if( m_bJoyFF[i] ){
			// DirectInputDevice(ForceFeedback Joystick)�����
			for (j=0; j<m_nJoyEFSum; j++) {
				if( m_pJoyEffect[i][j] ){
					m_pJoyEffect[i][j]->Unload();
					m_pJoyEffect[i][j]->Release();
					m_pJoyEffect[i][j] = NULL;
				}
			}

		}

		// DirectInputDevice(joystick)�����
		if( m_pBufferJoy[i] ){
			delete []m_pBufferJoy[i];	// �o�b�t�@�����
			m_pBufferJoy[i] = NULL;
		}

		if (m_pJoy[i]) {
			m_pJoy[i]->Unacquire();
			m_pJoy[i]->Release();
			m_pJoy[i] = NULL;
		}
	}

	// DirectInput�����
	if (m_pDI8) {
		m_pDI8->Release();
		m_pDI8 = NULL;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// ���̓f�o�C�X�̃A�N�Z�X���̐���
//-----------------------------------------------------------------------------
void CDirectInput::SetAcquire(void)
{
	int i;

	if (m_bInputActive) {
		if (m_pKey) m_pKey->Acquire();
		if (m_pMouse) m_pMouse->Acquire();
		for (i=0; i<m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Acquire();
	} else {
		if (m_pKey) m_pKey->Unacquire();
		if (m_pMouse) m_pMouse->Unacquire();
		for (i=0; i<m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Unacquire();
	}
}
//-----------------------------------------------------------------------------
// �f�[�^�ǂݍ���
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::GetInput(void)
{
	if (m_pKey) GetKey();
	if (m_pMouse) GetMouse();
	if (m_pJoy[0]) GetJoy();
	return TRUE;
}
//*****************************************************************************
//*** Keyboard                                                              ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// Keyboard�f�o�C�X�̏�����
// 
// ����		HWND hWnd	�E�B���h�E�n���h��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::InitKey(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// keyboard�f�o�C�X�̃C���X�^���X�쐬�E������
	m_hr = m_pDI8->CreateDevice(GUID_SysKeyboard, &m_pKey, NULL);
	if (DI_OK != m_hr) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(keyboard)�����������邱�Ƃ��ł��܂���B"),NULL,MB_OK);
		return FALSE;
	}
	// DirectInput�f�o�C�X�̃f�[�^�`���̐ݒ�
	m_pKey->SetDataFormat(&c_dfDIKeyboard);
	if (DI_OK != m_hr) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : DirectInput�f�o�C�X�̃f�[�^�`���̐ݒ�Ɏ��s���܂����B"),NULL,MB_OK);
		return FALSE;
	}
	// keyboard�f�o�C�X�̃C���X�^���X�ɑ΂��鋦�����x���̊m��
	m_pKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (DI_OK != m_hr) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : keyboard�f�o�C�X�̃C���X�^���X�ɑ΂��鋦�����x���̊m���Ɏ��s���܂����B"),NULL,MB_OK);
		return FALSE;
	}
	return SetPropertyKey();
}
//-----------------------------------------------------------------------------
// �o�b�t�@�T�C�Y�̐ݒ�(keyboard)
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::SetPropertyKey(void)
{
	m_BufferRestKey = 0;
	m_pBufferPositionKey = NULL;
	m_pBufferKey = new DIDEVICEOBJECTDATA[BufferSize];	// �o�b�t�@���m��
	DIPROPDWORD dipdw;
	ZeroMemory(&dipdw, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = BufferSize;
	m_pKey->Unacquire();				// ��U�A�N�Z�X�����������
	m_hr = m_pKey->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	m_pKey->Acquire();					// �A�N�Z�X���𓾂�
	return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// �L�[�{�[�h�̏�Ԃ𓾂�
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::GetKey(void)
{
	if (!m_pKey) return FALSE;
	if (m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState) != DI_OK) {
		m_pKey->Acquire();
		if (FAILED(m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState))) {
			//MessageBox(NULL,_T("DInput.cpp : GetKey() : GetDeviceState���ُ�I�����܂����B") ,NULL,MB_OK );
			return FALSE;
		}
	}

	m_BufferRestKey = BufferSize;
	if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
		// ���s���������1�x�������݂�
		m_BufferRestKey = BufferSize;
		m_pKey->Acquire();
		if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
			//MessageBox(NULL,_T("DInput.cpp : GetKey() : GetDeviceData���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
	}
	m_BufferRestBackupKey = m_BufferRestKey;	// �ǂݍ��񂾃o�b�t�@����ޔ�

	return TRUE;
}
//-----------------------------------------------------------------------------
// �L�[�����`�F�b�N(keyboard)
// 
// ����		const int&		kmode	�`�F�b�N���郂�[�h
//			const DWORD&	kcode	�`�F�b�N����L�[�R�[�h�@
// 
// �߂�l	TRUE:�`�F�b�N�n�j�@FALSE:�`�F�b�N�m�f
//-----------------------------------------------------------------------------
BOOL CDirectInput::CheckKey(const int& kmode, const DWORD& kcode)
{
	if (!m_bInputActive || !m_pKey) return FALSE;
	switch (kmode) {
	case KD_DAT :						// ���ڃf�[�^(���݂̃L�[�������)
		return m_diKeyState[kcode] & 0x80;
	case KD_TRG :						// �L�[���������u��
	case KD_UTRG :						// �L�[�𗣂����u��
		m_BufferRestKey = m_BufferRestBackupKey;	// �ǂݍ��񂾃o�b�t�@���̕���
		m_pBufferPositionKey = m_pBufferKey;
		while (m_BufferRestKey > 0) {
			m_BufferRestKey--;
			m_didodKey = m_pBufferPositionKey;
			m_pBufferPositionKey++;
			if (m_didodKey->dwOfs == kcode) {	//�L�[�̎��
				if (kmode == KD_TRG) {
					if (m_didodKey->dwData) return TRUE;
				} else {
					if (!(m_didodKey->dwData)) return TRUE;
				}
			}
		}
		return FALSE;
	default :
		MessageBox(NULL, _T("DInput.cpp : CheckKey() : �w�肵���L�[���[�h���Ԉ���Ă��܂��B"), NULL, MB_OK);
		return FALSE;
	}
}
//*****************************************************************************
//*** Mouse                                                                 ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// Mouse�f�o�C�X�̏�����
// 
// ����		HWND hWnd	�E�B���h�E�n���h��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::InitMouse(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// DirectInputDevice������(mouse)
	m_hr = m_pDI8->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (DI_OK != m_hr) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(mouse)�����������邱�Ƃ��ł��܂���B"),NULL,MB_OK);
		return FALSE;
	}
	if (m_pMouse->SetDataFormat(&c_dfDIMouse) != DI_OK) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetDataFormat���\�b�h(mouse)���ُ�I�����܂����B"), NULL, MB_OK);
		return FALSE;
	}
	if (m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK) {
		MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevel���\�b�h(mouse)���ُ�I�����܂����B"), NULL, MB_OK);
		return FALSE;
	}

	return SetPropertyMouse();
}
//-----------------------------------------------------------------------------
// �o�b�t�@�T�C�Y�̐ݒ�(mouse)
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::SetPropertyMouse(void)
{
	m_BufferRestMouse = 0;
	m_pBufferPositionMouse = NULL;
	m_pBufferMouse = new DIDEVICEOBJECTDATA[BufferSize];	// �o�b�t�@���m��
	DIPROPDWORD dipdw;
	ZeroMemory(&dipdw, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = BufferSize;
	m_pMouse->Unacquire();				// ��U�A�N�Z�X�����������
	m_hr = m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	m_pMouse->Acquire();					// �A�N�Z�X���𓾂�
	return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// �}�E�X�̏�Ԃ𓾂�
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::GetMouse(void)
{
	if (!m_pMouse) return FALSE;
	if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims) != DI_OK) {
		m_pMouse->Acquire();
		if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims))) {
			//MessageBox(NULL,_T("DInput.cpp : GetMouse() : GetDeviceState���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
	}

	m_BufferRestMouse = BufferSize;
	if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
		// ���s���������1�x�������݂�
		m_BufferRestMouse = BufferSize;
		m_pMouse->Acquire();
		if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
			//MessageBox(NULL,_T("DInput.cpp : GetMouse() : GetDeviceData���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
	}
	m_BufferRestBackupMouse = m_BufferRestMouse;	// �ǂݍ��񂾃o�b�t�@����ޔ�

	return TRUE;
}
//-----------------------------------------------------------------------------
// �L�[�����`�F�b�N(mouse)
// 
// ����		const int&		kmode	�`�F�b�N���郂�[�h
//			const DWORD&	kcode	�`�F�b�N����L�[�R�[�h�@
// 
// �߂�l	TRUE:�`�F�b�N�n�j�@FALSE:�`�F�b�N�m�f
//-----------------------------------------------------------------------------
BOOL CDirectInput::CheckMouse(const int& kmode, const DWORD& kcode)
{
	if (!m_bInputActive || !m_pMouse) return FALSE;

	// �ړ������̎擾�̂Ƃ�
	if (kcode == DIM_LEFT || kcode == DIM_RIGHT || kcode == DIM_UP || kcode == DIM_DOWN) {

		if (m_dims.lX > 0 && kcode == DIM_RIGHT) {
			return TRUE;
		}
		else if (m_dims.lX < 0 && kcode == DIM_LEFT) {
			return TRUE;
		}
		else if (m_dims.lY < 0 && kcode == DIM_UP) {
			return TRUE;
		}
		else if (m_dims.lY > 0 && kcode == DIM_DOWN) {
			return TRUE;
		}
		return FALSE;

	}else{ // �e�L�[��Ԃ̎擾�̂Ƃ�

		switch (kmode) {
			case KD_DAT:						// ���ڃf�[�^(���݂̃L�[�������)
				return m_dims.rgbButtons[kcode] & 0x80;
			case KD_TRG:						// �L�[���������u��
			case KD_UTRG:						// �L�[�𗣂����u��
				m_BufferRestMouse = m_BufferRestBackupMouse;	// �ǂݍ��񂾃o�b�t�@���̕���
				m_pBufferPositionMouse = m_pBufferMouse;
				while (m_BufferRestMouse > 0) {
					m_BufferRestMouse--;
					m_didodMouse = m_pBufferPositionMouse;
					m_pBufferPositionMouse++;
					if (m_didodMouse->dwOfs == DIMOFS_BUTTON(kcode)) {	//�L�[�̎��
						if (kmode == KD_TRG) {
							if (m_didodMouse->dwData) return TRUE;
						}
						else {
							if (!(m_didodMouse->dwData)) return TRUE;
						}
					}
				}
				return FALSE;
			default:
				MessageBox(NULL, _T("DInput.cpp : CheckMouse() : �w�肵���L�[���[�h���Ԉ���Ă��܂��B"), NULL, MB_OK);
				return FALSE;
		}
	}

}
//-----------------------------------------------------------------------------
// �}�E�X�X�e�[�g�𓾂�(mouse)
//
// �߂�l	DIMOUSESTATE�\���̂̒l�B�}�E�X�f�o�C�X�̏�Ԃ�\���B
//-----------------------------------------------------------------------------
DIMOUSESTATE CDirectInput::GetMouseState( void )
{
	return  m_dims;
}

//-----------------------------------------------------------------------------
// �}�E�X���W�𓾂�(mouse)
//
// ���̊֐���DirectInput�ł͂Ȃ�Windows�̊֐����g�p���Ă���
//
// �߂�l	POINT�\���̂̒l�B�}�E�X�̂w�x���W��\���B
//-----------------------------------------------------------------------------
POINT CDirectInput::GetMousePos(void)
{
	POINT pt;
	RECT  rc;
	float scaleX, scaleY;
	GetCursorPos(&pt); //�}�E�X�̌��݂̃X�N���[�����W���擾����
	ScreenToClient(m_hWnd, &pt); // �N���C�A���g���W�ɕϊ�����

	// ��ʂ̊g�k�ɑΉ����ă}�E�X���W���v�Z����
	GetClientRect(m_hWnd, &rc); // �N���C�A���g�̈�̃T�C�Y
	scaleX = m_ViewWidth  / (rc.right - rc.left);	// ���@�䗦
	scaleY = m_ViewHeight / (rc.bottom - rc.top);	// �����䗦
	
	pt.x = (long)(pt.x * scaleX);
	pt.y = (long)(pt.y * scaleY);

	return pt;
}
//-----------------------------------------------------------------------------
// �}�E�X�J�[�\���̕\���E��\����؂�ւ���
//
// ���̊֐���DirectInput�ł͂Ȃ�Windows�̊֐����g�p���Ă���
//
// ����  BOOL bFlag  TRUE:�J�[�\���\��  FLASE:�J�[�\����\��
//
// �߂�l	�Ȃ�
//-----------------------------------------------------------------------------
void CDirectInput::ShowMouseCursor(BOOL bFlag)
{
	ShowCursor(bFlag);
}

//*****************************************************************************
//*** Joystick�̏���                                                        ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// �񋓃R�[���o�b�N�֐� (Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
	return g_pDI->EnumJoysticksCb( pInst, lpvContext );
}

//-----------------------------------------------------------------------------
// �񋓃R�[���o�b�N (Joystick)�Ή����\�b�h
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
	LPDIRECTINPUTDEVICE8 pDevice = NULL;
	DIDEVCAPS            diDevCaps = {NULL};

	m_hr = m_pDI8->CreateDevice(pInst->guidInstance, &pDevice, NULL);
	if (DI_OK != m_hr) return DIENUM_CONTINUE;	// ���̃f�o�C�X���

	diDevCaps.dwSize = sizeof( DIDEVCAPS );
	m_hr = pDevice->GetCapabilities( &diDevCaps );
	if( FAILED(m_hr) ){
		pDevice->Release();
		pDevice = NULL;
		return DIENUM_CONTINUE;	// ���̃f�o�C�X���
	}

	m_pJoy[m_nJoySum] = pDevice;

	//if( diDevCaps.dwFlags == DIDC_FORCEFEEDBACK ){
	if( pInst->guidFFDriver != GUID_NULL ){
		m_bJoyFF[m_nJoySum] = TRUE;		// �t�H�[�X�t�B�[�h�o�b�N�W���C�X�X�e�B�b�N
		m_nJoyFFNum++;
	}else{
		m_bJoyFF[m_nJoySum] = FALSE;	// �ʏ�̃W���C�X�e�B�b�N
	}

	if (++m_nJoySum >= JOYSTICK_COUNT) return DIENUM_STOP;
	return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Joystick�f�o�C�X�̏�����
// 
// ����		HWND hWnd	�E�B���h�E�n���h��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::InitJoy(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// �A�^�b�`�\�ȃf�o�C�X��񋓂���(joystick)
	m_pDI8->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	if(!m_pJoy[0]) {
		//MessageBox(NULL,_T("DInput.cpp : StartDirectInput() : joystick���ڑ�����Ă��܂���B"), NULL, MB_OK);
		return FALSE;
	}
	int i;
	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;

		m_hr = m_pJoy[i]->SetDataFormat(&c_dfDIJoystick2);
		if (DI_OK != m_hr) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetDataFormat���\�b�h(joystick)���ُ�I�����܂����B"),NULL,MB_OK);
			return FALSE;
		}

		if( m_nJoyFFNum > 0 ){	// �e�e�W���C�X�e�B�b�N���P�ł�����Ƃ��́A�S����r�����[�h�ɂ���
			m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		}else{
			m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		}
		if (DI_OK != m_hr) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevel���\�b�h(joystick)���ُ�I�����܂����B"),NULL,MB_OK);
			return FALSE;
		}
	}

	InitJoyNormal(hWnd);	// �ʏ�W���C�X�e�B�b�N�̏�����
	InitJoyFF(hWnd);		// �e�e�W���C�X�e�B�b�N�̏�����

	return TRUE;

}
//-----------------------------------------------------------------------------
// �ʏ��Joystick�f�o�C�X�̏�����
// 
// ����		HWND hWnd	�E�B���h�E�n���h��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::InitJoyNormal(HWND hWnd)
{
	int  i;

	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( m_bJoyFF[i] ) continue;	// �e�e�W���C�X�e�B�b�N������ 

		diprg.diph.dwObj = DIJOFS_X;
		m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (DI_OK != m_hr) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty���\�b�h(joystick:DIJOFS_X)���ُ�I�����܂����B"),NULL,MB_OK);
			return FALSE;
		}

		diprg.diph.dwObj = DIJOFS_Y;
		m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (DI_OK != m_hr) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty���\�b�h(joystick:DIJOFS_Y)���ُ�I�����܂����B"),NULL,MB_OK);
			return FALSE;
		}
	}

	return SetPropertyJoy();
}
//-----------------------------------------------------------------------------
// �ʏ��joystick�̃o�b�t�@�T�C�Y�̐ݒ�(joystick)
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::SetPropertyJoy(void)
{
	m_BufferRestJoy = 0;
	m_pBufferPositionJoy = NULL;
	for (int i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( m_bJoyFF[i] ) continue;	// �e�e�W���C�X�e�B�b�N������

		m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];	// �o�b�t�@���m��
		DIPROPDWORD dipdw;
		ZeroMemory(&dipdw, sizeof(dipdw));
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = BufferSize;
		m_pJoy[i]->Unacquire();				// ��U�A�N�Z�X�����������
		m_hr = m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		m_pJoy[i]->Acquire();					// �A�N�Z�X���𓾂�
		if (FAILED(m_hr)) return !FAILED(m_hr);
	}
	return TRUE;
}
//-----------------------------------------------------------------------------
// ForceFeedback Joystick�f�o�C�X�̏�����
// 
// ����		HWND hWnd	�E�B���h�E�n���h��
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::InitJoyFF(HWND hWnd)
{
	int  i;

	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize = sizeof(dipr.diph);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.lMin = RANGE_MIN;				// negative to the left/top
	dipr.lMax = RANGE_MAX;				// positive to the right/bottom

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// �ʏ�W���C�X�e�B�b�N������

		m_pJoy[i]->Unacquire();

		// X���͈͂̐ݒ�
		dipr.diph.dwObj = DIJOFS_X;
		if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty(X���͈�(RANGE):FFJoystick)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// Y���͈͂̐ݒ�
		dipr.diph.dwObj = DIJOFS_Y;
		if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty(Y���͈�(RANGE):FFJoystick)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
	}

	// deadzone�̐ݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// �ʏ�W���C�X�e�B�b�N������

		// X���͈͂̐ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty(X���͈�(DEADZONE):FFJoystick)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// Y���͈͂̐ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty(Y���͈�(RANGE):FFJoystick)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
	}

	// �����Z���^�����O
	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// �ʏ�W���C�X�e�B�b�N������

		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		//dipdw.dwData = DIPROPAUTOCENTER_ON;
		dipdw.dwData = DIPROPAUTOCENTER_OFF;	// �����Z���^�����O����
		m_hr = m_pJoy[i]->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
		if (FAILED(m_hr)) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : SetProperty(�����Z���^�����O):FFJoystick)���ُ�I�����܂����B"),NULL,MB_OK);
			return FALSE;
		}
	}

	m_BufferRestJoy = 0;
	m_pBufferPositionJoy = NULL;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// �ʏ�W���C�X�e�B�b�N������

		m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];	// �o�b�t�@���m��
		ZeroMemory(&dipdw, sizeof(dipdw));
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = BufferSize;
		// ���A�l�̓`�F�b�N���Ȃ�(DI_POLLEDDEVICE���Ԃ�)
		m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		if (m_pJoy[i]->Acquire() != DI_OK) {
			//CreateJoyEffectStandard(); // �Ȃ������Ɠ�d�ɂȂ��Ă����̂ō폜����
			;
		}

		// get the device capabilities
		DIDEVCAPS didc;
		didc.dwSize = sizeof(DIDEVCAPS);
		if (m_pJoy[i]->GetCapabilities(&didc) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : StartDirectInput() : GetCapabilities(ForceFeedback Joystick)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// �W���̂e�e���ʂ�ݒ肷��
		if (didc.dwFlags & DIDC_FORCEFEEDBACK) {
			//MessageBox(NULL,_T("DInput.cpp : StartDirectInput() : ForceFeedback device found.\n"), NULL, MB_OK); // -- 2018.8.27
			if (!CreateJoyEffectStandard()) {
				MessageBox(NULL,_T("DInput.cpp : StartDirectInput() : CreateEffect(ForceFeedback Joystick)���ُ�I�����܂����B"), NULL, MB_OK);
				return FALSE;
			}
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �W���C�X�e�B�b�N�̌��݂̃L�[�����`�F�b�N(���ڃf�[�^)
// 
// ����		DWORD	kcode	�`�F�b�N����L�[�R�[�h
//			int		nSum	�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
// 
// �߂�l	TRUE:�`�F�b�N�n�j�@FALSE:�`�F�b�N�m�f
//-----------------------------------------------------------------------------
BOOL CDirectInput::CheckJoyImm(DWORD kcode, int nSum)
{
	if (DIJ_LEFT > kcode) {
		return m_js[nSum].rgbButtons[kcode] & 0x80;
	} else {
		switch (kcode) {	// �㉺���E�́A�f�W�^���iDIJ_VOLUME���E�j�Ŕ��f
		case DIJ_LEFT :		// DIF_LEFT������R�[�h
			return m_js[nSum].lX < (-1 * DIJ_VOLUME);
		case DIJ_RIGHT :	// DIF_RIGHT������R�[�h
			return m_js[nSum].lX > DIJ_VOLUME;
		case DIJ_UP :		// DIF_UP������R�[�h
			return m_js[nSum].lY < (-1 * DIJ_VOLUME);
		case DIJ_DOWN :		// DIF_DOWN������R�[�h
			return m_js[nSum].lY > DIJ_VOLUME;
		default :
			MessageBox(NULL, _T("DInput.cpp : CheckJoystickImm() : kcode�̎w�肪�Ԉ���Ă��܂��B"), NULL, MB_OK);
			return FALSE;
		}
	}
}
//-----------------------------------------------------------------------------
// �W���C�X�e�B�b�N�̏�Ԃ𓾂�
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::GetJoy(void)
{
	for (int i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) return FALSE;
		m_pJoy[i]->Poll();
		m_pJoy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_js[i]);

		m_BufferRestJoy = BufferSize;
		m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
		if (FAILED(m_hr)) {				// ���s������1�x�������g���C
			m_BufferRestJoy = BufferSize;
			m_pJoy[i]->Acquire();
			m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
			if (FAILED(m_hr)) {
				//MessageBox(NULL,_T("DInput.cpp : GetJoy() : GetDeviceData���ُ�I�����܂����B"),NULL,MB_OK);
				return FALSE;
			}
		}
		m_BufferRestBackupJoy[i] = m_BufferRestJoy;	// �ǂݍ��񂾃o�b�t�@����ޔ�
	}
	return TRUE;
}
//-----------------------------------------------------------------------------
// Joystick �㉺���E�̐���`�F�b�N(joystick)
// 
// ����		int		kmode	�`�F�b�N���郂�[�h
//			DWORD	kcode	�`�F�b�N����L�[�R�[�h�@
//			int		nSum	�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
// 
// �߂�l	TRUE:�`�F�b�N�n�j�@FALSE:�`�F�b�N�m�f
//-----------------------------------------------------------------------------
BOOL CDirectInput::CheckUpDownLeftRight(int kmode, DWORD kcode, int nSum)
{
	if (m_didodJoy[nSum]->dwOfs == 4) {	// Y��(�㉺)?
		switch (kcode) {
		case DIJ_UP :
			if (kmode == KD_TRG) {
				if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return TRUE;
			} else {
				if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return TRUE;
			}
			break;
		case DIJ_DOWN :
			if (kmode == KD_TRG) {
				if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return TRUE;
			} else {
				if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return TRUE;
			}
		}
	} else {
		if (m_didodJoy[nSum]->dwOfs == 0) {	// X��(���E)?
			switch (kcode) {
			case DIJ_LEFT :
				if (kmode == KD_TRG) {
					if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return TRUE;
				} else {
					if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return TRUE;
				}
				break;
			case DIJ_RIGHT :
				if (kmode == KD_TRG) {
					if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return TRUE;
				} else {
					if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// �L�[�����`�F�b�N(joystick) 
// 
// ����		const int&		kmode	�`�F�b�N���郂�[�h
//			const DWORD&	kcode	�`�F�b�N����L�[�R�[�h�@
//			int		nSum	�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
// 
// �߂�l	TRUE:�`�F�b�N�n�j�@FALSE:�`�F�b�N�m�f
//-----------------------------------------------------------------------------
BOOL CDirectInput::CheckJoy(const int& kmode, const DWORD& kcode, int nSum)
{
	if (!m_bInputActive || !m_pJoy[nSum]) return FALSE;


	switch (kmode) {
	case KD_DAT :						// ���ڃf�[�^(���݂̃L�[�������)
		return CheckJoyImm(kcode, nSum);
	case KD_TRG :						// �L�[���������u��
	case KD_UTRG :						// �L�[�𗣂����u��
		m_BufferRestJoy = m_BufferRestBackupJoy[nSum];	// �ǂݍ��񂾃o�b�t�@���̕���
		m_pBufferPositionJoy = m_pBufferJoy[nSum];
		while (m_BufferRestJoy > 0) {
			m_BufferRestJoy--;
			m_didodJoy[nSum] = m_pBufferPositionJoy;
			m_pBufferPositionJoy++;
#if 0
				TCHAR chBuffer[128];
				_stprintf(chBuffer, _T("DInput.cpp : CheckNJoy() : cdwOfs=%ld, dwData=%ld\n"), m_didodJoy[nSum]->dwOfs, m_didodJoy[nSum]->dwData);
				ErrorMessage(chBuffer);
#endif
			if (kcode >= DIJ_LEFT && kcode <= DIJ_DOWN) {
				return CheckUpDownLeftRight(kmode, kcode, nSum);
			} else {
				if (m_didodJoy[nSum]->dwOfs == (DIJOFS_BUTTON(kcode))) {	//�L�[�̎��
					if (kmode == KD_TRG) {
						if (m_didodJoy[nSum]->dwData) return TRUE;
					} else {
						if (!(m_didodJoy[nSum]->dwData)) return TRUE;
					}
				}
			}
		}
		return FALSE;
	default :
		MessageBox(NULL, _T("DInput.cpp : CheckJoy() : �w�肵���L�[���[�h���Ԉ���Ă��܂��B"), NULL, MB_OK);
		return FALSE;
	}
}
//-----------------------------------------------------------------------------
// �t�H�[�X�t�B�[�h�o�b�N�f�B�o�C�X���ǂ������`�F�b�N����(joystick) 
//
// ����		int nSum		�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
//
// �߂�l	TRUE:�t�H�[�X�t�B�[�h�o�b�N�f�B�o�C�X�@FALSE:��ʃf�B�o�C�X
//-----------------------------------------------------------------------------
BOOL CDirectInput::IfJoyFF(int nSum)
{
	if(!m_pJoy[nSum]) return FALSE;
	if( m_bJoyFF[nSum] ){	// ForceFeedback Joystick���ǂ���
		return TRUE;
	}else{
		return FALSE;
	}
}
//-----------------------------------------------------------------------------
// �W���C�X�e�B�b�N�̃X�e�[�g�𓾂�
//
// ����		int nSum		�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
//
// �߂�l	DIJOYSTATE2�\���̂̒l�B�g���@�\�����W���C�X�e�B�b�N �f�o�C�X�̏�Ԃ�\���B
//			���ɁALONG lX;�@LONG lY;�@�͕����L�[�̃A�i���O�l��\���B
//-----------------------------------------------------------------------------
DIJOYSTATE2 CDirectInput::GetJoyState(int nSum)
{
	return m_js[nSum];
}
//-----------------------------------------------------------------------------
// �ڑ�����Ă���W���C�X�e�B�b�N�̐��𓾂�              // -- 2018.8.27
//
// ����		�Ȃ�
//
// �߂�l	int �ڑ�����Ă���W���C�X�e�B�b�N�̐�
//-----------------------------------------------------------------------------
int CDirectInput::GetJoyNum(void)
{
	return m_nJoySum;
}

/*
//-----------------------------------------------------------------------------
// �W�����ʂ̐��� (for ForceFeedback Joystick)
//                                                             !! OLD  !!
// �iEF_BOUNCE�EF_FIRE�EF_EXPLODE�̂R�̌��ʂ𐶐�����j
//-----------------------------------------------------------------------------
BOOL CDirectInput::CreateJoyEffectStandard(void)
{
	DIEFFECT diEffect;
	DIENVELOPE diEnvelope;
	DWORD rgdwAxes[2];
	LONG rglDirections[2];
	DICONSTANTFORCE dicf;
	DIPERIODIC dipf;
	int i;

	ZeroMemory(&diEffect, sizeof(DIEFFECT));
	ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

	// these fields are the same for all effects we will be creating
	diEffect.dwSize = sizeof(DIEFFECT);
	diEffect.dwSamplePeriod = 0;		// use default sample period
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;
	diEffect.dwTriggerRepeatInterval = 0;
	diEffect.rgdwAxes = rgdwAxes;
	diEffect.rglDirection = rglDirections;
	diEffect.dwGain = FF_BODYBUILDER;

	for (i = 0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if (!m_bJoyFF[i]) continue;	// �ʏ�W���C�X�e�B�b�N������


		// BOUNCE�G�t�F�N�g�̐���
		if (m_pJoyEffect[i][EF_BOUNCE]) {
			m_pJoyEffect[i][EF_BOUNCE]->Release();
			m_pJoyEffect[i][EF_BOUNCE] = NULL;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = DIJOFS_Y;
		rglDirections[0] = 0;
		rglDirections[1] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
		diEffect.dwDuration = 200000;
		diEffect.cAxes = 2;
		diEffect.lpEnvelope = NULL;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_BOUNCE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// FIRE�G�t�F�N�g�̐���
		if (m_pJoyEffect[i][EF_FIRE]) {
			m_pJoyEffect[i][EF_FIRE]->Release();
			m_pJoyEffect[i][EF_FIRE] = NULL;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_Y;
		rglDirections[0] = 1;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 20000;
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = NULL;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_FIRE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// EXPLODE�G�t�F�N�g�̐���
		if (m_pJoyEffect[i][EF_EXPLODE]) {
			m_pJoyEffect[i][EF_EXPLODE]->Release();
			m_pJoyEffect[i][EF_EXPLODE] = NULL;
		}

		diEnvelope.dwSize = sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel = 0;
		diEnvelope.dwAttackTime = 0;
		diEnvelope.dwFadeLevel = 0;
		diEnvelope.dwFadeTime = 1000000;

		dipf.dwMagnitude = 10000;
		dipf.lOffset = 0;
		dipf.dwPhase = 0;
		dipf.dwPeriod = 100000;

		rgdwAxes[0] = DIJOFS_X;
		rglDirections[0] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 1000000;
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = &diEnvelope;
		diEffect.cbTypeSpecificParams = sizeof(DIPERIODIC);
		diEffect.lpvTypeSpecificParams = &dipf;

		if (m_pJoy[i]->CreateEffect(GUID_Square, &diEffect, &m_pJoyEffect[i][EF_EXPLODE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		m_nJoyEFSum = 3;	// �W�����ʂ��R�o�^���ꂽ

	}

	return TRUE;
}
*/

//-----------------------------------------------------------------------------
// �W�����ʂ̐��� (for ForceFeedback Joystick)                  // -- 2018.8.27
//
// �iEF_BOUNCE�EF_FIRE�EF_EXPLODE�̂R�̌��ʂ𐶐�����j
//-----------------------------------------------------------------------------
BOOL CDirectInput::CreateJoyEffectStandard(void)
{
	DIEFFECT diEffect;
	DIENVELOPE diEnvelope;
	DWORD rgdwAxes[2] = { 0,0 };
	LONG rglDirections[2] = { 0,0 };
	DICONSTANTFORCE dicf = { NULL };
	DIPERIODIC dipf = { NULL };
	int i;


	for ( i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// �ʏ�W���C�X�e�B�b�N������

		// �G�t�F�N�g�̏�����
		ZeroMemory(&diEffect, sizeof(DIEFFECT));
		ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

		// these fields are the same for all effects we will be creating
		diEffect.dwSize = sizeof(DIEFFECT);
		diEffect.dwSamplePeriod = 0;		// use default sample period
		diEffect.dwTriggerButton = DIEB_NOTRIGGER;
		diEffect.dwTriggerRepeatInterval = 0;
		diEffect.rgdwAxes = rgdwAxes;           // rgdwAxes�z��̃A�h���X�ݒ�
		diEffect.rglDirection = rglDirections;  // rglDirections�z��̃A�h���X�ݒ�
		diEffect.dwGain = FF_BODYBUILDER;    // �����@100%
		//diEffect.dwGain = FF_ADULT;    // �����@75%

		// BOUNCE�G�t�F�N�g�̐���
		if (m_pJoyEffect[i][EF_BOUNCE]) {
			m_pJoyEffect[i][EF_BOUNCE]->Release();
			m_pJoyEffect[i][EF_BOUNCE] = NULL;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = DIJOFS_Y;
		rglDirections[0] = 0;
		rglDirections[1] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
		diEffect.dwDuration = 200000;
		diEffect.cAxes = 2;
		diEffect.lpEnvelope = NULL;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_BOUNCE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		// FIRE�G�t�F�N�g�̐���
		if (m_pJoyEffect[i][EF_FIRE]) {
			m_pJoyEffect[i][EF_FIRE]->Release();
			m_pJoyEffect[i][EF_FIRE] = NULL;
		}

		dicf.lMagnitude = 3000;   // -- 2018.8.27

		rgdwAxes[0] = DIJOFS_Y;
		rgdwAxes[1] = 0;          // -- 2018.8.27
		rglDirections[0] = 1;
		rglDirections[1] = 0;     // -- 2018.8.27

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 20000;
		diEffect.cAxes = 2;       // -- 2018.8.27
		diEffect.lpEnvelope = NULL;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_FIRE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
		
		// EXPLODE�G�t�F�N�g�̐���
		if(m_pJoyEffect[i][EF_EXPLODE]) {
			m_pJoyEffect[i][EF_EXPLODE]->Release();
			m_pJoyEffect[i][EF_EXPLODE] = NULL;
		}

		diEnvelope.dwSize = sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel = 0;
		diEnvelope.dwAttackTime = 0;
		diEnvelope.dwFadeLevel = 0;
		diEnvelope.dwFadeTime = 1000000;

		dipf.dwMagnitude = 10000;
		dipf.lOffset = 0;
		dipf.dwPhase = 0;
		dipf.dwPeriod = 100000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = 0;        // -- 2018.8.27
		rglDirections[0] = 0;
		rglDirections[1] = 0;   // -- 2018.8.27

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 1000000;
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = &diEnvelope;
		diEffect.cbTypeSpecificParams = sizeof(DIPERIODIC);
		diEffect.lpvTypeSpecificParams = &dipf;

		if (m_pJoy[i]->CreateEffect(GUID_Square, &diEffect, &m_pJoyEffect[i][EF_EXPLODE], NULL) != DI_OK) {
			MessageBox(NULL, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}

		m_nJoyEFSum = 3;	// �W�����ʂ��R�o�^���ꂽ

	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// ForceFeedback���ʗ񋓂̃R�[���o�b�N�֐�  (for ForceFeedback Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumEffectsInFileProc(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
	
	return g_pDI->EnumEffectsInFileCb( lpdife,  pvRef );

}
//-----------------------------------------------------------------------------
// ForceFeedback���ʗ񋓂̃R�[���o�b�N(for ForceFeedback Joystic)�Ή��̃��\�b�h  
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
	HRESULT hr;

	hr = m_pJoy[m_nJoyEFI]->CreateEffect( lpdife->GuidEffect, lpdife->lpDiEffect,
												&m_pJoyEffect[m_nJoyEFI][m_nJoyEFSum], NULL );
	if( DI_OK != hr){
		MessageBox(NULL, _T("DInput.cpp : EnumEffectsInFileProc() : CreateEffect()���ُ�I�����܂����B"), NULL, MB_OK);
		return DIENUM_CONTINUE;	// ���̃f�o�C�X���
	}

	if( ++m_nJoyEFSum >=  JOY_EF_COUNT) return DIENUM_STOP;
	return DIENUM_CONTINUE;

}

//-----------------------------------------------------------------------------
// ForceFeedback�̌��ʂ̓ǂݍ��� (for ForceFeedback Joy)
// 
// ����		TCHAR* szFName	���ʃt�@�C����
//			int nEffectNo	���ʔԍ��i�߂�l�j�@
//			int nNum		��̌��ʂ̌��i�߂�l�j
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::ReadJoyEffect(TCHAR* szFName, int &nEffectNo, int &nNum)
{
	int  i, nWSum, nMaxSum;

	nMaxSum = nWSum = m_nJoyEFSum;
	nEffectNo = 0;
	nNum   = 0;

	for ( i=0; i<m_nJoySum; i++) {
		if( !m_pJoy[i] ) continue;
		if( !m_bJoyFF[i] )    continue;		// �ʏ�W���C�X�e�B�b�N������

		m_nJoyEFSum = nWSum;
		m_nJoyEFI   = i;

		if( m_pJoy[i]->EnumEffectsInFile( szFName, EnumEffectsInFileProc, NULL, DIFEF_MODIFYIFNEEDED ) != DI_OK ){
			MessageBox(NULL, _T("DInput.cpp : ReadJoyEffect() : EnumEffectsInFile()���ُ�I�����܂����B"), NULL, MB_OK);
			return FALSE;
		}
		if( m_nJoyEFSum > nMaxSum )  nMaxSum = m_nJoyEFSum;
	}
	m_nJoyEFSum = nMaxSum;
	nEffectNo   = nWSum;
	nNum        = m_nJoyEFSum - nEffectNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ForceFeedback�̌��ʉ��t
// 
// ����		int nEffectNo	���ʔԍ�(ReadJoyEffect�֐��̖߂�l���g�p����)�@
//			int nNum		��̌��ʂ̌�(ReadJoyEffect�֐��̖߂�l���g�p����)�B�ȗ��l�͂P
//			int nSum		�W���C�X�e�B�b�N�ԍ��B�ȗ��l��JOY_PLAYER1
// 
// �߂�l	TRUE:�����@FALSE:���s
//-----------------------------------------------------------------------------
BOOL CDirectInput::PlayJoyEffect(int nEffectNo, int nNum, int nSum)
{
	int  i;

	if( !m_bJoyFF[nSum] ) return FALSE;	// �ʏ�W���C�X�e�B�b�N������

	for( i = 0; i < nNum; i++ ){
		if (m_pJoyEffect[nSum][nEffectNo+i]) {
			if (m_pJoyEffect[nSum][nEffectNo+i]->Start(1, 0) != DI_OK) {
				MessageBox(NULL, _T("DInput.cpp : PlayJoyEffect() : Start(Joy effect)���ُ�I�����܂����B"), NULL, MB_OK);
				return FALSE;
			}
		}
	}
	return TRUE;
}