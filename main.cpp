#include <iostream>
#include <string>
#include "Board.h"
#include "InputHandler.h"

int main() {
    InputHandler input_handler;
    std::string filename = input_handler.readFileName();

    int height, width;
    input_handler.readSize(height, width);

    Board b(height, width, filename);
    std::cout << "Please write words as follows: 'Aa H WORD'\n" << "Aa is the position of the first letter\n" <<
              "H or V represents the orientation of the word (vertical or horizontal)" <<
              "WORD is the word you wish to output\n" << "If you wish to see current board type 'display\n" <<
              "When done please type 'exit' with no whitespace\n";

    bool exit = false;
    while(!std::cin.eof() && !exit){
        std::cout << "> ";
        std::string input;
        getline(std::cin, input);

        if(input == "exit"){
            exit = true;
            continue;
        }
        else if(input == "display"){
            b.print();
            continue;
        }
        else if(input[2] != ' ' || input[4] != ' '){
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        char vertical_char = input[0], horizontal_char = input[1], orientation = input[3];
        std::string word = input.substr(5);
        Word w{vertical_char, horizontal_char, orientation, word};
        if(!b.addWord(w)){
            std::cout << "Invalid word, please try again" << std::endl;
        }
    }
    b.save();
    return 0;
}