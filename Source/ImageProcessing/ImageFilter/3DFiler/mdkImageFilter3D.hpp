#ifndef __mdkImageFilter3D_hpp
#define __mdkImageFilter3D_hpp

//#include "ImageFilter3D.h"

namespace mdk
{

template<typename InputVoxelType, typename OutputVoxelType>
ImageFilter3D<InputVoxelType, OutputVoxelType>::ImageFilter3D()
{
    this->Clear();
}


template<typename InputVoxelType, typename OutputVoxelType>
ImageFilter3D<InputVoxelType, OutputVoxelType>::~ImageFilter3D()
{
	// do nothing
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegionOf3DIndex = nullptr;
	m_VoxelLinearIndexList_Of_InputRegionOf3DIndex.Clear();

    m_Input3DIndexList = nullptr;

    m_Input3DPositionList = nullptr;

	//ClearOutput
	m_OutputImage.Clear();
	m_OutputArray.Clear();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputVoxelNumber = 0;

    m_MinVoxelNumberPerThread = 1;

    m_MaxNumberOfThreads = 1;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetInputImage(const Image3D<InputVoxelType>* InputImage)
{
	m_InputImage = InputImage;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion)
{
    m_InputRegionOf3DIndex = InputRegion;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetInput3DIndexList(const DenseMatrix<double>* Input3DIndexList)
{
    m_Input3DIndexList = Input3DIndexList;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetInput3DPositionList(const DenseMatrix<double>* Input3DPositionList)
{
    m_Input3DPositionList = Input3DPositionList;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetOutputImage(Image3D<OutputVoxelType>* OutputImage)
{
	if (OutputImage == nullptr)
	{		
		MDK_Error("input is nullptr @ImageFilter3D::SetOutputImage(...)")
		return;
	}
	m_OutputImage.Share(OutputImage);
	m_Flag_OutputImage = true;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetOutputArray(ObjectArray<OutputVoxelType>* OutputArray)
{
	if (OutputArray == nullptr)
	{
		MDK_Error("input is nullptr @ImageFilter3D::SetOutputImage(...)")
		return;
	}
	m_OutputArray.Share(OutputArray);
	m_Flag_OutputArray = true;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::SetMaxNumberOfThreads(int_max MaxNumber)
{
	m_MaxNumberOfThreads = MaxNumber;
}


template<typename InputVoxelType, typename OutputVoxelType>
int_max ImageFilter3D<InputVoxelType, OutputVoxelType>::GetNumberOfThreadsTobeCreated()
{
    return Compute_NumberOfThreadsTobeCreated_For_ParallelBlock(m_TotalOutputVoxelNumber, m_MaxNumberOfThreads, m_MinVoxelNumberPerThread);
}


template<typename InputVoxelType, typename OutputVoxelType>
bool ImageFilter3D<InputVoxelType, OutputVoxelType>::Preprocess()
{
	return true;
}


template<typename InputVoxelType, typename OutputVoxelType>
bool ImageFilter3D<InputVoxelType, OutputVoxelType>::Postprocess()
{
    return true;
}


template<typename InputVoxelType, typename OutputVoxelType>
bool ImageFilter3D<InputVoxelType, OutputVoxelType>::CheckInput()
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
    m_TotalOutputVoxelNumber = 0;

    if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        auto InputSize = m_InputImage->GetSize();

        m_TotalOutputVoxelNumber = InputSize.Lx*InputSize.Ly*InputSize.Lz;
    }
    else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputVoxelNumber = m_InputRegionOf3DIndex->Lx()*m_InputRegionOf3DIndex->Ly()*m_InputRegionOf3DIndex->Lz();             
    }
    else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputVoxelNumber = m_Input3DIndexList->GetColNumber();
    }
    else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
	{
		m_TotalOutputVoxelNumber = m_Input3DPositionList->GetColNumber();
	}    
	else
	{
		MDK_Error("Invalid input @ ImageFilter3D::CheckInput()")
		return false;
	}

	if (m_InputRegionOf3DIndex != nullptr)
	{
		m_VoxelLinearIndexList_Of_InputRegionOf3DIndex.FastResize(0);
		m_VoxelLinearIndexList_Of_InputRegionOf3DIndex.ReserveCapacity(m_TotalOutputVoxelNumber);
		for (int_max z_Index = m_InputRegionOf3DIndex->z0; z_Index <= m_InputRegionOf3DIndex->z1; ++z_Index)
		{
			for (int_max y_Index = m_InputRegionOf3DIndex->y0; y_Index <= m_InputRegionOf3DIndex->y1; ++y_Index)
			{
				for (int_max x_Index = m_InputRegionOf3DIndex->x0; x_Index <= m_InputRegionOf3DIndex->x1; ++x_Index)
				{
					auto tempLinearIndex = m_InputImage->Transform3DIndexToLinearIndex(x_Index, y_Index, z_Index);
					m_VoxelLinearIndexList_Of_InputRegionOf3DIndex.Append(tempLinearIndex);
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

            if (OutputSize.Lx != InputSize.Lx || OutputSize.Ly != InputSize.Ly || OutputSize.Lz != InputSize.Lz)
            {
                m_OutputImage.SetSize(m_InputImage->GetSize());
                m_OutputImage.SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
            }
        }
        else
        {
            auto OutputSize = m_OutputImage.GetSize();

            if (m_InputRegionOf3DIndex->Lx() != OutputSize.Lx || m_InputRegionOf3DIndex->Ly() != OutputSize.Ly || m_InputRegionOf3DIndex->Lz() != OutputSize.Lz)
            {
                OutputSize.Lx = m_InputRegionOf3DIndex->Lx();
                OutputSize.Ly = m_InputRegionOf3DIndex->Ly();
                OutputSize.Lz = m_InputRegionOf3DIndex->Lz();

                m_OutputImage.SetSize(OutputSize);
                m_OutputImage.SetSpacing(m_InputImage->GetSpacing());
                m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
            }
        }        
    }

    if (m_Flag_OutputArray == true)
    {
        if (m_OutputArray.GetElementNumber() != m_TotalOutputVoxelNumber)
        {
            m_OutputArray.Clear();
            m_OutputArray.Resize(m_TotalOutputVoxelNumber);
        }
    }

    //------------------------------

	return true;
}


template<typename InputVoxelType, typename OutputVoxelType>
bool ImageFilter3D<InputVoxelType, OutputVoxelType>::Update()
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
                  0, m_TotalOutputVoxelNumber - 1, m_MaxNumberOfThreads, m_MinVoxelNumberPerThread);

    //---------------------------------------------------------------------------
    if (this->Postprocess() == false)
    {
        return false;
    }

    return true;
}


template<typename InputVoxelType, typename OutputVoxelType>
void ImageFilter3D<InputVoxelType, OutputVoxelType>::Update_in_a_Thread(int_max OutputVoxelIndex_start, int_max OutputVoxelIndex_end, int_max ThreadIndex)
{
    std::cout << "ImageFilter3D ThreadIndex: " << ThreadIndex
		      << ", OutputVoxelIndex_start: " << OutputVoxelIndex_start 
              << ", OutputVoxelIndex_end: " << OutputVoxelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_InputImage->TransformLinearIndexTo3DIndex(VoxelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                this->FilterFunctionAt3DIndex(m_OutputImage(VoxelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
                for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
					double FilterOrigin3DIndex[3];
					m_InputImage->TransformLinearIndexTo3DIndex(m_VoxelLinearIndexList_Of_InputRegionOf3DIndex[VoxelIndex], 
													            FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                    this->FilterFunctionAt3DIndex(m_OutputImage(VoxelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
                }
            }
            else
            {
                for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
					double FilterOrigin3DIndex[3];
					m_InputImage->TransformLinearIndexTo3DIndex(m_VoxelLinearIndexList_Of_InputRegionOf3DIndex[VoxelIndex],
																FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                    this->FilterFunctionAt3DIndex(m_OutputArray[VoxelIndex], FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
                }
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
					double FilterOrigin3DIndex[3];
                    FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, VoxelIndex);
                    FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, VoxelIndex);
                    FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, VoxelIndex);

                    this->FilterFunctionAt3DIndex(m_OutputImage(VoxelIndex), FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
				}
            }
            else // if (m_Flag_OutputArray == true)
            {
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
					double FilterOrigin3DIndex[3];
                    FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, VoxelIndex);
                    FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, VoxelIndex);
                    FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, VoxelIndex);

                    this->FilterFunctionAt3DIndex(m_OutputArray[VoxelIndex], FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);
				}
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{				
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{					
					double FilterOrigin3DPosition[3];
					FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
					FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
					FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                    this->FilterFunctionAt3DPosition(m_OutputImage(VoxelIndex), FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);
				}
			}
			else
			{				
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{					
					double FilterOrigin3DPosition[3];
					FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
					FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
					FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                    this->FilterFunctionAt3DPosition(m_OutputArray[VoxelIndex], FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);
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
		OutputVoxelType tempOutputVoxel;

		if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_InputImage->TransformLinearIndexTo3DIndex(VoxelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);

                this->FilterFunctionAt3DIndex(tempOutputVoxel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(VoxelIndex, tempOutputVoxel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex != nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {	
            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
				double FilterOrigin3DIndex[3];
                m_OutputImage.TransformLinearIndexTo3DIndex(VoxelIndex, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2]);
				FilterOrigin3DIndex[0] += double(m_InputRegionOf3DIndex->x0);
				FilterOrigin3DIndex[1] += double(m_InputRegionOf3DIndex->y0);
				FilterOrigin3DIndex[2] += double(m_InputRegionOf3DIndex->z0);

                this->FilterFunctionAt3DIndex(tempOutputVoxel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(VoxelIndex, tempOutputVoxel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList != nullptr && m_Input3DPositionList == nullptr)
        {			
            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
				double FilterOrigin3DIndex[3];
                FilterOrigin3DIndex[0] = (*m_Input3DIndexList)(0, VoxelIndex);
                FilterOrigin3DIndex[1] = (*m_Input3DIndexList)(1, VoxelIndex);
                FilterOrigin3DIndex[2] = (*m_Input3DIndexList)(2, VoxelIndex);

                this->FilterFunctionAt3DIndex(tempOutputVoxel, FilterOrigin3DIndex[0], FilterOrigin3DIndex[1], FilterOrigin3DIndex[2], ThreadIndex);

                this->OutputFunction(VoxelIndex, tempOutputVoxel, ThreadIndex);
            }
        }
		else if (m_InputRegionOf3DIndex == nullptr && m_Input3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{			
			for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
			{				
				double FilterOrigin3DPosition[3];
				FilterOrigin3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
				FilterOrigin3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
				FilterOrigin3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                this->FilterFunctionAt3DPosition(tempOutputVoxel, FilterOrigin3DPosition[0], FilterOrigin3DPosition[1], FilterOrigin3DPosition[2], ThreadIndex);

                this->OutputFunction(VoxelIndex, tempOutputVoxel, ThreadIndex);
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter3D::Update_in_a_Thread")
        }
    }
}


template<typename InputVoxelType, typename OutputVoxelType>
inline
void ImageFilter3D<InputVoxelType, OutputVoxelType>::OutputFunction(int_max OutputVoxelIndex, OutputVoxelType& OutputVoxel, int_max ThreadIndex)
{
    if (m_Flag_OutputImage == true)
    {
        m_OutputImage(OutputVoxelIndex) = OutputVoxel;
    }
    else if (m_Flag_OutputArray == true)
    {
        m_OutputArray[OutputVoxelIndex] = OutputVoxel;
    }
    else
    {
        MDK_Warning("OutputFunction is empty @ ImageFilter3D::OutputFunction")
    }
}


template<typename InputVoxelType, typename OutputVoxelType>
Image3D<OutputVoxelType>& ImageFilter3D<InputVoxelType, OutputVoxelType>::OutputImage()
{
    return m_OutputImage;
}


template<typename InputVoxelType, typename OutputVoxelType>
ObjectArray<OutputVoxelType>& ImageFilter3D<InputVoxelType, OutputVoxelType>::OutputArray()
{
    return m_OutputArray;
}



}//end namespace mdk

#endif