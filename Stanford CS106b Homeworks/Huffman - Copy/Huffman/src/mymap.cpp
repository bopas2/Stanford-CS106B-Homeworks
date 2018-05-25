#include "mymap.h"
#include "vector.h"
#include "HuffmanNode.h"
#include "pqueue.h"
using namespace std;

const int AMOUNT_OF_BUCKETS = 10;

MyMap::MyMap() {
    buckets = createBucketArray(AMOUNT_OF_BUCKETS);
    nBuckets = AMOUNT_OF_BUCKETS;
    nElems = 0;
}

MyMap::~MyMap() {
    delete [] buckets;
}

void MyMap::put(int key, int value) {
    if(containsKey(key)) {
        key_val_pair * traversalPointer = buckets[hashFunction(key) % AMOUNT_OF_BUCKETS];
        while(traversalPointer != nullptr) {
            if(traversalPointer->key == key) {
                traversalPointer->value = value;
                return;
            }
            else {
                traversalPointer = traversalPointer->next;
            }
        }
    }
    else {
        key_val_pair * newNode = new key_val_pair;
        newNode->key = key; newNode->value = value;
        newNode->next = buckets[hashFunction(key) % AMOUNT_OF_BUCKETS];
        buckets[hashFunction(key) % AMOUNT_OF_BUCKETS] = newNode;
    }
}

int MyMap::get(int key) const {
    key_val_pair * pointer = buckets[hashFunction(key) % AMOUNT_OF_BUCKETS];
    while(pointer != nullptr) {
        if(pointer->key == key) {
            return pointer->value;
        }
        else {
            pointer = pointer->next;
        }
    }
    return 0;
}

bool MyMap::containsKey(int key) {
    key_val_pair * pointer = buckets[hashFunction(key) % AMOUNT_OF_BUCKETS];
    while(pointer != nullptr) {
        if(pointer->key == key) {
            return true;
        }
        else {
            pointer = pointer->next;
        }
    }
    return false;
}

Vector<int> MyMap:: keys() const {
    PriorityQueue<key_val_pair> sort;
    for(int i = 0; i < AMOUNT_OF_BUCKETS; i++) {
        key_val_pair * traversalPointer = buckets[i];
        while(traversalPointer != nullptr) {
            sort.add(*traversalPointer,traversalPointer->key);
            traversalPointer = traversalPointer->next;
        }
    }


    Vector<int> keyVect;
    while(sort.size() != 0){
        keyVect.add(sort.dequeue().key);

    }
    return keyVect;
}

int MyMap::size() {
    return (keys()).size();
}

//Tells us the constant we will add to the characters based on the password
int MyMap::cesearCipher(string password) {
    int ascii = 0;  //starts at 0
    password = toLowerCase(password); //password isn't case sensitive
    for(char x : password) {
        ascii += hashFunction(x); //go through the string, and put the char through the hashfunction
    }
    return ascii % password.length(); //returns the value % passwords length as the value to add to each char
}

/**
 * STARTER CODE, DO NOT MODIFY
 */

// copy constructor
MyMap::MyMap(const MyMap &myMap) {
    // make a deep copy of the map
    nBuckets = myMap.nBuckets;

    buckets = createBucketArray(nBuckets);

    // walk through the old array and add all elements to this one
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }

}

// assignment overload
MyMap& MyMap::operator= (const MyMap &myMap) {
    // make a deep copy of the map

    // watch for self-assignment
    if (this == &myMap) {
        return *this;
    }

    // if data exists in the map, delete it
    for (int i=0; i < nBuckets; i++) {
        MyMap::key_val_pair* bucket = buckets[i];
        while (bucket != nullptr) {
            // walk the linked list and delete each node
            MyMap::key_val_pair* temp = bucket;
            bucket = bucket->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    // walk through the old array and add all elements to this one
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        put(key,value);
    }

    // return the existing object so we can chain this operator
    return *this;
}

ostream &operator<<(ostream &out, MyMap &myMap) {
    out << "{";
    Vector<int> keys = myMap.keys();
    for (int i=0; i < keys.size(); i++) {
        int key = keys[i];
        int value = myMap.get(key);
        out << key << ":" << value;
        if (i < keys.size() - 1) { // print the comma after each pair except the last
            out << ", ";
        }
    }
    out << "}";
    return out;
}

istream &operator>>(istream &in, MyMap &myMap) {
    // assume the format {1:2, 3:4}
    bool done = false;
    in.get(); // get the first char, {
    int nextChar = in.get(); // get the first real character
    while (!done) {
        string nextInput;
        while (nextChar != ',' and nextChar != '}') {
            nextInput += nextChar;
            nextChar = in.get();
        }
        if (nextChar == ',') {
            // read the space as well
            in.get(); // should be a space
            nextChar = in.get(); // get the next character
        } else {
            done = true; // we have reached }
        }
        // at this point, nextInput should be in the form 1:2
        // (we should have two integers separated by a colon)
        // BUT, we might have an empty map (special case)
        if (nextInput != "") {
            vector<string> kvp = stringSplit(nextInput,":");
            myMap.put(stringToInteger(kvp[0]),stringToInteger(kvp[1]));
        }
    }
    return in;
}
/**
 * STARTER CODE, DO NOT MODIFY
 *
 * Given a number of buckets, creates a hashtable (array of linked list heads).
 *
 * @param nBuckets the number of buckets you want in the hashtable.
 * return an array of heads of linked lists of key_val_pairs
 */
MyMap::bucketArray MyMap::createBucketArray(int nBuckets) {
    bucketArray newBuckets = new key_val_pair*[nBuckets];
    for (int i = 0; i < nBuckets; i++) {
        newBuckets[i] = nullptr;
    }
    return newBuckets;
}

/**
 * STARTER CODE
 * The hash function for your HashMap implementation.
 * For an extension, you might want to improve this function.
 *
 * @param input - an integer to be hashed
 * return the hashed integer
 */
int MyMap::hashFunction(int input) const {
    // use unsigned integers for calculation
    // we are also using so-called "magic numbers"
    // see https://stackoverflow.com/a/12996028/561677 for details
    unsigned int temp = ((input >> 16) ^ input) * 0x45d9f3b;
    temp = ((temp >> 16) ^ temp) * 0x45d9f3b;
    temp = (temp >> 16) ^ temp;

    // convert back to positive signed int
    // (note: this ignores half the possible hashes!)
    int hash = (int) temp;
    if (hash < 0) {
        hash *= -1;
    }

    return hash;
}

/**
* @brief MyMap::sanityCheck
*
* Checks a map for internal consistency
*/
void MyMap::sanityCheck(){
    for(int i = 0; i < 1000; i++) {
        put(i,i);
    }
    for(int i = 0; i < 1000; i++) {

        if(!containsKey(i)) {
            string err = integerToString(i) + " should be a key in the map but cannot be found";
        }
        int val;

        try {
            val = get(i);
        } catch(string exception) {
            string err = "Unable to get value for " + integerToString(i);
            throw(err);
        }

        if (i != val) {
            string err = integerToString(i) + " should be mapped to " + integerToString(i) + " but is mapped to " + integerToString(val) + "instead";
            throw(err);
        }
    }
    cout << "Map seems ok" << endl;
}
