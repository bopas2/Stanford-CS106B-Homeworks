/*
 *  Name: Thomas Lang
 *  Class: CS 106b
 *  Section Leader: Joshua
 *
 *  encoding.cpp contains the code the will encode and decode the files.
 *
 *  First, the code counts the frequency of each character and stores them in the
 *  hashmap object 'MyMap' which I created.
 *
 *  Second, the code builds a binary tree where each node represents a character,
 *  higher frequency characters will be placed at the top of the binary tree, this is
 *  done using a priority queue.
 *
 *  Third, a Map<int, string> is made. This map will convert an ascii value of a character
 *  into it's huffman binary counterpart, which we will be using to write to our file.
 *
 *  Forth, we traverse the tree, using our map to write the new COMPRESSED data, to the the file.
 *
 *  Finally, we decode the file by first reading the map, that we first outputted into the file,
 *  to create an encoding tree, which is more of a decoding tree this time. As we read the file we
 *  traverse the tree, each 1, we go down right the tree, each 0 we go left down the tree, until we find a
 *  character. The character is outputted to the file, and this is done until it is completely decoded.
 *
 */

#include "encoding.h"
#include "mymap.h"
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"
#include "map.h"

const int AMOUNT_OF_BUCKETS = 10; //amount of 'buckets' we are using in our hash function
const int END_OF_FILE = 256;    //ascii value for EOF

//helper functions
void buildEncodingMapRecursion(Map<int,string> &theMap, string val, HuffmanNode* currentNode);
void decodeDataRecursion(HuffmanNode * pointer, ibitstream& input, ostream& output, int &answer, bool &eofReached);

//Creates a hashmap that stores the frequency of each character in the file
MyMap buildFrequencyTable(istream& input) {
    MyMap * newFrequencyTable = new MyMap; //What we're returning
    while(true) {
        int character = input.get();    //Read the complete file
        if(character == -1) {   //Indicates end of file, break loop
            break;
        }
        else if(newFrequencyTable->containsKey(character)) {
            newFrequencyTable->put(character,newFrequencyTable->get(character) + 1); //We know this character is in the hashmap, so we update it
        }
        else {
            newFrequencyTable->put(character,1); //this character isn't in our hashfunction, so we can add it with a value of one
        }

    }
    newFrequencyTable->put(END_OF_FILE,1); //Adds the EOF character
    return *newFrequencyTable;
}

//Creates the encodingTree, where the top of the tree holds the most common elements.
//No parent's children are both characters, one is a character, the other is a node that is
//the count of both of their children's nodes, and so on and so forth
HuffmanNode* buildEncodingTree(const MyMap& freqTable) {
    //Creates the priority queue and sorts by count
    PriorityQueue<HuffmanNode> pQueue;
    for(int i = 0; i < freqTable.keys().size(); i++) {
        HuffmanNode * newPtr = new HuffmanNode(freqTable.keys().get(i),freqTable.get(freqTable.keys().get(i)),nullptr,nullptr);
        pQueue.enqueue(*newPtr,newPtr->count);
    }
    //Creates the actual tree that we are using by dequeing two elements, jointing them to
    //a node, and adds the node back to the pQueue. This is done until only one node is left, this node is the final tree
    while(pQueue.size() > 1) {
        HuffmanNode left = pQueue.dequeue();    //first one dequeued is left child
        HuffmanNode * Lptr = new HuffmanNode(left.character,left.count,left.zero,left.one);

        HuffmanNode right = pQueue.dequeue();   //second dequeue is right child
        HuffmanNode * Rptr = new HuffmanNode(right.character,right.count,right.zero,right.one);
        //join them together
        HuffmanNode * jointedNode = new HuffmanNode(NOT_A_CHAR,Rptr->count + Lptr->count,Lptr,Rptr);
        pQueue.add(*jointedNode,jointedNode->count); //add it to queue
    }

    HuffmanNode final = pQueue.dequeue(); //this is our pointer to the tree
    HuffmanNode * finalPtr = new HuffmanNode(final.character,final.count,final.zero,final.one);

    return finalPtr;
}

