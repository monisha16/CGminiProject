#include<stdio.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>
#define pi 3.14
int speed;
float theta=0,t=0;
short currentScene=0;
int clouds=0;
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius,float R, float G, float B,int type) // for semicircle type=2,for circle in only 1st quadrant type=4
{
    int i;
    int triangleAmount = 125; //# of triangles used to draw circle
    glColor3f(R,G,B);
    GLfloat twicePi = 2.0f * pi;

    glBegin(GL_TRIANGLE_FAN);
    glPushMatrix();
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount/type;i++)
    {
        glVertex2f(
            (x + (radius * cos(i * twicePi / triangleAmount))),
            (y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
    glPopMatrix();
    glutPostRedisplay();
    glFlush();
}
void keyboard(int key,int i,int j)
{
    switch (key) {
        case GLUT_KEY_LEFT:
            currentScene=1;
            break;
        case GLUT_KEY_RIGHT:
            currentScene = 0;
            break;
        default:
            break;
    }
}
void bg()
{
    glBegin(GL_QUADS);
    glColor3f(0.3,1,0.7);
    glVertex3f(0, 0,0);
    glVertex3f(1920, 0,0);
    glColor3f(1,1,1);
    glVertex3f(1920,1280,0);
    glVertex3f(0,1280,0);
    glEnd();
}

void bino(int n,int c[])    //coefficient for bezeir curve
{
    int i,j;
    for(i=0;i<=n;i++)
    {
        c[i]=1;
        for(j=n;j>=i+1;j--)
            c[i]=c[i]*j;
        for(j=n-i;j>=2;j--)
            c[i]=c[i]/j;
    }
}
void bezsnd(int c[],int snd[][2],int n) // sand curve
{
    float x,y,t,val;
    int i;
    glBegin(GL_POLYGON);
    glPushMatrix();
    for(t=0;t<1.0;t+=.01)
    {
        x=0;
        y=0;
        for(i=0;i<4;i++)
        {
            val=c[i]*pow(t,i)*pow(1-t,n-i);
            x=x+snd[i][0]*val;
            y=y+snd[i][1]*val;
        }
        glVertex2f(x,y);
    }
    glVertex2f(1920,0);
    glVertex2f(0,0);
    glVertex2f(0,200);
    glEnd();
    glPopMatrix();
}
void bez(int c[],int cp[][2],int n) // weed curve
{
    float x,y,t,val;
    int i;
    glBegin(GL_POINTS);
    glPushMatrix();
    for(t=0;t<1.0;t+=.01)
    {
        x=0;
        y=0;
        for(i=0;i<4;i++)
        {
            val=c[i]*pow(t,i)*pow(1-t,n-i);
            x=x+cp[i][0]*val;
            y=y+cp[i][1]*val;
        }
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();
}
void anamonie()         //all the  rocks
{
    //right side
    drawFilledCircle(2015,-25,200,.6,.6,.55,2);
    drawFilledCircle(2020,-30,200,.5,.5,.4,2);
    drawFilledCircle(1945,-105,200,.6,.6,.55,2);
    drawFilledCircle(1950,-110,200,.5,.5,.4,2);
    drawFilledCircle(1815,-55,100,.6,.6,.55,2);
    drawFilledCircle(1820,-60,100,.5,.5,.4,2);
    //left side
    drawFilledCircle(142,-50,120,.6,.6,.55,2);
    drawFilledCircle(135,-55,120,.5,.5,.4,2);
    drawFilledCircle(-5,-40,150,.6,.6,.55,2);
    drawFilledCircle(-10,-45,150,.5,.5,.4,2);
    drawFilledCircle(75,-90,150,.6,.6,.55,2);
    drawFilledCircle(68,-93,150,.5,.5,.4,2);

}
void sand() // filling  the color for the curve generated
{
    glClear(GL_COLOR_BUFFER_BIT);
    int a[4],n=3,i=0;
    int sn[4][2]={{0,200},{500,400},{700,200},{1920,220}};
    sn[1][0]+=10*sin(t*pi/180.0);
    sn[1][1]+=5*sin(t*pi/180.0);
    sn[2][0]-=10*sin((t+30)*pi/180.0);
    sn[2][1]-=10*sin((t+30)*pi/180.0);
    sn[3][0]-=5*sin(t*pi/180.0);
    sn[3][1]+=sin((t-30)*pi/180.0);
    //theta+=speed;     not required because we dont want it to wiggle. so sticking with the first curve coordinates
    glPointSize(4);
    bino(n,a);
    glPushMatrix();
    glColor3f(0.98,.695,.246);
    for(i=0;i<25;i++)
    {
        glTranslatef(0,-.08,0);
        bezsnd(a,sn,n);
    }
    glEnd();
    glPopMatrix();
    glutPostRedisplay();
}
void sesky()        // water
{
    glBegin(GL_QUADS);
    glColor3f(0,0.5,1);
    glVertex3f(0,1280,-2);
    glVertex3f(0,200,-2);
    glVertex3f(1920,200,-2);
    glVertex3f(1920,1280,-2);
    glEnd();
 }
void weed(int x , int y)        //smoke weed every-day
{
    int c[4],n=3,i=0;
    int cp[4][2]={{x,y},{x-5,y+70},{x,y+100},{x,y+120}};//80,40
    cp[1][0]+=10*sin(theta*pi/180.0);
    cp[1][1]+=5*sin(theta*pi/180.0);
    cp[2][0]-=10*sin((theta+30)*pi/180.0);
    cp[2][1]-=10*sin((theta+30)*pi/180.0);
    cp[3][0]-=5*sin(theta*pi/180.0);
    cp[3][1]+=sin((theta-30)*pi/180.0);
    theta+=speed;  // it helps to wiggle by finding the new co-ordinates
    glPointSize(7);
    bino(n,c);
    glPushMatrix();
    glColor3f(.24,.4,.02);
    for(i=0;i<25;i++)
    {
        glTranslatef(0,.1,0);
        bez(c,cp,n);
    }
    glEnd();
    glPopMatrix();
    glutPostRedisplay();
    //glutSwapBuffers();
}
void underwater()
{
            sand();             //sand
            sesky();        //blue water
            anamonie();     //all the rocks
            weed(85,60);    // 1st weed from left
            weed(100,68);   // 2nd weed from left
            weed(115,66);   //3rd weed from left
            weed(130,68);   //4th weed from left
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (currentScene) {
        case 0:
            underwater();
            break;
        case 1:
            bg();   //dummy scene used for testing
            break;
        default:
            break;
    }
    glFlush();
    glutSwapBuffers();
}
void reshape(int width, int height)
{
  //GLfloat fieldOfView = 90.0f;
  glViewport (0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1920, 0, 1280, -450, 450);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void mymenu(int option)
{
    if (option==1)
        speed=5;
    if (option==2)
        speed=2;
}

void idle()
{
	glutPostRedisplay();
}
void init(int width,int height)
{
	reshape(width, height);
    glClearColor(1, 1, 1, 0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //gluOrtho2D(0,1920,0,1280);
    //glMatrixMode(GL_MODELVIEW);
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1920,1280);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Test");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutCreateMenu(mymenu);
    glutAddMenuEntry("wiggle  wiggle",1);   //mouse function to set the speed for wiggling
    glutAddMenuEntry("wiggle",2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutSpecialFunc(keyboard);
    glutIdleFunc(idle);
    init(1920,1280);
    glutMainLoop();
    return 0;
}
