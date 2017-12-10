#include <gl/glut.h>   
#include <Windows.h> 
#include <iostream>

#include "Texture.h"
#include "DrawElement.h"
#include "ObjLoader.h"


#define ON 1
#define OFF 0

#define LEFT -1
#define RIGHT 1

#define TPP 0	//third-person perspective
#define FPP 1	//first-person perspective

using namespace std;

ObjLoader objLoader;
vector<glm::vec3> vertices;
vector<glm::vec2> uvs;
vector<glm::vec3> normals;

GLuint l0 = OFF, l1 = OFF, l2 = OFF;	//light state
GLuint texGround, texLeft, texRight, texFront, texBack, texUp;

GLfloat x0 = 2.0f, step = 0.025f, rb = 0;
GLuint front = LEFT;	//当前朝向

GLuint arm = 0;
GLfloat leg_angle = 15.0f, leg_step = 15.0 / 5;
GLuint leg = 0;
GLfloat arm_angle = 45.0f, arm_step = 45.0 / 5;

GLuint fog = OFF; //fog


GLuint view_dir = TPP;

GLfloat xAngle = 0.0f, yAngle = 0.0f;	//相机角度
GLfloat lx = 0.0f, ly = 0.0f, lz = -5.0f;	//视线方向
GLfloat cx = 0.0f, cy = 0.0f, cz = 5.0f;	//相机位置
GLfloat xDeltaAngle = 0.0f, yDeltaAngle = 0.0f;	//相机旋转角
int xOrigin = -1, yOrigin = -1;


void setupLight(void)
{
	//设置环境光
	{
		GLfloat l0_position[] = { 5.0f, 5.0f, 5.0f, 0.0f };
		GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat l0_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat l0_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, l0_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);
	}

	glPushMatrix();
	//设置路灯光源
	{
		GLfloat lamp_light_position[] = { -1.9f, 1.15f, -0.05f, 1.0f };
		GLfloat lamp_light_ambient[] = { 0.0, 0.0f, 0.0f, 1.0f };
		GLfloat lamp_light_diffuse[] = { 0.3f, 0.3f, 0.0f, 1.0f };
		GLfloat lamp_light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat lamp_light_direction[] = { 0.0f, -0.5f, -3.0f };

		glLightfv(GL_LIGHT1, GL_AMBIENT, lamp_light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp_light_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lamp_light_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, lamp_light_position);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lamp_light_direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0f);	
	}
	glPopMatrix();

	glPushMatrix();
	//设置手电光源
	{
		//GLfloat torch_light_position[] = { x0 - 0.075, -0.45f, 0.035f, 1.0f };
		GLfloat torch_light_ambient[] = { 0.0, 0.0f, 0.0f, 1.0f };
		GLfloat torch_light_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat torch_light_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		//GLfloat torch_light_direction[] = { front * 2.0, 0.0f, -1.5f };

		glLightfv(GL_LIGHT2, GL_AMBIENT, torch_light_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, torch_light_diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, torch_light_specular);
		//glLightfv(GL_LIGHT2, GL_POSITION, torch_light_position);
		//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, torch_light_direction);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0f);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 80.0f);
	}
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void loadTex(void)
{
	texGround = load_texture("down.bmp");
	texLeft = load_texture("left.bmp");
	texRight = load_texture("right.bmp");
	texFront = load_texture("front.bmp");
	texBack = load_texture("back.bmp");
	texUp = load_texture("up.bmp");
}

void init(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);

	//雾
	GLfloat h[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	glFogfv(GL_FOG_COLOR, h);
	glFogf(GL_FOG_START, 17.5f);
	glFogf(GL_FOG_END, 100.0f);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint(GL_FOG_HINT, GL_NICEST);

	setupLight();
}

