#ifndef __mdkMatrix_Common_h
#define __mdkMatrix_Common_h

#include <vector>
#include <iomanip>


#include "mdkOSPlatformConfig.h"
#include "mdkType.h"
#include "mdkDebugConfig.h"

namespace mdk
{

enum struct MatrixElementTypeEnum
{
    Scalar_DOUBLE64,
    Scalar_FLOAT32,
    Scalar_INT8,
    Scalar_INT16,
    Scalar_INT32,
    Scalar_INT64,
    Scalar_UINT8,
    Scalar_UINT16,
    Scalar_UINT32,
    Scalar_UINT64,

    Other_DOUBLE64,
    Other_FLOAT32,
    Other_INT8,
    Other_INT16,
    Other_INT32,
    Other_INT64,
    Other_UINT8,
    Other_UINT16,
    Other_UINT32,
    Other_UINT64,

    UNKNOWN,
};

// ----------------------------- mdkMatrixSize struct -------------------------------------------------------------//

struct MatrixSize
{
    int_max RowNumber;  // RowNumber = the Number of Rows 
    int_max ColNumber;  // ColNumber = the Number of Columns
};

//------------------------------------------- ALL Symbol --------------------------------------------------------------------------//

struct InputStruct_For_ALL_Symbol_For_Matrix_Operator
{
    std::string Name = "InputStruct_For_ALL_Symbol_For_Matrix_Operator";
};

struct ALL_Symbol_For_Matrix_Operator
{
    ALL_Symbol_For_Matrix_Operator(const InputStruct_For_ALL_Symbol_For_Matrix_Operator& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_ALL_Symbol_For_Matrix_Operator")
        {
            MDK_Error("ALL Symbol error @ ALL_Symbol_For_mdkMatrix_Operator")
        }
    }

    ~ALL_Symbol_For_Matrix_Operator() {}

    // deleted:
    ALL_Symbol_For_Matrix_Operator() = delete;
    ALL_Symbol_For_Matrix_Operator(const ALL_Symbol_For_Matrix_Operator&) = delete;
    ALL_Symbol_For_Matrix_Operator(ALL_Symbol_For_Matrix_Operator&&) = delete;
    void operator=(const ALL_Symbol_For_Matrix_Operator&) = delete;
    void operator=(ALL_Symbol_For_Matrix_Operator&&) = delete;
};

static InputStruct_For_ALL_Symbol_For_Matrix_Operator This_Is_InputStruct_For_ALL_Symbol_For_Matrix_Operator;

static ALL_Symbol_For_Matrix_Operator This_Is_ALL_Symbol_For_Matrix_Operator(This_Is_InputStruct_For_ALL_Symbol_For_Matrix_Operator);

//refer to all the cols or rows, or all the elements
#define ALL This_Is_ALL_Symbol_For_Matrix_Operator

//-----------------------------------span: e.g., span(1,10) is 1:10 in Matlab, or span(1, 2, 10) is 1:2:10 in Matlab -----------------//

inline std::vector<int_max> span(int_max Index_A, int_max Index_B);
inline std::vector<int_max> span(int_max Index_A, int_max Step, int_max Index_B);


//------------------------------- Empty_Matrix_Symbol to Construct empty Matrix with/without memory allocation ----------------------------------------//

struct InputStruct_For_Pure_Empty_Matrix_Symbol
{
    std::string Name = "InputStruct_For_Pure_Empty_Matrix_Symbol";
};

struct Pure_Empty_Matrix_Symbol
{
    Pure_Empty_Matrix_Symbol(const InputStruct_For_Pure_Empty_Matrix_Symbol& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_Pure_Empty_Matrix_Symbol")
        {
            MDK_Error("Symbol error @ Pure_Empty_Matrix_Symbol")
        }
    }

    ~Pure_Empty_Matrix_Symbol() {}

    // deleted:
    Pure_Empty_Matrix_Symbol() = delete;
    Pure_Empty_Matrix_Symbol(const Pure_Empty_Matrix_Symbol&) = delete;
    Pure_Empty_Matrix_Symbol(Pure_Empty_Matrix_Symbol&&) = delete;
    void operator=(const Pure_Empty_Matrix_Symbol&) = delete;
    void operator=(Pure_Empty_Matrix_Symbol&&) = delete;
};

static InputStruct_For_Pure_Empty_Matrix_Symbol This_Is_InputStruct_For_Pure_Empty_Matrix_Symbol;

static Pure_Empty_Matrix_Symbol This_Pure_Empty_Matrix_Symbol(This_Is_InputStruct_For_Pure_Empty_Matrix_Symbol);

#define MDK_PURE_EMPTY_MATRIX This_Pure_Empty_Matrix_Symbol
//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
MatrixElementTypeEnum FindMatrixElementType(ElementType Element);


template<typename ElementType>
inline
ElementType GetMatrixNaNElement();

template<typename ElementType>
inline
ElementType GetMatrixNaNElement(const ElementType& ReferenceElement);

//------------------------------------------------------------//


template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
void DisplayMatrix(const std::string& Name, const DenseMatrix<ElementType>& InputMatrix, int_max precision = 0, bool Flag_scientific = false)
{
    std::cout << Name << " = " << '\n';

    if (Flag_scientific == true)
    {
        std::cout << std::scientific << std::setprecision(precision);
    }
    else
    {
        std::cout << std::setprecision(precision) << std::fixed;
    }

    for (int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            std::cout << std::setw(6 + precision) << InputMatrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}

}//end namespace mdk

#include "mdkMatrix_Common.hpp"

#endif
