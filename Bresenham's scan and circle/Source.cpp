//created by Levin Yu at Oct/05/2017
#include <stdlib.h>
#include <stdio.h>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600


void draw_circle(void);
void draw_many_circles(void);
void draw_moving_circles(void);
void myinit(void);
void file_in(void);
vector<int> convert_coodinate(int, int);

int x;
int y;
int r;
int num;
vector<int> circles;
float scale = WINDOW_WIDTH / 2;
int K = 2000;
int K_step = K;
bool moving = false;


/*-----------------
The main function
------------------*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	/* Use both double buffering and Z buffer */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(XOFF, YOFF);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS6533/CS4533 Assignment 1");
	file_in();

	/* Function call to handle file input here */
	printf("choose a section form \"c, d, e\" and press enter ");
	char choice = getchar();


	if (choice == 'c')
	{
		printf("input x coodinate ");
		cin >> x;
		printf("input y coodinate ");
		cin >> y;
		printf("input ridius ");
		cin >> r;
		glutDisplayFunc(draw_circle);
	}
	 if (choice == 'd')
	{
		glutDisplayFunc(draw_many_circles);

	}
	 if (choice == 'e')
	 {
		moving = true;
		printf("set a integer K and press enter (ideally 5000) ");
		cin >> K;
		glutDisplayFunc(draw_moving_circles);

	 }

	myinit();
	glutMainLoop();

	return 0;
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(void)
{
	fstream fin("input_circles.txt");//read data in the working dictionary

	if (fin)
	{
		int a, b, c;
		fin >> num;
		while (fin >> a >> b >> c)
		{

			vector<int> window = convert_coodinate(a, b);
			circles.push_back(window[0]);
			circles.push_back(window[1]);
			circles.push_back(c);
			int size = max(max(abs(a + c), abs(b + c)), max(abs(a - c), abs(b - c)));
			if ( size > scale)
			{
				scale = size;//scaling
			}
		}

		scale = scale / (WINDOW_WIDTH / 2);
		fin.close();
	}
	else
	{
		cerr << "fail" << endl;
	}
}


/*---------------------------------------------------------------------
display(): This function is called once for _every_ frame.
---------------------------------------------------------------------*/
void draw_circle(void)
{
	glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
	glPointSize(1.0);                     /* size of each point */

	int D = 0;
	glBegin(GL_POINTS);
	glVertex2i(x, y + r);
	glVertex2i(x, y - r);
	glVertex2i(x + r, y);
	glVertex2i(x - r, y);

	int i = 1;
	int j = r;

	while(i <= r/sqrt(2))
		//8 points

	{		
		if (D <= 0)
		{
			glVertex2i(x + i, y + j);
			glVertex2i(x - i, y + j);
			glVertex2i(x + j, y + i);
			glVertex2i(x - j, y + i);
			glVertex2i(x -i, y - j);
			glVertex2i(x + i, y - j);
			glVertex2i(x - j, y - i);
			glVertex2i(x + j, y - i);
			D = D + 2 * i + 1;
		}
		else {
			glBegin(GL_POINTS);
			glVertex2i(x + i, y + j - 1);
			glVertex2i(x - i, y + j - 1);
			glVertex2i(x + j - 1, y + i);
			glVertex2i(x - j + 1, y + i);
			glVertex2i(x - i, y - j + 1);
			glVertex2i(x + i, y - j + 1);
			glVertex2i(x - j + 1, y - i);
			glVertex2i(x + j - 1, y - i);

			j = j - 1;
			D = D + 2 * i - 2 * j + 1;
		}	
		i += 1; 
	}
	glEnd();
	glFlush();
	
	if (!moving)
	{
		glutSwapBuffers();                    /* swap buffers */
	}
	
	
}

void draw_many_circles(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < num * 3; i = i + 3)
	{		
		if (!moving)
		{
			glutSwapBuffers();
		}
		x = (circles[i] - WINDOW_WIDTH / 2) / scale + WINDOW_WIDTH / 2;
		y = (circles[i + 1] - WINDOW_WIDTH / 2) / scale + WINDOW_WIDTH / 2;
		r = int ((circles[i + 2] / scale) * K_step / K );
		draw_circle();

	}
}

void draw_moving_circles(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < K; i = i + 1)
	{
		K_step = i +1 ;
		
		if (i == K - 1)
		{
			i = 0;
		}
		draw_many_circles();
		glutSwapBuffers();
	}
}

vector<int> convert_coodinate(int x, int y)
{
	return { x + WINDOW_WIDTH / 2, y + WINDOW_HEIGHT / 2 };

}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

										  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}
