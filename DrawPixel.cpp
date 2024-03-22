#include "DxLib.h"

#define SHOT 2

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
	int SikakuDamageFlag, SikakuDamageCounter, SikakuDamageGraph;
	int ShotX[SHOT], ShotY[SHOT], ShotFlag[SHOT], ShotGraph;
	int SikakuW, SikakuH, ShotW, ShotH;
	int ShotBFlag;
	int WindowSizeX, WindowSizeY;

	// ボール君のグラフィックをメモリにロード＆表示座標をセット
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// 四角君のグラフィックをメモリにロード＆表示座標をセット
	SikakuGraph = LoadGraph("Sikaku.png");
	SikakuX = 0; SikakuY = 50;

	// 四角君のダメージ時のグラフィックをメモリにロード
	SikakuDamageGraph = LoadGraph("SikakuDam.png");

	// 四角君が顔を歪めているかどうかの変数に『歪めていない』を表す０を代入
	SikakuDamageFlag = 0;

	// 四角君の顔を歪めている時間を測るカウンタ変数に０を代入
	SikakuDamageCounter = 0;

	// 四角君の移動方向をセット
	SikakuMuki = 1;

	// 弾のグラフィックをメモリにロード
	ShotGraph = LoadGraph("Shot.png");

	// 弾のグラフィックのサイズを得る
	GetGraphSize(ShotGraph, &ShotW, &ShotH);

	// 四角君のグラフィックのサイズを得る
	GetGraphSize(SikakuGraph, &SikakuW, &SikakuH);

	// 弾１・２が画面上に存在しているか保持する変数に『存在していない』を意味する０を代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		ShotFlag[i] = 0;
		ShotX[i] = 0;
		ShotY[i] = 0;
	}

	// ショットボタンが前のフレームで押されたかどうかを保存する変数に０(押されいない)を代入
	ShotBFlag = 0;

	// ウィンドウサイズを取得する
	GetWindowSize(&WindowSizeX, &WindowSizeY);

	// 移動ルーチン
	while (TRUE)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 変数を表示する
#ifdef _DEBUG
		//DrawFormatString(0, WindowSizeY - 15, GetColor(255, 255, 255), "%d", SikakuH);
		DrawFormatString(0,  0, GetColor(255, 255, 255), "ShotFlag[0] : %d", ShotFlag[0]);
		DrawFormatString(0, 15, GetColor(255, 255, 255), "ShotFlag[1] : %d", ShotFlag[1]);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "ShotX[0] : %d", ShotX[0]);
		DrawFormatString(0, 45, GetColor(255, 255, 255), "ShotY[0] : %d", ShotY[0]);
		DrawFormatString(0, 60, GetColor(255, 255, 255), "ShotBFlag : %d", ShotBFlag);
		DrawFormatString(0, 75, GetColor(255, 255, 255), "SikakuDamageFlag : %d", SikakuDamageFlag);
		DrawFormatString(0, 90, GetColor(255, 255, 255), "SikakuDamageCounter : %d", SikakuDamageCounter);
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

			// ボタン１を押した場合は処理を分岐
			if (key & PAD_INPUT_1)
			{
				// 前フレームでショットボタンを押したかが保存されている変数が０だったら弾を発射
				if (ShotBFlag == 0)
				{
					// 弾iが画面上にでていない場合はその弾を画面に出す
					for (int i = 0; i < SHOT; i++)
					{
						if (ShotFlag[i] == 0)
						{
							int Bw, Bh, Sw, Sh;

							// ボール君と弾の画像のサイズを得る
							GetGraphSize(BallGraph, &Bw, &Bh);
							GetGraphSize(ShotGraph, &Sw, &Sh);

							// 弾の位置をセット、位置はボール君の中心にする
							ShotX[i] = (Bw - Sw) / 2 + BallX;
							ShotY[i] = (Bh - Sh) / 2 + BallY;

							// 弾は現時点を持って存在するので、存在状態を保持する変数に１を代入する
							ShotFlag[i] = 1;

							// 一つ弾を出したので弾を出すループから抜けます
							break;
						}
					}
				}

				// 前フレームでショットボタンを押されていたかを保存する変数に１(おされていた)を代入
				ShotBFlag = 1;
			}
			else
			{
				// ショットボタンが押されていなかった場合は
				// 前フレームでショットボタンが押されていたかを保存する変数に０(おされていない)を代入
				ShotBFlag = 0;
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

		// 弾の数だけ弾を動かす処理を繰り返す
		for (int i = 0; i < SHOT; i++)
		{
			// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容が１(存在する)の場合のみ行う )
			if (ShotFlag[i] == 1)
			{
				// 弾を１６ドット上に移動させる
				ShotY[i] -= 16;

				// 画面外に出てしまった場合は存在状態を保持している変数に０(存在しない)を代入する
				if (ShotY[i] < -80)
				{
					ShotFlag[i] = 0;
				}

				// 画面に弾を描画する
				DrawGraph(ShotX[i], ShotY[i], ShotGraph, FALSE);
			}
		}

		// 四角君の移動ルーチン
		{
			// 顔を歪めているかどうかで処理を分岐
			if (SikakuDamageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(SikakuX, SikakuY, SikakuDamageGraph, FALSE);

				// 顔を歪めている時間を測るカウンターに１を加算する
				SikakuDamageCounter++;

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (SikakuDamageCounter == 30)
				{
					// 『歪んでいない』を表す０を代入
					SikakuDamageFlag = 0;
				}
			}
			else
			{
				// 歪んでいない場合は今まで通りの処理

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
		}

		// 弾と敵の当たり判定、弾の数だけ繰り返す
		for (int i = 0; i < SHOT; i++)
		{
			// 弾iが存在している場合のみ次の処理に映る
			if (ShotFlag[i] == 1)
			{
				// 四角君との当たり判定
				if (((ShotX[i] > SikakuX && ShotX[i] < SikakuX + SikakuW) ||
					(SikakuX > ShotX[i] && SikakuX < ShotX[i] + ShotW)) &&
					((ShotY[i] > SikakuY && ShotY[i] < SikakuY + SikakuH) ||
						(SikakuY > ShotY[i] && SikakuY < ShotY[i] + ShotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					ShotFlag[i] = 0;

					// 四角君の顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					SikakuDamageFlag = 1;

					// 四角君の顔を歪めている時間を測るカウンタ変数に０を代入
					SikakuDamageCounter = 0;
				}
			}
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