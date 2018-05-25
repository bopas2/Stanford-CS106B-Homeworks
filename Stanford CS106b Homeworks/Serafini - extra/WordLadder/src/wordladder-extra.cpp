// Thomas Lang's WordLadder code for CS106B
//
// "A word ladder is a bridge between one word and another, formed by changing one letter at a time,
// with the constraint that at each step the sequence of letters still forms a valid word."
//
// Simple summary of how this is achieved:
//
// There is a queue of stacks. The program loops through these queues, peeking at the top value of each queue. It takes this top string and
// creates word permutations by looping through the alphabet and replacing singular characters at certain indexes of the word
// with singular characters of the alphabet. Legal permutations are added to new stacks (which are copies of the original stack we 'peeked at' and added to the queue.
// This process continues until we find the goal word, the first stack that reaches the goal word will be returned as the wordLadder, as it reached it the quickest.
//
// After the interactive grading session last week I made some changes to my style of coding.
// First of all, I broke up my code into seprate functions to make the code easier to read. Looking back it is much easier to understand than blocks of code
// Secondly, when prompting for user information I tried to avoid using 'flags' and ints to continue or break while loops. I tried to use more direct measurements
// like, for example, while(!fileExists(name)) instead of while (x == 0). Sometimes I found that thsi wasn't possible and used bools instead of ints.
//
// Finally, I do have many in-line comments in some functions. I know you said having many comments means I should simplify my code into smaller sections,
// but I believe that I have devided my code into healthy sections, and that my comments should make my code easier to read for both you and me.
//
// Without further ado...I present to you my wordLadder code! :)
//
//
// EXTENSIONS : The first words do not need to be in the dictionary. I did this by simply deleting the lines the check if word1 and word2 are already in the dictionary, then the program
// continues as usual
//
//

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
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
#include "lexicon.h"
#include "queue.h"
#include "stack.h"
#include "set.h"

using namespace std;

//declaring functions for the main

Stack<string> ladderBuilder(string &word1, string &word2, Lexicon &dict);
void greetUser();
string getFileID();
void printLadder(Stack<string> &answer);
string legalWordsTester(string word1, string word2, Lexicon &englishDictionary);
Queue<Stack<string>> wordLadderQueue(string word1);

int main() {

    greetUser();                                //greets the user
    string dictionaryName = getFileID();        //gets the file name from user
    cout << endl;                               //skips a line to follow the sample format
    Lexicon englishDictionary(dictionaryName);  //creates the lexicon
    bool run = true;

    //This while loop will run until the user wishes to quit. I didn't create a seprate method for this because if the user wants to quit using the program I can simply
    //return 0 to halt the program. I used other functions like "legalWordsTester" to make this function more efficient and not a block of code
    while(run) {
        string word1 = ""; string word2 = "";
        while(legalWordsTester(word1,word2,englishDictionary) != "legal") {              //will loop until two legal words are entered
            word1 = getLine("Word #1 (or Enter to quit): "); word1 = toLowerCase(word1); //gets word1 and removes case sensitivity
            if(word1.compare("") == 0) {
                cout << "Have a nice day." << endl;                                      //for quitting the program
                return 0;
            }
            word2 = getLine("Word #2 (or Enter to quit): "); word2 = toLowerCase(word2); //gets word2 and removes case sensitivity
            if(word2.compare("") == 0) {
                cout << "Have a nice day." << endl;                                      //for quitting the program
                return 0;
            }
            else if(legalWordsTester(word1,word2,englishDictionary) != "legal") {
                cout << legalWordsTester(word1,word2,englishDictionary) << endl;
            }
        }

        //we now have the user's words, we will now run them through the ladder function to output a response.

        Stack<string> answer = ladderBuilder(word1,word2,englishDictionary);
        if(answer.isEmpty()) {
            cout << "No word ladder found from " + word1 + " to " + word2 << endl << endl;  //if there is no word ladder, we say so
        }
        else {
            printLadder(answer);                                                            //otherwise we call printLadder to print the word ladder to the console
        }

        // The user now has their wordladder! We will loop again and see if the user wants to continue and enter more words.

    }
    return 0;
}

