#include <iostream>

namespace mdk
{

#define  mdkDebug(x)    x

//#define  mdkDebug(x) 

#define  mdkWarning    std::cout << "Warning: "

#define  mdkError      std::cout << '\a' << "ERROR !~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n'

//------------------------------ mdkMatrix -----------------------------------------------//
#define MDK_Matrix_Operator_CheckBound
//----------------------------------------------------------------------------------------//

}//end namespace mdk