#include "encoding.h"
#include "mymap.h"
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"
#include "map.h"

const int AMOUNT_OF_BUCKETS = 10;
const int END_OF_FILE = 256;

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

void compress(istream& input, obitstream& output) {
    MyMap freqMap;
    freqMap = buildFrequencyTable(input);
    output << freqMap;
    HuffmanNode * encodingTree = buildEncodingTree(freqMap);
    Map<int, string> mapOfKeys = buildEncodingMap(encodingTree);
    encodeData(input,mapOfKeys,output);
}

void decompress(ibitstream& input, ostream& output) {
\
    MyMap map;
    input >> map;
    cout << map << endl;
    decodeData(input,buildEncodingTree(map),output);
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
