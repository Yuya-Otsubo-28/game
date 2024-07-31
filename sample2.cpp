#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

//1.ライフの減りに応じて効果を付ける
//2.自機の見た目を変更する
//3.BGM,効果音を追加
//4.障害物を配置する（衝突判定）
//5.アイテムを配置する（衝突判定）


// グローバル変数
static bool KeyUpON    = false;	// 矢印キーの状態フラグ
static bool KeyDownON  = false;	// 矢印キーの状態フラグ
static bool KeyLeftON  = false;	// 矢印キーの状態フラグ
static bool KeyRightON = false;	// 矢印キーの状態フラグ

static bool KeyUpON1 = false;	// 矢印キーの状態フラグ
static bool KeyDownON1 = false;	// 矢印キーの状態フラグ
static bool KeyLeftON1 = false;	// 矢印キーの状態フラグ
static bool KeyRightON1 = false;	// 矢印キーの状態フラグ

static int MouseX = 0;	// マウスX座標
static int MouseY = 0;	// マウスY座標
static float SpinX = 0;	// X軸周りの回転量
static float SpinY = 0;	// Y軸周りの回転量

static float MoveX = 0;	// X軸方向の移動量
static float MoveZ = 0;	// Y軸方向の移動量
static float Scale = 1.0; // 拡大量

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PI 3.1415926535
#define FIELD_SIZE 50.0f
#define SHIP_NUM 2
#define DEFAULT_LIFE 1

// 自機の弾の情報を格納する構造体
#define MAX_SHOT  3 // 同時に撃てる弾の最大数
typedef struct _MyShot{
	int isAlive; // 自機の弾が生きてるか？
	float x;  // 弾のx座標
	float z;  // 弾のz座標
	float vx; // 弾のx軸方向速度
	float vz; // 弾のz軸方向速度
} MyShot;

// 自機の情報を格納する構造体
typedef struct _MyShip{
	int Life;   // 自機が生きてるか？
	float x;  // 自機のx座標
	float z;  // 自機のz座標
	float theta;  // 自機の向き
	MyShot myShot[MAX_SHOT];// 弾を管理する構造体配列
} MyShip;

MyShip ship[SHIP_NUM]; // 自機の構造体を作成

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

void timer(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
	// 矢印キーを押し続けている間は自機を移動
	if (KeyUpON == 1) { 
		ship[0].z += 0.1*cos(ship[0].theta /180.0*PI);
		ship[0].x += 0.1*sin(ship[0].theta / 180.0*PI);
	}
	if (KeyDownON == 1) {
		ship[0].z -= 0.1*cos(ship[0].theta / 180.0*PI);
		ship[0].x -= 0.1*sin(ship[0].theta / 180.0*PI);
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

//カメラの視点変更
void setGluLookAt(int shipid =1) {
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
	setGluLookAt();
}


void DrawSurface(void) { // 地面を描画
	glPushMatrix();
	GLfloat mat0ambi[] = { 0.8,  0.6, 0.6, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat0ambi); //環境光の反射率を設定
	GLfloat mat0diff[] = { 0.8,  0.6, 0.6, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat0diff); //拡散光の反射率を設定
	GLfloat mat0spec[] = { 0.992157,  0.941176, 0.807843, 1.0 };//真鍮
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定

	glNormal3f(0.0, 1.0, 0.0); //法線方向の設定

	glTranslatef(0.0, -0.5, 0.0);
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

// 自機と弾の描画2
void DrawMyShip2(void)
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

	//自機の描画
	glTranslatef(ship[1].x, 0.0, ship[1].z); //X軸方向にMoveX，Y軸方向にMoveYだけ移動
	glRotatef(ship[1].theta - 90, 0.0, 1.0, 0.0);  //Y軸周りに90度回転
	glutSolidTeapot(1.0);            //自機はティーポット(笑)
	glPopMatrix();

	//自機の弾の描画
	for (int i = 0; i<MAX_SHOT; i++) {
		if (ship[1].myShot[i].isAlive) {
			glPushMatrix();
			glTranslatef(ship[1].myShot[i].x, 0.0, ship[1].myShot[i].z); //現在の弾の位置
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
void display1(int x, int y, int w, int h) {

	glViewport(x, y, w, h); //ビューポート変換
	setGluLookAt();

	//地面
	DrawSurface();

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
	if (ship[1].Life < 1) {
		// 文字列の描画
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f(-1.0f + ship[0].x, 5.0f, 0.0f + ship[0].z);
		char *str = "You Win";
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			++str;
		}
		glPopMatrix();
	}
	else {
		DrawMyShip2();	  //DrawEnemy();  // 敵の描画
	}
	// 座標軸の描画
	DrawAxis();
	
}

void display2(int x, int y, int w, int h) {

	glViewport(x, y, w, h); //ビューポート変換
	setGluLookAt(2);

	//地面
	DrawSurface();

	// 自機が死んでいたらGame Overを表示する
	if (ship[1].Life < 1) {
		// 文字列の描画
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f( ship[1].x, 5.0f, 0.0f + ship[1].z);
		char *str = "You Lose";
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			++str;
		}
		glPopMatrix();
	}
	else {
		// 自機が生きていたら自機と敵を描画する
		DrawMyShip2(); // 自機の描画
		
	}
	if (ship[0].Life < 1) {
		// 文字列の描画
		glPushMatrix();
		GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matString);
		glRasterPos3f(-1.0f + ship[0].x, 5.0f, 0.0f + ship[0].z);
		char *str = "You Win";
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
			++str;
		}
		glPopMatrix();
	}
	else {
		DrawMyShip();
	}

	// 座標軸の描画
	DrawAxis();
}
void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
	glEnable(GL_DEPTH_TEST); // 隠面消去を有効

	display1(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //プレイヤ１の画面描画
	display2(WINDOW_WIDTH+10, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  //プレイヤ2の画面描画

	glutSwapBuffers(); // 描画実行
}

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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);          // GLUT初期化

	// Windowの設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH*2+10, WINDOW_HEIGHT);   // ウィンドウサイズの指定
	glutCreateWindow("window");     // 表示ウィンドウ作成
	glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
	glutDisplayFunc(display);       // Displayコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutKeyboardUpFunc(keyboardUp);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	//ランドの初期化
	srand(time(NULL));
	

	// 自機の構造体データを初期化
	for (int j = 0; j < SHIP_NUM; j++) {
		ship[j].x = rand()% 50;	// 自機の初期位置(X座標)
		ship[j].z = rand() % 50;	// 自機の初期位置(Z座標)
		ship[j].theta = rand() % 360;	// 自機の初期角度
		ship[j].Life = DEFAULT_LIFE;	// 自機は生きている

	// 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
		for (int i = 0; i < MAX_SHOT; i++) {
			ship[j].myShot[i].isAlive = 0; // 弾は発射されていない
			ship[j].myShot[i].x = ship[j].x;		// 弾の初期位置(X座標)
			ship[j].myShot[i].z = ship[j].z;		// 弾の初期位置(X座標)
			ship[j].myShot[i].vx = 0;	// 弾の基本速度(X座標)
			ship[j].myShot[i].vz = -1.0; // 弾の基本速度(X座標)

		}
	}
	
	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)
	glutMainLoop(); // メインループへ

	return 0;
}
