#ifndef __mdkDisplayMatrix_h
#define __mdkDisplayMatrix_h

#include <string>

namespace mdk
{
//------------- forward declare-----------------------//
template<typename ElementType>
class DenseMatrix;

template<typename ElementType, int_max Length>
class DenseVector;

template<typename ElementType>
class SparseMatrix;

template<typename ElementType>
class SparseVector;
//-----------------------------------------------------//

template<typename ElementType>
void DisplayMatrix(const std::string& Name, const DenseMatrix<ElementType>& InputMatrix, int_max precision = 0, bool Flag_scientific = false);

template<typename ElementType>
void DisplayMatrix(const std::string& Name, const SparseMatrix<ElementType>& InputMatrix, int_max precision = 0, bool Flag_scientific = false);

template<typename ElementType, int_max Length>
void DisplayVector(const std::string& Name, const DenseVector<ElementType, Length>& InputVector, int_max precision = 0, bool Flag_scientific = false);

template<typename ElementType>
void DisplayVector(const std::string& Name, const SparseVector<ElementType>& InputVector, int_max precision = 0, bool Flag_scientific = false);

}//end namespace mdk

#include "mdkDisplayMatrix.hpp"

#endif