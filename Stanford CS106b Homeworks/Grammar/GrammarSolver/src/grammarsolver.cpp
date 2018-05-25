/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Tom Lang
 * Section leader: Joshua
 * This file contains grammar generating code for CS106B.
 * No extensions
 * :)
 */
#include "set.h"
#include "grammarsolver.h"

using namespace std;
Map<string, Vector<string> > createMap(istream& input);
string grammarGenerate(Map<string,Vector<string>> &grammarMap, string key, string &answer);

//prints out the number of grammar strings the user wants
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string,Vector<string>> map = createMap(input);
    int count = 0;
    while(count < times) {
        count++;
        string answer = "";
        cout << " " << count << ": " <<  grammarGenerate(map,symbol,answer) << endl;
    }
    return {};           // This is only here so it will compile
}
// recursive function that creates the grammar item the user wishes
string grammarGenerate(Map<string,Vector<string>> &grammarMap, string key, string &answer) {
    if(!grammarMap.containsKey(key)) {
        answer += key + " ";                //if the value isn't a key we know it is a terminal value so we add it to the answer and begin to return
        return key;
    }
    else {
        Vector<string> a = stringSplit(grammarMap.get(key)[randomInteger(0,grammarMap.get(key).size() - 1)], " "); //we look at the key given, and pick a random value for that given key
        for(string i : a) {
            grammarGenerate(grammarMap,i,answer);       //we go through the vector we aqquired and run it through the recursive function
        }
    }
    return answer;
}
//this function creates the map that we will use for the rest of the program
Map<string,Vector<string>> createMap(istream& input) {
    Map<string,Vector<string>> answer;
    string line;
    while(!input.eof()) {
        try {
            getline(input,line);                                                   // get the line
            Vector<string> keyAndMap = stringSplit(line,"::=");                     // divide the line between the key and the value
            string key = keyAndMap[0];                                              // keep track of key
            Vector<string> values = stringSplit(keyAndMap[1], "|");                 // get the values that they key should map to
            answer.add(key,values);                                             //add these pairs to the map
        }
        catch(exception blankLine) {
            //basically if there is a blank line we will skip over it
        }
    }
    return answer;
}
