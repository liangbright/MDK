#ifndef __mdkLinearAlgebra_Function_Part_0_hpp
#define __mdkLinearAlgebra_Function_Part_0_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_0.h"

namespace mdk
{

template<typename ElementType>
bool Sort(const DenseMatrix<ElementType>& InputDataArray, DenseMatrix<ElementType>& OutputDataArray, DenseMatrix<ElementType>& OutputIndexList, const std::string& Order)
{
    // input should be a vector

    auto InputSize = InputDataArray.GetSize();

    if (InputSize.RowNumber != 1 && InputSize.ColNumber != 1) // not a none-empty vector
    {
        MDK_Error << "Invalid InputDataArray @mdkLinearAlgebra Sort(input is Matrix)" << '\n';
        return false;
    }

    auto OutputSize = OutputDataArray.GetSize();

    if ((OutputSize.RowNumber != 1 && OutputSize.ColNumber != 1) 
        || OutputSize.RowNumber*OutputSize.ColNumber != InputSize.RowNumber*InputSize.ColNumber)
    {
        if (OutputDataArray.IsSizeFixed() == true)
        {
            MDK_Error << "Invalid OutputDataArray @mdkLinearAlgebra Sort(input is Matrix)" << '\n';
            return false;
        }
        else
        {
            OutputDataArray.FastResize(InputSize.RowNumber, InputSize.ColNumber);
        }
    }

    auto OutputIndexListSize = OutputIndexList.GetSize();
        
    if ((OutputIndexListSize.RowNumber != 1 && OutputIndexListSize.ColNumber != 1)
        || OutputIndexListSize.RowNumber*OutputIndexListSize.ColNumber != InputSize.RowNumber*InputSize.ColNumber)
    {
        if (OutputIndexList.IsSizeFixed() == true)
        {
            MDK_Error << "Invalid OutputIndexList @mdkLinearAlgebra Sort(input is Matrix)" << '\n';
            return false;
        }
        else
        {
            OutputIndexListSize.FastResize(InputSize.RowNumber, InputSize.ColNumber);
        }
    }


    return Sort(InputDataArray.GetElementPointer(), 
                InputSize.RowNumber*InputSize.ColNumber, 
                OutputDataArray.GetElementPointer(),
                OutputIndexList.GetElementPointer(),
                Order);
}


template<typename ElementType>
bool Sort(const std::vector<ElementType>& InputDataArray, std::vector<ElementType>& OutputDataArray, std::vector<ElementType>& OutputIndexList, const std::string& Order)
{
    auto InputLength = int64(InputDataArray.size());

    auto OutputLength = int64(OutputDataArray.size());

    auto OutputIndexListLength = int64(OutputIndexList.size());

    if (OutputLength != InputLength)
    {
        OutputDataArray.resize(InputLength);
    }

    if (OutputIndexListLength != InputLength)
    {
        OutputIndexList.resize(InputLength);
    }

    return Sort(InputDataArray.data(), InputLength, OutputDataArray.data(), OutputIndexList.data(), Order);
}


template<typename ElementType>
bool Sort(const ElementType* InputDataArray, int64 Length, ElementType* OutputDataArray, ElementType* OutputIndexList, const std::string& Order)
{
    if (InputDataArray == nullptr || Length == 0 || OutputDataArray == nullptr || OutputIndexList == nullptr)
    {
        MDK_Error << "Invalid input @mdkLinearAlgebra Sort(pointers)" << '\n';
        return false;
    }

    if (Order != "ascend" && Order != "descend")
    {
        MDK_Error << " unknown  Order @mdkLinearAlgebra Sort(pointers)" << '\n';
        return false;
    }

    std::vector<mdkPairForSort<ElementType>> tempPairList(Length);

    for (int64 i = 0; i < Length; ++i)
    {
        tempPairList[i].Element = InputDataArray[i];

        tempPairList[i].Index = i;
    }

    if (Order == "ascend")
    {
        std::sort(tempPairList.begin(), tempPairList.end(), 
                  [](const mdkPairForSort<ElementType>& a, const mdkPairForSort<ElementType>& b) { return a.Element < b.Element; });
    }
    else
    {
        std::sort(tempPairList.begin(), tempPairList.end(),
                  [](const mdkPairForSort<ElementType>& a, const mdkPairForSort<ElementType>& b) { return a.Element > b.Element; });
    }


    for (int64 i = 0; i < Length; ++i)
    {
        OutputDataArray[i] = tempPairList[i].Element;

        OutputIndexList[i] = tempPairList[i].Index;
    }

    return true;
}


}//end namespace mdk

#endif