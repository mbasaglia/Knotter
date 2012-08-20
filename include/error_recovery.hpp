#ifndef ERROR_RECOVERY_HPP
#define ERROR_RECOVERY_HPP

#include "knotview.hpp"

/**
    \brief Quick and dirty class used to save a backup file on error
*/
class ErrorRecovery
{
    private:
        static void sigsegv(int sig);

    public:
        static KnotView* recover;
        static void initialize();
};

#endif // ERROR_RECOVERY_HPP
