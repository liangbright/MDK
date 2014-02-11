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

    m_InputVoxelSet = nullptr;

    m_OutputImage = nullptr;

    m_OutputArray = nullptr;

    m_MaxThreadNumber = 1;

    m_IsBoundCheckEnabled = true;

    m_IsInputFilterFunctionObtained = false;

    m_InputZeroVoxel -= m_InputZeroVoxel;

    m_OutputZeroVoxel -= m_OutputZeroVoxel;

    m_IsInputZeroVoxelObtained = false;

    m_IsOutputZeroVoxelObtained = false;

    m_Flag_OutputArray = false;

    m_Flag_OutputArray = false;

    m_Flag_OutputToOtherPlace = false;

    m_TotalOutputVoxelNumber = 0;
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
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputZeroVoxel(const VoxelType_Input& InputZeroVoxel)
{
    m_InputZeroVoxel = InputZeroVoxel;

    m_IsInputZeroVoxelObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputRegion(const mdkMatrix<uint64>* InputRegion)
{
    m_IInputRegion = InputRegion;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputVoxelSet(const mdkMatrix<uint64>* InputVoxelSet)
{
	m_InputVoxelSet = InputVoxelSet;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> InputFilterFunction)
{
	m_InputFilterFunction = InputFilterFunction;

	m_IsInputFilterFunctionObtained = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage)
{
	m_OutputImage = OutputImage;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputZeroVoxel(const VoxelType_Output& OutputZeroVoxel)
{
    m_OutputZeroVoxel = OutputZeroVoxel;

    m_IsOutputZeroVoxelObtained = true;
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

    if (m_InputRegion != nullptr && m_InputVoxelSet == nullptr)
    {
        if (m_OutputImage != nullptr)
        {
            auto Size = m_OutputImage->GetImageSize();

            if ((*m_InputRegion)(0, 1) != Size.Lx || (*m_InputRegion)(1, 1) != Size.Ly || (*m_InputRegion)(2, 1) != Size.Lz)
            {
                mdkError << "Invalid input: m_InputRegion does not match m_OutputImage @ mdk3DImageFilter::Run" << '\n';
                return false;
            }            
        }

        m_TotalOutputVoxelNumber = (*m_InputRegion)(0, 1)*(*m_InputRegion)(0, 2)*(*m_InputRegion)(0, 3);
    }
    else if (m_InputRegion == nullptr && m_InputVoxelSet != nullptr)
    {
        m_TotalOutputVoxelNumber = m_InputVoxelSet->GetElementNumber();
    }
    else
    {
        auto Size = m_InputImage->GetImageSize();

        m_TotalOutputVoxelNumber = Size.Lx*Size.Ly*Size.Lz;
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
    if (m_MaxThreadNumber > 1 && m_TotalOutputVoxelNumber > 1000)
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

	uint64 RegionOrigin[3] = { 0, 0, 0 }; // [x0, y0, z0]

	if (m_InputRegion != nullptr)
	{
		RegionOrigin[0] = (*m_InputRegion)(0, 0);
		RegionOrigin[1] = (*m_InputRegion)(1, 0);
		RegionOrigin[2] = (*m_InputRegion)(2, 0);
	}

	uint64 FilterCenter[3] = {0,0,0};     // [xc, yc, zc]

    if (m_Flag_OutputToOtherPlace == false)
    {
        if (m_InputRegion == nullptr && m_InputVoxelSet == nullptr)
        {
            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputImage)(VoxelIndex));
            }
        }
        else if (m_InputRegion != nullptr && m_InputVoxelSet == nullptr)
        {
            if (m_Flag_OutputImage == true)
            {
                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                    FilterCenter[0] += RegionOrigin[0];
                    FilterCenter[1] += RegionOrigin[1];
                    FilterCenter[2] += RegionOrigin[2];

                    this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputImage)(VoxelIndex));
                }
            }
            else
            {
                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                    FilterCenter[0] += RegionOrigin[0];
                    FilterCenter[1] += RegionOrigin[1];
                    FilterCenter[2] += RegionOrigin[2];

                    this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputArray)[VoxelIndex]);
                }
            }
        }
        else if (m_InputRegion == nullptr && m_InputVoxelSet != nullptr)
        {
            if (m_Flag_OutputArray == true)
            {
                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                    this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputArray)[VoxelIndex]);
                }
            }
            else
            {
                for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
                {
                    m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                    this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputImage)(VoxelIndex));
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
        auto tempOutputVoxel = m_OutputZeroVoxel;

        if (m_InputRegion == nullptr && m_InputVoxelSet == nullptr)
        {
            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
        else if (m_InputRegion != nullptr && m_InputVoxelSet == nullptr)
        {
            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                FilterCenter[0] += RegionOrigin[0];
                FilterCenter[1] += RegionOrigin[1];
                FilterCenter[2] += RegionOrigin[2];

                this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], tempOutputVoxel);

                this->OutputFunction(VoxelIndex, tempOutputVoxel);
            }
        }
        else if (m_InputRegion == nullptr && m_InputVoxelSet != nullptr)
        {
            for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
            {
                m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

                this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], tempOutputVoxel);

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

	uint64 MinVoxelNumberPerThread = 100;

	for (uint64 i = m_MaxThreadNumber; i > 0; --i)
	{
		VoxelNumberPerThread = TotalVoxelNumber / i;

		if (VoxelNumberPerThread >= MinVoxelNumberPerThread)
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
FilterFunction(uint64 xIndex_center, uint64 yIndex_center, uint64 zIndex_center, VoxelType_Output& OutputVoxel)
{
	/*
	VoxelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
		Output += i * (*m_InputImage)(xIndex_center, yIndex_center, zIndex_center);
	}
	*/

	if (m_IsInputFilterFunctionObtained == true)
	{
        m_InputFilterFunction(xIndex_center, yIndex_center, zIndex_center, *m_InputImage, OutputVoxel);
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


template<typename VoxelType_Input, typename VoxelType_Output>
template<typename FilterFunctionType>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run(FilterFunctionType InputFilterFunction)
{
    auto IsOK = this->CheckInputData();

    if (IsOK == false)
    {
        return false;
    }

    //--------------------------------------------------------------------------------

	auto IsGood = this->Preprocess();

	if (IsGood == false)
	{
		return false;
	}

	// multi-thread ----------------------------------------------------------------------
    if (m_MaxThreadNumber > 1 && m_TotalOutputVoxelNumber > 1000)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_start;
		std::vector<uint64> IndexList_end;

        this->DivideData(0, m_TotalOutputVoxelNumber - 1, IndexList_start, IndexList_end);

		uint64 ThreadNumber = IndexList_start.size();

		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread[i] = std::thread([&]{this->Run_in_a_Thread(IndexList_start[i], IndexList_end[i], InputFilterFunction); });
		}

		//wait for all the threads
		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread[i].join();
		}
	}
	else//single-thread
	{
        this->Run_in_a_Thread(0, m_TotalOutputVoxelNumber - 1, InputFilterFunction);
	}

    //-------------------------------------------------------------------------------

    return this->Postprocess();
}


