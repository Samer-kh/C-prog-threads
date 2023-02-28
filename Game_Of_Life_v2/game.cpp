#include "game.h"
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;
using namespace std;

#define NB_Thread 20
//implementation du constructeur
game::game()
{
//initialisation du grid
grid = new int*[grid_size_x];
 for (int i=0;i<grid_size_x;i++)
    {
     grid[i]=new int[grid_size_y];

    }
 for(int i=0;i<grid_size_x;i++)
 {
     for (int j=0;j<grid_size_y;j++)
     {
         grid[i][j]=0;
     }
 }
//initialisation du mutex
 verrou = new std::mutex();
 value=0;
 }
//implementation du destructeur
game::~game()
{
    for(int i=0 ; i<grid_size_x ;i++)
    {
        delete[] grid[i];
    }
    delete[] grid;

}
void game::run()
{   int count_r = 0;
    init_grid();
    emit changed();
    while(count_r<2)
    {
        sleep_until(system_clock::now() + seconds(1));
        next_generation();
        display_grid();
        count_r++;
    }
}

void game::init_grid()
{

    //initialise random cells with a random number
    int cell_initialized=rand() % cell_number;
    int count=0;
    int x,y;
    while (count < cell_initialized)
    {
        x= (rand() % grid_size_x);
        y= (rand() % grid_size_y);
        if (grid[x][y]==0)
        {
            grid[x][y]=1;
            count++;
        }
    }
}
int game::getLivingNieghbors(game* g,int row,int col)
{
    {
       int count = 0;
       int rows = g->grid_size_x;
       int cols = g->grid_size_y;

       // Check the eight neighbors of the cell
       for (int i = row - 1; i <= row + 1; i++) {
           for (int j = col - 1; j <= col + 1; j++) {
               int r = (i + rows) % rows; // Wrap around rows
               int c = (j + cols) % cols; // Wrap around columns
               if (r == row && c == col) continue; // Skip the cell itself
               if (g->grid[r][c] == 1) count++; // Increment the count if the neighbor is alive
           }
       }

       g->liv[g->nb_thread]=count;
   }


}
void game::next_generation_per_thread()
{   int living=0;
    for (int i=0;i<grid_size_x;i++)
    {
        for (int j=0;j<grid_size_y;j++)
        {


            if (nb_thread<NB_THREADS)
            {   verrou->lock();
                nb_thread++;
                verrou->unlock();
                std::thread t=std::thread(getLivingNieghbors,this,i,j);
                t.join();
                living=liv[nb_thread];
                verrou->lock();
                nb_thread--;
                verrou->unlock();

            }
            else
            {
                living=getLivingNieghbors(this,i,j);
            }
            //test for under population
            if ((living <2) && (grid[i][j]==1))
            {
                grid[i][j]=0;
            }
            else if ((living > 3) && (grid[i][j]==1))
            {
                grid[i][j]=0;
            }
            else if ((living==3) && (grid[i][j]==0))
            {
                grid[i][j]=1;
            }
            else if (((living==3)||(living==2))&&(grid[i][j]==1))
            {
                grid[i][j]=1;
            }
            else
            {
                grid[i][j]=0;
            }

        }
    }
}

void game::next_generation()
{
next_generation_per_thread();
count =-1;
emit changed();
display_grid();
}

void game::display_grid() {
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
     cout << endl;
}

int game::get_grid_x()
{
    return grid_size_x;
}
int game::get_grid_y()
{
    return grid_size_y;
}

int** game::get_grid()
{
    return grid;
}
int game::get_case()
{
    count++;
    value= grid[(int) count / grid_size_y][count % grid_size_y];
    return value;
}
int game::get_x_size()
{
    return grid_size_x;
}
int game::get_y_size()
{
    return grid_size_x;
}
int game::get_cell_number()
{   emit changed();
    return cell_number;
}
