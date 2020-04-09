#include <stdio.h>
#include <windows.h>		//Windows��
#include <windowsx.h>		//Windows��
#include <process.h>		//�X���b�h�p
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")//�����x�^�C�}

#include "resource.h"		//���\�[�X�t�@�C��

//�֐��錾
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);	//���C���_�C�A���O�v���V�[�W��
BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, LPCTSTR cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hDC);//�q�E�B���h�E�𐶐�
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//�q�E�B���h�E�v���V�[�W��

//�萔�錾
#define DEF_APP_NAME	TEXT("Waveform Test")
#define DEF_MUTEX_NAME	DEF_APP_NAME			//�~���[�e�b�N�X��
#define DTMAX 8192

static COLORREF	color, colorPen;	//�F
static FILE *fp;

//======================================
//��������_�C�A���O�o�[�W����

//���C���֐�(�_�C�A���O�o�[�W����)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HANDLE hMutex;

	//���d�N������
	hMutex = CreateMutex(NULL, TRUE, DEF_MUTEX_NAME);		//�~���[�e�b�N�X�I�u�W�F�N�g�̐���
	if (GetLastError() == ERROR_ALREADY_EXISTS) {				//2�d�N���̗L�����m�F
		MessageBox(NULL, TEXT("���ɋN������Ă��܂��D"), NULL, MB_OK | MB_ICONERROR);
		return 0;											//�I��
	}

	//�_�C�A���O�N��
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainDlgProc);

	return FALSE;			//�I��
}

//�ҏW����̂͂�������

//���C���v���V�[�W���i�_�C�A���O�j
/********************************

�V�X�e����ʁi.rc�t�@�C���j�ɔz�u�����{�^�����������ꂽ�Ƃ���C
�������C�I�����ɂǂ̂悤�ȏ������s�����������ɋL�q����D

********************************/
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hPict1, hPict2;		//�E�B���h�E�n���h���iPictureBox�j
	static HWND hWnd;		//�q�E�B���h�E�n���h��
	static HWND hStart;

	switch (uMsg) {
	case WM_INITDIALOG:		//�_�C�A���O������(exe���_�u���N���b�N������)


		/*******
		�V�X�e����ʁi.rc�t�@�C���j�Ɏ����Œǉ������{�^����Picture Control��
		�͕K��HWND�^��GetDlgItem�֐���p���ăn���h�����擾���܂��D

		��FhWnd = GetDlgItem(hDlg, ID);
		hWnd�F�擾�������n���h��
		hDlg�F�_�C�A���O�̃n���h���D�����͊�{�I�ɂ��̂܂�
		ID�F�擾�������n���h���̑ΏۂƂȂ�{�^����Picture Control��
			��ID�D.rc�t�@�C������m�F�ł���D�ڍׂ͔z�z�������Q�l



		�����Ńw�b�_�����Ă݂�ƁChRadioR�EhRadioG�EhRadioB�Ƃ����n���h�����錾
		����Ă��܂��D
		��������ꂼ��V�X�e����ʏ�̃��W�I�{�^���ԁE�΁E�ɑΉ�����悤��
		�錾���Ă݂܂��傤�D
	   ********/
	   //��������



		//�����܂�

		SendMessage(hDlg, WM_COMMAND, 0, 0);	//
		return TRUE;

	case WM_COMMAND:		//�{�^���������ꂽ��
		/*******
		case WM_COMMAND:�ł͊e�{�^���������ꂽ�Ƃ��̏������L�ڂ��܂��D
	   ********/
		switch (LOWORD(wParam)) {
		case ID_START:			//�J�n�{�^��
			/***********************
			OK�{�^���������ꂽ�Ƃ��ɕ`����J�n���܂��D
			�`����J�n���邽�߂ɂ�PictureControl�̃n���h�����擾����K�v������܂�
			�����Q�l�Ƀn���h�� hPict��GetDlgItem�֐���p���Ď擾���܂��傤�D
			***********************/
			//��������
			hStart = GetDlgItem(hDlg, ID_START);
			EnableWindow(hStart, FALSE);
			hPict1 = GetDlgItem(hDlg, IDC_PICTBOX1);
			hPict2 = GetDlgItem(hDlg, IDC_PICTBOX2);



			//�����܂�	
			WinInitialize(NULL, hDlg, (HMENU)110, TEXT("TEST1"), hPict1, WndProc, &hWnd); //������
			WinInitialize(NULL, hDlg, (HMENU)110, TEXT("TEST2"), hPict2, WndProc, &hWnd);
			return TRUE;

		case ID_END:		//�L�����Z���{�^��
			EndDialog(hDlg, 0);		//�_�C�A���O�I��
			return TRUE;

		}
		break;



	case WM_CTLCOLORDLG:	//�_�C�A���O�w�i�`��
		return ((BOOL)GetStockObject(WHITE_BRUSH));			//�_�C�A���O�w�i�𔒂�

	case WM_CLOSE:
		EndDialog(hDlg, 0);			//�_�C�A���O�I��
		return TRUE;
	}

	SendMessage(hWnd, uMsg, wParam, lParam);

	//�I�[�i�[�`���ɍĕ`��
	if (uMsg == WM_PAINT) {
		InvalidateRect(hWnd, NULL, TRUE);	//�ĕ`��
	}

	return FALSE;
}