//The ladderBuilder function actually creates the wordLadder.
//We find all the permuations of word1 by looping through each index. In each index we loop through the alphabet to replace the index with the respective character
//in the alphabet. If this new permuation is an english word, AND hasn't appeared in a previous stack we will create a copy of the previous stack and add this new word
//to the stack. We add the new stack to the queue and continue. This continues until word2 is found, or if the loop ends. If the loop ends we return 'no wordladder'

Stack<string> ladderBuilder(string &word1, string &word2, Lexicon &dict) {
    Stack<string> voidLadder;                                  //basically if no ladder can be found it will return this
    Queue<Stack<string>> ladderQueue = wordLadderQueue(word1); //Creates a queue of stacks, and currents holds one stack which holds word1
    Set<string> stackedWords;                                  //intializes a set which will ensure we don't add words to stacks that are already in other stacks
    stackedWords.add(word1);                                   //adds the starter word to the set
    string alphabet = "abcdefghijklmnopqrstuvwxyz";            //used to find neighbors of a word

    while(word1.length() < word2.length() || word2.length() < word1.length()) {


        while(!ladderQueue.isEmpty()) {                            //loop continues while the queue has stacks in it
            Stack<string> tempStack = ladderQueue.dequeue();       //we remove the first stack from the queue and store it in a temporary stack
            string nextWord = tempStack.peek();                    //looks at the top word of the stack, we will find the existing 'neighbors' for the word
            for(int h = 0; h < nextWord.length(); h++) {           //loops through each index of the word
                for(int k = 0; k < 26; k++) {                      //loops through each char of the alphabet,
                    string potentialNeighbor = nextWord.substr(0,h) + alphabet.substr(k,1) + nextWord.substr(h + 1); //creates a permuation of the word
                    if(dict.contains(potentialNeighbor) == true && stackedWords.contains(potentialNeighbor) == false) { //sees if we can use this permutation
                        if(potentialNeighbor == word2) {
                            tempStack.add(potentialNeighbor);      //sees if this permutation is our goal, if so, we return the stack and it is our ladder!
                            return tempStack;
                        }
                        else  {
                            Stack<string> tempstack2 = tempStack;
                            tempstack2.add(potentialNeighbor + " ");
                            ladderQueue.enqueue(tempStack);         //otherwise we requeue the previous stack and the new stack which has the permutation
                            ladderQueue.enqueue(tempstack2);
                        }
                        stackedWords.add(potentialNeighbor);        //we add the new word to the set so we don't use it again
                    }
                }
            }
        }
    }
    return voidLadder;                                          //if no wordladder can be found we return an empty ladder signaling that no ladder exists
}
//This function prints to the console a greeting
void greetUser() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
}
//This function returns the name of the file the user wishes to use as the dictionary
string getFileID() {
    string dictionaryName = "";
    while(!fileExists(dictionaryName)) {
        dictionaryName = getLine("Dictionary file name? ");
        if(fileExists((dictionaryName))) {
            return dictionaryName;
        }
        if(!fileExists(dictionaryName)) {
            cout << "Unable to open that file.  Try again." << endl; //reprompt
        }
    }
    return dictionaryName;
}
//This function intakes a stack, pops off all of it's values and prints them to the console.
void printLadder(Stack<string> &answer) {
    while(!answer.isEmpty()) {
        cout << answer.pop() << " ";
    }
    cout << endl << endl;
}
//This function sees if the user inputed two legal words
string legalWordsTester(string word1, string word2, Lexicon &englishDictionary) {
    if(word1 == word2) {
        return "The two words must be different.";
    }
    else {
        return "legal";
    }
}
//This function was used at the very beginning of ladderBuilder to create the first stack and queue, really used to be more efficient and less cluttered.
Queue<Stack<string>> wordLadderQueue(string word1) {
    Queue<Stack<string>> ladderQueue;   // intializes the queue of stacks that holds potential word ladders
    Stack<string> starterStack;         // intializes the first stack
    starterStack.add(word1);            // adds word1 to first stack
    ladderQueue.enqueue(starterStack);  // adds the stack to the queue
    return ladderQueue;
}

// Have a good day!
