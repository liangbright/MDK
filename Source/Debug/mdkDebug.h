#ifndef mdk_Debug_h
#define mdk_Debug_h

namespace mdk
{

inline void MDKError_Pause_Here_To_Debug()
{// set a break point here
    std::system("pause");
    auto a = double(1);
}

}// namespace mdk

#endif