#ifndef __mdkLinearAlgebra_Function_SparseMatrix_Part_1_h
#define __mdkLinearAlgebra_Function_SparseMatrix_Part_1_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>

#include <armadillo.h>

//#include "mdkSparseMatrix.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkSparseMatrix;

//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//

//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixAdd(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixSubtract(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = ElementA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = MatrixA operator ElementB --------------------------------------------

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixAdd(const mdkSparseMatrix<ElementType>& Matrix, const ElementType& ElementB);

template<typename ElementType>
inline bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixSubtract(const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixSubtract(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixMultiply(const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementMultiply(const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementDivide(const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementDivide(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& Matrix, const ElementType& Element);

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName, const mdkSparseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix);


template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&)> Operation,
                                                           const mdkSparseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrix, 
                                   std::function<ElementType(const ElementType&)> Operation, 
                                   const mdkSparseMatrix<ElementType>& InputMatrix);

// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkSparseMatrix<ElementType>& InputMatrixA,
                                   const mdkSparseMatrix<ElementType>& InputMatrixB);


template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkSparseMatrix<ElementType>& InputMatrixA,
                                   const mdkSparseMatrix<ElementType>& InputMatrixB);

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const ElementType& InputElementB);

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkSparseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                           const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                           const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkSparseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline mdkSparseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                        const std::vector<const mdkSparseMatrix<ElementType>*>& MatrixPtrList, 
                                                        const ElementType& IndependentElement);

template<typename ElementType>
inline bool MatrixLinearCombine(mdkSparseMatrix<ElementType>& OutputMatrix,
                                const std::vector<ElementType>& CoefPtrList, 
                                const std::vector<const mdkSparseMatrix<ElementType>*>& MatrixPtrList,
                                const ElementType& IndependentElement);

//----------------------------------------------------------------------------------------------------------------//

}//end namespace mdk

#include "mdkLinearAlgebra_Function_SparseMatrix_Part_1.hpp"

#endif
