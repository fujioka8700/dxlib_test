#include "DxLib.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int i;

	ChangeWindowMode(TRUE);               //��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 32);           //��ʃT�C�Y�w��
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// ��ԍ���ɊG��\���A�Q�O���̂P�b�҂A���P�R��J��Ԃ�
	for (i = 0; i < 13; i++)
	{
		// ��ʂɕ`����Ă�����̂����S������
		ClearDrawScreen();

		// �G��\���A�G��`���w���W�� i ���P�����邲�ƂɂP�O�h�b�g�ÂE�ɂ���Ă䂭
		LoadGraphScreen(i * 20, 100, "test1.bmp", FALSE);

		// �Q�O���̂P�b�҂�
		WaitTimer(50);
	}

	WaitKey();				// �L�[���͑҂�

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}