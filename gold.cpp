// Lab 0:  finding gold!!

#include <iostream>
using namespace std;

int main () {

char currDig;
int val;
int gold;

while (cin >> currDig){

val = currDig;

if (currDig != '.' && currDig != '-')
    gold = gold + (val - 64);
//cout << "val is " << currDig << "(or? " << val << ")" << endl;
  }

cout << gold << endl;

   return 0;
 }

