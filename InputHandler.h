#pragma once
#include <string>
#include "Word.h"

class InputHandler{
public:
    std::string readFileName();
    void readSize(short &height, short &width);
    int readWord(Word &word);
private:
    short m_height, m_width;
    bool checkFileName(const std::string &name) const;
    bool checkWordInput(const std::string &input) const;
};