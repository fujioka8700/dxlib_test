#include "DxLib.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuMuki, SikakuGraph;

	ChangeWindowMode(TRUE);               //��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 16);           //��ʃT�C�Y�w��
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	// �{�[���N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// �l�p�N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// �l�p�N�̈ړ��������Z�b�g
	SikakuMuki = 1;

	// �ړ����[�`��
	while (1)
	{
		// ��ʂ�������(�^�����ɂ���)
		ClearDrawScreen();

		// �{�[���N�̑��샋�[�`��
		{
			// ���L�[�������Ă�����{�[���N����Ɉړ�������
			if (CheckHitKey(KEY_INPUT_UP) == 1) BallY -= 3;

			// ���L�[�������Ă�����{�[���N�����Ɉړ�������
			if (CheckHitKey(KEY_INPUT_DOWN) == 1) BallY += 3;

			// ���L�[�������Ă�����{�[���N�����Ɉړ�������
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) BallX -= 3;

			// ���L�[�������Ă�����{�[���N���E�Ɉړ�������
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) BallX += 3;

			// �{�[���N����ʍ��[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
			if (BallX < 0) BallX = 0;

			// �{�[���N����ʉE�[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
			if (BallX > 640 - 64) BallX = 640 - 64;

			// �{�[���N����ʏ�[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
			if (BallY < 0) BallY = 0;

			// �{�[���N����ʉ��[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
			if (BallY > 480 - 64) BallY = 480 - 64;

			// �{�[���N��`��
			DrawGraph(BallX, BallY, BallGraph, FALSE);
		}


		// �l�p�N�̈ړ����[�`��
		{
			// �l�p�N�̍��W���ړ����Ă�������Ɉړ�����
			if (SikakuMuki == 1) SikakuX += 3;
			if (SikakuMuki == 0) SikakuX -= 3;

			// �l�p�N����ʉE�[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
			if (SikakuX > 576)
			{
				SikakuX = 576;
				SikakuMuki = 0;
			}

			// �l�p�N����ʍ��[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
			if (SikakuX < 0)
			{
				SikakuX = 0;
				SikakuMuki = 1;
			}

			// �l�p�N��`��
			DrawGraph(SikakuX, SikakuY, SikakuGraph, FALSE);
		}

		// ����ʂ̓��e��\��ʂɃR�s�[����
		ScreenFlip();


		// Windows ���L�̖ʓ|�ȏ������c�w���C�u�����ɂ�点��
		// -1 ���Ԃ��Ă����烋�[�v�𔲂���
		if (ProcessMessage() < 0) break;

		// �����d�r�b�L�[��������Ă����烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}