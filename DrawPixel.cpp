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

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	int x = 0, y = 0;

	while (1)
	{
		// ��ʂɕ`����Ă�����̂����ׂď���
		ClearDrawScreen();

		// �㉺���E�̃L�[���͂ɑΉ����� x, y �̍��W�l��ύX����
		if (CheckHitKey(KEY_INPUT_LEFT)  == 1) x -= 8;
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) x += 8;
		if (CheckHitKey(KEY_INPUT_UP)    == 1) y -= 8;
		if (CheckHitKey(KEY_INPUT_DOWN)  == 1) y += 8;

		// x , y ��������ʍ��W�ɂa�l�o�摜 test1.bmp ��`�悷��
		LoadGraphScreen(x, y, "test1.bmp", TRUE);

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// �҂��Ȃ��Ə�������������̂ł����łQ�O�~���b�҂�
		WaitTimer(20);

		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1) break;

		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}