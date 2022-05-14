#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include<string.h>
#include "glut.h"
#pragma warning (disable: 4996)

double poscomand1[2] = { 0.0,-1 };
double poscomand2[2] = { 1,-1 };
double poscomand3[2] = { -1,-1 };
double poscomand4[2] = { 1,-1 };

int haikei = 0;
double posblock[4][2];
double velblock[2] = { 0,-0.1 };
double colorblock[4][3];
double posakuma[2] = { 0.0,-0.6 };
int score=0;
int damage=0;
int iscrushblock[4] = { 0 };
int c = 0;
int check = 0;
int start = 0;
int end = 0;
//キーの状態を表すグローバル変数（０：押されてない、１：押されている）
int KeyUp = 0, KeyDown = 0, KeyLeft = 0, KeyRight = 0;

void cheakSpecialKeyPressed(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:KeyUp = 1; break;
	case GLUT_KEY_DOWN:KeyDown = 1; break;
	case GLUT_KEY_LEFT:KeyLeft = 1; break;
	case GLUT_KEY_RIGHT:KeyRight = 1; break;
	}
}

void cheakSpecialKeyReleased(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: KeyUp = 0; break;
	case GLUT_KEY_DOWN:KeyDown = 0; break;
	case GLUT_KEY_LEFT:KeyLeft = 0; break;
	case GLUT_KEY_RIGHT:KeyRight = 0; break;
	}
}

//初期化
void init(void) {
	glClearColor(1.0, 0.0, 0.0, 1.0);
}

struct rectangle {
	double pos[2];//左上の頂点の座標
	double size[2];//サイズ
	double color[3];//色
};

//描画する矩形
struct rectangle *newRect = NULL;

//矩形の座標
double posbegin[2];
double posend[2];

//短径描画関数
void drawRect(struct rectangle *r) {
	glColor3d(r->color[0], r->color[1], r->color[2]);
	glBegin(GL_POLYGON);
	glVertex2d(r->pos[0], r->pos[1]);
	glVertex2d(r->pos[0] + r->size[0], r->pos[1]);
	glVertex2d(r->pos[0] + r->size[0], r->pos[1] - r->size[1]);
	glVertex2d(r->pos[0], r->pos[1] - r->size[1]);
	glEnd();
}
//円描画関数
void drawCircle(double pos_x, double pos_y, double radius, int startAngle, int endAngle) {
	int i;
	glBegin(GL_POLYGON);//折れ線
	for (i = startAngle; i <= endAngle; i += 360.0 / 36) {

		glVertex2d(
			pos_x + radius*cos(3.14*i / 180),
			pos_y + radius*sin(3.14*i / 180)
		);
	}
	glEnd();

}


void drawakuma(double posgiven[2]) {
	double size = 0.15;

	//輪郭
	glColor3d(0.5, 0.0, 0.5);
	drawCircle(posgiven[0], posgiven[1], size*1.0, 0, 360);
	//右目
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] + 0.01, posgiven[1]);
	glVertex2d(posgiven[0] + size*0.4, posgiven[1]);
	glVertex2d(posgiven[0] + size*0.4, posgiven[1] + size*0.3);
	glEnd();
	//左目
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] - 0.01, posgiven[1]);
	glVertex2d(posgiven[0] - size*0.4, posgiven[1]);
	glVertex2d(posgiven[0] - size*0.4, posgiven[1] + size * 0.3);
	glEnd();
	//口
	glColor3d(1.0, 0.0, 0.0);
	drawCircle(posgiven[0], posgiven[1] + size*0.2, size*0.8, 225, 315);
	//角左
	glColor3d(0.5, 0.0, 0.5);
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] - size / 1.4, posgiven[1] + size / 1.4);
	glVertex2d(posgiven[0] - size / 1.3, posgiven[1] + size / 1.5);
	glVertex2d(posgiven[0] - (size * 2) / 1.4, posgiven[1] + (size * 2) / 1.4);
	glEnd();
	drawCircle(posgiven[0] - (size * 2) / 1.4, posgiven[1] + (size * 2) / 1.4, size*0.1, 0, 360);
	//角右
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] + size / 1.4, posgiven[1] + size / 1.4);
	glVertex2d(posgiven[0] + size / 1.3, posgiven[1] + size / 1.5);
	glVertex2d(posgiven[0] + (size * 2) / 1.4, posgiven[1] + (size * 2) / 1.4);
	glEnd();
	drawCircle(posgiven[0] + (size * 2) / 1.4, posgiven[1] + (size * 2) / 1.4, size*0.1, 0, 360);
	//牙左
	glColor3b(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] - size*0.2, posgiven[1] + size*0.3 - size*0.6);
	glVertex2d(posgiven[0] - size*0.4, posgiven[1] + size*0.3 - size*0.6);
	glVertex2d(posgiven[0] - size*0.3, posgiven[1] + size*0.3 - size*0.6 - size*0.7);
	glEnd();
	//牙右
	glBegin(GL_POLYGON);
	glVertex2d(posgiven[0] + size*0.2, posgiven[1] + size*0.3 - size*0.6);
	glVertex2d(posgiven[0] + size*0.4, posgiven[1] + size*0.3 - size*0.6);
	glVertex2d(posgiven[0] + size*0.3, posgiven[1] + size*0.3 - size*0.6 - size*0.7);
	glEnd();
}

