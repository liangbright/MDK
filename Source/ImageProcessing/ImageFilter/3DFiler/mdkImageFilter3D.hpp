#ifndef __mdkImageFilter3D_hpp
#define __mdkImageFilter3D_hpp

//#include "ImageFilter3D.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ImageFilter3D<InputPixelType, OutputPixelType>::ImageFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
ImageFilter3D<InputPixelType, OutputPixelType>::~ImageFilter3D()
{
	// do nothing
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegion = nullptr;

    m_InputPixel3DIndexList = nullptr;

    m_InputPixel3DPositionList = nullptr;

    this->ClearPipelineOutput();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputPixelNumber = 0;

    m_MinPixelNumberPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::ClearPipelineOutput()
{
    m_OutputImage_SharedCopy.Clear();

    m_OutputImage = &m_OutputImage_SharedCopy;

    m_OutputArray_SharedCopy.Clear();

    m_OutputArray = &m_OutputArray_SharedCopy;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::UpdatePipelineOutput()
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


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInputImage(const Image3D<InputPixelType>* InputImage)
{
	m_InputImage = InputImage;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInputRegion(const DenseMatrix<int_max>* InputRegion)
{
    m_InputRegion = InputRegion;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInputPixel3DIndexList(const DenseMatrix<int_max>* InputPixel3DIndexList)
{
    m_InputPixel3DIndexList = InputPixel3DIndexList;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInputPixel3DPositionList(const DenseMatrix<float>* InputPixel3DPositionList)
{
    m_InputPixel3DPositionList = InputPixel3DPositionList;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetOutputImage(Image3D<OutputPixelType>* OutputImage)
{
    if (OutputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter3D::SetOutputImage")
        return;
    }

	m_OutputImage = OutputImage;

    m_OutputImage_SharedCopy.Share(OutputImage);

    m_Flag_OutputImage = true;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetOutputArray(DataArray<OutputPixelType>* OutputArray)
{
    if (OutputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter3D::OutputArray")
        return;
    }

	m_OutputArray = OutputArray;

    m_OutputArray_SharedCopy.Share(OutputArray);

    m_Flag_OutputArray = true;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetMaxNumberOfThreads(int_max MaxNumber)
{
	m_MaxNumberOfThreads = MaxNumber;
}


template<typename InputPixelType, typename OutputPixelType>
int_max ImageFilter3D<InputPixelType, OutputPixelType>::GetNumberOfThreadsTobeCreated()
{
    return Compute_NumberOfThreadsTobeCreated_For_ParallelBlock(m_TotalOutputPixelNumber, m_MaxNumberOfThreads, m_MinPixelNumberPerThread);
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilter3D<InputPixelType, OutputPixelType>::Preprocess()
{
	return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilter3D<InputPixelType, OutputPixelType>::Postprocess()
{
    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilter3D<InputPixelType, OutputPixelType>::CheckInput()
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

    if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList == nullptr)
    {
        auto InputSize = m_InputImage->GetSize();

        m_TotalOutputPixelNumber = InputSize.Lx*InputSize.Ly*InputSize.Lz;
    }
    else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_InputRegion->Lx()*m_InputRegion->Ly()*m_InputRegion->Lz();             
    }
    else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr && m_InputPixel3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_InputPixel3DIndexList->GetColNumber();
    }
    else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList != nullptr)
	{
		m_TotalOutputPixelNumber = m_InputPixel3DPositionList->GetColNumber();
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
                m_OutputImage->SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage->SetOrigin(m_InputImage->GetOrigin());
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
                m_OutputImage->SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage->SetOrigin(m_InputImage->GetOrigin());
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


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilter3D<InputPixelType, OutputPixelType>::Update()
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
   
    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex){this->Update_in_a_Thread(Index_start, Index_end, ThreadIndex); },
                  0, m_TotalOutputPixelNumber - 1, m_MaxNumberOfThreads, m_MinPixelNumberPerThread);

    //---------------------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex)
{
    std::cout << "ThreadIndex: " << ThreadIndex
              << ", OutputPixelIndex_start: " << OutputPixelIndex_start 
              << ", OutputPixelIndex_end: " << OutputPixelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr  && m_InputPixel3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);
            }
        }
		else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr  && m_InputPixel3DPositionList == nullptr)
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
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr  && m_InputPixel3DPositionList == nullptr)
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
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{
				double FilterCenter3DPosition[3];

				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_InputPixel3DPositionList)(0, PixelIndex);
					FilterCenter3DPosition[1] = (*m_InputPixel3DPositionList)(1, PixelIndex);
					FilterCenter3DPosition[2] = (*m_InputPixel3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputImage)(PixelIndex), FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);
				}
			}
			else
			{
				double FilterCenter3DPosition[3];

				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_InputPixel3DPositionList)(0, PixelIndex);
					FilterCenter3DPosition[1] = (*m_InputPixel3DPositionList)(1, PixelIndex);
					FilterCenter3DPosition[2] = (*m_InputPixel3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputArray)[PixelIndex], FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);
				}
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Update_in_a_Thread")
        }
    }
    else // output to another place
    {
        auto tempOutputPixel = OutputPixelType(0);

		if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegion != nullptr && m_InputPixel3DIndexList == nullptr && m_InputPixel3DPositionList == nullptr)
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
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList != nullptr && m_InputPixel3DPositionList == nullptr)
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
		else if (m_InputRegion == nullptr && m_InputPixel3DIndexList == nullptr  && m_InputPixel3DPositionList != nullptr)
		{
			double FilterCenter3DPosition[3];

			for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
			{				
				FilterCenter3DPosition[0] = (*m_InputPixel3DPositionList)(0, PixelIndex);
				FilterCenter3DPosition[1] = (*m_InputPixel3DPositionList)(1, PixelIndex);
				FilterCenter3DPosition[2] = (*m_InputPixel3DPositionList)(2, PixelIndex);

                this->FilterFunctionAt3DPosition(tempOutputPixel, FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Update_in_a_Thread")
        }
    }
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ImageFilter3D<InputPixelType, OutputPixelType>::OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex)
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


template<typename InputPixelType, typename OutputPixelType>
Image3D<OutputPixelType>* ImageFilter3D<InputPixelType, OutputPixelType>::GetOutputImage()
{
    return &m_OutputImage_SharedCopy;
}


template<typename InputPixelType, typename OutputPixelType>
DataArray<OutputPixelType>* ImageFilter3D<InputPixelType, OutputPixelType>::GetOutputArray()
{
    return &m_OutputArray_SharedCopy;
}



}//end namespace mdk

#endif