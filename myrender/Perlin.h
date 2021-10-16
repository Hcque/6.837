#ifndef PERLIN_H
#define PERLIN_H

#include "vec.h"
#include "Common.h"
static const int point_count = 256;

class Perlin
{
public:
    double noise(const point3& p) const
    {
        double s = 3.0;
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());

        u = u*u*(3- 2*u);
        v = v*v*(3- 2*v);
        w = w*w*(3- 2*w);

            // auto i = static_cast<int>(s*p.x()) & 255;
            // auto j = static_cast<int>(s*p.y()) & 255;
            // auto k = static_cast<int>(s*p.z()) & 255;
            auto i = static_cast<int>(floor(p.x())) & 255;
            auto j = static_cast<int>(floor(p.y())) & 255;
            auto k = static_cast<int>(floor(p.z())) & 255;

        double c[2][2][2];
        for (int di = 0; di < 2; di++){
            for (int dj = 0; dj < 2; dj++){
                for (int dk = 0; dk < 2; dk++){
                    int ii = perm_x[i+di]^ perm_y[j+dj]^ perm_z[k+dk];
                    c[di][dj][dk] = ranfloat[ii];
                }
            }
        }
        return linear_interploation(c, u, v, w);
        // return ranfloat[perm_x[i]^ perm_y[j]^ perm_z[k]];
    }

    double turb(const point3& p, int depth = 1) const{
        double accum = 0.0;
        point3 tmp_p = p;
        double weight = 1.0;
        double accum_w = 0;
        for (int i = 0; i < depth; i++){
            double aa = noise(tmp_p);
            assert(aa <= 1);
            assert(aa >= 0);
            accum_w += weight;
            accum += aa;
            tmp_p = tmp_p*2;
            weight *= 0.5;
        }
        // assert(accum >= 0);
        // assert(! isnan(accum) );
        double res = accum / accum_w;
        // DEBUG(res);
        assert(res >= 0 && res <= 1.01);
        // DEBUG();
        return res;
    }

    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
    static double *ranfloat;

    private:
    double linear_interploation(double c[2][2][2],  double u, double v, double w) const {
        double accum;
        for(int di = 0; di < 2; di++){
            for(int dj = 0; dj < 2; dj++){
                for (int dk = 0; dk < 2; dk++){
                    accum +=  (di*u + (1-di)*(1-u)) * 
                             (dj*v + (1-dj)*(1-v)) *
                             (dk*w + (1-dk)*(1-w)) * c[di][dj][dk];
                }
            }
        }
        // DEBUG(accum);
        return accum;
    }
};


double* gen_ranfloat(){
    double* ranfloat = new double[point_count];
    for (int i = 0; i < point_count; i++){
        ranfloat[i] = 1.0f*rand()/(RAND_MAX+1.0);
    }
    return ranfloat;
}
double*  Perlin::ranfloat = gen_ranfloat();


static int* helprandom(){
        int* perm_x = new int[point_count];
        for (int i = 0; i < point_count; i++){
            perm_x[i] = i;
        }
        int tmp;
        for (int i = point_count-1; i > 0; i--){
            tmp = perm_x[i];
            int rand_i = rand()%(i);
            perm_x[i] = perm_x[rand_i];
            perm_x[rand_i] = tmp;
        }
        return perm_x;
    }

int* Perlin::perm_x = helprandom();
int* Perlin::perm_y = helprandom();
int* Perlin::perm_z = helprandom();

#endif