#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include "opengl.h"

using namespace std;

GLdouble cameraPosition[] = { 0.0, 200.0, 3500.0 };
GLdouble targetPosition[] = { 0.0, 500.0, 0.0 };
GLdouble theta[] = { 0.0, 0.0, 0.0 };

int width;
int height;

int mousebtn;
int prevx;
int prevy;
int prevthetax;
int prevthetay;
int prevrange;

double ground = 935.0; //fixed

float col_skin[3] = { 1.0f, 0.8941f, 0.7686f }; //skin
float col_white[3] = { 1.0f, 1.0f, 1.0f }; //shirt
float col_skirt[3] = { 0.2117f,0.4666f,0.8117f }; //skirt
float col_yel[3] = { 0.0f, 0.0f, 0.0f };
float col_red[3] = { 1.0f, 0.0f, 0.0f }; //ª°
float col_org[3] = { 1.0f, 0.7f, 0.0f }; //¡÷
float col_yell[3] = { 1.0f, 0.9f, 0.0f }; //≥Î
float col_grn[3] = { 0.27f, 0.78f, 0.24f }; //√ 
float col_sky[3] = { 0.35f, 0.68f, 1.0f }; //∆ƒ
float col_vio[3] = { 0.65f, 0.4f, 1.0f }; //∫∏

double right_hand[3] = { 120.0, 490.0 - ground, 0.0 };
double right_elbow[3] = { 120.0, 650.0 - ground, 0.0 };
double right_shoulder[3] = { 120.0, 810.0 - ground, 0.0 };

double left_hand[3] = { 120.0, 490.0 - ground, 0.0 };
double left_elbow[3] = { 120.0, 650.0 - ground, 0.0 };
double left_shoulder[3] = { 120.0, 810.0 - ground, 0.0 };

double right_foot[3] = { 80.0, 20.0 - ground, 0.0 };
double right_joint[3] = { 80.0, 205.0 - ground, 0.0 };
double right_leg[3] = { 80.0, 410.0 - ground, 0.0 };

double left_foot[3] = { 80.0, 20.0 - ground, 0.0 };
double left_joint[3] = { 80.0, 205.0 - ground, 0.0 };
double left_leg[3] = { 80.0, 410.0 - ground, 0.0 };

double neck[3] = { 0.0, 850.0 - ground, 0.0 };

//****************
double right_middle_arm[3];
double right_high_arm[3];
double left_middle_arm[3];
double left_high_arm[3];
double right_middle_leg[3];
double right_high_leg[3];
double left_middle_leg[3];
double left_high_leg[3];
double chest[3];
double waist[3];
double bottom[3];
double head[3];

GLuint textures; //texture
GLuint texName;
GLubyte *pBytes;

double dotProduct(double x1, double y1, double z1, double x2, double y2, double z2);
void crossProduct(double x1, double y1, double z1, double x2, double y2, double z2, double *ptrx, double *ptry, double *ptrz);
void MyDrawChar(const float color[3]);
void MyDrawFace();
void MySphere(double x1, double y1, double z1, double x2, double y2, double z2, double ptrx, double ptry, double ptrz, double round);
void MyDisplay();
void MyMouse(int btn, int state, int x, int y);
void Motion(int x, int y);
void MyReshape(int w, int h);
GLubyte *loadBitmap(const char filename[]);
void initTextures();

int glfmain(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("GL");
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(MyDisplay);
	glutMouseFunc(MyMouse);
	glutMotionFunc(Motion);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	initTextures();
	glutMainLoop();

	return 0;
}

double dotProduct(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double v, theta, degree;

	v = sqrt(pow(x1, 2) + pow(y1, 2) + pow(z1, 2));
	x1 /= v;
	y1 /= v;
	z1 /= v;

	v = sqrt(pow(x2, 2) + pow(y2, 2) + pow(z2, 2));
	x2 /= v;
	y2 /= v;
	z2 /= v;

	theta = x1 * x2 + y1 * y2 + z1 * z2;
	theta = acos(theta);
	degree = theta * (180 / 3.141592);

	return degree;
}

