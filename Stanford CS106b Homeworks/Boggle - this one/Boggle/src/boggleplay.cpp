//Thomas Lang's Boggle
//No significant comments to be said

#include "lexicon.h"
#include "Boggle.h"
#include "console.h"
#include "bogglegui.h"
#include "simpio.h"
#include "strlib.h"

void computerTurn(Boggle &game);
bool containsNumberTest(string input);
void firstPrint(Boggle &game);
Boggle promptRandom(Lexicon &dictionary);
void humanTurn(Boggle &game);

void playOneGame(Lexicon& dictionary) {

    Boggle game = promptRandom(dictionary);
    firstPrint(game);
    BoggleGUI::setStatusMessage("It's your turn! ");
    humanTurn(game);
    computerTurn(game);

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
//tests to see if there are numbers in the user input, NO NUMBERS allowed on a boggle board, so user shouldn't be able to input numbers
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
void humanTurn(Boggle &game) {
    while(true) {
        string guess = getLine("Type a word (or Enter to stop): ");
        guess = toUpperCase(guess);
        if(guess.length() == 0) {
            clearConsole();
            cout << "Game over!" << endl << game << endl; //final messages
            game.printScore();
            BoggleGUI::setStatusMessage("Game over! ");
            break;
        }
        if(!game.checkWord(guess)) {
            clearConsole();
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
            cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
        else {
            clearConsole();
            BoggleGUI::setStatusMessage( "You found a new word! \"" + guess + "\"");
            cout << "You found a new word! \"" << guess << "\"" << endl;
        }
        cout << game << endl;
        game.printScore();
    }
}
//computer output
void computerTurn(Boggle &game) {
    cout << endl << "It's my turn!" << endl;
    Set<string> computerWords = game.computerWordSearch();
    cout << "My words (" << computerWords.size() << "): " << computerWords << endl;
    cout << "My Score: " << game.getScoreComputer() << endl;
    if(game.getScoreComputer() >= game.getScoreHuman()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Computer Victory!");
    }
    else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("Human Victory!");
    }
}
