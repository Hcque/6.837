#include <iostream>
#include <cstring>
#include <cassert>
// #include <omp.h>

#include "Common.h"
#include "Ray.h"
// #include "BVH.h"
#include "Group.h"
#include "Object3D.h"
#include "Camera.h"
#include "Material.h"
#include "XY_Rect.h"

#include "Scene.h"


color ray_color(const Ray& ray, Group& group, int m_bounce, const color& bg)
{
	hit_record rec;
	double t_min = 0.01; // for numerics
	double t_max = infinity;
	bool hit = group.hit(ray, t_min, t_max, rec);

	if (hit){
		color tmp_emiss;
		if (rec.material->emission(tmp_emiss)){
			// return tmp_emiss;
			// DEBUG(tmp_emiss);
		}
		Ray diffuseRay;
		color attenu;
		if (m_bounce >= 0 && rec.material->scatter(ray, diffuseRay, rec, attenu)){
			color c =   vec3(0.2,0.2,0.2) + tmp_emiss + attenu * ray_color(diffuseRay, group, m_bounce-1, bg);
			// DEBUG(c);
			return c;
			// return (rec.normal + vec3(1,1,1)) * 0.5;
		}
		else {
			return tmp_emiss;
		}
	}
	else {
		return bg;
	// auto dir = ray.direction();
	// double alpha = ( dir.y()+1 )* 0.5;
	// return (1-alpha)*vec3(1,1,1) + alpha*vec3(0.5,0.7,1); 
	}
}

int main(int argc, char** argv)
{
	double aspect_ratio;
	int image_width, image_height;
	int samples_per_pixel;
	point3 lookfrom, lookat;
	color background;
	double vfov;
	Group group;

	const int max_bounce = 20;
	int c = 6;

	switch (c){
		case 0:
	group = test_3Ball();
	aspect_ratio = 1.0;
	image_width = 400;
	samples_per_pixel = 20;
	background = color(1,1,1);
	lookfrom = point3(0,0.4,1);
	lookat = point3(0,0,-1);
	// lookfrom = point3(-2,2,1);
	// lookat = point3(2, -1, -1);
	vfov = 90.0;
	// camera = PerspectiveCamera(lookfrom, lookat, vec3(0,1,0), vfov, aspect_ratio);
		break;

		case 1:
	group = randomScene();
	aspect_ratio = 3.0/2.0;
	image_width = 200;
	samples_per_pixel = 10;
	background = color(0.7,0.8,1);
	lookfrom = point3(13,2,3);
	lookat = point3(0,0, 0);
	vfov = 20.0;
	// camera = PerspectiveCamera(lookfrom, lookat, vec3(0,1,0), vfov, aspect_ratio, image_width);
	break;

	        case 3:
            group = perlin_2Sphere();
				aspect_ratio = 1.0;
			image_width = 400;
            samples_per_pixel = 400;
            background = color(1,1,1);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;

		case 5:
            group = simple_light();
			aspect_ratio = 1.0;
			image_width = 400;
            samples_per_pixel = 400;
            background = color(0,0,0);
            lookfrom = point3(26,3,6);
            lookat = point3(0,2,0);
            vfov = 20.0;
            break;

		case 6:
	group =  cornell_box();
	// group = BVH(tmp,0,tmp.size());
	aspect_ratio = 1.0;
	image_width = 200;
	samples_per_pixel = 10;
	background = color(1,1,1);
            lookfrom = point3(400, 278, -800);
            // lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
	vfov = 40.0;
	// camera = PerspectiveCamera(lookfrom, lookat, vec3(0,1,0), vfov, aspect_ratio);
		break;
		        case 7:
            group = smoke();
            aspect_ratio = 1.0;
            image_width = 200;
		background = color(0.3,0.3,1);
            samples_per_pixel = 100;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
	}
	PerspectiveCamera camera = PerspectiveCamera(lookfrom, lookat, vec3(0,1,0), 
	vfov, aspect_ratio, image_width, 10);
 	
	image_height = static_cast<int>(1.0f*image_width/aspect_ratio);
	// DEBUG(refract(vec3(-1,-1,-1), vec3(0,1,0),1.2));

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	// double start_time = omp_get_wtime();

	for (int j = image_height-1;j>=0; --j){
		DEBUG(1 - j*1.0f / image_height);
		// DEBUG(image_height);
		for (int i = 0; i < image_width; i++){
			
			color pixel;
			// omp_set_num_threads(2);
			// #pragma omp parallel
			// {
			// #pragma omp for
			for (int n = 0; n < samples_per_pixel; n++){
				double r1 = generateRandom(-.5,0.5);
				double r2 = generateRandom(-.5,0.5);
				double x_ratio = (double)(i + r1)/(image_width-1);
				double y_ratio = (double)(j + r2)/(image_height-1);
				Ray ray = camera.generateRay(x_ratio, y_ratio);
				// #pragma omp critical
				pixel += ray_color(ray, group, max_bounce, background)/samples_per_pixel;
			// }
			}
			writecolor(std::cout, pixel);
		}
	} 

	// } //omp

	// double elapsed = omp_get_wtime() - start_time;

	// std::cerr <<  "Total:" << elapsed << " time \n";
	std::cerr << "Done\n";
	return 0;
}