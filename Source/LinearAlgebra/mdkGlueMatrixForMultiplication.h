#ifndef __mdkGlueMatrixForMultiplication_h
#define __mdkGlueMatrixForMultiplication_h

#include "mdkObject.h"
#include "mdkLinearAlgebraConfig.h"
//#include "mdkMatrix.h"

#if defined MDK_Enable_GlueMatrix //=====================================================================================

namespace mdk
{

//------------------------------ forward-declare -----------//

template<typename ElementType>
class mdkMatrix;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrixForLinearCombination;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//

//---------------------------------------------------------------------------------------------------//
// 10 matrix should be OK
// reserve the capacity of std::vector<mdkMatrix<ElementType>>
#define MDK_GlueMatrixForMultiplication_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkGlueMatrixForMultiplication : public mdkObject
{
private:

    uint64 m_RowNumber;

    uint64 m_ColNumber;

    std::vector<mdkMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    ElementType m_Element_Coef;

    //------------------- constructor and destructor ------------------------------------//
private:
    inline mdkGlueMatrixForMultiplication();

public:
    inline ~mdkGlueMatrixForMultiplication();

private:
    inline mdkGlueMatrixForMultiplication(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) = delete;

    inline mdkGlueMatrixForMultiplication(mdkGlueMatrixForMultiplication<ElementType>&& GlueMatrix);

    //---------------------- Other ----------------------------------------//
private:
    inline void Reset();

    inline uint64 GetRowNumber() const;

    inline uint64 GetColNumber() const;

    inline uint64 GetElementNumber() const;

    inline mdkMatrixSize GetSize() const;

    inline bool IsEmpty() const;

    inline mdkMatrix<ElementType> CreateMatrix() const;

    inline void CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const;

public:

    inline mdkMatrix<ElementType> ElementMultiply(const mdkMatrix<ElementType>& targetMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix);

    inline mdkMatrix<ElementType> ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix);

private:
    //--------------------------- deleted ----------------------------------------//

    void operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix) = delete;
    void operator=(mdkGlueMatrixForMultiplication<ElementType>&& GlueMatrix) = delete;

    //--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename T>
    friend class mdkMatrix;

    //--------------------------- friend function ----------------------------------------------------------------------------------//

};


}// end namespace mdk

#include "mdkGlueMatrixForMultiplication.hpp"

#endif // MDK_Enable_GlueMatrix =====================================================================================

#endif