#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>


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

// キャラクタの情報を格納する構造体
typedef struct _MyCharactor{
	int isAlive; // キャラクタが生きてるか？
	float x;                   // キャラクタのx座標
	int moveX_flg;             // X軸方向移動アニメーションフラグ
	int moveX_timer;           // X軸方向移動アニメーションタイマー
	float z;                   // キャラクタのz座標
	int moveZ_flg;             // Z軸方向移動アニメーションフラグ
	int moveZ_timer;           // Z軸方向移動アニメーションタイマー

	int leftArm_shoulder;      // キャラクタの左肩の角度
	int leftArm_elbow;         // キャラクタの左肘の角度
	int leftArm_animationFlg;  // 左腕アニメーションフラグ
	int leftArm_timer;         // 左腕アニメーションタイマー

	int rightArm_shoulder;     // キャラクタの右肩の角度
	int rightArm_elbow;        // キャラクタの右肘の角度
	int rightArm_animationFlg; // 左腕アニメーションフラグ
	int rightArm_timer;        // 左腕アニメーションタイマー
} MyCharactor;
// 自分のキャラクタ 
MyCharactor myCharactor;
// 相手のキャラクタ 
MyCharactor yourCharactor;

// キーボード入力(スペースキーで剣を振る)
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'g':
	  //printf("(%3d,%3d)で'o'が押されました\n", x, y);
	  myCharactor.leftArm_animationFlg = 1;
	  break;
  case ' ':
	  //printf("(%3d,%3d)でスペースが押されました\n", x, y);
	  myCharactor.rightArm_animationFlg = 1;
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

static int MouseLB_ON=0; //左マウスボタン押下フラグ
static int MouseRB_ON=0; //右マウスボタン押下フラグ

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		MouseLB_ON = 1; printf("(%3d,%3d)で左ボタンが押されました\n", x, y);
	}else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		MouseLB_ON = 0; printf("(%3d,%3d)で左ボタンを離しました\n", x, y);
	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		MouseRB_ON = 1; printf("(%3d,%3d)で右ボタンが押されました\n", x, y);
	}else if  (button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		MouseRB_ON = 0; printf("(%3d,%3d)で右ボタンを離しました\n", x, y);
	}
}

void dragMotion(int x, int y)
{
	if (MouseLB_ON == 1){
		printf("(%3d,%3d)で左ドラッグ中...\n", x, y);
		// マウスの移動量から回転量を作る
		SpinX += x - MouseX;
		SpinY += y - MouseY;
		// マウス座標の更新
		MouseX = x;
		MouseY = y;
		glutPostRedisplay();
	}
	else if (MouseRB_ON == 1){
		printf("(%3d,%3d)で右ドラッグ中...\n", x, y);
		// マウスの移動量から拡大量を作る
		Scale += (float)(y - MouseY)/100;
		// マウス座標の更新
		MouseX = x;
		MouseY = y;
		glutPostRedisplay();
	}
}

