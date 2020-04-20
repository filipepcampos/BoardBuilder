#pragma once
#include <string>

class InputHandler{
public:
    std::string readFileName();
    void readSize(int &height, int &width);
};