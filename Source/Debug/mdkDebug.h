#ifndef __mdkDebug_h
#define __mdkDebug_h


namespace mdk
{

template<typename T>
void Pause_Here_To_Debug()
{// set a break point here
    std::system("pause");
    auto a = T(1);
}


}// namespace mdk

#endif