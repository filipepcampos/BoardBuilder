#include <iostream>
#include <algorithm>
#include <utility>
#include "Board.h"
#include "IO.h"

Board::Board(short height, short width, std::string file_name)
    : m_height(height), m_width(width), m_file_name(std::move(file_name))
{
    m_board = new detail::Tile* [height];
    for(int i = 0; i < height; ++i){
        m_board[i] = new detail::Tile[width];
    }
    readWordsFile();
}

Board::~Board(){
    for(int i=0; i < m_height; ++i){
        delete [] m_board[i];
    }
    delete [] m_board;
}

bool Board::save(){
    int count = 0;
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            if(m_board[i][j].letter != ' '){
                count++;
            }
        }
    }
    // Minimum players is 2,since each players must start with 7 letters the board must have at least 14 letters
    if(count >= 14){
        std::ofstream file;
        file.open(m_file_name);
        file << m_height << " x " << m_width << std::endl;
        for(const auto &w : m_board_words){
            file << w;
        }
        print(file);
        file.close();
        return true;
    }
    return false;
}

void Board::print(std::ostream &stream) const{
    if(&stream == &std::cout){
        stream << CLEAR;
    }
    // Print column letters
    stream << "\n  │ ";
    for(char c = 'a'; c < 'a' + m_width; ++c){
        stream << c << " ";
    }
    stream << "\n──┴";

    // Print bar
    for(int i = 0; i < 2 * m_width; ++i){
        stream << "─";
    }
    stream << '\n';

    // Print rest of the board
    for(int i = 0; i < m_height; ++i){
        char row_char = 'A' + i;
        stream << row_char << " │ ";
        for(int j = 0; j < m_width; ++j){
            stream << m_board[i][j].letter << " ";
        }
        stream << '\n';
    }
    stream << '\n';
}

bool Board::addWord(Word &word) {
    if(validateWord(word)){
        orientation line = word.getOrientation();
        std::string text = word.getText();
        std::pair<char, char> pos = word.getPosition();
        for(int i = 0; i < text.length(); ++i){
            detail::Tile *tile = getPosition(pos, i, line);
            tile->letter = text[i];
            tile->occupied[line] = true;
            tile->reserved[line] = true;
            reserveAdjacent(pos, i, line);
        }

        int line_int = line == V ? 1 : 0;
        std::pair<char, char> end_pos{pos.first + (text.length()  - 1) * line_int,
                                      pos.second + (text.length()  - 1) * (1-line_int)};
        occupyEnds(pos, end_pos, line);
        m_board_words.push_back(word);
        return true;
    }
    return false;
}

void Board::reserveAdjacent(const std::pair<short, short> &pos, int n, orientation line) {
    // Reserve tiles on side tile, considering the letter 'O' from this VERTICAL word:
    //    $ D $   -> $ represents a spot that was reserved before
    //    # O #   -> # represents a spot that will be reserved on this call,
    //      G
    // If pos = {0, 5} (pos is the position of the first letter of word), then position of 'O' is (1, 5) with n = 1
    // then new_coordinate would become 4, reserving (1, 4) and 6, reserving (1, 6).
    // If for example width of the board was 6 (max index = 5) the loop would skip to avoid undefined behaviour
    for(int i = -1; i<=1; i+=2){
        std::pair<short, short> positions = pos;
        short &new_coordinate = line == H ? positions.first : positions.second;
        new_coordinate += i;
        const short &max_value = line == H ? m_height : m_width;
        if(new_coordinate < 0 || new_coordinate >= max_value){
            continue;
        }
        getPosition(positions, n, line)->reserved[line] = true;
    }
}

bool Board::validateWord(const Word &word) {
    orientation line = word.getOrientation();
    int line_int = line == V ? 1 : 0;
    std::string text = word.getText();
    std::pair<char, char> pos = word.getPosition();
    std::pair<char, char> end_pos{pos.first + (text.length()  - 1) * line_int,
                                  pos.second + (text.length()  - 1) * (1-line_int)};

    if(pos.first < 0 || pos.second < 0 || end_pos.first >= m_height || end_pos.second >= m_width
        || !validateEachTile(word.getText(), pos, line)){
        return false;
    }
    return searchWord(text);
}

// Occupy Tiles behind first letter and beyond last letter, if not empty -> the word isn't valid
void Board::occupyEnds(const std::pair<short, short> &pos, const std::pair<short, short> &end_pos, orientation line) {
    detail::Tile *tile = getPosition(pos, -1, line);
    if(tile){
        tile->occupied[H] = true;
        tile->occupied[V] = true;
    }
    tile = getPosition(end_pos, 1, line);
    if(tile){
        tile->occupied[H] = true;
        tile->occupied[V] = true;
    }
}
bool Board::validateEachTile(const std::string &text, const std::pair<short, short> &pos, orientation line) {
    for(int i = 0; i < text.length(); ++i){
        detail::Tile *tile = getPosition(pos, i, line);
        orientation opposite_line = line == V ? H : V;
        if((tile->letter != ' ' && tile->letter != text[i])
           || (tile->reserved[line] && !tile->occupied[opposite_line]) // Can only be placed on reserved tile if it's occupied by other line
           || tile->occupied[line]){
            return false;
        }
    }
    return true;
}

detail::Tile* Board::getPosition(const std::pair<short, short> &pos, int n, orientation line) const{
    short line_int = line == V ? 1 : 0;
    int v = pos.first + n * line_int;
    int h = pos.second + n * (1-line_int);
    if(v < 0 || v >= m_height || h < 0 || h >= m_width){
        return nullptr;
    }
    return &m_board[v][h];
}

void Board::readWordsFile(){
    std::ifstream words_file{m_words_file_name};
    std::string buffer;
    while(getline(words_file, buffer) && !buffer.empty()){
        std::stringstream{buffer} >> buffer; // Solves problem of /r at end of lines
        m_words_set.insert(buffer);
    }
    words_file.close();
}

bool Board::searchWord(std::string text) const{
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    if(m_words_set.find(text) == m_words_set.end()){
        suggestWords(text);
        return false;
    }
    return true;
}

void Board::suggestWords(const std::string &text) const {
    auto it = m_words_set.upper_bound(text); // Can range from  set.begin() (first word) to set.end() (beyond last word)
    // it points to first word > text (or set.end()), std::prev(it) will point to last word < text
    std::string suggest1 = it != m_words_set.end() ? *it : ""; // Get word immediately after text
    std::string suggest2 = it != m_words_set.begin() ? *(std::prev(it)) : ""; // Get word immediately before
    IO::suggestionMessage(suggest2, suggest1);
}