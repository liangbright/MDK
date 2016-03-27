#pragma once

namespace mdk
{

template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::ScalarDenseImageGradientFilter3D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::~ScalarDenseImageGradientFilter3D()
{
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::ClearSelf()
{
	m_Radius = 0;
	m_Flag_MaskOriginLocation = 0;
	m_MaskList.Clear();
	m_SphereResolution = 0;
	m_SphereBuilder.Clear();
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetRadius(double Radius)
{
	m_Radius = Radius;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetMaskOriginInMiddle()
{
	m_Flag_MaskOriginLocation = 0;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetMaskOriginAsPositivePole()
{
	m_Flag_MaskOriginLocation = 1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetMaskOriginAsNegativePole()
{
	m_Flag_MaskOriginLocation = -1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetSphereResolution(int_max Resolution)
{
	m_SphereResolution = Resolution;
}


template<typename InputPixelType, typename ScalarType>
bool ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::CheckInput()
{
	if (this->ImageFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("m_Radius <= 0.0 @ ScalarDenseImageGradientFilter3D::CheckInput(...)")
		return false;
	}

	if (m_SphereResolution <= 0.0)
	{
		MDK_Error("m_SphereResolution <= 0.0 @ ScalarDenseImageGradientFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::BuildMask()
{
	this->InitializeMaskList();

	for (int_max k = 0; k < m_MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::BuildMaskWithGradientPrior(const DenseVector<ScalarType, 3>& GradientPrior)
{
	this->InitializeMaskList();

	// Select a subset from MaskList[0] Level 0 based on GradientPrior
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[0].GetElementCount());
	for (int_max n = 0; n < m_MaskList[0].GetElementCount(); ++n)
	{
		auto Direction = m_MaskList[0][n].PointP - m_MaskList[0][n].PointN;
		DotProductList[n] = Direction[0] * GradientPrior[0] + Direction[1] * GradientPrior[1] + Direction[2] * GradientPrior[2];
	}
	auto IndexList_sort = DotProductList.Sort("descend");

	int_max MaskCount_keep = 10;// keep 10 of MaskList[0]
	auto MaskIndexList_keep = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
	m_MaskList[0] = m_MaskList[0].GetSubSet(MaskIndexList_keep);

	// Link
	for (int_max k = 0; k < m_MaskList.GetElementCount() - 1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::InitializeMaskList()
{
	//-----------------------------------------------------
	m_SphereBuilder.Clear();

	int_max MaxDepth = 0;
	if (m_SphereResolution <= 20)
	{
		MaxDepth = 0;
	}
	else if (m_SphereResolution <= 42)
	{
		MaxDepth = 1;
	}
	else if (m_SphereResolution <= 162)
	{
		MaxDepth = 2;
	}
	else if (m_SphereResolution <= 642)
	{
		MaxDepth = 3;
	}
	else if (m_SphereResolution <= 2562)
	{
		MaxDepth = 4;
	}
	else if (m_SphereResolution <= 10242)
	{
		MaxDepth = 5;
	}
	else if (m_SphereResolution <= 40962)
	{
		MaxDepth = 6;
	}
	else
	{
		MaxDepth = (std::log2(m_SphereResolution) / 2) - 2;
	}

	if (m_SphereResolution > 40962)
	{
		MDK_Warning("m_SphereResolution > 40962 and MaxDepth > 6 @ ScalarDenseImageGradientFilter3D::InitializeMaskList()")
	}

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

		if (m_Flag_MaskOriginLocation == 0)
		{
			double HalfRadius = m_Radius / 2.0;
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;				
				m_MaskList[Level][MaskIndex].PointP = Direction*HalfRadius;
				m_MaskList[Level][MaskIndex].PointN = Direction*(-HalfRadius);
			}
		}
		else if (m_Flag_MaskOriginLocation == 1)
		{
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;
				m_MaskList[Level][MaskIndex].PointP = { 0, 0 };
				m_MaskList[Level][MaskIndex].PointN = Direction*(-m_Radius);
			}
		}
		else // m_Flag_MaskOriginLocation == -1
		{
			int_max MaskIndex = -1;
			for (auto it = Sphere.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
			{
				auto Direction = it.Point().GetPosition();
				MaskIndex += 1;
				m_MaskList[Level][MaskIndex].PointP = Direction*m_Radius;
				m_MaskList[Level][MaskIndex].PointN = { 0, 0 };
			}
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::BuildMaskLink(int_max Level, int_max MaskIndex)
{
	int_max Level_next=Level+1;

	auto VectorA = m_MaskList[Level][MaskIndex].PointP - m_MaskList[Level][MaskIndex].PointN;
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[Level_next].GetElementCount());
	for (int_max n = 0; n < m_MaskList[Level_next].GetElementCount(); ++n)
	{
		auto VectorB = m_MaskList[Level_next][n].PointP - m_MaskList[Level_next][n].PointN;
		DotProductList[n] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1] + VectorA[2] * VectorB[2];
	}

	auto IndexList_sort = DotProductList.Sort("descend");
	
	int_max MaskCount_keep = 6;// 6 neighbor at the next level

	m_MaskList[Level][MaskIndex].MaskIndexListAtNextLevel = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{	
	return this->EvaluateAt3DPositionInInputImage(x0, y0, z0);
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage(ScalarType x0, ScalarType y0, ScalarType z0)
{
	DenseVector<ScalarType, 3> Gradient_max;
	int_max MaskIndex_max = -1;

	// Level 0
	DenseVector<int_max> MaskIndexList = span(0, m_MaskList[0].GetLength() - 1);
	this->EvaluateAt3DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, z0, 0, MaskIndexList);

	//from Level 1
	for (int_max k = 1; k < m_MaskList.GetLength(); ++k)
	{
		MaskIndexList = m_MaskList[k - 1][MaskIndex_max].MaskIndexListAtNextLevel;
		this->EvaluateAt3DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, z0, k, MaskIndexList);
	}

	return Gradient_max;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, ScalarType z0, int_max Level, const DenseVector<int_max>& MaskIndexList)
{
	ScalarType Magnitude = 0;

	for (int_max k = 0; k < MaskIndexList.GetLength(); ++k)
	{
		auto MaskIndex = MaskIndexList[k];

		auto xp = m_MaskList[Level][MaskIndex].PointP[0] + x0;
		auto yp = m_MaskList[Level][MaskIndex].PointP[1] + y0;
		auto zp = m_MaskList[Level][MaskIndex].PointP[2] + z0;

		auto xn = m_MaskList[Level][MaskIndex].PointN[0] + x0;
		auto yn = m_MaskList[Level][MaskIndex].PointN[1] + y0;
		auto zn = m_MaskList[Level][MaskIndex].PointN[2] + z0;

		auto PixelP = m_InputImage->GetPixelAt3DPosition<ScalarType>(xp, yp, zp, m_ImageInterpolationOption);
		auto PixelN = m_InputImage->GetPixelAt3DPosition<ScalarType>(xn, yn, zn, m_ImageInterpolationOption);
		auto Difference = PixelP - PixelN;

		if (k == 0 || Magnitude < Difference)
		{
			Magnitude = Difference;
			MaskIndex_max = MaskIndex;
			Gradient_max[0] = xp - xn;
			Gradient_max[1] = yp - yn;
			Gradient_max[2] = zp - zn;
		}
	}

	Gradient_max /= Gradient_max.L2Norm();
	Gradient_max *= Magnitude;
}

}//end namespace mdk
