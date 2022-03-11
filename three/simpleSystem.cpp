
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem(int _k, int m): k(_k), ParticleSystem(m)
{
	vector<Vector3f> newf(m);

	for (int i = 0; i < m; i ++ )
	{
		newf[i] = {0.1,0.1,0.1};
		
	}
	setState(newf);

}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> newf(m_numParticles);

	// YOUR CODE HERE
	for (int i = 0; i < m_numParticles; i += 2 )
	{
		Vector3f v = state[i];
		Vector3f F = state[i+1];

		Vector3f new_F = -1.0f * k * v ;
		Vector3f new_v = v + F;
		newf[i] = new_v; newf[i+1] = new_F;
	}

	return newf;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		vector<Vector3f> s = getState();

		for (int i = 0; i < m_numParticles; i += 2 )
		{
			Vector3f pos = s[i];//YOUR PARTICLE POSITION
	  glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();

		}

		
}
