//HeapPatientQueue.h file

#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
#include "limits"
using namespace std;

class HeapPatientQueue : public PatientQueue  {
public: //Mandatory functions
    HeapPatientQueue();
    ~HeapPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private: //My functions / vars to improved efficiency
    void expand();  //increases size of array
    void processingFunc(int index, int priority); //helps sort binaryheap after processing an element
    PatientNode *arrayName; //pointer to array
    int currentArraySize; //current array size
    int dataLength; //current length of data in the array
};
