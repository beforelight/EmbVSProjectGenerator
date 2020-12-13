//
// Created by 17616 on 2020/12/12.
//

#include "error.h"
error::error(const std::string &_msg, const std::string &file, const std::string &function, size_t line)
        : msg(file + ":" + std::to_string(line) + ":" + function + ":\t" + _msg) {}
const char *error::what() const  {
    return msg.c_str();
}
