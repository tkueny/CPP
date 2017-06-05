#include <iostream>
#include <cstdlib>
using namespace std;

int test[10] = {3, 5, 1, 7, 9, 8,2,4,6, 0};
const int MAX_SIZE = 100;

template <class ItemType>
class Heap {

public:

    Heap(){
        data[0] = 0;
        heapLength = 0;
    };
    Heap(ItemType initData[], int length);

    //return true if heap property is satisfied
    bool isHeap();
    void heapify(int i);
    void buildHeap(int length);
    void insert(ItemType item);
    void display();


private:

    ItemType data[MAX_SIZE];
    int heapLength;

};

inline int parent(int i) {return i/2;}
inline int leftchild(int i) {return 2*i;}
inline int rightchild(int i) {return 2*i+1;}

template <class ItemType>
bool Heap<ItemType>::isHeap(){
    for (int i=0; i<=(heapLength-2)/2; i++)
    {
        if (data[2*i +1] > data[i])
                return false;

        if (data[2*i+2] > data[i])
                return false;
    }
    return true;
}

template <class ItemType>
void Heap<ItemType>::Heap(ItemType initData[], int length)

template <class ItemType>
void Heap<ItemType>::heapify(int i){

    int max;
    int left = leftchild(i);
    int right = rightchild(i);
    i--;
    if(left <= heapLength && (data[left] > data[i]))
        max = left;
    else
        max = i;
    if(right <= heapLength && (data[right] > data[max]))
        max = right;
    if(max != i){
        ItemType temp = data[i];
        data[i] = data[max];
        data[max] = temp;
        heapify(max);
    }
}
template <class ItemType>
void Heap<ItemType>::buildHeap(int length){
    for(int i=length/2; i>0; i--)
        heapify(i);
}
template <class ItemType>
void Heap<ItemType>::insert(ItemType item){
    heapLength++;
    data[heapLength-1] = item;
    buildHeap(heapLength);
}
template <class ItemType>
void Heap<ItemType>::display(){
    for(int i = 0; i < heapLength; i++){
        cout << data[i] << " ";
    }
    cout << endl;
}

int main(){
    Heap<int> heep(test, 10);
    cout << heep.isHeap() << endl;
    heep.display();
    heep.insert(20);
    heep.display();
    cout << heep.isHeap() << endl;
    return 0;
}
