#ifndef __mdkImageFilter_Common_Function_h
#define __mdkImageFilter_Common_Function_h

namespace mdk
{
//---------------------------
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class Image2D;

template<typename ElementType>
class Image3D;
//---------------------------

// Signal can be 1D, 2D, 3D
// Signal < Signal_lb will be discarded
// Signal > Signal_ub will be discarded

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const DenseMatrix<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const Image2D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const Image3D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const ElementType* Signal, int_max SignalLength, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

template<typename ElementType>
bool ComputeHistogram(DenseMatrix<int_max> Histogram,
                      const ElementType* Signal, int_max SignalLength, 
                      ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber);

// smooth a histogram by using Kernal function, such as Gaussian function (Sigma) within Radius
// ElementType is float or double

template<typename ElementType>
DenseMatrix<ElementType> GaussianSmoothHistogram(const DenseMatrix<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
bool GaussianSmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, ElementType Sigma, int_max Radius);

template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal);

template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal);

template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const int_max* Histogram, int_max HistLength, const ElementType* Kernal, int_max KernalLength);

template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const int_max* Histogram, int_max HistLength, const ElementType* Kernal, int_max KernalLength);

}// namespace mdk

#include "mdkImageFilter_Common_Function.hpp"

#endif
