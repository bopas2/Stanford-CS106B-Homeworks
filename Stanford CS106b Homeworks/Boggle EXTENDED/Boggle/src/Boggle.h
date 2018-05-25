// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h
#include "grid.h"
#include <iostream>
#include <string>
#include "lexicon.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    bool humanWordSearch2(string word);
    Set<string> computerWordSearch();
    Set<string> remainingWords;
    int getScoreHuman();
    int getScoreComputer();
    Lexicon dictionary;
    void promptRandom();
    void printScore();
    bool checkWord2(string word);
    void victory(string x);
    void timeScore1(long x);
    void timeScore2(long x);


    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    void createBoard(string boggles[16], Grid<char> &board);
    void createBoard(string input);
    Set<string> correctHumanGuesses;
    Set<string> computerGuesses;
    bool humanWordSearch(string word, int row, int col, Grid<char> boardCopy);
    int humanScore;
    string computerWordSearch(string word, int row, int col, Grid<char> &board);
    int computerScore;
    Grid<char> board;
};

#endif // _boggle_h
