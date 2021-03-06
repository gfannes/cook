#include "cook/chai/mss.hpp"

namespace {

static cook::Logger *& meyers_logger()
{
    static cook::Logger * logger_ = nullptr;
    return logger_;
}

}

namespace cook { namespace chai {

Logger * log(const Result & rc)
{
    auto ptr = meyers_logger();
    ptr->log(rc);
    return ptr;
}
void set_logger(Logger * logger)
{
    meyers_logger() = logger;
}

} }


