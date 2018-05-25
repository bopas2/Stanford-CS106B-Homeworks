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
    cout << toBeSorted << endl << endl;
    for(int i = 0; i < toBeSorted.size(); i++) {
        for(int h = i - 1; h >= 0; h--) {
            if(toBeSorted[h] < toBeSorted[i]) {
                break;
            }
            else {
                int temp = toBeSorted[h];
                toBeSorted[h] = toBeSorted[i];
                toBeSorted[i] = temp;
            }
            cout << toBeSorted << endl;
        }
    }
    cout << toBeSorted << endl << endl;
    return 0;
}
