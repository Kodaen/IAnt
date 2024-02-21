#ifndef TIMER_H_
#define TIMER_H_

/*
    struct for checking how long it has been since the start of the turn.
*/
#ifdef _WIN32 //Windows timer (DON'T USE THIS TIMER UNLESS YOU'RE ON WINDOWS!)
    #include <io.h>
    #include <windows.h>

    struct Timer
    {
        clock_t _startTime, _currentTime;

        Timer()
        {

        };

        void start()
        {
            _startTime = clock();
        };

        double getTime()
        {
            _currentTime = clock();

            return (double)(_currentTime - _startTime);
        };
    };

#else //Mac/Linux Timer*
#include <sys/time.h>
    struct Timer
    {
        timeval _timer;
        double _startTime, _currentTime;

        Timer()
        {

        };

        //starts the timer
        void start()
        {
            gettimeofday(&_timer, NULL);
            _startTime = _timer.tv_sec+(_timer.tv_usec/1000000.0);
        };

        //returns how long it has been since the timer was last started in milliseconds
        double getTime()
        {
            gettimeofday(&_timer, NULL);
            _currentTime = _timer.tv_sec+(_timer.tv_usec/1000000.0);
            return (_currentTime-_startTime)*1000.0;
        };
    };
#endif


#endif //TIMER_H_
