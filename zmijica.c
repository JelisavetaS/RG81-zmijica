#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include <stdbool.h>
#include <string.h>

/*pomocne promenljive za obradu kolizija*/
static int game = 1;
static int poruka = 1;
static int score = 0;
static int best = 0;

static int animation_parametar = 0;
static bool animation_active = false;

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
static void Specialkey(int key, int x, int y);
static void counter(void);

/*Karakteristike vockice*/
static int pozicija_x, pozicija_z;
static bool prikazivanje = false;

/*Pratimo kretanje zmijice*/
static int duzina = 1;

/*Lista u kojoj cuvamo pozicije delova zmijice*/
struct Delovi{
	float x;
	float y;
	float z;
};

static struct Delovi *zmijica;

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
    glutSpecialFunc(Specialkey);
    glutIdleFunc(idle);

    /* Obavljamo inicijalizaciju  OpenGL-a. */
    glClearColor(0.8, 0.8, 0.8, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.5);

    /*Realokacija*/
    zmijica = (struct Delovi *)calloc(1,sizeof(struct Delovi));
    if(zmijica == NULL)
    	return 0;


    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

	free(zmijica);
    return 0;
}


static void on_timer(int value){
    /*Kretanje zmijice - poslednji deo se premesta na deo pretposlednjeg itd...*/
    if(value == 1){
    	for(int i = duzina - 1; i> 0;i--)
    	{
    		zmijica[i].x = zmijica[i - 1].x;
    		zmijica[i].z = zmijica[i - 1].z;
    	}

    	/*Koordinate zmijice se menjaju u odnosu na smer kretanja*/
    	if(game == 1){
    		//printf("%f %f %f %f \n", zmijica[0].x, zmijica[0].z, move_X, move_Z);
	    	zmijica[0].x += move_X;
	    	zmijica[0].z += move_Z;
    	}	

        glutPostRedisplay();
        glutTimerFunc(20, on_timer, 1);
    }

    /*Pojavljivanje vockice - random generisanje*/
    if(value == 2 && !prikazivanje && game == 1){
    	srand(time(NULL));
        pozicija_x = rand() % 25 - 12;
        pozicija_z = rand() % 25  - 12;

        prikazivanje = true;
    }

    /*Koliko puta ce se scena zarotirati*/
    if(value == 3){
    	if(animation_parametar >= 3 * 180)
    	{
    		animation_parametar = 0;
    		game = 1;
    		animation_active = false;
    		score = 0;
    	}
    	else{
    		animation_parametar += 1;
    	 	glutTimerFunc(5, on_timer, 3);
    	}
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

    /*Zmijica skrece levo. Menjamo koordinate i zabranjujemo da glava i rep samo obrnu mesta - analogno i za ostale tastere*/
    case 'A':
    case 'a':
    	
        if(((move_X != 0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = -0.4;
            move_Z = 0;
        }

        break;


    /*Zmijica skrece desno. Menjamo koordinate*/
    case 'D':
    case 'd':
    	
        if(((move_X != -0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = 0.4;
            move_Z = 0;
        }

        break;

    /*Zmijica ide na gore. Menjamo koordinate*/
    case 'W':
    case 'w':
    	
        if(((move_X != 0 && move_Z != 0.4) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = 0;
            move_Z = -0.4;
        }

        break;
        

    /*Zmijica ide na dole . menjamo koordinate*/
    case 's':
    case 'S':
    	
        if(((move_X != 0 && move_Z != -0.4) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = 0;
            move_Z = 0.4;
        }

        break;

    /*Pokrecemo igricu ponovo, resetujemo stanja*/
    case 'r':
    case 'R':

    	if(game == 0){
    		duzina = 1;
    		zmijica[0].x = 0;
    		zmijica[0].z = 0;
    		move_X = 0;
            move_Z = 0;
            game = 1;
            srand(time(NULL));
       		pozicija_x = rand() % 25 - 12;
        	pozicija_z = rand() % 25  - 12;
    	}

        

    }

    glutPostRedisplay();

}

/*Funkcija koja omogucava da kontrolisemo kretanje zmijica pomocu strelica*/
static void Specialkey(int key, int x, int y)
{

    switch (key) {
    case 27:
        /* Program se zavrsava. */
        exit(0);
        break;

    /*Zmijica skrece kevo. Menjamo koordinate i zabranjujemo da glava i rep samo obrnu mesta - analogno i za ostale tastere*/
    case GLUT_KEY_LEFT:
    	
        if(((move_X != 0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = -0.4;
            move_Z = 0;
        }

        break;


    /*Zmijica skrece desno. Menjamo koordinate*/
    case GLUT_KEY_RIGHT:
    	
        if(((move_X != -0.4 && move_Z != 0) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = 0.4;
            move_Z = 0;
        }

        break;

    /*Zmijica ide na gore. Menjamo koordinate*/
    case GLUT_KEY_UP:
    	
        if(((move_X != 0 && move_Z != 0.4) || (move_X == 0 && move_Z == 0)) && game == 1){
            move_X = 0;
            move_Z = -0.4;
        }

        break;
        

    /*Zmijica ide na dole. Menjamo koordinate*/
    case GLUT_KEY_DOWN:
    	
        if(((move_X != 0 && move_Z != -0.4) || (move_X == 0 && move_Z == 0)) && game == 1){
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
	/*Ukoliko se dodje do odredjenog skora pokrece se animacija pobede*/
	if(score == 800){
		if(!animation_active){
			animation_active = true;
			glutTimerFunc(5, on_timer, 3);
		}
		return;
	}

	/*Obradjivanje kolizija - ako pokupi vockicu, ako udari u neki od zidova*/
	/*Razdaljina izmedju zmijice i vocke, ako je dovoljno mala zmija jede vockicu i raste*/
    float distance = sqrt(pow((zmijica[0].x - pozicija_x),2) + pow((zmijica[0].z - pozicija_z), 2));

    if(distance <= 1 && prikazivanje){
    	duzina += 1;
    	score += 5;
    	/*Azuriraju se scorovi*/
    	if(score > best)
    		best = score;
        prikazivanje = false;
        zmijica = (struct Delovi *)realloc(zmijica, sizeof(struct Delovi)*duzina);
        /*Uzima se kockica koja se nalazi suprotno od vektora kretanja*/
        if(duzina == 2)
        {
        	zmijica[1].x = zmijica[0].x - move_X;
        	zmijica[1].z = zmijica[0].z - move_Z;
        }
        /*Uzima se suprotan pomeraj od poslednje do pretposlednje kockice u zmijici i on se obrce*/
        else{	
			zmijica[duzina - 1].x = -zmijica[duzina - 3].x + 2 * zmijica[duzina - 2].x;
        	zmijica[duzina - 1].z = -zmijica[duzina - 3].z + 2 * zmijica[duzina - 2].z;
        }
   		
        glutTimerFunc(20, on_timer, 2);
    }

    /*Ukoliko se zmijica previse priblizi desnom zidu - analogno za ostale*/
    if(zmijica[0].x >= 15-0.5){
    	game = 0;
    	score = 0;
    	if(poruka == 1){
    		printf("Udario si u desni zid - KRAJ IGRE!\n");
    		--poruka;
    		
    	}
    	
    }

    else if(zmijica[0].x <= -15+0.5){
    	game = 0;
    	score = 0;
    	if(poruka == 1){
    		printf("Udario si u levi zid - KRAJ IGRE!\n");
    		--poruka;
    	}
    	
    }

    else if(zmijica[0].z >= 15-0.5){
    	game = 0;
    	score = 0;
    	if(poruka == 1){
    		printf("Udario si u gornji zid - KRAJ IGRE!\n");
    		--poruka;
    	}
    	
    }
    else if(zmijica[0].z <= -15+0.5){
    	game = 0;
    	score = 0;
    	if(poruka == 1){
    		printf("Udario si u donji zid - KRAJ IGRE!\n");
    		--poruka;
    	}
    	
    }
 
    /*Ako zmijica udari sama u sebe, posmatramo da li se koordinata glave zmijice nalazi na istoj poziciji kao i neki deo tela*/
    int i = 2;
    
    for(i = 2; i < duzina; i++){
    	if(zmijica[0].x == zmijica[i].x && zmijica[0].z == zmijica[i].z)
    	{
    		game = 0;
    		score = 0;
    		if(poruka == 1){
	    		printf("Zmijica se zaplela :(\n");
	    		--poruka;
	    	}
    	}
    }
    	glutTimerFunc(20, on_timer, 2);
    
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
            18, 35, 18,
            0, 0, 0,
            0, 1, 0
        );

    
	    /*
	    glBegin(GL_LINES);
	    	
	        glColor3f(1,0,0); 
	        glVertex3f(0,0,0);
	        glVertex3f(100,0,0);
	        
	        
	        glColor3f(0,0,1);
	        glVertex3f(0,0,0);
	        glVertex3f(0,100,0);
	        
	        
	        glColor3f(0.25,0.25,0.25);
	        glVertex3f(0,0,0);
	        glVertex3f(0,0,100);
	    glEnd();
	    */

	/*Podesavamo boje razlicitih vrsta osvetljenja osvetljenja*/
    GLfloat light_position[] = { 5, 15, 0, 0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

	/* Ukljucujemo se osvjetljenje i podesavamo parametre svetla*/
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	
	/*Podesavamo materijale*/
	GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
	GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
	GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
	GLfloat shininess = 30;

	/*Ukljucujemo materijale i podesavamo parametre*/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_COLOR_MATERIAL);

	/*Ukoliko se postigne odredjeni score igra se zaustavlja i scena se vrti u znak pobede*/
	if(score == 800){
		game = 0;
		glRotatef(animation_parametar%180, 0, 1, 0);
	}
	/*Pod napravljen od kocke translirane tako da
	njena gornja ivica bude u centru koordinatnog 
	sistema*/
	{
		glColor3f(0,0.4,0);
		glPushMatrix();
		glTranslatef(0, -1, 0);
		glScalef(31, 0, 31);
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

		/*Pravljenje zmijice, ide se celom njenom duzinom i delovi se prave u onosu na pomeraj koji je igrac napravio*/
		for(int i = 0 ; i< duzina; i++)
		{
			// printf("%d %d\n",zmijica[i].x, zmijica[i].z );
			glColor3f(0.5, 1, 0.5);
			glPushMatrix();
			glTranslatef(zmijica[i].x ,0, zmijica[i].z);
			glutSolidCube(1);
			glPopMatrix();	
		}

		/*Pravljenje vockice random generisano*/
		if(prikazivanje)
		{
			glColor3f(0.96, 0.04, 0.04);
		 	glPushMatrix();
		  	glTranslatef(pozicija_x ,1, pozicija_z);
		    glutSolidSphere(0.5, 50, 50);
		    glPopMatrix();
		}
	}

	/*Pozivamo funkciju koja nam omogucava prikaz score-a*/
	counter();


    /* Nova slika se salje na ekran*/
    glutSwapBuffers();

}

/*Funkcija preuzeta od koleginice RG40-lonely-room*/
void counter()
{
    int current_width = glutGet(GLUT_WINDOW_WIDTH);  
    int current_height = glutGet(GLUT_WINDOW_HEIGHT);  
    glMatrixMode(GL_PROJECTION); 
    glPushMatrix();  
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); 
        glPushMatrix(); 
            glLoadIdentity();
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            gluOrtho2D(0.0, current_width, current_height, 0.0);                 
            char display_string[32];
            int words = sprintf(display_string,"%s %d %s %d", "Score: ", score, "Best: ", best);
            if(words < 0)
                exit(1);    
            glRasterPos2i(25.0f, 25.0f); 
            int d = (int) strlen(display_string);

            for (int i = 0; i < d; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, display_string[i]);
                
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glMatrixMode(GL_PROJECTION); 
        glPopMatrix(); 
        glMatrixMode(GL_MODELVIEW); 
    glPopMatrix(); 
    glutPostRedisplay();
}


