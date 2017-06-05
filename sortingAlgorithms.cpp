/*
* Thomas Kueny
* Computer Algorithms
* Lab 1v2 with iterative merge Sort
* Sorting Algorithms
*
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>

using namespace std;

//global variables to use during testing of run time
int size = 20;
int test[] = {1,7,2,1,7,9,2,3,8,9,4,6,7,1,5,3,6,9,0,1};

/*
* Bubble sort algorithm
* takes in array, iterates through to move the largest item
* to n - 1 index, then iterates through the array again to find the
* next largest until array is sorted.
*/
void bubbleSort(int arr[])
{
    for(int i = 1; i < size; i++)
    {
        for(int j = 0; j < size-1; j++)
        {
            int temp;
            if(arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
/*
* selectionSort algorithm
* takes in an array, iterates to find the smallest item
* then swaps that item with the targeted index i
*/
void selectionSort(int arr[])
{
    int i,j,tiny,temp;

    for(i = size-1; i > 0; i--)
    {
        tiny = 0;
        for(j = 1; j <= i; j++)
        {
            if(arr[j] > arr[tiny])
                tiny = j;
        }
        temp = arr[tiny];
        arr[tiny] = arr[i];
        arr[i] = temp;
    }
}

/*
* insertion sort algorithm
* takes an array, then finds whether an element is smaller than
* the largest element of previously sorted portion of the array. If it is
* it is then inserted into the appropriate index of the sorted portion of the
* array.
*/
void insertionSort(int arr[])
{
    int i,j,target;
    for(j = 1; j < size; j++)
    {
        target = arr[j];
        for(i = j - 1; i >= 0 && arr[i] > target; i--)
        {
            arr[i+1] = arr[i];
        }
        arr[i+1] = target;
    }
}
/*
* merge helper function for recursive merge sort algorithm
*/
void merge_(int arr[], int temp[], int left, int middle, int right)
{
    int i, end_l, length, position;
    end_l = middle - 1;
    position = left;
    length = (right - left + 1);

    while(left <= end_l && middle <= right)
    {
        if(arr[left] <= arr[middle])
        {
            temp[position] = arr[left];
            position++;
            left++;
        } else {
            temp[position] = arr[middle];
            position++;
            middle++;
        }
    }
    while(left <= end_l)
    {
        temp[position] = arr[left];
        left++;
        position++;
    }
    while(middle <= right)
    {
        temp[position] = arr[middle];
        middle++;
        position++;
    }
    for(i = 0; i <= length; i++)
    {
        arr[right] = temp[right];
        right--;
    }
}

/*
* mergeSort algorithm
* divides array into n/2 portions and sorts them, then merges the split
* and sorted arrays into a sorted array
*/
void mergeSort_Recursive(int arr[], int temp[], int left, int right)
{
    int middle;

    if(right > left)
    {
        middle = (right + left)/2;
        mergeSort_Recursive(arr, temp, left, middle);
        mergeSort_Recursive(arr, temp, (middle + 1), right);

        merge_(arr, temp, left, (middle + 1), right);
    }
}
/*
* merge sort call for initial call of function
*/
void mergeSort_(int arr[], int temp[], int arraySize)
{
    mergeSort_Recursive(arr, temp, 0, arraySize - 1);
}

/*
* quick sort algorithm
*/
void quickSort(int arr[], int left, int right){

    int mid = (left+right)/2;

    int i = left;
    int j = right;
    int temp;
    int target = arr[mid];

    //partition
    while(left < j || i< right)
    {
        while(arr[i]< target)
            i++;
        while(arr[j]> target)
            j--;

        if(i<=j){
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        else{
            if(left < j)
                quickSort(arr, left, j);
            if(i < right)
                quickSort(arr,i,right);
            return;
        }
    }
}

int mini(int x, int y){
    if(x > y)
        return y;
    else
        return x;
}
/*
* iterative mergesort algorithm
* iterates through array comparing two elements at a time, sorting them
* and then merging them together. Then the size of arrays that will be sorted
* increases, and the algorithm continues. The same merge method is used as
* in the recursive inmplementation
*/
void iterativeMergeSort(int arr[], int temp[20], int min, int max){

    int i, j;
    for(j = 1; j < max - min; j += j){
        for(i = min; i < max; i += j+j){
            int start = i;
            int middle = i + j - 1;
            int stop = mini(i + j + j -1, max);

            merge_(arr, temp, start, middle, stop);
        }
    }
}

//displays int array to show if sorted
void display(int arr[])
{
    cout << "{";
    for(int i = 0; i < 19; i++)
    {
        cout << arr[i] << ", ";
    }
    cout << "}" << endl;
}

//method to randomly create an array
void newArray(int arr[]){
    for(int i=0; i < size; i++)
        arr[i] = rand() % 1001;
}
int main()
{
    // int temporary[20];
    float totalRT = 0.0;
    for(int i = 0; i < 20; i++){
        newArray(test);
        cout << "Before bubble sort:\n";
        display(test);
        clock_t t1 = clock();
        bubbleSort(test);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR BUBBLE SORT: " << totalRT/20 << endl;
    totalRT = 0.0;
    for(int i = 0; i < 20; i++){
        newArray(test);
        cout << "Before insertion sort:\n";
        display(test);
        clock_t t1 = clock();
        insertionSort(test);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR INSERTION SORT: " << totalRT/20 << endl;
    totalRT = 0.0;

    for(int i = 0; i < 20; i++){
        newArray(test);
        cout << "Before selection sort:\n";
        display(test);
        clock_t t1 = clock();
        selectionSort(test);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR SELECTION SORT: " << totalRT/20 << endl;
    totalRT = 0.0;

    for(int i = 0; i < 20; i++){
        int temporary[20];
        newArray(test);
        cout << "Before merge sort:\n";
        display(test);
        clock_t t1 = clock();
        mergeSort_(test, temporary, 20);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR MERGE SORT: " << totalRT/20 << endl;
    totalRT = 0.0;

    for(int i = 0; i < 20; i++){
        newArray(test);
        cout << "Before quick sort:\n";
        display(test);
        clock_t t1 = clock();
        quickSort(test, 0, 20);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR QUICK SORT: " << totalRT/20 << endl;
    totalRT = 0.0;

    for(int i = 0; i < 20; i++){
        int temporary[20];
        newArray(test);
        cout << "Before iterative merge sort:\n";
        display(test);
        clock_t t1 = clock();
        iterativeMergeSort(test, temporary, 0, 20);
        clock_t t2 = clock();
        float runTime = float(t2) - float(t1);
        cout << "After sort:\nRun Time: " << runTime << endl;
        display(test);
        totalRT += runTime;
    }
    cout << "AVG RUN TIME FOR ITERATIVE MERGE SORT: " << totalRT/20 << endl;
    totalRT = 0.0;
    return 0;
}
