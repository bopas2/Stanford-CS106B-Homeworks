// I did two extensions for boggle
// The first extension was adding sounds where I found approprite
// Secondly I created a version of the game where you can play with a friend.
// Because both users are humans, I also made it so the score was based on time,
// so the faster you get a correct guess, the more points one gets.
// this was done using the timer library and a function i created myself
// the time function is : .006 / (((timeTaken/60000) + 1) / 1000) where timeTaken is in milliseconds.

#include "lexicon.h"
#include "Boggle.h"
#include "console.h"
#include "bogglegui.h"
#include "simpio.h"
#include "strlib.h"
#include "timer.h"

void computerTurn(Boggle &game);
bool containsNumberTest(string input);
void twoPlayer(Boggle &game);
void firstPrint(Boggle &game);
Boggle promptRandom(Lexicon &dictionary);
void humanTurn(Boggle &game);
void quit(Boggle &game);
void finalWords(Boggle &game);

void playOneGame(Lexicon& dictionary) {

    Boggle game = promptRandom(dictionary);
    BoggleGUI::playSound("dice-rattle.wav");
    firstPrint(game);
    twoPlayer(game);

}
//basic starter code that must be ran at the start of each game
void firstPrint(Boggle &game) {
    clearConsole();
    cout << "It's your turn!" << endl << game << endl;
    game.printScore();
}
//prompts to see if we should create a random boggle board or if user wants to create their own
Boggle promptRandom(Lexicon &dictionary) {
    while(true) {
        if(getYesOrNo("Do you want to generate a random board? ", "Please type a word that begins with 'y' or 'n'.")) {
            return Boggle(dictionary,"");;
        }
        while(true) {
            string input = getLine("Type the 16 letters to appear on the board: ");
            if(input.length() != 16 || containsNumberTest(input)) {
                cout << "That is not a valid 16-letter board string. Try again." << endl;
            }
            else {
                input = toUpperCase(input);
                return Boggle(dictionary,input);
            }
        }
    }
}
//tests to see if there are numbers in the user input, NO NUMBERS allowed on a boggle board!
bool containsNumberTest(string input) {
    for(int i = 0; i < 10; i++) {
        string test = integerToString(i);
        if(stringContains(input,(test))) {
            return true;
        }
    }
    return false;
}
//human guessing
void twoPlayer(Boggle &game) {
    while(true) {
        Timer timer; long p1time;
        timer.start();      //timer begins to see how long it takes to get a correct guess
        while(true) {
            BoggleGUI::setStatusMessage("Player one, enter a guess!, or enter to quit");
            string guess = getLine("Player one, enter a guess, or enter to quit");
            guess = toUpperCase(guess);
            if(guess.length() == 0) {
                quit(game);
                return;
            }
            if(!game.checkWord(guess)) {
                clearConsole();
                BoggleGUI::playSound("moo.wav");
                BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
                cout << game << endl;
                game.printScore();
            }
            else {
                timer.stop();               //stop timer
                p1time = timer.elapsed();       // get time
                game.timeScore1(p1time);        //calculate bonus points
                clearConsole();
                BoggleGUI::playSound("tinkerbell.wav");
                BoggleGUI::setStatusMessage( "You found a new word! \"" + guess + "\"");
                cout << "You found a new word! \"" << guess << "\"" << endl;
                break;
            }
        }

        cout << game << endl;
        game.printScore();
        cout << "Time : " << p1time / 1000 << " Time bonus: " << (int)((double).006 / ((((double)p1time/60000) + 1) / 1000)) << endl;


        Timer timer1; long p2time;
        timer1.start();
        while(true) {
            BoggleGUI::setStatusMessage("Player Two, enter a guess, or enter to quit");
            string guess = getLine("Player Two, enter a guess, or enter to quit");
            guess = toUpperCase(guess);
            if(guess.length() == 0) {
                quit(game);
                return;
            }
            if(!game.checkWord2(guess)) {
                clearConsole();
                BoggleGUI::playSound("moo.wav");
                BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
                cout << game << endl;
                game.printScore();
            }
            else {
                timer1.stop();
                p2time = timer1.elapsed();
                game.timeScore2(p2time);
                clearConsole();
                BoggleGUI::playSound("tinkerbell.wav");
                BoggleGUI::setStatusMessage( "You found a new word! \"" + guess + "\"");
                cout << "You found a new word! \"" << guess << "\"" << endl;
                break;
            }
        }
        cout << game << endl;
        game.printScore();
        cout << "Time : " << p2time / 1000 << " Time bonus: " << (int)((double).006 / ((((double)p2time/60000) + 1) / 1000)) << endl;
    }
}
void quit(Boggle &game) { // determines who wins

    BoggleGUI::playSound("tweetle.wav");

    if(game.getScoreComputer() > game.getScoreHuman()) {
        cout << endl << "Player TWO Wins!!!!" << endl;
        BoggleGUI::setStatusMessage("Player TWO wins!");
        game.victory("TEAMTWOaWINSaaaa");
    }
    else if(game.getScoreComputer() == game.getScoreHuman()) {
        cout << endl << "TIE!" << endl;
        BoggleGUI::setStatusMessage("TIE!");
        game.victory("aaaaaaaaTIEaaaaa");
    }
    else {
        cout << endl << "Player One Wins!!!!" << endl;
        BoggleGUI::setStatusMessage("Player One wins!");
        game.victory("TEAMONEaWINSaaaa");
    }
}
