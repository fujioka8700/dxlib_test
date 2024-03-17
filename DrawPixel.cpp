#include "DxLib.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);               //��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 32);           //��ʃT�C�Y�w��
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�

	int GraphHandle;

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �O���t�B�b�N�wtest1.bmp�x���������Ƀ��[�h
	GraphHandle = LoadGraph("test1.bmp");

	// 0,0 ���W�Ƀ������ɓǂ݂��񂾃O���t�B�b�N��`��
	DrawGraph(0, 0, GraphHandle, FALSE);

	// �L�[���͂�҂�
	WaitKey();

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}