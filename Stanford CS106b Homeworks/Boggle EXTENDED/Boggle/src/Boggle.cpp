// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "simpio.h"
#include "bogglegui.h"
#include "shuffle.h"

const int width = 4;
const int height = 4;
// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};
//constructor
Boggle::Boggle(Lexicon  &dictionary, string boardText) {
    this->dictionary = dictionary;
    board.resize(width,height);
    BoggleGUI::initialize(width,height);
    if(boardText.length() == 0) {
        createBoard(CUBES,board);
    }
    else
        createBoard(boardText);
    humanScore = 0;
    computerScore = 0;
}
//creates a random board
void Boggle::createBoard(string boggles[16], Grid<char> &board) {
    int i = 0;                              //used to tell which cube we are on.
    shuffle(boggles,width * height);        //shuffle so it is different every time
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            char newChar = (stringToChar(boggles[i].substr(randomInteger(0,boggles[i].length() - 1),1))); //picks a random letter from a cube
            board.set(row,col, newChar);
            BoggleGUI::labelCube(row,col,newChar,false);
            i++;
        }
    }
}
//creates a board based on user input
void Boggle::createBoard(string input) {
    for(int row = 0; row < width; row++) {
        for(int col = 0; col < height; col ++) {
            board[row][col] = stringToChar(toUpperCase(input.substr(col + (width * row), 1)));
        }
    }
    BoggleGUI::labelAllCubes(input);
}
//returns the value at the certain position
char Boggle::getLetter(int row, int col) {
    if(!board.inBounds(row,col))
        throw -1;
    else
        return board.get(row,col);
}
//checks whether the given word string is suitable to search for on the board for person1
bool Boggle::checkWord(string word) {
    if(!dictionary.contains(word) || word.length() < 4 || correctHumanGuesses.contains(word) || !humanWordSearch(word) || computerGuesses.contains(word)) {
        return false;
    }
    return true;
}
//checks word for person2
bool Boggle::checkWord2(string word) {
    if(!dictionary.contains(word) || word.length() < 4 || computerGuesses.contains(word) || correctHumanGuesses.contains(word) || !humanWordSearch2(word) ) {
        return false;
    }
    return true;
}
// returns a boolean result of whether the word can be formed and will add it to human1's set
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    for(int row = 0; row < width; row++) {
        for(int col = 0; col < height; col++) {
            BoggleGUI::clearHighlighting();
            BoggleGUI::setHighlighted(row,col,true);
            if(humanWordSearch(word,row,col,board)) {
                if(!correctHumanGuesses.contains(word) && !computerGuesses.contains(word)) {
                    correctHumanGuesses.add(word);
                    BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
                    humanScore += word.length() - 3;
                    BoggleGUI::setScore(humanScore,BoggleGUI::HUMAN);
                }

                return true;
            }
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::setHighlighted(row,col,false);
        }
    }
    BoggleGUI::clearHighlighting();
    return false;
}
// returns a boolean result of whether the word can be formed and will add it to human2's set
bool Boggle::humanWordSearch2(string word) {
    BoggleGUI::clearHighlighting();
    for(int row = 0; row < width; row++) {
        for(int col = 0; col < height; col++) {
            BoggleGUI::clearHighlighting();
            BoggleGUI::setHighlighted(row,col,true);
            if(humanWordSearch(word,row,col,board)) {
                if(!correctHumanGuesses.contains(word) && !computerGuesses.contains(word)) {
                    computerScore += word.length() - 3;
                    BoggleGUI::setScore(computerScore,BoggleGUI::COMPUTER);
                    computerGuesses.add(word);
                    BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
                }
                return true;
            }
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::setHighlighted(row,col,false);
        }
    }
    BoggleGUI::clearHighlighting();
    return false;
}
//recursive function that sees if the word can be made
bool Boggle::humanWordSearch(string word, int row, int col, Grid<char> boardCopy) {
    if(!boardCopy.inBounds(row,col)) {
        return false;
    }
    else if(word.length() == 0) {
        return true;
    }
    else if(boardCopy[row][col] == '.') {
        return false;
    }
    else if(boardCopy[row][col] == stringToChar(word.substr(0,1))) {
        BoggleGUI::setHighlighted(row,col,true);
        BoggleGUI::setAnimationDelay(100);
        string newword = word.substr(1);
        boardCopy[row][col] = '.';
        return(humanWordSearch(newword,row,col + 1, boardCopy) || humanWordSearch(newword,row + 1,col + 1, boardCopy)) || humanWordSearch(newword,row + 1,col, boardCopy)
                || humanWordSearch(newword,row + 1,col - 1, boardCopy) || humanWordSearch(newword,row,col - 1, boardCopy) || humanWordSearch(newword,row - 1,col - 1, boardCopy)
                || humanWordSearch(newword,row - 1,col, boardCopy) || humanWordSearch(newword,row - 1,col + 1, boardCopy);
    }
    else {
        BoggleGUI::setHighlighted(row,col,true);
        BoggleGUI::setAnimationDelay(100);
        BoggleGUI::setHighlighted(row,col,false);
        return false;
    }
}
//returns human score
int Boggle::getScoreHuman() {
    return humanScore;
}

//return's computer's score
int Boggle::getScoreComputer() {
    return computerScore;
}
//prints out user's words and current score
void Boggle::printScore() {
    cout << endl << "Player one's words (" << (int)correctHumanGuesses.size() << "): " << correctHumanGuesses << endl << "Player one's score: " << getScoreHuman() << endl;
    cout << endl << "Player two's words (" << (int)computerGuesses.size() << "): " << computerGuesses << endl << "Player two's score: " << getScoreComputer() << endl << endl;
}
//victor grid
void Boggle::victory(string x) {
    createBoard(x);
}
//adds time score for person1
void Boggle::timeScore1(long x) {
    int newScore = (int)((double).006 / ((((double)x/60000) + 1) / 1000));
    humanScore += newScore;
}
//add time score for person2
void Boggle::timeScore2(long x) {
    int newScore = (int)((double).006 / ((((double)x/60000) + 1) / 1000));
    computerScore += newScore;
}
//prints out the boggle board
ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int row = 0; row < width; row++) {
        for(int col = 0; col < height; col++) {
            cout << boggle.board.get(row,col);
        }
        cout << endl;
    }
    return out;
}
