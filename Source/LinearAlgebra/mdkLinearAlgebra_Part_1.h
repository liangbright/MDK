#ifndef __mdkLinearAlgebra_Part_1_h
#define __mdkLinearAlgebra_Part_1_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>
#include <functional>

//#include "mdkMatrix.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class mdkMatrix;

//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//

//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = ElementA operator MatrixB --------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = MatrixA operator ElementB --------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& Matrix, const ElementType& ElementB);

template<typename ElementType>
inline bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixSubtract(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName, const mdkMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix);


template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&)> Operation,
                                                     const mdkMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrix, 
                                   std::function<ElementType(const ElementType&)> Operation, 
                                   const mdkMatrix<ElementType>& InputMatrix);

// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const mdkMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const mdkMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkMatrix<ElementType>& InputMatrixA,
                                   const mdkMatrix<ElementType>& InputMatrixB);


template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const mdkMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkMatrix<ElementType>& InputMatrixA,
                                   const mdkMatrix<ElementType>& InputMatrixB);

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const ElementType& InputElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                                   const std::string& OperationName,
                                   const mdkMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


template<typename ElementType>
inline mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                     const mdkMatrix<ElementType>& InputMatrixA,
                                                     const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                                   std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                   const mdkMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//


template<typename ElementType>
inline mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                  const std::vector<mdkMatrix<ElementType>*>& MatrixList);

template<typename ElementType>
inline bool MatrixLinearCombine(mdkMatrix<ElementType>& OutputMatrix,
                                const std::vector<ElementType>& CoefList, 
                                const std::vector<mdkMatrix<ElementType>*>& MatrixList);

template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_1(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_2(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_3(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_4(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_5(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_6(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_7(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_8(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* CoefPtr, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList);


//----------------------------------------------------------------------------------------------------------------//

}//end namespace mdk

#include "mdkLinearAlgebra_Part_1.hpp"

#endif
