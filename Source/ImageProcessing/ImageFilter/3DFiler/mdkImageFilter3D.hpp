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

    m_InputRegionOf3DIndex = nullptr;
	m_PixelLinearIndexList_Of_InputRegionOf3DIndex.Clear();

    m_Input3DIndexList = nullptr;

    m_Input3DPositionList = nullptr;

    this->ClearOutput();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputPixelNumber = 0;

    m_MinPixelNumberPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::ClearOutput()
{
    m_OutputImage_SharedCopy.Clear();

    m_OutputImage = &m_OutputImage_SharedCopy;

    m_OutputArray_SharedCopy.Clear();

    m_OutputArray = &m_OutputArray_SharedCopy;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::UpdateOutputPort()
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
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion)
{
    m_InputRegionOf3DIndex = InputRegion;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInput3DIndexList(const DenseMatrix<double>* Input3DIndexList)
{
    m_Input3DIndexList = Input3DIndexList;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::SetInput3DPositionList(const DenseMatrix<double>* Input3DPositionList)
{
    m_Input3DPositionList = Input3DPositionList;
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
void ImageFilter3D<InputPixelType, OutputPixelType>::SetOutputArray(ObjectArray<OutputPixelType>* OutputArray)
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

    if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        auto InputSize = m_InputImage->GetSize();

        m_TotalOutputPixelNumber = InputSize.Lx*InputSize.Ly*InputSize.Lz;
    }
    else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_InputRegionOf3DIndex->Lx()*m_InputRegionOf3DIndex->Ly()*m_InputRegionOf3DIndex->Lz();             
    }
    else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputPixelNumber = m_Input3DIndexList->GetColNumber();
    }
    else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
	{
		m_TotalOutputPixelNumber = m_Input3DPositionList->GetColNumber();
	}    
	else
	{
		MDK_Error("Invalid input @ ImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputRegionOf3DIndex != nullptr)
	{
		m_PixelLinearIndexList_Of_InputRegionOf3DIndex.FastResize(0);
		m_PixelLinearIndexList_Of_InputRegionOf3DIndex.ReserveCapacity(m_TotalOutputPixelNumber);
		for (int_max z_Index = m_InputRegionOf3DIndex->z0; z_Index <= m_InputRegionOf3DIndex->z1; ++z_Index)
		{
			for (int_max y_Index = m_InputRegionOf3DIndex->y0; y_Index <= m_InputRegionOf3DIndex->y1; ++y_Index)
			{
				for (int_max x_Index = m_InputRegionOf3DIndex->x0; x_Index <= m_InputRegionOf3DIndex->x1; ++x_Index)
				{
					auto tempLinearIndex = m_InputImage->Transform3DIndexToLinearIndex(x_Index, y_Index, z_Index);
					m_PixelLinearIndexList_Of_InputRegionOf3DIndex.Append(tempLinearIndex);
				}
			}
		}
	}

    //------------------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputImage == true)
    {
        if (m_InputRegionOf3DIndex == nullptr)
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

            if (m_InputRegionOf3DIndex->Lx() != OutputSize.Lx || m_InputRegionOf3DIndex->Ly() != OutputSize.Ly || m_InputRegionOf3DIndex->Lz() != OutputSize.Lz)
            {
                OutputSize.Lx = m_InputRegionOf3DIndex->Lx();
                OutputSize.Ly = m_InputRegionOf3DIndex->Ly();
                OutputSize.Lz = m_InputRegionOf3DIndex->Lz();

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

    this->UpdateOutputPort();

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilter3D<InputPixelType, OutputPixelType>::Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex)
{
    std::cout << "ImageFilter3D ThreadIndex: " << ThreadIndex
		      << ", OutputPixelIndex_start: " << OutputPixelIndex_start 
              << ", OutputPixelIndex_end: " << OutputPixelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
					double FilterOrigin3DIndex[3];
					m_InputImage->TransformLinearIndexTo3DIndex(m_PixelLinearIndexList_Of_InputRegionOf3DIndex[PixelIndex], 
													            FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                    this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
                }
            }
            else
            {
                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
					double FilterOrigin3DIndex[3];
					m_InputImage->TransformLinearIndexTo3DIndex(m_PixelLinearIndexList_Of_InputRegionOf3DIndex[PixelIndex],
																FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                    this->FilterFunctionAt3DIndex((*m_OutputArray)[PixelIndex], FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
                }
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
					double FilterOrigin3DIndex[3];
                    FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, PixelIndex);
                    FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, PixelIndex);
                    FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, PixelIndex);

                    this->FilterFunctionAt3DIndex((*m_OutputImage)(PixelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
				}
            }
            else // if (m_Flag_OutputArray == true)
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
					double FilterOrigin3DIndex[3];
                    FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, PixelIndex);
                    FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, PixelIndex);
                    FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, PixelIndex);

                    this->FilterFunctionAt3DIndex((*m_OutputArray)[PixelIndex], FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
				}
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{				
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					double FilterOrigin3DPosition[3];
					FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputImage)(PixelIndex), FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);
				}
			}
			else
			{				
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					double FilterOrigin3DPosition[3];
					FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                    this->FilterFunctionAt3DPosition((*m_OutputArray)[PixelIndex], FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);
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

		if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {	
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_OutputImage->TransformLinearIndexTo3DIndex(PixelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);
				FilterOrigin3DIndex[0] += double(m_InputRegionOf3DIndex->x0);
				FilterOrigin3DIndex[1] += double(m_InputRegionOf3DIndex->y0);
				FilterOrigin3DIndex[2] += double(m_InputRegionOf3DIndex->z0);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				double FilterOrigin3DIndex[3];
                FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, PixelIndex);
                FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, PixelIndex);
                FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, PixelIndex);

                this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{			
			for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
			{				
				double FilterOrigin3DPosition[3];
				FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, PixelIndex);
				FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, PixelIndex);
				FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, PixelIndex);

                this->FilterFunctionAt3DPosition(tempOutputPixel, FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);

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
ObjectArray<OutputPixelType>* ImageFilter3D<InputPixelType, OutputPixelType>::GetOutputArray()
{
    return &m_OutputArray_SharedCopy;
}



}//end namespace mdk

#endif