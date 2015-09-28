#ifndef mdk_StdObjectVector_h
#define mdk_StdObjectVector_h

#include <vector>
#include <memory>

#include "mdkObject.h"
#include "mdkDenseVector_ForwardDeclare.h"
#include "mdkString.h"

namespace mdk
{

// forward declare ------------------------------
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class ObjectArray;
//------------------------------------------------

#if defined MDK_DEBUG_MODE
#define MDK_DEBUG_StdObjectVector_Operator_CheckBound
#endif

//----------------------------------------------------------------------------------------------------------------------------//

template<typename Element_Type>
class StdObjectVector : public Object
{     
public:
	typedef Element_Type  ElementType;

private:
    std::vector<ElementType> m_StdVector;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline StdObjectVector();
	inline StdObjectVector(const StdObjectVector<ElementType>& InputArray);
	inline StdObjectVector(StdObjectVector<ElementType>&& InputArray) noexcept;
    
	inline StdObjectVector(const std::initializer_list<ElementType>& InputArray);
	inline StdObjectVector(const ObjectArray<ElementType>& InputArray);

	inline ~StdObjectVector();

    //-------------------- get/set std vector -----------------------------------//

    inline std::vector<ElementType>& StdVector();
    inline const std::vector<ElementType>& StdVector() const;

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const StdObjectVector<ElementType>& InputArray);
    inline void operator=(StdObjectVector<ElementType>&& InputArray);

	inline void operator=(const std::initializer_list<ElementType>& InputList);
	inline void operator=(const ObjectArray<ElementType>& InputArray);

    //----------------------  Copy  ----------------------------------------//

	inline void Copy(const StdObjectVector<ElementType>& InputArray);
	inline void Copy(StdObjectVector<ElementType>&& InputArray);
	
	inline void Copy(const ObjectArray<ElementType>& InputArray);
	inline void Copy(const ElementType* InputElementPointer, int_max InputLength);

	inline void Fill(const ElementType& Element);

    //------------------------- Clear -------------------------------------------//
    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

	inline void Resize(int_max InputLength); // try to keep the old data
	inline void FastResize(int_max InputLength); // do not care about old data

	inline void SetCapacity(int_max InputElementNumber); // reserve memory, current Length does not change
    inline void ReleaseUnusedCapacity();

    inline bool IsEmpty() const;

    inline int_max GetLength() const;
    inline int_max GetElementCount() const; // the same as GetLength();

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the pointer of the first element
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer(); //  the pointer of the first element
    inline const ElementType* GetPointer() const;

    inline ElementType* begin();
    inline const ElementType* begin() const;

    inline ElementType* end(); // 1 + pointer of the last element
    inline const ElementType* end() const; // 1 + pointer of the last element

	//----------- Get/Set by Index -----------------------------------//

    // operator[] or () : no bound check

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;
    
    // at(): bound check

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

	//-------------------------------------------------------------------------------

	inline void Append(ElementType Element);
	inline void Append(const ElementType* InputArray, int_max InputLength);

	inline void Delete(int_max Index);
	inline void Delete(const std::initializer_list<int_max>& IndexList);
	inline void Delete(const StdObjectVector<int_max>& IndexList);
	inline void Delete(const ObjectArray<int_max>& IndexList);
	inline void Delete(const DenseMatrix<int_max>& IndexList);	

	template<int_max TemplateLength>
	inline void Delete(const DenseVector<int_max, TemplateLength>& IndexList);

	inline void Delete(const int_max* ColIndexList, int_max ListLength);

	inline void Delete(int_max Index_start, int_max Index_end);

	inline void Insert(int_max Index, const ElementType& Element);
	inline void Insert(int_max Index, const std::initializer_list<ElementType>& InputArray);
	inline void Insert(int_max Index, const StdObjectVector<ElementType>& InputArray);
	inline void Insert(int_max Index, const ObjectArray<ElementType>& InputArray);
	inline void Insert(int_max Index, const ElementType* InputArray, int_max InputLength);

    //------------- use StdObjectVector as a stack ----------------------------//

	inline void PushBack(ElementType Element);
    inline ElementType PopBack();

	//----------------------- Get subset ------------------------------//

	inline StdObjectVector<ElementType> GetSubSet(int_max Index_start, int_max Index_end);
	inline StdObjectVector<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);
	inline StdObjectVector<ElementType> GetSubSet(const StdObjectVector<int_max>& IndexList);
	inline StdObjectVector<ElementType> GetSubSet(const ObjectArray<int_max>& IndexList);
	inline StdObjectVector<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

	template<int_max TemplateLength>
	inline StdObjectVector<ElementType> GetSubSet(const DenseVector<int_max, TemplateLength>& IndexList);

	inline StdObjectVector<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

	//----------------------- Set subset ------------------------------//

	inline void SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet);
	inline void SetSubSet(const StdObjectVector<int_max>& IndexList, const StdObjectVector<ElementType>& SubSet);
	inline void SetSubSet(const ObjectArray<int_max>& IndexList, const StdObjectVector<ElementType>& SubSet);
	inline void SetSubSet(const DenseMatrix<int_max>& IndexList, const StdObjectVector<ElementType>& SubSet);

	template<int_max TemplateLength>
	inline void SetSubSet(const DenseVector<int_max, TemplateLength>& IndexList, const StdObjectVector<ElementType>& SubSet);

	inline void SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max SubSetElementNumber);

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline StdObjectVector<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline StdObjectVector<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline StdObjectVector<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

	//-------------------- ExactMatch (use operator == ) ---------------------------------------//

	inline StdObjectVector<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const String& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline StdObjectVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline StdObjectVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

private:

};


}//end namespace mdk

#include "mdkStdObjectVector.hpp"

#endif
