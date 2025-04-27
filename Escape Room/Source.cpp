#define STB_IMAGE_IMPLEMENTATION
#include<iostream>
#include<Windows.h>
#include<GL/stb_image.h>
#include<GL/glut.h>

unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;
bool fullScreenMode = true , lightOn = false;
float ratio, eyey = 10, eyez = 40, eyex = 0 , centerX, centerY, centerZ = -20;
float matamb[] = { 1.0f,1.0f,1.0f,1.0f },
matdiff[] = { 1.0f,1.0f,1.0f,1.0f },
matspec[] = { 0.64f,1.0f,1.0f,1.0f },
matshin[] = { 4 },
lightamb[] = { 1,0,1,1 },
lightdiff[] = { 0,.6,.6,1 },
lightspec[] = { .2,.2,.2,1 },
lightPos[] = { 1.0, 1.0, 1.0, 1.0 };
void background();
void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void load(int imgnum);
void check(unsigned char* data);

void rightWall();
void leftWall();
void frontWall();
void backWall();
void floor();
void roof();
void Room();
void drawChair();
void chair();


void main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutCreateWindow(" 3D shapes and animation");
	glutFullScreen();

	background();
	glutDisplayFunc(mydraw);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
}

void background() {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	// Lighting
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT, GL_AMBIENT, matamb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matshin);

}
void toggleLight(int value) {
	lightOn = !lightOn;

	if (lightOn) {
		glEnable(GL_LIGHT0);  // ON
	}
	else {
		glDisable(GL_LIGHT0); // OFF
	}
	//glutPostRedisplay();
}

void mydraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eyex, eyey, eyez,
		centerX, centerY, centerZ,
		0, 1, 0);

	Room();
	chair();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (h == 0) h = 1;
	ratio = w / (float)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void timer(int v) {
	toggleLight(v);
	glutPostRedisplay();
	glutTimerFunc(10000, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 27)
		exit(0);
}

void specialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		fullScreenMode = !fullScreenMode;
		if (fullScreenMode) {
			glutFullScreen();
		}
		else {
			glutReshapeWindow(GetSystemMetrics(SM_CXSCREEN) / 2,
				GetSystemMetrics(SM_CYSCREEN) / 2);
			glutPositionWindow(GetSystemMetrics(SM_CXSCREEN) / 4,
				GetSystemMetrics(SM_CYSCREEN) / 4);
		}
	}
	if (key == GLUT_KEY_RIGHT) {
		centerX += .9;
		centerZ += .001;
	}
	if (key == GLUT_KEY_LEFT) {
		centerX -= .9;
		centerZ -= .001;
	}
	if (key == GLUT_KEY_DOWN) {
		eyez += 1;
		centerZ +=1;
		
	}
	if (key == GLUT_KEY_UP) {
		eyez -= 1;
		centerZ -=1;
	}
	if (key == GLUT_KEY_F3) {
		if (centerY > -2) { // Don't go lower than floor
			centerY -= 0.5;
		}
	}
	if (key == GLUT_KEY_F4) {
		if (centerY < 20) { // Don't go higher than roof
			centerY += 0.5;
		}
	}
	glutPostRedisplay();
}


void load(int imgnum) {
	if (imgnum == 1) {

		data = stbi_load("floor.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 2) {

		data = stbi_load("roof.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 3) {
		data = stbi_load("chair-wood.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
}

void check(unsigned char* data) {
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void floor() {
	load(1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(15, -2, -20);
	glEnd();
}

void roof() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, 20, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void rightWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(15, 20, -20);
	glEnd();
}

void leftWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void frontWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, -20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, -20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, -20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, -20);
	glEnd();
}

void backWall() {
	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-15, -2, 20);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(15, -2, 20);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(15, 20, 20);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-15, 20, 20);

	glEnd();
}

void Room()
{
	floor();
	roof();
	frontWall();
	//backWall();
	rightWall();
	leftWall();
	glDisable(GL_TEXTURE_2D);
}

void drawChair()
{
	glEnable(GL_TEXTURE_2D);
	load(3);
	// Surface
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.5f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.5f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.5f, -1.0f);
	glEnd();

	float legWidth = 0.2f;
	float legHeight = 1.5f;

	// Front Right Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.9f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.7f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, 0.9f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7f, legHeight, 0.9f);
	glEnd();

	// Front Left Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.9f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.7f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, legHeight, 0.9f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7f, 0.0f, 0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, 0.9f);
	glEnd();

	// Back Right Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.7f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.9f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.9f, legHeight, -0.7f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7f, legHeight, -0.7f);
	glEnd();

	// Back Left Leg
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.7f);
	// Back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.9f);
	// Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.9f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, legHeight, -0.7f);
	// Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7f, 0.0f, -0.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, legHeight, -0.7f);
	glEnd();

	// Back support
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 3.5f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 3.5f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.5f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.5f, -1.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


