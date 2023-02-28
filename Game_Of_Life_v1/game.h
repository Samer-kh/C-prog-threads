#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <vector>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>


using namespace std;
#define NB_THREADS 2
class game : public QObject {
     Q_OBJECT
public:
  //the grid
  int** grid;
  //constructeur
  game();
  //destructeur
  ~game();
  //methode pour lancer le jeu
  Q_INVOKABLE void run();
  //initialiser le grid du jeu
  void init_grid();
  //passage à la generation suivante
  static void next_generation_per_thread(game* g,int begin_x, int end_x,int begin_y, int end_y);
  //passage à la generation suivante
  Q_INVOKABLE void  next_generation();
  //affichage du gid
  void display_grid(); // show grid in terminal ( for test purposes )
  //getters
  int get_grid_x();
  int get_grid_y();
  int** get_grid();
  int nb_thread=0;
  Q_INVOKABLE int get_case();
  Q_INVOKABLE int get_x_size();
  Q_INVOKABLE int get_y_size();
  Q_INVOKABLE int get_cell_number();
  //properties to invoke change
  Q_PROPERTY(int value READ get_case NOTIFY changed());
signals:
    void changed(); // signal pour les valeurs du grille
private:
  //verrou pour la protection du grid
  std::mutex* verrou;
  int count=-1;
  int grid_size_x = 10;
  int grid_size_y = 10;
  int cell_number = 100;
  bool isRunning=false;
  int value=0;
  int getLivingNieghbors(int x,int y);
};
#endif // GAME_H
