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