
// OpenGL_exe1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "StdAfx.h"
#include <stdlib.h>
#include <GL/glut.h>

#include<opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<cmath>
#include<Windows.h>
#include<iostream>
using namespace cv;
using namespace std;

GLfloat angle = 0;
GLfloat angle_1 = 0;
GLfloat angle_2 = 0;
GLfloat x_trens = 0;
GLfloat y_trens = 0;
GLfloat z_trens = 0;


GLfloat point[100][100][3] = { 0.0 };   //100*100����������
struct vectPoint
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct vectPoint normface[100][100][2]; //��ķ���
struct vectPoint va, vb, vc, vd;
struct vectPoint normEveryPoint[100][100];	//���㷨��
GLfloat multiple = 10.0f;

void drawPoint(GLfloat x, GLfloat y) {//x,y��100*100Ҫ���Եı���ϵ��

	GLfloat up;  //����
	GLfloat right; //����
	GLfloat z;
	Mat image = imread("point.jpg", 0);
	for (int j = 0, up = -50.0f; j< 100; j++, up++)
	{
		for (int i = 0, right = -50.0f; i < 100; i++, right++) {
			
			point[j][i][0] = right / 100.0;
			point[j][i][1] = up / 100.0;
			point[j][i][2] = image.at<uchar>(j, i)/255.0;
		}
	}
}

int checkSize(int x)
{
	if (x == 2 || x == 4 ||
		x == 8 || x == 16 ||
		x == 32 || x == 64 ||
		x == 128 || x == 256 || x == 512)
		return 1;
	else return 0;
}

GLuint load_texture(char *name)
{
	//OpenCV��ȡͼ��
	Mat image = imread(name);
	
	int width = image.cols;
	int height = image.rows;

	/*make sure dimension is a power of 2 **/
	if (!checkSize(height) || !checkSize(width))
		return 0;
	/*no image data*/
	if (!image.data)
		return 0;
	imshow("����",image);
	//����ͼ��ָ��
	GLubyte* imageData;
	
	//OpenGL��������������ʾ
	GLuint texture_ID;

	//��ȡͼ��ָ��
	int pixellength = width*height * 3;
	imageData = new GLubyte[pixellength];
	//����
	memcpy(imageData, image.data, pixellength * sizeof(uchar));

	//��texture_ID����Ϊ2D������Ϣ
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//����Ŵ���Сʹ�����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ˮƽ��ֱ��������ʹ���ظ���ͼ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//����ˮƽ��ֱ��������ʹ�ñ�Ե������ͼ(���ظ���ͼ��ѡһ)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//ָ��������ͼ�Ͳ��ʻ�ϵķ�ʽ
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//��ͼ���ڴ�����������Ϣ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageData);

	free(imageData);
	return texture_ID;
}


void normSurface() {   //��ÿ����ķ��� 
	for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 99; j++) {
			va.x = point[i][j][0]-point[i + 1][j][0];
			va.y = point[i][j][1] - point[i + 1][j][1];
			va.z = point[i][j][2] - point[i + 1][j][2];
			vb.x = point[i + 1][j + 1][0] - point[i+1][j][0];
			vb.y = point[i + 1][j + 1][1] - point[i + 1][j][1];
			vb.z= point[i + 1][j + 1][2] - point[i + 1][j][2];
			normface[i][j][0].x = va.y * vb.z - va.z * vb.y;
			normface[i][j][0].y = va.z * vb.x - va.x * vb.z;
			normface[i][j][0].z = va.x * vb.y - va.y * vb.x;

			vc.x = point[i][j][0] - point[i][j+1][0];
			vc.y = point[i][j][1] - point[i][j + 1][1];
			vc.z = point[i][j][2] - point[i][j + 1][2];
			vd.x = point[i + 1][j + 1][0] - point[i][j+1][0];
			vd.y = point[i + 1][j + 1][1] - point[i][j + 1][1];
			vd.z = point[i + 1][j + 1][2] - point[i][j + 1][2];
			normface[i][j][1].x = vc.y * vd.z - vc.z * vd.y;
			normface[i][j][1].y = vc.z * vd.x - vc.x * vd.z;
			normface[i][j][1].z = vc.x * vd.y - vc.y * vd.x;

			//��һ��
			for (int k = 0; k < 2; k++) {
				GLfloat L = sqrt(normface[i][j][k].x*normface[i][j][k].x+ normface[i][j][k].y*normface[i][j][k].y+ normface[i][j][k].z*normface[i][j][k].z);
				normface[i][j][k].x = normface[i][j][k].x / L;
				normface[i][j][k].y = normface[i][j][k].y / L;
				normface[i][j][k].z = normface[i][j][k].z / L;
			}
		}
	}
}

