#include "TimeStepper.hpp"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{

    vector<Vector3f> old_s = particleSystem->getState();
    int n = old_s.size();
    vector<Vector3f> new_s(n);
    vector<Vector3f> df = particleSystem->evalF(old_s);

    for (int i = 0; i < n; i += 1){
        new_s[i] = old_s[i] + stepSize * df[i];
    }
    particleSystem->setState(new_s);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{

}
