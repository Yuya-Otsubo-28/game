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