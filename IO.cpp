#include "IO.h"
#include <iostream>
#include <sstream>
#include <limits>

#define BLUE "\u001b[36m"
#define RED "\u001b[31m"
#define RESET "\u001b[0m"

void IO::instructions() {
    std::cout << CLEAR;
    std::cout << "\n\nPlease write words as follows: '" << BLUE << "Aa H WORD" << RESET << "'\n" <<
              BLUE << "Aa" << RESET << " is the position of the first letter\n" <<
              BLUE << "H" << RESET << " or " << BLUE << "V" << RESET <<
              " represents the orientation of the word (vertical or horizontal)\n" <<
              BLUE << "WORD" << RESET << " is the word you wish to output" <<
              "\nIf you wish to see these instructions again type '" << BLUE << "instructions" << RESET <<
              "'\nWhen done please type '" << RED << "exit" << RESET <<"' with no whitespace\n" <<
              "If your board doesn't have at least 14 letters it won't be saved\n\n";
    pressToContinue();
    std::cout << CLEAR;
}

void IO::invalidAction() {
    error("Invalid action, please try again");
}

void IO::savedMessage(bool saved){
    if(saved){
        std::cout << BLUE << "Board has been saved" << RESET << std::endl << std::endl;
    }
    else{
        error("Board wasn't saved due to lack of letters", false);
    }
}

void IO::suggestionMessage(const std::string &s1, const std::string &s2){
    std::cout << "Suggestion: " << BLUE << s1 << RESET << " or " << BLUE << s2 << RESET << std::endl;
}
void IO::suggestionMessage(const std::string &s){
    std::cout << "Suggestion: " << BLUE << s << RESET << std::endl;
}

void IO::displayTitle(){
    std::cout << CLEAR;
    std::cout << BLUE << "______                     _  ______       _ _     _           \n"
                         "| ___ \\                   | | | ___ \\     (_) |   | |          \n"
                         "| |_/ / ___   __ _ _ __ __| | | |_/ /_   _ _| | __| | ___ _ __ \n"
                         "| ___ \\/ _ \\ / _` | '__/ _` | | ___ \\ | | | | |/ _` |/ _ \\ '__|\n"
                         "| |_/ / (_) | (_| | | | (_| | | |_/ / |_| | | | (_| |  __/ |   \n"
                         "\\____/ \\___/ \\__,_|_|  \\__,_| \\____/ \\__,_|_|_|\\__,_|\\___|_|   \n\n\n" << RESET;
}

void IO::pressToContinue() {
    std::cout << "\nPlease press any key to continue ...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename T>
bool IO::read(T &var, bool(*test)(const T &var), bool(*convert)(T &var, const std::string &str)) {
    std::cout << "\n > ";
    std::string buffer;
    std::getline(std::cin, buffer);

    if(std::cin.eof()){
        var = T{};
        return true;
    }
    if((*convert)(var, buffer)){
        return test(var);
    }
    return false;
}

bool IO::stringConverter(std::string &var, const std::string &str) {
    var = str;
    return true;
}
bool IO::sizeConverter(std::pair<short, short> &size, const std::string &str) {
    std::stringstream ss{str};
    char divider;
    ss >> size.first >> divider >> size.second;
    if(ss.fail() || ss.rdbuf()->in_avail() || ::tolower(divider) != 'x') {
        error("Invalid input", false);
        return false;
    }
    return true;
}

// -------------------------- File name ----------------------------
std::string IO::readFileName() {
    std::string file_name;
    while(true){
        std::cout << "Board file name";
        if(read(file_name, testFileName, stringConverter)){
            break;
        }
    }
    if(!file_name.empty()){
        file_name.append(".txt");
    }
    return file_name;
}
bool IO::testFileName(const std::string &name){
    if(!name.empty()){
        if (name == "WORDS") {
            error("Can't use reserved name as file name", false);
            return false;
        }
        for (auto c : name) {
            if (!isalnum(c)) {
                error("File name must contain only alphanumeric characters without whitespace", false);
                return false;
            }
        }
        return true;
    }
    return false;
}

// ---------------------- Size -------------------------------
int IO::readSize(short &height, short &width) {
    std::pair<short, short> pos{};
    while(true){
        std::cout << "Size (10 x 10 for example)";
        if(read(pos, testSize, sizeConverter)){
            break;
        }
    }
    height = pos.first;
    width = pos.second;
    return height;
}

bool IO::testSize(const std::pair<short, short> &size) {
    if(size.first <= 0 || size.first > 20 || size.second <= 0 || size.second > 20 || size.second * size.first < 14){
        error("Invalid size", false);
        return false;
    }
    return true;
}

// -------------------------- Word input ----------------------------------
int IO::readWordInput(Word &word) {
    std::string raw_word;
    if(read(raw_word, testWordInput, stringConverter)){
        if (raw_word == "exit" || raw_word.empty()) {
            return -1;
        }
        else if (raw_word == "instructions"){
            IO::instructions();
            return 1;
        }
        else{
            word.setValues(raw_word[0], raw_word[1], raw_word[3], raw_word.substr(5));
            return 0;
        }
    }
    return 1;
}

bool IO::testWordInput(const std::string &input) {
    if(input == "exit" || input == "instructions"){
        return true;
    }
    if (input[2] != ' ' || input[4] != ' ') {
        error("Input wrongly formatted");
        return false;
    }
    if (input[3] != 'H' && input[3] != 'V') {
        error("Orientation value not allowed (Only 'H' or 'V' is allowed)");
        return false;
    }
    std::string word = input.substr(5);
    if (word.size() < 2) {
        error("Word must have at least two characters");
        return false;
    }
    for (auto c : word) {
        if (!isalpha(c)) {
            error("Word must contain only alphabetic characters without whitespace");
            return false;
        }
    }
    return true;
}

void IO::error(const std::string &s, bool wait) {
    std::cout << RED << s << RESET << std::endl;
    if(wait){
        pressToContinue();
    }
}