//Creates a map that maps an ascii value to it's huffman binary counterpart
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    buildEncodingMapRecursion(encodingMap,"",encodingTree); //calls recursive helper function
    return encodingMap;
}

//This is the function that actually populates the EncodingMap. Our base case is to check if the node
//is a leaf, which in that case we return the current value associated with it. Otherwise we go down
//both right and left. When we traverse left down the map we add a '0' to our current value, and when we go
//right we assign a '1' to our value.
void buildEncodingMapRecursion(Map<int,string> &theMap, string val, HuffmanNode* currentNode) {
    if(currentNode->isLeaf()) {
        theMap.add(currentNode->character,val);
    }
    else {
        buildEncodingMapRecursion(theMap, val + integerToString(0),currentNode->zero);
        buildEncodingMapRecursion(theMap, val + integerToString(1),currentNode->one);
    }

}

//This function encodes our data and outputs it to a file
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input); //ensures we are at the top of the stream
    while(true) {
        int character = input.get();    //we get characters while we aren't at the end of the file
        if(character == -1) {
            break;
        }
        string encoded = encodingMap.get(character); //we get the huffman binary associated with that character
        for(int i = 0; i < encoded.length(); i++) {
            output.writeBit(stringToInteger(encoded.substr(i,1))); //and output the binary one bit at a time
        }
    }
    string x = (encodingMap.get(PSEUDO_EOF));
    for(int i = 0; i < x.length(); i ++) {
        output.writeBit(stringToInteger(x.substr(i,1)));   //This adds the EOF huffman binary to the end of the file
    }
}
//Decodes the data
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    bool eofFlag = false;
    while(!input.eof()) {   //while we aren't at the end of the file
        int answer;
        decodeDataRecursion(encodingTree, input, output, answer, eofFlag); //call the recursive function
        if(eofFlag == true) {
            return; //if the recursive function indicates we reached the EOF pointer, we end our decoding
        }
        output.put(answer);
    }
}

//The recursive function that will read the file until it finds a character, which it will then return
void decodeDataRecursion(HuffmanNode * pointer, ibitstream& input, ostream& output, int &answer, bool &eofReached) {
    if(pointer->character == PSEUDO_EOF) {
        eofReached = true;  //If we have reached the end of the file, flag this to the main function and return.
        return;
    }
    if(pointer->isLeaf()) {
        answer = pointer->character;    //If we are at a leaf, we have reached a character, we return this character as an answer
        return;
    }
    else {
        int x = input.readBit(); //Otherwise, we read a bit
        if(x == 0) {    //If it is a zero
            pointer = pointer->zero;    //We go to the parent's left child
            decodeDataRecursion(pointer,input,output,answer, eofReached); //And call this function one more
        }
        else if(x == 1) {
            pointer = pointer->one; //Same idea has above, except we look at the right child
            decodeDataRecursion(pointer,input,output,answer, eofReached);
        }
    }
}

//This function pulls together all of the previous funtions to encode the data in one step
void compress(istream& input, obitstream& output) {
    MyMap freqMap;
    freqMap = buildFrequencyTable(input);   //makes the frequency table
    output << freqMap;                      //outputs it to the file for decoding later
    HuffmanNode * encodingTree = buildEncodingTree(freqMap);    //creates the encoding tree
    Map<int, string> mapOfKeys = buildEncodingMap(encodingTree);    //and the map
    encodeData(input,mapOfKeys,output); //and encodes the data to the file
}

//This function pulls together the functions that are used to decode so we can decode in a single step
void decompress(ibitstream& input, ostream& output) {
    MyMap map;
    input >> map; // takes in the frequency table from the file.
    decodeData(input,buildEncodingTree(map),output); //creates the encoding tree, and decodes the data to the new file
}

//frees the memory associated with the binary tree, recursive
void freeTree(HuffmanNode* node) {
    freeTree(node->zero); //goes to left child
    free(node->one);      //right child
    delete node;          //deletes parent
}
