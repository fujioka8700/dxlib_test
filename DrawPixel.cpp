#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
#ifndef _DEBUG
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないように設定
#endif // !_DEBUG

	ChangeWindowMode(TRUE);               //非全画面にセット
	SetGraphMode(640, 480, 16);           //画面サイズ指定

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 背景色を黒色に変更
	SetBackgroundColor(0, 0, 0);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	int BallX, BallY, BallGraph;
	int SikakuX, SikakuY, SikakuMuki, SikakuGraph;
	int Shot1X, Shot1Y, Shot1Flag, ShotGraph;
	int Shot2X, Shot2Y, Shot2Flag;
	int WindowSizeX, WindowSizeY;

	// ボール君のグラフィックをメモリにロード＆表示座標をセット
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// 四角君のグラフィックをメモリにロード＆表示座標をセット
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// 四角君の移動方向をセット
	SikakuMuki = 1;

	// 弾のグラフィックをメモリにロード
	ShotGraph = LoadGraph("Shot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味する０を代入しておく
	Shot1Flag = 0;
	Shot2Flag = 0;

	// 弾の位置
	Shot1X = 0; Shot1Y = 0;

	// ウィンドウサイズを取得する
	GetWindowSize(&WindowSizeX, &WindowSizeY);

	// 移動ルーチン
	while (TRUE)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 変数を表示する
#ifdef _DEBUG
		DrawFormatString(0,  0, GetColor(255, 255, 255), "Shot1Flag : %d", Shot1Flag);
		DrawFormatString(0, 15, GetColor(255, 255, 255), "Shot2Flag : %d", Shot2Flag);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "Shot1X : %d", Shot1X);
		DrawFormatString(0, 45, GetColor(255, 255, 255), "Shot1Y : %d", Shot1Y);
#endif // _DEBUG

		// ボール君の操作ルーチン
		{
			int key = GetJoypadInputState(DX_INPUT_KEY_PAD1); // キー・パッド１の入力
			if (key & PAD_INPUT_UP)    BallY -= 3; // 上が押されている
			if (key & PAD_INPUT_DOWN)  BallY += 3; // 下が押されている
			if (key & PAD_INPUT_LEFT)  BallX -= 3; // 左が押されている
			if (key & PAD_INPUT_RIGHT) BallX += 3; // 右が押されている

#ifdef _DEBUG
			// 入力されているパッドのボタン表示
			for (int i = 0; i < 28; i++) { // ボタン28個分ループ
				if (key & (1 << i)) {      // ボタンiの入力フラグが立っていたら
					DrawFormatString(0, (i * 15) + WindowSizeY / 2, GetColor(255, 255, 255), "%dのキーが押されています", i);
				}
			}
#endif // _DEBUG

			// スペースキーを押していて、且弾が撃ち出されていなかったら弾を発射する
			if (key & PAD_INPUT_1)
			{
				if (Shot1Flag == 0)
				{
					int Bw, Bh, Sw, Sh;

					// ボール君と弾の画像のサイズを得る
					GetGraphSize(BallGraph, &Bw, &Bh);
					GetGraphSize(ShotGraph, &Sw, &Sh);

					// 弾の位置をセット、位置はボール君の中心にする
					Shot1X = (Bw - Sw) / 2 + BallX;
					Shot1Y = (Bh - Sh) / 2 + BallY;

					// 弾は現時点を持って存在するので、存在状態を保持する変数に１を代入する
					Shot1Flag = 1;
				}
				else if (Shot2Flag == 0)
				{
					// 弾２が画面に存在していない場合は弾２を出す
					int Bw, Bh, Sw, Sh;

					// ボール君と弾の画像のサイズを得る
					GetGraphSize(BallGraph, &Bw, &Bh);
					GetGraphSize(ShotGraph, &Sw, &Sh);

					// 弾２の位置をセット、位置はボール君の中心にする
					Shot2X = (Bw - Sw) / 2 + BallX;
					Shot2Y = (Bh - Sh) / 2 + BallY;

					// 弾２は現時点を持って存在するので、存在状態を保持する変数に１を代入する
					Shot2Flag = 1;
				}
			}

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

		// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容が１(存在する)の場合のみ行う )
		if (Shot1Flag == 1)
		{
			// 弾を１６ドット上に移動させる
			Shot1Y -= 16;

			// 画面外に出てしまった場合は存在状態を保持している変数に０(存在しない)を代入する
			if (Shot1Y < -80)
			{
				Shot1Flag = 0;
			}

			// 画面に弾を描画する
			DrawGraph(Shot1X, Shot1Y, ShotGraph, FALSE);
		}

		// 自機の弾２の移動ルーチン( 存在状態を保持している変数の内容が１(存在する)の場合のみ行う )
		if (Shot2Flag == 1)
		{
			// 弾２を１６ドット上に移動させる
			Shot2Y -= 16;

			// 画面外に出てしまった場合は存在状態を保持している変数に０(存在しない)を代入する
			if (Shot2Y < -80)
			{
				Shot2Flag = 0;
			}

			// 画面に弾２を描画する
			DrawGraph(Shot2X, Shot2Y, ShotGraph, FALSE);
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

	DxLib_End(); // ＤＸライブラリ使用の終了処理

	return 0;    // ソフトの終了
}