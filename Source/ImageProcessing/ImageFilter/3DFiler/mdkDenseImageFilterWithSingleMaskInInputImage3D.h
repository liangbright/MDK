#ifndef mdk_DenseImageFilterWithSingleMaskInInputImage3D_h
#define mdk_DenseImageFilterWithSingleMaskInInputImage3D_h

#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class DenseImageFilterWithSingleMaskInInputImage3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	bool m_Flag_UseMaskOf3DPosition;
	// true: m_PointMask_3DPosition_InputImage
	// false: m_PointMask_3DIndex_InputImage

	bool m_Flag_AutoSelectMask;

	DenseMatrix<ScalarType> m_PointMask_3DPosition_InputImage;
    // row_0: dx
    // row_1: dy
    // row_2: dz

	DenseMatrix<ScalarType> m_PointMask_3DIndex_InputImage; // 3DIndex in m_InputImage
	// row_0: dx_Index
	// row_1: dy_Index
	// row_2: dz_Index

	BoxRegionOf3DIndexInImage3D    m_NOBoundCheckRegion_3DIndex;
	BoxRegionOf3DPositionInImage3D m_NOBoundCheckRegion_3DPosition;

protected:		
	DenseImageFilterWithSingleMaskInInputImage3D();
	virtual ~DenseImageFilterWithSingleMaskInInputImage3D();

public:
    virtual void Clear(); // must be virtual

private:
	void ClearSelf();

protected:
	void SelectMaskOf3DIndex();
	void SelectMaskOf3DPosition();
	void EnableAutoSelectMask(bool OnOFF = true);
	bool Is3DPositionUsedInSelectedMask();
	void AutoSelectMask();

	virtual bool Preprocess();                     // must be virtual
    virtual void BuildMask_3DIndex() {}            // must be virtual
	virtual void BuildMask_3DPosition() {}         // must be virtual

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z);
	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z);

	//-------------
	// PointMask may be m_PointMask_3DPosition or m_PointMask_3DIndex_InputImage
	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMask_At3DPositionInOutputImage(ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMask_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0);

	//-------------
	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPositionInOutputImage(ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPosition_At3DPositionInOutputImage(ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPosition_At3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseVector<int_max, 3>& Index3D);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPosition_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPosition_At3DIndexInOutputImage(const DenseVector<int_max, 3>& Index3D);

private:
    void ComputeRegionOfNOBoundCheck_3DIndex();
	void ComputeRegionOfNOBoundCheck_3DPosition();

private:
	DenseImageFilterWithSingleMaskInInputImage3D(const DenseImageFilterWithSingleMaskInInputImage3D&) = delete;
	void operator=(const DenseImageFilterWithSingleMaskInInputImage3D&) = delete;
};

}//end namespace mdk


#include "mdkDenseImageFilterWithSingleMaskInInputImage3D.hpp"

#endif