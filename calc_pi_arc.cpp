// janv 2022
// LA version  non entrelacée
#include  <thread>
#include  <iostream>
#include  <cmath>
#include <random>
#include  <cstdlib>
using namespace std;

#define NB_POINTS 1000000L
#define NB_THREADS 4
double somme_globale=0.0f;
int nb_iteration_d_un_thread=NB_POINTS;
 // IMPORTANT de ne pas initialiser avec un simple ’0’
void calcul_pi(int myNum){  
    cout << "Thread No " << myNum << " lance\n"; 
    double hit = 0.0f;
    double miss = 0.0f;
    double x = 0.0f;
    double y = 0.0f;
    for (int i = 0; i < nb_iteration_d_un_thread ; i ++){ 
        x=  (float)rand() / (float)RAND_MAX;
        y = (float)rand() / (float)RAND_MAX;
        if (x*x + y*y <= 1 )
        {
            hit+=1;
        }     
    }
    // division 1/N de la somme locale
    somme_globale +=(hit/nb_iteration_d_un_thread);         
}

int main() {
    std::thread Ids[NB_THREADS];
    for(int k=0; k<NB_THREADS; k++)
        Ids[k]=thread(calcul_pi, k);
    for(int k=0; k<NB_THREADS; k++)
        Ids[k].join();
    cout << "Valeur approchee de Pi : " << somme_globale << endl;
    return 0;
}