void timer(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
	//----------------------------------------------------------------------------
	// 自分のキャラクタのアニメーション
	//----------------------------------------------------------------------------
	// 矢印キーを押し続けている間は自機を移動
	if(KeyUpON   == 1) myCharactor.z -= 0.1;
	if(KeyDownON == 1) myCharactor.z += 0.1;
	if(KeyLeftON == 1) myCharactor.x -= 0.1;
	if(KeyRightON== 1) myCharactor.x += 0.1;

	// 右腕のアニメーション（関節角度を変更）
	if(myCharactor.rightArm_animationFlg == 1 || myCharactor.rightArm_timer > 0 ){
		myCharactor.rightArm_timer++;
		if(myCharactor.rightArm_timer <= 30){
		myCharactor.rightArm_elbow    = -30 + myCharactor.rightArm_timer;
		myCharactor.rightArm_shoulder = -30 + myCharactor.rightArm_timer*6;
		}
		if(myCharactor.rightArm_timer > 30 && myCharactor.rightArm_timer < 45 ){
		myCharactor.rightArm_elbow    = 0 - (myCharactor.rightArm_timer-30)*2;
		myCharactor.rightArm_shoulder = 150 - (myCharactor.rightArm_timer-30)*12;
		}
		if(myCharactor.rightArm_timer >= 45){
			myCharactor.rightArm_elbow    = -30;
			myCharactor.rightArm_shoulder = -30;
			myCharactor.rightArm_animationFlg = 0;
			myCharactor.rightArm_timer    = 0;
		}
		//printf("elbow:%5d,shoulder:%5d\n",myCharactor.rightArm_elbow,myCharactor.rightArm_shoulder);
	}
	// 左腕のアニメーション（関節角度を変更）：盾で防御と思ったけど未実装
	if(myCharactor.leftArm_animationFlg == 1 || myCharactor.leftArm_timer > 0 ){
		myCharactor.leftArm_timer++;
		if(myCharactor.leftArm_timer <= 30){
			myCharactor.leftArm_elbow    = 0 - myCharactor.leftArm_timer*2;
			myCharactor.leftArm_shoulder = 180 - myCharactor.leftArm_timer*1;
		}
		else if(myCharactor.leftArm_timer > 30 && myCharactor.leftArm_timer < 45 ){
			myCharactor.leftArm_elbow    = -60 + (myCharactor.leftArm_timer-30)*4;
			myCharactor.leftArm_shoulder = 150 + (myCharactor.leftArm_timer-30)*2;
		}
		else if(myCharactor.leftArm_timer >= 45){
			myCharactor.leftArm_elbow    = 0;
			myCharactor.leftArm_shoulder = 180;
			myCharactor.leftArm_animationFlg = 0;
			myCharactor.leftArm_timer    = 0;
		}
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// 相手のキャラクタのアニメーション
	//----------------------------------------------------------------------------
	// 乱数を使って相手の位置をランダムに移動
	
	if(yourCharactor.moveX_flg == 0){ // X軸方向の移動をランダムに決定(キャラクタの前後)
		if( ((float)rand()/RAND_MAX) > 0.5 ) yourCharactor.moveX_flg = 1;
		else yourCharactor.moveX_flg = -1;
	}
	if(yourCharactor.moveZ_flg == 0){ // Z軸方向の移動をランダムに決定(キャラクタの左右)
		if( ((float)rand()/RAND_MAX) > 0.5 ) yourCharactor.moveZ_flg = 1;
		else yourCharactor.moveZ_flg = -1;
	}

	// 決定した方向に10フレームの間アニメーション
	if(yourCharactor.moveX_flg > 0){
		yourCharactor.x += 0.1;
		if(yourCharactor.moveX_timer++ > 10){
			yourCharactor.moveX_timer = 0;
			yourCharactor.moveX_flg = 0;
		}
	}
	if(yourCharactor.moveX_flg < 0){
		yourCharactor.x -= 0.1;
		if(yourCharactor.moveX_timer++ > 10){
			yourCharactor.moveX_timer = 0;
			yourCharactor.moveX_flg = 0;
		}
	}
	if(yourCharactor.moveZ_flg > 0){
		yourCharactor.z += 0.1;
		if(yourCharactor.moveZ_timer++ > 10){
			yourCharactor.moveZ_timer = 0;
			yourCharactor.moveZ_flg = 0;
		}
	}
	if(yourCharactor.moveZ_flg < 0){
		yourCharactor.z -= 0.1;
		if(yourCharactor.moveZ_timer++ > 10){
			yourCharactor.moveZ_timer = 0;
			yourCharactor.moveZ_flg = 0;
		}
	}

	// 乱数を使って適当なタイミングで剣を振る
	if( ((float)rand()/RAND_MAX) > 0.99 ) yourCharactor.rightArm_animationFlg = 1;

	// 右腕のアニメーション（関節角度を変更）
	if(yourCharactor.rightArm_animationFlg == 1 || yourCharactor.rightArm_timer > 0 ){
		yourCharactor.rightArm_timer++;
		if(yourCharactor.rightArm_timer <= 30){
		yourCharactor.rightArm_elbow    = -30 + yourCharactor.rightArm_timer;
		yourCharactor.rightArm_shoulder = -30 + yourCharactor.rightArm_timer*6;
		}
		if(yourCharactor.rightArm_timer > 30 && yourCharactor.rightArm_timer < 45 ){
		yourCharactor.rightArm_elbow    = 0 - (yourCharactor.rightArm_timer-30)*2;
		yourCharactor.rightArm_shoulder = 150 - (yourCharactor.rightArm_timer-30)*12;
		}
		if(yourCharactor.rightArm_timer >= 45){
			yourCharactor.rightArm_elbow    = -30;
			yourCharactor.rightArm_shoulder = -30;
			yourCharactor.rightArm_animationFlg = 0;
			yourCharactor.rightArm_timer    = 0;
		}
		//printf("elbow:%5d,shoulder:%5d\n",yourCharactor.rightArm_elbow,yourCharactor.rightArm_shoulder);
	}
	// 左腕のアニメーション（関節角度を変更）
	if(yourCharactor.leftArm_animationFlg == 1 || yourCharactor.leftArm_timer > 0 ){
		yourCharactor.leftArm_timer++;
		if(yourCharactor.leftArm_timer <= 30){
		yourCharactor.leftArm_elbow    = 0 - yourCharactor.leftArm_timer*2;
		yourCharactor.leftArm_shoulder = 180 - yourCharactor.leftArm_timer*1;
		}
		else if(yourCharactor.leftArm_timer > 30 && yourCharactor.leftArm_timer < 45 ){
		yourCharactor.leftArm_elbow    = -60 + (yourCharactor.leftArm_timer-30)*4;
		yourCharactor.leftArm_shoulder = 150 + (yourCharactor.leftArm_timer-30)*2;
		}
		else if(yourCharactor.leftArm_timer >= 45){
			yourCharactor.leftArm_elbow    = 0;
			yourCharactor.leftArm_shoulder = 180;
			yourCharactor.leftArm_animationFlg = 0;
			yourCharactor.leftArm_timer    = 0;
		}
	}
	//----------------------------------------------------------------------------

	// 超単純な当たり判定（近い距離で剣を振り切られたらやられる）
	double dist = (myCharactor.x-yourCharactor.x)*(myCharactor.x-yourCharactor.x) + (myCharactor.z-yourCharactor.z)*(myCharactor.z-yourCharactor.z);
	if(dist < 5.0){
		if(myCharactor.rightArm_timer > 30)	  yourCharactor.isAlive = 0;
		if(yourCharactor.rightArm_timer > 30) myCharactor.isAlive = 0;
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

// 自分のキャラクタの描画
void DrawMyCharactor(void)
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

		glTranslatef(myCharactor.x, 0.0, myCharactor.z); //矢印キーでのX軸方向，Z軸方向の移動
		glRotatef(-90, 0.0, 1.0, 0.0);

		//右腕の描画
		glPushMatrix();
			glTranslatef(-0.6, 0.0, 0.0);
			glRotatef(myCharactor.rightArm_shoulder/3, 0.0, 1.0, 0.0);

			//右上腕の描画
			glRotatef(myCharactor.rightArm_shoulder, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.25,1.0,0.25);
				glutSolidCube(1.0);
			glPopMatrix();

			glTranslatef(0.0, 0.5, 0.0);

			//右前腕の描画
			glRotatef(myCharactor.rightArm_elbow, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.2,1.0,0.2);
				glutSolidCube(1.0);
			glPopMatrix();
			//剣の描画
			glPushMatrix();
				glTranslatef(0.0, 0.5, -0.75);
				glScalef(0.05,0.05,1.5);
				glutSolidCube(1.0);
			glPopMatrix();

		glPopMatrix();

		//左腕の描画
		glPushMatrix();
			glTranslatef(0.6, 0.0, 0.0);
			glRotatef(10, 0.0, 0.0, 1.0);

			//左上腕の描画
			glRotatef(myCharactor.leftArm_shoulder, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.25,1.0,0.25);
				glutSolidCube(1.0);
			glPopMatrix();

			glTranslatef(0.0, 0.5, 0.0);
		
			//左前腕の描画
			glRotatef(myCharactor.leftArm_elbow, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.2,1.0,0.2);
				glutSolidCube(1.0);
			glPopMatrix();

		glPopMatrix();

		//頭の描画
		glTranslatef(0.0, 0.5, 0.0);
		glutSolidSphere(0.5,12,12);

		//身体の描画
		glTranslatef(0.0, -1.0, 0.0);
		glScalef(1.0,1.0,0.2);
		glutSolidCube(1.0);

	glPopMatrix();
}

// 相手のキャラクタの描画
void DrawYourCharactor(void)
{
	glPushMatrix();
		GLfloat mat0ambi[] = {0.1     ,  0.18725, 0.1745, 1.0};//トルコ石
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mat0ambi); //環境光の反射率を設定
		GLfloat mat0diff[] = {0.396,  0.74151, 0.69102, 1.0};//トルコ石
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat0diff); //拡散光の反射率を設定
		GLfloat mat0spec[] = {0.297254,  0.30829, 0.306678, 1.0};//トルコ石
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat0spec); //鏡面光の反射率を設定
		GLfloat mat0shine[] ={ 12.8 };//トルコ石
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat0shine);

		glTranslatef(yourCharactor.x, 0.0, yourCharactor.z); //ランダムにX軸方向，Z軸方向に移動
		glRotatef(90, 0.0, 1.0, 0.0);

		//右腕の描画
		glPushMatrix();
			glTranslatef(-0.6, 0.0, 0.0);
			glRotatef(yourCharactor.rightArm_shoulder/3, 0.0, 1.0, 0.0);

			//右上腕の描画
			glRotatef(yourCharactor.rightArm_shoulder, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.25,1.0,0.25);
				glutSolidCube(1.0);
			glPopMatrix();

			glTranslatef(0.0, 0.5, 0.0);

			//右前腕の描画
			glRotatef(yourCharactor.rightArm_elbow, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.2,1.0,0.2);
				glutSolidCube(1.0);
			glPopMatrix();
			//剣の描画
			glPushMatrix();
				glTranslatef(0.0, 0.5, -0.75);
				glScalef(0.05,0.05,1.5);
				glutSolidCube(1.0);
			glPopMatrix();

		glPopMatrix();

		//左腕の描画
		glPushMatrix();
			glTranslatef(0.6, 0.0, 0.0);
			glRotatef(10, 0.0, 0.0, 1.0);

			//左上腕の描画
			glRotatef(yourCharactor.leftArm_shoulder, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.25,1.0,0.25);
				glutSolidCube(1.0);
			glPopMatrix();

			glTranslatef(0.0, 0.5, 0.0);
		
			//左前腕の描画
			glRotatef(yourCharactor.leftArm_elbow, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.5, 0.0);
			glPushMatrix();
				glScalef(0.2,1.0,0.2);
				glutSolidCube(1.0);
			glPopMatrix();

		glPopMatrix();

		//頭の描画
		glTranslatef(0.0, 0.5, 0.0);
		glutSolidSphere(0.5,12,12);

		//身体の描画
		glTranslatef(0.0, -1.0, 0.0);
		glScalef(1.0,1.0,0.2);
		glutSolidCube(1.0);

	glPopMatrix();

}

