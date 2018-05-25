/*
    Name : Thomas Lang
    Class : 106b
    SL : Joshua

    Description : This is the implementation of the priority queue using a binary heap.

    The binary heap  aspect of this assignment is achieved by having each an array of patientNodes.
    This is not treated like a regular array, this array maintains a "heap ordering" property, where
    each index has two "children" index I + 2 and I * 2 + 1. Where parents always hold a higher priority
    than their children. Different functions bubble up and down to ensure correct sorting of the binary
    heap.

    New patients are added and based on their priority (how urgent their injuries are)
    they are processed based on their priorities.

    When printed to the console a binary heap is not sorted. Still, the patient with the lowest priority
    is always returned first, then the second lowest and so on and so forth.

    A patient is represented in the program by the struct 'patientNode', which was provided by the professor

    A brief explanation of each method is provided, along with expected big O values

    Big O Analysis of my heap program:

    PatientQueue(); O(1)
    ~PatientQueue(); O(1)
    pq.newPatient(name,priority); O(N)
    pq.processPatient(); O(1)
    pq.frontName(); O(1)
    pq.frontPriority(); O(1)
    pq.upgradePatient(name,newPriority); O(N)
    pq.isEmpty(); O(1)
    pq.clear(); O(1)
    toString(); O(N)

*/

#include "HeapPatientQueue.h"
#include "strlib.h"

const int starterSize = 15;

//Constructor, creates a new array that our pointer will point to.
//O(1))
HeapPatientQueue::HeapPatientQueue() {
    currentArraySize = starterSize;
    dataLength = 0;
    PatientNode * temp = new PatientNode[starterSize];
    arrayName = temp;                                   //pointer points to array.
}
//Deletes the array
//O(1)
HeapPatientQueue::~HeapPatientQueue() {
    delete [] arrayName;
    arrayName = nullptr;
}

//Checks if the array's size is large enough, if not we create a new array with twice the size
//Copy the values into and point our pointer to it.
//O(N)
void HeapPatientQueue::expand() {
    if(dataLength == currentArraySize - 1) {        //sees if we need a new array (data == size allocated for data).
        PatientNode * newArray = new PatientNode[currentArraySize * 2]; // Makes the array
        for(int i = 1; i < currentArraySize; i++) {
            newArray[i] = arrayName[i];     //copies data into it
        }
        currentArraySize *= 2;  //updates the size var
        delete &arrayName;  //deletes old data
        arrayName = newArray; //update the pointer
    }
}
//creates a new empty node of size 15, deletes the old data and sets the pointer to our new array.
//also updates the vars that track data size and array size.
//O(1)
void HeapPatientQueue::clear() {
    PatientNode * temp = new PatientNode[starterSize];
    currentArraySize = starterSize; dataLength = 0;
    delete [] arrayName;
    arrayName = temp;
}
//returns the most urgent patient's name
//O(1)
string HeapPatientQueue::frontName() {
    return arrayName[1].name;
}
//returns the most urgent patient's priority
//O(1)
int HeapPatientQueue::frontPriority() {
    return arrayName[1].priority;
}
//sees if the length of our data is 0
//O(1)
bool HeapPatientQueue::isEmpty() {
    return dataLength < 1;
}
//Adds a new patient
//O(N)
void HeapPatientQueue::newPatient(string name, int priority) {
    dataLength++; // adding a new word
    expand();   // expands array if needed
    PatientNode * pat = new PatientNode(name,priority,nullptr); //new node to be added

    if(dataLength == 1) {
        arrayName[1] = *pat;   //if there are no elements so far we set the first element
    }

    else {
        arrayName[dataLength] = *pat;           //we add it to the very end of our heap
        for(int index = dataLength / 2; index > 0; index = index / 2) {     //bubble up
            if(pat->priority < arrayName[index].priority) {
                PatientNode temp = arrayName[index];
                arrayName[index] = (*pat);      //if the new patients priority is less than it's parents we swap it
                arrayName[dataLength] = temp;
            }
        }
    }
}
//removes the top most element and returns that patient's name,
//then we put the last patient on the heap on the very top of heap,
//O(n)
string HeapPatientQueue::processPatient() {
    string ans = arrayName[1].name;
    arrayName[1] = arrayName[dataLength];
    dataLength--;
    processingFunc(1,arrayName[1].priority);
    return ans;
}
//function that helps with processing patients and bubbling up.
void HeapPatientQueue::processingFunc(int index, int priority) {
    for(int i = 1; i*2 < dataLength; i *= 2) {
        if(arrayName[i].priority > arrayName[(2 * i)].priority && arrayName[(2 * i) + 1].priority > arrayName[(2 * i)].priority) {
            PatientNode temp = arrayName[(2 * i)];
            arrayName[(2 * i)] = arrayName[i];
            arrayName[i] = temp;
        }
        else if(arrayName[i].priority > arrayName[(2 * i) + 1].priority && arrayName[(2 * i) + 1].priority < arrayName[(2 * i)].priority) {
            PatientNode temp = arrayName[(2 * i) + 1];
            arrayName[(2 * i) + 1] = arrayName[i];
            arrayName[i] = temp;
        }
        else {
            break;
        }
    }
}
//Upgrades a given patient based on their name and priority
//This is done by looping through the array and finding the best patient who is most 'qualified' to be upgraded
//This patient is upgraded, and then bubbled up from it's position in the pQueue.
// O(N)
void HeapPatientQueue::upgradePatient(string name, int newPriority) {
    PatientNode urgent("",numeric_limits<int>::max(),nullptr); int urgentIndex = 0; int index = 2;
    if(dataLength == 1) {
        PatientNode temp(name,newPriority,nullptr); //If there is only 1 element, we upgrade it directly
        arrayName[1] = temp;
        return;
    }
    else {
        for(int i = 2; i <= dataLength; i++) { //other wise we find the most urgent index
            if(arrayName[i].name == name && arrayName[i].priority > newPriority && urgent.priority > arrayName[i].priority) {
                urgent = arrayName[i];
                urgentIndex = index;
            }
            index++;
        }
    }
    if(!(urgent.name.length() > 0)) { //return an error if no patient qualifies to be upgraded
        throw "the patient you wish to upgrade does not exist or already has a higher priority";
    }
    arrayName[urgentIndex].priority = newPriority;
    for(int index = urgentIndex; index > 0; index = index / 2) {        //bubble up that patient to it's correct position
        if(arrayName[index].priority < arrayName[index/2].priority) {
            PatientNode temp = arrayName[index];
            arrayName[index] = arrayName[index/2];
            arrayName[index/2] = temp;
        }
    }

}
//prints out the pQueue
//O(N)
string HeapPatientQueue::toString() {
    string answer = "{";
    for(int i = 1; i < dataLength + 1; i++) {
        answer += integerToString(arrayName[i].priority) + ":" + arrayName[i].name + ", ";
    }
    return answer.substr(0, answer.length() - 2) + "}"; //substr to remove comma
}
