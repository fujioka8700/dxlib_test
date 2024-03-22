#include "DxLib.h"

#define SHOT 2

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
#ifndef _DEBUG
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɐݒ�
#endif // !_DEBUG

	ChangeWindowMode(TRUE);               //��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 16);           //��ʃT�C�Y�w��

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �w�i�F�����F�ɕύX
	SetBackgroundColor(0, 0, 0);

	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuMuki, SikakuGraph;
	int ShotX[SHOT], ShotY[SHOT], ShotFlag[SHOT], ShotGraph;
	int WindowSizeX, WindowSizeY;

	// �{�[���N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// �l�p�N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// �l�p�N�̈ړ��������Z�b�g
	SikakuMuki = 1;

	// �e�̃O���t�B�b�N���������Ƀ��[�h
	ShotGraph = LoadGraph("Shot.png");

	// �e�P�E�Q����ʏ�ɑ��݂��Ă��邩�ێ�����ϐ��Ɂw���݂��Ă��Ȃ��x���Ӗ�����O�������Ă���
	for (int i = 0; i < SHOT; i++)
	{
		ShotFlag[i] = 0;
		ShotX[i] = 0;
		ShotY[i] = 0;
	}

	// �E�B���h�E�T�C�Y���擾����
	GetWindowSize(&WindowSizeX, &WindowSizeY);

	// �ړ����[�`��
	while (TRUE)
	{
		// ��ʂ�������(�^�����ɂ���)
		ClearDrawScreen();

		// �ϐ���\������
#ifdef _DEBUG
		DrawFormatString(0,  0, GetColor(255, 255, 255), "ShotFlag[0] : %d", ShotFlag[0]);
		DrawFormatString(0, 15, GetColor(255, 255, 255), "ShotFlag[1] : %d", ShotFlag[1]);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "ShotX[0] : %d", ShotX[0]);
		DrawFormatString(0, 45, GetColor(255, 255, 255), "ShotY[0] : %d", ShotY[0]);
#endif // _DEBUG

		// �{�[���N�̑��샋�[�`��
		{
			int key = GetJoypadInputState(DX_INPUT_KEY_PAD1); // �L�[�E�p�b�h�P�̓���
			if (key & PAD_INPUT_UP)    BallY -= 3; // �オ������Ă���
			if (key & PAD_INPUT_DOWN)  BallY += 3; // ����������Ă���
			if (key & PAD_INPUT_LEFT)  BallX -= 3; // ����������Ă���
			if (key & PAD_INPUT_RIGHT) BallX += 3; // �E��������Ă���

#ifdef _DEBUG
			// ���͂���Ă���p�b�h�̃{�^���\��
			for (int i = 0; i < 28; i++) { // �{�^��28�����[�v
				if (key & (1 << i)) {      // �{�^��i�̓��̓t���O�������Ă�����
					DrawFormatString(0, (i * 15) + WindowSizeY / 2, GetColor(255, 255, 255), "%d�̃L�[��������Ă��܂�", i);
				}
			}
#endif // _DEBUG

			// �{�^���P���������ꍇ�͏����𕪊�
			if (key & PAD_INPUT_1)
			{
				// �ei����ʏ�ɂłĂ��Ȃ��ꍇ�͂��̒e����ʂɏo��
				for (int i = 0; i < SHOT; i++)
				{
					if (ShotFlag[i] == 0)
					{
						int Bw, Bh, Sw, Sh;

						// �{�[���N�ƒe�̉摜�̃T�C�Y�𓾂�
						GetGraphSize(BallGraph, &Bw, &Bh);
						GetGraphSize(ShotGraph, &Sw, &Sh);

						// �e�̈ʒu���Z�b�g�A�ʒu�̓{�[���N�̒��S�ɂ���
						ShotX[i] = (Bw - Sw) / 2 + BallX;
						ShotY[i] = (Bh - Sh) / 2 + BallY;

						// �e�͌����_�������đ��݂���̂ŁA���ݏ�Ԃ�ێ�����ϐ��ɂP��������
						ShotFlag[i] = 1;

						// ��e���o�����̂Œe���o�����[�v���甲���܂�
						break;
					}
				}
			}

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

		// �e�̐������e�𓮂����������J��Ԃ�
		for (int i = 0; i < SHOT; i++)
		{
			// ���@�̒e�̈ړ����[�`��( ���ݏ�Ԃ�ێ����Ă���ϐ��̓��e���P(���݂���)�̏ꍇ�̂ݍs�� )
			if (ShotFlag[i] == 1)
			{
				// �e���P�U�h�b�g��Ɉړ�������
				ShotY[i] -= 16;

				// ��ʊO�ɏo�Ă��܂����ꍇ�͑��ݏ�Ԃ�ێ����Ă���ϐ��ɂO(���݂��Ȃ�)��������
				if (ShotY[i] < -80)
				{
					ShotFlag[i] = 0;
				}

				// ��ʂɒe��`�悷��
				DrawGraph(ShotX[i], ShotY[i], ShotGraph, FALSE);
			}
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

	DxLib_End(); // �c�w���C�u�����g�p�̏I������

	return 0;    // �\�t�g�̏I��
}