template<typename VoxelType_Input, typename VoxelType_Output>
template<typename FilterFunctionType>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 OutputVoxelIndex_start, uint64 OutputVoxelIndex_end, FilterFunctionType InputFilterFunction)
{
    std::cout << "VoxelIndex_start " << OutputVoxelIndex_start << '\n';
    std::cout << "VoxelIndex_end   " << OutputVoxelIndex_end << '\n';

	uint64 RegionOrigin[3] = { 0, 0, 0 }; // [x0, y0, z0]

	if (m_InputRegion != nullptr)
	{
		RegionOrigin[0] = (*m_InputRegion)(0, 0);
		RegionOrigin[1] = (*m_InputRegion)(1, 0);
		RegionOrigin[2] = (*m_InputRegion)(2, 0);
	}

	uint64 FilterCenter[3] = { 0, 0, 0 };     // [xc, yc, zc]

    VoxelType_Output tempOutputVoxel;

    if (m_InputRegion == nullptr && m_InputVoxelSet == nullptr)
    {
        for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
        {
            m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

            InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, tempOutputVoxel);

            this->OutputFunction(VoxelIndex, tempOutputVoxel);
        }
    }
    else if (m_InputRegion != nullptr && m_InputVoxelSet == nullptr)
    {
        for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
        {
            m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

            FilterCenter[0] += RegionOrigin[0];
            FilterCenter[1] += RegionOrigin[1];
            FilterCenter[2] += RegionOrigin[2];

            InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, tempOutputVoxel);

            this->OutputFunction(VoxelIndex, tempOutputVoxel);
        }
    }
    else if (m_InputRegion == nullptr && m_InputVoxelSet != nullptr)
    {
        for (uint64 VoxelIndex = OutputVoxelIndex_start; VoxelIndex <= OutputVoxelIndex_end; ++VoxelIndex)
        {
            m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

            InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, tempOutputVoxel);

            this->OutputFunction(VoxelIndex, tempOutputVoxel);
        }
    }
    else
    {
        mdkError << "Invalid Input @ mdk3DImageFilter::Run_in_a_Thread" << '\n';
    }
}

}//end namespace mdk

#endif