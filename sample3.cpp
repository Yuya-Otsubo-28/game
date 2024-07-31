#define GL_SILENCE_DEPRECATION

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _USE_MATH_DEFINES
#include<math.h>

#include <GL/glut.h>

#define FALSE 0
#define TRUE  1

typedef struct{
    unsigned char c;
    int x,y;
}Key;
Key _key;

//迷路
#define MAZE_WIDTH  10 //描画の都合上正方形にしている
#define MAZE_HEIGHT 10
//迷路の情報 0:空き 1:壁 2:自機の初期位置 左上原点，→x正↓y正
//端は1で埋めておかないと盤外に出てしまう可能性が出てくるので注意
static const int maze[MAZE_HEIGHT][MAZE_WIDTH]={
    {1,1,1,1,1,1,1,1,1,1},
    {1,2,1,0,1,0,1,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};
static const double HEX_SIZE=1.0/MAZE_WIDTH; //マスのサイズ

enum DIR{UP,RIGHT,BOTTOM,LEFT,DIR_NUM};  //向き
static const int DX[DIR_NUM]={0,-1,0,1}; //向きに対応したxyの増分の表
static const int DY[DIR_NUM]={1,0,-1,0};
static const double DT[DIR_NUM]={M_PI/2.0,M_PI,-M_PI/2,0.0}; //向きに対応したradian
enum MODE{MODE_STOP,MODE_GO,MODE_TURN}; //移動モード
typedef struct{
    unsigned char alive; //生きてる？
    unsigned int fcnt; //フレームカウント
    int ix,iy; //迷路位置
    unsigned int d; //向き
    float cx,cy; //今の描画位置
    double dx,dy; //次の描画位置
    double bx,by; //前の描画位置
    double cct;//今のカメラ向き
    double bct; //前のカメラ向き
    double dct; //次のカメラ向き
    unsigned int mode; //移動モード
    unsigned int fdul; //アニメーション時間
}MyBody; //自機の構造体
MyBody mb; //自機オブジェクト

//迷路描画
void draw_maze(void)
{
    //図形の色 (白)
    static const GLfloat white[] = { 0.9, 0.9, 0.9, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    
    for(int r=0;r<MAZE_HEIGHT;++r){
        for(int c=0;c<MAZE_WIDTH;++c){
            glPushMatrix();{
                //床
                glTranslatef((c+0.5)*HEX_SIZE,-HEX_SIZE,(r+0.5)*HEX_SIZE);
                glutSolidCube(HEX_SIZE);
                if(maze[r][c]==1){
                    //壁
                    glPushMatrix();{
                        glTranslatef(0,HEX_SIZE,0); //上に移動
                        glutSolidCube(HEX_SIZE);
                    }glPopMatrix();
                }
            }glPopMatrix();
        }}
}

//自機描画（デバッグ用？）
void draw_mb(void)
{
    glPushMatrix();{
        //図形の色 (赤)
        static const GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
        glTranslatef(mb.cx,0.0,mb.cy);
        glutSolidSphere(HEX_SIZE*0.25,50,50);
    }glPopMatrix();
}

void timerFunc(int t)             // 指定時間後に呼び出される関数（Timerコールバック関数）
{
    //キー入力ハンドル
    switch(_key.c){
        case 's':{ //up
            if(mb.mode==MODE_STOP){ //移動中は受け付けない
                int tx=mb.ix+DX[mb.d]; int ty=mb.iy+DY[mb.d]; //前進位置を計算
                if(maze[ty][tx]!=1){ //壁じゃなければ
                    //前に進む
                    mb.ix=tx; mb.iy=ty;
                    //アニメーションを登録
                    mb.bx=mb.cx; mb.by=mb.cy; //元
                    mb.dx=(mb.ix+0.5)*HEX_SIZE; mb.dy=(mb.iy+0.5)*HEX_SIZE; //先
                    mb.fcnt=0;
                    mb.mode=MODE_GO;
                }
            }
            break;
        }case 'c':{ //right
            if(mb.mode==MODE_STOP){ //移動中は受け付けない
                //右向く
                //アニメーションを登録
                mb.bct=DT[mb.d];
                mb.d=(mb.d+1)%DIR_NUM;
                mb.dct=mb.bct+M_PI/2.0; //右向け右
                mb.fcnt=0;
                mb.mode=MODE_TURN;
            }
            break;
        }case 'x':{ //bottom
            if(mb.mode==MODE_STOP){ //移動中は受け付けない
                //後ろ向く
                //アニメーションを登録
                mb.bct=DT[mb.d];
                mb.d=(mb.d+2)%DIR_NUM;
                mb.dct=mb.bct+M_PI; //回れ右
                mb.fcnt=0;
                mb.mode=MODE_TURN;
            }
            break;
        }case 'z':{ //left
            if(mb.mode==MODE_STOP){ //移動中は受け付けない
                //左向く
                //アニメーションを登録
                mb.bct=DT[mb.d];
                mb.d=(mb.d-1+DIR_NUM)%DIR_NUM;
                mb.dct=mb.bct-M_PI/2.0; //左向け左
                mb.fcnt=0;
                mb.mode=MODE_TURN;
            }
            break;
        }default:
            break;
    }
    _key.c=0; //キー入力を初期化
    
    //アニメーション
    if(mb.mode==MODE_GO){ //前進
        //等速度で移動
        mb.cx=(mb.dx-mb.bx)*mb.fcnt/mb.fdul+mb.bx;
        mb.cy=(mb.dy-mb.by)*mb.fcnt/mb.fdul+mb.by;
        mb.fcnt+=1;
        if(mb.fcnt==mb.fdul){ //終端
            mb.cx=(mb.ix+0.5)*HEX_SIZE;
            mb.cy=(mb.iy+0.5)*HEX_SIZE;
            mb.mode=MODE_STOP;
        }
    }else if(mb.mode==MODE_TURN){ //回転
        //等速度で移動
        mb.cct=(mb.dct-mb.bct)*mb.fcnt/mb.fdul+mb.bct;
        mb.fcnt+=1;
        if(mb.fcnt==mb.fdul){ //終端
            mb.cct=DT[mb.d];
            mb.mode=MODE_STOP;
        }
    }
    
    //描画依頼
    glutPostRedisplay();
    //次タイマー登録
    glutTimerFunc(30,timerFunc,0);
}

void specialKey(int key,int x,int y)
{
    //キー入力は構造体に一時保存しておく
    _key.x=x;
    _key.y=y;
    
    switch (key) {
        case GLUT_KEY_UP:
            //printf("(%3d,%3d)で[↑]が押されました\n", x, y);
            _key.c='s';
            break;
        case GLUT_KEY_DOWN:
            //printf("(%3d,%3d)で[↓]が押されました\n", x, y);
            _key.c='x';
            break;
        case GLUT_KEY_LEFT:
            //printf("(%3d,%3d)で[←]が押されました\n", x, y);
            _key.c='z';
            break;
        case GLUT_KEY_RIGHT:
            //printf("(%3d,%3d)で[→]が押されました\n", x, y);
            _key.c='c';
            break;
    }
}

void display(void)            // 描画時に呼び出される関数（Displayコールバック関数）
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 画面バッファクリア
    glEnable(GL_DEPTH_TEST); // 隠面消去を有効
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //視点をアニメーションする
    gluLookAt(mb.cx,0.03,mb.cy, mb.cx+cos(mb.cct),0.03,mb.cy+sin(mb.cct), 0,1,0);
    
    //光源の設定
    //自機位置に光源
    GLfloat light_pos[] = { mb.cx, (float)(0.5*HEX_SIZE), mb.cy, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    GLfloat  light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    GLfloat  light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    GLfloat  light_specular[] = {0.1, 0.1, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //スポットライトの設定（一つ前のブロックを照らす）
    GLfloat  spot_dir[] = {(float)(HEX_SIZE*cos(mb.cct)),-(float)(HEX_SIZE*0.5), (float)(HEX_SIZE*sin(mb.cct))};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 25.0);
    //光の減衰の設定（遠くまで光が届かないようにする）
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);
    glEnable(GL_LIGHT0);
    
    //迷路の描画
    draw_maze();
    
    //自分の描画(デバック用？)
    draw_mb();
    
    //バッファの切り替え
    glutSwapBuffers();
}

void lightInit(void)        // 光源の初期設定(まとめて関数にしているだけ)
{
    glEnable(GL_LIGHTING);  //光源の設定を有効にする
    glEnable(GL_LIGHT0);    //0番目の光源を有効にする(8個まで設定可能)
    glEnable(GL_NORMALIZE); //法線ベクトルの自動正規化を有効
    
    glShadeModel(GL_SMOOTH); //スムーズシェーディングに設定
}

void reshape(int w, int h) // ウインドウサイズ変更時に呼び出される関数（Reshapeコールバック関数）
{
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (double)w/h, 0.01, 10.0); // 透視投影
}

