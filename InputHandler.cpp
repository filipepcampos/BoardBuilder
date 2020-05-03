#include "InputHandler.h"
#include <iostream>

std::string InputHandler::readFileName() {
    std::string file_name;
    bool valid;
    do {
        std::cout << "Board file name: "; getline(std::cin, file_name);

        if(std::cin.eof()){
            std::cout << "EOF has occurred" << std::endl;
            file_name.clear();
            return file_name;
        }
        else if(!file_name.empty()){
            valid = checkFileName(file_name);
        }
        else{
            std::cout << "File name can't be empty" << std::endl;
            valid = false;
        }

    } while(!valid);
    file_name.append(".txt");
    return file_name;
}

bool InputHandler::checkFileName(const std::string &name) const {
    if(name == "WORDS"){
        std::cout <<  "Can't use reserved name as file name" << std::endl;
        return false;
    }
    for(auto c : name){
        if(c == ' '){
            std::cout << "File name can't contain whitespace" << std::endl;
            return false;
        }
        else if(c == '.'){
            std::cout << "File name can't contain '.'" << std::endl;
            return false;
        }
    }
    return true;
}

int InputHandler::readSize(short &height, short &width){
    bool valid;
    short h, w;
    do{
        valid = true;
        std::string input;
        std::cout << "Size (10 x 10 for example): "; std::getline(std::cin, input, '\n');
        if(std::cin.eof()){
            std::cout << "EOF has occurred" << std::endl;
            return -1;
        }

        if(std::sscanf(input.c_str(), "%hd x %hd", &h, &w ) != 2){
            std::cout << "Invalid input" << std::endl;
            valid = false;
        }
        else if(h <= 0 || h > 20 || w <= 0 || w > 20){
            std::cout << "Invalid size" << std::endl;
            valid = false;
        }
    }while(!valid);
    height = h; m_height = h;
    width = w; m_width = w;
    return 0;
}

int InputHandler::readWord(Word &word){
    bool valid;
    std::string input;
    do{
        std::cout << "> ";
        getline(std::cin, input);
        if(input == "exit"){
            return -1;
        }
        else if(input == "display"){
            return 1;
        }
        valid = checkWordInput(input);
    } while(!valid);
    word = Word{input[0], input[1], input[3], input.substr(5)};
    return 0;
}

bool InputHandler::checkWordInput(const std::string &input) const {
    if(input[2] != ' ' || input[4] != ' '){
        std::cout << "Input wrongly formatted" << std::endl;
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