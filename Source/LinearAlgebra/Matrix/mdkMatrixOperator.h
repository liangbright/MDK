﻿#ifndef mdk_MatrixOperator_h
#define mdk_MatrixOperator_h

namespace mdk
{

//forward-declare -------------------------//

template<typename ElementType>
class mdkMatrix;

struct mdkMatrixSize;

template<typename ElementType>
class mdkShadowMatrix;

template<typename ElementType>
class mdkGlueMatrixForLinearCombination;

template<typename ElementType>
class mdkGlueMatrixForMultiplication;

// end of forward-declare------------------//


//============================================================================================================================================//
//
//                                                    {Matrix} v.s {Matrix, Element}
//
//============================================================================================================================================//

// ----------------------------------------------------- Matrix  {+ - * /}  Matrix ---------------------------------------------------------//

// note: Matrix {+ -} Matrix return GlueMatrixForLinearCombination
// note: Matrix {*}   Matrix return MultiplyGlueMatrix
// note: Matrix {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

// ---------------------------------------------------- Matrix  {+ - * /}  Element ------------------------------------------------------//

// note: Matrix {+ -}  Element return GlueMatrixForLinearCombination
// note: Matrix {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB);

// --------------------------------------------------- Element {+ - * /} Matrix  ---------------------------------------------------------//

// note: Element {+ -} Matrix return GlueMatrixForLinearCombination
// note: Element {*}   Matrix return GlueMatrixForMultiplication
// note: Element {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB);
//============================================================================================================================================//
//
//                                          {ShadowMatrix} v.s {Matrix, Element, ShadowMatrix}
//
//============================================================================================================================================//

// ------------------------------------------ ShadowMatrix {+ - * /}  Matrix ----------------------------------------------------------//

// note: ShadowMatrix {+ -} Matrix return Matrix
// note: ShadowMatrix {*}   Matrix return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   Matrix return Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkMatrix<ElementType>& MatrixB);

// ---------------------------------------------- Matrix {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: Matrix {+ -} ShadowMatrix return Matrix
// note: Matrix {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: Matrix {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * /}  Element ------------------------------------------------------------//

// note: ShadowMatrix {+ - * /} Element return Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const ElementType& ElementB);

// ----------------------------------------- Element {+ - * /} ShadowMatrix --------------------------------------------------------------//

// note: Element {+ - * /} ShadowMatrix return Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ---------------------------------------- ShadowMatrix {+ - * /}  ShadowMatrix -------------------------------------------------------//

// note: Matrix {+ -} Matrix return Matrix
// note: Matrix {*}   Matrix return GlueMatrixForMultiplication
// note: Matrix {/}   Matrix return Matrix

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

//============================================================================================================================================//
//
//                          {GlueMatrixForLinearCombination} vs {Matrix, Element, ShadowMatrix, GlueMatrixForLinearCombination}
//
//============================================================================================================================================//

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication (definition in mdkGlueMatrixForMultiplication.h)
// note: Matrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

// ------------------------------------------------- GlueMatrixForLinearCombination  {+ - * /}  Element -----------------------------------------//

// note: GlueMatrixForLinearCombination {+ - * /}  Element return GlueMatrixForLinearCombination

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator*(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator/(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const ElementType& ElementB);

// ----------------------------------------------- Element {+ - * /} GlueMatrixForLinearCombination ----------------------------------------------//

// note: Element {+ - *} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: Element {/}     GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator*(const ElementType& ElementA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

// -------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForLinearCombination -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

// --------------------------------------------- GlueMatrixForLinearCombination {+ - * /}  GlueMatrixForLinearCombination ---------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB);

//====================================================================================================================================================//
//
//          {GlueMatrixForMultiplication} vs {Matrix, Element, ShadowMatrix, GlueMatrixForLinearCombination, GlueMatrixForMultiplication}
//
//====================================================================================================================================================//

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  Matrix ---------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} Matrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   Matrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   Matrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkMatrix<ElementType>& MatrixB);

// --------------------------------------------- Matrix or {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: Matrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Matrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Matrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

// -------------------------------------------- GlueMatrixForMultiplication  {+ - * /}  Element ------------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -}  Element return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {* /}  Element return GlueMatrixForMultiplication

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator/(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const ElementType& ElementB);

// ---------------------------------------------- Element {+ - * /} GlueMatrixForMultiplication -----------------------------------------------------//

// note: Element {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: Element {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: Element {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const ElementType& ElementA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// -------------------------------------------- GlueMatrixForMultiplication {+ - * /}  ShadowMatrix -----------------------------------------------------//

// note: GlueMatrixForMultiplication {+ -} ShadowMatrix return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   ShadowMatrix return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   ShadowMatrix return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkShadowMatrix<ElementType>& ShadowMatrixB);

// ------------------------------------------ ShadowMatrix {+ - * /} GlueMatrixForMultiplication -------------------------------------------------------//

// note: ShadowMatrix {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: ShadowMatrix {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: ShadowMatrix {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkShadowMatrix<ElementType>& ShadowMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkShadowMatrix<ElementType>& ShadowMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

// -------------------------------------------- GlueMatrixForMultiplication {+ - /}  GlueMatrixForLinearCombination -----------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForLinearCombination return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForLinearCombination return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   GlueMatrixForLinearCombination return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixB_L);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA_M, const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixB_L);

// ------------------------------------------ GlueMatrixForLinearCombination {+ - * /} GlueMatrixForMultiplication -------------------------------------------//

// note: GlueMatrixForLinearCombination {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForLinearCombination {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: GlueMatrixForLinearCombination {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB_M);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrixA_L, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB_M);

// --------------------------------------------- GlueMatrixForMultiplication {+ - * /}  GlueMatrixForMultiplication ---------------------------------------//

// note: GlueMatrixForMultiplication {+ -} GlueMatrixForMultiplication return GlueMatrixForLinearCombination
// note: GlueMatrixForMultiplication {*}   GlueMatrixForMultiplication return GlueMatrixForMultiplication
// note: GlueMatrixForMultiplication {/}   GlueMatrixForMultiplication return Matrix

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator+(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForLinearCombination<ElementType> operator-(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkGlueMatrixForMultiplication<ElementType> operator*(mdkGlueMatrixForMultiplication<ElementType> GlueMatrixA, mdkGlueMatrixForMultiplication<ElementType> GlueMatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixA, const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrixB);

} // namespace mdk

#include "mdkMatrixOperator_Matrix.hpp"
#include "mdkMatrixOperator_ShadowMatrix.hpp"
#include "mdkMatrixOperator_GlueMatrixForLinearCombination.hpp"
#include "mdkMatrixOperator_GlueMatrixForMultiplication.hpp"

#endif 