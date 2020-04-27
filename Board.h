#pragma once
#include <fstream>
#include <string>
#include <map>

struct Word{
    char vertical_char;
    char horizontal_char;
    char orientation;
    std::string text;
};

struct Tile{
    char letter = ' ';
    std::map<char, bool> placed = {{'H', false}, {'V', false}};
};

class Board{
public:
    /**
     * Dynamically allocate space for 2D board given the dimensions
     * @param (int) height - Number of rows
     * @param (int) width - Number of columns
     */
    Board(int height, int width, const std::string &file_name);

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
     * Write 2D Board to file and close fstreams
     * @return (none)
     */
    void save();

    /**
     * Output the 2D representation of the game board to a ostream (std::cout by default)
     * @return (none)
     */
    void print(std::ostream &stream = std::cout);

private:
    /**
     * Verify if word is in m_words_file
     * @return (bool) true if it is, false otherwise
     */
    bool validateWord(Word &word);

    /**
     * Check is a given word is in the words_file
     * @param (std::string) word
     * @return (bool)
     */
    bool searchWord(std::string &word);

    Tile **m_board;
    const int m_width, m_height;
    const std::string m_words_file_name = "WORDS.TXT";

    std::ofstream m_file;
    std::ifstream m_words_file;
};

