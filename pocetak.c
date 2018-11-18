#include <stdlib.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

int main(int argc, char **argv)
{
    /* Inicijalizujemo GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreiramo prozor. */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Koordinatni sistem");

    /* Registrujemo callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavljamo inicijalizaciju  OpenGL-a. */
    glClearColor(0.8, 0.8, 0.8, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.5);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Program se zavrsava. */
        exit(0);
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height;
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


	/*Zmijica - samo pocetni objekat koji treba skalirati*/
	glColor3f(0.5, 1, 0);
	glutSolidCube(1);

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}