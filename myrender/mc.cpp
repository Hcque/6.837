#include <cstdlib>
#include <iostream>

using namespace std;

void a()
{
        int N = 1000;
    int inC = 0;
    for (int i = 0; i < N; i++){
        double x = 1.0f*rand() / (RAND_MAX);
        double y = 1.0f*rand() / (RAND_MAX);
        // cout << x << "|" <<  y << endl;
        if (x*x + y*y <= 1) {
            inC++;
        }
    }
    cout << 4.0f*inC / N << endl;
}

void b()
{
    int N = 1000000;
    int inC = 0;
    for (int i = 0; i < N; i++){
        double x = (1.0f*rand() / (RAND_MAX)) * 2.0;
        // cout << x << "|" <<  y << endl;
        inC += x*x;
    }
    cout << 1.0f*inC/N * 2 << endl;

}

double pdf(double x)
{
    return 0.5 ;
}
int main()
{
    // b();

  int N = 1000000;
    int inC = 0;
    for (int i = 0; i < N; i++){
        double x = (1.0f*rand() / (RAND_MAX))*2 ;
        inC += x*x / pdf(x);
    }
    cout << 1.0f*inC/N  << endl;





}