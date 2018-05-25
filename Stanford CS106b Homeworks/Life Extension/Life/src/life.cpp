//Thomas Lang's 'Game of Life' with extensions, the extensions include a random function and the GUI
// lines of code that refer to the random grid : 18,45-61,265-280
//The random grid passthrough had two seprate things. First I had to see if the user wanted a random grid or not and then act appropritely this is 45-61
//The second half was the function that actually made the random grid. I took a random number between 0 and 100. If # < 50 then that location in the
//grid would have an "X" otherwise it would have a "-". This new grid is used instead of the user input, the rest of the code uses the random code
//as if it was inputted by the user, and functions the same way. This function is on line 265-280.
//lines of code that refer to gui : 45,171-184
// for the gui I basically just edited my print function so instead of printing to the seprate screen it would draw the cell, fit into the loop
//fine. I had to edit the print method's parameters to include a GUI object, but not too much was changed in the code.



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
#include "lifegui.h"
using namespace std;

//state functions so main can use them
void random(Grid<string> &board, int row, int collumn, LifeGUI &typea); //function to create a random grid - EXTENSION
void print(Grid<string> &board, int row, int collumn, LifeGUI &typea);              //prints the board onto screen
int countNeighborsNW(Grid<string> &board, int row, int collumn);    //counts the # of neighbors for non-wrapped runthrough
int countNeighborsW(Grid<string> &board, int row, int collumn);     //counts the # of neighbors for wrapped runthrough
void nextTickNW(Grid<string> &board, int row, int collumn);         //changes the grid based on set rules (not wrapped)
void nextTickW(Grid<string> &board, int row, int collumn);          //changes the grid based on set rules (yes wrapped)

