#ifndef __mdkImageFilter_hpp
#define __mdkImageFilter_hpp

//#include "ImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
ImageFilter<VoxelType_Input, VoxelType_Output>::ImageFilter()
{
    this->Clear();
}


template<typename VoxelType_Input, typename VoxelType_Output>
ImageFilter<VoxelType_Input, VoxelType_Output>::~ImageFilter()
{
	// do nothing
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegion = nullptr;

    m_InputVoxel3DIndexList = nullptr;

	m_Input3DPositionList = nullptr;

    this->SetupDefaultPipelineOutput();

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_IsBoundCheckEnabled = true;

    m_IsInputFilterFunctionAt3DPositionObtained = false;

    m_IsInputFilterFunctionAt3DIndexObtained = false;

    m_ZeroVoxelOfInputImage = m_ZeroVoxelOfInputImage - m_ZeroVoxelOfInputImage;

    m_ZeroVoxelOfOutputImage = m_ZeroVoxelOfOutputImage - m_ZeroVoxelOfOutputImage;

    m_TotalOutputVoxelNumber = 0;

    m_MinVoxelNumberPerThread = 1;

    m_MaxThreadNumber = 1;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetupDefaultPipelineOutput()
{
    m_OutputImage_SharedCopy.Clear();

    m_OutputImage = &m_OutputImage_SharedCopy;

    m_OutputArray_SharedCopy.Clear();

    m_OutputArray = &m_OutputArray_SharedCopy;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::UpdatePipelineOutput()
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


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::EnableBoundCheck()
{
	m_IsBoundCheckEnabled = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::DisableBoundCheck()
{
    m_IsBoundCheckEnabled = false;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>:: SetInputImage(const Image<VoxelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter::SetInputImage")
        return;
    }

	m_InputImage = InputImage;

    m_ZeroVoxelOfInputImage = InputImage->GetZeroVoxel();

    m_ZeroVoxelOfInputImage -= m_ZeroVoxelOfInputImage;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetInputRegion(const DenseMatrix<int_max>* InputRegion)
{
    m_InputRegion = InputRegion;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetInputVoxel3DIndexList(const DenseMatrix<int_max>* InputVoxel3DIndexList)
{
    m_InputVoxel3DIndexList = InputVoxel3DIndexList;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList)
{
	m_Input3DPositionList = Input3DPositionList;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DIndex(std::function<void(int_max, int_max, int_max, const Image<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DIndex = InputFilterFunction;

    m_IsInputFilterFunctionAt3DIndexObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const Image<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DPosition = InputFilterFunction;

    m_IsInputFilterFunctionAt3DPositionObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputImage(Image<VoxelType_Output>* OutputImage)
{
    if (OutputImage == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter::SetOutputImage")
        return;
    }

	m_OutputImage = OutputImage;

    m_ZeroVoxelOfOutputImage = OutputImage->GetZeroVoxel();

    m_Flag_OutputImage = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputArray(DenseMatrix<VoxelType_Output>* OutputArray)
{
    if (OutputArray == nullptr)
    {
        MDK_Error("Input is nullptr @ ImageFilter::OutputArray")
        return;
    }

	m_OutputArray = OutputArray;

    m_Flag_OutputArray = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::SetMaxThreadNumber(int_max MaxNumber)
{
	m_MaxThreadNumber = MaxNumber;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool ImageFilter<VoxelType_Input, VoxelType_Output>::Preprocess()
{
	return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool ImageFilter<VoxelType_Input, VoxelType_Output>::Postprocess()
{
    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool ImageFilter<VoxelType_Input, VoxelType_Output>::CheckInput()
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("Input image is Empty (nullptr) @ ImageFilter::CheckInput()")
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error("Input image is Empty @ ImageFilter::CheckInput()")
		return false;
	}

	//------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputToOtherPlace == false)
    {
        if (m_Flag_OutputImage == true && m_Flag_OutputArray == true)
        {
            MDK_Error("Can not output image and array at the same time @ ImageFilter::CheckInput()")
            return false;
        }

        if (m_Flag_OutputImage == false && m_Flag_OutputArray == false)
        {
            MDK_Error("NO output is selected @ ImageFilter::CheckInput()")
            return false;
        }
    }
    else
    {
        if (m_Flag_OutputImage == true || m_Flag_OutputArray == true)
        {
            MDK_Error("Can not output image or array when m_Flag_OutputToOtherPlace is true  @ ImageFilter::CheckInput()")
            return false;
        }
    }
        
    //-------------------------------------------------------------------------------------------------------------
    m_TotalOutputVoxelNumber = 0;

    if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        auto InputDimension = m_InputImage->GetDimension();

        m_TotalOutputVoxelNumber = InputDimension.Lx*InputDimension.Ly*InputDimension.Lz;
    }
    else if (m_InputRegion != nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputVoxelNumber = m_InputRegion->Lx()*m_InputRegion->Ly()*m_InputRegion->Lz();             
    }
    else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList != nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputVoxelNumber = m_InputVoxel3DIndexList->GetColNumber();
    }
    else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList != nullptr)
	{
		m_TotalOutputVoxelNumber = m_Input3DPositionList->GetColNumber();
	}    
	else
	{
		MDK_Error("Invalid input @ ImageFilter::CheckInput()")
		return false;
	}

    //------------------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputImage == true)
    {
        if (m_InputRegion == nullptr)
        {
            auto InputDimension = m_InputImage->GetDimension();

            auto OutputDimension = m_OutputImage->GetDimension();

            if (OutputDimension.Lx != InputDimension.Lx || OutputDimension.Ly != InputDimension.Ly || OutputDimension.Lz != InputDimension.Lz)
            {
                m_OutputImage->ReInitialize(m_InputImage->GetDimension(), m_InputImage->GetPhysicalOrigin(), m_InputImage->GetVoxelSpacing());
            }
        }
        else
        {
            auto OutputDimension = m_OutputImage->GetDimension();

            if (m_InputRegion->Lx() != OutputDimension.Lx || m_InputRegion->Ly() != OutputDimension.Ly || m_InputRegion->Lz() != OutputDimension.Lz)
            {
                OutputDimension.Lx = m_InputRegion->Lx();
                OutputDimension.Ly = m_InputRegion->Ly();
                OutputDimension.Lz = m_InputRegion->Lz();

                m_OutputImage->ReInitialize(OutputDimension, m_InputImage->GetPhysicalOrigin(), m_InputImage->GetVoxelSpacing());
            }
        }        
    }

    if (m_Flag_OutputArray == true)
    {
        if (m_OutputArray->GetElementNumber() != m_TotalOutputVoxelNumber)
        {
            m_OutputArray->Clear();
            m_OutputArray->Resize(1, m_TotalOutputVoxelNumber);
        }
    }

    //------------------------------

	return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool ImageFilter<VoxelType_Input, VoxelType_Output>::Update()
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
   
    ParallelBlock([&](int_max Index_start, int_max Index_end){this->Run_in_a_Thread(Index_start, Index_end); },
                  0, m_TotalOutputVoxelNumber - 1, m_MaxThreadNumber, m_MinVoxelNumberPerThread);

    //---------------------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageFilter<VoxelType_Input, VoxelType_Output>::Run_in_a_Thread(int_max OutputVoxelIndex_start, int_max OutputVoxelIndex_end)
{
    std::cout << "VoxelIndex_start " << OutputVoxelIndex_start << '\n';
    std::cout << "VoxelIndex_end   " << OutputVoxelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0_Index;
            RegionOrigin[1] = m_InputRegion->y0_Index;
            RegionOrigin[2] = m_InputRegion->z0_Index;

            if (m_Flag_OutputImage == true)
            {
				int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                    FilterCenter3DIndex[0] += RegionOrigin[0];
                    FilterCenter3DIndex[1] += RegionOrigin[1];
                    FilterCenter3DIndex[2] += RegionOrigin[2];

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
                }
            }
            else
            {
				int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                    FilterCenter3DIndex[0] += RegionOrigin[0];
                    FilterCenter3DIndex[1] += RegionOrigin[1];
                    FilterCenter3DIndex[2] += RegionOrigin[2];

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputArray)[VoxelIndex]);
                }
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];  // [xc, yc, zc]

            if (m_Flag_OutputImage == true)
            {
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
                    FilterCenter3DIndex[0] = (*m_InputVoxel3DIndexList)(0, VoxelIndex);
                    FilterCenter3DIndex[1] = (*m_InputVoxel3DIndexList)(1, VoxelIndex);
                    FilterCenter3DIndex[2] = (*m_InputVoxel3DIndexList)(2, VoxelIndex);

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
				}
            }
            else
            {
				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
                    FilterCenter3DIndex[0] = (*m_InputVoxel3DIndexList)(0, VoxelIndex);
                    FilterCenter3DIndex[1] = (*m_InputVoxel3DIndexList)(1, VoxelIndex);
                    FilterCenter3DIndex[2] = (*m_InputVoxel3DIndexList)(2, VoxelIndex);

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputArray)[VoxelIndex]);
				}
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{
				double FilterCenter3DPosition[3];

				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
					FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
					FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                    this->FilterFunctionAt3DPosition(FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], (*m_OutputImage)(VoxelIndex));
				}
			}
			else
			{
				double FilterCenter3DPosition[3];

				for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{					
					FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
					FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
					FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                    this->FilterFunctionAt3DPosition(FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], (*m_OutputArray)[VoxelIndex]);
				}
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter::Run_in_a_Thread")
        }
    }
    else // output to another place
    {
        auto tempOutputVoxel = m_ZeroVoxelOfOutputImage;

		if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

			int_max RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0_Index;
            RegionOrigin[1] = m_InputRegion->y0_Index;
            RegionOrigin[2] = m_InputRegion->z0_Index;

            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                FilterCenter3DIndex[0] += RegionOrigin[0];
                FilterCenter3DIndex[1] += RegionOrigin[1];
                FilterCenter3DIndex[2] += RegionOrigin[2];

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList != nullptr && m_Input3DPositionList == nullptr)
        {
			int_max FilterCenter3DIndex[3];

            for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                FilterCenter3DIndex[0] = (*m_InputVoxel3DIndexList)(0, VoxelIndex);
                FilterCenter3DIndex[1] = (*m_InputVoxel3DIndexList)(1, VoxelIndex);
                FilterCenter3DIndex[2] = (*m_InputVoxel3DIndexList)(2, VoxelIndex);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr  && m_Input3DPositionList != nullptr)
		{
			double FilterCenter3DPosition[3];

			for (int_max VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
			{				
				FilterCenter3DPosition[0] = (*m_Input3DPositionList)(0, VoxelIndex);
				FilterCenter3DPosition[1] = (*m_Input3DPositionList)(1, VoxelIndex);
				FilterCenter3DPosition[2] = (*m_Input3DPositionList)(2, VoxelIndex);

                this->FilterFunctionAt3DPosition(FilterCenter3DPosition[0], FilterCenter3DPosition[1], FilterCenter3DPosition[2], tempOutputVoxel);

				this->OutputFunction(VoxelIndex, tempOutputVoxel);
			}
		}
        else
        {
            MDK_Error("Invalid Input @ ImageFilter::Run_in_a_Thread")
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void ImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel)
{
	// input (x,y,z) may be 3DIndex or 3DPosition, or 3DPosition with spacing =[1,1,1]
	/*
	VoxelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
	Output += i * (*m_InputImage)(xIndex_center, yIndex_center, zIndex_center);
	}
	*/

    if (m_IsInputFilterFunctionAt3DIndexObtained == true)
	{
		m_InputFilterFunction_At3DIndex(x_Index, y_Index, z_Index, *m_InputImage, OutputVoxel);
	}
    else
    {
        MDK_Warning("m_IsInputFilterFunctionAt3DIndexObtained == false @ ImageFilter::FilterFunctionAt3DIndex")
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void ImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel)
{
// input (x,y,z) may be 3DIndex or 3DPosition, or 3DPosition with spacing =[1,1,1]
	/*
	VoxelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
		Output += i * (*m_InputImage)(xIndex_center, yIndex_center, zIndex_center);
	}
	*/

    if (m_IsInputFilterFunctionAt3DPositionObtained == true)
	{
        m_InputFilterFunction_At3DPosition(x, y, z, *m_InputImage, OutputVoxel);
	}
    else
    {
        MDK_Warning("m_IsInputFilterFunctionAt3DPositionObtained == false @ ImageFilter::FilterFunctionAt3DPosition")
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void ImageFilter<VoxelType_Input, VoxelType_Output>::OutputFunction(int_max OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
{
    if (m_Flag_OutputImage == true)
    {
        (*m_OutputImage)(OutputVoxelIndex) = OutputVoxel;
    }
    else if (m_Flag_OutputArray == true)
    {
        (*m_OutputArray)[OutputVoxelIndex] = OutputVoxel;
    }
    else
    {
        MDK_Warning("OutputFunction is empty @ ImageFilter::OutputFunction")
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
Image<VoxelType_Output>* ImageFilter<VoxelType_Input, VoxelType_Output>::GetOutputImage()
{
    return &m_OutputImage_SharedCopy;
}


template<typename VoxelType_Input, typename VoxelType_Output>
DenseMatrix<VoxelType_Output>* ImageFilter<VoxelType_Input, VoxelType_Output>::GetOutputArray()
{
    return &m_OutputArray_SharedCopy;
}



}//end namespace mdk

#endif