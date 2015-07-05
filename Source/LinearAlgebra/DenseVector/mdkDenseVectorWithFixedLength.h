#ifndef mdk_DenseVectorWithFixedLength_h
#define mdk_DenseVectorWithFixedLength_h

#include <utility>
#include <array>
#include <vector>
#include <limits>

#include "mdkDebugConfig.h"
#include "mdkCommonType.h"
#include "mdkObject.h"
#include "mdkObjectArray.h"
#include "mdkStdObjectVector.h"
#include "mdkDenseVector_ForwardDeclare.h"
#include "mdkLinearAlgebra_Function_DenseVector.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough or for 3D position
// for example, use it as a voxel in a 3D vector image

namespace mdk
{
//-------------------------------
// "ambiguous symbol"
//template<typename ElementType>
//class DenseMatrix;
//------------------------------

template<typename Element_Type, int_max Length>
class DenseVector : public Object
{
public:
	typedef Element_Type ElementType;

private:
    std::array<ElementType, Length> m_StdArray;

public:
    
    inline DenseVector();
    
    inline DenseVector(const ElementType& Element);

    inline DenseVector(const std::initializer_list<ElementType>& InputVector);

	inline DenseVector(const std::vector<ElementType>& InputVector);

	inline DenseVector(const StdObjectVector<ElementType>& InputVector);

	inline DenseVector(const ObjectArray<ElementType>& InputVector);

	inline DenseVector(const DenseMatrix<ElementType>& InputVector);

    inline DenseVector(const DenseVector<ElementType, Length>& InputVector);

    inline DenseVector(DenseVector<ElementType, Length>&& InputVector);
    
    inline ~DenseVector();

    //------------------------------------------------------------

	// must not define this function
	//inline void operator=(const ElementType& Element);

    inline void operator=(const DenseVector<ElementType, Length>& InputVector);

    inline void operator=(DenseVector<ElementType, Length>&& InputVector);

    inline void operator=(const std::initializer_list<ElementType>& InputVector);

	inline void operator=(const std::vector<ElementType>& InputVector);

	inline void operator=(const StdObjectVector<ElementType>& InputVector);

	inline void operator=(const ObjectArray<ElementType>& InputVector);

	inline void operator=(const DenseVector<ElementType>& InputVector);

    inline void operator=(const DenseMatrix<ElementType>& InputVector);

    //------------------------------------------------------------

    template<typename ElementType_input>
    inline void Copy(const DenseVector<ElementType_input, Length>& InputVector);

    template<typename ElementType_input>
    inline void Copy(const ElementType_input* InputVector);

    //------------------------------------------------------------

    inline void Clear(); // set every element to 0

    inline void Fill(const ElementType& Element);

    inline int_max GetLength() const;

    inline int_max GetElementCount() const;

    inline bool IsEmpty() const;

    inline bool IsLengthFixed() const;

    inline bool IsSizeFixed() const;

    //-----------element access------------------//

    inline ElementType* GetElementPointer();
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer();
    inline const ElementType* GetPointer() const;

    inline ElementType* begin();
    inline const ElementType* begin() const;

    inline ElementType* end();
    inline const ElementType* end() const;

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

    //-------------------Get std Array -------------------------------

	inline std::array<ElementType, Length>& StdArray();
	
	inline const std::array<ElementType, Length>& StdArray() const;

	//----------------------- Convert to std vector -------------------------------
	
	std::vector<ElementType> ConvertToStdVector() const;

	//----------------------- Convert to DenseVectorWithVariableLength -------------------------------

	DenseVector<ElementType> ConvertToDenseVectorWithVariableLength() const;

    //---------- convert to regular vector represented by DenseMatrix ----------------//

    DenseMatrix<ElementType> ConvertToDenseMatrixAsRowVector() const;

    void ConvertToDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const;

    DenseMatrix<ElementType> ConvertToDenseMatrixAsColVector() const;

    void ConvertToDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const;

    //---------------------- GetSubSet --------------------------------------

    inline DenseVector<ElementType> GetSubSet(int_max Index_start, int_max Index_end) const;

    inline DenseVector<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const std::vector<int_max>& IndexList) const;

	inline DenseVector<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList) const;

    template<int_max InputLengthParameter>
	inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max, InputLengthParameter>& IndexList) const;

    //---------------------- SetSubSet --------------------------------------

	// SetSubSet(Index_start, Index_end, SubVector),  Index_start >= or <= Index_end

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector);

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::vector<ElementType>& SubVector);

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const DenseMatrix<ElementType>& SubVector);

	template<int_max InputLengthParameter>
	inline bool SetSubSet(int_max Index_start, int_max Index_end, const DenseVector<ElementType, InputLengthParameter>& SubVector);

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const ElementType* SubVector);

	// SetSubSet(IndexList, SubVector)

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubVector);

	inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

	template<int_max LengthParameterA, int_max LengthParameterB>
	inline bool SetSubSet(const DenseVector<int_max, LengthParameterA>& IndexList, const DenseVector<ElementType, LengthParameterB>& SubVector);

	// SetSubSet(span(a, b), SubVector)

	inline bool SetSubSet(const std::vector<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

	template<int_max InputLengthParameter>
	inline bool SetSubSet(const std::vector<int_max>& IndexList, const DenseVector<ElementType, InputLengthParameter>& SubVector);

	// base function
	inline bool SetSubSet(const int_max* IndexList, const ElementType* SubVector, int_max SubVectorLength);

    //-------------------- find ---------------------------------------//
    // return index list

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const;

	// find the first/last that match the condition
	template<typename MatchFunctionType>
	inline int_max Find(const std::string& first_or_last, MatchFunctionType MatchFunction) const;

	//-------------------- ExactMatch (use == operator) ---------------------------------------//

	inline DenseVector<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//
    // return index list

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    inline DenseVector<int_max> Sort(const char* Order);
    // Order: ascend or descend
    inline DenseVector<int_max> Sort(const std::string& Order);
    
    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

    inline void SortInPlace(const char* Order);
    // Order: ascend or descend
    inline void SortInPlace(const std::string& Order);

    //------------------------- unique --------------------------------//
    // return index list of unique element
    inline DenseVector<int_max> FindUnique() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseVector<int_max> FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const;

    //---------------------------------------------------- Self {+= -= *= /=} Vector ---------------------------------------------// 

    inline void operator+=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator+=(const DenseVector<ElementType>& InputVector);

    inline void operator-=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator-=(const DenseVector<ElementType>& InputVector);

    inline void operator*=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator*=(const DenseVector<ElementType>& InputVector);

    inline void operator/=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator/=(const DenseVector<ElementType>& InputVector);

    //---------------------------------------------------- Self {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //------------------------------------------------------------------------------------------------------------------------------//

    inline ElementType Sum() const;

    inline ElementType Mean() const;

    inline ElementType Std() const; // Standard Deviation
	
    inline ElementType Max() const;

    inline int_max IndexOfMax() const;

    inline ElementType Min() const;

    inline int_max IndexOfMin() const;

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;
    
};


}// namespace mdk

#include "mdkDenseVectorWithFixedLength.hpp"
#include "mdkDenseVectorWithFixedLength_Operator.h"

#endif