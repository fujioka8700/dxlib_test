#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int i;

	ChangeWindowMode(TRUE);               //非全画面にセット
	SetGraphMode(640, 480, 32);           //画面サイズ指定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 一番左上に絵を表示、２０分の１秒待つ、を１３回繰り返す
	for (i = 0; i < 13; i++)
	{
		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// 絵を表示、絵を描くＸ座標は i が１増えるごとに１０ドットづつ右にずれてゆく
		LoadGraphScreen(i * 20, 100, "test1.bmp", FALSE);

		// ２０分の１秒待つ
		WaitTimer(50);
	}

	WaitKey();				// キー入力待ち

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}