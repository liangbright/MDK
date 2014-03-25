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

    m_OutputImage_SharedCopy.Clear();

    m_OutputImage = &m_OutputImage_SharedCopy;

    m_OutputArray_SharedCopy.Clear();

    m_OutputArray = &m_OutputArray_SharedCopy;

    m_Flag_OutputImage = true; // default to output image with the same size 

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_IsBoundCheckEnabled = true;

    m_IsInputFilterFunctionAt3DPositionObtained = false;

    m_IsInputFilterFunctionAt3DIndexObtained = false;

    m_ZeroVoxelOfInputImage = m_ZeroVoxelOfInputImage - m_ZeroVoxelOfInputImage;

    m_ZeroVoxelOfOutputImage = m_ZeroVoxelOfOutputImage - m_ZeroVoxelOfOutputImage;

    m_TotalOutputVoxelNumber = 0;

    m_MinVoxelNumberPerThread = 100;

    m_MaxThreadNumber = 1;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
EnableBoundCheck()
{
	m_IsBoundCheckEnabled = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
DisableBoundCheck()
{
    m_IsBoundCheckEnabled = false;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputImage(const Image<VoxelType_Input>* InputImage)
{
    if (InputImage == nullptr)
    {
        MDK_Error << "Input is nullptr @ ImageFilter::SetInputImage" << '\n';
        return false;
    }

	m_InputImage = InputImage;

    m_ZeroVoxelOfInputImage = InputImage->GetZeroVoxel();

    m_ZeroVoxelOfInputImage -= m_ZeroVoxelOfInputImage;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputRegion(const DenseMatrix<int64>* InputRegion)
{
//    if (InputRegion == nullptr)
//    {
//        MDK_Error << "Input is nullptr @ ImageFilter::SetInputRegion" << '\n';
//        return false;
//    }

    m_InputRegion = InputRegion;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputVoxel3DIndexList(const DenseMatrix<int64>* InputVoxel3DIndexList)
{
//    if (InputVoxel3DIndexList == nullptr)
//    {
//        MDK_Error << "Input is nullptr @ ImageFilter::SetInputVoxel3DIndexList" << '\n';
//        return false;
//    }

    m_InputVoxel3DIndexList = InputVoxel3DIndexList;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList)
{
//    if (Input3DPositionList == nullptr)
//    {
//        MDK_Error << "Input is nullptr @ ImageFilter::SetInput3DPositionList" << '\n';
//        return false;
//    }

	m_Input3DPositionList = Input3DPositionList;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DIndex(std::function<void(int64, int64, int64, const Image<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DIndex = InputFilterFunction;

    m_IsInputFilterFunctionAt3DIndexObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const Image<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DPosition = InputFilterFunction;

    m_IsInputFilterFunctionAt3DPositionObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputImage(Image<VoxelType_Output>* OutputImage)
{
    if (OutputImage == nullptr)
    {
        MDK_Error << "Input is nullptr @ ImageFilter::SetOutputImage" << '\n';
        return false;
    }

	m_OutputImage = OutputImage;

    m_OutputImage_SharedCopy.Share(OutputImage);

    m_ZeroVoxelOfOutputImage = OutputImage->GetZeroVoxel();

    m_ZeroVoxelOfOutputImage -= m_ZeroVoxelOfOutputImage;

    m_Flag_OutputImage = true;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputArray(DenseMatrix<VoxelType_Output>* OutputArray)
{
    if (OutputArray == nullptr)
    {
        MDK_Error << "Input is nullptr @ ImageFilter::OutputArray" << '\n';
        return false;
    }

	m_OutputArray = OutputArray;

    m_OutputArray_SharedCopy.Share(OutputArray);

    m_Flag_OutputArray = true;

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
ImageFilter<VoxelType_Input, VoxelType_Output>::
SetMaxThreadNumber(int64 MaxNumber)
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
		MDK_Error << "Empty input image (nullptr) @ ImageFilter::CheckInput" << '\n';
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		MDK_Error << "Empty input image @ ImageFilter::CheckInput" << '\n';
		return false;
	}

	//------------------------------------------------------------------------------------------------------------

    if (m_Flag_OutputToOtherPlace == false)
    {
        if (m_Flag_OutputImage == true && m_Flag_OutputArray == true)
        {
            MDK_Error << "Can not output image and array at the same time @ ImageFilter::CheckInput" << '\n';
            return false;
        }

        if (m_Flag_OutputImage == false && m_Flag_OutputArray == false)
        {
            MDK_Error << "No output is selected @ ImageFilter::CheckInput" << '\n';
            return false;
        }
    }
    else
    {
        if (m_Flag_OutputImage == true || m_Flag_OutputArray == true)
        {
            MDK_Error << "Can not output image or array when m_Flag_OutputToOtherPlace is true  @ ImageFilter::CheckInput" << '\n';
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
		MDK_Error << "Invalid input @ ImageFilter::Run" << '\n';
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
    auto IsOK = this->CheckInput();

    if (IsOK == false)
    {
        return false;
    }

    //-------------------------------------------------------------------------------

    auto IsGood = this->Preprocess();

	if (IsGood == false)
	{
		return false;
	}

	// multi-thread -----------------------------------------------------------------
   
    // divide the output image into groups

	std::vector<int64> IndexList_start;
	std::vector<int64> IndexList_end;

    this->DivideData(0, m_TotalOutputVoxelNumber - 1, m_MinVoxelNumberPerThread, IndexList_start, IndexList_end);

	int64 ThreadNumber = IndexList_start.size();
	
    if (ThreadNumber > 1)
    {
		// create and start the threads
		std::vector<std::thread> ThreadList(ThreadNumber);

        for (int64 i = 0; i < ThreadNumber; ++i)
		{
            ThreadList[i] = std::thread([&]{this->Update_in_a_Thread(IndexList_start[i], IndexList_end[i]); });
		}

		//wait for all the threads
        for (int64 i = 0; i < ThreadNumber; ++i)
		{
            ThreadList[i].join();
		}
	}
	else//single-thread
	{
        this->Update_in_a_Thread(0, m_TotalOutputVoxelNumber - 1);
	}

    //---------------------------------------------------------------------------

    if (m_OutputImage != &m_OutputImage_SharedCopy)
    {
        m_OutputImage_SharedCopy.Share(m_OutputImage);
    }

    if (m_OutputArray != &m_OutputArray_SharedCopy)
    {
        m_OutputArray_SharedCopy.Share(m_OutputArray);
    }

    //---------------------------------------------------------------------------

    return this->Postprocess();
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
Update_in_a_Thread(int64 OutputVoxelIndex_start, int64 OutputVoxelIndex_end)
{
    std::cout << "VoxelIndex_start " << OutputVoxelIndex_start << '\n';
    std::cout << "VoxelIndex_end   " << OutputVoxelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int64 FilterCenter3DIndex[3];  // [xc, yc, zc]

            for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxel3DIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			int64 RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0_Index;
            RegionOrigin[1] = m_InputRegion->y0_Index;
            RegionOrigin[2] = m_InputRegion->z0_Index;

            if (m_Flag_OutputImage == true)
            {
				int64 FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
				int64 FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
			int64 FilterCenter3DIndex[3];  // [xc, yc, zc]

            if (m_Flag_OutputImage == true)
            {
				for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
                    FilterCenter3DIndex[0] = (*m_InputVoxel3DIndexList)(0, VoxelIndex);
                    FilterCenter3DIndex[1] = (*m_InputVoxel3DIndexList)(1, VoxelIndex);
                    FilterCenter3DIndex[2] = (*m_InputVoxel3DIndexList)(2, VoxelIndex);

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
				}
            }
            else
            {
				for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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

				for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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

				for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
            MDK_Error << "Invalid Input @ ImageFilter::Update_in_a_Thread" << '\n';
        }
    }
    else // output to another place
    {
        auto tempOutputVoxel = m_ZeroVoxelOfOutputImage;

		if (m_InputRegion == nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int64 FilterCenter3DIndex[3];

            for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxel3DIndexList == nullptr && m_Input3DPositionList == nullptr)
        {
			int64 FilterCenter3DIndex[3];

			int64 RegionOrigin[3]; // [x0, y0, z0]
            RegionOrigin[0] = m_InputRegion->x0_Index;
            RegionOrigin[1] = m_InputRegion->y0_Index;
            RegionOrigin[2] = m_InputRegion->z0_Index;

            for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
			int64 FilterCenter3DIndex[3];

            for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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

			for (int64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
            MDK_Error << "Invalid Input @ ImageFilter::Update_in_a_Thread" << '\n';
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
DivideData(int64 Index_min, int64 Index_max, int64 MinDataNumberPerThread,
           std::vector<int64>& IndexList_start, std::vector<int64>& IndexList_end)
{
    if (m_MaxThreadNumber == 1)
    {
        IndexList_start.push_back(Index_min);
        IndexList_end.push_back(Index_max);
        return;
    }

	auto TotalDataNumber = Index_max - Index_min + 1;

	int64 ThreadNumber = 1;

	int64 DataNumberPerThread = 0;

	for (int64 i = m_MaxThreadNumber; i > 0; --i)
	{
        DataNumberPerThread = TotalDataNumber / i;

        if (DataNumberPerThread >= MinDataNumberPerThread)
		{
			ThreadNumber = i;
			break;
		}
	}

	if (ThreadNumber == 1)
	{//one thread is enough

		IndexList_start.push_back(Index_min);
		IndexList_end.push_back(Index_max);
		return;
	}

    int64 tempIndex = Index_min;

    for (int64 i = 0; i < ThreadNumber; ++i)
	{
        IndexList_start.push_back(tempIndex);
        IndexList_end.push_back(tempIndex + DataNumberPerThread - 1);

        tempIndex += DataNumberPerThread;
	}

    IndexList_end[ThreadNumber - 1] = Index_max;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel)
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
        MDK_Warning << "m_IsInputFilterFunctionAt3DIndexObtained = false @ ImageFilter::FilterFunctionAt3DIndex" << '\n';
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
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
        MDK_Warning << "m_IsInputFilterFunctionAt3DPositionObtained = false @ ImageFilter::FilterFunctionAt3DPosition" << '\n';
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
ImageFilter<VoxelType_Input, VoxelType_Output>::
OutputFunction(int64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
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
        MDK_Warning << "OutputFunction is empty @ ImageFilter::OutputFunction" << '\n';
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
Image<VoxelType_Output>* 
ImageFilter<VoxelType_Input, VoxelType_Output>::GetOutputImage()
{
    return &m_OutputImage_SharedCopy;
}


template<typename VoxelType_Input, typename VoxelType_Output>
DenseMatrix<VoxelType_Output>*
ImageFilter<VoxelType_Input, VoxelType_Output>::GetOutputArray()
{
    return &m_OutputArray_SharedCopy;
}



}//end namespace mdk

#endif