#ifndef __mdkMatrix_h
#define __mdkMatrix_h

#include <vector>
#include <memory>
#include <initializer_list>

#include "mdkObject.h"

namespace mdk
{

// 2D Matrix Class Template, each entry is a scalar
// column major, i.e., a column is an element in mdkStructuredDataArray
// RowNumber  =  ElementSize[0]
// ColumNumber = ElementNumberInArray
// a matrix can dynamicly expand in column direction: via member function InsertColumn implemented in this class
//
// linear algebra library Armadillo works better with column major matrix
// If heavy linear algebra is required, then convert mdkMatrix to Armadillo matrix, do something, and convert back
//

//forward-declare the template class
template<typename ScalarType>
class mdkMatrix;

struct MatrixSize
{
	uint64 RowNumber;
	uint64 ColNumber;
};


template<typename ScalarType>
struct mdkMatrixSVDResult
{
// Matrix = U*S*V;
	mdkMatrix<ScalarType> U;
	//mdkDiagonalMatrix<ScalarType> S;
	mdkMatrix<ScalarType> V;
};


template<typename ScalarType>
struct mdkMatrixPCAResult
{
	mdkMatrix<ScalarType> M; // mean
	mdkMatrix<ScalarType> U; // eigenvector
	//mdkDiagonalMatrix<ScalarType> S; // eigenvalue
};


template<typename ScalarType>
class mdkMatrix : public mdkObject
{

private:

	uint64 m_RowNumber;
	
	uint64 m_ColNumber;

	std::shared_ptr<std::vector<ScalarType>> m_ScalarData;

	bool m_IsSizeFixed;

	mdkScalarTypeEnum m_ScalarType;

	uint64 m_ColExpansionStep;

	uint64 m_RowExpansionStep;

	ScalarType  m_EmptyScalar;

public:		
	
	inline mdkMatrix();

	inline mdkMatrix(mdkMatrix<ScalarType>& sourceMatrix);

	inline mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

	inline ~mdkMatrix();

	inline void Clear();

	inline std::shared_ptr<std::vector<ScalarType>> GetScalarDataSharedPointer();

	inline ScalarType* GetScalarDataRawPointer();

	inline mdkScalarTypeEnum GetScalarType();

	//---------------------- Matrix Size ----------------------------------------//

	inline bool SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false);

	inline void FixSize(bool Fix);

	inline bool ReSize(uint64 RowNumber, uint64 ColNumber);

	inline void GetSize(uint64* RowNumber, uint64* ColNumber);

	inline MatrixSize GetSize();

	inline bool IsSizeFixed();

	inline bool IsEmpty();
	//---------------------- Initilize Matrix ----------------------------------------//

