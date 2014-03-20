#ifndef __mdkLinearAlgebra_DenseMatrix_Part_1_h
#define __mdkLinearAlgebra_DenseMatrix_Part_1_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>

#include <armadillo.h>

//#include "mdkDenseMatrix.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkDenseMatrix;

//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//

//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixAdd(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixSubtract(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementDivide(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = ElementA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = MatrixA operator ElementB --------------------------------------------

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixAdd(const mdkDenseMatrix<ElementType>& Matrix, const ElementType& ElementB);

template<typename ElementType>
inline bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixSubtract(const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixSubtract(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixMultiply(const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementMultiply(const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementDivide(const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& Matrix, const ElementType& Element);

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const char* OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);


template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&)> Operation,
                                                          const mdkDenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrix, 
                                   std::function<ElementType(const ElementType&)> Operation, 
                                   const mdkDenseMatrix<ElementType>& InputMatrix);

// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const mdkDenseMatrix<ElementType>& InputMatrixB);


template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const mdkDenseMatrix<ElementType>& InputMatrixB);

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const ElementType& InputElementB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                       const std::vector<const mdkDenseMatrix<ElementType>*>& MatrixPtrList, 
                                                       const ElementType& IndependentElement);

template<typename ElementType>
inline bool MatrixLinearCombine(mdkDenseMatrix<ElementType>& OutputMatrix,
                                const std::vector<ElementType>& CoefPtrList, 
                                const std::vector<const mdkDenseMatrix<ElementType>*>& MatrixPtrList,
                                const ElementType& IndependentElement);

template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_1(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_2(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_3(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_4(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_5(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_6(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_7(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_8(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_9(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);

template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_10(ElementType* Output, uint64 ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement);
//----------------------------------------------------------------------------------------------------------------//

}//end namespace mdk

#include "mdkLinearAlgebra_DenseMatrix_Part_1.hpp"

#endif
