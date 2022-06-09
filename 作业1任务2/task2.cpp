#include"iostream"
#include"math.h"
#include"GL/glut.h"
#include"stdlib.h"
using namespace std;

#define PI acos(-1)//����pi�Ĵ�С
#define WINDOW_WIDTH 600 //���ڿ��
#define GRID_WIDTH 20//ÿ��դ��Ŀ��

enum Color//��ɫ��ö��
{
	red, green, blue, cyan, yellow
};

//դ�����
void table(int tableWidth) 
{
	glBegin(GL_LINES);//��������
	for (int i = 0 - tableWidth / 2; i <= tableWidth / 2; i += GRID_WIDTH) 
	{
		glVertex2i(i, -tableWidth / 2);
		glVertex2i(i, tableWidth / 2);
	}
	glEnd();//������ͼ����ӦglBegin()
	glBegin(GL_LINES);//��������
	for (int i = 0 - tableWidth / 2; i <= tableWidth / 2; i += GRID_WIDTH) 
	{
		glVertex2i(-tableWidth / 2, i);
		glVertex2i(tableWidth / 2, i);
	}
	glEnd();//������ͼ����ӦglBegin()
}

//�������ͷ����
void arrow(int radiusMin) 
{
	GLint a1 = radiusMin - GRID_WIDTH;
	GLint b1 = 0;
	GLint a2 = a1 - GRID_WIDTH / 2;
	GLint b2 = GRID_WIDTH / 2;
	glBegin(GL_LINE_STRIP);//����x���ͷ
	glVertex2i(a2, -b2);
	glVertex2i(a1, b1);
	glVertex2i(a2, b2);
	glEnd();//������ͼ����ӦglBegin()
	glBegin(GL_LINE_STRIP);//����y���ͷ
	glVertex2i(b2, a2);
	glVertex2i(b1, a1);
	glVertex2i(-b2, a2);
	glEnd();//������ͼ����ӦglBegin()
}

//���ص����
void drawPoint(int x, int y, Color value) 
{
	switch (value) //ѡȡ��ɫ
	{
	case red:
		glColor3f(1.0f, 0.0f, 0.0f);//���ú�ɫ
		break;
	case green:
		glColor3f(0.0f, 1.0f, 0.0f);//������ɫ
		break;
	case blue:
		glColor3f(0.0f, 0.0f, 1.0f);//������ɫ
		break;
	case cyan:
		glColor3f(0.0f, 1.0f, 1.0f);//���õ���ɫ
		break;
	case yellow:
		glColor3f(1.0f, 1.0f, 0.0f);//���û�ɫ
		break;
	}
	glBegin(GL_POINTS);//��ʼ���ƣ���ӦglEnd()
	glVertex2i(x, y);//�������ص�
	glEnd();//������ͼ����ӦglBegin()
}

//Բ�ƽ�����ƣ�ʹ���е�Բ�����Ż��㷨
void CirclePoints(int radius, int xpoint, int ypoint, Color value)
{ //xpoint,ypointΪ�ƽ�������λ��
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
		if (d < 0) { //ѡ��T
			d += dt;
			dt += 2;
			db += 2;
		}
		else {//ѡ��B
			d += db;
			dt += 2;
			db += 4;
			y--;
		}
		x++;
	}
}

//�е�Բ�����㷨
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
		if (d < 0) //ѡ��T
			d += 2 * x * gridWidth + 3 * gridWidth * gridWidth;
		else //ѡ��B
		{
			d += 2 * (x - y) * gridWidth + 5 * gridWidth * gridWidth;
			y -= gridWidth;
		}
		x += gridWidth;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);//�Ѵ������Ϊ��ǰ��ɫ
	int tableWidth = WINDOW_WIDTH - WINDOW_WIDTH % (2 * GRID_WIDTH);//������դ���ܿ��
	int radiusMin = (tableWidth - tableWidth % (10 * GRID_WIDTH)) / 10;//��СԲ�İ뾶
	glColor3f(0.4f, 0.4f, 0.4f);//������ɫ,����ɫ����դ�����
	table(tableWidth);//դ�����
	glColor3f(1, 1, 0);//������ɫ,����ɫ�����������ͷ����
	arrow(radiusMin);//�������ͷ����
	MidPointCircleInt(radiusMin, GRID_WIDTH, red);//(2)�ƽ�Բ1
	CirclePoints(radiusMin, 0, 0, red);//(1)Բ1
	MidPointCircleInt(radiusMin * 2, GRID_WIDTH, green);//(2)�ƽ�Բ2
	CirclePoints(radiusMin * 2, 0, 0, green);//(1)Բ2
	MidPointCircleInt(radiusMin * 3, GRID_WIDTH, blue);//(2)�ƽ�Բ3
	CirclePoints(radiusMin * 3, 0, 0, blue);//(1)Բ3
	MidPointCircleInt(radiusMin * 4, GRID_WIDTH, cyan);//(2)�ƽ�Բ4
	CirclePoints(radiusMin * 4, 0, 0, cyan);//(1)Բ4
	MidPointCircleInt(radiusMin * 5, GRID_WIDTH, yellow);//(2)�ƽ�Բ5
	CirclePoints(radiusMin * 5, 0, 0, yellow);//(1)Բ5
	glFlush();//ǿ��ˢ�»��壬��֤��ͼ��ִ��
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//����ͼ����ʾģʽ
	glutInitWindowPosition(350, 50);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);//���ô��ڴ�С
	glutCreateWindow("�е�Բ��դ���㷨");//��������
	glClearColor(0.0, 0.0, 0.0, 0.0);//�����ɫ����
	glMatrixMode(GL_PROJECTION);//����͸��
	glLoadIdentity();//�ָ���ʼ����ϵ
	gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_WIDTH / 2, WINDOW_WIDTH / 2);//������ʾ����
	glutDisplayFunc(display);//����һ������
	glutMainLoop();//����GLUT�¼�����ѭ��
	system("pause");
	return 0;
}