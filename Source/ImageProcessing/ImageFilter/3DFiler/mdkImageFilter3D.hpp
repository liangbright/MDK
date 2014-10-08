#ifndef __mdkImageFilter3D_hpp
#define __mdkImageFilter3D_hpp

//#include "ImageFilter3D.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::ImageFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ImageFilter3D()
{
	// do nothing
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegionOf3DIndex = nullptr;
	m_PixelLinearIndexList_Of_InputRegionOf3DIndex.Clear();

    m_Input3DIndexList = nullptr;

    m_Input3DPositionList = nullptr;

	//ClearOutput
	m_OutputImage.Clear();
	m_OutputArray.Clear();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputPixelNumber = 0;

    m_MinPixelNumberPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetInputImage(const Image3D<InputPixelType>* InputImage)
{
	m_InputImage = InputImage;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion)
{
    m_InputRegionOf3DIndex = InputRegion;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetInput3DIndexList(const DenseMatrix<ScalarType>* Input3DIndexList)
{
    m_Input3DIndexList = Input3DIndexList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetInput3DPositionList(const DenseMatrix<ScalarType>* Input3DPositionList)
{
    m_Input3DPositionList = Input3DPositionList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImage(Image3D<OutputPixelType>* OutputImage)
{
	if (OutputImage == nullptr)
	{		
		MDK_Error("input is nullptr @ImageFilter3D::SetOutputImage(...)")
		return;
	}
	m_OutputImage.Share(OutputImage);
	m_Flag_OutputImage = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputArray(ObjectArray<OutputPixelType>* OutputArray)
{
	if (OutputArray == nullptr)
	{
		MDK_Error("input is nullptr @ImageFilter3D::SetOutputImage(...)")
		return;
	}
	m_OutputArray.Share(OutputArray);
	m_Flag_OutputArray = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetMaxNumberOfThreads(int_max MaxNumber)
{
	m_MaxNumberOfThreads = MaxNumber;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
int_max ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::GetNumberOfThreadTobeCreated()
{
    return Compute_NumberOfThreadTobeCreated_For_ParallelBlock(m_TotalOutputPixelNumber, m_MaxNumberOfThreads, m_MinPixelNumberPerThread);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
    return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
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

        m_TotalOutputPixelNumber = InputSize[0]*InputSize[1]*InputSize[2];
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
            auto OutputSize = m_OutputImage.GetSize();
            if (OutputSize[0] != InputSize[0] || OutputSize[1] != InputSize[1] || OutputSize[2] != InputSize[2])
            {
                m_OutputImage.SetSize(m_InputImage->GetSize());
                m_OutputImage.SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
            }
        }
        else
        {
            auto OutputSize = m_OutputImage.GetSize();

            if (m_InputRegionOf3DIndex->Lx() != OutputSize[0] || m_InputRegionOf3DIndex->Ly() != OutputSize[1] || m_InputRegionOf3DIndex->Lz() != OutputSize[2])
            {
                OutputSize[0] = m_InputRegionOf3DIndex->Lx();
                OutputSize[1] = m_InputRegionOf3DIndex->Ly();
                OutputSize[2] = m_InputRegionOf3DIndex->Lz();

                m_OutputImage.SetSize(OutputSize);
                m_OutputImage.SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
            }
        }        
    }

    if (m_Flag_OutputArray == true)
    {
        if (m_OutputArray.GetElementNumber() != m_TotalOutputPixelNumber)
        {
            m_OutputArray.Clear();
            m_OutputArray.Resize(m_TotalOutputPixelNumber);
        }
    }

    //------------------------------

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update()
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

    return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex)
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
				auto FilterOrigin = m_InputImage->TransformLinearIndexTo3DIndex(PixelIndex);

				this->FilterFunctionAt3DIndex(m_OutputImage(PixelIndex), FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
					auto FilterOrigin =m_InputImage->TransformLinearIndexTo3DIndex(m_PixelLinearIndexList_Of_InputRegionOf3DIndex[PixelIndex]);

					this->FilterFunctionAt3DIndex(m_OutputImage(PixelIndex), FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
                }
            }
            else
            {
                for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
                {
					auto FilterOrigin =m_InputImage->TransformLinearIndexTo3DIndex(m_PixelLinearIndexList_Of_InputRegionOf3DIndex[PixelIndex]);

					this->FilterFunctionAt3DIndex(m_OutputArray[PixelIndex], FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
                }
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
					ScalarType FilterOrigin[3];
					FilterOrigin[0] = (*m_Input3DIndexList)(0, PixelIndex);
					FilterOrigin[1] = (*m_Input3DIndexList)(1, PixelIndex);
					FilterOrigin[2] = (*m_Input3DIndexList)(2, PixelIndex);

					this->FilterFunctionAt3DIndex(m_OutputImage(PixelIndex), FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
				}
            }
            else // if (m_Flag_OutputArray == true)
            {
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{
					ScalarType FilterOrigin[3];
					FilterOrigin[0] = (*m_Input3DIndexList)(0, PixelIndex);
					FilterOrigin[1] = (*m_Input3DIndexList)(1, PixelIndex);
					FilterOrigin[2] = (*m_Input3DIndexList)(2, PixelIndex);

					this->FilterFunctionAt3DIndex(m_OutputArray[PixelIndex], FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
				}
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{				
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					ScalarType FilterOrigin[3];
					FilterOrigin[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterOrigin[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterOrigin[2] = (*m_Input3DPositionList)(2, PixelIndex);

					this->FilterFunctionAt3DPosition(m_OutputImage(PixelIndex), FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
				}
			}
			else
			{				
				for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
				{					
					ScalarType FilterOrigin[3];
					FilterOrigin[0] = (*m_Input3DPositionList)(0, PixelIndex);
					FilterOrigin[1] = (*m_Input3DPositionList)(1, PixelIndex);
					FilterOrigin[2] = (*m_Input3DPositionList)(2, PixelIndex);

					this->FilterFunctionAt3DPosition(m_OutputArray[PixelIndex], FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);
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
		OutputPixelType tempOutputPixel;

		if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				auto FilterOrigin = m_InputImage->TransformLinearIndexTo3DIndex<ScalarType>(PixelIndex);

				this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {	
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				auto FilterOrigin = m_InputImage->TransformLinearIndexTo3DIndex<ScalarType>(PixelIndex);
				FilterOrigin[0] += ScalarType(m_InputRegionOf3DIndex->x0);
				FilterOrigin[1] += ScalarType(m_InputRegionOf3DIndex->y0);
				FilterOrigin[2] += ScalarType(m_InputRegionOf3DIndex->z0);

				this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
            {
				ScalarType FilterOrigin[3];
				FilterOrigin[0] = (*m_Input3DIndexList)(0, PixelIndex);
				FilterOrigin[1] = (*m_Input3DIndexList)(1, PixelIndex);
				FilterOrigin[2] = (*m_Input3DIndexList)(2, PixelIndex);

				this->FilterFunctionAt3DIndex(tempOutputPixel, FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{			
			for (int_max PixelIndex = OutputPixelIndex_start; PixelIndex <= OutputPixelIndex_end; ++PixelIndex)
			{				
				ScalarType FilterOrigin[3];
				FilterOrigin[0] = (*m_Input3DPositionList)(0, PixelIndex);
				FilterOrigin[1] = (*m_Input3DPositionList)(1, PixelIndex);
				FilterOrigin[2] = (*m_Input3DPositionList)(2, PixelIndex);

				this->FilterFunctionAt3DPosition(tempOutputPixel, FilterOrigin[0], FilterOrigin[1], FilterOrigin[2], ThreadIndex);

                this->OutputFunction(PixelIndex, tempOutputPixel, ThreadIndex);
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Update_in_a_Thread")
        }
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
void ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::OutputFunction(int_max OutputPixelIndex, OutputPixelType& OutputPixel, int_max ThreadIndex)
{
    if (m_Flag_OutputImage == true)
    {
        m_OutputImage(OutputPixelIndex) = OutputPixel;
    }
    else if (m_Flag_OutputArray == true)
    {
        m_OutputArray[OutputPixelIndex] = OutputPixel;
    }
    else
    {
        MDK_Warning("OutputFunction is empty @ ImageFilter3D::OutputFunction")
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
Image3D<OutputPixelType>& ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::OutputImage()
{
    return m_OutputImage;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ObjectArray<OutputPixelType>& ImageFilter3D<InputPixelType, OutputPixelType, ScalarType>::OutputArray()
{
    return m_OutputArray;
}



}//end namespace mdk

#endif