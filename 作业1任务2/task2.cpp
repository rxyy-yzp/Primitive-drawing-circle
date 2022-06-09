#include"iostream"
#include"math.h"
#include"GL/glut.h"
#include"stdlib.h"
using namespace std;

#define PI acos(-1)//定义pi的大小
#define WINDOW_WIDTH 600 //窗口宽度
#define GRID_WIDTH 20//每个栅格的宽度

enum Color//颜色的枚举
{
	red, green, blue, cyan, yellow
};

//栅格绘制
void table(int tableWidth) 
{
	glBegin(GL_LINES);//绘制列线
	for (int i = 0 - tableWidth / 2; i <= tableWidth / 2; i += GRID_WIDTH) 
	{
		glVertex2i(i, -tableWidth / 2);
		glVertex2i(i, tableWidth / 2);
	}
	glEnd();//结束绘图，对应glBegin()
	glBegin(GL_LINES);//绘制行线
	for (int i = 0 - tableWidth / 2; i <= tableWidth / 2; i += GRID_WIDTH) 
	{
		glVertex2i(-tableWidth / 2, i);
		glVertex2i(tableWidth / 2, i);
	}
	glEnd();//结束绘图，对应glBegin()
}

//坐标轴箭头绘制
void arrow(int radiusMin) 
{
	GLint a1 = radiusMin - GRID_WIDTH;
	GLint b1 = 0;
	GLint a2 = a1 - GRID_WIDTH / 2;
	GLint b2 = GRID_WIDTH / 2;
	glBegin(GL_LINE_STRIP);//绘制x轴箭头
	glVertex2i(a2, -b2);
	glVertex2i(a1, b1);
	glVertex2i(a2, b2);
	glEnd();//结束绘图，对应glBegin()
	glBegin(GL_LINE_STRIP);//绘制y轴箭头
	glVertex2i(b2, a2);
	glVertex2i(b1, a1);
	glVertex2i(-b2, a2);
	glEnd();//结束绘图，对应glBegin()
}

//像素点绘制
void drawPoint(int x, int y, Color value) 
{
	switch (value) //选取颜色
	{
	case red:
		glColor3f(1.0f, 0.0f, 0.0f);//设置红色
		break;
	case green:
		glColor3f(0.0f, 1.0f, 0.0f);//设置绿色
		break;
	case blue:
		glColor3f(0.0f, 0.0f, 1.0f);//设置蓝色
		break;
	case cyan:
		glColor3f(0.0f, 1.0f, 1.0f);//设置淡蓝色
		break;
	case yellow:
		glColor3f(1.0f, 1.0f, 0.0f);//设置黄色
		break;
	}
	glBegin(GL_POINTS);//开始绘制，对应glEnd()
	glVertex2i(x, y);//绘制像素点
	glEnd();//结束绘图，对应glBegin()
}

//圆逼近点绘制，使用中点圆整数优化算法
void CirclePoints(int radius, int xpoint, int ypoint, Color value)
{ //xpoint,ypoint为逼近点所在位置
	int x = xpoint, y = radius + ypoint;
	int d = (int)1.25 - radius;
	int dt = 3;
	int db = -2 * radius + 5;
	while (y - ypoint >= x - xpoint) {
		if (y - ypoint > x - xpoint) {
			drawPoint(x, y, value);
			drawPoint(y, x, value);
			drawPoint(-y, -x, value);
			drawPoint(-y, x, value);
			drawPoint(y, -x, value);
			drawPoint(x, -y, value);
			drawPoint(-x, y, value);
			drawPoint(-x, -y, value);
		}
		else if (x - xpoint == 0) {
			drawPoint(x, y, value);
			drawPoint(x, -y, value);
			drawPoint(-y, x, value);
			drawPoint(y, -x, value);
		}
		else if (x - xpoint == y - ypoint) {
			drawPoint(x, y, value);
			drawPoint(x, -y, value);
			drawPoint(-x, y, value);
			drawPoint(-x, -y, value);
		}
		if (d < 0) { //选择T
			d += dt;
			dt += 2;
			db += 2;
		}
		else {//选择B
			d += db;
			dt += 2;
			db += 4;
			y--;
		}
		x++;
	}
}

