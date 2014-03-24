#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_1_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_1_h

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
inline mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation, const mdkDenseMatrix<ElementType>& InputMatrix);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrix, OperationType Operation, const mdkDenseMatrix<ElementType>& InputMatrix);

// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                        const std::string& OperationName,
                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                        const mdkDenseMatrix<ElementType>& InputMatrixB);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   OperationType Operation,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const mdkDenseMatrix<ElementType>& InputMatrixB);

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                               const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                        const char OperationName,
                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                        const ElementType& InputElementB);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                               const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                               const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                        const std::string& OperationName,
                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                        const ElementType& InputElementB);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                          const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                          const ElementType& InputElementB);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                   OperationType Operation,
                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


//=========================================================================================================================================//
//------------------------------------------ MatrixColOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixColOperation on InputMatrix at InputColIndex

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputColIndex,
                                    const std::string& OperationName, 
                                    const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputColIndex,
                               OperationType Operation, 
                               const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputMatrixB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixColNamedOperation(const char OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                           const bool Enable_BoundCheck = true);
template<typename ElementType>
inline bool MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                                    const char OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                    const mdkDenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                                    const std::string& OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                    const mdkDenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                               OperationType Operation,
                               const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                               const mdkDenseMatrix<ElementType>& InputMatrixB,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixColNamedOperation(const char OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                           const ElementType& InputElementB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                                    const char OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                           const ElementType& InputElementB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                                    const std::string& OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                                                      const ElementType& InputElementB,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                               OperationType Operation,
                               const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                               const ElementType& InputElementB,
                               const bool Enable_BoundCheck = true);


//=========================================================================================================================================//
//------------------------------------------ MatrixRowOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixRowOperation on InputMatrix at InputRowIndex

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputRowIndex,
                                    const std::string& OperationName, 
                                    const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputRowIndex,
                               OperationType Operation, 
                               const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixRowOperation on InputMatrixA at InputRowIndexA with InputMatrixB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixRowNamedOperation(const char OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                                    const char OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                    const mdkDenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                                    const std::string& OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                    const mdkDenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixB,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                               OperationType Operation,
                               const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                               const mdkDenseMatrix<ElementType>& InputMatrixB,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixRowOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixRowNamedOperation(const char OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                           const ElementType& InputElementB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                                    const char OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline mdkDenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                           const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                           const ElementType& InputElementB,
                                                           const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                                    const std::string& OperationName,
                                    const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline mdkDenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                      const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                                                      const ElementType& InputElementB,
                                                      const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                               OperationType Operation,
                               const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                               const ElementType& InputElementB,
                               const bool Enable_BoundCheck = true);


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

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_1.hpp"

#endif
