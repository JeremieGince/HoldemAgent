#ifndef UTIL_H
#define UTIL_H

#include <iostream> 
#include <vector>
#include <string>
#include "contract_exception.h"





namespace game{

    class ProgressBar
    {
    public:

        ProgressBar(unsigned int max, std::string message = std::string(""));

        ~ProgressBar();

        void progress();

        void operator++();

        void operator++(int);

    protected:

        unsigned int _max;
        unsigned int _progress;
        unsigned int _counter;

        std::string _message;
        std::string _bar;
    };



}


#endif //UTIL_H