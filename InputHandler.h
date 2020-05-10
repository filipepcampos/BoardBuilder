#pragma once
#include <string>
#include "Word.h"
#define RED "\u001b[31m"
#define RESET "\u001b[0m"

class InputHandler{
public:
    static std::string readFileName();
    int readSize(short &height, short &width);
    int readWord(Word &word) const;
private:
    short m_height, m_width;
    static bool checkFileName(const std::string &name) ;
    bool checkWordInput(const std::string &input) const;
};