void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
	glEnable(GL_DEPTH_TEST); // 隠面消去を有効

	// 自分が死んでいたらYou Lose!を表示する
	if (myCharactor.isAlive==0){
		// 文字列の描画
		glPushMatrix();
			GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,  matString);
			glRasterPos3f(-1.0f, 0.0f, 0.0f);
			char *str = "You Lose!";
			while(*str){
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
				++str;
			}
		glPopMatrix();
	}
	// 相手が死んでいたらYou Win!を表示する
	else if (yourCharactor.isAlive==0){
		// 文字列の描画
		glPushMatrix();
			GLfloat matString[] = { 0.8, 0.0, 0.2, 1.0 }; //環境光と拡散光の反射率をまとめて設定
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,  matString);
			glRasterPos3f(-1.0f, 0.0f, 0.0f);
			char *str = "You Win!";
			while(*str){
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
				++str;
			}
		glPopMatrix();
	}
	// 自分と相手を描画する
	else{
		glPushMatrix();

			//マウスでの視点移動
			glRotatef(SpinX, 1.0, 0.0, 0.0); //X軸周りにSpinXだけ回転
			glRotatef(SpinY, 0.0, 1.0, 0.0); //Y軸周りにSpinYだけ回転
			glScalef(Scale, Scale, Scale);   //Scaleだけ拡大縮小

			// 自分と相手を描画する
			DrawMyCharactor();   // 自分のキャラクタの描画
			DrawYourCharactor(); // 相手のキャラクタの描画

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

		glPopMatrix();
	}

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

	glutMouseFunc(mouse);           // マウスクリックコールバック関数の指定
	glutMotionFunc(dragMotion);     // マウスドラッグコールバック関数の指定

	glutKeyboardFunc(keyboard);     // 通常キーコールバック関数の指定(押したとき)
	glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
	glutSpecialUpFunc(specialKeyUp);// 特殊キーコールバック関数の指定(離したとき)

	glutTimerFunc(20, timer, 20);   // 定期的に呼び出される関数の指定

	// 自分のキャラクターの構造体データを初期化
	myCharactor.isAlive = 1;	 // 生きている
	myCharactor.x = 2;           // キャラクタの初期位置(X座標)
	myCharactor.moveX_flg = 0;   // X軸方向移動アニメーションフラグ
	myCharactor.moveX_timer = 0; // X軸方向移動アニメーションタイマー
	myCharactor.z = 0;	         // キャラクタの初期位置(Z座標)
	myCharactor.moveZ_flg = 0;   // Z軸方向移動アニメーションフラグ
	myCharactor.moveZ_timer = 0; // Z軸方向移動アニメーションタイマー

	myCharactor.rightArm_shoulder = -30;   // キャラクタの右肩の角度
	myCharactor.rightArm_elbow    = -30;   // キャラクタの右肘の角度
	myCharactor.rightArm_timer    = 0;     // 右腕アニメーションフラグ
	myCharactor.rightArm_animationFlg = 0; // 右腕アニメーションタイマー
	myCharactor.leftArm_shoulder = 180;    // キャラクタの左肩の角度
	myCharactor.leftArm_elbow    = 0;      // キャラクタの左肘の角度
	myCharactor.leftArm_timer    = 0;      // 左腕アニメーションフラグ
	myCharactor.leftArm_animationFlg = 0;  // 左腕アニメーションタイマー

	// 相手のキャラクターの構造体データを初期化
	yourCharactor.isAlive = 1;	   // 生きている
	yourCharactor.x = -2;          // キャラクタの初期位置(X座標)
	yourCharactor.moveX_flg = 0;   // X軸方向移動アニメーションフラグ
	yourCharactor.moveX_timer = 0; // X軸方向移動アニメーションタイマー
	yourCharactor.z = 0;	       // キャラクタの初期位置(Z座標)
	yourCharactor.moveZ_flg = 0;   // Z軸方向移動アニメーションフラグ
	yourCharactor.moveZ_timer = 0; // Z軸方向移動アニメーションタイマー

	yourCharactor.rightArm_shoulder = -30;   // キャラクタの右肩の角度
	yourCharactor.rightArm_elbow    = -30;   // キャラクタの右肘の角度
	yourCharactor.rightArm_timer    = 0;     // 右腕アニメーションフラグ
	yourCharactor.rightArm_animationFlg = 0; // 右腕アニメーションタイマー
	yourCharactor.leftArm_shoulder = 180;    // キャラクタの左肩の角度
	yourCharactor.leftArm_elbow    = 0;      // キャラクタの左肘の角度
	yourCharactor.leftArm_timer    = 0;      // 左腕アニメーションフラグ
	yourCharactor.leftArm_animationFlg = 0;  // 左腕アニメーションタイマー

	lightInit();    // 光源の初期設定(まとめて関数にしているだけ)

	glutMainLoop(); // メインループへ

	return 0;
}
