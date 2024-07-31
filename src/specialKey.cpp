#include "game.h"

extern bool KeyUpON;
extern bool KeyDownON;
extern bool KeyLeftON;
extern bool KeyRightON;
extern int MouseX;
extern int MouseY;
extern float SpinX;
extern float SpinY;
extern float MoveX;
extern float MoveZ;
extern float Scale;
extern MyShip myShip;
extern Enemy enemy[MAX_ENEMY];

// キーボード入力(矢印キーを押し続けている間は自機を移動)
void specialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		//printf("(%3d,%3d)で[↑]が押されました\n", x, y);
		KeyUpON = true;
		break;
	case GLUT_KEY_DOWN:
		//printf("(%3d,%3d)で[↓]が押されました\n", x, y);
		KeyDownON  = true;
		break;
	case GLUT_KEY_LEFT:
		//printf("(%3d,%3d)で[←]が押されました\n", x, y);
		KeyLeftON  = true;
		break;
	case GLUT_KEY_RIGHT:
		//printf("(%3d,%3d)で[→]が押されました\n", x, y);
		KeyRightON = true;
		break;
	}
}

void specialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		//printf("(%3d,%3d)で[↑]が離されました\n", x, y);
		KeyUpON    = false;
		break;
	case GLUT_KEY_DOWN:
		//printf("(%3d,%3d)で[↓]が離されました\n", x, y);
		KeyDownON  = false;
		break;
	case GLUT_KEY_LEFT:
		//printf("(%3d,%3d)で[←]が離されました\n", x, y);
		KeyLeftON  = false;
		break;
	case GLUT_KEY_RIGHT:
		//printf("(%3d,%3d)で[→]が離されました\n", x, y);
		KeyRightON = false;
		break;
	}
}