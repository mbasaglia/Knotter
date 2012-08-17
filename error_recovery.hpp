#ifndef ERROR_RECOVERY_HPP
#define ERROR_RECOVERY_HPP

#include "graphics/knotview.hpp"

class ErrorRecovery
{
    private:
        static void sigsegv(int sig);

    public:
        static KnotView* recover;
        static void initialize();
};

#endif // ERROR_RECOVERY_HPP
