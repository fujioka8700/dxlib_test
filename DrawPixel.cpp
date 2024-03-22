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
	int SikakuDamageFlag, SikakuDamageCounter, SikakuDamageGraph;
	int ShotX[SHOT], ShotY[SHOT], ShotFlag[SHOT], ShotGraph;
	int SikakuW, SikakuH, ShotW, ShotH;
	int ShotBFlag;
	int ETamaX, ETamaY, ETamaFlag;
	int ETamaW, ETamaH, ETamaGraph;
	int ETamaCounter;
	int WindowSizeX, WindowSizeY;

	// �{�[���N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// �l�p�N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// �l�p�N�̃_���[�W���̃O���t�B�b�N���������Ƀ��[�h
	SikakuDamageGraph = LoadGraph("SikakuDam.png");

	// �l�p�N�����c�߂Ă��邩�ǂ����̕ϐ��Ɂw�c�߂Ă��Ȃ��x��\���O����
	SikakuDamageFlag = 0;

	// �l�p�N�̊��c�߂Ă��鎞�Ԃ𑪂�J�E���^�ϐ��ɂO����
	SikakuDamageCounter = 0;

	// �l�p�N�̈ړ��������Z�b�g
	SikakuMuki = 1;

	// �G�̒e�̃O���t�B�b�N�����[�h
	ETamaGraph = LoadGraph("EShot.png");

	// �G�̒e�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(ETamaGraph, &ETamaW, &ETamaH);

	// �G�̒e�����ł��邩�ǂ�����ێ�����ϐ��Ɂw���ł��Ȃ��x��\���O����
	ETamaFlag = 0;

	// �G���e�����^�C�~���O����邽�߂̌v���p�ϐ��ɂO����
	ETamaCounter = 0;

	// �e�̃O���t�B�b�N���������Ƀ��[�h
	ShotGraph = LoadGraph("Shot.png");

	// �e�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(ShotGraph, &ShotW, &ShotH);

	// �l�p�N�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(SikakuGraph, &SikakuW, &SikakuH);

	// �e�P�E�Q����ʏ�ɑ��݂��Ă��邩�ێ�����ϐ��Ɂw���݂��Ă��Ȃ��x���Ӗ�����O�������Ă���
	for (int i = 0; i < SHOT; i++)
	{
		ShotFlag[i] = 0;
		ShotX[i] = 0;
		ShotY[i] = 0;
	}

	// �V���b�g�{�^�����O�̃t���[���ŉ����ꂽ���ǂ�����ۑ�����ϐ��ɂO(�����ꂢ�Ȃ�)����
	ShotBFlag = 0;

	// �E�B���h�E�T�C�Y���擾����
	GetWindowSize(&WindowSizeX, &WindowSizeY);

	// �ړ����[�`��
	while (TRUE)
	{
		// ��ʂ�������(�^�����ɂ���)
		ClearDrawScreen();

		// �ϐ���\������
#ifdef _DEBUG
		DrawFormatString(0,  0, GetColor(255, 255, 255), "ETamaFlag : %d", ETamaFlag);
		DrawFormatString(0, 15, GetColor(255, 255, 255), "ETamaCounter : %d", ETamaCounter);
		//DrawFormatString(0, 30, GetColor(255, 255, 255), "ShotX[0] : %d", ShotX[0]);
		//DrawFormatString(0, 45, GetColor(255, 255, 255), "ShotY[0] : %d", ShotY[0]);
		//DrawFormatString(0, 60, GetColor(255, 255, 255), "ShotBFlag : %d", ShotBFlag);
		//DrawFormatString(0, 75, GetColor(255, 255, 255), "SikakuDamageFlag : %d", SikakuDamageFlag);
		//DrawFormatString(0, 90, GetColor(255, 255, 255), "SikakuDamageCounter : %d", SikakuDamageCounter);
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
				// �O�t���[���ŃV���b�g�{�^���������������ۑ�����Ă���ϐ����O��������e�𔭎�
				if (ShotBFlag == 0)
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

				// �O�t���[���ŃV���b�g�{�^����������Ă�������ۑ�����ϐ��ɂP(������Ă���)����
				ShotBFlag = 1;
			}
			else
			{
				// �V���b�g�{�^����������Ă��Ȃ������ꍇ��
				// �O�t���[���ŃV���b�g�{�^����������Ă�������ۑ�����ϐ��ɂO(������Ă��Ȃ�)����
				ShotBFlag = 0;
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
			// ���c�߂Ă��邩�ǂ����ŏ����𕪊�
			if (SikakuDamageFlag == 1)
			{
				// ���c�߂Ă���ꍇ�̓_���[�W���̃O���t�B�b�N��`�悷��
				DrawGraph(SikakuX, SikakuY, SikakuDamageGraph, FALSE);

				// ���c�߂Ă��鎞�Ԃ𑪂�J�E���^�[�ɂP�����Z����
				SikakuDamageCounter++;

				// �������c�ߏ��߂� �R�O �t���[���o�߂��Ă������̘c�񂾏�Ԃ���
				// ���ɖ߂��Ă�����
				if (SikakuDamageCounter == 30)
				{
					// �w�c��ł��Ȃ��x��\���O����
					SikakuDamageFlag = 0;
				}
			}
			else
			{
				// �c��ł��Ȃ��ꍇ�͍��܂Œʂ�̏���

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

				// �e�����^�C�~���O���v�����邽�߂̃J�E���^�[�ɂP�𑫂�
				ETamaCounter++;

				// �����J�E���^�[�ϐ����U�O�������ꍇ�͒e�����������s��
				if (ETamaCounter == 60)
				{
					// �������ɒe���w���ł��Ȃ��x��Ԃ������ꍇ�̂ݔ��ˏ������s��
					if (ETamaFlag == 0)
					{
						// �e�̔��ˈʒu��ݒ肷��
						ETamaX = SikakuX + SikakuW / 2 - ETamaW / 2;
						ETamaY = SikakuY + SikakuH / 2 - ETamaH / 2;

						// �e�̏�Ԃ�ێ�����ϐ��Ɂw���ł���x�������P��������
						ETamaFlag = 1;
					}

					// �e��ł^�C�~���O���v�����邽�߂̕ϐ��ɂO����
					ETamaCounter = 0;
				}
			}
		}

		// �G�̒e�̏�Ԃ��w���ł���x�ꍇ�̂ݒe�̈ړ��������s��
		if (ETamaFlag == 1)
		{
			// �������ɂ��炷
			ETamaY += 8;

			// �����e����ʉ��[����͂ݏo�Ă��܂����ꍇ�͒e�̏�Ԃ��w���ł��Ȃ��x
			// ��\���O�ɂ���
			if (ETamaY > 480) ETamaFlag = 0;

			// ��ʂɕ`�悷��( ETamaGraph : �G�̒e�̃O���t�B�b�N�̃n���h�� )
			DrawGraph(ETamaX, ETamaY, ETamaGraph, FALSE);
		}

		// �e�ƓG�̓����蔻��A�e�̐������J��Ԃ�
		for (int i = 0; i < SHOT; i++)
		{
			// �ei�����݂��Ă���ꍇ�̂ݎ��̏����ɉf��
			if (ShotFlag[i] == 1)
			{
				// �l�p�N�Ƃ̓����蔻��
				if (((ShotX[i] > SikakuX && ShotX[i] < SikakuX + SikakuW) ||
					(SikakuX > ShotX[i] && SikakuX < ShotX[i] + ShotW)) &&
					((ShotY[i] > SikakuY && ShotY[i] < SikakuY + SikakuH) ||
						(SikakuY > ShotY[i] && SikakuY < ShotY[i] + ShotH)))
				{
					// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
					ShotFlag[i] = 0;

					// �l�p�N�̊��c�߂Ă��邩�ǂ�����ێ�����ϐ��Ɂw�c�߂Ă���x��\���P����
					SikakuDamageFlag = 1;

					// �l�p�N�̊��c�߂Ă��鎞�Ԃ𑪂�J�E���^�ϐ��ɂO����
					SikakuDamageCounter = 0;
				}
			}
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