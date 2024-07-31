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

//カメラの視点変更
void setGluLookAt(int shipid) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (shipid == 1) {
		float cosTheta = cos(ship[0].theta / 180.0*PI);
		float sinTheta = sin(ship[0].theta / 180.0*PI);
		gluLookAt(ship[0].x + (-10.0)*sinTheta, 5.0, ship[0].z + cosTheta*(-10.0),
			ship[0].x + sinTheta * (100.0), 0.0, ship[0].z + 100.0 * cosTheta,
			0.0, 1.0, 0.0);
	}
	else if (shipid == 2) {
		float cosTheta = cos(ship[1].theta / 180.0*PI);
		float sinTheta = sin(ship[1].theta / 180.0*PI);
		gluLookAt(ship[1].x + (-10.0)*sinTheta, 5.0, ship[1].z + cosTheta*(-10.0),
			ship[1].x + sinTheta * (100.0), 0.0, ship[1].z + 100.0 * cosTheta,
			0.0, 1.0, 0.0);

	}
}

void reshape(int w, int h)    // ウインドウサイズ変更時に呼び出される関数（Reshapeコールバック関数）
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w/h, 1.0, 1000.0);
	setGluLookAt(1);
}