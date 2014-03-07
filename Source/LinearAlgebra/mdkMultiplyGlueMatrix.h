#ifndef __mdkMultipyGlueMatrix_h
#define __mdkMultipyGlueMatrix_h

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
class mdkLinearCombineGlueMatrix;

template<typename ElementType>
class mdkMultiplyGlueMatrix;

struct mdkMatrixSize;
//--------------------------- end of forward-declare -------//



//---------------------------------------------------------------------------------------------------------------------------------//

// 100 scalars and 100 matrix should be OK
// reserve the capacity of std::vector
#define MDK_MultipyGlueMatrix_ReservedCapacity  100
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkMultipyGlueMatrix : public mdkObject
{
private:

    std::vector<mdkMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    //------------------- constructor and destructor ------------------------------------//
private:
    inline mdkMultipyGlueMatrix();

public:
    inline ~mdkMultipyGlueMatrix();

private:
    inline mdkMultipyGlueMatrix(const mdkMultipyGlueMatrix<ElementType>& GlueMatrix) = delete;

    inline mdkMultipyGlueMatrix(mdkMultipyGlueMatrix<ElementType>&& GlueMatrix);

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

    //--------------------------- deleted ----------------------------------------//

    void operator=(const mdkMultipyGlueMatrix<ElementType>& GlueMatrix) = delete;
    void operator=(mdkMultipyGlueMatrix<ElementType>&& GlueMatrix) = delete;

    //--------------------------- friend class ----------------------------------------------------------------------------------//

    template<typename T>
    friend class mdkMatrix;

    //--------------------------- friend function ----------------------------------------------------------------------------------//

};

}// end namespace mdk

#include "mdkMultiplyGlueMatrix.hpp"

#endif // MDK_Enable_GlueMatrix =====================================================================================

#endif