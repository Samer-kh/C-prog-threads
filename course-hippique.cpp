#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <future>

#define NB_threads 20
#define NB_colonnes 130

using namespace std;

void moveto(int lig, int col)  {printf("\033[%d;%df",lig, col);}
void erase_scr()  {printf("\033[2J");}
void set_curseur_visible() {printf("\x1B[?25h");  }        
void set_curseur_invisible() {printf("\x1B[?25l"); }
void erase_line() {printf("\x1B[2K"); }
 
void  courrir(int ma_ligne) {
	cout << "Thread << ma_ligne <<  lancé \n" ;
	int i;
	char mon_signe[3] = {(char)(ma_ligne+((int)'A')-1),'>',0};		// donnerra de 'A'.. 'Z'

	for (i=0; i<NB_colonnes; i++){
		 moveto(ma_ligne, i);erase_line();
		 moveto(ma_ligne, i);puts(mon_signe);
		 usleep(50000*(rand()%3+1));
		 // OU std::this_thread::sleep_for(std::chrono::nanoseconds(50000*(rand()%3+1)));		  
	}
	set_curseur_visible();
}

int main()
{  int i ;
   std::thread tab_id1[NB_threads];
   int tab_num_ligne_a_ecran[NB_threads];	// les valeurs envoyées aux threads
   srand(time(NULL));
   
   erase_scr();	 // On efface l'écran
   set_curseur_invisible();
   moveto(20,1);
   cout << "Créations des threads \n";

   for (i=0; i< NB_threads; i++){
	  tab_num_ligne_a_ecran[i]=i+1;
	  tab_id1[i]=std::thread(courrir, tab_num_ligne_a_ecran[i]);
   } 

    moveto(20,2);
    cout<< "ATTENTIONS : Sans Join, Main termine et les threads s'arrêtent\n"  ;
    for (i=0; i< NB_threads; i++){
      tab_id1[i].join();
    }
    cout << "\n\n\n\n Fin de main\n";
    set_curseur_visible();
    return 0;
}
