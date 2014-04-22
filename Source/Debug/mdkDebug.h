#ifndef __mdkDebug_h
#define __mdkDebug_h

#include "mdkType.h"


namespace mdk
{

inline void Pause_Here_To_Debug()
{// set a break point here
    std::system("pause");
    auto a = double(1);
}

}// namespace mdk

#endif