/********************************

������PictureControl�̕`����s���܂��D

�q�E�B���h�E�v���V�[�W������hWnd��PictureControl�̃n���h���ł��D

********************************/
//�q�E�B���h�E�v���V�[�W��
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC			hdc;				//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;					//(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��	
	HBRUSH		hBrush, hOldBrush;	//�u���V
	HPEN		hPen, hOldPen;		//�y��

	switch (uMsg) {
	case WM_CREATE:
		colorPen = RGB(255, 255, 255);	//�F�w��
		break;

	case WM_PAINT:

		/********************************

		PictureControl�ɕ`�悷�邽�߂ɂ́CHDC�^�̃n���h����ʂɎ擾����
		�K�v������܂��D

		��Fhdc = BeginPaint(hWnd, &ps);
		hdc:�f�o�C�X�R���e�L�X�g�̃n���h��
		hWnd:PictureControl�̃n���h��
		ps�F(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��

		********************************/

		hdc = BeginPaint(hWnd, &ps);//�f�o�C�X�R���e�L�X�g�̃n���h���擾

		/********************************

		PictureControl�ɕ`�悷�邽�߂ɂ́C�������������Ƃ��̓y���C
		�h��Ԃ��ۂɂ̓u���V���K�v�ł��D

		********************************/

		//�y���C�u���V����
		hBrush = CreateSolidBrush(color);				//�u���V����
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);	//�u���V�ݒ�
		hPen = CreatePen(PS_SOLID, 2, colorPen);		//�y������
		hOldPen = (HPEN)SelectObject(hdc, hPen);		//�y���ݒ�

		//�`��
		/********************************

		�}�`��`�悷�邽�߂ɂ͈ȉ��̊֐���p���܂��D
		�����`�FRectangle(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);
		�~�FEllipse(HDC hdc ,int nLeftRect , int nTopRect ,int nRightRect , int nBottomRect);

		 nLiftRect�F�����`�̍���X���W
		  nTopRect�F����Y���W
		  nRightRect�F�E��X���W
		  nBottomRect�F�E����Y���W

		���������ɂ͈ȉ��̊֐���p���܂��D

		���̎n�_�ݒ�FMoveToEx(HDC hdc , int X , int Y , NULL);
		  X,Y�F���̎n�_�̍��W
		���GLineTo(HDC hdc , int nXEnd , int nYEnd);
		  nXEnd, nYEnd�F���̏I�_�̐ݒ�


		  �ȏ���Q�l�ɐ}�`��`�悷��֐����ȉ��ɋL�q���܂��傤
		********************************/
		//��������
		MoveToEx(hdc, 30, 75, NULL);
		LineTo(hdc, 700, 75);
		MoveToEx(hdc, 30, 10, NULL);
		LineTo(hdc, 30, 150);



		//�����܂�
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 350, 140, TEXT("Time [s]"), 8);		//�e�L�X�g�`��

		//�y���C�u���V�p��
		/********************************

		�g���I������y���ƃu���V�͔j������K�v������܂��D

		********************************/
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);

		//�f�o�C�X�R���e�L�X�g�̃n���h���j��
		EndPaint(hWnd, &ps);
		break;
	}

	return TRUE;
}

