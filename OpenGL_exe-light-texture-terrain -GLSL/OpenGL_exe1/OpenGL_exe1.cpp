
//Phong GLSL
//

#include "StdAfx.h"
#include <stdlib.h>
#include<GL\glew.h>
#include <GL/glut.h>
#include "textfile.h"
#include<opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<cmath>
#include<Windows.h>
#include<iostream>

using namespace cv;
using namespace std;

#pragma comment(lib,"glew32.lib")  


GLfloat roate = 0.0;
GLfloat rote = 0.0;
GLfloat anglex = 0.0;
GLfloat angley = 0.0;
GLfloat anglez = 0.0;
GLint WinW = 400;
GLint WinH = 400;
GLfloat oldx;
GLfloat oldy;

GLfloat angle = 0;
GLfloat angle_1 = 0;
GLfloat angle_2 = 0;
GLfloat x_trens = 0;
GLfloat y_trens = 0;
GLfloat z_trens = 0;
GLfloat point[100][100][3] = { 0.0 };   

struct vectPoint
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct vectPoint normface[100][100][2];
struct vectPoint va, vb, vc, vd;
struct vectPoint normEveryPoint[100][100];	
GLfloat multiple = 10.0f;

GLhandleARB p, f, v;
///输出着色器日志
void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}
///输出程序日志
void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}
///设置着色
void setShaders()
{

	char *vs, *fs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shader/five.vert");
	fs = textFileRead("shader/five.frag");

	const char *vv = vs;
	const char *ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShader(v);
	printShaderInfoLog(v);
	glCompileShader(f);
	printShaderInfoLog(f);

	p = glCreateProgram();

	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	printProgramInfoLog(p);
	glUseProgram(p);
}
///生成顶点数据
void drawPoint(GLfloat x, GLfloat y) {//x,yÊÇ100*100Òª³ýÒÔµÄ±ÈÀýÏµÊý

	GLfloat up;  //ÏòÉÏ
	GLfloat right; //ÏòÓÒ
	GLfloat z;
	Mat image = imread("jpg/point.jpg", 0);
	for (int j = 0, up = -50.0f; j< 100; j++, up++)
	{
		for (int i = 0, right = -50.0f; i < 100; i++, right++) {
			
			point[j][i][0] = right / 100.0;
			point[j][i][1] = up / 100.0;
			point[j][i][2] = image.at<uchar>(j, i)/255.0;
		}
	}
}
///检测图像大小是否符合2的整数次幂
int checkSize(int x)
{
	if (x == 2 || x == 4 ||
		x == 8 || x == 16 ||
		x == 32 || x == 64 ||
		x == 128 || x == 256 || x == 512)
		return 1;
	else return 0;
}
///加载纹理
GLuint load_texture(char *name)
{
	//OpenCV¶ÁÈ¡Í¼Ïñ
	Mat image = imread(name);
	
	int width = image.cols;
	int height = image.rows;

	/*make sure dimension is a power of 2 **/
	if (!checkSize(height) || !checkSize(width))
		return 0;
	/*no image data*/
	if (!image.data)
		return 0;
	//imshow("纹理图",image);
	GLubyte* imageData;
	
	GLuint texture_ID;

	int pixellength = width*height * 3;
	imageData = new GLubyte[pixellength];

	memcpy(imageData, image.data, pixellength * sizeof(uchar));


	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageData);

	free(imageData);
	return texture_ID;
}
///计算面的法线
void normSurface() { 
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

			for (int k = 0; k < 2; k++) {
				GLfloat L = sqrt(normface[i][j][k].x*normface[i][j][k].x+ normface[i][j][k].y*normface[i][j][k].y+ normface[i][j][k].z*normface[i][j][k].z);
				normface[i][j][k].x = normface[i][j][k].x / L;
				normface[i][j][k].y = normface[i][j][k].y / L;
				normface[i][j][k].z = normface[i][j][k].z / L;
			}
		}
	}
}
///计算顶点的法线
void normPoint() { 

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
	  //¹éÒ»»¯
			GLfloat L = sqrt(normEveryPoint[i][j].x*normEveryPoint[i][j].x + normEveryPoint[i][j].y*normEveryPoint[i][j].y + normEveryPoint[i][j].z*normEveryPoint[i][j].z);
			normEveryPoint[i][j].x = normEveryPoint[i][j].x / L;
			normEveryPoint[i][j].y = normEveryPoint[i][j].y / L;
			normEveryPoint[i][j].z = normEveryPoint[i][j].z / L;
		}
	}
}
///初始化
void init(void)
{
	

	glewInit();
	setShaders();

	GLfloat position0[] = { 1.0, 1.0, 12.0, 1.0 };
	GLfloat ambient0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular0[] = { 1.0, 1.0,1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, position0);  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0); 

	//glEnable(GL_LIGHT0);	
	////glEnable(GL_LIGHTING);  
	////glEnable(GL_DEPTH_TEST);

	GLfloat ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);	
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	/*GLint baseMapLoc = glGetUniformLocation(p, "baseMap");
	glUniform1i(baseMapLoc, 1);   */              //设置纹理  
	//glShadeModel(GL_FLAT); 
	//glShadeModel(GL_SMOOTH);   
	//GLint lightPosLoc = glGetUniformLocation(p, "fvLightPosition");
	//float lightPos[] = { 1.0, 1.0, 10.0 };      //光源位置
	//glUniform3fv(lightPosLoc, 1, lightPos);

	GLint eyePosLoc = glGetUniformLocation(p, "eyePosition");
	float eyePos[] = { 0.0, 0.0, 20.0 };        //视点位置
	glUniform3fv(eyePosLoc, 1, eyePos);

	//GLint specularLoc = glGetUniformLocation(p, "fvSpecular");
	//float specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射颜色：白色
	//glUniform4fv(specularLoc, 1, specular);

	//GLint diffuseLoc = glGetUniformLocation(p, "fvDiffuse");
	//float diffuse[] = { 0.6, 0.48, 0.18, 1.0 }; //散射颜色：棕色
	//glUniform4fv(diffuseLoc, 1, diffuse);

	//GLint spePowLoc = glGetUniformLocation(p, "fSpecularPower");
	//glUniform1f(spePowLoc, 50.0);               //镜面反射指数

	

}
///显示
void display(void){ 

	glClearColor(0.3, 0.7, 0.5, 0);   

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //ÇåÀíÑÕÉ«ºÍÉî¶È»º´æ     

	drawPoint(100.0, 100.0);
	normSurface();
	normPoint();
														 // ´´½¨Í¸ÊÓÐ§¹ûÊÓÍ¼      
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

	glRotatef(rote, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);
	rote += roate;

	//glActiveTexture(GL_TEXTURE0);
	GLuint image = load_texture("jpg/red.jpg");
	glBindTexture(GL_TEXTURE_2D, image); 

	//glEnable(GL_TEXTURE_2D); /* enable texture mapping */
	for (int j = 0; j < 99; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 100; i++) {
			//µÚi¸ö´ø
			glNormal3f(normEveryPoint[j + 1][i].x, normEveryPoint[j + 1][i].y, normEveryPoint[j + 1][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j + 1][i][1] + 0.5) / 1.0);
			glVertex3f(point[j+1][i][0], point[j + 1][i][1], point[j+1][i][2]);  //上边点

			glNormal3f(normEveryPoint[j][i].x, normEveryPoint[j][i].y, normEveryPoint[j][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j][i][1] + 0.5) / 1.0);
			glVertex3f(point[j][i][0], point[j][i][1], point[j][i][2]);  //下边点
		}
		glEnd();
	}
	//glDisable(GL_TEXTURE_2D); /* disable texture mapping */
	glFlush(); 
}
///键盘响应函数
void keyboard(unsigned char key, int x, int y)    //¼üÅÌ¿ØÖÆÐý×ªºÍÒÆ¶¯
{
	switch (key) {
	case 27:        // When Escape Is Pressed...
		exit(0);    // Exit The Program
					//Æ½ÒÆ
	case 'd':
		x_trens += 0.05, glutPostRedisplay(); break;
	case 'a':
		x_trens -= 0.05, glutPostRedisplay(); break;
		//Ëõ·Å
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
	default:        // Now Wrap It Up
		break;
	}
}
//重绘
void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
///鼠标响应函数
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate = 0;
			rote = 0;
			oldx = x;
			oldy = y;
			cout << "left" << endl;
		}

	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate += 1.0f;
			cout << "right" << endl;
		}

	}

}
///检测姿势
void motion(int x, int y)
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	anglex += 360 * (GLfloat)deltax / (GLfloat)WinW;
	angley += 360 * (GLfloat)deltay / (GLfloat)WinH;
	anglez += 360 * (GLfloat)deltay / (GLfloat)WinH;
	oldx = x;
	oldy = y;
	glutPostRedisplay();
	glutPostRedisplay();
}
//主函数
int main(int argc, char** argv)
{


	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

	glutInitWindowPosition(500, 300);  

	glutInitWindowSize(700, 700);

	glutCreateWindow("GLSL地形");	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}




