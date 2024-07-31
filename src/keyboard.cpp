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

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case ' ':
	  //printf("(%3d,%3d)でスペースが押されました\n", x, y);
	  for (int i=0;i<MAX_SHOT; i++){
		if(myShip.myShot[i].isAlive == 0){
			myShip.myShot[i].isAlive = 1;
			myShip.myShot[i].x = myShip.x;
			myShip.myShot[i].z = myShip.z;
			break;
		}
	  }
	  break;
  }
}
