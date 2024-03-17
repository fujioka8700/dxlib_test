#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuGraph;
	int i;

	ChangeWindowMode(TRUE);               //非全画面にセット
	SetGraphMode(640, 480, 16);           //画面サイズ指定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// ボール君のグラフィックをメモリにロード＆表示座標をセット
	BallGraph = LoadGraph("Ball.png");
	BallX = -64; BallY = 0;

	// 四角君のグラフィックをメモリにロード＆表示座標をセット
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 640; SikakuY = 300;

	// 移動ルーチン
	for (i = 0; i < 400; i++)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// ボール君の座標を少しずらす
		BallX += 3;

		// ボール君を描画
		DrawGraph(BallX, BallY, BallGraph, FALSE);

		// 四角君の座標を少しずらす
		SikakuX -= 3;
		SikakuY -= 2;

		// 四角君を描画
		DrawGraph(SikakuX, SikakuY, SikakuGraph, FALSE);

		// 裏画面の内容を表画面にコピーする
		ScreenFlip();

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// -1 が返ってきたらループを抜ける
		if (ProcessMessage() < 0) break;

		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}