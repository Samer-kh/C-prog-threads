
#include <thread> 
#include <iostream> 
#include <sys/types.h> 
#include <unistd.h>
#include <mutex>
int var_globale=0,nb_iterations=10000; 
std::mutex verrou;
void incrementer(int num) {
    verrou.lock();
    std::cout << "thread " << num  << " d’identite (" << (long)getpid()<< ") lancé \n";
    verrou.unlock(); 
    std::cout << "ok \n";
    for(int boucle = 0; boucle < nb_iterations; boucle++) {
        verrou.lock();
        var_globale= var_globale+1; 
        verrou.unlock();
    }}
int main(){
    std::thread Ids[3];
    for(int i=0; i< 3 ; i++)
        Ids[i]= std::thread(incrementer, i);

    for(int i=0; i< 3 ; i++) Ids[i].join();
    std::cout << "var globale = " << var_globale << " au lieu de " << 3*nb_iterations << std::endl; return 0;
    return 0;
}