/*
    Name : Thomas Lang
    Class : 106b
    SL : Joshua

    Description : This is the implementation of the priority queue using the vector.
    New patients are added and based on their priority (how urgent their injuries are)
    they are processed based on their priorities.

    It doesn't matter the order in which patients are added, but when they are removed the
    patient with the lowest priority always comes out first, then the second lowest and so on
    and so forth.

    The vector is, by design, is NOT sorted by priority.

    A patient is represented in the program by the struct 'patient' which appears as follows

        struct patient {
            string name;    //patient's name
            int priority;   //patient's priority
            int time;       //used to determine entry order of patients
            int index;      //used for upgrading or removing patients
        };

   A brief explanation of each method is provided, along with expected big O values

*/

#include "VectorPatientQueue.h"

//Vector has already been created in .h file, the constructor ensures it's is empty
// O(1)
VectorPatientQueue::VectorPatientQueue() {
   //timeCount allows us to sort patients by time of entry which is usefull for determining which
    timeCount = 0;          //patient should be processed when two patients have the same priority.
}

//deletes the data in the vector
// O(1)
VectorPatientQueue::~VectorPatientQueue() {
    delete &patientList;
}

// clears the vector of all values
// O(1)
void VectorPatientQueue::clear() {
    patientList.clear();
}

// This function returns the most urgent patient, frontName and frontPriority call this instead of having redundent code
// O(n)
patient VectorPatientQueue::frontPatient(Vector<patient> patients) {
    int indexCount = -1;             //indexCount tell us where the most urgent patient is in the vector for removing or upgrading patients
    if(patients.size() == 0) {
        throw "the queue does not contain any patients"; // If the queue contains no patients we throw an error
    }
    patient urgent = patients[0];
    for(patient i : patients) {              //We loop through every value in the vector
        if(i.priority < urgent.priority) {  //If the current patient has a lower priority than our current most urgent patient
            urgent = i;                     //We set the most urgent patient to the new most-urgent patient
            urgent.index = indexCount;
        }
        else if(i.priority == urgent.priority) {
            if(i.time < urgent.time) {      //If the two priorities are equal, we determine the more urgent patient based on
                urgent = i;                 //their timestamp.
                urgent.index = indexCount;
            }
        }
        indexCount++;
    }
    return urgent;
}

//returns the most urgent patient's name
//O(N)
string VectorPatientQueue::frontName() {
    return frontPatient(patientList).name;
}

//returns the most urgent patient's priority
//O(N)
int VectorPatientQueue::frontPriority() {
    return frontPatient(patientList).priority;
}

//Return a bool if the vector is empty
//O(1)
bool VectorPatientQueue::isEmpty() {
    return patientList.isEmpty();
}

//Creates a new patient struct, implementing specific details (name / priority etc).
//O(1)
patient VectorPatientQueue::createPatient(string name, int priority, int time) {
    patient newp;
    newp.name = name;
    newp.priority = priority;       //sets data
    newp.time = timeCount;
    timeCount++;                    //increments time stamp so we can continue to sort patients by time
    return newp;
}

//adds a new patient
//O(1)
void VectorPatientQueue::newPatient(string name, int priority) {
    patient newPat = createPatient(name,priority,timeCount);        //creates new patient
    patientList.add(newPat);                                        //adds new patient to the vector
}

//removes the most urgent patient and returns it's name
//O(N)
string VectorPatientQueue::processPatient() {
    if(patientList.isEmpty()) throw "Queue contains no patients!"; //Throws an error of there are no patients in queue
    patient temp = frontPatient(patientList);                      //Determines most urgent patient
    patientList.remove(temp.index);                                //Removes the patient
    return temp.name;                                              //Returns their name
}

//returns a struct that holds two arrays. The first array is an array of patients that could be upgraded based on our constraints
//the second array contains those patient's orignial indexes in the main array
potentialPatients VectorPatientQueue::findPatients(string name, int newPriority) {
    potentialPatients arrays;

    Vector<patient> potentialPatients; //Creates a vector that will hold the names of potential upgradable patients
    Vector<int> indexes;               //Saves the indexes of these patients
    int indexCount = 0;                //Used to see which index we are on
    for(patient i : patientList) {
        if(i.name == name) {
            indexes.add(indexCount);        //If the names match, we add the index to the index vector and the patient
            potentialPatients.add(i);       //to the potentialPatients vector
        }
        indexCount++;
    }

    if(potentialPatients.size() == 0) {
        throw "Patient does not exist in queue";    //Throw an exception if the patient isn't in queue
    }

    arrays.indexes = indexes;
    arrays.patients = potentialPatients;
    return arrays;
}

//Changes a patient's priority to be more urgent
//O(N)
void VectorPatientQueue::upgradePatient(string name, int newPriority) {

    potentialPatients toBeUpgraded = findPatients(name,newPriority);

    int mostUrgent = numeric_limits<int>::max();    //Makes it so any patient will have a lower priority
    int urgentindex = 0;                            //Used to track index of most urgent patient
    int index = 0;
    bool proofOfPurchase = true;              //If this isn't changed to false that tells us that no patient fit the constraints to be upgraded
    for(patient i : toBeUpgraded.patients)  {
        if(i.priority > newPriority && i.priority < mostUrgent) { //if our patient has a less urgent priority than the new priority,
            mostUrgent = i.priority;                              //and is more urgent than our most urgent so far,
            proofOfPurchase = false;                                //we make that patient the most urgent
            urgentindex = index;
        }
        index++;
    }

    if(proofOfPurchase) {  //if the new priority is lower than original, throw an exception
        throw "The given patient is present in the queue and already has a more urgent priority than the given new priority!";
    }

    patientList[toBeUpgraded.indexes[urgentindex]].priority = newPriority; //upgrades the correct element
}

//Vector toString for printing the queue to console
//O(n)
string VectorPatientQueue::toString() {
    string answer = "{";
    for(patient i : patientList) {
        answer += integerToString(i.priority) + ":" + i.name + ", ";
    }
    return answer.substr(0, answer.length() - 2) + "}"; //The substring is used to remove the final comma and space
}