//void drawChair() {
//
//	glBegin(GL_QUADS);
//
//	glVertex3f(1.0f, 1.5f, 1.0f);
//	glVertex3f(-1.0f, 1.5f, 1.0f);
//	glVertex3f(-1.0f, 1.5f, -1.0f);
//	glVertex3f(1.0f, 1.5f, -1.0f);
//	glEnd();
//
//	
//	float legWidth = 0.2f;
//	float legHeight = 1.5f;
//
//	// Front Right Leg
//	glBegin(GL_QUADS);
//	// Front
//	glVertex3f(0.9f, 0.0f, 0.9f);
//	glVertex3f(0.7f, 0.0f, 0.9f);
//	glVertex3f(0.7f, legHeight, 0.9f);
//	glVertex3f(0.9f, legHeight, 0.9f);
//	// Back
//	glVertex3f(0.9f, 0.0f, 0.7f);
//	glVertex3f(0.7f, 0.0f, 0.7f);
//	glVertex3f(0.7f, legHeight, 0.7f);
//	glVertex3f(0.9f, legHeight, 0.7f);
//	// Right 
//	glVertex3f(0.9f, 0.0f, 0.9f);
//	glVertex3f(0.9f, 0.0f, 0.7f);
//	glVertex3f(0.9f, legHeight, 0.7f);
//	glVertex3f(0.9f, legHeight, 0.9f);
//	// Left
//	glVertex3f(0.7f, 0.0f, 0.9f);
//	glVertex3f(0.7f, 0.0f, 0.7f);
//	glVertex3f(0.7f, legHeight, 0.7f);
//	glVertex3f(0.7f, legHeight, 0.9f);
//	glEnd();
//
//	// Front Left Leg
//	glBegin(GL_QUADS);
//	glVertex3f(-0.7f, 0.0f, 0.9f);
//	glVertex3f(-0.9f, 0.0f, 0.9f);
//	glVertex3f(-0.9f, legHeight, 0.9f);
//	glVertex3f(-0.7f, legHeight, 0.9f);
//
//	glVertex3f(-0.7f, 0.0f, 0.7f);
//	glVertex3f(-0.9f, 0.0f, 0.7f);
//	glVertex3f(-0.9f, legHeight, 0.7f);
//	glVertex3f(-0.7f, legHeight, 0.7f);
//
//	glVertex3f(-0.9f, 0.0f, 0.9f);
//	glVertex3f(-0.9f, 0.0f, 0.7f);
//	glVertex3f(-0.9f, legHeight, 0.7f);
//	glVertex3f(-0.9f, legHeight, 0.9f);
//
//	glVertex3f(-0.7f, 0.0f, 0.9f);
//	glVertex3f(-0.7f, 0.0f, 0.7f);
//	glVertex3f(-0.7f, legHeight, 0.7f);
//	glVertex3f(-0.7f, legHeight, 0.9f);
//	glEnd();
//
//	// Back Right Leg
//	glBegin(GL_QUADS);
//	glVertex3f(0.9f, 0.0f, -0.7f);
//	glVertex3f(0.7f, 0.0f, -0.7f);
//	glVertex3f(0.7f, legHeight, -0.7f);
//	glVertex3f(0.9f, legHeight, -0.7f);
//
//	glVertex3f(0.9f, 0.0f, -0.9f);
//	glVertex3f(0.7f, 0.0f, -0.9f);
//	glVertex3f(0.7f, legHeight, -0.9f);
//	glVertex3f(0.9f, legHeight, -0.9f);
//
//	glVertex3f(0.9f, 0.0f, -0.7f);
//	glVertex3f(0.9f, 0.0f, -0.9f);
//	glVertex3f(0.9f, legHeight, -0.9f);
//	glVertex3f(0.9f, legHeight, -0.7f);
//
//	glVertex3f(0.7f, 0.0f, -0.7f);
//	glVertex3f(0.7f, 0.0f, -0.9f);
//	glVertex3f(0.7f, legHeight, -0.9f);
//	glVertex3f(0.7f, legHeight, -0.7f);
//	glEnd();
//
//	// Back Left Leg
//	glBegin(GL_QUADS);
//	glVertex3f(-0.7f, 0.0f, -0.7f);
//	glVertex3f(-0.9f, 0.0f, -0.7f);
//	glVertex3f(-0.9f, legHeight, -0.7f);
//	glVertex3f(-0.7f, legHeight, -0.7f);
//
//	glVertex3f(-0.7f, 0.0f, -0.9f);
//	glVertex3f(-0.9f, 0.0f, -0.9f);
//	glVertex3f(-0.9f, legHeight, -0.9f);
//	glVertex3f(-0.7f, legHeight, -0.9f);
//
//	glVertex3f(-0.9f, 0.0f, -0.7f);
//	glVertex3f(-0.9f, 0.0f, -0.9f);
//	glVertex3f(-0.9f, legHeight, -0.9f);
//	glVertex3f(-0.9f, legHeight, -0.7f);
//
//	glVertex3f(-0.7f, 0.0f, -0.7f);
//	glVertex3f(-0.7f, 0.0f, -0.9f);
//	glVertex3f(-0.7f, legHeight, -0.9f);
//	glVertex3f(-0.7f, legHeight, -0.7f);
//	glEnd();
//
//	// Back
//	glBegin(GL_QUADS);
//	glVertex3f(1.0f, 3.5f, -1.0f);
//	glVertex3f(-1.0f, 3.5f, -1.0f);
//	glVertex3f(-1.0f, 1.5f, -1.0f);
//	glVertex3f(1.0f, 1.5f, -1.0f);
//	glEnd();
//		
//}

void chair()
{
	glPushMatrix();
	glTranslatef(12, -2, -16); 
	glScalef(2.0f, 2.0f, 2.0f);    
	drawChair();
	glPopMatrix();
}