void crossProduct(double x1, double y1, double z1, double x2, double y2, double z2, double *ptrx, double *ptry, double *ptrz)
{
	*ptrx = y1 * z2 - z1 * y2;
	*ptry = z1 * x2 - x1 * z2;
	*ptrz = x1 * y2 - y1 * x2;
}

void MyDrawChar(const float color[3])
{
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-100, 100, -20);
	glVertex3f(-100, 100, 20);
	glVertex3f(100, 100, 20);
	glVertex3f(100, 100, -20); //¿≠∏È

	glVertex3f(-100, 100, 20);
	glVertex3f(-20, -100, 20);
	glVertex3f(20, -100, 20);
	glVertex3f(100, 100, 20); //æ’∏È

	glVertex3f(-100, 100, -20);
	glVertex3f(-20, -100, -20);
	glVertex3f(-20, -100, 20);
	glVertex3f(-100, 100, 20); //æ’±‚¡ÿøﬁ∏È

	glVertex3f(100, 100, 20);
	glVertex3f(20, 100, -20);
	glVertex3f(20, -100, -20);
	glVertex3f(100, 100, -20); //æ’±‚¡ÿø¿∏È

	glVertex3f(100, 100, -20);
	glVertex3f(20, -100, -20);
	glVertex3f(-20, -100, -20);
	glVertex3f(-100, 100, -20); //µﬁ∏È

	glVertex3f(-20, -100, -20);
	glVertex3f(-20, -100, 20);
	glVertex3f(20, -100, 20);
	glVertex3f(20, -100, -20); // πÿ∏È
	glEnd();
}

