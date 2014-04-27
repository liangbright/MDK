#ifndef __mdkImageFilter_Common_Function_hpp
#define __mdkImageFilter_Common_Function_hpp

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const DenseMatrix<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    return ComputeHistogram(Signal.GetElementPointer(), Signal.GetElementNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const Image2D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    return ComputeHistogram(Signal.GetPixelPointer(), Signal.GetPixelNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const Image3D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    return ComputeHistogram(Signal.GetPixelPointer(), Signal.GetPixelNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const ElementType* Signal, int_max SignalLength, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    DenseMatrix<int_max> Histogram;

    ComputeHistogram(Histogram, Signal, SignalLength, Signal_lb, Signal_ub, BinNumber);

    return Histogram;
}


template<typename ElementType>
bool ComputeHistogram(DenseMatrix<int_max>& Histogram,
                      const ElementType* Signal, int_max SignalLength, 
                      ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    if (Signal == nullptr || SignalLength <= 0 || Signal_lb >= Signal_ub || BinNumber <= 0)
    {
        MDK_Error("Invalid input @ ImageFilter_Common_Function ComputeHistogram(...)")
        return false;
    }

    Histogram.FastResize(1, BinNumber);

    Histogram.Fill(0);

    auto BinLength = (double(Signal_ub) - double(Signal_lb)) / double(BinNumber);

    for (int_max i = 0; i < SignalLength; ++i)
    {
        auto temp = double(Signal[i]);

        if (temp >= double(Signal_lb) && temp <= double(Signal_ub))
        {
            auto Index = int_max((temp - double(Signal_lb)) / BinLength);
            if (Index == BinNumber)
            {
                Index = BinNumber - 1;
            }

            Histogram[Index] += 1;
        }
    }

    return true;
}


template<typename ElementType>
DenseMatrix<ElementType> GaussianSmoothHistogram(const DenseMatrix<int_max>& Histogram, ElementType Sigma, int_max Radius)
{
    DenseMatrix<ElementType> SmoothedHistogram;

    GaussianSmoothHistogram(SmoothedHistogram, Sigma, Radius);

    return SmoothedHistogram;
}


template<typename ElementType>
bool GaussianSmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, ElementType Sigma, int_max Radius)
{
    if (Histogram.IsEmpty() == true || Sigma <= ElementType(0) || Radius <= 0)
    {
        MDK_Error("Invalid input @ ImageFilter_Common_Function GaussianSmoothHistogram(...)")
        return false;
    }

    //----------- compute kernal ----------------------------//

    DenseMatrix<ElementType> kernal(1, 2 * Radius + 1);

    auto sum = ElementType(0);

    for (int_max k = 0; k < 2 * Radius + 1; ++k)
    {
        auto temp = std::exp(-0.5*(k - Radius)*(k - Radius) / (Sigma*Sigma));
        Kernal[k] = temp;
        sum += temp;
    }

    Kernal /= sum;

    //--------------- smooth--------------------------------//

    return SmoothHistogram(SmoothedHistogram,
                           Histogram.GetElementPointer(), Histogram.GetElementNumber(),
                           Kernal.GetElementPointer(), Kernal.GetElementNumber());
}


template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal)
{
    return SmoothHistogram(Histogram.GetElementPointer(), Histogram.GetElementNumber(),
                           Kernal.GetElementPointer(),    Kernal.GetElementNumber());
}


template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal)
{
    return SmoothHistogram(SmoothedHistogram, 
                           Histogram.GetElementPointer(), Histogram.GetElementNumber(),
                           Kernal.GetElementPointer(), Kernal.GetElementNumber());
}



template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const int_max* Histogram, int_max HistLength, const ElementType* Kernal, int_max KernalLength)
{
    DenseMatrix<ElementType> SmoothedHistogram;

    SmoothHistogram(SmoothedHistogram, Histogram, HistLength, Kernal, KernalLength);

    return SmoothedHistogram;
}


template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const int_max* Histogram, int_max HistLength, const ElementType* Kernal, int_max KernalLength)
{
    if (Histogram == nullptr || HistLength <= 0 || Kernal == nullptr || KernalLength <= 0)
    {
        MDK_Error("Invalid input @ ImageFilter_Common_Function SmoothHistogram(...)")
        return false;
    }

    //----------- check kernal ----------------------------//

    auto Radius = KernalLength / 2;
    auto rem = KernalLength % 2;

    if (rem != 1)
    {
        MDK_Error("Kernal Length must be odd (e.g., 3, 5, 7) < Kernal Length @ ImageFilter_Common_Function SmoothHistogram(...)")
        return false;
    }

    if (HistLength < KernalLength)
    {
        MDK_Error("Histogram Length < Kernal Length @ ImageFilter_Common_Function SmoothHistogram(...)")
        return false;
    }

    //---------------- smooth -------------------------//

    SmoothedHistogram.FastResize(1, HistLength);

    for (int_max i = 0; i < HistLength; ++i)
    {
        auto temp = ElementType(0);

        for (int_max k = 0; k < KernalLength; ++k)
        {
            auto Index = i - Radius + k;

            if (Index < 0)
            {
                temp += Kernal[k] * Histogram[0];
            }
            else if (Index >= HistLength)
            {
                temp += Kernal[k] * Histogram[HistLength - 1];
            }
            else
            {
                temp += Kernal[k] * Histogram[Index];
            }                    
        }

        SmoothedHistogram[i] = temp;
    }

    return true;
}


}

#endif