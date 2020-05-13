#pragma once
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Word.h"

namespace detail{
    /**
     * - Both 'reserved' and 'occupied' are used to filter invalid positions, occupied deals with overlapping words along
     * the same line while reserved checks for side-by-side words.
     * - 'occupied' are Tiles occupied by a word in a given line
     * - 'reserved' are Tiles adjacent to a word in a given line, can only be placed if occupied by a letter belonging
     * to opposite
     */
    struct Tile{
        char letter = ' ';
        std::map<orientation, bool> reserved = {{H, false}, {V, false}};
        std::map<orientation, bool> occupied = {{H, false}, {V, false}};
    };
}

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
    void print(std::ostream &stream = std::cout) const;

private:
    detail::Tile **m_board;
    const short m_width, m_height;
    const std::string m_words_file_name = "WORDS.TXT";
    const std::string m_file_name;

    std::vector<Word> m_word_vector;
    std::ifstream m_words_file;

    /**
     * Get nth position along a line starting in a given position
     * @return (Tile*)
     */
    detail::Tile* getPosition(const std::pair<short, short> &pos, int n, orientation line) const;

    /**
     * Reserve tiles adjacent to n'th letter of a word
     * @param pos - Position of first letter of word
     * @param n - index of letter to reserve
     * @param line - orientation of the word
     * @return (none)
     */
    void reserveAdjacent(const std::pair<short, short> &pos, int n, orientation line);

    /**
     * Verify if word is in m_words_file
     * @return (bool) true if it is, false otherwise
     */
    bool validateWord(const Word &word);

    /**
     * Check is a given word is in the words_file
     * If a text isn't present in WORDS file, then closest words will be suggested
     * @param (std::string) word
     * @return (bool)
     */
    bool searchWord(std::string &text);
};

