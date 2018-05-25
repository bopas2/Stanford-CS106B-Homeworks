/*
    * I completed one extension for the huffman encoding, this was the additional data encryption.
    *
    * Basically the user enters a password. Each char is put through the myMap's hashfunction, and summed together
    * Then that sum is 'modulused ( % )' by the length of the password. Then, we encrypt the map
    * We do this by looping through each char key, and adding the number we obtained previously.
    * Only the map's keys are encrypted, the huffman binary and the values the keys map to remain the same.
    *
    * For decoding we ask for the user password. If the password is correct we obtain the same constant that we added to the chars previously.
    * We loop through the keys, subtracting the value, creating our original map which allows us to correctly decode the file
    *
    * One should note the EOF character isn't encrypted.
    * Also special care was taken just in case the value added created a sum greater than 255. I used wrapping to have the new value wrap
    * to the beginning (0).
    *
    * I don't know if there is a name for the encryption. I originally wanted to use a vignere cipher, but ran into difficulty with the maps.
    *
    * Non extension code comments can be found on my main assignement, comments pertaining to the extension are written
    *
    * Thanks :)
    *
    *
    * Extension lines :: 155 - 210 in this file and 90 - 95 in the mymap.cpp file
*/
#include "encoding.h"
#include "mymap.h"
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"
#include "map.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
#include "math.h"
#include "strlib.h"

const int AMOUNT_OF_BUCKETS = 10;
const int END_OF_FILE = 256;

MyMap encryptMap(MyMap &map, string password);
MyMap decryptMap(MyMap &map, string password);

MyMap buildFrequencyTable(istream& input) {
    MyMap * newFrequencyTable = new MyMap;
    while(true) {
        int character = input.get();
        if(character == -1) {
            break;
        }
        newFrequencyTable->put(character,newFrequencyTable->get(character) + 1);
    }
    newFrequencyTable->put(END_OF_FILE,newFrequencyTable->get(END_OF_FILE) + 1);
    return *newFrequencyTable;
}
HuffmanNode* buildEncodingTree(const MyMap& freqTable) {

    PriorityQueue<HuffmanNode> pQueue;
    for(int i = 0; i < freqTable.keys().size(); i++) {
        HuffmanNode * newPtr = new HuffmanNode(freqTable.keys().get(i),freqTable.get(freqTable.keys().get(i)),nullptr,nullptr);
        pQueue.enqueue(*newPtr,newPtr->count);
    }

    while(pQueue.size() > 1) {
        HuffmanNode left = pQueue.dequeue();
        HuffmanNode * Lptr = new HuffmanNode(left.character,left.count,left.zero,left.one);

        HuffmanNode right = pQueue.dequeue();
        HuffmanNode * Rptr = new HuffmanNode(right.character,right.count,right.zero,right.one);

        HuffmanNode * jointedNode = new HuffmanNode(NOT_A_CHAR,Rptr->count + Lptr->count,Lptr,Rptr);
        pQueue.add(*jointedNode,jointedNode->count);
    }

    HuffmanNode final = pQueue.dequeue();
    HuffmanNode * finalPtr = new HuffmanNode(final.character,final.count,final.zero,final.one);

    return finalPtr;
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildEncodingMapRecursion(encodingMap,"",encodingTree);
    return encodingMap;
}

void buildEncodingMapRecursion(Map<int,string> &theMap, string val, HuffmanNode* currentNode) {
    if(currentNode->isLeaf()) {
        theMap.add(currentNode->character,val);
    }
    else {
        buildEncodingMapRecursion(theMap, val + integerToString(0),currentNode->zero);
        buildEncodingMapRecursion(theMap, val + integerToString(1),currentNode->one);
    }

}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    while(true) {
        int character = input.get();
        if(character == -1) {
            break;
        }
        string encoded = encodingMap.get(character);
        for(int i = 0; i < encoded.length(); i++) {
            output.writeBit(stringToInteger(encoded.substr(i,1)));
        }
    }
    string x = (encodingMap.get(PSEUDO_EOF));
    for(int i = 0; i < x.length(); i ++) {
        output.writeBit(stringToInteger(x.substr(i,1)));
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    bool eofFlag = false;
    while(!input.eof()) {
        int answer;
        decodeDataRecursion(encodingTree, input, output, answer, eofFlag);
        if(eofFlag == true) {
            return;
        }
        output.put(answer);
    }
}

void decodeDataRecursion(HuffmanNode * pointer, ibitstream& input, ostream& output, int &answer, bool &eofReached) {
    if(pointer->character == PSEUDO_EOF) {
        eofReached = true;
        return;
    }
    if(pointer->isLeaf()) {
        answer = pointer->character;
        return;
    }
    else {
        int x = input.readBit();
        if(x == 0) {
            pointer = pointer->zero;
            decodeDataRecursion(pointer,input,output,answer, eofReached);
        }
        else if(x == 1) {
            pointer = pointer->one;
            decodeDataRecursion(pointer,input,output,answer, eofReached);
        }
    }
}

//Compresses and encrypts the file
void compress(istream& input, obitstream& output) {
    MyMap freqMap;
    freqMap = buildFrequencyTable(input);
    string password = getLine("Enter a password to encode your data: "); //prompt for password
    MyMap temp = encryptMap(freqMap, password); //encrypt the map using the password
    output << temp; //output the map
    HuffmanNode * encodingTree = buildEncodingTree(freqMap);
    Map<int, string> mapOfKeys = buildEncodingMap(encodingTree); //output the huffman binary like usual
    encodeData(input,mapOfKeys,output);
}

//Function that encrypts the map
MyMap encryptMap(MyMap &map, string password) {
    MyMap temp;
    int encryptorAddition = map.cesearCipher(password); //determines what we are adding to each character
    for(int i : map.keys()) {
        if(i == END_OF_FILE) {
            temp.put(END_OF_FILE,map.get(END_OF_FILE)); //ignore EOF
        }
        else {
            int newAscii = i + encryptorAddition;
            newAscii = newAscii % 255;      // % the value by 255 wraps the array, so the char is 0-255
            temp.put(newAscii,map.get(i));
        }
    }
    return temp; //returns encrypted map
}

//decrypts the map so we can decode
MyMap decryptMap(MyMap &map, string password) {
    MyMap temp;
    int encryptorAddition = map.cesearCipher(password); // our constant
    for(int i : map.keys()) {
        if(i == END_OF_FILE) {
            temp.put(END_OF_FILE,map.get(END_OF_FILE)); //ignores EOF
        }
        else {
            int newAscii = i - encryptorAddition; //subtracts the addition
            if(newAscii < 0) {
                newAscii = abs(abs(i - encryptorAddition)-255); //if it needs to be unwrapped we do so using this algorithim
            }
            temp.put(newAscii,map.get(i));
        }
    }
    return temp; //returns decrypted map
}

//decompresses and decrypts the file
void decompress(ibitstream& input, ostream& output) {
    MyMap map;
    input >> map; //takes in map
    string password = getLine("Enter the password used to encrypt this data: ");
    MyMap temp = decryptMap(map,password); //gets the decrypted map
    HuffmanNode * encodingTree = buildEncodingTree(temp); //and decompresses it like normal
    decodeData(input,encodingTree,output);
}

//frees the memory associated with the binary tree, recursive
void freeTree(HuffmanNode* node) {
    freeTree(node->zero); //goes to left child
    free(node->one);      //right child
    delete node;          //deletes parent
}
