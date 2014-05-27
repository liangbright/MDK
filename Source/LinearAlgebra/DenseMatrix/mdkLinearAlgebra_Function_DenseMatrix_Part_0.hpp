#ifndef __mdkLinearAlgebra_Function_Part_0_hpp
#define __mdkLinearAlgebra_Function_Part_0_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_0.h"

namespace mdk
{

template<typename ElementType>
bool Sort(const DenseMatrix<ElementType>& InputDataArray, DenseMatrix<ElementType>& OutputDataArray, 
          DenseMatrix<int_max>& OutputIndexList, const std::string& Order)
{
    // input should be a vector

    auto InputSize = InputDataArray.GetSize();

    if (InputSize.RowNumber != 1 && InputSize.ColNumber != 1) // not a none-empty vector
    {
        MDK_Error("Invalid InputDataArray @mdkLinearAlgebra Sort(input is Matrix)")
        return false;
    }

    if (OutputDataArray.IsSharedWith(InputDataArray) == false)
    {
        auto IsGood = OutputDataArray.FastResize(InputSize.RowNumber, InputSize.ColNumber);

        if (IsGood == false)
        {
            MDK_Error("Invalid OutputDataArray @mdkLinearAlgebra Sort(input is Matrix)")
            return false;
        }
    }

    auto IsOK = OutputIndexList.FastResize(InputSize.RowNumber, InputSize.ColNumber);

    if (IsOK == false)
    {
        MDK_Error("Invalid OutputIndexList @mdkLinearAlgebra Sort(input is Matrix)")
        return false;    
    }

    return Sort(InputDataArray.GetElementPointer(), 
                InputSize.RowNumber*InputSize.ColNumber, 
                OutputDataArray.GetElementPointer(),
                OutputIndexList.GetElementPointer(),
                Order);
}


template<typename ElementType>
bool Sort(const std::vector<ElementType>& InputDataArray, std::vector<ElementType>& OutputDataArray, 
          std::vector<int_max>& OutputIndexList, const std::string& Order)
{
    auto InputLength = int_max(InputDataArray.size());

    auto OutputLength = int_max(OutputDataArray.size());

    auto OutputIndexListLength = int_max(OutputIndexList.size());

    if (OutputLength != InputLength)
    {
        OutputDataArray.resize(InputLength);
    }

    if (OutputIndexListLength != InputLength) // note: OutputDataArray can be just the InputDataArray
    {
        OutputIndexList.resize(InputLength);
    }

    return Sort(InputDataArray.data(), InputLength, OutputDataArray.data(), OutputIndexList.data(), Order);
}


template<typename ElementType>
bool Sort(const ElementType* InputDataArray, int_max Length, ElementType* OutputDataArray, int_max* OutputIndexList, const std::string& Order, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (InputDataArray == nullptr || Length == 0 || OutputDataArray == nullptr || OutputIndexList == nullptr)
        {
            MDK_Error("Invalid input @mdkLinearAlgebra Sort(pointers)")
            return false;
        }

        if (Order != "ascend" && Order != "descend")
        {
            MDK_Error(" unknown  Order @mdkLinearAlgebra Sort(pointers)")
            return false;
        }
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputIndexList[i] = i;
    }

    if (Order == "ascend")
    {
        std::sort(OutputIndexList, OutputIndexList + Length, [&](int_max a, int_max b) { return InputDataArray[a] < InputDataArray[b]; });
    }
    else
    {
        std::sort(OutputIndexList, OutputIndexList + Length, [&](int_max a, int_max b) { return InputDataArray[a] > InputDataArray[b]; });
    }

    for (int_max i = 0; i < Length; ++i)
    {
        OutputDataArray[i] = InputDataArray[OutputIndexList[i]];
    }

    return true;
}


}//end namespace mdk

#endif