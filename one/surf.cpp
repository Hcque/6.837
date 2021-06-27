#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

#define printV3(V) printf("%f, %f, %f\n", V[0], V[1], V[2] );
Matrix3f rotation(float theta){
    return Matrix3f(cos(theta), 0, -sin(theta),
                    0, 1, 0,
                    sin(theta), 0, cos(theta)
                                             );
} 

Matrix3f trans_norm(float theta){
    Matrix3f rot = rotation(theta);
    return rot.transposed().inverse();
}
Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // TODO: Here you should build the surface.  See surf.h for details.
    vector< Vector3f> VV;
    vector< Vector3f> VN;
    vector< Tup3u> VF;

    // rotate the curve
    int pro_len = profile.size();
    float theta = 2.0 * M_PI / steps;
    printf("size of profile: %d\n", pro_len);
    printf("steps : %d\n", steps);

    for (unsigned i = 0; i < profile.size(); i++){
        for (unsigned j = 0; j < steps; j++){
            Vector4f x = Vector4f( profile[i].V, 1.0);
            Matrix4f rota =  Matrix4f::rotateY(theta*j);
            Matrix3f rotaN =  rota.getSubmatrix3x3(0,0).transposed().inverse();
            Vector3f N = (-1.0) *trans_norm(theta*j) * profile[j].N;
            VV.push_back( (rota * x).xyz() );
            VN.push_back( (1) * rotaN * profile[i].N );
        }
    }
    printf("collected size (VV): %d\n ", VV.size());
    // collecting vertices
    for (unsigned i = 0; i < profile.size()-1; i++){
        for (unsigned j = 0; j < steps-1; j++){

            // assume not hit the end of the line;
            // assume on its right has next line;
            Tup3u t3(i*steps + j  , (i+1)*steps + j + 1, i*steps + j + 1);
            VF.push_back(t3);

            Tup3u tt3(i*steps + j , (i+1)*steps + j , (i+1)*steps + j + 1);
            VF.push_back(tt3);
        }
    }
    printf("vf size:%d\n", VF.size());
    // collecting surface
    surface.VV = VV;
    surface.VN = VN;
    surface.VF = VF;
    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // TODO: Here you should build the surface.  See surf.h for details.
    for (int i = 0; i < sweep.size(); i++){
        Matrix4f coor(
            sweep[i].N[0], sweep[i].B[0], sweep[i].T[0], sweep[i].V[0],
            sweep[i].N[1], sweep[i].B[1], sweep[i].T[1], sweep[i].V[1],
            sweep[i].N[2], sweep[i].B[2], sweep[i].T[2], sweep[i].V[2],
            0.0, 0.0, 0.0, 1.0
            );
        Matrix3f rota = coor.getSubmatrix3x3(0,0);
        Matrix3f rota_N = rota.transposed().inverse();
        for (int j = 0; j < profile.size(); j++){
            surface.VV.push_back( (coor * Vector4f(profile[j].V, 1.0)).xyz() );
            surface.VN.push_back( -1 * rota_N * profile[j].N );
        }
    }
    for (int i = 0; i < sweep.size()-1; i++){
        for (int j = 0; j < profile.size()-1; j++){
            surface.VF.push_back( Tup3u( i, i + profile.size(), i+1) );
            surface.VF.push_back( Tup3u( i+1, i + profile.size(), i+1 + profile.size()) );
        }
    }
    // cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." <<endl;

    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
