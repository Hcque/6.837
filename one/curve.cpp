#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    
}
    
const Matrix4f Bernstein( 1, -3, 3, -1,
                            0, 3, -6, 3,
                            0, 0, 3, -3,
                            0, 0, 0, 1);

Vector4f monomial_basis(float t) {
    return Vector4f(1,t,t*t, t*t*t);
} 
Vector4f monomial_tangent(float t) {
    return Vector4f(0,1,2*t, 3*t*t);
} 
Vector4f monomial_tangent_deri(float t) {
    return Vector4f(0,0,2, 6*t);
} 

Vector3f V4toV3(Vector4f V){
    return Vector3f(V[0], V[1], V[2]);
}

Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;

    Curve curve;
    for( unsigned i = 0; i < P.size() - 3; i+=3 )
    {
        // cerr << "\t>>> " << P[i] << endl;
        
        // select 4 control points
        Vector4f P0(P[i+0], 0.0);
        Vector4f P1(P[i+1], 0.0);
        Vector4f P2(P[i+2], 0.0);
        Vector4f P3(P[i+3], 0.0);
        Matrix4f ctr_points(P0, P1, P2, P3);

        for (float t = 0; t < 1; t += 1.0/steps){
            // printf("t:%f, %d\n", t, steps);
            Vector3f V = V4toV3( ctr_points * Bernstein * monomial_basis(t) );
            // printf("%f, %f ,%f\n", V[0], V[1], V[2]);

            Vector3f T = V4toV3( ctr_points * Bernstein * monomial_tangent(t) );

            // in 3d , need recursive solution

            Vector3f T_deri = V4toV3( ctr_points * Bernstein * monomial_tangent_deri(t) );
            Vector3f N = T_deri.normalized();
            Vector3f B = Vector3f::cross(T, N);

            CurvePoint cp = { V , T , N, B};
            curve.push_back(cp);
        }
    }
    cerr << "\t>>> Steps (type steps): " << steps << endl;
    return curve;
}

const Matrix4f B_spline( 1.0/6, -3.0/6, 3.0/6, -1.0/6,
                            4.0/6, 0, -6.0/6, 3.0/6,
                            1.0/6, 3.0/6, 3.0/6, -3.0/6,
                            0, 0, 0, 1.0/6);


Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    Curve curve;
    for( unsigned i = 0; i < P.size()-3; ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
         Vector4f P0(P[i+0], 0.0);
        Vector4f P1(P[i+1], 0.0);
        Vector4f P2(P[i+2], 0.0);
        Vector4f P3(P[i+3], 0.0);
        Matrix4f ctr_points(P0, P1, P2, P3);

        // printf("ctrl points: %f, %f ,%f\n", B_spline[0][0], P1[1], P2[2]);

        for (float t = 0; t < 1; t += 1.0/steps){
            // printf("t:%f, %d\n", t, steps);
            Vector3f V = V4toV3( ctr_points * B_spline * monomial_basis(t) );
            // printf("%f, %f ,%f\n", V[0], V[1], V[2]);

            Vector3f T = V4toV3( ctr_points * B_spline * monomial_tangent(t) );

            // in 3d , need recursive solution

            Vector3f T_deri = V4toV3( ctr_points * B_spline * monomial_tangent_deri(t) );
            Vector3f N = T_deri.normalized();
            Vector3f B = Vector3f::cross(T, N);

            CurvePoint cp = { V , T , N, B};
            curve.push_back(cp);
        }
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;

    return curve;
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

