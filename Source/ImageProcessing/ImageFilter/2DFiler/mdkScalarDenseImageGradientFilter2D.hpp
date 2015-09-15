#ifndef mdk_ScalarDenseImageGradientFilter2D_hpp
#define mdk_ScalarDenseImageGradientFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::ScalarDenseImageGradientFilter2D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::~ScalarDenseImageGradientFilter2D()
{
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::Clear()
{
	this->ImageFilter2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::ClearSelf()
{
	m_Radius = 0;
	m_Flag_MaskOriginLocation = 0;
	m_AngleResolution = 1;
	m_MaskList.Clear();
	m_MaskCountAtEachLevel.Clear();
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SetRadius(double Radius)
{
	m_Radius = Radius;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SetMaskOriginInMiddle()
{
	m_Flag_MaskOriginLocation = 0;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SetMaskOriginAsPositivePole()
{
	m_Flag_MaskOriginLocation = 1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SetMaskOriginAsNegativePole()
{
	m_Flag_MaskOriginLocation = -1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SetAngleResolution(ScalarType Resolution)
{
	m_AngleResolution = Resolution;
}

/*
template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::SaveMask(const String& FilePathAndName)
{
	
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::LoadMask(const String& FilePathAndName)
{

}
*/

template<typename InputPixelType, typename ScalarType>
bool ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::CheckInput()
{
	if (this->ImageFilter2D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("m_Radius <= 0.0 @ ScalarDenseImageGradientFilter2D::CheckInput(...)")
		return false;
	}

	if (m_AngleResolution < 0 || m_AngleResolution > 6.2832)
	{
		MDK_Error("m_AngleResolution is invalid @ ScalarDenseImageGradientFilter2D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::BuildMaskWithGradientPrior(const DenseVector<ScalarType, 2>& GradientPrior)
{
	this->InitializeMaskList();

	// Select a subset from MaskList[0] Level 0 based on GradientPrior
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[0].GetElementCount());
	for (int_max n = 0; n < m_MaskList[0].GetElementCount(); ++n)
	{
		auto Direction = m_MaskList[0][n].PointP - m_MaskList[0][n].PointN;
		DotProductList[n] = Direction[0] * GradientPrior[0] + Direction[1] * GradientPrior[1];
	}
	auto IndexList_sort = DotProductList.Sort("descend");
	int_max MaskCount_keep = int_max(double(m_MaskCountAtEachLevel[0]) * 0.1) + 1;// keep 10% of MaskList[0]
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
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::BuildMask()
{
	this->InitializeMaskList();

	for (int_max k = 0; k < m_MaskList.GetElementCount()-1; ++k)
	{
		for (int_max n = 0; n < m_MaskList[k].GetElementCount(); ++n)
		{
			this->BuildMaskLink(k, n);
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::InitializeMaskList()
{
	//------------------------------------------------------
	const auto pi = ScalarType(std::acos(-1.0));
	auto MaskCount_max = int_max(2*pi/m_AngleResolution);
	int_max Level_max = std::log2(MaskCount_max+1)/2 - 1;
	if (Level_max < 0) { Level_max = 0; }
    //------------------------------------------------------
	m_MaskCountAtEachLevel.Clear();
	m_MaskCountAtEachLevel.Resize(Level_max+1);
	m_MaskCountAtEachLevel[0] = 8; //Level 0
	if (Level_max >= 1)
	{
		m_MaskCountAtEachLevel[1] = 32;// Level 1			
	}
	// from Level 2
	for (int_max k = 2; k <= Level_max; ++k)
	{
		m_MaskCountAtEachLevel[k] = 4 * m_MaskCountAtEachLevel[k - 1];
	}
	//------------------------------------------------------
	m_MaskList.Clear();
	m_MaskList.Resize(m_MaskCountAtEachLevel.GetElementCount());
	for (int_max k = 0; k < m_MaskList.GetElementCount(); ++k)
	{
		m_MaskList[k].Resize(m_MaskCountAtEachLevel[k]);
		this->InitializeMaskAtLevel(k);
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::InitializeMaskAtLevel(int_max Level)
{
	auto MaskCount = m_MaskCountAtEachLevel[Level];

	const double pi = std::acos(-1.0);
	auto Theta = 2*pi/double(MaskCount);

	if (m_Flag_MaskOriginLocation == 0)
	{
		double HalfRadius = m_Radius / 2.0;

		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = HalfRadius * std::sin(Angle);
			auto x = HalfRadius * std::cos(Angle);

			m_MaskList[Level][k].PointP = {x, y};
			m_MaskList[Level][k].PointN = { -x, -y };
		}
	}
	else if (m_Flag_MaskOriginLocation == 1)
	{
		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = m_Radius * std::sin(Angle);
			auto x = m_Radius * std::cos(Angle);

			m_MaskList[Level][k].PointP = { 0, 0 };
			m_MaskList[Level][k].PointN = { -x, -y };
		}
	}
	else // m_Flag_MaskOriginLocation == -1
	{
		for (int_max k = 0; k < MaskCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = m_Radius * std::sin(Angle);
			auto x = m_Radius * std::cos(Angle);

			m_MaskList[Level][k].PointP = { x, y };
			m_MaskList[Level][k].PointN = { 0, 0 };
		}
	}
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::BuildMaskLink(int_max Level, int_max MaskIndex)
{
	auto Level_next = Level+1;

	auto VectorA = m_MaskList[Level][MaskIndex].PointP - m_MaskList[Level][MaskIndex].PointN;
	DenseVector<ScalarType> DotProductList;
	DotProductList.Resize(m_MaskList[Level_next].GetElementCount());
	for (int_max n = 0; n < m_MaskList[Level_next].GetElementCount(); ++n)
	{
		auto VectorB = m_MaskList[Level_next][n].PointP - m_MaskList[Level_next][n].PointN;
		DotProductList[n] = VectorA[0] * VectorB[0] + VectorA[1] * VectorB[1];
	}

	auto IndexList_sort = DotProductList.Sort("descend");
	auto MaskCount_keep = int_max(double(m_MaskCountAtEachLevel[Level_next]) / double(m_MaskCountAtEachLevel[Level])) + 1;
	m_MaskList[Level][MaskIndex].MaskIndexListAtNextLevel = IndexList_sort.GetSubSet(0, MaskCount_keep - 1);
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{	
	return this->EvaluateAt2DPositionInInputImage(x0, y0);
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 2> ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(ScalarType x0, ScalarType y0)
{
	DenseVector<ScalarType, 2> Gradient_max;
	int_max MaskIndex_max = -1;

	// Level 0
	DenseVector<int_max> MaskIndexList = span(0, m_MaskList[0].GetLength()-1);
	this->EvaluateAt2DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, 0, MaskIndexList);
	
	//from Level 1
	for (int_max k = 1; k < m_MaskList.GetLength(); ++k)
	{
		MaskIndexList = m_MaskList[k - 1][MaskIndex_max].MaskIndexListAtNextLevel;
		this->EvaluateAt2DPositionInInputImage_SingleLevel(MaskIndex_max, Gradient_max, x0, y0, k, MaskIndexList);
	}

	return Gradient_max;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter2D<InputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, int_max Level, const DenseVector<int_max>& MaskIndexList)
{	
	ScalarType Magnitude = 0;

	for (int_max k = 0; k < MaskIndexList.GetLength(); ++k)
	{
		auto MaskIndex = MaskIndexList[k];
		
		auto xp = m_MaskList[Level][MaskIndex].PointP[0] + x0;
		auto yp = m_MaskList[Level][MaskIndex].PointP[1] + y0;

		auto xn = m_MaskList[Level][MaskIndex].PointN[0] + x0;
		auto yn = m_MaskList[Level][MaskIndex].PointN[1] + y0;

		auto PixelP = m_InputImage->GetPixelAt2DPosition<ScalarType>(xp, yp, m_ImageInterpolationOption);
		auto PixelN = m_InputImage->GetPixelAt2DPosition<ScalarType>(xn, yn, m_ImageInterpolationOption);
		auto Difference = PixelP - PixelN;

		if (k == 0 || Magnitude < Difference)
		{
			Magnitude = Difference;
			MaskIndex_max = MaskIndex;
			Gradient_max[0] = xp - xn;
			Gradient_max[1] = yp - yn;
		}
	}

	Gradient_max /= Gradient_max.L2Norm();
	Gradient_max *= Magnitude;
}

}//end namespace mdk

#endif