#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#define NB_THREADS 4


std::mutex verrou ;
int nb_thread = 0 ; // variable global décrivant le nombre de threads instantié 

int partitionnement(std::vector<int> &arr , int first_arr , int last_arr )
{
 int i= first_arr;
 int j = last_arr;
 int pivot = arr[first_arr];

 //on commence a tester et faire les swaps 
    while (i <= j)
    {
        while (i <= j && arr[i] <= pivot)
        {
            i++;
        }

        while (i <= j && arr[j] > pivot)
        {
            j--;
        }

        if (i <= j)
        {
            std::swap(arr[i], arr[j]);
        }
    }

  std::swap(arr[first_arr], arr[j]);
  return j;
}
void quickSortWithThreads(std::vector<int> &arr, int first_arr , int last_arr)
{
  if (first_arr<last_arr)
  {
    int index_pivot = partitionnement(arr , first_arr , last_arr);
    if (nb_thread<NB_THREADS)
    {
      verrou.lock();
      nb_thread++;
      verrou.unlock();
      std::thread FirstThread(quickSortWithThreads,std::ref(arr),first_arr, index_pivot-1);
      FirstThread.join();
      verrou.lock();
      nb_thread--;
      verrou.unlock();
    }
    else
    {
      quickSortWithThreads(std::ref(arr),first_arr , index_pivot-1);
    }
    if (nb_thread<NB_THREADS)
    {      
      verrou.lock();
      nb_thread++;
      verrou.unlock();
      std::thread SecondThread(quickSortWithThreads,std::ref(arr),index_pivot + 1,last_arr);
      SecondThread.join();
      verrou.lock();
      nb_thread--;
      verrou.unlock();
    }
    else
    {
      quickSortWithThreads(std::ref(arr),index_pivot + 1,last_arr);
    }
    
    if (nb_thread>0)
    {
    verrou.lock();
    nb_thread--;
    verrou.unlock();
    }
  
  }
}

int main() {
  std::vector<int> arr = {3, 44, 38, 5, 47, 15, 36, 26, 27, 2, 46, 4, 19, 50, 48, 4, 8, 1, 0, 58, 9, 0, 6, 5};
  quickSortWithThreads(arr, 0, arr.size() - 1);
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
  }
  return 0;
}