void drawObj(void)	//绘制模型
{
	glEnable(GL_NORMALIZE);
	
	GLfloat ambient[] = { 0.3f, 0.6f, 0.3f, 1.0f };
	GLfloat diffuse[] = { 0.3f, 0.6f, 0.3f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess = 20.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glPushMatrix();
		glTranslatef(3.0f, 0.0f, 0.0f);
		glScalef(0.001f, 0.001, 0.001f);

		glBegin(GL_TRIANGLES);
		int size = normals.size();
		for (int i = 0; i < size; i++)
		{
			glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
	glPopMatrix();

}

//绘制天空盒
void drawSKybox(void)
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
		
		glBindTexture(GL_TEXTURE_2D, texFront);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 19.0f, -20.0f);
			glTexCoord2f(0.0f, 0.45f); glVertex3f(-20.0f, -1.0f, -20.0f);
			glTexCoord2f(1.0f, 0.45f); glVertex3f(20.0f, -1.0f, -20.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 19.0f, -20.0f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texLeft);
		glBegin(GL_QUADS);
		{
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 19.0f, 20.0f);
			glTexCoord2f(0.0f, 0.45f); glVertex3f(-20.0f, -1.0f, 20.0f);
			glTexCoord2f(1.0f, 0.45f); glVertex3f(-20.0f, -1.0f, -20.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, 19.0f, -20.0f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texBack);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 19.0f, 20.0f);
			glTexCoord2f(0.0f, 0.45f); glVertex3f(20.0f, -1.0f, 20.0f);
			glTexCoord2f(1.0f, 0.45f); glVertex3f(-20.0f, -1.0f, 20.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, 19.0f, 20.0f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texRight);
		glBegin(GL_QUADS);
		{
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 19.0f, -20.0f);
			glTexCoord2f(0.0f, 0.45f); glVertex3f(20.0f, -1.0f, -20.0f);
			glTexCoord2f(1.0f, 0.45f); glVertex3f(20.0f, -1.0f, 20.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 19.0f, 20.0f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texUp);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 19.0f, 20.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 19.0f, -20.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, 19.0f, -20.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 19.0f, 20.0f);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texGround);
		glBegin(GL_QUADS);
		{
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -1.0f, -20.0f);
			glTexCoord2f(0.0f, 2.0f); glVertex3f(-20.0f, -1.0f, 20.0f);
			glTexCoord2f(2.0f, 2.0f); glVertex3f(20.0f, -1.0f, 20.0f);
			glTexCoord2f(2.0f, 0.0f); glVertex3f(20.0f, -1.0f, -20.0f);
		}
		glEnd();

		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

//绘制路灯
void drawLamp(void)
{
	glPushMatrix();
		GLfloat lamp_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };  //定义材质的环境光颜色  
		GLfloat lamp_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };  //定义材质的漫反射光颜色  
		GLfloat lamp_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };   //定义材质的镜面反射光颜色
		GLfloat lamp_shininess = 10.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, lamp_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, lamp_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, lamp_shininess);

		DrawCube(-2.5f, 1.5f, 0.0f, 0.05f, 2.5f, 0.05f);	//灯柱
		DrawCube(-1.75f, 1.25f, 0.0f, 1.0f, 0.05f, 0.05f);

		if (l1)	//灯泡材质
		{
			GLfloat lamp_emission[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_EMISSION, lamp_emission);
		}
		else
		{
			GLfloat lamp_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_EMISSION, lamp_emission);
		}
		glTranslatef(-1.9f, 1.15f, -0.05f);
		glutSolidSphere(0.1f, 40, 40);
	glPopMatrix();
}

//绘制机器人
void drawBot(void)
{
	glPushMatrix();
		GLfloat human_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };  
		GLfloat human_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 
		GLfloat human_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat human_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat human_shininess = 40.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, human_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, human_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, human_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, human_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, human_shininess);

		DrawCube(x0, 0.2f, 0.0f, 0.15f, 0.15f, 0.15f);	//头
		DrawCube(x0, 0.0f, 0.0f, 0.15f, 0.45f, 0.15f);	//身

		//臂
		glPushMatrix();
			glTranslatef(x0 - 0.0375f, -0.05f, 0.07f);
			glRotatef(-arm_angle, 0.0f, 0.0f, 1.0f);
			DrawCube(0.0f, 0.0f, 0.0f, 0.075f, 0.4f, 0.07f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x0 - 0.0375f, -0.05f, -0.15f);
			glRotatef(arm_angle, 0.0f, 0.0f, 1.0f);
			DrawCube(0.0f, 0.0f, 0.0f, 0.075f, 0.4f, 0.07f);
		glPopMatrix();

		//腿
		glPushMatrix();
			glTranslatef(x0 - 0.025f, -0.5f, 0.0f);
			glRotatef(leg_angle, 0.0f, 0.0f, 1.0f);	
			DrawCube(0.0f, 0.0f, 0.0f, 0.1f, 0.5f, 0.07f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x0 - 0.025f, -0.5f, -0.08f);
			glRotatef(-leg_angle, 0.0f, 0.0f, 1.0f);
			DrawCube(0.0f, 0.0f, 0.0f, 0.1f, 0.5f, 0.07f);
		glPopMatrix();

		//手电
		if (l2)
		{
			GLfloat torch_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_EMISSION, torch_emission);
		}
		else
		{
			GLfloat torch_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_EMISSION, torch_emission);
		}

		glPushMatrix();
			glTranslatef(x0 - 0.075, 0.0f, 0.035f);
			glRotatef(-arm_angle, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, -0.45f, 0.0f);
			glutSolidSphere(0.05f, 40, 40);
		glPopMatrix();
	glPopMatrix();
}

//摆臂摆腿
void armAndLeg()
{
	if (!arm)
	{
		if (arm_angle - arm_step > -45.0)
			arm_angle -= arm_step;
		else arm = 1;
	}
	else
	{
		if (arm_angle + arm_step < 45.0)
			arm_angle += arm_step;
		else arm = 0;
	}

	//迈腿
	if (!leg)
	{
		if (leg_angle - leg_step > -15.0)
			leg_angle -= leg_step;
		else leg = 1;
	}
	else
	{
		if (leg_angle + leg_step < 15.0)
			leg_angle += leg_step;
		else leg = 0;
	}
}

