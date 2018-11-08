#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <sstream>

#include <GL/glut.h>
using namespace std;

int iter = 0;
int Rating = 0;
float angle = 0;
int bestRating = 0;


float RandomFloat(float mn, float mx)
{
    float r = (float)rand() / (float)RAND_MAX;
    return mn + r * (mx - mn);
}

void OutText(float x, float y, float z, char *string){
    glRasterPos3f(x,y,z);
    for (char *c=string; *c!= '\0'; c++ ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


class Teapot
{
   public: float x;
   public: float y;
    float z;

    float r;
    float g;
    float b;

    float size;

    public:
    bool colliding;

    Teapot(){}

    Teapot(float _x, float _y, float _z, float _size)
    {
        x=_x;
        y=_y;
        z=_z;
        r=1.0f;
        g=1.0f;
        b=1.0f;
        size = _size;
        colliding = false;

    }

    void Show()
    {
            	switch (iter % 3) {
		case 0 : glColor3f(0,1,1); break;
		case 1 : glColor3f(1,0,1); break;
		case 2 : glColor3f(1,1,0); break;
	}
	iter++;

        glTranslatef(x,y,z);
        glRotatef(angle, 0, 1, 0);
        glutSolidTeapot(size);
        glRotatef(-angle, 0, 1, 0);
        glTranslatef(-x,-y,-z);

    }

    void Move(float _x, float _y, float _z)
    {
        x+=_x;
        y+=_y;
        z+=_z;
    }

    void SetColor(float _r, float _g, float _b)
    {
        r=_r;
        g=_g;
        b=_b;
    }

    bool CheckCollisions(float _x, float _y, float _z, float radius)
    {
        float distance = sqrt((x-_x)*(x-_x)+(y-_y)*(y-_y)+(z-_z)*(z-_z));
        colliding = distance < size + radius;
        return colliding;
    }
};

Teapot teapots[] =
{
    {0,0,0,0.25f},
    {0,0,0,0.25f},
    {0,0,0,0.25f},
};

float ratio;

void resize(int w,int h)
{
    if (h == 0) h = 1;
   ratio=w*1.0/h;
   glViewport(0,0,w,h);
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective(45.0, ratio, 0.1,10);
   gluLookAt( 0,0,5, 0,0,0, 0,1,0 );
   glMatrixMode( GL_MODELVIEW );
}

void init (void)
{
float pos[4] = {3,3,3,1};
float color[4] = {1,1,1,1};
float sp[4] = {0.5,0.5,0.5,0.5};
float mat_specular[] = {1,1,1,1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, sp);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
}

float korx = 0.4;
char* char_type;
float FallSpead = 0.015;

void GameOver (){

stringstream strs;
strs << bestRating;
string temp_str = strs.str();
char* char_type_best = (char*) temp_str.c_str();

glutIdleFunc(NULL);
glColor3f(1,1,1);
OutText(-0.5 , 1, 0, "GAME OVER");
OutText(-1, 0.75, 0, "Press F12 to restart");
OutText(0, 0.5, 0, "Your result :"); OutText(1, 0.5, 0, char_type);
OutText(0, 0, 0, "Best result :"); OutText(1, 0, 0, char_type_best);

}

void Restart();

void falldown(){

    for (int i = 0; i < 3; i ++) {

    if (teapots[0].y == teapots[1].y || teapots[1].y == teapots[2].y || teapots[2].y == teapots[0].y) Restart();
        if (teapots[i].CheckCollisions(korx, -2.0,0, 0.5)) {
            Rating++;
                    if (Rating % 10 == 0 ) FallSpead*= 1.2;
            teapots[i].Move(RandomFloat(-2.2,2.2), 6 , 0);
            if (teapots[i].x > 2.2) teapots[i].x -= 1.5;
            if (teapots[i].x < -2.2) teapots[i].x += 1.5;
        }

            teapots[i].y -= FallSpead;
            teapots[i].Show();

            angle += 0.2;
            angle++; if (angle >= 360) {angle = 0;}

        if (teapots[i].y < -2.5) GameOver();
    }

}


void display(void)
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
   glPushMatrix();

   stringstream strs;
  strs << Rating;
  string temp_str = strs.str();
   char_type = (char*) temp_str.c_str();
   glColor3f(1.0,1.0,1.0);
   OutText(-2.1,1.8,0, char_type);

    falldown();

   if (korx <= -ratio / 0.555555555555f) korx+=0.2;
   if (korx >= ratio / 0.555555555555f) korx-=0.2;


   	switch (Rating % 3) {
		case 0 : glColor3f(1,0,0); break;
		case 1 : glColor3f(0,1,0); break;
		case 2 : glColor3f(0,0,1); break;
	}
    glTranslatef(korx, -2.0, 0);
    glutSolidCube(1);
   glPopMatrix();

   glutSwapBuffers();
}

void Restart() {

    teapots[0].Move(0, 2, 0);
    teapots[1].Move(0, 4, 0);
    teapots[2].Move(0, 6, 0);


    glutIdleFunc(display);

    if (bestRating < Rating) bestRating = Rating;

    Rating = 0;
    FallSpead = 0.015;

}


void keyboard(unsigned char key, int x, int y)
{
    float moveX = 0.0f;
	switch (key) {
		case 'd': moveX=0.1; break;
		case 'a': moveX=-0.1; break;
	}
	korx += moveX;
}

void arrow_keys (int keys, int x, int y)
{
  switch (keys) {
    case GLUT_KEY_UP:
      glutFullScreen();
      break;
    case GLUT_KEY_DOWN:
      glutReshapeWindow(800,600);
      break;
    case GLUT_KEY_F12: Restart();
    default:
      break;
  }
}

int main(int argc, char **argv)
{

    teapots[0].Move(RandomFloat(-2.2,2.2), 2, 0);
    teapots[1].Move(RandomFloat(-2.2,2.2), 4, 0);
    teapots[2].Move(RandomFloat(-2.2,2.2), 6, 0);


	glutInit(&argc, argv);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow("OpenGL1 Window");
    init();
    glutIdleFunc(display);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);

    glutMainLoop();
}