//中点圆整数算法
void MidPointCircleInt(int radius, int gridWidth, Color value) 
{
	int x = 0, y = radius;
	int d = (int)1.25 * gridWidth * gridWidth - radius * gridWidth;
	while (y >= x) 
	{
		if (y > x) 
		{
			CirclePoints(GRID_WIDTH / 5, x, y, value);
			CirclePoints(GRID_WIDTH / 5, y, x, value);
			CirclePoints(GRID_WIDTH / 5, -y, -x, value);
			CirclePoints(GRID_WIDTH / 5, -y, x, value);
			CirclePoints(GRID_WIDTH / 5, y, -x, value);
			CirclePoints(GRID_WIDTH / 5, x, -y, value);
			CirclePoints(GRID_WIDTH / 5, -x, y, value);
			CirclePoints(GRID_WIDTH / 5, -x, -y, value);
		}
		else if (x == 0)
		{
			CirclePoints(GRID_WIDTH / 5, x, y, value);
			CirclePoints(GRID_WIDTH / 5, x, -y, value);
			CirclePoints(GRID_WIDTH / 5, -y, x, value);
			CirclePoints(GRID_WIDTH / 5, y, -x, value);
		}
		else if (x == y) 
		{
			CirclePoints(GRID_WIDTH / 5, x, y, value);
			CirclePoints(GRID_WIDTH / 5, x, -y, value);
			CirclePoints(GRID_WIDTH / 5, -x, y, value);
			CirclePoints(GRID_WIDTH / 5, -x, -y, value);
		}
		if (d < 0) //选择T
			d += 2 * x * gridWidth + 3 * gridWidth * gridWidth;
		else //选择B
		{
			d += 2 * (x - y) * gridWidth + 5 * gridWidth * gridWidth;
			y -= gridWidth;
		}
		x += gridWidth;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);//把窗口清除为当前颜色
	int tableWidth = WINDOW_WIDTH - WINDOW_WIDTH % (2 * GRID_WIDTH);//正方形栅格总宽度
	int radiusMin = (tableWidth - tableWidth % (10 * GRID_WIDTH)) / 10;//最小圆的半径
	glColor3f(0.4f, 0.4f, 0.4f);//设置颜色,此颜色用于栅格绘制
	table(tableWidth);//栅格绘制
	glColor3f(1, 1, 0);//设置颜色,此颜色用于坐标轴箭头绘制
	arrow(radiusMin);//坐标轴箭头绘制
	MidPointCircleInt(radiusMin, GRID_WIDTH, red);//(2)逼近圆1
	CirclePoints(radiusMin, 0, 0, red);//(1)圆1
	MidPointCircleInt(radiusMin * 2, GRID_WIDTH, green);//(2)逼近圆2
	CirclePoints(radiusMin * 2, 0, 0, green);//(1)圆2
	MidPointCircleInt(radiusMin * 3, GRID_WIDTH, blue);//(2)逼近圆3
	CirclePoints(radiusMin * 3, 0, 0, blue);//(1)圆3
	MidPointCircleInt(radiusMin * 4, GRID_WIDTH, cyan);//(2)逼近圆4
	CirclePoints(radiusMin * 4, 0, 0, cyan);//(1)圆4
	MidPointCircleInt(radiusMin * 5, GRID_WIDTH, yellow);//(2)逼近圆5
	CirclePoints(radiusMin * 5, 0, 0, yellow);//(1)圆5
	glFlush();//强制刷新缓冲，保证绘图命执行
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//设置图形显示模式
	glutInitWindowPosition(350, 50);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);//设置窗口大小
	glutCreateWindow("中点圆光栅化算法");//创建窗口
	glClearColor(0.0, 0.0, 0.0, 0.0);//清除颜色缓存
	glMatrixMode(GL_PROJECTION);//增加透视
	glLoadIdentity();//恢复初始坐标系
	gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_WIDTH / 2, WINDOW_WIDTH / 2);//设置显示区域
	glutDisplayFunc(display);//设置一个函数
	glutMainLoop();//进入GLUT事件处理循环
	system("pause");
	return 0;
}