#include "DxLib.h"
#include <math.h>

#define SHOT 2
#define SIKAKULIFE 3
#define BALLLIFE 3

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

	// 味方のボール
	int BallX, BallY, BallW, BallH;
	int BallGraph;
	int BallDamageFlag, BallDamageCounter, BallDamageGraph;
	int BallLife;

	// 味方のボールが打つ弾
	int BallShotW, BallShotH;
	int BallShotX[SHOT], BallShotY[SHOT], ShotFlag[SHOT], ShotGraph;
	int ShotBFlag;

	// 敵の四角
	int SikakuX, SikakuY, SikakuW, SikakuH;
	int SikakuMuki, SikakuGraph;
	int SikakuShotW, SikakuShotH;
	int SikakuDamageFlag, SikakuDamageCounter, SikakuDamageGraph;
	int SikakuLife;

	// 敵の四角が打つ弾
	double ETamaX, ETamaY;
	double ETamaSx, ETamaSy;
	int ETamaW, ETamaH, ETamaGraph;
	int ETamaFlag;
	int ETamaCounter;

	// ウィンドウサイズ
	int WindowSizeX, WindowSizeY;

	// ボール君のグラフィックをメモリにロード＆表示座標をセット
	BallGraph = LoadGraph("Ball.png");
	BallX = 288; BallY = 400;

	// ボール君のダメージ時のグラフィックをメモリにロード
	BallDamageGraph = LoadGraph("BallDam.png");

	// ボール君の体力をセット
	BallLife = BALLLIFE;

	// ボール君が顔を歪めているかどうかの変数に『歪めていない』を表す０を代入
	BallDamageFlag = 0;

	// ボール君の顔を歪めている時間を測るカウンタ変数に０を代入
	BallDamageCounter = 0;

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

	// 四角君の体力をセット
	SikakuLife = SIKAKULIFE;

	// 敵の弾のグラフィックをロード
	ETamaGraph = LoadGraph("EShot.png");

	// 敵の弾のグラフィックのサイズを得る
	GetGraphSize(ETamaGraph, &ETamaW, &ETamaH);

	// 敵の弾が飛んでいるかどうかを保持する変数に『飛んでいない』を表す０を代入
	ETamaFlag = 0;

	// 敵が弾を撃つタイミングを取るための計測用変数に０を代入
	ETamaCounter = 0;

	// 弾のグラフィックをメモリにロード
	ShotGraph = LoadGraph("Shot.png");

	// 弾のグラフィックのサイズを得る
	GetGraphSize(ShotGraph, &SikakuShotW, &SikakuShotH);

	// 四角君のグラフィックのサイズを得る
	GetGraphSize(SikakuGraph, &SikakuW, &SikakuH);

	// 弾１・２が画面上に存在しているか保持する変数に『存在していない』を意味する０を代入しておく
	for (int i = 0; i < SHOT; i++)
	{
		ShotFlag[i] = 0;
		BallShotX[i] = 0;
		BallShotY[i] = 0;
	}

	// ショットボタンが前のフレームで押されたかどうかを保存する変数に０(押されいない)を代入
	ShotBFlag = 0;

	// ボール君と弾の画像のサイズを得る
	GetGraphSize(BallGraph, &BallW, &BallH);
	GetGraphSize(ShotGraph, &BallShotW, &BallShotH);

	// ウィンドウサイズを取得する
	GetWindowSize(&WindowSizeX, &WindowSizeY);

	// 移動ルーチン
	while (TRUE)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 変数を表示する
#ifdef _DEBUG
		DrawFormatString(0,  0, GetColor(255, 255, 255), "BallLife : %d", BallLife);
		DrawFormatString(0, 15, GetColor(255, 255, 255), "SikakuLife : %d", SikakuLife);
		//DrawFormatString(0, 30, GetColor(255, 255, 255), "BallLife : %d", BallLife);
		//DrawFormatString(0, 45, GetColor(255, 255, 255), "%d", BallDamageCounter);
		//DrawFormatString(0, 60, GetColor(255, 255, 255), "ShotBFlag : %d", ShotBFlag);
		//DrawFormatString(0, 75, GetColor(255, 255, 255), "SikakuDamageFlag : %d", SikakuDamageFlag);
		//DrawFormatString(0, 90, GetColor(255, 255, 255), "SikakuDamageCounter : %d", SikakuDamageCounter);