	template<typename ScalarType_target>
	inline void operator=(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline void operator=(ScalarType_target Scalar);

	inline void operator=(const std::initializer_list<ScalarType>& list);

	template<typename ScalarType_target>
	inline bool Copy(mdkMatrix<ScalarType_target>& targetMatrix);

	inline bool Fill(ScalarType Scalar);

	inline bool FillDiangonal(ScalarType Scalar);

	inline bool FillDiangonal(std::vector<ScalarType> ScalarList);

	//---------------------- Get/Set Matrix(Linear Index) ----------------------------------------//

	inline ScalarType& operator()(uint64 LinearIndex);

	inline const ScalarType& operator()(uint64 LinearIndex) const;

	//---------------------- Get/Set Matrix(i,j) ----------------------------------------//

	inline ScalarType& operator()(uint64 RowIndex, uint64 ColIndex);

	inline const ScalarType& operator()(uint64 RowIndex, uint64 ColIndex) const;

	//---------------------- Get Matrix(i_s to i_e, j_s to j_e) ----------------------------------------//

	inline mdkMatrix operator()(uint64 RowIndex_s, uint64 RowIndex_e, uint64 ColIndex_s, uint64 ColIndex_e);

	//---------------------- Get/Set a column ----------------------------------------//
	
	inline mdkMatrix GetCol(uint64 ColIndex);

	inline std::vector<ScalarType> GetColData(uint64 ColIndex);

	inline bool GetColData(uint64 ColIndex, ScalarType* ColData);

	inline bool SetCol(uint64 ColIndex, const ScalarType* ColData);

	inline bool SetCol(uint64 ColIndex, const std::vector<ScalarType>& ColData);

	inline bool AppendCol(const ScalarType* ColData);

	inline bool AppendCol(const std::vector<ScalarType>& ColData);

	//---------------------- Get/Set a row or column ----------------------------------------//
	
	inline mdkMatrix GetRow(uint64 RowIndex);

	inline std::vector<ScalarType> GetRowData(uint64 RowIndex);

	inline bool GetRowData(uint64 ColIndex, ScalarType* RowData);

	inline bool SetRow(uint64 RowIndex, const ScalarType* RowData);

	inline bool SetRow(uint64 RowIndex, const std::vector<ScalarType>& RowData);

	inline bool AppendRow(const ScalarType* RowData);

	inline bool AppendRow(const std::vector<ScalarType>& RowData);

	//---------------------- Matrix + - * / == >= <=  > < ----------------------------------------//

	template<typename ScalarType_target>
	inline mdkMatrix operator+(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline mdkMatrix operator-(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline mdkMatrix operator*(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline mdkMatrix operator/(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline void operator+=(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline void operator-=(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline void operator*=(mdkMatrix<ScalarType_target>& targetMatrix);

	template<typename ScalarType_target>
	inline void operator/=(mdkMatrix<ScalarType_target>& targetMatrix);

    //---------------------- scalar + - * / == >= <=  > < ----------------------------------------//

	template<typename ScalarType_target>
	inline mdkMatrix operator+(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline mdkMatrix operator-(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline mdkMatrix operator*(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline mdkMatrix operator/(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline void operator+=(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline void operator-=(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline void operator*=(ScalarType_target Scalar);

	template<typename ScalarType_target>
	inline void operator/=(ScalarType_target Scalar);

	//-------------------- element operation  ------------------------------------------//

	template<typename ScalarType_target>
	inline mdkMatrix ElementOperation(std::string Operator, mdkMatrix<ScalarType_target>& targetMatrix);

//	template<typename OperatorType, typename ScalarType_target>
//	inline mdkMatrix ElementOperation(OperatorType Operator, mdkMatrix<ScalarType_target>& targetMatrix);

//	template<typename ScalarType_target>
//	inline mdkMatrix ElementOperation(std::string Operator, ScalarType_target Scalar);

//	template<typename OperatorType, typename ScalarType_target>
//	inline mdkMatrix ElementOperation(OperatorType Operator, ScalarType_target Scalar);

	//-------------------- calculate sum mean min max ------------------------------------------//

	inline ScalarType Mean();

	inline mdkMatrix RowMean();

	inline mdkMatrix ColMean();

	inline ScalarType Sum();

	inline mdkMatrix RowSum();

	inline mdkMatrix ColSum();

	inline ScalarType Max();

	inline mdkMatrix RowMax();

	inline mdkMatrix ColMax();

	inline ScalarType Min();

	inline mdkMatrix RowMin();

	inline mdkMatrix ColMin();

	//----------------------------------- transpose -----------------------------------------//

	inline mdkMatrix GetTranspose();

	//----------------------------------- inverse -----------------------------------------//

	inline mdkMatrix GetInverse();

	inline mdkMatrix GetPseudoInverse();

	//----------------------------------- SVD -----------------------------------------//

	inline mdkMatrixSVDResult<ScalarType> GetSVD();

	//----------------------------------- PCA -----------------------------------------//

	inline mdkMatrixPCAResult<ScalarType> GetPCA();

	//----------------------------------- Convolution -----------------------------------------//

	template<typename ScalarType_target>
	inline mdkMatrix GetConvolution(mdkMatrix<ScalarType_target> MaskMatrix);

	//----------------------------------- LinearCombine -----------------------------------------//

	inline static mdkMatrix LinearCombine(std::vector<double> AlphaList, std::vector<mdkMatrix<ScalarType>*> MatrixList);


	//---------------------------- private functions ---------------------------------------//

private:
	inline mdkScalarTypeEnum FindScalarType();

	inline bool CopyOnWrite();
};

}//end namespace mdk

#include "mdkMatrix.hpp"

#endif