#ifndef MOONLANDER_FSEXCEPTION_HPP
#define MOONLANDER_FSEXCEPTION_HPP

#include <string>
#include "basegameexception.h"

/**
 * Exception class that can be throw when happened some error
 * with file system(can't open file, no access, etc..)
 */
class FSException: public BaseGameException
{
public:
    explicit FSException(std::string msg,
                         std::string fileLog = "moonlander_log.log",
                         Category category = Category::INTERNAL_ERROR)
                         : BaseGameException(msg, fileLog, category) {}
};


#endif //MOONLANDER_FSEXCEPTION_HPP
