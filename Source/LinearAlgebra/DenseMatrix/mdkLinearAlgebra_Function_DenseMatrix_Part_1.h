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
class DenseMatrix;

//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//

//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixAdd(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixSubtract(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixMultiply(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementMultiply(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementDivide(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = ElementA operator MatrixB --------------------------------------------

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB);

//------------------- OutputMatrixC = MatrixA operator ElementB --------------------------------------------

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixAdd(const DenseMatrix<ElementType>& Matrix, const ElementType& ElementB);

template<typename ElementType>
inline bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixSubtract(const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixMultiply(const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementMultiply(const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementDivide(const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& Matrix, const ElementType& Element);

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrix, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix);

// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                            const DenseMatrix<ElementType>& InputMatrixA,
                                                            const DenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                            const DenseMatrix<ElementType>& InputMatrixA,
                                                            const DenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                        const std::string& OperationName,
                                        const DenseMatrix<ElementType>& InputMatrixA,
                                        const DenseMatrix<ElementType>& InputMatrixB);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                       const DenseMatrix<ElementType>& InputMatrixA,
                                                       const DenseMatrix<ElementType>& InputMatrixB);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                   OperationType Operation,
                                   const DenseMatrix<ElementType>& InputMatrixA,
                                   const DenseMatrix<ElementType>& InputMatrixB);

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                            const DenseMatrix<ElementType>& InputMatrixA,
                                                            const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                        const char OperationName,
                                        const DenseMatrix<ElementType>& InputMatrixA,
                                        const ElementType& InputElementB);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                            const DenseMatrix<ElementType>& InputMatrixA,
                                                            const ElementType& InputElementB);

template<typename ElementType>
inline bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                        const std::string& OperationName,
                                        const DenseMatrix<ElementType>& InputMatrixA,
                                        const ElementType& InputElementB);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                       const DenseMatrix<ElementType>& InputMatrixA,
                                                       const ElementType& InputElementB);

template<typename ElementType, typename OperationType>
inline bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                   OperationType Operation,
                                   const DenseMatrix<ElementType>& InputMatrixA,
                                   const ElementType& InputElementB);


//---------------------- convenient function ----------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixAbs(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixExp(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixLog(const DenseMatrix<ElementType>& InputMatrix);

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSqrt(const DenseMatrix<ElementType>& InputMatrix);


//=========================================================================================================================================//
//------------------------------------------ MatrixColOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixColOperation on InputMatrix at InputColIndex

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputColIndex,
                                    const std::string& OperationName, 
                                    const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputColIndex,
                               OperationType Operation, 
                               const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputMatrixB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixColNamedOperation(const char OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                        const DenseMatrix<ElementType>& InputMatrixB,
                                                        const bool Enable_BoundCheck = true);
template<typename ElementType>
inline bool MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                                    const char OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                    const DenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                        const DenseMatrix<ElementType>& InputMatrixB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                                    const std::string& OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                    const DenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                   const DenseMatrix<ElementType>& InputMatrixB,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                               OperationType Operation,
                               const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                               const DenseMatrix<ElementType>& InputMatrixB,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixColNamedOperation(const char OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                        const ElementType& InputElementB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                                    const char OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixColNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                        const ElementType& InputElementB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                                    const std::string& OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixColOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                                                   const ElementType& InputElementB,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixColOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                               OperationType Operation,
                               const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                               const ElementType& InputElementB,
                               const bool Enable_BoundCheck = true);

//=========================================================================================================================================//
//------------------------------------------ MatrixRowOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixRowOperation on InputMatrix at InputRowIndex

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputRowIndex,
                                    const std::string& OperationName, 
                                    const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputRowIndex,
                               OperationType Operation, 
                               const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixRowOperation on InputMatrixA at InputRowIndexA with InputMatrixB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixRowNamedOperation(const char OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                        const DenseMatrix<ElementType>& InputMatrixB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                                    const char OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                    const DenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                        const DenseMatrix<ElementType>& InputMatrixB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                                    const std::string& OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                    const DenseMatrix<ElementType>& InputMatrixB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                   const DenseMatrix<ElementType>& InputMatrixB,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                               OperationType Operation,
                               const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                               const DenseMatrix<ElementType>& InputMatrixB,
                               const bool Enable_BoundCheck = true);

// ------------------------ MatrixRowOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixRowNamedOperation(const char OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                        const ElementType& InputElementB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                                    const char OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixRowNamedOperation(const std::string& OperationName,
                                                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                        const ElementType& InputElementB,
                                                        const bool Enable_BoundCheck = true);

template<typename ElementType>
inline bool MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                                    const std::string& OperationName,
                                    const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                    const ElementType& InputElementB,
                                    const bool Enable_BoundCheck = true);


template<typename ElementType, typename OperationType>
inline DenseMatrix<ElementType> MatrixRowOperation(OperationType Operation,
                                                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                                                   const ElementType& InputElementB,
                                                   const bool Enable_BoundCheck = true);

template<typename ElementType, typename OperationType>
inline bool MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                               OperationType Operation,
                               const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                               const ElementType& InputElementB,
                               const bool Enable_BoundCheck = true);


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline DenseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                    const std::vector<const DenseMatrix<ElementType>*>& MatrixList, 
                                                    const ElementType& IndependentElement);

template<typename ElementType>
inline bool MatrixLinearCombine(DenseMatrix<ElementType>& OutputMatrix,
                                const std::vector<ElementType>& CoefPtrList, 
                                const std::vector<const DenseMatrix<ElementType>*>& MatrixList,
                                const ElementType& IndependentElement);

template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_1(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_2(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_3(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_4(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_5(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_6(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_7(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_8(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_9(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);

template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_10(ElementType* Output, uint_max ElementNumber,
                                        const std::vector<ElementType>& CoefList, 
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement);
//----------------------------------------------------------------------------------------------------------------//

}//end namespace mdk

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_1.hpp"

#endif
