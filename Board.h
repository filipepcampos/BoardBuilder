#pragma once
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Word.h"

namespace detail{
    /**
     * RESERVED and OCCUPIED will be accessed by (enum orientation, declared in Word.h) index to make code clearer
     *
     * - Both 'reserved' and 'occupied' are used to filter invalid positions, occupied deals with overlapping words along
     *    the same line while reserved checks for invalid side-by-side words.
     * - 'occupied' is true if Tile is occupied by a word in a given line
     * - 'reserved' are Tiles adjacent to a word in a given line, can only be placed if occupied by a letter belonging
     * to opposite line
     * For example, consider this horizontal word:
     *      # # #
     *      D O G
     *      # # #, D through G are occupied and # are reserved
     *
     *      If someone tries to place a word like this:
     *      D O G
     *          O T T E R, it won't be allowed because a letter of the word is in a reserved position
     *
     *      But if the 'reserved' Tile is 'occupied' vertically it will be allowed:
     *      D O G
     *          O T T E R
     *          O
     *          D
     */
    struct Tile{
        char letter = ' ';
        bool reserved[2] = {false, false};
        bool occupied[2] = {false, false};
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
     * @return (bool) true if save was successful
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
     * @param pos - starting position
     * @param n - index of letter to get counting from starting position
     * @param line - H or V
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
     * Check if edges of the word are empty, the edges are the positions
     * before the first letter and beyond the last letter
     * @param pos - Position of first letter
     * @param end_pos - Position of last letter
     * @param line - Orientation of the word
     * @return
     */
    bool validateEdges(const std::pair<short, short> &pos, const std::pair<short, short> &end_pos, orientation line);

    /**
     * Validate each tile of a word, making sure the letter matches up, there isn't any occupied position or
     * any reserved position that's unoccupied by another word
     * @param text
     * @param pos
     * @param line
     * @return
     */
    bool validateEachTile(const std::string &text, const std::pair<short, short> &pos, orientation line);

    /**
     * Check is a given word is in the words_file
     * If a text isn't present in WORDS file, then closest words will be suggested
     * @param (std::string) word
     * @return (bool)
     */
    bool searchWord(std::string &text);
};