#endif // _DEBUG

		// ボール君の操作ルーチン
		{
			// 顔を歪めているかどうかで処理を分岐
			if (BallDamageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(BallX, BallY, BallDamageGraph, FALSE);

				if (BallLife > 0)
				{
					// 四角君のライフが0以上の時は、
					// 顔を歪めている時間を測るカウンターに１を加算する
					BallDamageCounter++;
				}

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (BallDamageCounter == 30)
				{
					// 四角君のライフが0になった時
					if (BallLife == 0)
					{
						// ボール君から弾を発射しない
						for (int i = 0; i < SHOT; i++)
						{
							ShotFlag[i] = 0;
						}
					}

					// 『歪んでいない』を表す０を代入
					BallDamageFlag = 0;

					// ボール君の顔を歪めている時間を測るカウンタ変数に０を代入
					BallDamageCounter = 0;
				}
			}
			else
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
								// 弾の位置をセット、位置はボール君の中心にする
								BallShotX[i] = (BallW - BallShotW) / 2 + BallX;
								BallShotY[i] = (BallH - BallShotH) / 2 + BallY;

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
		}

		// 弾の数だけ弾を動かす処理を繰り返す
		for (int i = 0; i < SHOT; i++)
		{
			// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容が１(存在する)の場合のみ行う )
			if (ShotFlag[i] == 1)
			{
				// 弾を１６ドット上に移動させる
				BallShotY[i] -= 16;

				// 画面外に出てしまった場合は存在状態を保持している変数に０(存在しない)を代入する
				if (BallShotY[i] < -80)
				{
					ShotFlag[i] = 0;
				}

				// 画面に弾を描画する
				DrawGraph(BallShotX[i], BallShotY[i], ShotGraph, FALSE);
			}
		}

		// 四角君の移動ルーチン
		{
			// 顔を歪めているかどうかで処理を分岐
			if (SikakuDamageFlag == 1)
			{
				// 顔を歪めている場合はダメージ時のグラフィックを描画する
				DrawGraph(SikakuX, SikakuY, SikakuDamageGraph, FALSE);

				if (SikakuLife > 0)
				{
					// 四角君のライフが0以上の時は、
					// 顔を歪めている時間を測るカウンターに１を加算する
					SikakuDamageCounter++;
				}

				// もし顔を歪め初めて ３０ フレーム経過していたら顔の歪んだ状態から
				// 元に戻してあげる
				if (SikakuDamageCounter == 30)
				{
					// 四角君のライフが0になった時
					if (SikakuLife == 0)
					{
						// 四角君から弾を発射しない
						ETamaFlag = 0;
					}

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

				// 弾を撃つタイミングを計測するためのカウンターに１を足す
				ETamaCounter++;

				// もしカウンター変数が６０だった場合は弾を撃つ処理を行う
				if (ETamaCounter == 60)
				{
					// 四角のライフが0になっておらず、
					// もし既に弾が『飛んでいない』状態だった場合のみ発射処理を行う
					if (ETamaFlag == 0 && SikakuLife > 0)
					{
						// 弾の発射位置を設定する
						ETamaX = SikakuX + SikakuW / 2 - ETamaW / 2;
						ETamaY = SikakuY + SikakuH / 2 - ETamaH / 2;

						// 弾の移動速度を設定する
						{
							double sb, sbx, sby, bx, by, sx, sy;

							sx = ETamaX + ETamaW / 2;
							sy = ETamaY + ETamaH / 2;

							bx = BallX + BallW / 2;
							by = BallY + BallH / 2;

							sbx = bx - sx;
							sby = by - sy;

							// 平方根を求めるのに標準関数の sqrt を使う、
							// これを使うには math.h をインクルードする必要がある
							sb = sqrt(sbx * sbx + sby * sby);

							// １フレーム当たり８ドット移動するようにする
							ETamaSx = sbx / sb * 8;
							ETamaSy = sby / sb * 8;
						}

						// 弾の状態を保持する変数に『飛んでいる』を示す１を代入する
						ETamaFlag = 1;
					}

					// 弾を打つタイミングを計測するための変数に０を代入
					ETamaCounter = 0;
				}
			}
		}

		// 敵の弾の状態が『飛んでいる』場合のみ弾の移動処理を行う
		if (ETamaFlag == 1)
		{
			// 弾を移動させる
			ETamaX += ETamaSx;
			ETamaY += ETamaSy;

			// ボール君との当たり判定
			if (((ETamaX > BallX && ETamaX < BallX + BallW) ||
				(BallX > ETamaX && BallX < ETamaX + BallShotW)) &&
				((ETamaY > BallY && ETamaY < BallY + BallH) ||
					(BallY > ETamaY && BallY < ETamaY + BallShotW)))
			{
				// 接触している場合は当たった弾の存在を消す
				ETamaFlag = 0;

				if (BallLife > 0)
				{
					// ボール君のライフが1以上の時、
					// 四角君の弾を受けたら、ボール君のライフを1減らす
					BallLife--;

					// ボール君の顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					BallDamageFlag = 1;
				}
			}

			// もし弾が画面からはみ出てしまった場合は弾の状態を『飛んでいない』
			// を表す０にする
			if (ETamaY > 480 || ETamaY < 0 ||
				ETamaX > 640 || ETamaX < 0) ETamaFlag = 0;

			// 画面に描画する( ETamaGraph : 敵の弾のグラフィックのハンドル )
			DrawGraph((int)ETamaX, (int)ETamaY, ETamaGraph, FALSE);
		}

		// 弾と敵の当たり判定、弾の数だけ繰り返す
		for (int i = 0; i < SHOT; i++)
		{
			// 弾iが存在している場合のみ次の処理に映る
			if (ShotFlag[i] == 1)
			{
				// 四角君との当たり判定
				if (((BallShotX[i] > SikakuX && BallShotX[i] < SikakuX + SikakuW) ||
					(SikakuX > BallShotX[i] && SikakuX < BallShotX[i] + SikakuShotW)) &&
					((BallShotY[i] > SikakuY && BallShotY[i] < SikakuY + SikakuH) ||
						(SikakuY > BallShotY[i] && SikakuY < BallShotY[i] + SikakuShotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					ShotFlag[i] = 0;

					// 四角君が顔を歪めている時は、ダメージを受けない
					if (SikakuDamageFlag == 0)
					{
						SikakuLife--;
					}

					// 四角君の顔を歪めているかどうかを保持する変数に『歪めている』を表す１を代入
					SikakuDamageFlag = 1;

					// 四角君の顔を歪めている時間を測るカウンタ変数に０を代入
					SikakuDamageCounter = 0;
				}
			}
		}

		{
			// 勝敗を表示する

			if (SikakuLife == 0)
			{
				char str[] = "あなたの勝ち";
				DrawFormatString(WindowSizeX / 2 - (GetDrawFormatStringWidth(str) / 2),
					0, GetColor(255, 255, 255), str);
			}

			if (BallLife == 0)
			{
				char str[] = "あなたの負け";
				DrawFormatString(WindowSizeX / 2 - (GetDrawFormatStringWidth(str) / 2),
					0, GetColor(255, 255, 255), str);
			}
		}

		{
			// ボール君と四角君のライフを表示する
			DrawFormatString(0, 0, GetColor(255, 255, 255), "BallLife : %d", BallLife);
			DrawFormatString(0, 15, GetColor(255, 255, 255), "SikakuLife : %d", SikakuLife);
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