#include "InputHandler.h"
#include <iostream>
#include <limits>

std::string InputHandler::readFileName() {
    std::string file_name;
    bool valid;
    do {
        valid = true;
        std::cout << "Board file name: "; getline(std::cin, file_name);
        for(auto c : file_name){
            if(c == ' '){
                valid = false;
                std::cout << "File name can't contain whitespace" << std::endl;
            }
        }
    } while(!valid);
    return file_name;
}

void InputHandler::readSize(int &height, int &width){
    bool valid;
    do{
        valid = true;
        char divider;
        std::cout << "Size (HEIGHT x WIDTH, example: 10 x 10): "; std::cin >> height >> divider >> width;
        if(divider != 'x' || std::cin.bad()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            valid = false;
        }
    } while(!valid);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    m_height = height;
    m_width = width;
}

int InputHandler::readWord(std::string &input){
    bool valid;
    do{
        std::cout << "> ";
        getline(std::cin, input);
        if(input == "exit"){
            return -1;
        }
        else if(input == "display"){
            return 1;
        }
        valid = testInput(input);
    } while(!valid);
    return 0;
}

bool InputHandler::testInput(const std::string &input) {
    if(input[2] != ' ' || input[4] != ' '){
        return false;
    }
    if(input[0] < 'A' || input[0] > 'A' + m_height - 1 || input[1] < 'a' || input[1] > 'a' + m_width - 1){
        std::cout << "Position outside of board boundaries" << std::endl;
        return false;
    }
    if(input[3] != 'H' && input[3] != 'V'){
        std::cout << "Orientation value not allowed (Only 'H' or 'V' is allowed)" << std::endl;
        return false;
    }
    std::string word = input.substr(5);
    for(auto c : word){
        if(c == ' '){
            std::cout << "Word can't contain whitespace" << std::endl;
            return false;
        }
    }
    return true;
}