#include <iostream>
#include <vector>
#include <future>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

bool make_thread = true;

void merge(int* arr, int l, int m, int r)
{
    if (l >= r)
        return;
    int nl (m - l + 1);
    int nr = r - m;

    
    std::vector<int> left(nl),
        right(nr);   

    if (make_thread && nr > 1000) {
        

        future<void> f1 = std::async(std::launch::async, [&]() { for (int i = 0; i < nl; i++) left[i] = arr[l + i]; });

        for (int j = 0; j < nr; j++)
            right[j] = arr[m + 1 + j];       
       
        
        //auto f2 = std::async(std::launch::async, [&]() { for (int j = 0; j < nr; j++) right[j] = arr[m + 1 + j]; });
       
       //thread t ([&]() { for (int i = 0; i < nl; i++) left[i] = arr[l + i]; });      
               
        f1.get();

        //f2.get();
        //t.join();

    }
    else {

        for (int i = 0; i < nl; i++)
            left[i] = arr[l + i];
        for (int j = 0; j < nr; j++)
            right[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0;
    int k = l;  

    while (i < nl && j < nr) {
        
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }

    
}

void mergeSort(int* arr, int l, int r)
{    
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;
    
    auto f1 = std::async(std::launch::deferred, [&]() { mergeSort(arr, l, m); });
    auto f2 = std::async(std::launch::deferred, [&]() { mergeSort(arr, m + 1, r); });

    f1.get();
    f2.get();
    
    
    //mergeSort(arr, l, m);
    //mergeSort(arr, m + 1, r);
   
    merge(arr, l, m, r);

}

int main()
{
    srand(0);
    long arr_size = 10000;
    int* array = new int[arr_size];

    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

    time_t start, end;    

    make_thread = true;

    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);    

    double seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    //for (int i = 0; i < arr_size; i++)
    //    std::cout << array[i] << " ";

    for (long i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }

    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }
    
    make_thread = false;
    
    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);
    
    seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    //for (int i = 0; i < arr_size; i++)
    //    std::cout << array[i] << " ";



    delete[] array;

    return 0;

}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    



