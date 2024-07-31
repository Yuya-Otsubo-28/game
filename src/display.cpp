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

void DrawMyShip(void)
{
	glPushMatrix();
		GLfloat mat0ambi[] = {0.329412,  0.223529, 0.027451, 1.0};//真鍮
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mat0ambi); //環境光の反射率を設定
		GLfloat mat0diff[] = {0.780392,  0.568627, 0.113725, 1.0};//真鍮
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat0diff); //拡散光の反射率を設定
		GLfloat mat0spec[] = {0.992157,  0.941176, 0.807843, 1.0};//真鍮
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
		GLfloat mat0shine[] ={ 27.89743616};//真鍮
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);
		//自機の描画
		glTranslatef(myShip.x, 0.0, myShip.z); //X軸方向にMoveX，Y軸方向にMoveYだけ移動
		glRotatef(90.0, 0.0, 1.0, 0.0);  //Y軸周りに90度回転
		glutSolidTeapot(1.0);            //自機はティーポット(笑)
	glPopMatrix();
	
	//自機の弾の描画
	for(int i=0;i<MAX_SHOT;i++){
		if(myShip.myShot[i].isAlive){
			glPushMatrix();
				glTranslatef(myShip.myShot[i].x, 0.0, myShip.myShot[i].z); //現在の弾の位置
				glutSolidSphere(0.1,10,10);            //弾は球体
			glPopMatrix();
		}
	}
}

// 敵の描画
void DrawEnemy(void)
{
	GLfloat mat0ambi[] = {0.329412,  0.223529, 0.027451, 1.0};//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = {0.780392,  0.568627, 0.113725, 1.0};//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = {0.992157,  0.941176, 0.807843, 1.0};//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
	GLfloat mat0shine[] ={ 27.89743616};//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	//物体の描画
	for(int i=0;i<MAX_ENEMY;i++){
		if(enemy[i].isAlive == 1){
			glPushMatrix();
				glTranslatef(enemy[i].x, 0.0, enemy[i].z); //現在の敵の位置
				glutSolidCube(1.0);    //敵は立方体
			glPopMatrix();
		}
	}
}

void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
	glEnable(GL_DEPTH_TEST); // 隠面消去を有効

	// 自機が死んでいたらGame Overを表示する
	if (myShip.isAlive==0){
		// 文字列の描画
		glPushMatrix();
			GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,  matString);
			glRasterPos3f(-1.0f, 0.0f, 0.0f);
			char *str = "Game Over";
			while(*str){
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
				++str;
			}
		glPopMatrix();
	}else{
		// 自機が生きていたら自機と敵を描画する
		DrawMyShip(); // 自機の描画
		DrawEnemy();  // 敵の描画
	}

	// 座標軸の描画
	glPushMatrix();	// X軸
		GLfloat mat1diff[] = { 0.6, 0.2, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,  mat1diff);
		glNormal3f(0.0, 1.0, 0.0); //法線方向の設定
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(2.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();
 	glPushMatrix();	// Y軸
		GLfloat mat2diff[] = { 0.2, 0.6, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,  mat2diff);
		glNormal3f(0.0, 1.0, 0.0); //法線方向の設定
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 2.0f, 0.0f);
		glEnd();
	glPopMatrix();
 	glPushMatrix();	// Z軸
		GLfloat mat3diff[] = { 0.2, 0.2, 0.6, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat3diff);
		glNormal3f(0.0, 1.0, 0.0); //法線方向の設定
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 2.0f);
		glEnd();
 	glPopMatrix();

	glutSwapBuffers(); // 描画実行
}
