////////////////////////////////////////////////////////////////////
// 追いかけっこプログラム
////////////////////////////////////////////////////////////////////

// インクルード -------------------------------------
#define _USE_MATH_DEFINES
#define HEIGHT 480		//ウィンドウの縦の長さ
#define WIDTH 640		//ウィンドウの幅の長さ
#define SHIPLENGTH 64	//船の縦横の長さ
#define TARGETX	600		//的の既定の位置
#define TARGETY 64		//的の縦の長さ
#define ARROWLENGTH 16	//矢の横の長さ
#define ARROWWIDTH 8	//矢の縦の幅

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "gs.h"


// ゲームモード用
enum
{
	Title = 0,	//タイトル画面
	Setting = 1,	//船の設定
	Update = 2,	//矢の更新
	Result = 3,	//結果
	Over = 4,	//オーバー画面
};
enum
{
	YSET		= 0,
	ANGLESET	= 1,
	POWERSET	= 2,
};
enum
{
	OFF = 0,
	ON	= 1,
};
// 関数の宣言 -----------------------------
void GameInit(void);	// ゲーム初期化
void GameLoop(void);	// ゲームループ
void GameTerm(void);	// ゲーム終了
void Playerset(void);	//プレイヤーの更新
void Arrowupdate(void);	//飛ばした矢の更新
void Setreset(void);	//船、的のリセット
bool Numbercheck(int count[], int i, int number);	//的が同じのがあるかチェック
void Pointdisplay();	//ポイント表示
void TitleMain();		//タイトル
void OverMain();		//オーバー

// 絵用の変数
LPDIRECT3DTEXTURE9	sBack;		// 背景
LPDIRECT3DTEXTURE9	sBack2;		// 背景2
LPDIRECT3DTEXTURE9	sBack3;		// 背景3
LPDIRECT3DTEXTURE9	sTitle1;		// タイトル1
LPDIRECT3DTEXTURE9	sTitle2;		// タイトル2
LPDIRECT3DTEXTURE9	sOver;		// ゲームオーバー
LPDIRECT3DTEXTURE9	sShip;		// シップ
LPDIRECT3DTEXTURE9	sTarget;	// 的
LPDIRECT3DTEXTURE9	sArrow;		// 矢
LPDIRECT3DTEXTURE9	sGrp1;		// グラフ1
LPDIRECT3DTEXTURE9	sGrp2;		// グラフ2

// グローバル変数
int status;						//ゲームの状態
int shipsetting;					//船の設定状態
int shipy;						//船の位置
double charge;					//矢の基本パワー
float powerx, powery;			//矢の更新のＸとＹ
float arrowx, arrowy;			//矢の位置
float targety[3];				//的の配列
int gHitKey;					//キーの状態
double shipangle;				//船の角度	
double arrowangle;				//矢の角度
int point;						//得点
int score;
int displaytime;				//表示時間
int bullet;						//矢の残り


// ----------------------------------------------------------
// ゲーム　前処理 
// プログラム開始時に１回のみシステムから呼ばれる 
// ----------------------------------------------------------
void GameInit(void)
{
	//絵の読み込み
	gsLoadTexture(&sBack,"bmp//bg.bmp", 0xff000000);		//手前の絵
	gsLoadTexture(&sBack2, "bmp//bg2.bmp", 0xff000000);		//真ん中の絵
	gsLoadTexture(&sBack3, "bmp//bg3.bmp", 0xff000000);		//一番後ろの絵
	gsLoadTexture(&sTitle1, "bmp//title1.bmp", 0xff000000);	//タイトル1の絵
	gsLoadTexture(&sTitle2, "bmp//title2.bmp", 0xff000000);	//タイトル2の絵
	gsLoadTexture(&sOver, "BMP/over.bmp", 0xff000000);		// ゲームオーバー
	gsLoadTexture(&sShip, "bmp//ship.bmp", 0xff000000);		//プレイヤーの船
	gsLoadTexture(&sTarget, "bmp//target.bmp", 0xffffffff);	//　的	
	gsLoadTexture(&sArrow, "bmp/arrow.bmp", 0xff000000);	//	矢
	gsLoadTexture(&sGrp1, "bmp/grp1.bmp", 0xff000000);		// メーター１（赤）
	gsLoadTexture(&sGrp2, "bmp/grp2.bmp", 0xff000000);		// メーター２（緑）

	//変数の初期化
	status = 0;
	shipsetting = 0;
	shipy = 300;
	shipangle = 0;
	gHitKey = 0;
	powerx = 1;
	charge = 0.2f;
	arrowx = 0;
	arrowy = 0;
	point = 10;
	score = 0;
	bullet = 3;
	int count[3];
	int t = (int)time(0);
	srand(t);
	//的の初期化3個
	for (int i = 0; i < 3;)
	{
		int number = rand() % 7;
		//今までに出てたら最初に戻る
		if (true == Numbercheck(count, i, number)) continue;
		count[i] = number;
		targety[i] = (float)64 * number;
		i++;
	}

}

