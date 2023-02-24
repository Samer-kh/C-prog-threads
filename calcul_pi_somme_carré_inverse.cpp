// On calcul pi comme atant la somme des inverses des carrés des entiers
// tq : π²/6=1/2 + 1/4 + 1/9 + 1/16 ...

#include  <thread>
#include  <iostream>
#include  <mutex>
#include  <cmath>

using namespace std;

#define NB_Entiers 100000L
#define NB_THREADS 4

//les verroux
std::mutex verrou;
std::mutex verrou_thread;

//la somme global 
double somme_globale=0.0f;

//nb thread lancé
int nb_thread=-1;

//contenu de chaque thread
void calcul_pi(int myNum){  
    cout << "Thread No " << myNum << " lancé\n"; 

    verrou_thread.lock();
    //incrementation du nb de threads
    nb_thread++;
    
    // avoir le debut du count pour chaque thread
    int begin = (int) (nb_thread * (NB_Entiers/NB_THREADS));
    if (begin == 0)
    {
        begin = 1;
    }
    verrou_thread.unlock();

    for (int i = begin; i < (int) (NB_Entiers/NB_THREADS) ; i ++)
    { 
        verrou.lock();
        somme_globale+=1/pow(i, 2);
        verrou.unlock();
    }
       
}

int main() {
    std::thread Ids[NB_THREADS];
    for(int k=0; k<NB_THREADS; k++)
    {
        Ids[k]=thread(calcul_pi, k);
    }
    for(int k=0; k<NB_THREADS; k++)
    {
        Ids[k].join();
    }
    cout << "Valeur approchée de Pi (methode basique 2) : " << sqrt(somme_globale*6) << endl;
    return 0;
}
