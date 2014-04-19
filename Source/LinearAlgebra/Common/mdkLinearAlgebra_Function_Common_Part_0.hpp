#ifndef __mdkLinearAlgebra_Function_Common_Part_0_hpp
#define __mdkLinearAlgebra_Function_Common_Part_0_hpp


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

    const std::vector<int_max>& ColIndexList = InputSparseMatrix.GetColIndexList();

    const std::vector<int_max>& RowIndexList = InputSparseMatrix.GetRowIndexList();

    const std::vector<ElementType>& DataArray = InputSparseMatrix.GetDataArray();

    OutputDenseMatrix.Fill(ElementType(0));

    for (int_max k = 0; k < int_max(DataArray.size()); ++k)
    {
        OutputDenseMatrix(RowIndexList[k], ColIndexList[k]) = DataArray[k];
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
    auto Size = InputDenseMatrix.GetSize();

    std::vector<int_max> ColIndexList;
    std::vector<int_max> RowIndexList;
    std::vector<ElementType> DataArray;

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto LinearIndex = j*Size.RowNumber + i;

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            if (std::abs(InputDenseMatrix[LinearIndex]) > absThreashold) // must use " > " because absThreashold may be 0 (e.g., ElementType is int)
            {
                ColIndexList.push_back(j);
                RowIndexList.push_back(i);
                DataArray.push_back(InputDenseMatrix[LinearIndex]);
            }
        }
    }

    OutputSparseMatrix.ConstructFromSortedData(std::move(RowIndexList), std::move(ColIndexList), std::move(DataArray), Size.RowNumber, Size.ColNumber);

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
