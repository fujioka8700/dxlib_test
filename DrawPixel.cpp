#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuMuki, SikakuGraph;

	ChangeWindowMode(TRUE);               //非全画面にセット
	SetGraphMode(640, 480, 16);           //画面サイズ指定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// ボール君のグラフィックをメモリにロード＆表示座標をセット
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// 四角君のグラフィックをメモリにロード＆表示座標をセット
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// 四角君の移動方向をセット
	SikakuMuki = 1;

	// 移動ルーチン
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// ボール君の操作ルーチン
		{
			// ↑キーを押していたらボール君を上に移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1) BallY -= 3;

			// ↓キーを押していたらボール君を下に移動させる
			if (CheckHitKey(KEY_INPUT_DOWN) == 1) BallY += 3;

			// ←キーを押していたらボール君を左に移動させる
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) BallX -= 3;

			// →キーを押していたらボール君を右に移動させる
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) BallX += 3;

			// ボール君が画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (BallX < 0) BallX = 0;

			// ボール君が画面右端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (BallX > 640 - 64) BallX = 640 - 64;

			// ボール君が画面上端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (BallY < 0) BallY = 0;

			// ボール君が画面下端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (BallY > 480 - 64) BallY = 480 - 64;

			// ボール君を描画
			DrawGraph(BallX, BallY, BallGraph, FALSE);
		}


		// 四角君の移動ルーチン
		{
			// 四角君の座標を移動している方向に移動する
			if (SikakuMuki == 1) SikakuX += 3;
			if (SikakuMuki == 0) SikakuX -= 3;

			// 四角君が画面右端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (SikakuX > 576)
			{
				SikakuX = 576;
				SikakuMuki = 0;
			}

			// 四角君が画面左端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
			if (SikakuX < 0)
			{
				SikakuX = 0;
				SikakuMuki = 1;
			}

			// 四角君を描画
			DrawGraph(SikakuX, SikakuY, SikakuGraph, FALSE);
		}

		// 裏画面の内容を表画面にコピーする
		ScreenFlip();


		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// -1 が返ってきたらループを抜ける
		if (ProcessMessage() < 0) break;

		// もしＥＳＣキーが押されていたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}