// my project

#include <iostream>
#include "console.h"
#include "gwindow.h" // for GWindow
#include "simpio.h"  // for getLine
#include "queue.h"
#include "vector.h"  // for Vector
#include "stack.h"
#include "grid.h"
#include "set.h"
using namespace std;

int main() {
    Vector<int> toBeSorted = {3,2,5,4};
    for(int i = 0; i < toBeSorted.size(); i++) {
        int smallest = toBeSorted[i]; int index = i;
        for(int h = i; h < toBeSorted.size(); h++) {
            if(toBeSorted[h] < smallest) {
                smallest = toBeSorted[h];
                index = h;
            }
        }
        int temp = toBeSorted[i];
        toBeSorted[i] = smallest;
        toBeSorted[index] = temp;
        cout << toBeSorted << endl;
    }
    return 0;
}
