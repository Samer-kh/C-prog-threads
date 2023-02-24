// janv 22
#define MAX_NUM_THREADS 5
#define taille 100

#include  <thread>
#include  <iostream>
#include  <cmath>
#include  <cstdlib>
using namespace std;

double somme_globale=0.0f;

// La fonction qui réalise la somme d’une tranche du tableau.
void somme_une_tranche(int tab[], int deb_inclu, int fin_exclue){
double somme_locale=0.0f;
    for (int i=deb_inclu ; i< fin_exclue; i++){
        somme_locale += tab[i];
    }
    somme_globale += somme_locale;
}

int main(int argc, char* argv[]) {
	int num_threads=MAX_NUM_THREADS;
	if (argc >1) num_threads=atoi(argv[1]);

    int tab[taille];
    // Remplissage du tabeau : T[i]=i+1. On peut également remplir par random.
    for (int i=0 ; i< taille; i++) tab[i]=i+1;
    //on pourrait écrire : tab[i]=random();
    // La taille d’une tranche du tableau (additionnée par chaque thread)
    int size_tranche= ceil(taille *1.0 / num_threads);
    std::thread Ids[num_threads]; // Le tableau qui recevra les Id des threads
    // La création des threads. Dès après leur création, les threads sont actifs.
    for(int k=0; k<num_threads; k++)
    Ids[k]=thread(somme_une_tranche, tab, k*size_tranche, (k+1)* size_tranche);
    // Attendre la fin des threads (qu’ils aient terminé leur ’tâche’).
    for(int k=0; k<num_threads; k++) Ids[k].join();
    cout << "somme totale : " << somme_globale << " (Avec " << num_threads << " Threads)\n";
    // Vérification : la somme a bien été réalisée ?
    cout << "Controle : on devrait avoir " << taille*(taille+1)/2 << endl ;
    return 0;
}
/*
$ g++  -std=c++17 somme_parallele.cpp -lpthread

$ ./a.out
somme totale : 5050 (Avec 5 Threads)
Controle : on devrait avoir 5050
(base) alex@mi77:~/ECL-ALL/ECL-21-22/3A-MSO-Avec-Rene/TR-3A-MA-19-20$ ./a.out 3
somme totale : 1.92577e+09 (Avec 3 Threads)
Controle : on devrait avoir 5050

*/