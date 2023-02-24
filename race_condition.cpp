#include <thread>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
int var_globale=0,nb_iterations=10000;
int main(){
std::thread Ids[3];
for(int i=0; i< 3 ; i++){
Ids[i]= std::thread([i]() { // Noter la lambda-d´efinition
std::cout << "thread " << i << " d identite (" << (long)getpid()<< ") lancee \n";
for(int boucle = 0; boucle < nb_iterations; boucle++) var_globale = var_globale +1;
});
}
for(int i=0; i< 3 ; i++) Ids[i].join();
std::cout << "var globale = " << var_globale << " au lieu de " << 3*nb_iterations << std::endl;
return 0;
}