int main(int argc,char **argv)
{
    glutInit(&argc, argv);          // GLUT初期化
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);   // ウィンドウサイズの指定
    glutCreateWindow("window");     // 表示ウィンドウ作成
    glutReshapeFunc(reshape);       // Reshapeコールバック関数の指定
    glutDisplayFunc(display);       // Displayコールバック関数の指定
    
    glutSpecialFunc(specialKey);    // 特殊キーコールバック関数の指定(押したとき)
    glutTimerFunc(30,timerFunc,0);  // 定期的に呼び出される関数の指定
    
    lightInit();   // 光源の初期設定(まとめて関数にしているだけ)
    
    //初期化処理
    //迷路から自機の初期位置を探す
    for(int r=0;r<MAZE_HEIGHT;++r){
        for(int c=0;c<MAZE_WIDTH;++c){
            if(maze[r][c]==2){
                mb.ix=c;
                mb.iy=r;
                goto loop_end;
            }
        }}
loop_end:
    
    //自機位置から描画位置を計算
    mb.cx=(mb.ix+0.5)*HEX_SIZE;
    mb.cy=(mb.iy+0.5)*HEX_SIZE;
    
    //その他設定
    mb.alive=TRUE; mb.fcnt=0;
    mb.d=UP;            //初期向き
    mb.cct=DT[mb.d];    //カメラ向き
    mb.mode=MODE_STOP;  //移動モード
    mb.fdul=10; //アニメーション時間 （＝移動スピードの逆数．浮動小数の累加誤差を懸念）
    
    glutMainLoop(); // メインループへ
    
    return 0;
}