//描画関数で短径描画関数を呼び出す
void display(void) {
	int color1;
	int blockid=4;
	int i;
	char str[128];
	char str2[128];
	char str3[128];
	char str4[128];

	glClear(GL_COLOR_BUFFER_BIT);

	if (start == 0) {
		sprintf(str3, "Devil Want To Escape!!");
		glColor3f(0.6, 0.6, 0.6);
		glRasterPos2f(-0.7, 0.3);
		for (i = 0; i < strlen(str3); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str3[i]);
		}
		sprintf(str4, "click to start");
		glColor3f(0.6, 0.6, 0.6);
		glRasterPos2f(-0.3, -0.1);
		for (i = 0; i < strlen(str4); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str4[i]);
		}
		drawakuma(posakuma);
		
	}
	else if (start == 1) {
		if (score >= 100 && check == 0 || check >= 100) {
			if (c == 0) {
				check = score;
				c++;
			}
			sprintf(str, "Game Clear!");
			glColor3f(0.6, 0.6, 0.6);
			glRasterPos2f(-0.4, 0.0);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			sprintf(str2, "you cleared %d blocks!", check);
			glColor3f(0.6, 0.6, 0.6);
			glRasterPos2f(-0.7, -0.2);
			for (i = 0; i < strlen(str2); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);
			}
			end = 1;

		}

		if (damage >= 5 && check < 100) {
			if (c == 0) {
				check = score;
				c++;
			}
			sprintf(str, "Game Over!\n");
			glColor3f(0.6, 0.6, 0.6);
			glRasterPos2f(-0.4, 0.0);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			sprintf(str2, "you cleared %d blocks", check);
			glColor3f(0.6, 0.6, 0.6);
			glRasterPos2f(-0.7, -0.2);
			for (i = 0; i < strlen(str2); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);
			}
			end = 1;
		}

		if (haikei == 0) {
			glClearColor(1.0, 0.0, 0.0, 1.0);
		}
		if (haikei == 1) {
			glClearColor(0.0, 1.0, 0.0, 1.0);
		}
		if (haikei == 2) {
			glClearColor(0.0, 0.0, 1.0, 1.0);
		}

		drawCircle(poscomand1[0], poscomand1[1], 0.2, 0, 360);
		drawCircle(poscomand2[0], poscomand2[1], 0.2, 0, 360);
		drawCircle(poscomand3[0], poscomand3[1], 0.2, 0, 360);

		newRect = (struct rectangle *)malloc(sizeof(struct rectangle));
		for (int j = 0; j < blockid; j++) {
			for (int i = 0; i < 2; i++) {
				newRect->pos[i] = posblock[j][i];
				newRect->size[i] = 0.3;
			}
			newRect->color[0] = colorblock[j][0];
			newRect->color[1] = colorblock[j][1];
			newRect->color[2] = colorblock[j][2];

			drawRect(newRect);
		}

		drawakuma(posakuma);
	}

	glutSwapBuffers();
	
	
}

