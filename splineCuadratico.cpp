#include <stdlib.h>
#include <GL/glut.h>

#if 1

    float Puntos[4][3] = {
        { 10,10,0 },
        {  5,10,2 },
        { -5,0,0 },
        {-10,5,-2}
    };
    #define NUM_PUNTOS 4

#else

    float Puntos[7][3] = {
        { 10,10,0 },
        {  5,10,2 },
        { -5,5,0 },
        {-10,5,-2},
        {-4,10,0},
        {-4,5,2},
        {-8,1,0}
    };
    #define NUM_PUNTOS 7

#endif

unsigned int LOD=20;

#define NUM_SEGMENTOS (NUM_PUNTOS+1)

float* ObtenerPunto(int i) {

    if (i<0) {
        return Puntos[0];
    }

    if (i<NUM_PUNTOS)
        return Puntos[i];

    return Puntos[NUM_PUNTOS-1];
}

void AlPresionarTecla(unsigned char tecla, int, int) {
    switch(tecla) {

    case '+':
        ++LOD;
        break;

    case '-':
        --LOD;

        if (LOD<2)
            LOD=2;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void EnDibujar() {

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(   1,10,30,
                0,0,0,
                0,1,0);

    glColor3f(0.5,0.2,0);
    glPointSize(3);

    glColor3f(0.3,0,0.5);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i!=NUM_PUNTOS;++i) {
        glVertex3fv( Puntos[i] );
    }
    glEnd();

    glColor3f(0,1,0);

    glBegin(GL_LINE_STRIP);

    for(int inicio_cv=-3,j=0;j!=NUM_SEGMENTOS;++j,++inicio_cv)
    {

        for(int i=0;i!=LOD;++i) {

            float t = (float)i/LOD;

            float it = 1.0f-t;

            float b0 = it*it*it/6.0f;
            float b1 = (3*t*t*t - 6*t*t +4)/6.0f;
            float b2 = (-3*t*t*t +3*t*t + 3*t + 1)/6.0f;
            float b3 =  t*t*t/6.0f;

            float x = b0 * ObtenerPunto( inicio_cv + 0 )[0] +
                      b1 * ObtenerPunto( inicio_cv + 1 )[0] +
                      b2 * ObtenerPunto( inicio_cv + 2 )[0] +
                      b3 * ObtenerPunto( inicio_cv + 3 )[0] ;

            float y = b0 * ObtenerPunto( inicio_cv + 0 )[1] +
                      b1 * ObtenerPunto( inicio_cv + 1 )[1] +
                      b2 * ObtenerPunto( inicio_cv + 2 )[1] +
                      b3 * ObtenerPunto( inicio_cv + 3 )[1] ;

            float z = b0 * ObtenerPunto( inicio_cv + 0 )[2] +
                      b1 * ObtenerPunto( inicio_cv + 1 )[2] +
                      b2 * ObtenerPunto( inicio_cv + 2 )[2] +
                      b3 * ObtenerPunto( inicio_cv + 3 )[2] ;

            glVertex3f( x,y,z );
        }
    }

    glVertex3fv( Puntos[NUM_PUNTOS-1] );
    glEnd();

    glBegin(GL_POINTS);
    for(int i=0;i!=NUM_PUNTOS;++i) {
        glVertex3fv( Puntos[i] );
    }
    glEnd();

    glutSwapBuffers();
}

void EnInicializacion() {
}

void AlSalir() {
}

void EnCambioTamañoVentana(int w, int h)
{

    if (h==0)
        h=1;

    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45,(float)w/h,0.1,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc,char** argv) {

    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

    glutInitWindowSize(640,480);

    glutCreateWindow("Curva de B-Spline Clamped: +/- para cambiar el nivel de detalle");

    glutDisplayFunc(EnDibujar);

    glutReshapeFunc(EnCambioTamañoVentana);

    glutKeyboardFunc(AlPresionarTecla);

    EnInicializacion();

    atexit(AlSalir);

    glutMainLoop();

    return 0;
}
