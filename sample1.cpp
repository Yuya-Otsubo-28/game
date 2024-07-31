#define GL_SILENCE_DEPRECATION
//#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

// グローバル変数
static bool KeyUpON    = false;	// 矢印キーの状態フラグ
static bool KeyDownON  = false;	// 矢印キーの状態フラグ
static bool KeyLeftON  = false;	// 矢印キーの状態フラグ
static bool KeyRightON = false;	// 矢印キーの状態フラグ

static int MouseX = 0;	// マウスX座標
static int MouseY = 0;	// マウスY座標
static float SpinX = 0;	// X軸周りの回転量
static float SpinY = 0;	// Y軸周りの回転量

static float MoveX = 0;	// X軸方向の移動量
static float MoveZ = 0;	// Y軸方向の移動量
static float Scale = 1.0; // 拡大量

// 自機の弾の情報を格納する構造体
#define MAX_SHOT  10 // 同時に撃てる弾の最大数
typedef struct _MyShot{
	int isAlive; // 自機の弾が生きてるか？
	float x;  // 弾のx座標
	float z;  // 弾のz座標
	float vx; // 弾のx軸方向速度
	float vz; // 弾のz軸方向速度
} MyShot;
// 自機の情報を格納する構造体
typedef struct _MyShip{
	int isAlive;   // 自機が生きてるか？
	float x;  // 自機のx座標
	float z;  // 自機のz座標
	MyShot myShot[MAX_SHOT];// 弾を管理する構造体配列
} MyShip;
MyShip myShip; // 自機の構造体を作成

// 敵の情報を格納する構造体
#define MAX_ENEMY  10 // 同時に出現する敵の最大数
typedef struct _Enemy{
	int isAlive;   // 敵が生きてるか？
	float x;  // 敵のx座標
	float z;  // 敵のz座標
	float vx; // 敵のx軸方向速度
	float vz; // 敵のz軸方向速度
} Enemy;
Enemy enemy[MAX_ENEMY]; // 敵の構造体配列を作成

// キーボード入力(スペースキーで弾を発射)
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

void timer(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
	// 矢印キーを押し続けている間は自機を移動
	if(KeyUpON   == 1) myShip.z -= 0.1;
	if(KeyDownON == 1) myShip.z += 0.1;
	if(KeyLeftON == 1) myShip.x -= 0.1;
	if(KeyRightON== 1) myShip.x += 0.1;

	// 速度を足して弾を移動させる
	for (int i=0;i<MAX_SHOT; i++){
		if(myShip.myShot[i].isAlive == 1){
			myShip.myShot[i].z += myShip.myShot[i].vx; //x軸方向の速度を加算
			myShip.myShot[i].z += myShip.myShot[i].vz; //z軸方向の速度を加算
		}
		if(myShip.myShot[i].z < -100) //画面外(領域外)判定
			myShip.myShot[i].isAlive = 0; //画面外(領域外)に出たら弾を消す
	}

	// 速度を足して敵を移動させる
	int NofEnemys = 0; // 出現している敵の数を数える変数
	for(int i=0;i<MAX_ENEMY; i++){
		if(enemy[i].isAlive == 1){ // i番目の敵は生きているか？
			 // 生きていたら敵の位置を乱数（ノイズ）で移動
			enemy[i].x += enemy[i].vx +((float)rand()/RAND_MAX - 0.5); //x軸方向の速度(と乱数)を加算
			enemy[i].z += enemy[i].vz + ((float)rand()/RAND_MAX); //z軸方向の速度(と乱数)を加算
			NofEnemys++; // 出現している敵の数を数える
		}
		if(enemy[i].z > 10 || enemy[i].x < -10 || enemy[i].x > 10) //画面外(領域外)判定
			enemy[i].isAlive = 0; //画面外(領域外)に出たら敵を消す
	}

	// 弾と敵の当たり判定
	for(int i=0;i<MAX_ENEMY;i++){
		for(int j=0;j<MAX_SHOT;j++){
			// 弾と敵の距離が一定値以内の場合に当たりと判定
			if(((enemy[i].x - myShip.myShot[j].x)*(enemy[i].x - myShip.myShot[j].x)+
				(enemy[i].z - myShip.myShot[j].z)*(enemy[i].z - myShip.myShot[j].z)) < 2.0 ){
				enemy[i].isAlive = 0;           //敵を消す
				myShip.myShot[j].isAlive = 0;   //弾を消す(ここで弾を消さないと貫通弾になる)
			}
		}
	}

	// 敵の出現
	// 現存する敵がMAX_ENEMYよりも少なかったら1つ新たに出現させる
	if (NofEnemys<MAX_ENEMY){
		for(int i=0;i<MAX_ENEMY; i++){
			// isAliveが0の構造体を見つけてisAliveを1にする
			// 出現位置はランダムに変化させる
			if(enemy[i].isAlive == 0){
				enemy[i].x = ((float)rand()/RAND_MAX - 0.5) * 10;
				enemy[i].z = -100 + ((float)rand()/RAND_MAX - 0.5) * 100;
				enemy[i].isAlive = 1;
				break;
			}
		}
	}

	// 敵と自機の当たり判定
	for(int i=0;i<MAX_ENEMY; i++){
		// 敵と自機の距離が一定値以内の場合に当たりと判定
		if(((enemy[i].x - myShip.x)*(enemy[i].x - myShip.x)
			+(enemy[i].z - myShip.z)*(enemy[i].z - myShip.z)) < 2.0 ){
			myShip.isAlive = 0; // 自機を消す
		}
	}

	glutPostRedisplay();          // 画面の再描画
	glutTimerFunc(20, timer, 20); // 次回のタイマー関数の呼び出しをセット
}

void reshape(int w, int h)    // ウインドウサイズ変更時に呼び出される関数（Reshapeコールバック関数）
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w/h, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// 自機と弾の描画
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);   // ウィンドウサイズの指定
	glutCreateWindow("window");     // 表示ウィンドウ作成
	glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
	glutDisplayFunc(display);       // Displayコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	// 自機の構造体データを初期化
	myShip.x = 0;	// 自機の初期位置(X座標)
	myShip.z = 0;	// 自機の初期位置(Z座標)
	myShip.isAlive = 1;	// 自機は生きている
	// 弾の構造体データを初期化(弾はZ軸方向に毎フレーム-1.0移動する)
	for(int i=0; i<MAX_SHOT; i++){
		myShip.myShot[i].isAlive = 0; // 弾は発射されていない
		myShip.myShot[i].x = 0;		// 弾の初期位置(X座標)
		myShip.myShot[i].z = 0;		// 弾の初期位置(X座標)
		myShip.myShot[i].vx = 0;	// 弾の基本速度(X座標)
		myShip.myShot[i].vz = -1.0; // 弾の基本速度(X座標)

	}
	// 敵の構造体データを初期化(敵はZ軸方向に毎フレーム0.1移動する)
	for(int i=0; i<MAX_ENEMY; i++){
		enemy[i].isAlive = 0; // 敵は出現していない
		enemy[i].x = 0;		// 敵の初期位置(X座標)
		enemy[i].z = -100;	// 敵の初期位置(Z座標)
		enemy[i].vx = 0;	// 敵の基本速度(X座標)
		enemy[i].vz = 0.1;	// 敵の基本速度(Z座標)
	}

	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)
	glutMainLoop(); // メインループへ

	return 0;
}
