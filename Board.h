#pragma once
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "Word.h"

struct Tile{
    char letter = ' ';
    std::map<orientation, bool> placed = {{H, false}, {V, false}};
};

class Board{
public:
    /**
     * Dynamically allocate space for 2D board given the dimensions
     * @param (int) height - Number of rows
     * @param (int) width - Number of columns
     */
    Board(short height, short width, std::string file_name);

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

    /**
     * Write board to file
     * @return (none)
     */
    bool save();

    /**
     * Output the 2D representation of the game board to a ostream (std::cout by default)
     * @return (none)
     */
    void print(std::ostream &stream = std::cout);

private:
    Tile **m_board;
    const short m_width, m_height;
    const std::string m_words_file_name = "WORDS.TXT";
    const std::string m_file_name;

    std::ifstream m_words_file;
    std::stringstream m_input_words;

    /**
     * Get nth position along a line starting in a given position
     * @return (Tile*)
     */
    Tile* getPosition(const std::pair<short, short> &pos, int n, orientation line) const;

    /**
     * "Place" tiles on adjacent lines to block side-by-side words
     * @param pos
     * @param n
     * @param line
     * @return (none)
     */
    void placeAdjacent(const std::pair<short, short> &pos, int n, orientation line);

    /**
     * Verify if word is in m_words_file
     * @return (bool) true if it is, false otherwise
     */
    bool validateWord(const Word &word);

    /**
     * Check is a given word is in the words_file
     * @param (std::string) word
     * @return (bool)
     */
    bool searchWord(std::string &text);
};

