#ifndef __mdk3DImageFilter_hpp
#define __mdk3DImageFilter_hpp

#include <thread>

#include "mdk3DImageFilter.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter()
{
    this->Clear();
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageFilter()
{
	// do nothing
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Clear()
{
    m_InputImage = nullptr;

    m_InputRegion = nullptr;

    m_InputVoxelLinearIndexList = nullptr;

	m_Input3DPositionList = nullptr;

    m_OutputImage = nullptr;

    m_OutputArray = nullptr;

    m_MaxThreadNumber = 1;

    m_IsBoundCheckEnabled = true;

    m_IsInputFilterFunctionAt3DPositionObtained = false;

    m_IsInputFilterFunctionAt3DIndexObtained = false;

    m_ZeroVoxelOfInputImage -= m_ZeroVoxelOfInputImage;

    m_ZeroVoxelOfOutputImage -= m_ZeroVoxelOfOutputImage;

    m_Flag_OutputImage = false;

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputVoxelNumber = 0;

    m_MinVoxelNumberPerThread = 100;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
EnableBoundCheck(bool On_Off)
{
	m_IsBoundCheckEnabled = On_Off;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputImage(const mdk3DImage<VoxelType_Input>* InputImage)
{
	m_InputImage = InputImage;

    m_ZeroVoxelOfInputImage = InputImage->GetEmptyVoxel();

    m_ZeroVoxelOfInputImage -= m_ZeroVoxelOfInputImage;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputRegion(const mdkMatrix<uint64>* InputRegion)
{
    m_InputRegion = InputRegion;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputVoxelLinearIndexList(const mdkMatrix<uint64>* InputVoxelLinearIndexList)
{
	m_InputVoxelLinearIndexList = InputVoxelLinearIndexList;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInput3DPositionList(const mdkMatrix<float>* Input3DPositionList)
{
	m_Input3DPositionList = Input3DPositionList;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DIndex(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DIndex = InputFilterFunction;

    m_IsInputFilterFunctionAt3DIndexObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction_At3DPosition = InputFilterFunction;

    m_IsInputFilterFunctionAt3DPositionObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage)
{
	m_OutputImage = OutputImage;

    m_ZeroVoxelOfOutputImage = OutputImage->GetEmptyVoxel();

    m_ZeroVoxelOfOutputImage -= m_ZeroVoxelOfOutputImage;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputArray(mdkMatrix<VoxelType_Output>* OutputArray)
{
	m_OutputArray = OutputArray;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetMaxThreadNumber(uint64 MaxNumber)
{
	m_MaxThreadNumber = MaxNumber;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Preprocess()
{
	return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Postprocess()
{
    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::CheckInputData()
{
	if (m_InputImage == nullptr)
	{
		mdkError << "Invalid input @ mdk3DImageFilter::CheckInputData" << '\n';
		return false;
	}

	if (m_InputImage->IsEmpty() == true)
	{
		mdkError << "Empty input image @ mdk3DImageFilter::CheckInputData" << '\n';
		return false;
	}

	//---------------------------------
    m_Flag_OutputToOtherPlace = false;

    if (m_OutputImage != nullptr && m_OutputArray == nullptr)
    {
        m_Flag_OutputImage = true;
        m_Flag_OutputArray = false;
    }
    else if (m_OutputImage == nullptr && m_OutputArray != nullptr)
    {
        m_Flag_OutputImage = false;
        m_Flag_OutputArray = true;
    }
    else if (m_OutputImage != nullptr && m_OutputArray != nullptr)
    {
        mdkError << "Can not output image and array at the same time @ mdk3DImageFilter::CheckInputData" << '\n';
        return false;
    }
    else
    {
        m_Flag_OutputToOtherPlace = true;
    }
        
    //---------------------------------
    m_TotalOutputVoxelNumber = 0;

	if (m_InputRegion != nullptr && m_InputVoxelLinearIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        if (m_OutputImage != nullptr)
        {
            auto Dimension = m_OutputImage->GetImageDimension();

            if ((*m_InputRegion)(0, 1) != Dimension.Lx || (*m_InputRegion)(1, 1) != Dimension.Ly || (*m_InputRegion)(2, 1) != Dimension.Lz)
            {
                mdkError << "Invalid input: m_InputRegion does not match m_OutputImage @ mdk3DImageFilter::Run" << '\n';
                return false;
            }            
        }

        m_TotalOutputVoxelNumber = (*m_InputRegion)(0, 1)*(*m_InputRegion)(0, 2)*(*m_InputRegion)(0, 3);
    }
	else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList != nullptr && m_Input3DPositionList == nullptr)
    {
        m_TotalOutputVoxelNumber = m_InputVoxelLinearIndexList->GetElementNumber();
    }
	else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr && m_Input3DPositionList != nullptr)
	{
		m_TotalOutputVoxelNumber = m_Input3DPositionList->GetColNumber();
	}
	else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr && m_Input3DPositionList == nullptr)
    {
        auto Dimension = m_InputImage->GetImageDimension();

        m_TotalOutputVoxelNumber = Dimension.Lx*Dimension.Ly*Dimension.Lz;
    }
	else
	{
		mdkError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
		return false;
	}

    //------------------------------

	return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run()
{
    //-------------------------------------------------------------------------------
    auto IsOK = this->CheckInputData();

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
    if (m_MaxThreadNumber > 1 && m_TotalOutputVoxelNumber > m_MinVoxelNumberPerThread)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_start;
		std::vector<uint64> IndexList_end;

        this->DivideData(0, m_TotalOutputVoxelNumber - 1, IndexList_start, IndexList_end);

		uint64 ThreadNumber = IndexList_start.size();
		
		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

        for (uint64 i = 0; i < ThreadNumber; ++i)
		{
			FilterThread[i] = std::thread([&]{this->Run_in_a_Thread(IndexList_start[i], IndexList_end[i]); });
		}

		//wait for all the threads
        for (uint64 i = 0; i < ThreadNumber; ++i)
		{
			FilterThread[i].join();
		}
	}
	else//single-thread
	{
        this->Run_in_a_Thread(0, m_TotalOutputVoxelNumber - 1);
	}

    //-------------------------------------------------------------------------------

    return this->Postprocess();
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 OutputVoxelIndex_start, uint64 OutputVoxelIndex_end)
{
    std::cout << "VoxelIndex_start " << OutputVoxelIndex_start << '\n';
    std::cout << "VoxelIndex_end   " << OutputVoxelIndex_end << '\n';
	
    if (m_Flag_OutputToOtherPlace == false)
    {
		if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			uint64 FilterCenter3DIndex[3];  // [xc, yc, zc]

            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxelLinearIndexList == nullptr  && m_Input3DPositionList == nullptr)
        {
			uint64 RegionOrigin[3]; // [x0, y0, z0]
			RegionOrigin[0] = (*m_InputRegion)(0, 0);
			RegionOrigin[1] = (*m_InputRegion)(1, 0);
			RegionOrigin[2] = (*m_InputRegion)(2, 0);

            if (m_Flag_OutputImage == true)
            {
				uint64 FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
				uint64 FilterCenter3DIndex[3];  // [xc, yc, zc]

                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                    FilterCenter3DIndex[0] += RegionOrigin[0];
                    FilterCenter3DIndex[1] += RegionOrigin[1];
                    FilterCenter3DIndex[2] += RegionOrigin[2];

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputArray)[VoxelIndex]);
                }
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList != nullptr  && m_Input3DPositionList == nullptr)
        {
			uint64 FilterCenter3DIndex[3];  // [xc, yc, zc]

            if (m_Flag_OutputImage == true)
            {
				for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
					m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelLinearIndexList)[VoxelIndex], &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputImage)(VoxelIndex));
				}
            }
            else
            {
				for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
				{
					m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelLinearIndexList)[VoxelIndex], &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                    this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], (*m_OutputArray)[VoxelIndex]);
				}
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr  && m_Input3DPositionList != nullptr)
		{
			if (m_Flag_OutputImage == true)
			{
				double FilterCenter3DPosition[3];

				for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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

				for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
            mdkError << "Invalid Input @ mdk3DImageFilter::Run_in_a_Thread" << '\n';
        }
    }
    else
    {
        auto tempOutputVoxel = m_ZeroVoxelOfOutputImage;

		if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr && m_Input3DPositionList != nullptr)
        {
			uint64 FilterCenter3DIndex[3];

            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion != nullptr && m_InputVoxelLinearIndexList == nullptr && m_Input3DPositionList != nullptr)
        {
			uint64 FilterCenter3DIndex[3];

			uint64 RegionOrigin[3]; // [x0, y0, z0]
			RegionOrigin[0] = (*m_InputRegion)(0, 0);
			RegionOrigin[1] = (*m_InputRegion)(1, 0);
			RegionOrigin[2] = (*m_InputRegion)(2, 0);

            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                FilterCenter3DIndex[0] += RegionOrigin[0];
                FilterCenter3DIndex[1] += RegionOrigin[1];
                FilterCenter3DIndex[2] += RegionOrigin[2];

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList != nullptr && m_Input3DPositionList != nullptr)
        {
			uint64 FilterCenter3DIndex[3];

            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelLinearIndexList)[VoxelIndex], &FilterCenter3DIndex[0], &FilterCenter3DIndex[1], &FilterCenter3DIndex[2]);

                this->FilterFunctionAt3DIndex(FilterCenter3DIndex[0], FilterCenter3DIndex[1], FilterCenter3DIndex[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
		else if (m_InputRegion == nullptr && m_InputVoxelLinearIndexList == nullptr  && m_Input3DPositionList != nullptr)
		{
			double FilterCenter3DPosition[3];

			for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
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
            mdkError << "Invalid Input @ mdk3DImageFilter::Run_in_a_Thread" << '\n';
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
DivideData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end)
{
	auto TotalVoxelNumber = Index_max - Index_min + 1;

	uint64 ThreadNumber = 1;

	uint64 VoxelNumberPerThread = 0;

	for (uint64 i = m_MaxThreadNumber; i > 0; --i)
	{
		VoxelNumberPerThread = TotalVoxelNumber / i;

        if (VoxelNumberPerThread >= m_MinVoxelNumberPerThread)
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

    uint64 tempNumber = Index_min;

	for (uint64 i = 0; i < m_MaxThreadNumber; ++i)
	{
		IndexList_start.push_back(tempNumber);
		IndexList_end.push_back(tempNumber + VoxelNumberPerThread - 1);

		tempNumber += VoxelNumberPerThread;
	}

	IndexList_end[m_MaxThreadNumber - 1] = Index_max;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunctionAt3DIndex(uint64 x_Index, uint64 y_Index, uint64 z_Index, VoxelType_Output& OutputVoxel)
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
        mdkWarning << "m_IsInputFilterFunctionAt3DIndexObtained = false @ mdk3DImageFilter::FilterFunctionAt3DIndex" << '\n';
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
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
        mdkWarning << "m_IsInputFilterFunctionAt3DPositionObtained = false @ mdk3DImageFilter::FilterFunctionAt3DPosition" << '\n';
    }

}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
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
        mdkWarning << "OutputFunction is empty @ mdk3DImageFilter::OutputFunction" << '\n';
    }
}

}//end namespace mdk

#endif