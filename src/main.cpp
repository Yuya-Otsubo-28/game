#include "game.h"
#include "g_var.h"

void	player_init(MyShip *ship)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (stage[i][j] == 2)
			{
				ship->x = j * FIELD_SIZE/7 + 2;
				ship->z = i * FIELD_SIZE/7 + 2;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);          // GLUT初期化

	// Windowの設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);   // ウィンドウサイズの指定
	glutCreateWindow("window");     // 表示ウィンドウ作成
	glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
	glutDisplayFunc(display);       // Displayコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutKeyboardUpFunc(keyboardUp);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	//ランドの初期化
	srand(time(NULL));
	

	// 自機の構造体データを初期化
	for (int j = 0; j < SHIP_NUM; j++) {
		player_init(&ship[j]);
		ship[j].theta = rand() % 360;	// 自機の初期角度
		ship[j].Life = DEFAULT_LIFE;	// 自機は生きている

	// 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
		for (int i = 0; i < MAX_SHOT; i++) {
			ship[j].myShot[i].isAlive = 0; // 弾は発射されていない
			ship[j].myShot[i].x = ship[j].x;		// 弾の初期位置(X座標)
			ship[j].myShot[i].z = ship[j].z;		// 弾の初期位置(X座標)
			ship[j].myShot[i].vx = 0;	// 弾の基本速度(X座標)
			ship[j].myShot[i].vz = -1.0; // 弾の基本速度(X座標)

		}
	}
	
	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)
	glutMainLoop(); // メインループへ

	return 0;
}