int main() {

    //Begin by greeting the user and stating rules

    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl << endl;

    //Obtain user data or ask for new file name

    string inputdata; string inputrand;
    int fileDNE = 0;  int randnum = 0;
    Grid<string> lifeBoard; int collumn; int row;
    LifeGUI lifeBoardGUI;

    while(randnum == 0) {
        inputrand = getLine("Random Colony? (y/n) ");       //determins if user wants random colony
        inputrand = toLowerCase(inputrand);         // so it isnt case sensitive
        if(inputrand.compare("y") == 0) {           //yes?
            int randrow = 30;                       //random grid by default is 30x30
            int randcol = 30;
            row = randrow; collumn = randcol;
            random(lifeBoard,randrow,randcol,lifeBoardGUI);         //calls random to create a random new grid
            fileDNE = 1; randnum = 2; // by setting fileDNE to 1 this tells the program that the user won't be inputing a file and by setting randnum
        }                             // to two tells the program that we skip opening any files and skip creating a new grid, since we already have.
        else if(inputrand.compare("n") == 0) {
            randnum = 1;              //ends this loops and asks for file, since user doesnt want a random  grid
        }
        else {
            cout << "Invalid input, try again. " << endl;       //reprompt
        }
    }

    while(fileDNE == 0) {                                   //while the user hasn't entered a correct input, reprompt
        inputdata = getLine("Grid input file name? ");
        bool doesfilexist = fileExists(inputdata);          //see if file actually exists or user needs to be re-prompted
        if(doesfilexist == 1) {
            fileDNE = 1;                                    //ends while loop allowing program to proceed IF the file exists
        }
        if(doesfilexist == 0) {
            cout << "Unable to open that file.  Try again." << endl; //reprompt
        }
    }

    //Ask if simulation should run wrapped

    string wrapped;
    int correctinput = 0;
    while(correctinput == 0) {                                  //Ask user if simulation should run wrapped, reprompt if invalid input
        wrapped = getLine("Should the simulation wrap around the grid (y/n)? ");
        wrapped = toLowerCase(wrapped);                         //removes case sensitivity
        if(wrapped.compare("y") == 0 || wrapped.compare("n") == 0) {
            correctinput = 1;                                   //checks for valid input, and end loop if input is valid
        }
        else {
            cout << "Invalid choice; please try again." << endl; //reprompt
        }
    }

    //implement the grid
    if(randnum != 2) {          //this is new, basically if the user wants the random grid we dont need to open any files, randnum is set to two
        string s;               //previouslly if the user wanted a random grid
        ifstream stream;                            //opening the stream / file
        openFile(stream,inputdata);
        getline(stream,s);
        row = stringToInteger(s);               //read first two lines to instantiate grid with width x height
        getline(stream,s);
        collumn = stringToInteger(s);
        lifeBoard.resize(row,collumn);
        lifeBoardGUI.resize(row,collumn);   //creating GUI and making sure it is correct in size

        //ok grid is initialized, but is filled with zeros, time to fill with user data

        for(int h = 0; h < row; h++) {
            getline(stream,s);
            for(int k = 0; k < collumn; k++) {
                lifeBoard[h][k] = s.substr(k,1); // found out substr isn't like java at all, second val is length not index. copying values from file to grid one by one
            }
        }
        stream.close(); //we're done with user file, lets close it now
    }

    //now the board is filled with data, lets ask the user how they wish to proceed

    print(lifeBoard,row,collumn,lifeBoardGUI);   // outputs the colony without any growth, matches file input
    int quit = 0;
    while(quit == 0) {          //program continues while 'quit' = 0
        int validAnswer = 0;
        string proceed;
        while(validAnswer == 0) {                               // prompting how the user wants to use the simulation
            proceed = getLine("a)nimate, t)ick, q)uit? ");
            proceed = toLowerCase(proceed);                     //case insensitive
            if(proceed.compare("a") == 0 || proceed.compare("t") == 0 || proceed.compare("q") == 0) {
                validAnswer = 1;
            }
            else {
                cout << "Invalid choice; please try again." << endl;        //reprompt for invalid inputs
            }
        }
        if(proceed.compare("q") == 0) { //quits program
            quit = 1;
        }
        else if(proceed.compare("t") == 0 && wrapped.compare("n") == 0) { //forward one non-wrapped growth
            nextTickNW(lifeBoard,row,collumn);
            print(lifeBoard,row,collumn,lifeBoardGUI);
        }
        else if(proceed.compare("t") == 0 && wrapped.compare("y") == 0) {//forward one wrapped growth
            nextTickW(lifeBoard,row,collumn);
            print(lifeBoard,row,collumn,lifeBoardGUI);
        }
        else if((proceed.compare("a") == 0 && wrapped.compare("n") == 0)) { //animate the non-wrapped growth
            nextTickNW(lifeBoard,row,collumn); //prepare next tick grid
            int ani = getInteger("How many frames?" ,"Illegal integer format. Try again."); //frame input
            int ticker = 0;
            while(ticker < ani) {
                clearConsole(); // clear screen
                print(lifeBoard,row,collumn,lifeBoardGUI); //print new colony
                nextTickNW(lifeBoard,row,collumn); //prepare next tick grid
                ticker++;   //proceed with next frame
            }
        }
        else if(proceed.compare("a") == 0 && wrapped.compare("y") == 0) {// animate the wrapped growth
            nextTickW(lifeBoard,row,collumn);
            int ani = getInteger("How many frames?" ,"Illegal integer format. Try again.");
            int ticker = 0;
            while(ticker < ani) { //same methodology
                clearConsole();
                print(lifeBoard,row,collumn,lifeBoardGUI);
                nextTickW(lifeBoard,row,collumn);
                ticker++;

            }
        }
    }
    // end program
    cout << "Have a nice Life!" << endl;
    return 0;
}
//prints the current grid to the screen
void print(Grid<string> &board,int row, int collumn,LifeGUI &typex) { //uses original grid, no copy, saves time/data
    //for printing the board
    for(int h = 0; h < row; h++) {
        for(int k = 0; k < collumn; k++) {
            if(board[h][k].compare("X") == 0) {
                typex.drawCell(h,k,true);   //draws a cell instead of printing it out, also it now has to check if the grid contains an
            }                               // "X" at the location to tell if it should pass true into the draw cell function
            else {
                typex.drawCell(h,k,false);
            }
        }
    }
}
//This function counts how many neighbors a cell has, NW stands for not wrapped
//basically checking nearby cells if they neighbor a cell, because it is wrapped it must be in bounds of the grid, otherwise it is
//ignored
int countNeighborsNW(Grid<string> &board, int row, int collumn) {
    int count = 0;
    if(board.inBounds(row,collumn + 1) == true && board.get(row,collumn + 1).compare("X") == 0)                count++;
    if(board.inBounds(row + 1,collumn + 1) == true && board.get(row + 1,collumn + 1).compare("X") == 0)        count++;
    if(board.inBounds(row + 1,collumn) == true && board.get(row + 1,collumn).compare("X") == 0)                count++;
    if(board.inBounds(row + 1,collumn - 1) == true && board.get(row + 1,collumn - 1).compare("X") == 0)        count++;
    if(board.inBounds(row,collumn - 1) == true && board.get(row,collumn - 1).compare("X") == 0)                count++;
    if(board.inBounds(row - 1,collumn - 1) == true && board.get(row - 1,collumn - 1).compare("X") == 0)        count++;
    if(board.inBounds(row - 1,collumn) == true && board.get(row - 1,collumn).compare("X") == 0)                count++;
    if(board.inBounds(row - 1,collumn + 1) == true && board.get(row - 1,collumn + 1).compare("X") == 0)        count++;
    return count;
}
// countneighbors function but with extra code that considers "wrapped" cells
int countNeighborsW(Grid<string> &board, int row, int collumn) {
    int count = 0;

    //This code checks for locations in bounds (nonwrapped)

    if(board.inBounds(row,collumn + 1) == true && board.get(row,collumn + 1).compare("X") == 0)                count++;
    if(board.inBounds(row + 1,collumn + 1) == true && board.get(row + 1,collumn + 1).compare("X") == 0)        count++;
    if(board.inBounds(row + 1,collumn) == true && board.get(row + 1,collumn).compare("X") == 0)                count++;
    if(board.inBounds(row + 1,collumn - 1) == true && board.get(row + 1,collumn - 1).compare("X") == 0)        count++;
    if(board.inBounds(row,collumn - 1) == true && board.get(row,collumn - 1).compare("X") == 0)                count++;
    if(board.inBounds(row - 1,collumn - 1) == true && board.get(row - 1,collumn - 1).compare("X") == 0)        count++;
    if(board.inBounds(row - 1,collumn) == true && board.get(row - 1,collumn).compare("X") == 0)                count++;
    if(board.inBounds(row - 1,collumn + 1) == true && board.get(row - 1,collumn + 1).compare("X") == 0)        count++;

    //This code checks across the grid for wrapped locations cardinal directions (up,down,right,left)  (wrapped)

    if(board.inBounds(row,(collumn + 1)) == false && (board.get(row,(collumn + 1) % board.width())).compare("X") == 0)       count++;
    if(board.inBounds((row + 1),collumn) == false && (board.get((row+1) % board.height(),collumn)).compare("X") == 0)        count++;
    if(board.inBounds(row,(collumn - 1)) == false && (board.get(row,collumn - 1 + board.width())).compare("X") == 0)         count++;
    if(board.inBounds((row - 1),collumn) == false && (board.get(row - 1 + board.height(),collumn)).compare("X") == 0)        count++;

    //Wrapping Corners

    if(board.inBounds((row+1),(collumn+1)) == false && (board.get((row+1) % board.height(),(collumn+1) % board.width())).compare("X") ==0)                                             count++; //bottom righthand corner
    if(board.inBounds(row - 1,collumn + 1) == false && (board.get((row - 1 + board.height()) % board.height(),(collumn + 1 + board.width()) % board.width())).compare("X") ==0)        count++; //upper righthand corner
    if(board.inBounds(row + 1,collumn - 1) == false && (board.get((row+1+board.height()) % board.height(), ((collumn - 1+board.width()) % board.width()))).compare("X") == 0)          count++; //bottom lefthand corner
    if(board.inBounds(row - 1, collumn - 1) == false && (board.get((row - 1 + board.height()) % board.height(),(collumn - 1 + board.width()) % board.width())).compare("X") == 0)      count++; //upper lefthand corner
    return count;

}
//this function moves the simulation one 'tick' forward, changes the original board once done. NOT WRAPPED
void nextTickNW(Grid<string> &board,int row, int collumn) {
    Grid<string> tempBoard (row,collumn);
    for(int h = 0; h < row; h++) {
        for(int k = 0; k < collumn; k++) {
            int neighborCount = countNeighborsNW(board,h,k); //see how many neighbors a cell has
            if(neighborCount <= 1 || neighborCount >= 4) { //cell dies if it has 1 <= 1 or it has 4+ neighbors
                tempBoard[h][k] = "-";
            }
            else if(neighborCount == 3) {                   //3 neighbors..? cell grows!
                tempBoard[h][k] = "X";
            }
            else
                tempBoard[h][k] = board.get(h,k);    // 2 neighbors..? well we just access original grid and the same value remains
        }
    }
    board = tempBoard; //replace original grid with new colony
}
//forwards the growth for a wrapped board
void nextTickW(Grid<string> &board, int row, int collumn) {
    Grid<string> tempBoard (row,collumn);
    for(int h = 0; h < row; h++) {
        for(int k = 0; k < collumn; k++) {
            int neighborCount = countNeighborsW(board,h,k);
            if(neighborCount <= 1 || neighborCount >= 4) {
                tempBoard[h][k] = "-";
            }
            else if(neighborCount == 3) {
                tempBoard[h][k] = "X";
            }
            else
                tempBoard[h][k] = board.get(h,k);
        }
    }
    board = tempBoard;
}
void random(Grid<string> &board, int row, int collumn,LifeGUI &typea) {
    board.resize(row,collumn);
    typea.resize(row,collumn);
    for(int r = 0; r < row; r++) {
        for(int c = 0; c < collumn; c++) {
            int randnum = randomInteger(0,100);  //random num 0-100
            if(randnum > 50) {                  // randnum > 50 means that spot has an "X" otherwise it has an "-"
                board[r][c] = "X";
            }
            else {
                board[r][c] = "-";
            }
        }
    }
}

//I hope my code wasn't too confusing =)
// -Thomas
