#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);               //非全画面にセット
	SetGraphMode(640, 480, 32);           //画面サイズ指定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	int x = 0, y = 0;
	int GraphHandle;

	// グラフィック『test1.bmp』をメモリにロード
	GraphHandle = LoadGraph("test1.bmp");

	while (true)
	{
		// 画面に描かれているものをすべて消す
		ClearDrawScreen();

		// 上下左右のキー入力に対応して x, y の座標値を変更する
		if (CheckHitKey(KEY_INPUT_LEFT)  == 1) x -= 8;
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) x += 8;
		if (CheckHitKey(KEY_INPUT_UP)    == 1) y -= 8;
		if (CheckHitKey(KEY_INPUT_DOWN)  == 1) y += 8;

		// 0,0 座標にメモリに読みこんだグラフィックを描画
		DrawGraph(x, y, GraphHandle, FALSE);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 待たないと処理が早すぎるのでここで２０ミリ秒待つ
		WaitTimer(20);

		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1) break;

		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}