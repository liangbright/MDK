#pragma once

namespace mdk
{
template<typename ScalarType>
DirectionGenerator3D<ScalarType>::DirectionGenerator3D()
{
	this->Clear();
}

template<typename ScalarType>
DirectionGenerator3D<ScalarType>::~DirectionGenerator3D()
{
}

template<typename ScalarType>
void DirectionGenerator3D<ScalarType>::Clear()
{
	m_MaskList.Clear();
	m_SphereBuilder.Clear();
	m_SphereResolution = 162;
}


template<typename ScalarType>
void DirectionGenerator3D<ScalarType>::SetSphereResolution(ScalarType Resolution)
{
	m_SphereResolution = Resolution;
}


template<typename ScalarType>
bool DirectionGenerator3D<ScalarType>::CheckInput()
{
	if (m_SphereResolution < 20)
	{
		m_SphereResolution = 20;
	}

	if (m_SphereResolution > 40962)
	{
		MDK_Error("m_SphereResolution > 40962 NOT supported @ DirectionGenerator3D::CheckInput()")
		return false;
	}

	return true;
}


template<typename ScalarType>
void DirectionGenerator3D<ScalarType>::BuildMask()
{
	if (this->CheckInput() == false)
	{
		return;
	}

	this->InitializeMaskList();

	for (int_max k = 0; k < m_MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename ScalarType>
void DirectionGenerator3D<ScalarType>::InitializeMaskList()
{
	//-----------------------------------------------------
	m_SphereBuilder.Clear();

	int_max MaxDepth = this->GetDepthByResolution(m_SphereResolution);

	m_SphereBuilder.SetMaxDepth(MaxDepth);
	m_SphereBuilder.Update();
	auto& SphereList = *m_SphereBuilder.GetSphereList();
	//-----------------------------------------------------
	m_MaskList.Clear();
	m_MaskList.Resize(SphereList.GetLength());
	for (int_max Level = 0; Level < SphereList.GetLength(); Level++)
	{
		auto& Sphere = SphereList[Level];

		m_MaskList[Level].Resize(Sphere.GetPointCount());

		int_max MaskIndex = -1;
		for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
		{
			MaskIndex += 1;
			m_MaskList[Level][MaskIndex].Level = Level;
			m_MaskList[Level][MaskIndex].Index = MaskIndex;
			m_MaskList[Level][MaskIndex].Direction = it.Point().GetPosition();
			m_MaskList[Level][MaskIndex].Flag = false;
		}	
	}
}


template<typename ScalarType>
void DirectionGenerator3D<ScalarType>::BuildMaskLink(int_max Level, int_max MaskIndex)
{
	int_max Level_next = Level + 1;

	auto DirectionA = m_MaskList[Level][MaskIndex].Direction;
	
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[Level_next].GetElementCount());

	for (int_max n = 0; n < m_MaskList[Level_next].GetElementCount(); ++n)
	{
		auto DirectionB = m_MaskList[Level_next][n].Direction;
		DotProductList[n] = DirectionA[0] * DirectionB[0] + DirectionA[1] * DirectionB[1] + DirectionA[2] * DirectionB[2];
	}

	auto IndexList_sort = DotProductList.Sort("descend");

	int_max MaskCount_keep = 6;// 6 neighbor at the next level

	m_MaskList[Level][MaskIndex].MaskIndexListAtNextLevel = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
}


template<typename ScalarType>
DenseMatrix<ScalarType> DirectionGenerator3D<ScalarType>::
FindDirectionCandidate(const DenseVector<ScalarType, 3> Direction_mean, ScalarType DirectionDotProduct_min, int_max Resolution)
{	
	DenseMatrix<ScalarType> DirectionList;
	DirectionList.ReserveCapacity(3 * 10);
	//DirectionList.AppendCol(Direction_mean);

	int_max MaskLevel_max = this->GetDepthByResolution(Resolution);
	if (MaskLevel_max <= 2)
	{		
		for (int_max k = 0; k < m_MaskList[MaskLevel_max].GetLength(); ++k)
		{
			auto Direction_k = m_MaskList[MaskLevel_max][k].Direction;
			auto DotProduct = Direction_mean[0] * Direction_k[0] + Direction_mean[1] * Direction_k[1] + Direction_mean[2] * Direction_k[2];
			if (DotProduct >= DirectionDotProduct_min)
			{
				DirectionList.AppendCol(Direction_k);
			}
		}
		return DirectionList;
	}
   
	// MaskLevel_max > 2
	// search MaskList by using queue
	std::queue<Mask_Of_DirectionGenerator3D<ScalarType>> MaskQueue;

	int_max IndexOfNearestMask_Level_0 = 0;
	ScalarType DotProduct_max = -10;
	for (int_max k = 0; k < m_MaskList[0].GetLength(); ++k)
	{
		auto Direction_k = m_MaskList[0][k].Direction;
		auto DotProduct_k = Direction_mean[0] * Direction_k[0] + Direction_mean[1] * Direction_k[1] + Direction_mean[2] * Direction_k[2];
		if (DotProduct_k >= DirectionDotProduct_min)
		{
			m_MaskList[0][k].Flag = false;
			MaskQueue.push(m_MaskList[0][k]);
		}

		if (DotProduct_max < DotProduct_k)
		{
			DotProduct_max = DotProduct_k;
			IndexOfNearestMask_Level_0 = k;
		}
	}

	if (MaskQueue.empty() == true)
	{
		MaskQueue.push(m_MaskList[0][IndexOfNearestMask_Level_0]);
	}

	int_max Counter = 0;

	while (MaskQueue.empty() == false)
	{
		Counter += 1;

		auto Mask = MaskQueue.front();//copy
		MaskQueue.pop();//delete
		if (m_MaskList[Mask.Level][Mask.Index].Flag == false)
		{
			if (Mask.Level == MaskLevel_max)
			{
				DirectionList.AppendCol(Mask.Direction);
			}
			else
			{
				int_max IndexOfNearestMask_Level_next = 0;
				ScalarType DotProduct_max_Level_next = -10;
				bool Flag_Atleast_OneMask_added_to_Queue = false;

				auto Level_next = Mask.Level + 1;
				for (int_max k = 0; k < Mask.MaskIndexListAtNextLevel.GetLength(); ++k)
				{
					auto MaskIndex_k = Mask.MaskIndexListAtNextLevel[k];
					auto Flag_k = m_MaskList[Level_next][MaskIndex_k].Flag;
					if (Flag_k == false)
					{
						auto Direction_k = m_MaskList[Level_next][MaskIndex_k].Direction;
						auto DotProduct_k = Direction_mean[0] * Direction_k[0] + Direction_mean[1] * Direction_k[1] + Direction_mean[2] * Direction_k[2];
						if (DotProduct_k >= DirectionDotProduct_min)
						{
							m_MaskList[Level_next][MaskIndex_k].Flag = false;
							MaskQueue.push(m_MaskList[Level_next][MaskIndex_k]);
							Flag_Atleast_OneMask_added_to_Queue = true;
						}

						if (DotProduct_max_Level_next < DotProduct_k)
						{
							DotProduct_max_Level_next = DotProduct_k;
							IndexOfNearestMask_Level_next = MaskIndex_k;
						}
					}
				}

				if (Flag_Atleast_OneMask_added_to_Queue == false && Level_next < MaskLevel_max)
				{
					MaskQueue.push(m_MaskList[Level_next][IndexOfNearestMask_Level_next]);
				}
			}
		}
		m_MaskList[Mask.Level][Mask.Index].Flag = true;
	}
	
	std::cout << "Counter = " << Counter << '\n';

	return DirectionList;
}


template<typename ScalarType>
int_max DirectionGenerator3D<ScalarType>::GetDepthByResolution(int_max Resolution)
{
	int_max Depth = 0;
	if (Resolution <= 20)
	{
		Depth = 0;
	}
	else if (Resolution <= 42)
	{
		Depth = 1;
	}
	else if (Resolution <= 162)
	{
		Depth = 2;
	}
	else if (Resolution <= 642)
	{
		Depth = 3;
	}
	else if (Resolution <= 2562)
	{
		Depth = 4;
	}
	else if (Resolution <= 10242)
	{
		Depth = 5;
	}
	else if (Resolution <= 40962)
	{
		Depth = 6;
	}
	else
	{
		MDK_Warning("Resolution > 40962 and Depth > 6  NOT supported @ DirectionGenerator3D::GetDepthByResolution()")
		Depth = 6;
	}
	return Depth;
}


}//namespace mdk
