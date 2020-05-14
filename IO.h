#pragma once
#include <string>
#include <sstream>
#include "Word.h"
#define RED "\u001b[31m"
#define BLUE "\u001b[36m"
#define RESET "\u001b[0m"
#define CLEAR "\033[2J\033[1;1H";

class IO{
public:
    /**
     * Display program title
     */
    static void displayTitle();

    /**
     * Display program instructions
     */
    static void instructions();

    /**
     * Output invalid action error
     */
    static void invalidAction();

    /**
     * Output message if board was or wasn't saved
     * @param saved
     */
    static void savedMessage(bool saved);

    /**
     * Output word suggestions after failed word search
     * @param s
     */
    static void suggestionMessage(const std::string &s);
    static void suggestionMessage(const std::string &s1, const std::string &s2);

    /**
     * Read a file name
     * @return file name read or empty string if EOF has occurred
     */
    static std::string readFileName();

    /**
     * Read board size through reference
     * @param height
     * @param width
     * @return 0 if read was successfull, -1 if EOF occurred
     */
    static int readSize(short &height, short &width);

    /**
     * Read a word or special command from user
     * 'instructions' - this will display the instructions again
     * 'exit' - send signal to exit program
     * @param word
     * @return -1 to exit program, 0 if a word was read, 1 if read was invalid
     */
    static int readWordInput(Word &word);
private:
    /**
     * Output an error message
     * @param s
     */
    static void error(const std::string &s, bool wait=true);

    /**
     * Wait for user to press a key to continue execution
     */
    static void pressToContinue();

    template <typename T>
    static bool read(T &var, bool(*test)(const T &var), bool(*convert)(T &var, const std::string &str));

    static bool stringConverter(std::string &var, const std::string &str);

    static bool testFileName(const std::string &name);
    static bool sizeConverter(std::pair<short, short> &size, const std::string &str);
    static bool testSize(const std::pair<short, short> &size);
    static bool testWordInput(const std::string &input);
};