#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <random>
#include <cstdlib>

using namespace std;
using namespace std::this_thread; 
using namespace std::chrono; 

//les températures et les pressions ( intervalles )
#define MIN_PRESSION 5
#define MAX_PRESSION 30
#define MIN_TEMP 20
#define MAX_TEMP 30

//declaration of global variables 
std::mutex verrou;
double seuil_t,seuil_p;
bool go_pompe=false,go_chauffage=false;

//define the temps and pressure
double min_temp=MIN_TEMP;
double max_temp=MAX_TEMP;
double min_p=MIN_PRESSION;
double max_p=MAX_PRESSION;

//deux fonctions pour la lecture de la pression et température externe 
//la lecture est aléatoire 
double Convertir_AD_temperature(double low ,double high)
{
    return low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
}

double Convertir_AD_pression(double low , double high)
{
    return low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
}


//declaration de la shared memory et initialisation des valeurs aléatoires de l'environnement externe 
double shared_mem_T= Convertir_AD_temperature(min_temp,max_temp);
double shared_mem_P= Convertir_AD_pression(min_p , max_p);

//determination des temps aléatoires entre une et trois secondes
int controller_time=rand() % 3 + 1;
int chauffage_time=rand() % 3 + 1;
int temp_time=rand() % 3 + 1;
int pression_time=rand() % 3 + 1;
int pompe_time=rand() % 3 + 1;

//declaration du controlleur
void controlleur()
{
    while(true)
    {
        //wait a second 
        sleep_until(system_clock::now() + seconds(controller_time));
        verrou.lock();
        double T =shared_mem_T; 
        double P=shared_mem_P;
        verrou.unlock();
        if (T>seuil_t)
        {
            go_chauffage=false;
            if (P>seuil_p)
            {
                go_pompe=true;
            }
            else
            {
                go_pompe=false;
            }
        }
        else if (T<seuil_t)
        {
            go_chauffage=true;
            go_pompe=true;
        }
        else if (T==seuil_t)
        {
            go_chauffage=false;
            if (P > seuil_p)
            {
                go_pompe=true;
            } 
            else
            {
                go_pompe=false;
            } 
        }
    }
}


void chauffage()
{
    while(true)
    {
        sleep_until(system_clock::now() + seconds(chauffage_time));
        if (go_chauffage)
        {
            //mise en action 
            verrou.lock();
            //action depend de la température
            shared_mem_T+=0.5;
            verrou.unlock();
            
        }
    }
}

void temperature()
{   double T;
    while(true)
    {
        sleep_until(system_clock::now() + seconds(temp_time));
        T = Convertir_AD_temperature(min_temp,max_temp);
        verrou.lock();
        shared_mem_T=T;
        verrou.unlock();
    }
}

void pression()
{   
    double P;
    while(true)
    {
        sleep_until(system_clock::now() + seconds(pression_time));
        P = Convertir_AD_pression(min_p,max_p);
        verrou.lock();
        shared_mem_P=P;
        verrou.unlock();
    }
}
void pompe()
{
    while(true)
    {
        sleep_until(system_clock::now() + seconds(pompe_time));
        if (go_pompe)
        {
            //mettre en route 
            verrou.lock();
            shared_mem_P-=0.5;
            verrou.unlock();
        }
    }
}
void Tache_Ecran()
{
    double T,P;
while (true)
{
    verrou.lock();
    T=shared_mem_T;
    P=shared_mem_P;
    verrou.unlock();
    //clear console before writting 
    system("cls");
    cout<<flush;
    cout<<"Seuil de la température : "<<seuil_t<<endl;
    cout<<"La température est : "<< T <<endl;
    cout<<"****************"<<endl;
    cout<<"Seuil de la pression : "<<seuil_p<<endl;
    cout<<"La pression est : "<< P <<endl;

}
}



//main to commence the program 
int main()
{
    //set thresholds
    seuil_t = (MAX_TEMP + MIN_TEMP) / 2.0;
    seuil_p = (MAX_PRESSION + MIN_PRESSION) / 2.0;

    //create threads
    thread t1(controlleur);
    thread t2(chauffage);
    thread t3(temperature);
    thread t4(pression);
    thread t5(pompe);
    thread t6(Tache_Ecran);

    //wait for threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return 0;
}
