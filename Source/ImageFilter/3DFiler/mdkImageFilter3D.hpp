#ifndef __mdkImageFilter3D_hpp
#define __mdkImageFilter3D_hpp

//#include "ImageFilter3D.h"

namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
ImageFilter3D<PixelType_Input, PixelType_Output>::ImageFilter3D()
{
    this->Clear();
}


template<typename PixelType_Input, typename PixelType_Output>
ImageFilter3D<PixelType_Input, PixelType_Output>::~ImageFilter3D()
{
	// do nothing
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegion = nullptr;

    m_InputPixel3DIndexList = nullptr;

	m_Input3DPositionList = nullptr;

    this->SetupDefaultPipelineOutput();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_IsBoundCheckEnabled = true;

    m_IsInputFilterFunctionAt3DPositionObtained = false;

    m_IsInputFilterFunctionAt3DIndexObtained = false;

    m_ZeroPixelOfInputImage = PixelType_Input(0);

    m_ZeroPixelOfOutputImage = PixelType_Output(0);

    m_TotalOutputPixelNumber = 0;

    m_MinPixelNumberPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetupDefaultPipelineOutput()
{
    m_OutputImage_SharedCopy.Clear();

    m_OutputImage = &m_OutputImage_SharedCopy;

    m_OutputArray_SharedCopy.Clear();

    m_OutputArray = &m_OutputArray_SharedCopy;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::UpdatePipelineOutput()
{
    if (m_OutputImage != &m_OutputImage_SharedCopy)
    {
        m_OutputImage_SharedCopy.Share(m_OutputImage);
    }

    if (m_OutputArray != &m_OutputArray_SharedCopy)
    {
        m_OutputArray_SharedCopy.Share(m_OutputArray);
    }
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::EnableBoundCheck()
{
	m_IsBoundCheckEnabled = true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::DisableBoundCheck()
{
    m_IsBoundCheckEnabled = false;
}


template<typename PixelType_Input, typename PixelType_Output>
inline
bool ImageFilter3D<PixelType_Input, PixelType_Output>::IsBoundCheckEnabled()
{
    return m_IsBoundCheckEnabled;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>:: SetInputImage(const Image3D<PixelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter3D::SetInputImage")
        return;
    }

	m_InputImage = InputImage;

    m_ZeroPixelOfInputImage = PixelType_Input(0);
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetInputRegion(const DenseMatrix<int_max>* InputRegion)
{
    m_InputRegion = InputRegion;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetInputPixel3DIndexList(const DenseMatrix<int_max>* InputPixel3DIndexList)
{
    m_InputPixel3DIndexList = InputPixel3DIndexList;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList)
{
	m_Input3DPositionList = Input3DPositionList;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::
SetInputFilterFunctionAt3DIndex(std::function<void(PixelType_Output&, int_max, int_max, int_max, const Image3D<PixelType_Input>&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DIndex = InputFilterFunction;

    m_IsInputFilterFunctionAt3DIndexObtained = true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::
SetInputFilterFunctionAt3DPosition(std::function<void(PixelType_Output&, double, double, double, const Image3D<PixelType_Input>&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DPosition = InputFilterFunction;

    m_IsInputFilterFunctionAt3DPositionObtained = true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetOutputImage(Image3D<PixelType_Output>* OutputImage)
{
    if (OutputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter3D::SetOutputImage")
        return;
    }

	m_OutputImage = OutputImage;

    m_ZeroPixelOfOutputImage = PixelType_Output(0);

    m_Flag_OutputImage = true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetOutputArray(DataContainer<PixelType_Output>* OutputArray)
{
    if (OutputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter3D::OutputArray")
        return;
    }

	m_OutputArray = OutputArray;

    m_Flag_OutputArray = true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::SetMaxNumberOfThreads(int_max MaxNumber)
{
	m_MaxNumberOfThreads = MaxNumber;
}


template<typename PixelType_Input, typename PixelType_Output>
int_max ImageFilter3D<PixelType_Input, PixelType_Output>::GetNumberOfThreadsTobeCreated()
{
    return Compute_NecessaryNumberOfThreads_For_ParallelBlock(m_TotalOutputPixelNumber, m_MaxNumberOfThreads, m_MinPixelNumberPerThread);
}


template<typename PixelType_Input, typename PixelType_Output>
bool ImageFilter3D<PixelType_Input, PixelType_Output>::Preprocess()
{
	return true;
}


template<typename PixelType_Input, typename PixelType_Output>
bool ImageFilter3D<PixelType_Input, PixelType_Output>::Postprocess()
{
    return true;
}


template<typename PixelType_Input, typename PixelType_Output>
bool ImageFilter3D<PixelType_Input, PixelType_Output>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ImageFilter3D::CheckInput()")
		return false;
	}

	//------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputToOtherPlace == false)
    {
        if (m_Flag_OutputImage == true && m_Flag_OutputArray == true)
        {
            MDK_Error("Can not output image and array at the same time @ ImageFilter3D::CheckInput()")
            return false;
        }

        if (m_Flag_OutputImage == false && m_Flag_OutputArray == false)
        {
            MDK_Error("NO output is selected @ ImageFilter3D::CheckInput()")
            return false;
        }
    }
    else
    {
        if (m_Flag_OutputImage == true || m_Flag_OutputArray == true)
        {
            MDK_Error("Can not output image or array when m_Flag_OutputToOtherPlace is true  @ ImageFilter3D::CheckInput()")
            return false;
        }
    }
        
    //-------------------------------------------------------------------------------------------------------------
    m_TotalOutputPixelNumber = 0;

    if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        auto InputSize = m_InputImage->GetSize();

        m_TotalOutputPixelNumber = InputSize.Lx*InputSize.Ly*InputSize.Lz;
    }
    else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_InputRegion->Lx()*m_InputRegion->Ly()*m_InputRegion->Lz();             
    }
    else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_InputPixel3DIndexList->GetColNumber();
    }
    else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList != nullptr)
	{
		m_TotalOutputPixelNumber = m_Input3DPositionList->GetColNumber();
	}    
	else
	{
		MDK_Error("Invalid input @ ImageFilter3D::CheckInput()")
		return false;
	}

    //------------------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputImage == true)
    {
        if (m_InputRegion == nullptr)
        {
            auto InputSize = m_InputImage->GetSize();

            auto OutputSize = m_OutputImage->GetSize();

            if (OutputSize.Lx != InputSize.Lx || OutputSize.Ly != InputSize.Ly || OutputSize.Lz != InputSize.Lz)
            {
                m_OutputImage->SetSize(m_InputImage->GetSize());
                m_OutputImage->SetPixelSpacing(m_InputImage->GetPixelSpacing());
                m_OutputImage->SetPhysicalOrigin(m_InputImage->GetPhysicalOrigin());
            }
        }
        else
        {
            auto OutputSize = m_OutputImage->GetSize();

            if (m_InputRegion->Lx() != OutputSize.Lx || m_InputRegion->Ly() != OutputSize.Ly || m_InputRegion->Lz() != OutputSize.Lz)
            {
                OutputSize.Lx = m_InputRegion->Lx();
                OutputSize.Ly = m_InputRegion->Ly();
                OutputSize.Lz = m_InputRegion->Lz();

                m_OutputImage->SetSize(OutputSize);
                m_OutputImage->SetPixelSpacing(m_InputImage->GetPixelSpacing());
                m_OutputImage->SetPhysicalOrigin(m_InputImage->GetPhysicalOrigin());
            }
        }        
    }

    if (m_Flag_OutputArray == true)
    {
        if (m_OutputArray->GetElementNumber() != m_TotalOutputPixelNumber)
        {
            m_OutputArray->Clear();
            m_OutputArray->Resize(m_TotalOutputPixelNumber);
        }
    }

    //------------------------------

	return true;
}


template<typename PixelType_Input, typename PixelType_Output>
bool ImageFilter3D<PixelType_Input, PixelType_Output>::Update()
{
    //-------------------------------------------------------------------------------

    if (this->CheckInput() == false)
    {
        return false;
    }

    //-------------------------------------------------------------------------------

    if (this->Preprocess() == false)
	{
		return false;
	}

	// multi-thread -----------------------------------------------------------------
   
    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->Run_in_a_Thread(Index_start, Index_end, ThreadIndex); },
                  0, m_TotalOutputPixelNumber - 1, m_MaxNumberOfThreads, m_MinPixelNumberPerThread);

    //---------------------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}


template<typename PixelType_Input, typename PixelType_Output>
void ImageFilter3D<PixelType_Input, PixelType_Output>::Run_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex)
{
    std::cout << "PixelIndex_start " << OutputPixelIndex_start << '\n';
    std::cout << "PixelIndex_end   " << OutputPixelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
            }
        }
		else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0;
            RegionOrigin[1] = m_InputRegion->y0;
            RegionOrigin[2] = m_InputRegion->z0;

            if (m_Flag_OutputImage == true)
            {
				int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
                    m_OutputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                    FilterCenter3DIndex[0] += RegionOrigin[0];
                    FilterCenter3DIndex[1] += RegionOrigin[1];
                    FilterCenter3DIndex[2] += RegionOrigin[2];

                    this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
                }
            }
            else
            {
				int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
                    m_OutputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                    FilterCenter3DIndex[0] += RegionOrigin[0];
                    FilterCenter3DIndex[1] += RegionOrigin[1];
                    FilterCenter3DIndex[2] += RegionOrigin[2];

                    this->FilterFunctionAt3DIndex((*m_OutputArray)[PixelIndex], FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
                }
            }
        }
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

            if (m_Flag_OutputImage == true)
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
                    FilterCenter3DIndex[0] = (*m_InputPixel3DIndexList)(0, PixelIndex);
                    FilterCenter3DIndex[1] = (*m_InputPixel3DIndexList)(1, PixelIndex);
                    FilterCenter3DIndex[2] = (*m_InputPixel3DIndexList)(2, PixelIndex);

                    this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
				}
            }
            else
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
                    FilterCenter3DIndex[0] = (*m_InputPixel3DIndexList)(0, PixelIndex);
                    FilterCenter3DIndex[1] = (*m_InputPixel3DIndexList)(1, PixelIndex);
                    FilterCenter3DIndex[2] = (*m_InputPixel3DIndexList)(2, PixelIndex);

                    this->FilterFunctionAt3DIndex((*m_OutputArray)[PixelIndex], FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
				}
            }
        }
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{
				double FilterCenter3DPosition[3];

				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputImage)(PixelIndex), FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);
				}
			}
			else
			{
				double FilterCenter3DPosition[3];

				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputArray)[PixelIndex], FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);
				}
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Run_in_a_Thread")
        }
    }
    else // output to another place
    {
        auto tempOutputPixel = m_ZeroPixelOfOutputImage;

		if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

			int_max RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0;
            RegionOrigin[1] = m_InputRegion->y0;
            RegionOrigin[2] = m_InputRegion->z0;

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                m_OutputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                FilterCenter3DIndex[0] += RegionOrigin[0];
                FilterCenter3DIndex[1] += RegionOrigin[1];
                FilterCenter3DIndex[2] += RegionOrigin[2];

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                FilterCenter3DIndex[0] = (*m_InputPixel3DIndexList)(0, PixelIndex);
                FilterCenter3DIndex[1] = (*m_InputPixel3DIndexList)(1, PixelIndex);
                FilterCenter3DIndex[2] = (*m_InputPixel3DIndexList)(2, PixelIndex);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr  && m_Input3DPositionList != nullptr)
		{
			double FilterCenter3DPosition[3];

			for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
			{				
				FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
				FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
				FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                this->FilterFunctionAt3DPosition(tempOutputPixel, FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Run_in_a_Thread")
        }
    }
}