//�����܂�

//-----------------------------------------------------------------------------
//�q�E�B���h�E������������
//�w�肵���E�B���h�E�n���h���̗̈�Ɏq�E�B���h�E�𐶐�����D
//----------------------------------------------------------
// hInst	: �����p�C���X�^���X�n���h��
// hPaWnd	: �e�E�B���h�E�̃n���h��
// chID		: �q�E�B���h�E��ID
// cWinName	: �q�E�B���h�E��
// PaintArea: �q�E�B���h�E�𐶐�����̈�̃f�o�C�X�n���h��
// WndProc	: �E�B���h�E�v���V�[�W��
// *hWnd	: �q�E�B���h�E�̃n���h���i�|�C���^�j
// �߂�l	: ������=true
//-----------------------------------------------------------------------------
BOOL WinInitialize(HINSTANCE hInst, HWND hPaWnd, HMENU chID, LPCTSTR cWinName, HWND PaintArea, WNDPROC WndProc, HWND* hWnd)
{
	WNDCLASS wc;			//�E�B���h�E�N���X
	WINDOWPLACEMENT	wplace;	//�q�E�B���h�E�����̈�v�Z�p�i��ʏ�̃E�B���h�E�̔z�u�����i�[����\���́j
	RECT WinRect;			//�q�E�B���h�E�����̈�
	ATOM atom;				//�A�g��

	//�E�B���h�E�N���X������
	wc.style = CS_HREDRAW ^ WS_MAXIMIZEBOX | CS_VREDRAW;	//�E�C���h�E�X�^�C��
	wc.lpfnWndProc = WndProc;									//�E�C���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^
	wc.cbClsExtra = 0;											//
	wc.cbWndExtra = 0;
	wc.hCursor = NULL;										//�v���O�����̃n���h��
	wc.hIcon = NULL;										//�A�C�R���̃n���h��
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//�E�C���h�E�w�i�F
	wc.hInstance = hInst;										//�E�C���h�E�v���V�[�W��������C���X�^���X�n���h��
	wc.lpszMenuName = NULL;										//���j���[��
	wc.lpszClassName = (LPCTSTR)cWinName;									//�E�C���h�E�N���X��

	if (!(atom = RegisterClass(&wc))) {
		MessageBox(hPaWnd, TEXT("�E�B���h�E�N���X�̐����Ɏ��s���܂����D"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	GetWindowPlacement(PaintArea, &wplace);	//�`��̈�n���h���̏����擾(�E�B���h�E�̕\����Ԃ��擾)
	WinRect = wplace.rcNormalPosition;		//�`��̈�̐ݒ�

	//�E�B���h�E����
	*hWnd = CreateWindow(
		(LPCTSTR)atom,
		(LPCTSTR)cWinName,
		WS_CHILD | WS_VISIBLE,//| WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME |WS_VISIBLE, 
		WinRect.left, WinRect.top,
		WinRect.right - WinRect.left, WinRect.bottom - WinRect.top,
		hPaWnd, chID, hInst, NULL
	);

	if (*hWnd == NULL) {
		MessageBox(hPaWnd, TEXT("�E�B���h�E�̐����Ɏ��s���܂����D"), NULL, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
