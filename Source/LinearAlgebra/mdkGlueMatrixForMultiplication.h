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

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  GlueMatrixForMultiplication ---------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForMultiplication return Matrix
// note: GlueMatrixForMultiplication {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// ----------------------------------------------- Matrix  {*}  Matrix ----------------------------------------------------------------------------//

// note: Matrix {+ -} Matrix return GlueMatrixForLinearCombination (definition in mdkGlueMatrixForLinearCombination.h)
// note: Matrix {*}   Matrix return GlueMatrixForMultiplication
// note: Matrix {/}   Matrix return Matrix (definition in mdkMatrix.h)

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Matrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// -------------------------------------------- GlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -}  Element return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType>& operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator/(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const ElementType& ElementB);

// ---------------------------------------------- Element {+ - * /} GlueMatrixForMultiplication -----------------------------------------------------//

// note: Element {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Element {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Element {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// -------------------------------------------- GlueMatrixForMultiplication {+ - /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// -------------------------------------------- GlueMatrixForMultiplication {+ - /}  GlueMatrixForLinearCombination -----------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

// ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType>& operator*(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M);


//---------------------------------------------------------------------------------------------------//
// 10 matrix should be OK
// reserve the capacity of std::vector<mdkMatrix<ElementType>>
#define MDK_GlueMatrixForMultiplication_ReservedCapacity  10
//--------------------------------------------------------------------------------------------------//

template<typename ElementType>
class mdkGlueMatrixForMultiplication : public mdkObject
{
private:

    std::vector<mdkMatrix<ElementType>> m_SourceMatrixSharedCopyList;

    ElementType m_Element_coef;

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