void updatepos(int value) {
	int i;
	int blockid;
	int color1;
	if (start == 0) {
		if (KeyLeft == 1)posakuma[0] -= 0.1;
		if (KeyRight == 1)posakuma[0] += 0.1;
		if (posakuma[0] >= 1.0)posakuma[0] = 1.0;
		if (posakuma[0] <= -1.0)posakuma[0] = -1.0;
	}
	if (start == 1) {
		for (blockid = 0; blockid < 4; blockid++) {
			for (i = 0; i < 2; i++) {
				posblock[blockid][i] += velblock[i];
			}
		}

		for (blockid = 0; blockid < 4; blockid++) {
			if (posblock[blockid][1] <= -0.5 || iscrushblock[blockid] > 0) {
				score++;
				posblock[blockid][0] = 3.0*rand() / RAND_MAX - 1.0;
				posblock[blockid][1] = 1.0;
				color1 = (int)rand() % 3;

				if (color1 == 0) {
					colorblock[blockid][0] = 1.0;
					colorblock[blockid][1] = 0.0;
					colorblock[blockid][2] = 0.0;
				}
				else if (color1 == 1) {
					colorblock[blockid][0] = 0.0;
					colorblock[blockid][1] = 1.0;
					colorblock[blockid][2] = 0.0;

				}
				else {
					colorblock[blockid][0] = 0.0;
					colorblock[blockid][1] = 0.0;
					colorblock[blockid][2] = 1.0;
				}
				break;
			}
		}

		if (KeyLeft == 1)posakuma[0] -= 0.1;
		if (KeyRight == 1)posakuma[0] += 0.1;
		if (posakuma[0] >= 1.0)posakuma[0] = 1.0;
		if (posakuma[0] <= -1.0)posakuma[0] = -1.0;

		for (blockid = 0; blockid < 4; blockid++) {
			iscrushblock[blockid] = 0;
			if (pow((posakuma[0] - posblock[blockid][0]), 2.0) + pow((posakuma[1] - posblock[blockid][1]), 2.0) <= 0.1) {
				colorblock[blockid][0] = 0.0;
				colorblock[blockid][1] = 0.0;
				colorblock[blockid][2] = 0.0;
				iscrushblock[blockid]++;
				score--;
				damage++;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, updatepos, 0);
}
//マウス操作イベント
void mouse(int button, int state, int x, int y) {
	double pos[2];
	if (button != GLUT_LEFT_BUTTON)return;
	if (state == GLUT_DOWN) {
		pos[0] = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
		pos[1] = -(double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 + 1;
		if (start == 0) {
			if (pos[0]<1 && pos[0]>-1 && pos[1] < 1 && pos[1]>-1) {
				start++;
			}
		}
		else if (start == 1) {
			if (pow(pos[0] - poscomand1[0], 2.0) + pow(pos[1] - (poscomand1[1]), 2.0) < 0.1) {
				haikei = 0;
				glutPostRedisplay();
			}
			if (pow(pos[0] - poscomand2[0], 2.0) + pow(pos[1] - poscomand2[1], 2.0) < 0.1) {
				haikei = 1;
				glutPostRedisplay();
			}
			if (pow(pos[0] - poscomand3[0], 2.0) + pow(pos[1] - poscomand3[1], 2.0) < 0.1) {
				haikei = 2;
				glutPostRedisplay();
			}
		}
		if (end== 1) {
			if (pos[0]<1 && pos[0]>-1 && pos[1] < 1 && pos[1]>-1) {
				start = 0;
				check = 0;
				c = 0;
				end = 0;
				score = 0;
				damage = 0;
			}
		}
		
	}
}
int main(int argc, char *argv[]) {

	int enemy1d, i;
	int color1;
	
	srand((unsigned)time(NULL));
	for (enemy1d = 0; enemy1d < 4; enemy1d++) {
		posblock[enemy1d][0] = 2.0*rand() / RAND_MAX - 1.0;
		posblock[enemy1d][1] = 2.0*rand() / RAND_MAX - 1.0+0.5;
		color1 = (int)rand() % 3;

		if (color1 == 0) {
			colorblock[enemy1d][0] = 1.0;
			colorblock[enemy1d][1] = 0.0;
			colorblock[enemy1d][2] = 0.0;
		}
		else if (color1 == 1) {
			colorblock[enemy1d][0] = 0.0;
			colorblock[enemy1d][1] = 1.0;
			colorblock[enemy1d][2] = 0.0;

		}
		else {
			colorblock[enemy1d][0] = 0.0;
			colorblock[enemy1d][1] = 0.0;
			colorblock[enemy1d][2] = 1.0;
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);

	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);

	glutSpecialFunc(cheakSpecialKeyPressed);
	glutSpecialUpFunc(cheakSpecialKeyReleased);

	glutMouseFunc(mouse);

	init();

	glutTimerFunc(1000, updatepos, 0);
	glutMainLoop();
	return EXIT_SUCCESS;
}
