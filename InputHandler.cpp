#include "InputHandler.h"
#include <iostream>
#include <limits>

std::string InputHandler::readFileName() {
    std::string file_name;
    bool valid;
    do {
        std::cout << "Board file name: "; getline(std::cin, file_name);

        if(std::cin.eof()){
            std::cout << RED <<"EOF" << RESET << " has occurred" << std::endl;
            file_name.clear();
            return file_name;
        }
        else if(!file_name.empty()){
            valid = checkFileName(file_name);
        }
        else{
            std::cout << "File name can't be " << RED << "empty" << RESET << std::endl;
            valid = false;
        }

    } while(!valid);
    file_name.append(".txt");
    return file_name;
}

bool InputHandler::checkFileName(const std::string &name) {
    if(name == "WORDS"){
        std::cout <<  "Can't use " << RED << "reserved name" << RESET " as file name" << std::endl;
        return false;
    }
    for(auto c : name){
        if(!isalnum(c)){
            std::cout << "File name must contain only " << RED << "alphanumeric characters without whitespace" << RESET << std::endl;
            return false;
        }
    }
    return true;
}

int InputHandler::readSize(short &height, short &width){
    bool valid;
    do{
        valid = true;
        char divider;
        std::cout << "Size (10 x 10 for example): ";
        std::cin >> m_height >> divider >> m_width;
        if(std::cin.eof()){
            std::cout << RED << "EOF" << RESET << " has occurred" << std::endl;
            return -1;
        }
        if(std::cin.fail()){
            valid = false;
            std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(divider != 'x' && divider != 'X'){
            std::cout << RED << "Invalid input" << RESET << std::endl;
            valid = false;
        }
        else if(m_height <= 0 || m_height > 20 || m_width <= 0 || m_width > 20 || m_width * m_height < 14){
            std::cout << RED << "Invalid size" << RESET << std::endl;
            valid = false;
        }
    }while(!valid);
    height = m_height;
    width = m_width;
    return 0;
}

int InputHandler::readWord(Word &word) const{
    bool valid;
    std::string input;
    do{
        std::cout << "> ";
        getline(std::cin, input);
        if(input == "exit" || std::cin.eof()){
            return -1;
        }
        else if(input == "display"){
            return 1;
        }
        valid = checkWordInput(input);
    } while(!valid);
    word.setValues(input[0], input[1], input[3], input.substr(5));
    return 0;
}

bool InputHandler::checkWordInput(const std::string &input) const {
    if(input[2] != ' ' || input[4] != ' '){
        std::cout << RED << "Input wrongly formatted" << RESET << std::endl;
        return false;
    }
    if(input[0] < 'A' || input[0] > 'A' + m_height - 1 || input[1] < 'a' || input[1] > 'a' + m_width - 1){
        std::cout << RED << "Position outside of board boundaries" << RESET << std::endl;
        return false;
    }
    if(input[3] != 'H' && input[3] != 'V'){
        std::cout << RED << "Orientation value not allowed (Only 'H' or 'V' is allowed)" << RESET << std::endl;
        return false;
    }
    std::string word = input.substr(5);
    for(auto c : word){
        if(!isalpha(c)){
            std::cout << "Word must contain only " << RED << "alphabetic characters without whitespace" << RESET << std::endl;
            return false;
        }
    }
    return true;
}