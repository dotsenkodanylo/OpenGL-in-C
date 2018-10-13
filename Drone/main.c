/*******************************************************************
           Multi-Part Model Construction and Manipulation
********************************************************************/
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"

const int meshSize = 16;    // Default Mesh Size
const int vWidth = 650;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

static int currentButton;
static unsigned char currentKey;

//Vector3D velocity = NewVector3D(xvel, yvel, zvel);
int width;
int depth;
float xr = 1;
float yr = 0;
float zr = 1;
float rotAngle = 0;

float currX = 0;
float currY = 0;

bool key_left = false;
bool key_right = false;
bool key_up = false;
bool key_down = false;


// Lighting/shading and material properties for drone - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat drone_mat_shininess[] = { 0.0F };

// A quad mesh representing the ground
static QuadMesh groundMesh;

// Structure defining a bounding box, currently unused
//struct BoundingBox {
//    Vector3D min;
//    Vector3D max;
//} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);
GLfloat angle = 45.0f;
int refreshmill = 1;




// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);   // This light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    // Set up ground quad mesh
    Vector3D origin = NewVector3D(-8.0f, 0.0f, 8.0f);
    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
    groundMesh = NewQuadMesh(meshSize);
    InitMeshQM(&groundMesh, meshSize, origin, 16.0, 16.0, dir1v, dir2v);

    Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
    Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
    SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

    // Set up the bounding box of the scene
    // Currently unused. You could set up bounding boxes for your objects eventually.
    //Set(&BBox.min, -8.0f, 0.0, -8.0);
    //Set(&BBox.max, 8.0f, 6.0,  8.0);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Drone

    // Set drone material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);

    // Apply transformations to move drone
    // ...

    // Apply transformations to construct drone, modify this!
    glPushMatrix();
    glTranslatef(currX, yr, currY + 6); 
    glRotatef(-rotAngle, 0.0, 1.0, 0.0);
    glTranslatef(-currX, -yr, -currY - 6); 

    
    glPushMatrix();
    glTranslatef(-1.0, 3.0, 5.0);    
    glTranslatef(currX, yr, currY);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.3);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 3.0, 5.0);    
    glTranslatef(currX, yr, currY);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.3);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 3.0, 7.0);    
    glTranslatef(currX, yr, currY);
    
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.3);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0, 3.0, 7.0);    
    glTranslatef(currX, yr, currY);    
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.3);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-1.0, 2.5, 7.0);    
    glTranslatef(currX, yr, currY);
    //glRotatef(-rotAngle, 0.0, 1.0,0.0);
    
    //glRotatef(25, 0.0, 1.0, 1.0);
    glScalef(0.5, 3.0, 0.5);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

     glPushMatrix();
    glTranslatef(1.0, 2.5, 7.0);    
    glTranslatef(currX, yr, currY);
    //glRotatef(-rotAngle, 0.0, 1.0,0.0);
    
    //glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(0.5, 3.0, 0.5);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0, 2.5, 5.0);    
    glTranslatef(currX, yr, currY); 
    //glRotatef(-rotAngle, 0.0, 1.0,0.0);
    
    //glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(0.5, 3.0, 0.5);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 2.5, 5.0);    
    glTranslatef(currX, yr, currY); 
    //glRotatef(-rotAngle, 0.0, 1.0,0.0);

    //glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(0.5, 3.0, 0.5);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 2.0, 5.0);    
    glTranslatef(currX, yr, currY); 
    //glRotatef(-rotAngle, 0.0, 1.0,0.0);
    //glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.2);

    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();



     glPushMatrix();
    glTranslatef(0.0, 2.0, 7.0);    
    glTranslatef(currX, yr, currY); 
    //glRotatef(-rotAngle, 0.0, 1.0, 0.0);

    //glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(5.0, 0.2, 0.2);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0, 2.0, 6.0);    
    glTranslatef(currX, yr, currY); 
    //glRotatef(-rotAngle, 0.0, 1.0, 0.0);
    glScalef(0.2, 0.2, 5.0);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);    

        
    glPopMatrix();
    glPopMatrix();


    /*
    glPushMatrix();
    //glTranslatef(xr, yr, 0);
    glScalef(10, 2, 2);
    glTranslatef(currX,  yr, currY);    
    
    glRotatef(-rotAngle, 0.0, 1.0,0.0);
    //glutSolidCube(1);
    //glTranslatef(1.0, 2.0, 0.0);
    glutSolidCube(0.4);  

        glPopMatrix();
    */
    angle+=20.0;
    /*glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);
    glRotatef(25.0, 0.0, 1.0, 0.0);
    glScalef(6.0, 1.0, 1.0);
    glutSolidCube(0.2);
    glPopMatrix();
*/
    
    // Draw ground mesh
    DrawMeshQM(&groundMesh, meshSize);

    glutSwapBuffers();   // Double buffering, swap buffers
    
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode - 
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    
    switch (key)
    {
    case 'f':
        currX += 0.5 * cosf(rotAngle * 3.14159 / 180);
        currY += 0.5 * sinf(rotAngle * 3.14159 / 180);
        break;
        printf("Hi there friends");
    case 'b': 
        currX -= 0.5 * cosf(rotAngle * 3.14159 / 180);
        currY -= 0.5 * sinf(rotAngle * 3.14159 / 180);
        break;

    }

    glutPostRedisplay();   // Trigger a window redisplay
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    // Help key
     /*switch(key){
      case GLUT_KEY_UP:
         yr = yr + 0.03;
         cout<<y<<endl;
         glutPostRedisplay();
         break;
      case GLUT_KEY_DOWN:
         yr = yr - 0.03;
         cout<<y<<endl;
         glutPostRedisplay();
         break;
      case GLUT_KEY_LEFT:
         xr = xr - 0.03;
         cout<<x<<endl;
         glutPostRedisplay();
         break;
      case GLUT_KEY_RIGHT:
         xr = xr + 0.03;
         cout<<x<<endl;
         glutPostRedisplay();
         break;

   }*/
     if(key == GLUT_KEY_UP){
         yr = yr + 0.5;
     } else if (key == GLUT_KEY_DOWN){
        yr = yr - 0.5;
     }
     else if (key == GLUT_KEY_LEFT){
        rotAngle-=10;
     }
      else if (key == GLUT_KEY_RIGHT){
        rotAngle+=10;  
      }
  


    /*if (key == GLUT_KEY_F1)
    {
        
    }*/
    // Do transformations with arrow keys
    //else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
    //{
    //}
    //else if {}

    glutPostRedisplay();   // Trigger a window redisplay
}
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 1");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callbacks
    glutDisplayFunc(display);
    glutIdleFunc(display);
    
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}

















// Mouse button cal
//
//
//
//
//
//
//
// lback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;

        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        ;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return NewVector3D(0, 0, 0);
}


