#include <iostream>

namespace mdk
{

#define  mdkDebug(x)    x

//#define  mdkDebug(x) 

#define  mdkWarning    std::cout << "Warning: "

#define  mdkError      std::cout << '\a' << "ERROR !~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n'

//------------------------------ mdkMatrix -----------------------------------------------//
#define MDK_Matrix_Operator_CheckBound

#define MDK_ShadowMatrix_Operator_CheckBound

//-------------------------------mdk3DImage---------------------------------------------------------//
#define MDK_3DImage_Operator_CheckBound

}//end namespace mdk