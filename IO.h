#pragma once
#include <string>
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
    int readSize(short &height, short &width);

    /**
     * Read a word or special command from user
     * 'instructions' - this will display the instructions again
     * 'exit' - send signal to exit program
     * @param word
     * @return -1 to exit program, 0 if a word was read, 1 if read was invalid
     */
    int readInput(Word &word) const;
private:
    short m_height, m_width;

    /**
     * Make sure file name is allowed
     * @param name
     * @return
     */
    static bool checkFileName(const std::string &name);

    /**
     * Make sure word is allowed
     * @param input
     * @return
     */
    bool checkWordInput(const std::string &input) const;

    /**
     * Output an error message
     * @param s
     */
    static void error(const std::string &s, bool wait=true);

    static std::string inputPrompt(const std::string &message);

    /**
     * Wait for user to press a key to continue execution
     */
    static void pressToContinue();
};