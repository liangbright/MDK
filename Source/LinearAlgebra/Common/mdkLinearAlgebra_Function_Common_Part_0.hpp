#ifndef __mdkLinearAlgebra_Function_Common_Part_0_hpp
#define __mdkLinearAlgebra_Function_Common_Part_0_hpp

//#include "mdkLinearAlgebra_Function_Common_Part_0.h"


namespace mdk
{

template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix)
{
    mdkDenseMatrix<ElementType> OutputDenseMatrix;

    ConvertSparseMatrixToDenseMatrix(InputSparseMatrix, OutputDenseMatrix);
}


template<typename ElementType>
inline
bool ConvertSparseMatrixToDenseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix, DenseMatrix<ElementType>& OutputDenseMatrix)
{
    auto Size = InputSparseMatrix.GetSize();

    if (OutputDenseMatrix.FastResize(Size.RowNumber, Size.ColNumber) == false)
    {
        return false;
    }

    for (int_max k = 0; k < Size.ColNumber*Size.RowNumber; ++k)
    {
        OutputDenseMatrix[k] = InputSparseMatrix.GetElement(k);
    }

    return true;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold)
{
    mdkSparseMatrix<ElementType> OutputSparseMatrix;

    return ConvertDenseMatrixToSparseMatrix(InputDenseMatrix, ElementType absThreashold, OutputSparseMatrix);
}


template<typename ElementType>
inline
bool ConvertDenseMatrixToSparseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold,
                                      SparseMatrix<ElementType>& OutputSparseMatrix )
{
    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixAsRowVectorFromSparseVector(const SparseMatrix<ElementType>& InputSparseVector)
{
    DenseMatrix<ElementType> OutputDenseVector;

    ConvertSparseVectorToDenseMatrixAsRowVector(OutputDenseVector);

    return OutputDenseVector;
}


template<typename ElementType>
inline
bool ConvertSparseVectorToDenseMatrixAsRowVector(const SparseVector<ElementType>& InputSparseVector, DenseMatrix<ElementType>& OutputDenseVector)
{
    auto Length = InputSparseVector.GetLength();

    const std::vector<int_max>& IndexList = InputSparseVector.IndexList();

    const std::vector<ElementType>& DataArray = InputSparseVector.DataArray();

    if (OutputDenseVector.FastResize(1, Length) == false)
    {
        return false;
    }

    auto RecordedElementNumber = int_max(IndexList.size());

    for (int_max i = 0; i < RecordedElementNumber; ++i)
    {
        OutputDenseVector[IndexList[i]] = DataArray[i];
    }

    for (int_max i = 0; i < IndexList[0]; ++i)
    {
        OutputDenseVector[i] = ElementType(0);
    }

    for (int_max i = IndexList[RecordedElementNumber - 1] + 1; i < Length; ++i)
    {
        OutputDenseVector[i] = ElementType(0);
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixAsColVectorFromSparseVector(const SparseMatrix<ElementType>& InputSparseVector)
{
    DenseMatrix<ElementType> OutputDenseVector;

    ConvertSparseVectorToDenseMatrixAsColVector(OutputDenseVector);
}


template<typename ElementType>
inline
bool ConvertSparseVectorToDenseMatrixAsColVector(const SparseVector<ElementType>& InputSparseVector, DenseMatrix<ElementType>& OutputDenseVector)
{
    auto Length = InputSparseVector.GetLength();

    const std::vector<int_max>& IndexList = InputSparseVector.IndexList();

    const std::vector<ElementType>& DataArray = InputSparseVector.DataArray();

    if (OutputDenseVector.FastResize(Length, 1) == false)
    {
        return false;
    }

    auto RecordedElementNumber = int_max(IndexList.size());

    for (int_max i = 0; i < RecordedElementNumber; ++i)
    {
        OutputDenseVector[IndexList[i]] = DataArray[i];
    }

    for (int_max i = 0; i < IndexList[0]; ++i)
    {
        OutputDenseVector[i] = ElementType(0);
    }

    for (int_max i = IndexList[RecordedElementNumber - 1] + 1; i < Length; ++i)
    {
        OutputDenseVector[i] = ElementType(0);
    }

    return true;
}


}// namespace mdk


#endif
