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

// キーボード入力(矢印キーを押し続けている間は自機を移動)
void specialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		//printf("(%3d,%3d)で[↑]が押されました\n", x, y);
		KeyUpON1 = true;
		break;
	case GLUT_KEY_DOWN:
		//printf("(%3d,%3d)で[↓]が押されました\n", x, y);
		KeyDownON1  = true;
		break;
	case GLUT_KEY_LEFT:
		//printf("(%3d,%3d)で[←]が押されました\n", x, y);
		KeyLeftON1  = true;
		break;
	case GLUT_KEY_RIGHT:
		//printf("(%3d,%3d)で[→]が押されました\n", x, y);
		KeyRightON1 = true;
		break;
	}
}

void specialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		//printf("(%3d,%3d)で[↑]が離されました\n", x, y);
		KeyUpON1    = false;
		break;
	case GLUT_KEY_DOWN:
		//printf("(%3d,%3d)で[↓]が離されました\n", x, y);
		KeyDownON1  = false;
		break;
	case GLUT_KEY_LEFT:
		//printf("(%3d,%3d)で[←]が離されました\n", x, y);
		KeyLeftON1  = false;
		break;
	case GLUT_KEY_RIGHT:
		//printf("(%3d,%3d)で[→]が離されました\n", x, y);
		KeyRightON1 = false;
		break;
	}
}