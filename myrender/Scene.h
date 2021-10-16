
#include "Common.h"
#include "Ray.h"
#include "Group.h"
#include "Object3D.h"
#include "Camera.h"
#include "Material.h"
#include "XY_Rect.h"
#include "Texture.h"
#include "Medium.h"
#include "Box.h"
#include "Transform.h"

#define X 0
#define Y 1
#define Z 2


Group test_3Ball()
{
	Group group;
	auto material_ground = make_shared<Lambientian>(color(0.8,0.8, 0));
	auto material_center = make_shared<Lambientian>(color(0.7,0.3, 0.3));
	auto material_left = make_shared<Snell>(1.5);
	auto material_right = make_shared<Reflective>(color(0.8,0.6,0.2), 0.3);

	group.addGroup(make_shared<Sphere>(Sphere(vec3(0,0,-1), 0.5, material_center) ) );
	group.addGroup(make_shared<Sphere>(Sphere(vec3(0,-100.5,-1), 100, material_ground)));
	group.addGroup(make_shared<Sphere>(Sphere(vec3(-1,0,-1), 0.5, material_left)));
	group.addGroup(make_shared<Sphere>(Sphere(vec3(1,0,-1), 0.5, material_right)));
	return group;
}


Group randomScene()
{
	Group world;
    auto checker = make_shared<Checker>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.addGroup(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambientian>(checker)));
	// auto ground_material = make_shared<Lambientian>(color(0.5, 0.5, 0.5));
    // world.addGroup(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

	for (int a = -5; a < 5; a++){
		for (int b = -5; b < 5; b++){
			double choose_mat = generateRandom(0,1);
			point3 center(a + .9 * generateRandom(0,1), 0.2, b + generateRandom(0,1));

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomColor();
                    sphere_material = make_shared<Lambientian>(albedo);
                    world.addGroup(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = randomColor(0.5,1);
                    auto fuzz = generateRandom(0,0.5);
                    sphere_material = make_shared<Reflective>(albedo, fuzz);
                    world.addGroup(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Snell>(1.5);
                    world.addGroup(make_shared<Sphere>(center, 0.2, sphere_material));
                }
			}
		}
	}

	auto material1 = make_shared<Snell>(1.5);
    world.addGroup(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambientian>(color(0.4, 0.2, 0.1));
    world.addGroup(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Reflective>(color(0.7, 0.6, 0.5), 0.0);
    world.addGroup(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
    return world;
}

Group simple_light()
{
	Group group;
    auto pertext = make_shared<Noise>(4);
    group.addGroup(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambientian>(pertext)));
    // group.addGroup(make_shared<Sphere>(point3(0,2,0), 2, make_shared<Lambientian>(pertext)));
// 
    auto difflight = make_shared<Emission>(color(14,14,14));
    group.addGroup(make_shared<XY_Rect<X,Y>>(3, 5, 1, 3, -2, difflight));
    group.addGroup(make_shared<Sphere>(point3(0,6,0), 1, difflight));
	return group;
}

Group perlin_2Sphere()
{
    Group group;
     auto pertext = make_shared<Noise>(4);
    group.addGroup(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambientian>(pertext)));
    group.addGroup(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Lambientian>(pertext)));
    return group;
}

Group cornell_box() {
    Group objects;
    auto red   = make_shared<Lambientian>(color(.65, .05, .05));
    auto white = make_shared<Lambientian>(color(.73, .73, .73));
    auto green = make_shared<Lambientian>(color(.12, .45, .15));
    auto light = make_shared<Emission>(color(15, 15, 15));

    objects.addGroup(make_shared<XY_Rect<Y,Z>>(0, 555, 0, 555, 550, green));
    objects.addGroup(make_shared<XY_Rect<Y,Z>>(0, 555, 0, 555, 0, red));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(213, 343, 227, 332, 554, light));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(0, 555, 0, 555, 0, white));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(0, 555, 0, 555, 555, white));
    objects.addGroup(make_shared<XY_Rect<X,Y>>(0, 555, 0, 555, 555, white));

    objects.addGroup(make_shared<Box>(point3(130, 0, 65), point3(295, 165, 230), red));
    objects.addGroup(make_shared<Box>(point3(265, 0, 295), point3(430, 330, 460), red));

    return objects;
}

Group smoke()
{
    Group objects;

    auto red   = make_shared<Lambientian>(color(.65, .05, .05));
    auto white = make_shared<Lambientian>(color(.73, .73, .73));
    auto green = make_shared<Lambientian>(color(.12, .45, .15));
    auto light = make_shared<Emission>(color(7, 7, 7));

    objects.addGroup(make_shared<XY_Rect<Y,Z>>(0, 555, 0, 555, 555, green));
    objects.addGroup(make_shared<XY_Rect<Y,Z>>(0, 555, 0, 555, 0, red));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(113, 443, 127, 432, 554, light));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(0, 555, 0, 555, 555, white));
    objects.addGroup(make_shared<XY_Rect<X,Z>>(0, 555, 0, 555, 0, white));
    objects.addGroup(make_shared<XY_Rect<X,Y>>(0, 555, 0, 555, 555, white));

    // shared_ptr<Object3D> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    // box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265,0,295));

    // shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    // box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130,0,65));
    shared_ptr<Object3D> box1 = make_shared<Sphere>(point3(165,165,165), 30, red);

    // objects.addGroup(box1);
    // objects.addGroup(make_shared<Medium>(box1, color(0.4,0,0), 1.3));
    // objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}