void normPoint() {  //����ÿ������ķ���

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (i==0&&j==0) {
				normEveryPoint[i][j].x = normface[i][j][0].x + normface[i][j][1].x;
				normEveryPoint[i][j].y = normface[i][j][0].y + normface[i][j][1].y;
				normEveryPoint[i][j].z = normface[i][j][0].z + normface[i][j][1].z;
			}
			else if (i==0&&j==99) {
				normEveryPoint[i][j].x = normface[i][j-1][1].x;
				normEveryPoint[i][j].y = normface[i][j-1][1].y;
				normEveryPoint[i][j].z = normface[i][j-1][1].z;
			}
			else if(j > 0 && j < 99 && i == 0){
				normEveryPoint[i][j].x = normface[i][j - 1][1].x + normface[i][j][0].x + normface[i][j][1].x ;
				normEveryPoint[i][j].y = normface[i][j - 1][1].y + normface[i][j][0].y + normface[i][j][1].y;
				normEveryPoint[i][j].z = normface[i][j - 1][1].z + normface[i][j][0].z + normface[i][j][1].z;
			}
			else if (i == 99 &&j == 99) {
				normEveryPoint[i-1][j].x = normface[i-1][j-1][1].x + normface[i - 1][j-1][0].x;
				normEveryPoint[i-1][j].y = normface[i-1][j-1][1].y + normface[i - 1][j-1][0].x;
				normEveryPoint[i-1][j].z = normface[i-1][j-1][1].z + normface[i - 1][j-1][0].x;
			}
			else if (i == 99 && j == 0) {
				normEveryPoint[i][j].x = normface[i-1][j][0].x;
				normEveryPoint[i][j].y = normface[i-1][j][0].y;
				normEveryPoint[i][j].z = normface[i-1][j][0].z;
			}
			else if (i > 0 && i < 99 && j == 99) {
				normEveryPoint[i][j].x = normface[i - 1][j - 1][1].x + normface[i - 1][j - 1][0].x + normface[i][j - 1][1].x;
				normEveryPoint[i][j].y = normface[i - 1][j - 1][1].y + normface[i - 1][j - 1][0].y + normface[i][j - 1][1].y;
				normEveryPoint[i][j].z = normface[i - 1][j - 1][1].z + normface[i - 1][j - 1][0].z + normface[i][j - 1][1].z;
			}
			else if (j > 0 && j < 99 && i == 99) {
				normEveryPoint[i][j].x = normface[i - 1][j-1][1].x + normface[i - 1][j-1][0].x + normface[i-1][j][0].x;
				normEveryPoint[i][j].y = normface[i - 1][j-1][1].y + normface[i - 1][j-1][0].y + normface[i-1][j][0].y;
				normEveryPoint[i][j].z = normface[i - 1][j-1][1].z + normface[i - 1][j-1][0].z + normface[i-1][j][0].z;
			}
			else if (i > 0 && i < 99 && j == 0) {
				normEveryPoint[i][j].x = normface[i-1][j][0].x + normface[i][j][0].x + normface[i][j][1].x;
				normEveryPoint[i][j].y = normface[i-1][j][0].y + normface[i][j][0].y + normface[i][j][1].y;
				normEveryPoint[i][j].z = normface[i-1][j][0].z + normface[i][j][0].z + normface[i][j][1].z;
			}
			else {
				normEveryPoint[i][j].x = normface[i][j - 1][1].x + normface[i][j][0].x + normface[i][j][1].x + normface[i - 1][j - 1][0].x + normface[i - 1][j - 1][1].x + normface[i - 1][j][0].x;
				normEveryPoint[i][j].y = normface[i][j - 1][1].y + normface[i][j][0].y + normface[i][j][1].y + normface[i - 1][j - 1][0].y + normface[i - 1][j - 1][1].y + normface[i - 1][j][0].y;
				normEveryPoint[i][j].z = normface[i][j - 1][1].z + normface[i][j][0].z + normface[i][j][1].z + normface[i - 1][j - 1][0].z + normface[i - 1][j - 1][1].z + normface[i - 1][j][0].z;
			}
	  //��һ��
			GLfloat L = sqrt(normEveryPoint[i][j].x*normEveryPoint[i][j].x + normEveryPoint[i][j].y*normEveryPoint[i][j].y + normEveryPoint[i][j].z*normEveryPoint[i][j].z);
			normEveryPoint[i][j].x = normEveryPoint[i][j].x / L;
			normEveryPoint[i][j].y = normEveryPoint[i][j].y / L;
			normEveryPoint[i][j].z = normEveryPoint[i][j].z / L;
		}
	}
}

void init(void)
{

	GLfloat position0[] = { 1.0, 1.0, 12.0, 1.0 };

	GLfloat ambient0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular0[] = { 1.0, 1.0,1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, position0);  //ָ����Դ��λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);  //ָ��������
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);  //ָ���������
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);  //ָ�����淴���y

	glEnable(GL_LIGHT0);	//ʹһյ����Ч
	glEnable(GL_LIGHTING);  //��������
	glEnable(GL_DEPTH_TEST); //��Ȳ���

	GLfloat ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);  //���ڹ��ռ���ĵ�ǰ��������    ���ʵĻ�����ɫ  ΢����ɫ
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);	//���ʵ�ɢ����ɫ		��ɫ
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);	//���ʵľ��淴����ɫ	��ɫ
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);   //��ɫƽ������
	
	
	drawPoint(100.0, 100.0);  //�������鸳ֵ
	normSurface();  //�����淨��
	normPoint();	//���㶥�㷨��

}

