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
void callcul_pi(int myNum){  
    cout << "Thread No " << myNum << " lancé\n"; 
    double hit = 0.0f;
    double miss = 0.0f;
    for (int i = 0; i < nb_iteration_d_un_thread ; i ++){ 
        double x=  (float)(rand()); 
        double y = (float)(rand());
        if (x*x + y*y < 1 )
        {
            hit++;
        }   
        else
        {
            miss++;
        }   
    }

    // division 1/N de la somme locale
    somme_globale +=(hit/nb_iteration_d_un_thread);         
}

int main() {
    std::thread Ids[NB_THREADS];
    for(int k=0; k<NB_THREADS; k++)
        Ids[k]=thread(callcul_pi, k);
    for(int k=0; k<NB_THREADS; k++)
        Ids[k].join();
    cout << "Valeur approchée de Pi (méthode basique 1) : " << somme_globale/NB_THREADS << endl;
    return 0;
}
/*
$ g++  -std=c++17 cd-fichier.cpp -lpthread
$ ./a.out

==> Remarquez les entrelacements des messages !

Thread No 0 lancé
Thread No Thread No 3 lancé
1 lancé
Thread No 2 lancé
Thread No 5 lancé
Thread No 6 lancé
Thread No 4 lancé
Thread No 7 lancé
Valeur approchée de Pi (méthode basique 1) : 3.14159

*/