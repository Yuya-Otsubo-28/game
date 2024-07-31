#include "game.h"

extern bool KeyUpON;
extern bool KeyDownON;
extern bool KeyLeftON;
extern bool KeyRightON;
extern bool KeyUpON1;
extern bool KeyDownON1;
extern bool KeyLeftON1;
extern bool KeyRightON1;
extern int MouseX;
extern int MouseY;
extern float SpinX;
extern float SpinY;
extern float MoveX;
extern float MoveZ;
extern float Scale;
extern MyShip ship[SHIP_NUM];
extern int stage[MAP_HEIGHT][MAP_WIDTH];

void timer(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
	// 矢印キーを押し続けている間は自機を移動
	if (KeyUpON == 1) { 
		ship[0].z += 0.2*cos(ship[0].theta /180.0*PI);
		ship[0].x += 0.2*sin(ship[0].theta / 180.0*PI);
	}
	if (KeyDownON == 1) {
		ship[0].z -= 0.2*cos(ship[0].theta / 180.0*PI);
		ship[0].x -= 0.2*sin(ship[0].theta / 180.0*PI);
	}
	if (KeyLeftON == 1) {
		ship[0].theta += 1;
	}
	if (KeyRightON == 1) {
		ship[0].theta -= 1;
	}

	// 矢印キーを押し続けている間は自機を移動
	if (KeyUpON1 == 1) {
		ship[1].z += 0.1*cos(ship[1].theta / 180.0*PI);
		ship[1].x += 0.1*sin(ship[1].theta / 180.0*PI);
	}
	if (KeyDownON1 == 1) {
		ship[1].z -= 0.1*cos(ship[1].theta / 180.0*PI);
		ship[1].x -= 0.1*sin(ship[1].theta / 180.0*PI);
	}
	if (KeyLeftON1 == 1) {
		ship[1].theta += 1;
	}
	if (KeyRightON1 == 1) {
		ship[1].theta -= 1;
	}

	// 速度を足して弾を移動させる
	for (int j = 0; j < SHIP_NUM; j++) {
		for (int i = 0; i < MAX_SHOT; i++) {
			if (ship[j].myShot[i].isAlive == 1) {
				ship[j].myShot[i].x += ship[j].myShot[i].vx; //x軸方向の速度を加算
				ship[j].myShot[i].z += ship[j].myShot[i].vz; //z軸方向の速度を加算
			}
			if (abs((int)ship[j].myShot[i].x) > FIELD_SIZE * 2) //画面外(領域外)判定
				ship[j].myShot[i].isAlive = 0; //画面外(領域外)に出たら弾を消す
			if (abs((int)ship[j].myShot[i].z) > FIELD_SIZE * 2) //画面外(領域外)判定
				ship[j].myShot[i].isAlive = 0; //画面外(領域外)に出たら弾を消す
			if (stage[abs((int)(ship[j].myShot[i].z / (FIELD_SIZE/7)))][abs((int)(ship[j].myShot[i].x / (FIELD_SIZE/7)))] == 1)
				ship[j].myShot[i].isAlive = 0;
		}
	}

	//　弾と自機の当たり判定
	if (ship[0].Life > 0 && ship[1].Life > 0) {
		for (int j = 0; j < MAX_SHOT; j++) {
			// For P1 弾と敵の距離が一定値以内の場合に当たりと判定
			if (((ship[1].myShot[j].x - ship[0].x)*(ship[1].myShot[j].x - ship[0].x) +
				(ship[1].myShot[j].z - ship[0].z)*(ship[1].myShot[j].z - ship[0].z)) < 2.0 && ship[1].myShot[j].isAlive) {
				ship[0].Life--;//自機を消す
				ship[1].myShot[j].isAlive = 0;//自機を消す
			}
			// For P2 弾と敵の距離が一定値以内の場合に当たりと判定
			if (((ship[0].myShot[j].x - ship[1].x)*(ship[0].myShot[j].x - ship[1].x) +
				(ship[0].myShot[j].z - ship[1].z)*(ship[0].myShot[j].z - ship[1].z)) < 2.0 && ship[0].myShot[j].isAlive) {
				ship[1].Life--;//自機を消す
				ship[0].myShot[j].isAlive = 0;//自機を消す
			}
		}
	}

	glutPostRedisplay();          // 画面の再描画
	glutTimerFunc(20, timer, 20); // 次回のタイマー関数の呼び出しをセット
}