template<typename PixelType_Input, typename PixelType_Output>
inline
void ImageFilter3D<PixelType_Input, PixelType_Output>::
FilterFunctionAt3DIndex(PixelType_Output& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
{
	// input (x,y,z) may be 3DIndex or 3DPosition, or 3DPosition with spacing =[1,1,1]
	/*
	PixelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
	Output += i * (*m_InputImage)(xIndex_center, yIndex_center, zIndex_center);
	}
	*/

    if (m_IsInputFilterFunctionAt3DIndexObtained == true)
	{
        m_InputFilterFunction_At3DIndex(OutputPixel, x_Index, y_Index, z_Index, *m_InputImage);
	}
    else
    {
        MDK_Warning("m_IsInputFilterFunctionAt3DIndexObtained == false @ ImageFilter3D::FilterFunctionAt3DIndex")
    }

}


template<typename PixelType_Input, typename PixelType_Output>
inline
void ImageFilter3D<PixelType_Input, PixelType_Output>::
FilterFunctionAt3DPosition(PixelType_Output& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
// input (x,y,z) may be 3DIndex or 3DPosition, or 3DPosition with spacing =[1,1,1]
	/*
	PixelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
		Output += i * (*m_InputImage)(xIndex_center, yIndex_center, zIndex_center);
	}
	*/

    if (m_IsInputFilterFunctionAt3DPositionObtained == true)
	{
        m_InputFilterFunction_At3DPosition(OutputPixel, x, y, z, *m_InputImage);
	}
    else
    {
        MDK_Warning("m_IsInputFilterFunctionAt3DPositionObtained == false @ ImageFilter3D::FilterFunctionAt3DPosition")
    }

}


template<typename PixelType_Input, typename PixelType_Output>
inline
void ImageFilter3D<PixelType_Input, PixelType_Output>::OutputFunction(int_max OutputPixelIndex, const PixelType_Output& OutputPixel, int_max ThreadIndex)
{
    if (m_Flag_OutputImage == true)
    {
        (*m_OutputImage)(OutputPixelIndex) = OutputPixel;
    }
    else if (m_Flag_OutputArray == true)
    {
        (*m_OutputArray)[OutputPixelIndex] = OutputPixel;
    }
    else
    {
        MDK_Warning("OutputFunction is empty @ ImageFilter3D::OutputFunction")
    }
}


template<typename PixelType_Input, typename PixelType_Output>
Image3D<PixelType_Output>* ImageFilter3D<PixelType_Input, PixelType_Output>::GetOutputImage()
{
    return &m_OutputImage_SharedCopy;
}


template<typename PixelType_Input, typename PixelType_Output>
DataContainer<PixelType_Output>* ImageFilter3D<PixelType_Input, PixelType_Output>::GetOutputArray()
{
    return &m_OutputArray_SharedCopy;
}



}//end namespace mdk

#endif