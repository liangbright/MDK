#ifndef mdk_Histogram_hpp
#define mdk_Histogram_hpp

namespace mdk
{

template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseVector<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
	return ComputeHistogram(Signal.GetElementPointer(), Signal.GetElementNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseMatrix<int_max> ComputeHistogram(const DenseMatrix<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
	DenseMatrix<int_max> Output = ComputeHistogram(Signal.GetElementPointer(), Signal.GetElementNumber(), Signal_lb, Signal_ub, BinNumber);
	return Output;
}


template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseImage2D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    return ComputeHistogram(Signal.GetPixelPointer(), Signal.GetPixelNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const DenseImage3D<ElementType>& Signal, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
    return ComputeHistogram(Signal.GetPixelPointer(), Signal.GetPixelNumber(), Signal_lb, Signal_ub, BinNumber);
}


template<typename ElementType>
DenseVector<int_max> ComputeHistogram(const ElementType* Signal, int_max SignalLength, ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
	DenseVector<int_max> Histogram;
	Histogram.Resize(BinNumber);
    ComputeHistogram(Histogram.GetElementPointer(), Signal, SignalLength, Signal_lb, Signal_ub, BinNumber);
    return Histogram;
}


template<typename ElementType>
bool ComputeHistogram(int_max* Histogram,
	                  const ElementType* Signal, int_max SignalLength, 
                      ElementType Signal_lb, ElementType Signal_ub, int_max BinNumber)
{
	if (Histogram == nullptr || Signal == nullptr || SignalLength <= 0 || Signal_lb >= Signal_ub || BinNumber <= 0)
    {
        MDK_Error("Invalid input @ mdkHistogram ComputeHistogram(...)")
        return false;
    }

	for (int_max i = 0; i < BinNumber; ++i)
	{
		Histogram[i] = 0;
	}

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
DenseVector<ElementType> GaussianSmoothHistogram(const DenseVector<int_max>& Histogram, ElementType Sigma, int_max Radius)
{
	DenseVector<ElementType> SmoothedHistogram;
	GaussianSmoothHistogram(SmoothedHistogram, Sigma, Radius);
	return SmoothedHistogram;
}


template<typename ElementType>
bool GaussianSmoothHistogram(DenseVector<ElementType>& SmoothedHistogram, const DenseVector<int_max>& Histogram, ElementType Sigma, int_max Radius)
{
	SmoothedHistogram.FastResize(Histogram.GetElementNumber());
	return GaussianSmoothHistogram(SmoothedHistogram.GetElementPointer(), Histogram.GetElementPointer(), Histogram.GetElementNumber(), Sigma, Radius);
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
	SmoothedHistogram.FastResize(1, Histogram.GetElementNumber());
	return GaussianSmoothHistogram(SmoothedHistogram.GetElementPointer(), Histogram.GetElementPointer(), Histogram.GetElementNumber(), Sigma, Radius);
}


template<typename ElementType>
bool GaussianSmoothHistogram(ElementType* SmoothedHistogram, const int_max* Histogram, int_max BinNumber, ElementType Sigma, int_max Radius)
{
	if (SmoothedHistogram == nullptr || Histogram == nullptr || BinNumber <= 0 || Sigma <= ElementType(0) || Radius <= 0)
	{
		MDK_Error("Invalid input @ mdkHistogram GaussianSmoothHistogram(...)")
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

	return SmoothHistogram(SmoothedHistogram, Histogram, BinNumber, Kernal.GetElementPointer(), Kernal.GetElementNumber());
}


template<typename ElementType>
DenseVector<ElementType> SmoothHistogram(const DenseVector<int_max>& Histogram, const DenseVector<ElementType>& Kernal)
{
	DenseVector<ElementType> SmoothedHistogram;
	SmoothHistogram(SmoothedHistogram,
	  		        Histogram.GetElementPointer(), Histogram.GetElementNumber(),
		            Kernal.GetElementPointer(), Kernal.GetElementNumber());
	return SmoothedHistogram;
}


template<typename ElementType>
bool SmoothHistogram(DenseVector<ElementType>& SmoothedHistogram, const DenseVector<int_max>& Histogram, const DenseVector<ElementType>& Kernal)
{
	SmoothedHistogram.FastResize(Histogram.GetElementNumber());
	return SmoothHistogram(SmoothedHistogram,
						   Histogram.GetElementPointer(), Histogram.GetElementNumber(),
						   Kernal.GetElementPointer(), Kernal.GetElementNumber());
}


template<typename ElementType>
DenseMatrix<ElementType> SmoothHistogram(const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal)
{
	DenseMatrix<ElementType> SmoothedHistogram;
	SmoothHistogram(SmoothedHistogram, 
	 			    Histogram.GetElementPointer(), Histogram.GetElementNumber(),
                    Kernal.GetElementPointer(),    Kernal.GetElementNumber());
	return SmoothedHistogram;
}


template<typename ElementType>
bool SmoothHistogram(DenseMatrix<ElementType>& SmoothedHistogram, const DenseMatrix<int_max>& Histogram, const DenseMatrix<ElementType>& Kernal)
{
	SmoothedHistogram.FastResize(1, Histogram.GetElementNumber());
	return SmoothHistogram(SmoothedHistogram.GetElementPointer(),
                           Histogram.GetElementPointer(), Histogram.GetElementNumber(),
                           Kernal.GetElementPointer(), Kernal.GetElementNumber());
}


template<typename ElementType>
bool SmoothHistogram(ElementType* SmoothedHistogram, const int_max* Histogram, int_max BinNumber, const ElementType* Kernal, int_max KernalLength)
{
	if (SmoothedHistogram == nullptr || Histogram == nullptr || BinNumber <= 0 || Kernal == nullptr || KernalLength <= 0)
    {
        MDK_Error("Invalid input @ mdkHistogram SmoothHistogram(...)")
        return false;
    }

    //----------- check kernal ----------------------------//

    auto Radius = KernalLength / 2;
    auto rem = KernalLength % 2;

    if (rem != 1)
    {
        MDK_Error("Kernal Length must be odd (e.g., 3, 5, 7) < Kernal Length @ mdkHistogram SmoothHistogram(...)")
        return false;
    }

	if (BinNumber < KernalLength)
    {
        MDK_Error("Histogram BinNumber < Kernal Length @ mdkHistogram SmoothHistogram(...)")
        return false;
    }

    //---------------- smooth -------------------------//

	for (int_max i = 0; i < BinNumber; ++i)
    {
        auto temp = ElementType(0);

        for (int_max k = 0; k < KernalLength; ++k)
        {
            auto Index = i - Radius + k;

            if (Index < 0)
            {
                temp += Kernal[k] * Histogram[0];
            }
			else if (Index >= BinNumber)
            {
				temp += Kernal[k] * Histogram[BinNumber - 1];
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


template<typename ElementType>
DenseVector<ElementType> NormalizeHistogram(const DenseVector<int_max>& Histogram)
{
	DenseVector<ElementType> Hist;
	Hist.Resize(Histogram.GetElementNumber());
	NormalizeHistogram(Hist.GetElementPointer(), Histogram.GetElementPointer(), Histogram.GetElementNumber());
	return Hist;
}


template<typename ElementType>
DenseMatrix<ElementType> NormalizeHistogram(const DenseMatrix<int_max>& Histogram)
{
	DenseMatrix<ElementType> Hist;
	Hist.Resize(1, Histogram.GetElementNumber());
	NormalizeHistogram(Hist.GetElementPointer(), Histogram.GetElementPointer(), Histogram.GetElementNumber());
	return Hist;
}


template<typename ElementType>
bool NormalizeHistogram(ElementType* NormalizedHistogram, const int_max* Histogram, int_max BinNumber)
{
	if (NormalizedHistogram == nullptr || Histogram == nullptr || BinNumber <= 0)
	{
		MDK_Error("Invalid input @ mdkHistogram NormalizeHistogram(...)")
		return false;
	}

	ElementType TotalCount = 0;
	for (int_max i = 0; i < BinNumber; ++i)
	{
		NormalizedHistogram[i] = ElementType(Histogram[i]);
		TotalCount += NormalizedHistogram[i];
	}

	auto EPSValue = std::numeric_limits<ElementType>::epsilon();

	if (TotalCount > EPSValue)
	{
		for (int_max i = 0; i < BinNumber; ++i)
		{
			NormalizedHistogram[i] /= TotalCount;
		}
	}
	else
	{
		MDK_Warning("TotalCount <= eps, set to uniform distribution @ mdkHistogram NormalizeHistogram(...)")

	    for (int_max i = 0; i < BinNumber; ++i)
		{
			NormalizedHistogram[i] = ElementType(1) / ElementType(BinNumber);
		}
	}

	return true;
}


}

#endif