// ----------------------------------------------------------
// ゲーム　ループ処理
// 毎ループごとにシステムから呼ばれる
// ----------------------------------------------------------
void GameLoop(void)
{

	// 背景を描く
	gsDraw2D(sBack3, 0, 0);
	gsDraw2D(sBack2, 0, 0);
	gsDraw2D(sBack, 0, 0);


	//モード
	switch (status)
	{
		case Title:
			TitleMain();
			break;
		case Setting:
			Playerset();
			break;
		case Update:
			Arrowupdate();
			break;
		case Result:
			Pointdisplay();
			break;
		case Over:
			OverMain();
			break;
	}

	// キーを離したかのチェック
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0)
	{
		gHitKey = 0;
	}
	if (status == 1 || status == 2 || status == 3)
	{
		//船の表示
		gsDraw2DPartEx(sShip, 100, shipy, SHIPLENGTH, SHIPLENGTH, 0, 0, 64, 64, (float)shipangle, 0xffffffff);

		//的の表示
		for (int i = 0; i < 3; i++)
		{
			gsDraw2D(sTarget, TARGETX, (int)targety[i]);
		}
		//	矢の表示
		if (status == Update || status == Result)
		{
			arrowangle = atan2(powery, powerx) * -1;
			gsDraw2DPartEx(sArrow, (int)arrowx, (int)arrowy, 16, 8, 0, 0, 16, 8, (float)arrowangle, 0xffffffff);
		}


		// タイトルを描く
		gsDrawText(10, 10, "的当て");

		//// 色々なステータスを描く
		//gsDrawNum(300, 10, shipy);
		//gsDrawNum(300, 30, shipangle * 180 / M_PI);	//発射角度(度)
		//gsDrawNum(300, 50, arrowangle);
		//gsDrawNum(300, 70, powery);
		//gsDrawNum(300, 90, score);

		//矢のパワーのメーター表示
		gsDraw2DEx(sGrp1, 100, 20, 10 * 10, 16); 	// 赤
		gsDraw2DEx(sGrp2, 100, 20, powerx * 10, 16, 0xff007700);	// 緑

		int ypoint = 50;

		//矢の残りの表示
		for (int i = 0; i < bullet; i++)
		{
			gsDraw2D(sArrow, 30, ypoint);
			ypoint += 20;
		}
	}
}

// ----------------------------------------------------------
// ゲーム　後処理
// プログラム終了時に１回のみシステムから呼ばれる
// ----------------------------------------------------------
void GameTerm(void)
{
	// 絵の解放
	gsReleaseTexture(sBack);
	gsReleaseTexture(sBack2);
	gsReleaseTexture(sBack3);
	gsReleaseTexture(sTitle1);
	gsReleaseTexture(sShip);
	gsReleaseTexture(sArrow);
	gsReleaseTexture(sGrp1);
	gsReleaseTexture(sGrp2);


}
// ----------------------------------------------------------
// プレイヤーの更新
// ----------------------------------------------------------
void Playerset(void)
{
	//船の高さを設定(y座標）
	switch (shipsetting)
	{
	case YSET:
		//上へ上昇
		if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
		{
			shipy -= 5;
		}
		//下へ下降
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
		{
			shipy += 5;
		}
		//はみ出し防止
		if (shipy < 100 + SHIPLENGTH)
		{
			shipy = 100 + SHIPLENGTH;
		}
		if (shipy > HEIGHT)
		{
			shipy = HEIGHT;
		}
		//スペースを押して高さを決めたか？
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			shipsetting = 1;			//ANGLESET
			gHitKey = 1;				//キーを押した
		}
		break;
	//船の角度を設定(ラジアン)
	case ANGLESET:
		//1ラジアンごとに角度を変化させる
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0)
		{
			shipangle += 1 * M_PI / 180;
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0)
		{
			shipangle -= 1 * M_PI / 180;
		}
		//０度以下になるときは０度にに固定
		if (shipangle < 0)
		{
			shipangle = 0;
		}
		//キーを一度離してからスペースを押したか？
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			shipsetting = 2;			//POWERSETへ
			gHitKey = 1;				//キーを押した
		}
		break;
	//矢の進む力を設定（x座標）
	case POWERSET:
		//パワーをチャージしていく
		powerx += charge;
		//パワーが規定を超えたらチャージを反転
		if (powerx < 1 || powerx>10)
		{
			charge *= -1;
		}
		//キーを一度話してからスペースを押されたか？
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			//矢の発射位置を代入
			arrowx = 100 + 32;
			arrowy = shipy - 10;
			powery = powerx * tan(shipangle) * -1;	//右上方向に飛んで行くのでマイナスをかけて置く
			gHitKey = 1;							//キーを押した
			status = 2;								//updateへ
		}
		break;
	}
}

