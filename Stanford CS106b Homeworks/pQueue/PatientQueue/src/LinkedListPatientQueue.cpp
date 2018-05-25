/*
    Name : Thomas Lang
    Class : 106b
    SL : Joshua

    Description : This is the implementation of the priority queue using linked list.

    The linked list aspect of this assignment is achieved by having each patientNode point to
    the next patientNode in the list.

    New patients are added and based on their priority (how urgent their injuries are)
    they are processed based on their priorities.

    Opposite to the vector implementation, patients are immediately sorted once added to the
    linked list priority queue. Still, the patient with the lowest priority always comes out first,
    then the second lowest and so on and so forth. However by sorting it this way, we are more efficient
    in our priority queue compared to the vector implementation.

    A patient is represented in the program by the struct 'patientNode', which was provided by the professor

    A brief explanation of each method is provided, along with expected big O values

*/

#include "LinkedListPatientQueue.h"
#include "strlib.h"

//The constructor ensures that the head (the pointer to the beginning of the link list) points to nothing (nullptr).
//O(1))
LinkedListPatientQueue::LinkedListPatientQueue() {
    head = nullptr;
}

//The destructor calls clear (which deletes the data the nodes use) and then delete the data head stores
//O(1))
LinkedListPatientQueue::~LinkedListPatientQueue() {
    clear();
    delete head;
}

//Clear deletes all nodes and resets head
//O(N))
void LinkedListPatientQueue::clear() {
    PatientNode * temp = nullptr;;
    while(head != nullptr) {
        temp = head->next;
        delete head;
        head = temp;
    }
    head = nullptr;
}

//Returns the most urgent patient's name
//O(1))
string LinkedListPatientQueue::frontName() {
    return head->name;
}
//Returns the most urgent patient's priority
//O(1))
int LinkedListPatientQueue::frontPriority() {
    return head->priority;
}
//Returns a bool signifying if the linked list is empty or not
//O(1))
bool LinkedListPatientQueue::isEmpty() {
    return( head == nullptr );
}
//Adds a new patient to the priority queue, seeing as the linked list is sorted, this function also ensures
//that the new patient is placed in the correct position
//O(N)
void LinkedListPatientQueue::newPatient(string name, int priority) {
    PatientNode * pat = new PatientNode(name,priority,nullptr); //our new patient
    PatientNode * pointer = head;                               //copy of head
    if(isEmpty()) {
        head = pat;     //if the list is empty, head points to our new patient
    }
    else if(pointer->priority > priority) {
        pat->next = pointer;    //if the first patient's priority is greater than the new patient,
        head = pat;             //we make head point to the new patient,
        return;                 //and the new patient point to the old starter patientNode
    }
    else {
        while(pointer->next != nullptr) {               //otherwise, we loop until the end of the list
            if(pointer->next->priority > priority) {    //if the next element's priority is greater than the new patients priority
                pat->next = pointer->next->next;        //the new patient points to the patient after the one we are on
                pointer->next = pat;                    //the one were on points to the new patient
                break;                                  //and we break out of the loop
            }
            pointer = pointer->next;                    //if not, we repeat at the next node
        }
        if(pointer->next == nullptr) {                  //if we reach the very last patient
            pointer->next = pat;                        //we have the last patient in the queue point to the new patient
            return;
        }
    }
}
//Treats the most urgent patient
//O(1)
string LinkedListPatientQueue::processPatient() {
    if(isEmpty()) {
        throw " the queue does not contain any patients";
    }
    string answer = head->name;             //what we return to the console
    PatientNode * tempaddress = head->next; // the new start of the linked list is (what was) the second element
    delete head;                            //delete our old data
    head = tempaddress;                     //update head
    return answer;                          //return answer
}
//upgrades a patient's urgency
//O(N)
void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    PatientNode * temp = head;

    //this is a special case for when there is only one patient in the queue
    //if there is only one element in the list, and it qualifies to be upgradeable
    if(temp->next == nullptr && temp->priority > newPriority && temp->name == name) {
        head = nullptr;
        newPatient(name,newPriority);
        return;
    }
    //any other case
    else {
        while(temp->next != nullptr) {  //loops through entire queue
            if(temp->next->name == name && temp->next->priority > newPriority) { //if the next patient node qualifies to be upgraded
                PatientNode * temporary = temp->next->next; //we copy it the patient after it
                delete temp->next;              //remove the patient (thats about to be upgraded) from the list
                temp->next = temporary;         // and re-add the patient (now upgraded) to the queue
                newPatient(name,newPriority);   //this is done, so the queue is sorted correctly
                return;
            }
            else {
                temp = temp->next;      //continue to next node
            }
        }
    }
    throw " The given patient is present in the queue and already has a more urgent priority than the given new priority, or the given patient is not already in the queue. ";
}
//prints out linked list
//O(N)
string LinkedListPatientQueue::toString() {
    string answer = "{";
    if(head == nullptr) {
        return "{}";        //no items in queue
    }
    PatientNode  * pointer = head;
    while(pointer != nullptr) {
        answer += integerToString(pointer->priority) + ":" + pointer->name + ", ";  //other wise loop through queue and output information
        pointer = pointer->next;
    }
    return answer.substr(0,answer.length()-2) + "}"; //substring removes the last comma and space from the string
}