//设置相机
void setCamera(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!view_dir)
	{
		gluLookAt(cx, cy, cz, cx + lx, cy + ly, cz + lz, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		if (front == LEFT) gluLookAt(x0 - 0.15, 0.025f, -0.075f, x0 - 2.0, 0.025f, -0.075f, 0.0f, 1.0f, 0.0f);
		else gluLookAt(x0, 0.025f, -0.075f, x0 + 2.0, 0.025f, -0.075f, 0.0f, 1.0f, 0.0f);
	}
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存  
	setCamera();

	glPushMatrix();
		glTranslatef(x0 - 0.075f, -0.45f, 0.035f);
		GLfloat torch_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, torch_light_position);
		if (front == LEFT)
		{
			GLfloat torch_light_direction[] = { -1.0f, 0.0f, 0.0f }; glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, torch_light_direction);
		}
		else
		{
			GLfloat torch_light_direction[] = { 1.0f, 0.0f, 0.0f }; glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, torch_light_direction);
		}
	glPopMatrix();

	{	//初始化材质
		GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat shininess = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	glPushMatrix();
		glTranslatef(0.0f, -1.0f, 0.0f);
		glutSolidSphere(0.1f, 40, 40);
	glPopMatrix();

	drawObj();

	drawSKybox();

	drawLamp();

	drawBot();

	glutSwapBuffers();
}

void myIdle(void)
{	
	renderScene();
}

void reshape(int w, int h)
{
	h = (h == 0) ? 1 : h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 0.1f, 10000.0f);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0); break;

	case 'f':
		if (!fog) glEnable(GL_FOG), fog = ON;
		else glDisable(GL_FOG), fog = OFF;
		break;

	case 'c':
		if (view_dir == TPP) view_dir = FPP;
		else
		{
			view_dir = TPP;
			xAngle = 0.0f, yAngle = 0.0f;
			lx = 0.0f, ly = 0.0f, lz = -5.0f;
			cx = 0.0f, cy = 0.0f, cz = 5.0f;
			xDeltaAngle = 0.0f, yDeltaAngle = 0.0f;
			xOrigin = -1, yOrigin = -1;
		}
	}
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		if (!l0) glEnable(GL_LIGHT0), l0 = ON;
		else glDisable(GL_LIGHT0), l0 = OFF;
		break;

	case GLUT_KEY_F2:
		if (!l1) glEnable(GL_LIGHT1), l1 = ON; 
		else glDisable(GL_LIGHT1), l1 = OFF;
		break;

	case GLUT_KEY_F3:
		if (!l2) glEnable(GL_LIGHT2), l2 = ON;
		else glDisable(GL_LIGHT2), l2 = OFF;
		break;

	case GLUT_KEY_LEFT:
		front = LEFT;
		if (x0 - step > -2.0) x0 -= step;
		else x0 = -2.0;
		armAndLeg();
		break;

	case GLUT_KEY_RIGHT:
		front = RIGHT;
		if (x0 + step < 2.0) x0 += step;
		else x0 = 2.0;
		armAndLeg();
		break;
	}

}

void mouseButton(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON)	//左键控制镜头
	{
		if (state == GLUT_UP)
		{
			xAngle += xDeltaAngle, yAngle += yDeltaAngle;
			xOrigin = -1, yOrigin = -1;
		}
		else 
		{
			xOrigin = x, yOrigin = y;
		}
	}

	if (button == GLUT_RIGHT_BUTTON)	//右键控制移动
	{
		if (state == GLUT_UP)
			rb = 0;
		else
			rb = 1;
	}
}

void mouseMove(int x, int y) 
{
	if (xOrigin >= 0 || yOrigin >= 0)	//左键按下
	{
		if (xOrigin >= 0)
			xDeltaAngle = (x - xOrigin) * 0.001f;
		if (yOrigin >= 0)
			yDeltaAngle = (y - yOrigin) * 0.001f;

		lx = sin(xAngle + xDeltaAngle) * cos(yAngle + yDeltaAngle) * 5.0;
		ly = -sin(yAngle + yDeltaAngle) * 5.0;
		lz = -cos(xAngle + xDeltaAngle) * cos(yAngle + yDeltaAngle) * 5.0;
	}

	if (rb)	//右键按下
	{
		GLfloat xx = (x - 400) / 100.0;
		if (xx > x0) front = RIGHT;
		else front = LEFT;
		if (xx <= 2.0 && xx >= -2.0) x0 = xx;
		armAndLeg();
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GL_Assignment");

	init();
	loadTex();
	bool res = objLoader.loadOBJ("bunny.obj", vertices, uvs, normals);	//导入模型

	glutKeyboardFunc(&processNormalKeys);
	glutSpecialFunc(&processSpecialKeys);

	glutMouseFunc(&mouseButton);
	glutMotionFunc(&mouseMove);

	glutDisplayFunc(&renderScene);
	glutIdleFunc(&myIdle);
	glutReshapeFunc(&reshape);

	glutMainLoop();

	return 0;
}