void MyDrawFace(void)
{
	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-20, 45, -15);
	glVertex3f(-20, 45, 15);
	glVertex3f(20, 45, 15);
	glVertex3f(20, 45, -15); //¿≠∏È
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-20, 45, 15);
	glVertex3d(-40, 25, 25);
	glVertex3d(40, 25, 25);
	glVertex3d(20, 45, 15); //æ’∏È1
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f);  glVertex3d(-40, 25, 25);
	glTexCoord2f(0.0f, 0.0f);  glVertex3d(-40, -25, 25);
	glTexCoord2f(1.0f, 0.0f);  glVertex3d(40, -25, 25);
	glTexCoord2f(1.0f, 1.0f);  glVertex3d(40, 25, 25); //æ’∏È2, æÛ±º
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(col_skin[0], col_skin[1], col_skin[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-40, -25, 25);
	glVertex3d(-20, -45, 15);
	glVertex3d(20, -45, 15);
	glVertex3d(40, -25, 25); //æ’∏È3
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-20, 45, -15);
	glVertex3f(-40, 25, -25);
	glVertex3f(-40, 25, 25);
	glVertex3f(-20, 45, 15); //æ’±‚¡ÿøﬁ∏È1
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-40, 25, -25);
	glVertex3f(-40, -25, -25);
	glVertex3f(-40, -25, 25);
	glVertex3f(-40, 25, 25); //æ’±‚¡ÿøﬁ∏È2
	glEnd();

	glColor3f(col_skin[0], col_skin[1], col_skin[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-40, -25, -25);
	glVertex3f(-20, -45, -15);
	glVertex3f(-20, -45, 15);
	glVertex3f(-40, -25, 25); //æ’±‚¡ÿøﬁ∏È3
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(20, 45, -15);
	glVertex3f(40, 25, -25);
	glVertex3f(40, 25, 25);
	glVertex3f(20, 45, 15); //æ’±‚¡ÿø¿∏•∏È1
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(40, 25, -25);
	glVertex3f(40, -25, -25);
	glVertex3f(40, -25, 25);
	glVertex3f(40, 25, 25); //æ’±‚¡ÿø¿∏•∏È2
	glEnd();

	glColor3f(col_skin[0], col_skin[1], col_skin[2]);
	glBegin(GL_POLYGON);
	glVertex3f(40, -25, -25);
	glVertex3f(20, -45, -15);
	glVertex3f(20, -45, 15);
	glVertex3f(40, -25, 25); //æ’±‚¡ÿø¿∏•∏È3
	glEnd();

	glColor3f(col_yel[0], col_yel[1], col_yel[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-20, 45, -15);
	glVertex3f(-40, 25, -25);
	glVertex3f(-40, -25, -25);
	glVertex3f(-20, -45, -15);
	glVertex3f(20, -45, -15);
	glVertex3f(40, -25, -25);
	glVertex3f(40, 25, -25);
	glVertex3f(20, 45, -15); //µﬁ∏È
	glEnd();

	glColor3f(col_skin[0], col_skin[1], col_skin[2]);
	glBegin(GL_POLYGON);
	glVertex3f(-20, -45, -15);
	glVertex3f(-20, -45, 15);
	glVertex3f(20, -45, 15);
	glVertex3f(20, -45, -15); //πÿ∏È
	glEnd();
}

void MySphere(double x1, double y1, double z1, double x2, double y2, double z2, double ptrx, double ptry, double ptrz, double round)
{

	double length = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
	double degree = dotProduct(0, length, 0, x2 - x1, y2 - y1, z2 - z1);
	crossProduct(0, length, 0, x2 - x1, y2 - y1, z2 - z1, &ptrx, &ptry, &ptrz);

	glPushMatrix();

	glTranslated((x2 - x1) / 2.0 + x1, (y2 - y1) / 2.0 + y1, (z2 - z1) / 2.0 + z1);
	glRotated(degree, ptrx, ptry, ptrz);
	glScaled(1.0, (length / 2) / round, 1.0);
	glColor3f(col_skin[0], col_skin[1], col_skin[2]);
	glutSolidSphere(round, 20.0, 20.0);

	glPopMatrix();
}

void MyDisplay()

{
	for (int i = 0; i < 3; i++) {
		right_middle_arm[i] = (right_hand[i] + right_elbow[i]) / 2.0;
	}
	for (int i = 0; i < 3; i++) {
		right_high_arm[i] = (right_shoulder[i] + right_elbow[i]) / 2.0;
	}

	for (int i = 0; i < 3; i++) {
		left_middle_arm[i] = (left_hand[i] + left_elbow[i]) / 2.0;
	}
	for (int i = 0; i < 3; i++) {
		left_high_arm[i] = (left_shoulder[i] + left_elbow[i]) / 2.0;
	}

	for (int i = 0; i < 3; i++) {
		right_middle_leg[i] = (right_foot[i] + right_joint[i]) / 2.0;
	}
	for (int i = 0; i < 3; i++) {
		right_high_leg[i] = (right_leg[i] + right_joint[i]) / 2.0;
	}

	for (int i = 0; i < 3; i++) {
		left_middle_leg[i] = (left_foot[i] + left_joint[i]) / 2.0;
	}
	for (int i = 0; i < 3; i++) {
		left_high_leg[i] = (left_leg[i] + left_joint[i]) / 2.0;
	}

	chest[0] = neck[0];
	chest[1] = neck[1] - 120.0;
	chest[2] = neck[2];

	waist[0] = neck[0]; waist[1] = neck[1] - 240.0; waist[2] = neck[2];
	bottom[0] = neck[0]; bottom[1] = neck[1] - 340.0; bottom[2] = neck[2];
	head[0] = neck[0]; head[1] = neck[1] + 85.0; head[2] = neck[2];

	/*GLUquadricObj *obj;
	obj = gluNewQuadric();

	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluQuadricOrientation(obj, GLU_OUTSIDE);
	gluQuadricTexture(obj, GL_FALSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL); //π‡±‚∏¶ ¡∂¿˝«ÿ¡ÿ¥Ÿ*/


	glClearColor(0.222, 0.184, 0.135, 1.0);//πË∞Êªˆ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], targetPosition[0], targetPosition[1], targetPosition[2], 0, 1, 0); //ƒ´∏ﬁ∂Û ¿ßƒ° º≥¡§. ∏∂¡ˆ∏∑¿∫ ∞«µÈ §§§§

	glRotated(theta[0], 1, 0, 0);
	glRotated(theta[1], 0, 1, 0);
	glRotated(theta[2], 0, 0, 1);

	glBegin(GL_QUADS);

	glColor3f(0.7f, 0.7f, 0.7f); //¡§∏È∫Æ
	glVertex3f(-4000, 5000 - ground, -4000);
	glVertex3f(-4000, 0 - ground, -4000);
	glVertex3f(4000, 0 - ground, -4000);
	glVertex3f(4000, 5000 - ground, -4000);

	glColor3f(0.2588f, 0.0039f, 0.0f); //πŸ¥⁄
	glVertex3f(-4000, 0 - ground, -4000);
	glVertex3f(-4000, 0 - ground, 4000);
	glVertex3f(4000, 0 - ground, 4000);
	glVertex3f(4000, 0 - ground, -4000);

	glColor3f(0.8f, 0.8f, 0.8f); //¡§∏È±‚¡ÿ øﬁ∫Æ
	glVertex3f(-4000, 5000 - ground, 4000);
	glVertex3f(-4000, 5000 - ground, -4000);
	glVertex3f(-4000, 0 - ground, -4000);
	glVertex3f(-4000, 0 - ground, 4000);

	glColor3f(0.6f, 0.6f, 0.6f); //¡§∏È±‚¡ÿ ø¿∫Æ
	glVertex3f(4000, 5000 - ground, 4000);
	glVertex3f(4000, 5000 - ground, -4000);
	glVertex3f(4000, 0 - ground, -4000);
	glVertex3f(4000, 0 - ground, 4000);

	glColor3f(0.5f, 0.5f, 0.5f); //¡§∏Èæ’∫Æ
	glVertex3f(-4000, 5000 - ground, 4000);
	glVertex3f(-4000, 0 - ground, 4000);
	glVertex3f(4000, 0 - ground, 4000);
	glVertex3f(4000, 5000 - ground, 4000);
	glEnd();

	//********πŸ¥⁄ø° √ºΩ∫∆« ±◊∏Æ±‚
	glPushMatrix();
	glTranslatef(200.0f, 2.0f - ground, 200.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 6; i++) //¬¶¡Ÿ ∞À¡§
	{
		for (int j = 0; j < 8; j++)
		{
			glRectf(width / 10 * j, height / 10 * i, width / 10 * (j + 1), height / 10 * (i + 1));
			j++;
		}
		i++;
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++) //¬¶¡Ÿ »Úªˆ
	{
		for (int j = 1; j < 9; j++)
		{
			glRectf(width / 10 * j, height / 10 * i, width / 10 * (j + 1), height / 10 * (i + 1));
			j++;
		}
		i++;
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 1; i < 7; i++) //»¶¡Ÿ ∞À¡§
	{
		for (int j = 1; j < 9; j++)
		{
			glRectf(width / 10 * j, height / 10 * i, width / 10 * (j + 1), height / 10 * (i + 1));
			j++;
		}
		i++;
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 1; i < 7; i++) //»¶¡Ÿ »Úªˆ
	{
		for (int j = 0; j < 8; j++)
		{
			glRectf(width / 10 * j, height / 10 * i, width / 10 * (j + 1), height / 10 * (i + 1));
			j++;
		}
		i++;
	}

	glPopMatrix();

	//*********∏”∏Æ
	glPushMatrix();
	{
		glTranslated(head[0], head[1], head[2]);
		glScaled(1.5, 1.5, 1.5);
		MyDrawFace();
	}
	glPopMatrix();
	//*********∏Ò
	glPushMatrix();
	{
		glTranslated(neck[0], neck[1], neck[2]);
		glColor3f(col_skin[0], col_skin[1], col_skin[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********∞°Ωø
	glPushMatrix();
	{
		glTranslated(chest[0], chest[1], chest[2]);
		MyDrawChar(col_white);
	}
	glPopMatrix();
	//*********«„∏Æ
	glPushMatrix();
	{
		glTranslated(waist[0], waist[1], waist[2]);
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********πË
	glPushMatrix();
	{
		glTranslated(bottom[0], bottom[1], bottom[2]);
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		glScalef(1.0f, 0.8f, 1.0f);
		MyDrawChar(col_skirt);
	}
	glPopMatrix();
	//*********ø¿∏•∞Òπ›
	glPushMatrix();
	{
		glTranslated(right_leg[0], right_leg[1], right_leg[2]);
		glColor3f(col_skin[0], col_skin[1], col_skin[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********ø¿∏•«„π˜¡ˆ
	double ptrx1 = 0;
	double ptry1 = 0;
	double ptrz1 = 0;

	MySphere(right_joint[0], right_joint[1], right_joint[2], right_leg[0], right_leg[1], right_leg[2], ptrx1, ptry1, ptrz1, 20.0);

	//*********ø¿∏•π´∏≠
	glPushMatrix();
	{
		glTranslated(right_joint[0], right_joint[1], right_joint[2]);
		glColor3f(col_skin[0], col_skin[1], col_skin[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********ø¿∏•¡ææ∆∏Æ
	double ptrx2 = 0;
	double ptry2 = 0;
	double ptrz2 = 0;

	MySphere(right_foot[0], right_foot[1], right_foot[2], right_joint[0], right_joint[1], right_joint[2], ptrx2, ptry2, ptrz2, 20.0);

	//*********ø¿∏•πﬂ
	glPushMatrix();
	{
		glTranslated(right_foot[0], right_foot[1], right_foot[2]);
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁ∞Òπ›
	glPushMatrix();
	{
		glTranslated(left_leg[0], left_leg[1], left_leg[2]);
		glColor3f(col_skin[0], col_skin[1], col_skin[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁ«„π˜¡ˆ
	double ptrx3 = 0;
	double ptry3 = 0;
	double ptrz3 = 0;

	MySphere(left_joint[0], left_joint[1], left_joint[2], left_leg[0], left_leg[1], left_leg[2], ptrx3, ptry3, ptrz3, 20.0);

	//*********øﬁπ´∏≠
	glPushMatrix();
	{
		glTranslated(left_joint[0], left_joint[1], left_joint[2]);
		glColor3f(col_skin[0], col_skin[1], col_skin[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁ¡ææ∆∏Æ
	double ptrx4 = 0;
	double ptry4 = 0;
	double ptrz4 = 0;

	MySphere(left_foot[0], left_foot[1], left_foot[2], left_joint[0], left_joint[1], left_joint[2], ptrx4, ptry4, ptrz4, 20.0);

	//*********øﬁπﬂ
	glPushMatrix();
	{
		glTranslated(left_foot[0], left_foot[1], left_foot[2]);
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();

	//*********ø¿∏•æÓ±˙
	glPushMatrix();
	{
		glTranslated(right_shoulder[0], right_shoulder[1], right_shoulder[2]);
		glColor3f(col_org[0], col_org[1], col_org[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********ø¿∏•∆»   
	double ptrx5 = 0;
	double ptry5 = 0;
	double ptrz5 = 0;

	MySphere(right_elbow[0], right_elbow[1], right_elbow[2], right_shoulder[0], right_shoulder[1], right_shoulder[2], ptrx5, ptry5, ptrz5, 20.0);

	//*********ø¿∏•∆»≤ﬁƒ°
	glPushMatrix();
	{
		glTranslated(right_elbow[0], right_elbow[1], right_elbow[2]);
		glColor3f(col_grn[0], col_grn[1], col_grn[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********ø¿∏•º’∏Ò
	double ptrx6 = 0;
	double ptry6 = 0;
	double ptrz6 = 0;

	MySphere(right_hand[0], right_hand[1], right_hand[2], right_elbow[0], right_elbow[1], right_elbow[2], ptrx6, ptry6, ptrz6, 20.0);

	//*********ø¿∏•º’
	glPushMatrix();
	{
		glTranslated(right_hand[0], right_hand[1], right_hand[2]);
		glColor3f(col_vio[0], col_vio[1], col_vio[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁæÓ±˙
	glPushMatrix();
	{
		glTranslated(left_shoulder[0], left_shoulder[1], left_shoulder[2]);
		glColor3f(col_red[0], col_red[1], col_red[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁ∆»   
	double ptrx7 = 0;
	double ptry7 = 0;
	double ptrz7 = 0;

	MySphere(left_elbow[0], left_elbow[1], left_elbow[2], left_shoulder[0], left_shoulder[1], left_shoulder[2], ptrx7, ptry7, ptrz7, 20.0);

	//*********øﬁ∆»≤ﬁƒ°
	glPushMatrix();
	{
		glTranslated(left_elbow[0], left_elbow[1], left_elbow[2]);
		glColor3f(col_yell[0], col_yell[1], col_yell[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();
	//*********øﬁº’∏Ò
	double ptrx8 = 0;
	double ptry8 = 0;
	double ptrz8 = 0;

	MySphere(left_hand[0], left_hand[1], left_hand[2], left_elbow[0], left_elbow[1], left_elbow[2], ptrx8, ptry8, ptrz8, 20.0);

	//*********øﬁº’
	glPushMatrix();
	{
		glTranslated(left_hand[0], left_hand[1], left_hand[2]);
		glColor3f(col_sky[0], col_sky[1], col_sky[2]);
		glutSolidSphere(20, 20, 20);
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void MyMouse(int btn, int state, int x, int y)
{
	mousebtn = btn;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		prevx = x;
		prevy = y;
		prevthetax = theta[0];
		prevthetay = theta[1];
	}

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

		prevx = x;
		prevy = y;
		prevrange = cameraPosition[2];
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	GLdouble thetax, thetay, range;

	switch (mousebtn) {
	case GLUT_LEFT_BUTTON:
		thetax = (GLdouble)(y - prevy) / (GLdouble)height * 180.0;
		thetay = (GLdouble)(x - prevx) / (GLdouble)width * 180.0;

		theta[0] = thetax + prevthetax;
		theta[1] = thetay + prevthetay;

		if (theta[0] > 360.0) theta[0] -= 360.0;
		if (theta[1] > 360.0) theta[1] -= 360.0;
		if (theta[0] < 0) theta[0] += 360.0;
		if (theta[1] < 0) theta[1] += 360.0;
		break;
	case GLUT_RIGHT_BUTTON:
		range = (GLdouble)(y - prevy) / (GLdouble)500 * 500;

		cameraPosition[2] = prevrange + range;
		if (cameraPosition[2] < 200.0) cameraPosition[2] = 200.0;
		if (cameraPosition[2] > 1800.0) cameraPosition[2] = 1800.0;
		break;
	}

	glutPostRedisplay();
}


void MyReshape(int w, int h)
{
	width = w;
	height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-800.0, 800.0, -600.0 * (GLdouble)h / (GLdouble)w, 600.0 * (GLdouble)h / (GLdouble)w, 800.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();

}

GLubyte *loadBitmap(const char filename[])
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char * data;

	FILE * file = fopen(filename, "rb");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	return data;
}

void initTextures() {
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pBytes = loadBitmap("./moon3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	//glEnable(GL_TEXTURE_2D);
}