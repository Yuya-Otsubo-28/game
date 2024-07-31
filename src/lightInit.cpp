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

void lightInit(void)        // 光源の初期設定(まとめて関数にしているだけ)
{
	glEnable(GL_LIGHTING);  //光源の設定を有効にする
	glEnable(GL_LIGHT0);    //0番目の光源を有効にする(8個まで設定可能)
	glEnable(GL_NORMALIZE); //法線ベクトルの自動正規化を有効

	GLfloat light0pos[] = { 0.0, 10.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos); //光源0の位置を設定

	GLfloat light0ambi[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0ambi); //光源0の環境光の色を設定
	GLfloat light0diff[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0diff); //光源0の拡散光の色を設定
	GLfloat light0spec[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0spec); //光源0の鏡面光の色を設定

	glShadeModel(GL_SMOOTH); //スムーズシェーディングに設定
}