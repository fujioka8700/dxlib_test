#include "DxLib.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuGraph;
	int i;

	ChangeWindowMode(TRUE);               //��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 16);           //��ʃT�C�Y�w��
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �{�[���N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	BallGraph = LoadGraph("Ball.png");
	BallX = -64; BallY = 0;

	// �l�p�N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 640; SikakuY = 300;

	// �ړ����[�`��
	for (i = 0; i < 400; i++)
	{
		// ��ʂ�������(�^�����ɂ���)
		ClearDrawScreen();

		// �{�[���N�̍��W���������炷
		BallX += 3;

		// �{�[���N��`��
		DrawGraph(BallX, BallY, BallGraph, FALSE);

		// �l�p�N�̍��W���������炷
		SikakuX -= 3;
		SikakuY -= 2;

		// �l�p�N��`��
		DrawGraph(SikakuX, SikakuY, SikakuGraph, FALSE);

		// ����ʂ̓��e��\��ʂɃR�s�[����
		ScreenFlip();

		// Windows ���L�̖ʓ|�ȏ������c�w���C�u�����ɂ�点��
		// -1 ���Ԃ��Ă����烋�[�v�𔲂���
		if (ProcessMessage() < 0) break;

		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}