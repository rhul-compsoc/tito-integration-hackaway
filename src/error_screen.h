#pragma once
#include <string>

#define ERROR_ACTION_RETRY 0
#define ERROR_ACTION_IGNORE 1

struct ErrorAction {
    int action;
};

/**
 * Asks the user to decide on an action when an error occurs.
 *
 * @param std::string message to show, i.e: error occurred when saving to file
 * @param int errorCode the error code of the exception
 * @return struct ErrorAction the action the user wants to take
 * @see ERROR_ACTION_IGNORE
 * @see ERROR_ACTION_RETRY
 */
struct ErrorAction showErrorMessage(std::string /*message*/,
                                    int /*errorCode*/);
