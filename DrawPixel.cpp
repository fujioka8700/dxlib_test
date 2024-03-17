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

	int GraphHandle;

	if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// グラフィック『test1.bmp』をメモリにロード
	GraphHandle = LoadGraph("test1.bmp");

	// 0,0 座標にメモリに読みこんだグラフィックを描画
	DrawGraph(0, 0, GraphHandle, FALSE);

	// キー入力を待つ
	WaitKey();

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}