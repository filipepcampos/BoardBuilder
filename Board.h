#pragma once
#include <fstream>
#include <string>

struct Word{
    char vertical_char;
    char horizontal_char;
    char orientation;
    std::string text;
};

struct Tile{
    char letter = ' ';
    bool placed_horizontal = false;
    bool placed_vertical = false;
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
     * Add a word horizontally
     * @param (int) v_pos
     * @param (int) h_pos
     * @param (std::string) word
     * @return (bool)
     */
    bool addHorizontal(int v_pos, int h_pos, const std::string &word);

    /**
     * Add a word vertically
     * @param (int) v_pos
     * @param (int) h_pos
     * @param (std::string) word
     * @return (bool)
     */
    bool addVertical(int v_pos, int h_pos, const std::string &word);

    /**
     * Check is a given word is in the words_file
     * @param (std::string) word
     * @return (bool)
     */
    bool searchWord(const std::string &word);

    Tile **m_board;
    const int m_width, m_height;
    const std::string m_words_file_name = "WORDS_FIXED.TXT";
    long m_total_lines;
    std::ofstream m_file;
    std::ifstream m_words_file;
};

