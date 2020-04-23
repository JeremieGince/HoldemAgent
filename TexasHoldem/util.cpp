

#include "util.h"

namespace game {

    ProgressBar::ProgressBar(unsigned int max, std::string message) : _max(max), _progress(0), _counter(0), _message(message), _bar(50, ' ')
    {
        _bar[0] = '#';
    }



    ProgressBar::~ProgressBar()
    {
        std::cout << std::endl;
    }

    void ProgressBar::progress()
    {
        if (_max > 1) {
            unsigned int currentProgress = (_counter * 100) / (_max - 1);

            if (_progress < currentProgress)
            {
                _progress = currentProgress;

                std::fill_n(_bar.begin(), _progress / 2, '#');

                std::cout << "\r" << _message << "  [" << _bar << "] "
                    << _progress << "%" << std::flush;
            }

            ++_counter;
        }
        
    }

    void ProgressBar::operator++()
    {
        progress();
    }

    void ProgressBar::operator++(int)
    {
        progress();
    }


}