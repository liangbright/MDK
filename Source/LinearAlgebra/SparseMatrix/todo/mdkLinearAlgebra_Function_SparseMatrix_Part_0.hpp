#ifndef mdk_LinearAlgebra_Function_SparseMatrix_Part_0_hpp
#define mdk_LinearAlgebra_Function_SparseMatrix_Part_0_hpp


namespace mdk
{

template<typename ElementType>
bool Sort(const mdkSparseMatrix<ElementType>& InputDataArray, mdkSparseMatrix<ElementType>& OutputDataArray, mdkSparseMatrix<ElementType>& OutputIndexList, const std::string& Order)
{
    mdkDenseMatrix<ElementType> tempInputDataArray = CreateDenseMatrixFromSparseMatrix(InputDataArray);

    mdkDenseMatrix<ElementType> tempOutputDataArray;

    mdkDenseMatrix<ElementType> tempOutputIndexList;

    auto IsOK = Sort(tempInputDataArray, tempOutputDataArray, tempOutputIndexList, Order);
    
    OutputDataArray = CreateSparseMatrixFromDenseMatrix(tempOutputDataArray);

    OutputIndexList = CreateSparseMatrixFromDenseMatrix(tempOutputIndexList);

    return IsOK;
}


}//end namespace mdk

#endif