// ----------------------------------------------------------
// 飛ばした矢の更新
// ----------------------------------------------------------
void Arrowupdate()
{
	//矢の更新
	arrowx += powerx;
	powery += 0.08f;	//重力を足していく
	arrowy += powery;

	//ウィンドウ外に飛んで行ったらリセットする
	if (WIDTH < arrowx || HEIGHT < arrowy)
	{
		point = 0;
		bullet--;				//矢の残りを減らす
		status = 3;				//結果へ
	}

	//的の数だけループ
	for (int i = 0; i < 3; i++)
	{
		//的に刺さってるか？
		if (TARGETX + 12 < arrowx + ARROWLENGTH&&
			TARGETX + 20 > arrowx + ARROWLENGTH&&
			targety[i]<arrowy + (ARROWWIDTH / 2) &&
			targety[i] + TARGETY>arrowy + (ARROWWIDTH / 2))
		{
			point = 10;		//得点
			status = 3;			//結果へ
			break;
		}
	}
}
// ----------------------------------------------------------
// 船、的のリセット
// ----------------------------------------------------------
void Setreset()
{
	//数値を初期化
	status = 1;
	shipsetting = 0;
	arrowx = 0;
	arrowy = 0;

	int count[3];
	//同じ数字が出てたら最初に戻る
	for (int i = 0; i < 3;)
	{
		int number = rand() % 7;
		//同じ数字があったか？
 		if (true == Numbercheck(count, i, number)) continue;
		//履歴に入れておく
		count[i] = number;
		//的の位置を代入
		targety[i] = (float)64 * number;
		i++;
	}
}
// ----------------------------------------------------------
// 的が同じのがあるかチェック
// ----------------------------------------------------------
bool Numbercheck(int count[], int i, int number)
{
	for (int j = 0; j < i; j++)
	{
		if (count[j] == number)
		{
			return true;
		}
	}
	return false;
}
// ----------------------------------------------------------
// ポイント表示
// ----------------------------------------------------------
void Pointdisplay()
{
	if (displaytime < 180)
	{
		gsDrawNum(WIDTH/2, HEIGHT/2, point);
		displaytime++;
	}
	else
	{
		displaytime = 0;
		score += point;

		//矢の残りがあるか？
		if (0 < bullet)
		{
			Setreset();
		}
		//ない場合
		else
		{
			status = 4;		//ゲームオーバーへ
		}
	}
}
// ----------------------------------------------------------
// タイトルの表示・更新
// ----------------------------------------------------------
void TitleMain()
{
	//タイトル1
	gsDraw2D(sTitle1, 90, 50);
	//
	gsDraw2D(sTitle2, 90, 150);
	
	//キーを一度話してからスペースを押されたか？
	if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		gHitKey = 1;	//キーを押した
		status = 1;		//プレイヤー更新へ
	}
}
// ------------------------------------------------ 
// ゲームオーバーの表示・更新
// ------------------------------------------------ 
void OverMain()
{
	//ゲームオーバーの表示
	gsDraw2D(sOver, 90, 50);
	//最終スコアを表示
	gsDrawNum(WIDTH / 2, HEIGHT / 2, point);

	//キーを一度話してからスペースを押されたか？
	if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		GameInit();		//すべてを初期化
		gHitKey = 1;	//キーを押した
		status = 0;		//タイトルへ
	}
}