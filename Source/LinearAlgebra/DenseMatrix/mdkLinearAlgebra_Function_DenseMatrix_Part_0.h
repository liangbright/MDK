#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_0_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_0_h

#include <vector>
#include <array>
#include <memory>
#include <string>


//#include "mdkDenseMatrix.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkDenseMatrix;

template<typename ElementType>
struct mdkPairForSort
{
    ElementType Element;

    int64 Index;
};

template<typename ElementType>
bool Sort(const mdkDenseMatrix<ElementType>& InputDataArray, mdkDenseMatrix<ElementType>& OutputDataArray, mdkDenseMatrix<ElementType>& OutputIndexList, const std::string& Order);

template<typename ElementType>
bool Sort(const std::vector<ElementType>& InputDataArray, std::vector<ElementType>& OutputDataArray, std::vector<ElementType>& OutputIndexList, const std::string& Order);

template<typename ElementType>
bool Sort(const ElementType* InputDataArray, int64 Length, ElementType* OutputDataArray, ElementType* OutputIndexList, const std::string& Order);


}//end namespace mdk

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_0.hpp"

#endif
