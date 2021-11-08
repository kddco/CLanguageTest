#include <iostream>
using namespace std;
void showArray(int* num) {
    for (int i = 0; i < 5; i++) {
        cout << *num++ <<'\n';
    }
}
int main()
{
    int array[5]{ 17,22,23,26,30 };
    int* parray = array;
    cout << "*parray = " << *parray<< endl; //存放array的值
    cout << "parray = " << parray << endl; //存放array的地址
    cout << "&parray = " << &parray << endl;//存放parray自己的地址
    cout << "&array = " << &array << endl; //array的地址
    showArray(parray);
    return 0;
}

/*
*parray = 17
parray = 012FF79C
&parray = 012FF790
&array = 012FF79C

17
22
23
26
30
*/
