// Hello Reader! This is Thomas Lang's ngrams program for 106b!
//
// This program creates a random "educated" output based on an imported text.
//
// I wrote some information about my code in the word ladder file if you haven't already looked at it :)
//

//Define libraries
#include "console.h"
#include "gwindow.h" // for GWindow
#include "simpio.h"  // for getLine
#include "vector.h"  // for Vector
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "map.h"

using namespace std;

//Declare future functions

void introStatement();
string fileName();
int nCount();
Map<Vector<string>,Vector<string>> mapMaker(int nAmount, string fileName, int NumOfWords);
string randomText(Map<Vector<string>, Vector<string> > &map, int numWords);

int main() {
    introStatement();                   //intro strings
    string fileID = fileName();         //Prompts user for file name
    int amountOfN = nCount();           //Prompts user for n amount

    // this basically runs the program until the user wishes to quit, prompts the user about amount of random words and creates an output based on that context.
    bool quit = false;
    int moreThanFour = -1;
    while(!quit) {
        moreThanFour = -1;
        while(moreThanFour < 4) {
            moreThanFour = getInteger("# of random words to generate (0 to quit)? ","Illegal integer format. Try again.");
            if(moreThanFour >= 4) {
                break;                          //breaks out of loop that is ensuring that the word count > 4
            }
            else if(moreThanFour == 0) {
                cout << "Exiting." << endl;
                return 0;                       //ends the main and the program
            }
            else if(moreThanFour < 4) {
                cout << "Must be at least 4 words." << endl;        //we do at least 4 outputted words at a time
            }
        }
        mapMaker(amountOfN,fileID, moreThanFour);     //actually creates the map and prints the new text to the console
    }

    //Loop back to see if user wants more text or wants to quit

    return 0;
}
//Greets the user and explains the program
void introStatement() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}
//Prompts the user for the file they wish to use as a source for their new text
string fileName() {
    string fileName = "";
    while(!fileExists(fileName)) {
        fileName = getLine("Input file name? ");
        if(fileExists(fileName)) {
            return fileName;
        }
        else {
            cout << "Unable to open that file.  Try again." << endl;
        }
    }
    return "";
}
//Prompts the user to see how many words should be in each "n-gram"
int nCount() {
    int nAmount = 0;
    while(nAmount < 2) {
        nAmount = getInteger("Value of N? ","Illegal integer format. Try again.");
        if(nAmount < 2) {
            cout << "N must be 2 or greater" << endl;
        }
    }
    cout << endl;
    return nAmount;
}
//This function creates the map where the key is a vector of the first n-1 words and the window is also a vector with the nth words
Map<Vector<string>,Vector<string>> mapMaker(int nAmount, string fileName, int NumOfWords) {
    Map<Vector<string>,Vector<string>> answer;      //what we return
    ifstream stream;                                //open stream
    openFile(stream,fileName);
    string word;
    Vector<string> nBlock;                          //holds n words at a time, instead of putting EVERY word into a vector this allows us to look at n words at a time.
    Vector<string> key;                             //used to the first n-1 words that will be used as the key value in the map
    Vector<string> tempWindow;                      //used to access the nth word
    Vector<string> vecStart;                        //this will hold the first n words of the file, this is extremely useful for when we have to wrap the final words of the file
    while(nBlock.size() < nAmount) {
        stream >> word;
        nBlock.add(word);                           //adds first n words into nBlock
        vecStart.add(word);                         //first n words, used later to wrap
    }
    while(!stream.eof()) {
        tempWindow.clear(); key.clear();
        for(int i = 0; i < nBlock.size() - 1; i++) {    // accesses n-1 words to create the vector that will be used as the key
            key.add(nBlock[i] + " ");
        }
        if(answer.containsKey(key)) {
            Vector<string> temp = answer.get(key);      //if the vector already contains a key, we'll add the nth value to the same key that already exists
            temp.add(nBlock[nBlock.size() - 1]);        //edits the pre-existing and adds the nth value
            answer.put(key,temp);                       //updates the map
        }
        else {
            tempWindow.add(nBlock[nBlock.size() - 1]);  //otherwise we just make a new key and window
            answer.put(key,tempWindow);                 //and update the map
        }
        nBlock.remove(0);       //remove first word
        stream >> word;         //we shift the view one word over
        nBlock.add(word);       //and look at the next word
    }
    //now we will wrap
    vecStart.remove(nAmount - 1);
    nBlock.remove(nAmount - 1);
    Vector<string> startAndEnd; //this will create a vector of the last n words and first n words to make wrapping easier
    for(string i : nBlock) {
        startAndEnd.add(i);
    }
    for(string i : vecStart) {
        startAndEnd.add(i);
    }
    for(int i = 0; i < nAmount - 1; i++) {
        Vector<string> newKey;
        Vector<string> newWindow;
        for(int h = 0; h < nAmount - 1; h ++) {
            newKey.add(startAndEnd[i + h] + " ");   //creating a key of n-1 strings
        }
        newWindow.add(startAndEnd[i + nAmount - 1]);    //the vector that holds the nth value
        if(answer.containsKey(newKey)) {
            Vector<string> temp = answer.get(key);  //if the key alreaedy exists we just update it with the new nth value
            temp.add(newWindow[0]);
            answer.put(newKey,temp);
        }
        else {
            tempWindow.add(newWindow[0]);
            answer.put(newKey,newWindow);       //create a new key/window pair
        }
    }
    cout << randomText(answer,NumOfWords) <<endl;       //takes the map and other user information to output a ngram
    stream.close();                                     //were done with the stream, lets just close it

    return answer;
}
//Takes the map made in mapmaker and outputs a string based on the map, number of specified words and the ncount.
string randomText(Map<Vector<string>,Vector<string>> &map, int numWords) {
    string answer = "... ";
    Vector<Vector<string>> keyVector = map.keys();  //puts all the keys of the map into a vector, this is used so we can find a random key to use as our starting point
    int size = keyVector.size();
    int numOfWords = 0;
    Vector<string> firstGroup = keyVector[randomInteger(0,size - 1)];   //takes the random key and places it in a placeholder variable named firstGroup
    for(string i : firstGroup) {
        if(numOfWords < numWords) {
            answer = answer + i;                                    //adds the words to the vector, if we haven't already hit the word limit
            numOfWords++;
        }
        else {
            break;
        }
    }
    string lastWord = map.get(firstGroup)[randomInteger(0,map.get(firstGroup).size() - 1)];
    if(numOfWords < numWords) {
        answer = answer + lastWord + " ";                                 //so far we've added only the key to the answer string, here we add the value that the key maps to.
        numOfWords++;
    }
    Vector<string> grouping = firstGroup;
    grouping.remove(0); grouping.add(lastWord + " ");                   //we remove the first word and add the "suffix" to the "prefixes"
    while(numOfWords < numWords) {
        for(Vector<string> i : keyVector) {
            if(i.equals(grouping) == true) {                            //we look for the key that matches our new vector
                string nextWord = map.get(i)[randomInteger(0,map.get(i).size() - 1)];   //we go to that key and take out a random suffix
                answer += nextWord + " ";
                grouping.remove(0);
                grouping.add(nextWord + " ");                           //we add the suffix to the answer, remove the first value and add the suffix to the prefixes and do the proccess again.
                numOfWords++;
                break;
            }
        }
    }
    return answer + "...";
}

