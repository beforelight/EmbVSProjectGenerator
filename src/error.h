//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_ERROR_H
#define VSPG_ERROR_H

#include <string>
class error : public std::exception {
public:
    error(const std::string &_msg, const std::string &file, const std::string &function, size_t line);
    const char *what() const  override;
private:
    std::string msg;
};
#define ERROR(msg) error(msg,__FILE__,__FUNCTION__ ,__LINE__)


#endif //VSPG_ERROR_H
