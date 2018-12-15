 
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include <stdbool.h>

/* Dimenzije prozora */
static int window_width, window_height;

/*Ose po kojima se zmijica krece*/
float move_Z = 0 , move_X = 0 , X = 0, Z = 0;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void idle(void);
static void on_timer(int value);

/*Pratimo kretanje zmijice*/
static int kretanje[30][30];
static int duzina = 3;

/*Karakteristike vockice*/
static int pozicija_x, pozicija_z;
static bool prikazivanje = false;

int main(int argc, char **argv)
{

    /* Inicijalizujemo GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreiramo prozor. */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Zmijica");

    /* Registrujemo callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutTimerFunc(20, on_timer, 1);
    glutTimerFunc(20, on_timer, 2);
    glutIdleFunc(idle);

    /* Obavljamo inicijalizaciju  OpenGL-a. */
    glClearColor(0.8, 0.8, 0.8, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.5);


    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_timer(int value){


    /*Kretanje zmijice*/
    if(value == 1){
        
        X += move_X;
        Z += move_Z;

        glutPostRedisplay();
        glutTimerFunc(20, on_timer, 1);

    }

    /*Pojavljivanje vockice*/
    if(value == 2 && !prikazivanje){
        pozicija_x = rand() % 25 - 12;
        pozicija_z = rand() % 25  - 12;

        prikazivanje = true;
    }

}


/*u odnosu na pritisnuti taster zmijica menja smer kretanja*/
static void on_keyboard(unsigned char key, int x, int y)
{

    switch (key) {
    case 27:
        /* Program se zavrsava. */
        exit(0);
        break;

    /*Zmijica skrece levo -> menjamo koordinate*/
    case 'A':
    case 'a':

        if((move_X != 0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)){
            move_X = -0.4;
            move_Z = 0;
        }

        break;


    /*Zmijica skrece desno -> menjamo koordinate*/
    case 'D':
    case 'd':
        if((move_X != -0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)){
            move_X = 0.4;
            move_Z = 0;
        }

        break;

    /*Zmijica ide na gore -> menjamo koordinate*/
    case 'W':
    case 'w':

        if((move_X != 0 && move_Z != 0.4) || (move_X == 0 && move_Z == 0)){
            move_X = 0;
            move_Z = -0.4;
        }

        break;
        

    /*Zmijica ide na dole -> menjamo koordinate*/
    case 's':
    case 'S':

        if((move_X != 0 && move_Z != -0.4) || (move_X == 0 && move_Z == 0)){
            move_X = 0;
            move_Z = 0.4;
        }

        break;
        

    }

    glutPostRedisplay();

}

static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height;
}

static void idle(void)
{
    float distance = sqrt(pow((X - pozicija_x),2) + pow((Z - pozicija_z), 2));
    if(distance <= 1){
        prikazivanje = false;
        glutTimerFunc(20, on_timer, 2);
    }
    
    glutPostRedisplay();
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesavamo projekciju. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 1500);

    /* Podesavamo tacku pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            20, 35, 20,
            0, 1, 0,
            0, 1, 0
        );

    glBegin(GL_LINES);
    	/* X osa */
        glColor3f(1,0,0); 
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        
        /* Y osa */
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        
        /* Z osa */
        glColor3f(0.25,0.25,0.25);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();

    /*Pod naprevljen od kocke translirane tako da
    njena gornja ivica bude u centru koordinatnog 
    sistema*/
	glColor3f(0,0.4,0);
	//glRotatef(45, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -1, 0);
	glScalef(30, 0, 30);
	glutSolidCube(1);
	glPopMatrix();

	/*Desni zid napravljen od kocke koja je translirana
	do desne ivice poda a zatim skalirana po celoj duzini z-ose
	poda i po y-osi kako bi dobio na visini*/
	glColor3f(0,0.2,0);
	glPushMatrix();
	glTranslatef(15.5, 0, 0);
	glScalef(0, 8, 31);
	glutSolidCube(1);
	glPopMatrix();

	/*Levi zid napravljen od kocke koja je translirana
	do leve ivice poda a zatim skalirana po celoj duzini z-ose
	poda i po y-osi kako bi dobio na visini*/
	glColor3f(0,0.2,0);
	glPushMatrix();
	glTranslatef(-15.5, 0, 0);
	glScalef(0, 8, 31);
	glutSolidCube(1);
	glPopMatrix();

	/*Gornji zid*/
	glColor3f(0.251,0.251,0.251);
	glPushMatrix();
	glTranslatef(0, 0, 15.5);
	glScalef(31, 8, 0);
	glutSolidCube(1);
	glPopMatrix();

	/*Donji zid*/
	glColor3f(0.251,0.251,0.251);
	glPushMatrix();
	glTranslatef(0, 0, -15.5);
	glScalef(31, 8, 0);
	glutSolidCube(1);
	glPopMatrix();

    /*Pravljenje zmijice*/
    glColor3f(0.5, 1, 0.5);
    glPushMatrix();
    glTranslatef(X ,0, Z);
    glutSolidCube(1);
    glPopMatrix();

    /*Pravljenje vockice*/
    if(prikazivanje){
        printf("Vockica!!\n");
        printf("x = %d,z = %d\n", pozicija_x, pozicija_z);
        glColor3f(1, 1, 0);
        glPushMatrix();
        glTranslatef(pozicija_x ,1, pozicija_z);
        glutSolidSphere(0.5, 50, 50);
        glPopMatrix();

    }
    

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();

}
