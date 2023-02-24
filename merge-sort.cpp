#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#define NB_THREADS 4
using namespace std;

std::mutex verrou;


//function to merge the two subarrays 
void merge(vector<int>& Tab, int left, int mid, int right)
{
    int first_length = mid - left + 1;
    int second_length = right - mid;

    //create the temporery buffers
    vector<int> L(first_length), R(second_length);

    //copy elements to the temporery buffers
    for (int i = 0; i < first_length; i++)
        L[i] = Tab[left + i];
    for (int j = 0; j < second_length; j++)
        R[j] = Tab[mid + 1 + j];

    //merge the two arrays 
    int i = 0;
    int j = 0;
    int k = left;
    while (i < first_length && j < second_length) {
        if (L[i] <= R[j]) {
            Tab[k] = L[i];
            i++;
        }
        else {
            Tab[k] = R[j];
            j++;
        }
        k++;
    }

    //copy the remaining elements of the left array if there is any

    while (i < first_length) {
        Tab[k] = L[i];
        i++;
        k++;
    }

    //copy the remaining elements of the right array if there is any
    while (j < second_length) {
        Tab[k] = R[j];
        j++;
        k++;
    }
}


// merge sort algorithm

int nb_thread = 0 ; // global variable for the number of threads started and working 
void mergeAlg(vector<int>& tab , int left , int right)
{
    //Getting the splitting factor 
    if (left <right)
    {
        int mid = left + (right - left) / 2;

        // starting the sort of the two arrays 
        //starting the first thread if possible

        if (nb_thread<NB_THREADS)
        {
            verrou.lock();
            nb_thread++;
            verrou.unlock();
            thread firstThread(mergeAlg, ref(tab), left, mid);
            firstThread.join();
        }
        else
        {
            mergeAlg(ref(tab), left , mid);
        }

        //starting the second thread if possible

        if (nb_thread<NB_THREADS)
        {
            verrou.lock();
            nb_thread++;
            verrou.unlock();
            thread secondThread(mergeAlg, ref(tab), mid + 1, right);
            secondThread.join();
        }
        else
        {
            mergeAlg(ref(tab), mid + 1 , right);
        }

        //merge the arrays 
        merge(tab, left, mid, right);
    }

}

//main to test the algorithm 
int main()
{
    vector<int> Tab = { 2, 8, 0, 5, 6, 7 ,7 ,1 ,10 ,3 ,12 ,18};
    int length_tab = Tab.size();

    mergeAlg(Tab, 0, length_tab - 1);

    cout << "the sorted array is : \n";
    for (int i = 0; i < length_tab; i++)
        cout << Tab[i] << " ";

    return 0;
}