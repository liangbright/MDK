#pragma once


namespace mdk
{

template<typename ElementType>
inline
DenseMatrix<ElementType> CreateDenseMatrixFromSparseMatrix(const SparseMatrix<ElementType>& InputSparseMatrix)
{
    DenseMatrix<ElementType> OutputDenseMatrix;
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

    const auto& ColIndexList = InputSparseMatrix.GetColIndexList();

	const auto& RowIndexList = InputSparseMatrix.GetRowIndexList();

	const auto& ElmentList = InputSparseMatrix.GetElmentList();

    OutputDenseMatrix.Fill(ElementType(0));

	for (int_max k = 0; k < int_max(ElmentList.size()); ++k)
    {
		OutputDenseMatrix(RowIndexList[k], ColIndexList[k]) = ElmentList[k];
    }

    return true;
}


template<typename ElementType>
inline
SparseMatrix<ElementType> CreateSparseMatrixFromDenseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold)
{
    SparseMatrix<ElementType> OutputSparseMatrix;
    return ConvertDenseMatrixToSparseMatrix(InputDenseMatrix, absThreashold, OutputSparseMatrix);
}


template<typename ElementType>
inline
bool ConvertDenseMatrixToSparseMatrix(const DenseMatrix<ElementType>& InputDenseMatrix, ElementType absThreashold, SparseMatrix<ElementType>& OutputSparseMatrix)
{
    auto Size = InputDenseMatrix.GetSize();

    std::vector<int_max> ColIndexList;
    std::vector<int_max> RowIndexList;
    std::vector<ElementType> ElementList;

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto LinearIndex = j*Size.RowNumber + i;

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            if (std::abs(InputDenseMatrix[LinearIndex]) > absThreashold) // must use " > " because absThreashold may be 0 (e.g., ElementType is int)
            {
                ColIndexList.push_back(j);
                RowIndexList.push_back(i);
                ElementList.push_back(InputDenseMatrix[LinearIndex]);
            }
        }
    }

    OutputSparseMatrix.ConstructFromSortedData(std::move(RowIndexList), std::move(ColIndexList), std::move(ElementList), Size.RowNumber, Size.ColNumber);

    return true;
}


}// namespace mdk

