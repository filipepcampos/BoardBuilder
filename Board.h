#pragma once
#include <fstream>
#include <string>

struct Word{
    char vertical_char;
    char horizontal_char;
    char orientation;
    std::string word;
};

class Board{
public:
    /**
     * Dynamically allocate space for 2D board given the dimensions
     * @param (int) height - Number of rows
     * @param (int) width - Number of columns
     */
    Board(int height, int width, std::string file_name);

    /**
     * Free the allocated space for the board
     */
    ~Board();

    /**
     * Add a word to the board if it's valid
     * @param word
     * @return (bool) true if it's valid, false if it's not
     */
    bool addWord(Word &word);

    void save();

    /**
     * Output the 2D representation of the game board to a ostream (std::cout by default)
     * @return (none)
     */
    void print(std::ostream &stream = std::cout);

private:
    bool addHorizontal(int v_pos, int h_pos, const std::string &word);
    bool addVertical(int v_pos, int h_pos, const std::string &word);
    char **board;
    int width, height;
    std::ofstream file;
};

