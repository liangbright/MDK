#ifndef mdk_Histogram_h
#define mdk_Histogram_h

#include <limits>

#include "mdkDenseVector_ForwardDeclare.h"

namespace mdk
{
//---------------------------
template<typename T>
class DenseMatrix;

template<typename T>
class DenseImage2D;

template<typename T>
class DenseImage3D;
//---------------------------

// Signal can be 1D, 2D, 3D, ElementType is float or double
// Signal < Signal_lb will be discarded
// Signal > Signal_ub will be discarded
// Signal_ub > Signal_lb 

template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseVector<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const DenseMatrix<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseImage2D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseImage3D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const ElementType* Signal, int_max SignalLength, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
bool ComputeHistogram(int_max* Histogram,
					  const ElementType* Signal, int_max SignalLength, 
                      ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

// smooth a histogram by using Kernal function, such as Gaussian function (Sigma) within Radius
// ElementType is float or double

template<typename ElementType>
DenseVector<ElementType> GaussianSmoothHistogram(const DenseVector<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
bool GaussianSmoothHistogram(DenseVector<ElementType>& SmoothedHistogram, const DenseVector<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
DenseMatrix<ElementType> GaussianSmoothHistogram(const DenseMatrix<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
bool GaussianSmoothHistogram(DenseVector<ElementType>& SmoothedHistogram, const DenseVector<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
bool GaussianSmoothHistogram(ElementType* SmoothedHistogram, const int_max* Histogram, int_max BinNumber, ElementType Sigma, int_max Radius);

template<typename ElementType>
DenseVector<ElementType> SmoothHistogram(const DenseVector<int_max>& Histogram, const DenseVector<ElementType>& Kernal);

template<typename ElementType>
bool SmoothHistogram(DenseVector<ElementType>& SmoothedHistogram, const DenseVector<int_max>& Histogram, const DenseVector<ElementType>& Kernal);

template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal);

template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal);

template<typename ElementType>
bool SmoothHistogram(ElementType* SmoothedHistogram, const int_max* Histogram, int_max BinNumber, const ElementType* Kernal, int_max KernalLength);

// Histogram sum to 1, ElementType is float or double
template<typename ElementType>
DenseVector<ElementType> NormalizeHistogram(const DenseVector<int_max>& Histogram);

template<typename ElementType>
DenseMatrix<ElementType> NormalizeHistogram(const DenseMatrix<int_max>& Histogram);

template<typename ElementType>
bool NormalizeHistogram(ElementType* NormalizedHistogram, const int_max* Histogram, int_max BinNumber);

}// namespace mdk

#include "mdkHistogram.hpp"

#endif
