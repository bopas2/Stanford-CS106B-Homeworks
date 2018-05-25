//The .h file for VectorPatientQueue

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
#include "vector.h"
#include "limits"
using namespace std;

struct patient {        //patient struct
    string name;        //name
    int priority;       //patient's priority
    int time;           //time of entry
    int index = 0;      //index in the queue
};

struct potentialPatients {      //struct that will hold vectors for upgrading
    Vector<patient> patients;
    Vector<int> indexes;
};

class VectorPatientQueue : public PatientQueue {
public: //mandatory functions to have
    VectorPatientQueue();
    ~VectorPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private: //additional functions that provide efficiency / clarity
    Vector<patient> patientList;        //our pqueue
    int timeCount;                      //tracks time of entry for patients
    patient frontPatient(Vector<patient> patients);                     //function that returns most urgent patient
    patient createPatient(string name, int priority, int time);         //function that creates a patient node
    potentialPatients findPatients(string name,int newPriority);        //finds the potential patients that qualify to be upgraded
};
