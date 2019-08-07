
// OpenGL_exe1.cpp : 定义控制台应用程序的入口点。
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


GLfloat point[100][100][3] = { 0.0 };   //100*100个顶点数组
struct vectPoint
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct vectPoint normface[100][100][2]; //面的法线
struct vectPoint va, vb, vc, vd;
struct vectPoint normEveryPoint[100][100];	//顶点法线
GLfloat multiple = 10.0f;

void drawPoint(GLfloat x, GLfloat y) {//x,y是100*100要除以的比例系数

	GLfloat up;  //向上
	GLfloat right; //向右
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
	//OpenCV读取图像
	Mat image = imread(name);
	
	int width = image.cols;
	int height = image.rows;

	/*make sure dimension is a power of 2 **/
	if (!checkSize(height) || !checkSize(width))
		return 0;
	/*no image data*/
	if (!image.data)
		return 0;
	imshow("纹理",image);
	//设置图像指针
	GLubyte* imageData;
	
	//OpenGL纹理用整型数表示
	GLuint texture_ID;

	//获取图像指针
	int pixellength = width*height * 3;
	imageData = new GLubyte[pixellength];
	//复制
	memcpy(imageData, image.data, pixellength * sizeof(uchar));

	//将texture_ID设置为2D纹理信息
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//纹理放大缩小使用线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//纹理水平竖直方向外扩使用重复贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹理水平竖直方向外扩使用边缘像素贴图(与重复贴图二选一)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//指定纹理贴图和材质混合的方式
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//将图像内存用作纹理信息
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageData);

	free(imageData);
	return texture_ID;
}


void normSurface() {   //求每个面的法线 
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

			//归一化
			for (int k = 0; k < 2; k++) {
				GLfloat L = sqrt(normface[i][j][k].x*normface[i][j][k].x+ normface[i][j][k].y*normface[i][j][k].y+ normface[i][j][k].z*normface[i][j][k].z);
				normface[i][j][k].x = normface[i][j][k].x / L;
				normface[i][j][k].y = normface[i][j][k].y / L;
				normface[i][j][k].z = normface[i][j][k].z / L;
			}
		}
	}
}

void normPoint() {  //计算每个顶点的法线

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
	  //归一化
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

	glLightfv(GL_LIGHT0, GL_POSITION, position0);  //指定光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);  //指定环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);  //指定漫反射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);  //指定镜面反射光y

	glEnable(GL_LIGHT0);	//使一盏灯有效
	glEnable(GL_LIGHTING);  //启动光照
	glEnable(GL_DEPTH_TEST); //深度测试

	GLfloat ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);  //用于光照计算的当前材质属性    材质的环境颜色  微弱白色
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);	//材质的散射颜色		黄色
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);	//材质的镜面反射颜色	白色
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);   //颜色平滑过渡
	
	
	drawPoint(100.0, 100.0);  //顶点数组赋值
	normSurface();  //计算面法线
	normPoint();	//计算顶点法线

}

void display(void)
{ 
	glClearColor(0.3, 0.7, 0.5, 0);   //背景色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存     

														 // 创建透视效果视图      
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
	//获取纹理对象
	GLuint image = load_texture("red.jpg");
	glEnable(GL_TEXTURE_2D); /* enable texture mapping */
	glBindTexture(GL_TEXTURE_2D, image); /* bind to our texture, has id of 1 */
	for (int j = 0; j < 99; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 100; i++) {
			//第i个带
			glNormal3f(normEveryPoint[j + 1][i].x, normEveryPoint[j + 1][i].y, normEveryPoint[j + 1][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j + 1][i][1] + 0.5) / 1.0);
			glVertex3f(point[j+1][i][0], point[j + 1][i][1], point[j+1][i][2]);  //上边点

			glNormal3f(normEveryPoint[j][i].x, normEveryPoint[j][i].y, normEveryPoint[j][i].z);
			glTexCoord2f((point[j][i][0] + 0.5) / 1.0, (point[j][i][1] + 0.5) / 1.0);
			glVertex3f(point[j][i][0], point[j][i][1], point[j][i][2]);  //下边点
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
	glFlush();  //快速显示
}

void keyboard(unsigned char key, int x, int y)    //键盘控制旋转和移动
{
	cout << "键盘1" << endl;
	switch (key) {
	//case 27:        // When Escape Is Pressed...
	//	exit(0);    // Exit The Program
					//平移
	case 'd':
		x_trens += 0.05, glutPostRedisplay(); break;
	case 'a':
		x_trens -= 0.05, glutPostRedisplay(); break;
		//缩放
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


	glutInit(&argc, argv);   //初始化glut库，如果失败，程序退出
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //初始化显示模式，这里声明采用双缓存并采用RGB的颜色模式

	glutInitWindowPosition(500, 300);  //初始化窗口的位置，以左上角为原点、向左为x轴正方向，向下为y轴方向 

	glutInitWindowSize(700, 700);//创建一个大小为500*500（像素）的窗口

	glutCreateWindow("纹理地形");	//创建一个窗口，并设定窗口的标题为“  ”
	init();
	glutDisplayFunc(display);//设置一个函数当需要进行画图时就调用这个函数  所以我们把所有需要绘制的图形都放在这个函数里
	glutKeyboardFunc(keyboard);
	glutMainLoop();//进入GLUT事件处理循环，让所有的与“事件”有关的函数调用无限循环
	return 0;
}




