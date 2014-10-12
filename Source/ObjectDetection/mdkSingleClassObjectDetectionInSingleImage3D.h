#ifndef __mdkSingleClassObjectDetectionInSingleImage3D_h
#define __mdkSingleClassObjectDetectionInSingleImage3D_h

#include "mdkProcessObject.h"
#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkImage3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename ScalarType = double>
struct ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D
{
	bool Flag_PhysicalPosition;
	// true:  use 3D physical position in PointSet
	// false: use 3D  index in PointSet

	DenseMatrix<ScalarType> PointSet;
	// each column is (x, y, z)
	// use 3D physical Position or 3D  index
	// Point_in_PointSet = RotationMatrix*ScaleMatrix*Point_in_the_databank
	// Point_in_PointSet is a point in input image coordinate system
	// Point_in_the_databank is a point in the databank coordinate system

	DataArray<DenseVector<int_max>> PointSubSetList;
	// PointSubSetList[k]: { Index of point in PointSubSet k}, k is index of PointSubSet
	// PointSubSet can overlap each other

	DenseVector<ScalarType, 3> Angle;
	// Angle[0]: angle about axes x
	// Angle[1]: angle about axes y
	// Angle[2]: angle about axes z

	DenseVector<ScalarType, 3> Scale;
	// Scale[0]: Scale about axes x
	// Scale[1]: Scale about axes y
	// Scale[2]: Scale about axes z

//---------------------------------------------------------------------------------------------
	ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D() {}
	~ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D() {}

	void Clear()
	{
		Flag_PhysicalPosition = false;
		PointSet.Clear();
		PointSubSetList.Clear();
		PartOriginList.Clear();
		Angle.Clear();
		Scale.Clear();
	}

	DenseVector<ScalarType, 3> GetPointPositionByIndex(int_max Index) const
	{
		DenseVector<ScalarType, 3> Position;
		PointSet.GetCol(Index, Position);
	}
};


template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class SingleClassObjectDetectionInSingleImage3D : public ProcessObject
{
public:
	typedef InputPixel_Type														  InputPixelType;
	typedef OutputPixel_Type													  OutputPixelType;
	typedef Scalar_Type															  ScalarType;
	typedef ObjectMask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>	  ObjectMaskType;

protected:
	//-------------------------- input --------------------------------------------------//
	const Image3D<InputPixelType>* m_ObjectImage;

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DPyhsicalPosition;   // evaluate object candidate at each point on m_ObjectMembershipImage

	DenseMatrix<int_max> m_CandidateOriginList_3DIndex;  // evaluate object candidate at each point on m_ObjectMembershipImage

	const DataArray<ObjectMaskType>* m_ObjectMaskList;

	int_max m_MaxNumberOfThread; // max number of threads

	//------------------------- internal data -------------------------------------------//

	DataArray<Image3DBoxRegionOf3DPhysicalPosition<ScalarType>> m_NOBoundCheckRegionList_3DPyhsicalPosition;
	DataArray<Image3DBoxRegionOf3DIndex> m_NOBoundCheckRegionList_3DIndex;

	int_max m_TotalCandidateOriginNumber;
	bool m_Flag_ScanWholeImageGrid; // m_ObjectMembershipImage
	//-----------------------------------------------------------------------------------//

	// output_0:
	Image3D<OutputPixelType> m_ObjectMembershipImage;

	// about output:
    // (1) use DataArray<SparseVector<ScalarType>>
	//     m_Output[n][k] is the score of object measured by using MaskList[k] at position (x_n, y_n, z_n)
	//     easy to locate each output, but not easy to sort them by score (and then select the top 100 candidates)
	// (2) use DataArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>
	//    m_Output[m] is the info of the object measured by using a Mask
	//    not easy to locate output by position/index,  but easy to sort them by score

protected:
	SingleClassObjectDetectionInSingleImage3D();
	virtual ~SingleClassObjectDetectionInSingleImage3D();

public:
	virtual void Clear();

	void SetObjectImage(const Image3D<InputPixelType>* InputImage);

	void SetInfoOfObjectMembershipImage(const DenseVector<ScalarType, 3>& Origin,
		                                const DenseVector<ScalarType, 3>& Spacing,
		                                const DenseVector<ScalarType, 3>& Size);

	void SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList);

	void SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList);

	void SetObjectMaskList(const DataArray<ObjectMaskType>* InputMaskList);	
	const DataArray<ObjectMaskType>* GetObjectMaskList();

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

	Image3D<OutputPixelType>* GetObjectMembershipImage();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }
	virtual void BuildMask() {}

	bool WhetherMaskIsInsideImage_AtOrigin_3DIndex(int_max MaskIndex, int_max x, int_max y, int_max z);
	bool WhetherMaskIsInsideImage_AtOrigin_3DPyhsicalPosition(int_max MaskIndex, ScalarType x, ScalarType y, ScalarType z);

	// Evaluate candidate object at Origin (x, y, z), x/y/z is 3DIndex
	inline virtual void EvaluateCandidateAtOrigin_3DIndex(int_max x0, int_max y0, int_max z0, int_max ThreadIndex) = 0;

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetPixelSetByObjectMaskAtOrigin_3DIndex(int_max MaskIndex, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetPixelSetByObjectMaskAtOrigin_3DPyhsicalPosition(int_max MaskIndex, ScalarType x0, ScalarType y0, ScalarType z0);

	int_max GetNumberOfThreadTobeCreated();

private:
	void ComputeRegionOfNOBoundCheck_3DIndex();
	void ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();

	void EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex);

private:
	SingleClassObjectDetectionInSingleImage3D(const SingleClassObjectDetectionInSingleImage3D&) = delete;
	void operator=(const SingleClassObjectDetectionInSingleImage3D&) = delete;
};

}// namespace mdk

#include "mdkSingleClassObjectDetectionInSingleImage3D.hpp"

#endif