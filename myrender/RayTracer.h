#include <vecmath/include/vecmath.h>
#include "SceneParser.h"
#include "Ray.h"

class RayTracer
{
public:
    RayTracer(SceneParser*, int);

    Vector3f traceRay(const Ray& ray, int max_bounce);  // main method for ray tracering

private:
    SceneParser* m_scene;
    int m_maxBounce;

};