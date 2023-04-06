/**
 * @file        util/askUserOK.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of askUserOK()
 * @copyright   Copyright (c) 2023
 */

#include "log.h"
#include "leafdebug.h"
#include "config.h"

#include "util.h"

#include <filesystem>
#include <iostream>

bool LeafUtil::askUserOK(Leaf::config& conf, std::string question, bool defaultOption){
    FUN();
    LEAF_DEBUG("LeafUtil::askUserOK()");

    if (conf.ask != Leaf::ASK_ASK){
        switch (conf.ask){
        case Leaf::ASK_DEFAULT:
            return defaultOption;
        case Leaf::ASK_YES:
            return true;
        case Leaf::ASK_NO:
            return false;
        }
    }

    LEAF_DEBUG("LeafUtil::askUserOK()-noAsk");

    if (defaultOption){
        LOGD("User question: Default action: true");

        std::cout << question << " (Y/n): ";
        std::string answer;
        getline(std::cin, answer);

        LOGD("User question: Answer: \"" + answer + "\"");

        if (answer == "y" || answer == "Y" || answer == ""){
            LOGD("User question: User agreed");
            return true;
        } else {
            LOGD("User question: User disagreed");
            return false;
        }
    }
    else
    {
        std::cout << question << " (y/N): ";
        std::string answer;
        getline(std::cin, answer);

        if (answer == "n" || answer == "N" || answer == "")
            return false;
        else
            return true;
    }
}
