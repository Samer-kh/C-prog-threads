#include "game.h"
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;
using namespace std;

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
int game::getLivingNieghbors(int row,int col)
{
    {
       int count = 0;
       int rows = grid_size_x;
       int cols = grid_size_y;

       // Check the eight neighbors of the cell
       for (int i = row - 1; i <= row + 1; i++) {
           for (int j = col - 1; j <= col + 1; j++) {
               int r = (i + rows) % rows; // Wrap around rows
               int c = (j + cols) % cols; // Wrap around columns
               if (r == row && c == col) continue; // Skip the cell itself
               if (grid[r][c] == 1) count++; // Increment the count if the neighbor is alive
           }
       }

       return count;
   }


}
void game::next_generation_per_thread(game* g,int begin_x, int end_x,int begin_y, int end_y)
{   int living=0;
    for (int i=begin_x;i<end_x;i++)
    {
        for (int j=begin_y;j<end_y;j++)
        {
            //locking the mutex
            g->verrou->lock();
            //test for under population

            living=g->getLivingNieghbors(i,j);
            if ((living <2) && (g->grid[i][j]==1))
            {
                g->grid[i][j]=0;
            }
            else if ((living > 3) && (g->grid[i][j]==1))
            {
                g->grid[i][j]=0;
            }
            else if ((living==3) && (g->grid[i][j]==0))
            {
                g->grid[i][j]=1;
            }
            else if (((living==3)||(living==2))&&(g->grid[i][j]==1))
            {
                g->grid[i][j]=1;
            }
            else
            {
                g->grid[i][j]=0;
            }
            //unlocking the verrou
            g->verrou->unlock();
        }
    }
}

void game::next_generation()
{
std::thread Ids[NB_THREADS];
for (int i=0;i<NB_THREADS;i++)
{
    cout<<(grid_size_x/NB_THREADS)*i<<endl;
    cout<<(grid_size_x/NB_THREADS)*(i+1)<<endl;
    Ids[i]=std::thread(next_generation_per_thread,this,(int) ((grid_size_x/NB_THREADS)*i),(int) ((grid_size_x/NB_THREADS)*(i+1)),(int) ((grid_size_y/NB_THREADS)*(i)),(int) ((grid_size_y/NB_THREADS)*(i+1)));

}
for(int i=0; i<NB_THREADS; i++)
{
    Ids[i].join();
}
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
