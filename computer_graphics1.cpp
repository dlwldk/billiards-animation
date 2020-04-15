#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

struct vector {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat s = 0;
};

struct point {
	GLfloat x = 0;
	GLfloat y = 0;
};

vector redv, whitev; //적구, 수구 벡터
point redp, whitep; //적구, 수구 위치

void RedCircle(float cx, float cy, float r) //적구, cx, cy: 원의 중심점
{
	float x1, y1, x2, y2;
	float angle;
	double radius = r;

	x1 = cx, y1 = cy;
	glColor3f(1.0, 0.0, 0.0); //빨간색

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle <= 361.0f; angle += 0.1)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

void WhiteCircle(float cx, float cy, float r) //수구
{
	float x1, y1, x2, y2;
	float angle;
	double radius = r;

	x1 = cx, y1 = cy;
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

}

void move() {
	redp.x += redv.x * 0.01;
	redp.y += redv.y * 0.01;
	whitep.x += whitev.x * 0.01;
	whitep.y += whitev.y * 0.01;

	//화면밖을 벗어났을 때
	if ((redv.x < 0 && redp.x < -0.9) || (redv.x > 0 && redp.x > 0.9)) {
		redv.x = -redv.x; // 원이 화면 범위를 벗어나게 되면 -1를 곱해서 방향 전환
	}
	if ((redv.y < 0 && redp.y < -1.9) || (redv.y > 0 && redp.y > 1.9)) { // 화면이 300*600 이기때문에 y의 범위는 -2~2 이다.
		redv.y = -redv.y;
	}
	if ((whitev.x < 0 && whitep.x < -0.9) || (whitev.x > 0 && whitep.x > 0.9)) {
		whitev.x = -whitev.x; // 원이 화면 범위를 벗어나게 되면 -1를 곱해서 방향 전환
	}
	if ((whitev.y < 0 && whitep.y < -1.9) || (whitev.y > 0 && whitep.y > 1.9)) { // 화면이 300*600 이기때문에 y의 범위는 -2~2 이다.
		whitev.y = -whitev.y;
	}
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	RedCircle(redp.x, redp.y, 0.1); //적구
	WhiteCircle(whitep.x, whitep.y, 0.1); //수구

	GLfloat originR = sqrt(pow(redv.x, 2) + pow(redv.y, 2));
	GLfloat originW = sqrt(pow(whitev.x, 2) + pow(whitev.y, 2));

	redp.x += originR != 0 ? (redv.x * redv.s) / originR : 0;
	redp.y += originR != 0 ? (redv.y * redv.s) / originR : 0;
	whitep.x += originW != 0 ? (whitev.x * whitev.s) / originW : 0;
	whitep.y += originW != 0 ? (whitev.y * whitev.s) / originW : 0;

	redv.s -= (redv.s <= 0 ? 0 : 0.0003); //마찰력때문에 속도 감소
	whitev.s -= (whitev.s <= 0 ? 0 : 0.0005); //마찰력때문에 속도 감소

	//충돌 했을 때
	if (sqrt(pow((redp.x - whitep.x), 2) + pow((redp.y - whitep.y), 2)) <= 0.2) {
		GLfloat cha1 = -60.0 * (3.1415926539 / 180);
		GLfloat cha2 = 30.0 * (3.1415926539 / 180);
		redv.x = whitev.x;
		redv.y = whitev.y;
		whitev.x = cos(cha1) * whitev.y - sin(cha1) * whitev.y;
		whitev.y = sin(cha1) * whitev.y + cos(cha1) * whitev.y;
		redv.x = cos(cha2) * redv.y - sin(cha2) * redv.y;
		redv.y = sin(cha2) * redv.y + cos(cha2) * redv.y;
		redv.s = whitev.s;
	}
	move();
	glutSwapBuffers();
}

void MyTimer(int Value) {
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {
	/*수구, 적구 초기 위치*/
	redp.x = 0, redp.y = 1;
	whitep.x = 0, whitep.y = -1;

	/*수구, 적구 초기 벡터*/
	whitev.x = 0, whitev.y = 0.06;
	whitev.s = 0.07;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(300, 600); // 당구대의 크기와 비슷하게 크기 조정
	glutInitWindowPosition(100, 100);
	glutCreateWindow("당구 animnation");
	glClearColor(0.2, 0.7, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -2.0, 2.0, 1.0, -1.0); // 화면이 1:2 비율이기 때문에, bottom 과 top 값을 두배로 늘려줘야함
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}