void display(void)
{ 
	glClearColor(0.3, 0.7, 0.5, 0);   //����ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //������ɫ����Ȼ���     

														 // ����͸��Ч����ͼ      
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(5.0f, 1.0f, 1.0f, 20.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(x_trens, y_trens, z_trens);
	glRotatef(angle, 0, 1, 0);
	glRotatef(angle_1, 1, 0, 0);
	glRotatef(angle_2, 0, 0, 1);
	//��ȡ�������
	GLuint image = load_texture("red.jpg");
	glEnable(GL_TEXTURE_2D); /* enable texture mapping */
	glBindTexture(GL_TEXTURE_2D, image); /* bind to our texture, has id of 1 */
	for (int j = 0; j < 99; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 100; i++) {
			//��i����
			glNormal3f(normEveryPoint[j + 1][i].x, normEveryPoint[j + 1][i].y, normEveryPoint[j + 1][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j + 1][i][1] + 0.5) / 1.0);
			glVertex3f(point[j+1][i][0], point[j + 1][i][1], point[j+1][i][2]);  //�ϱߵ�

			glNormal3f(normEveryPoint[j][i].x, normEveryPoint[j][i].y, normEveryPoint[j][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j][i][1] + 0.5) / 1.0);
			glVertex3f(point[j][i][0], point[j][i][1], point[j][i][2]);  //�±ߵ�
		}
		glEnd();
	}
	


	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(point[0][0][0], point[0][0][1], point[0][0][2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(point[0][99][0], point[0][99][1], point[0][99][2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(point[99][99][0], point[99][99][1], point[99][99][2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(point[99][0][0], point[99][0][1], point[99][0][2]);
	*/
	/*for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 99; j++) {
			glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(point[i][j][0], point[i][j][1], point[i][j][2]);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(point[i + 1][j][0], point[i + 1][j][1], point[i + 1][j][2]);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(point[i + 1][j + 1][0], point[i + 1][j + 1][1], point[i + 1][j + 1][2]);
			glEnd();
			glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(point[i][j][0], point[i][j][1], point[i][j][2]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(point[i][j + 1][0], point[i][j + 1][1], point[i][j + 1][2]);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(point[i + 1][j + 1][0], point[i + 1][j + 1][1], point[i + 1][j + 1][2]);
			glEnd();	
		}
	}
	*/
	glDisable(GL_TEXTURE_2D); /* disable texture mapping */
	glFlush();  //������ʾ
}

void keyboard(unsigned char key, int x, int y)    //���̿�����ת���ƶ�
{
	cout << "����1" << endl;
	switch (key) {
	//case 27:        // When Escape Is Pressed...
	//	exit(0);    // Exit The Program
					//ƽ��
	case 'd':
		x_trens += 0.05, glutPostRedisplay(); break;
	case 'a':
		x_trens -= 0.05, glutPostRedisplay(); break;
		//����
	case 'w':
		y_trens += 0.05, glutPostRedisplay(); break;
	case 's':
		y_trens -= 0.05, glutPostRedisplay(); break;
	case 'q':
		z_trens *= 0.5, glutPostRedisplay(); break;
	case 'e':
		z_trens /= 0.5, glutPostRedisplay(); break;
	case 'j':
		angle += 2, glutPostRedisplay(); break;
	case 'l':
		angle -= 2, glutPostRedisplay(); break;
	case 'i':
		angle_1 += 2, glutPostRedisplay(); break;
	case 'k':
		angle_1 -= 2, glutPostRedisplay(); break;
	case 'u':
		angle_2 += 2, glutPostRedisplay(); break;
	case 'o':
		angle_2 -= 2, glutPostRedisplay(); break;
		//
		// Ready For Next Case
	default:        // Now Wrap It Up
		break;
	}
}

int main(int argc, char** argv)
{


	glutInit(&argc, argv);   //��ʼ��glut�⣬���ʧ�ܣ������˳�
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //��ʼ����ʾģʽ��������������˫���沢����RGB����ɫģʽ

	glutInitWindowPosition(500, 300);  //��ʼ�����ڵ�λ�ã������Ͻ�Ϊԭ�㡢����Ϊx������������Ϊy�᷽�� 

	glutInitWindowSize(700, 700);//����һ����СΪ500*500�����أ��Ĵ���

	glutCreateWindow("�������");	//����һ�����ڣ����趨���ڵı���Ϊ��  ��
	init();
	glutDisplayFunc(display);//����һ����������Ҫ���л�ͼʱ�͵����������  �������ǰ�������Ҫ���Ƶ�ͼ�ζ��������������
	glutKeyboardFunc(keyboard);
	glutMainLoop();//����GLUT�¼�����ѭ���������е��롰�¼����йصĺ�����������ѭ��
	return 0;
}




