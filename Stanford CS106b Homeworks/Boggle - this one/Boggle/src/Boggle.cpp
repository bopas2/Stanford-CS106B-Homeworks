//Thomas Lang's Boggle
//No specific comments needed

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
    board.resize(width,height);     BoggleGUI::initialize(width,height);
    //resize the board and initialized the board
    if(boardText.length() == 0) {
        createBoard(CUBES,board);   // creates a random board if no string input
    }
    else {
        createBoard(boardText); // creates board with user input
    }
}
//creates a random board
void Boggle::createBoard(string boggles[16], Grid<char> &board) {
    int i = 0;                              //used to tell which cube we are on.
    shuffle(boggles,width * height);            //shuffle so it is different every time
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            char newChar = (stringToChar(boggles[i].substr(randomInteger(0,boggles[i].length() - 1),1))); //picks a random letter from a cube
            board.set(row,col, newChar);
            BoggleGUI::labelCube(row,col,newChar,false);    //creating the grid
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
    if(!board.inBounds(row,col)) throw -1;
    else return board.get(row,col);
}
//checks whether the given word string is suitable to search for on the board
bool Boggle::checkWord(string word) {
    if(!dictionary.contains(word) || word.length() < 4 || correctHumanGuesses.contains(word) || !humanWordSearch(word)) {
        return false;
    }
    return true;
}
// returns a boolean result of whether the word can be formed
bool Boggle::humanWordSearch(string word) {
    for(int row = 0; row < width; row++) {
        for(int col = 0; col < height; col++) {     //we want to search starting from each individual letter
            BoggleGUI::clearHighlighting();
            BoggleGUI::setHighlighted(row,col,true);
            if(humanWordSearch(word,row,col,board)) {       // call the recursive function to see if the word can
                if(!correctHumanGuesses.contains(word)) {   // be created from that specific starting point
                    humanScore += word.length() - 3;
                    BoggleGUI::setScore(humanScore,BoggleGUI::HUMAN);
                    BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
                }
                correctHumanGuesses.add(word);
                return true;
            }
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
        return false;                           // '.' represents we've already visited that cell
    }
    else if(boardCopy[row][col] == stringToChar(word.substr(0,1))) {    //if the char in the cell is the char that we are looking for
        BoggleGUI::setHighlighted(row,col,true);
        BoggleGUI::setAnimationDelay(100);
        string newword = word.substr(1);
        boardCopy[row][col] = '.';
        return(humanWordSearch(newword,row,col + 1, boardCopy)
               || humanWordSearch(newword,row + 1,col + 1, boardCopy))
               || humanWordSearch(newword,row + 1,col, boardCopy)
               || humanWordSearch(newword,row + 1,col - 1, boardCopy)       //checks all neighbors for next word
               || humanWordSearch(newword,row,col - 1, boardCopy)
               || humanWordSearch(newword,row - 1,col - 1, boardCopy)
               || humanWordSearch(newword,row - 1,col, boardCopy)
               || humanWordSearch(newword,row - 1,col + 1, boardCopy);
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
//finds all the remaining words that the user hasn't found
string Boggle::computerWordSearch(string word, int row, int col, Grid<char> &board) {
    if(word.length() >= width && dictionary.contains(word) && !correctHumanGuesses.contains(word) && !computerGuesses.contains(word)) {
        computerGuesses.add(word);
        BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
        computerScore += word.length() - 3;
        setScore(computerScore,BoggleGUI::COMPUTER);
    }
    if(!board.inBounds(row,col)) {
        return word;
    }
    else if(board[row][col] == '.' && word.length() > 0) {
        return word;
    }
    else if(!dictionary.containsPrefix(word)) {
        return word;    //speeds up program significantly
    }
    char x = board[row][col];
    string newWord = word + x;
    board[row][col] = '.';
    for(int a = -1; a < 2; a++) {           //checks neighbors
        for(int b = -1; b < 2; b++) {
            computerWordSearch(newWord, row + a, col + b, board);
        }
    }
    board[row][col] = x;

    return "";
}
//starts the recursive search, must begin at each individual cell
Set<string> Boggle::computerWordSearch() {
    for(int row = 0; row < width; row++ ) {
        for(int col = 0; col < height; col++) {
            computerWordSearch("",row,col,board);
        }
    }
    return computerGuesses;
}
//return's computer's score
int Boggle::getScoreComputer() {
    return computerScore;
}
//prints out user's words and current score
void Boggle::printScore() {
    cout << endl << "Your words (" << (int)correctHumanGuesses.size() << "): " << correctHumanGuesses << endl << "Your score: " << getScoreHuman() << endl;
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
