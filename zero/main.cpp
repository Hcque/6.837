#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>
using namespace std;

// Globals
#define MAX_BUF_SIZE 1024

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes
int color = 0;

// light change
int dx = 0;
int dy = 0;

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        color = (color+1) % 6;
		// cout << "Unhandled key press " << key << "." << endl; 
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
		// cout << "Unhandled key press: up arrow." << endl;
        dy++;
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position
        dy--;
		// cout << "Unhandled key press: down arrow." << endl;
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position
        dx--;
		// cout << "Unhandled key press: left arrow." << endl;
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
		// cout << "Unhandled key press: right arrow." << endl;
        dx++;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[6][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.4, 0.3, 1.0},
                                 {0.9, 0.8, 0.7, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[color]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {1.0f + dx*0.5f, 1.0f + dy*0.5f, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	// glutSolidTeapot(1.0);

    for (unsigned m = 0; m < vecf.size(); m++){
        unsigned a, c, d, f, g, i;
        vector<unsigned> v = vecf[m];
        a = v[0];
        c = v[2];
        d = v[3];
        f = v[5];
        g = v[6];
        i = v[8];
        glBegin(GL_TRIANGLES);
        glNormal3d(vecn[c-1][0],vecn[c-1][1],vecn[c-1][2]);
        glVertex3d(vecv[a-1][0],vecn[a-1][1],vecn[a-1][2]);
        glNormal3d(vecn[f-1][0],vecn[f-1][1],vecn[f-1][2]);
        glVertex3d(vecv[d-1][0],vecn[d-1][1],vecn[d-1][2]);
        glNormal3d(vecn[i-1][0],vecn[i-1][1],vecn[i-1][2]);
        glVertex3d(vecv[g-1][0],vecn[g-1][1],vecn[g-1][2]);
        glEnd();
    }
    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{
	// load the OBJ file here
    char buf[MAX_BUF_SIZE];    
    while (cin.getline(buf, MAX_BUF_SIZE)){
        cout << buf << endl;
        stringstream ss(buf);
        string s;
        ss >> s;
        if (s == "v"){
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(v);
        }
        if (s == "vn"){
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vecn.push_back(v);
        }
        if (s == "f"){
            char del;
            unsigned int a, b, c, d, e, f, g, h, i;
            
            ss >> a >> del >> b >> del >> c;
            ss >> d >> del >> e >> del >> f;
            ss >> g >> del >> h >> del >> i;
            vector<unsigned> v = {};
            v.push_back(a);
            v.push_back(b);
            v.push_back(c);
            v.push_back(d);
            v.push_back(e);
            v.push_back(f);
            v.push_back(g);
            v.push_back(h);
            v.push_back(i);
            vecf.push_back( v);
        }
    }
}

struct vdata{
    float err;
    Matrix4f Q;
    int idx;
    vector<int> face_idxs;
};

vector<vdata> errors;

void fillin_vdata()
{
    for (int i = 0; i < vecv.size(); i++){
        vdata vd = {0, NULL, i,  NULL};
        errors.push_back(vd);
    }
    
}

void calc_Q_each_face(int f_idx, Matrix4f& Q)
{



}

void initial_Q()
{
    
    // init Q for vectices
    for (int i = 0; i < vecv.size(); i++){

        Matrix4f Q(0);
        int faces = errors[i].face_idxs.size();
        for (int j = 0; j < faces; j++){
            Matrix4f each_face_Q(0);
            calc_Q_each_face( errors[i].face_idxs[j] , each_face_Q);
            Q = Q + each_face_Q;
        }
        Q = Q / faces;
        errors[i].Q = Q;
    }
    
}

void simplify_mesh()
{

    initial_Q();
}


// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
