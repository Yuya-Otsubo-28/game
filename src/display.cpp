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
extern int ship_count;

void DrawSurface(void) { // 地面を描画
	glPushMatrix();
	GLfloat mat0ambi[] = { 0.8,  0.6, 0.6, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = { 0.8,  0.6, 0.6, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = { 0.992157,  0.941176, 0.807843, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定

	glNormal3f(0.0, 1.0, 0.0); //法線方向の設定

	glTranslatef(0.0, 0.0, 0.0);
	glScalef(FIELD_SIZE, FIELD_SIZE, FIELD_SIZE);
	glBegin(GL_QUADS);
	glVertex3f( 1.0f, 0.0f,  1.0f);
	glVertex3f(-1.0f, 0.0f,  1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f( 1.0f, 0.0f, -1.0f);
	glEnd();
	glPopMatrix();

}

// 自機と弾の描画1
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
	glTranslatef(ship[0].x, 0.0, ship[0].z); //X軸方向にMoveX，Y軸方向にMoveYだけ移動
	glRotatef(ship[0].theta-90, 0.0, 1.0, 0.0);  //Y軸周りに90度回転
	glutSolidTeapot(1.0);            //自機はティーポット(笑)
	glPopMatrix();
	
	//自機の弾の描画
	for(int i=0;i<MAX_SHOT;i++){
		if(ship[0].myShot[i].isAlive){
			glPushMatrix();
				glTranslatef(ship[0].myShot[i].x, 0.0, ship[0].myShot[i].z); //現在の弾の位置
				glutSolidSphere(0.3,10,10);            //弾は球体
			glPopMatrix();
		}
	}

}

// 敵機と弾の描画2
void DrawMyShip2(int j)
{
	glPushMatrix();
	GLfloat mat0ambi[] = { 0.329412,  0.223529, 0.027451, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = { 0.4380392,  0.368627, 0.913725, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = { 0.992157,  0.441176, 0.407843, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
	GLfloat mat0shine[] = { 27.89743616 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

	//敵機の描画
	glTranslatef(ship[j].x, 0.0, ship[j].z); //X軸方向にMoveX，Y軸方向にMoveYだけ移動
	glRotatef(ship[j].theta - 90, 0.0, 1.0, 0.0);  //Y軸周りに90度回転
	glutSolidTeapot(1.0);            //自機はティーポット(笑)
	glPopMatrix();

	//敵機の弾の描画
	for (int i = 0; i<MAX_SHOT; i++) {
		if (ship[j].myShot[i].isAlive) {
			glPushMatrix();
			glTranslatef(ship[j].myShot[i].x, 0.0, ship[j].myShot[i].z); //現在の弾の位置
			glutSolidSphere(0.3, 10, 10);            //弾は球体
			glPopMatrix();
		}
	}

}

void DrawAxis() {
	//軸の描画
	glPushMatrix();	// X軸
	GLfloat mat1diff[] = { 0.6, 0.2, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat1diff);
	glNormal3f(0.0, 1.0, 0.0); //法線方向の設定
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	glPushMatrix();	// Y軸
	GLfloat mat2diff[] = { 0.2, 0.6, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat2diff);
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

};

void	custom_itoa(int n, char *str)
{
	int	i = 0;
	char	tmp;

	tmp = n / 10;
	if (tmp)
		str[i++] = tmp + '0';
	n = n % 10;
	if (n)
		str[i] = n + '0';
}

void display1(int x, int y, int w, int h) {

	glViewport(x, y, w, h); //ビューポート変換
	setGluLookAt(1);

	//地面
	// DrawSurface();
	draw_map();

	// 自機が死んでいたらGame Overを表示する
	if (ship[0].Life < 1) {
		// 文字列の描画
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f(-1.0f + ship[0].x, 5.0f, 0.0f + ship[0].z);
		char *str = "You Lose";
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			++str;
		}
		glPopMatrix();
	}
	else {
		// 自機が生きていたら自機と敵を描画する
		DrawMyShip(); // 自機の描画
	}

	int	kill_count = 0;
	for (int i = 0; i < ship_count; i++)
	{
		if (ship[i + 1].Life < 1)
			kill_count++;
		else {
			DrawMyShip2(i + 1);	  //DrawEnemy();  // 敵の描画
		}
	}

	glPushMatrix();
		GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f(-1.0f + ship[0].x, 5.0f, 0.0f + ship[0].z);
		if (ship[0].Life > 0 && kill_count == ship_count)
		{
			char *str = "You Win";
			while (*str) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
				++str;
			}
		} else if (ship[0].Life > 0 && kill_count) {
			char *str1 = "You Killed";
			char str2[3] = {0, 0, 0};
			custom_itoa(kill_count, str2);
			char *str3 = "enemies!";
			while (*str1) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str1);
				++str1;
			}
			int j = 0;
			while (str2[j]) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str2[j]);
				++j;
			}
			while (*str3) {
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str3);
				++str3;
			}
		}
		glPopMatrix();

	// 座標軸の描画
	DrawAxis();
	
}

void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
	glEnable(GL_DEPTH_TEST); // 隠面消去を有効

	display1(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //プレイヤ１の画面描画

	glutSwapBuffers(); // 描画実行
}