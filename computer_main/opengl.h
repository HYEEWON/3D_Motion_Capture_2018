#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstdlib>

extern double right_hand[3];
extern double left_hand[3];
extern double right_elbow[3];
extern double left_elbow[3];
extern double right_shoulder[3];
extern double left_shoulder[3];
extern double right_foot[3];
extern double right_middle_arm[3];
extern double right_high_arm[3];
extern double left_middle_arm[3];
extern double left_high_arm[3];
extern double right_middle_leg[3];
extern double right_high_leg[3];
extern double left_middle_leg[3];
extern double left_high_leg[3];
extern double chest[3];
extern double waist[3];
extern double bottom[3];
extern double head[3];
extern double neck[3];

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
int glfmain(int argc, char *argv[]);

#endif