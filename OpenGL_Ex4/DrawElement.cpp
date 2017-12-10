#include "DrawElement.h"

void Draw(void)
{
	glBegin(GL_POLYGON);
	{
		//����
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 0.0f);

		//����
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -1.0f, 0.0f);

		//����
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, -1.0f);

		//����
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);

		//����
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);

		//����
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f);
	}
	glEnd();
}

//���Ƶ�λ������
void DrawCube(GLfloat xP, GLfloat yP, GLfloat zP, GLfloat xS, GLfloat yS, GLfloat zS, GLuint tex)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glPushMatrix();
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTranslatef(xP, yP, zP);
		glScalef(xS, yS, zS);

		Draw();

		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void DrawCube(GLfloat xP, GLfloat yP, GLfloat zP, GLfloat xS, GLfloat yS, GLfloat zS)
{
	glPushMatrix();
	glEnable(GL_NORMALIZE);

	glTranslatef(xP, yP, zP);
	glScalef(xS, yS, zS);

	Draw();

	glPopMatrix();
}