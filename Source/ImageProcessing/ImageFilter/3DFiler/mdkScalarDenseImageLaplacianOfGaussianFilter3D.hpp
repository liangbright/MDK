#ifndef mdk_ScalarDenseImageLaplacianOfGaussianFilter3D_hpp
#define mdk_ScalarDenseImageLaplacianOfGaussianFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageLaplacianOfGaussianFilter3D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageLaplacianOfGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ScalarDenseImageConvolutionFilter3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
    m_SigmaList.Clear();
    m_RotationMatrix.Clear();
    m_CutOffRatio = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(const DenseVector<ScalarType, 3>& SigmaList, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio)
{    
	m_SigmaList = SigmaList;
    m_RotationMatrix = RotationMatrix;    
    m_CutOffRatio = CutOffRatio;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ScalarDenseImageConvolutionFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_SigmaList[0] <= 0.0 || m_SigmaList[1] <= 0.0 || m_SigmaList[2] <= 0.0)
	{
		MDK_Error("Sigma_x <= 0.0 || Sigma_y <= 0.0 || Sigma_z <= 0.0 @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_RotationMatrix.GetColCount() != 3 || m_RotationMatrix.GetColCount() != 3)
	{
		MDK_Error("RotationMatrix is invalid @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarDenseImageLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask()
{
// to do
}


}//end namespace mdk

#endif