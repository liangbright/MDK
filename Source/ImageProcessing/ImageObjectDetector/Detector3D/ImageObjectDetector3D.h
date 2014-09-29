#ifndef __mdkImageObjectDetector3D_h
#define __mdkImageObjectDetector3D_h

#include "mdkSimpleObjectArray.h"
#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename ScalarType = double>
struct Mask_Of_ImageObjectDetector3D
{
	int_max Flag_3DPosition = -1;
	// 1: use 3D physical position in PointSet
	// 0: use 3D Index in PointSet
	// -1: unknown

	SimpleObjectArray<DenseMatrix<ScalarType>> PointSet;
	// PointSet[k]: { 3DIndex or 3D Position of each point }
	// PointSet[k](0, :) : x
	// PointSet[k](1, :) : y
	// PointSet[k](2, :) : z

	DenseVector<ScalarType, 3> Angle;
	// Angle[0]: angle about axes x
	// Angle[1]: angle about axes y
	// Angle[2]: angle about axes z

	DenseVector<ScalarType, 3> Scale;
	// Scale[0]: Scale about axes x
	// Scale[1]: Scale about axes y
	// Scale[2]: Scale about axes z
};

struct Output_Of_ImageObjectDetector3D
{
	int_max MaskOriginIndex;
	int_max MaskIndex;
	double  Probability;
};

template<typename InputPixel_Type, typename Scalar_Type = double>
class ImageObjectDetector3D : public ImageFilter3D<InputPixel_Type, SparseVector<Scalar_Type>>
{
public:
	typedef InputPixel_Type                           InputPixelType;
	typedef Scalar_Type								  ScalarType;
	typedef SparseVector<ScalarType>				  OutputPixelType;
	typedef Mask_Of_ImageObjectDetector3D<ScalarType> MaskType;

protected:
	int_max m_Flag_3DPositionInMask;
	// 1: use 3D physical position in Region
	// 0: use 3D Index in Region
	// -1: unknown

	ObjectArray<Mask_Of_ImageObjectDetector3D<ScalarType>> m_MaskList;

	ObjectArray<Image3DBoxRegionOf3DIndex>            m_NOBoundCheckRegionList_3DIndex;
	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPosition;

	ObjectArray<SparseVector<ScalarType>> m_Output;
	// m_Output[n][k] is the probability of object measured by using MaskList[k] at position (x_n, y_n, z_n)

public:
	ImageObjectDetector3D();
	~ImageObjectDetector3D();

	virtual void Clear();
	virtual void ClearOutput();

	ObjectArray<Mask_Of_ImageObjectDetector3D<ScalarType>>& MaskList();
	const ObjectArray<Mask_Of_ImageObjectDetector3D<ScalarType>>& MaskList() const;

	void Use3DIndexInMask();
	void Use3DPhysicalPositionInMask();

	const ObjectArray<SparseVector<ScalarType>>& GetOutput() const;

protected:
	virtual bool Preprocess();
	virtual void BuildMaskOf3DIndex() {}
	virtual void BuildMaskOf3DPosition() {}

	inline bool WhetherToCheckBoundAtMaskOrigin_3DIndex(double x, double y, double z, int_max MaskIndex);
	inline bool WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z, int_max MaskIndex);

	inline void OutputFunction(int_max OutputPixelIndex, OutputPixelType& OutputPixel, int_max ThreadIndex);

private:
	void ComputeRegionOfNOBoundCheck_3DIndex();
	void ComputeRegionOfNOBoundCheck_3DPosition();

private:
	// do not need these
	void SetOutputImage(Image3D<OutputPixelType>* OutputImage) {}
	void SetOutputArray(ObjectArray<OutputPixelType>* OutputArray)  {}
	Image3D<OutputPixelType>* GetOutputImage()  { return nullptr; }
	ObjectArray<OutputPixelType>* GetOutputArray()  { return nullptr; }

private:
	ImageObjectDetector3D(const ImageObjectDetector3D&) = delete;
	void operator=(const ImageObjectDetector3D&) = delete;
};

}// namespace mdk

#include "mdkImageObjectDetector3D.hpp"

#endif