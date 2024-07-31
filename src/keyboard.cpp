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

// キーボード入力(スペースキーで弾を発射)
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case ' ':
	  //printf("(%3d,%3d)でスペースが押されました\n", x, y);
	  for (int i = 0; i < MAX_SHOT; i++) {
			  if (ship[0].myShot[i].isAlive == 0) {
				  ship[0].myShot[i].isAlive = 1;
				  ship[0].myShot[i].x = ship[0].x;
				  ship[0].myShot[i].z = ship[0].z;
				  ship[0].myShot[i].vx = sin(ship[0].theta / 180.0 * PI);
				  ship[0].myShot[i].vz = cos(ship[0].theta / 180.0 * PI);
				  break;
			  }
		  }
	  
	  break;
  case 'l':
	  //printf("(%3d,%3d)でスペースが押されました\n", x, y);
	  for (int i = 0; i < MAX_SHOT; i++) {
		  if (ship[1].myShot[i].isAlive == 0) {
			  ship[1].myShot[i].isAlive = 1;
			  ship[1].myShot[i].x = ship[1].x;
			  ship[1].myShot[i].z = ship[1].z;
			  ship[1].myShot[i].vx = sin(ship[1].theta / 180.0 * PI);
			  ship[1].myShot[i].vz = cos(ship[1].theta / 180.0 * PI);
			  break;
		  }
	  }

	  break;

  case 'w':
	  //printf("(%3d,%3d)で[↑]が押されました\n", x, y);
	  KeyUpON = true;
	  break;
  case 's':
	  //printf("(%3d,%3d)で[↓]が押されました\n", x, y);
	  KeyDownON = true;
	  break;
  case 'a':
	  //printf("(%3d,%3d)で[←]が押されました\n", x, y);
	  KeyLeftON = true;
	  break;
  case 'd':
	  //printf("(%3d,%3d)で[→]が押されました\n", x, y);
	  KeyRightON = true;
	  break;

  case 'r':
	  for (int j = 0; j<SHIP_NUM; j++) {
		  // 自機の構造体データを初期化
		  ship[j].x = rand()%50;	// 自機の初期位置(X座標)
		  ship[j].z = rand() % 50;	// 自機の初期位置(Z座標)
		  ship[j].theta = rand() % 360;	// 自機の初期位置(Z座標)
		  ship[j].Life = DEFAULT_LIFE;	// 自機は生きている

		  // 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
		  for (int i = 0; i < MAX_SHOT; i++) {
			  ship[j].myShot[i].isAlive = 0; // 弾は発射されていない
			  ship[j].myShot[i].x = 0;		// 弾の初期位置(X座標)
			  ship[j].myShot[i].z = 0;		// 弾の初期位置(X座標)
			  ship[j].myShot[i].vx = 0;	// 弾の基本速度(X座標)
			  ship[j].myShot[i].vz = -1.0; // 弾の基本速度(X座標)
		  }
	  }
	  break;
  }
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		//printf("(%3d,%3d)で[↑]が離されました\n", x, y);
		KeyUpON = false;
		break;
	case 's':
		//printf("(%3d,%3d)で[↓]が離されました\n", x, y);
		KeyDownON = false;
		break;
	case 'a':
		//printf("(%3d,%3d)で[←]が離されました\n", x, y);
		KeyLeftON = false;
		break;
	case 'd':
		//printf("(%3d,%3d)で[→]が離されました\n", x, y);
		KeyRightON = false;
		break;
	}
}