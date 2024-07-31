#include "game.h"

// extern bool KeyUpON;
// extern bool KeyDownON;
// extern bool KeyLeftON;
// extern bool KeyRightON;
// extern int MouseX;
// extern int MouseY;
// extern float SpinX;
// extern float SpinY;
// extern float MoveX;
// extern float MoveZ;
// extern float Scale;
// extern MyShip myShip;
// extern Enemy enemy[MAX_ENEMY];


// グローバル変数

bool KeyUpON    = false;	// 矢印キーの状態フラグ
bool KeyDownON  = false;	// 矢印キーの状態フラグ
bool KeyLeftON  = false;	// 矢印キーの状態フラグ
bool KeyRightON = false;	// 矢印キーの状態フラグ

int MouseX = 0;	// マウスX座標
int MouseY = 0;	// マウスY座標
float SpinX = 0;	// X軸周りの回転量
float SpinY = 0;	// Y軸周りの回転量

float MoveX = 0;	// X軸方向の移動量
float MoveZ = 0;	// Y軸方向の移動量
float Scale = 1.0; // 拡大量
MyShip myShip;
Enemy enemy[MAX_ENEMY];

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);          // GLUT初期化
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);   // ウィンドウサイズの指定
	glutCreateWindow("window");     // 表示ウィンドウ作成
	glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
	glutDisplayFunc(display);       // Displayコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	// 自機の構造体データを初期化
	myShip.x = 0;	// 自機の初期位置(X座標)
	myShip.z = 0;	// 自機の初期位置(Z座標)
	myShip.isAlive = 1;	// 自機は生きている
	// 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
	for(int i=0; i<MAX_SHOT; i++){
		myShip.myShot[i].isAlive = 0; // 弾は発射されていない
		myShip.myShot[i].x = 0;		// 弾の初期位置(X座標)
		myShip.myShot[i].z = 0;		// 弾の初期位置(X座標)
		myShip.myShot[i].vx = 0;	// 弾の基本速度(X座標)
		myShip.myShot[i].vz = -1.0; // 弾の基本速度(X座標)

	}
	// 敵の構造体データを初期化(敵はZ軸方向に毎フレーム0.1移動する)
	for(int i=0; i<MAX_ENEMY; i++){
		enemy[i].isAlive = 0; // 敵は出現していない
		enemy[i].x = 0;		// 敵の初期位置(X座標)
		enemy[i].z = -100;	// 敵の初期位置(Z座標)
		enemy[i].vx = 0;	// 敵の基本速度(X座標)
		enemy[i].vz = 0.1;	// 敵の基本速度(Z座標)
	}

	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)
	glutMainLoop(); // メインループへ

	return 0;
}
