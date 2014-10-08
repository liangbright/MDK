#ifndef __mdkSingleClassObjectDetectionInSingleImage3D_h
#define __mdkSingleClassObjectDetectionInSingleImage3D_h

#include "mdkProcessObject.h"
#include "mdkSimpleObjectArray.h"
#include "mdkObjectArray.h"

namespace mdk
{

template<typename ScalarType = double>
struct Mask_Of_SingleClassObjectDetectionInSingleImage3D
{
	bool Flag_3DPhysicalPosition;
	// true:  use 3D physical position in PointSet
	// false: use 3D continuous index in PointSet

	SimpleObjectArray<DenseMatrix<ScalarType>> PointSet;	
	// PointSet[k]: { 3DIndex or 3D Position of each point }, k is index of region/PointSet
	// PointSet[k](0, :) : x
	// PointSet[k](1, :) : y
	// PointSet[k](2, :) : z
	// use 3D physical Position or 3D continuous index
	// Point_in_PointSet = RotationMatrix*ScaleMatrix*Point_in_the_databank
	// Point_in_PointSet is a point in input image coordinate system
	// Point_in_the_databank is a point in the databank coordinate system

	DenseVector<ScalarType, 3> Angle;
	// Angle[0]: angle about axes x
	// Angle[1]: angle about axes y
	// Angle[2]: angle about axes z

	DenseVector<ScalarType, 3> Scale;
	// Scale[0]: Scale about axes x
	// Scale[1]: Scale about axes y
	// Scale[2]: Scale about axes z
};

template<typename ScalarType = double>
struct EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D  // Object Candidate Evaluation Result
{
	DenseVector<ScalarType, 3> Origin; //  Origin of Mask, 3D physical Position or 3D continuous Index (determined by m_Flag_3DPositionInMask)
	int_max OriginIndex;
	int_max MaskIndex;                 //  index in m_MaskList
	ScalarType  Score;                 // could be probability or energy (log(p))
};

template<typename Voxel_Type, typename Scalar_Type = double>
class SingleClassObjectDetectionInSingleImage3D : public ProcessObject
{
public:
	typedef Voxel_Type                                                    VoxelType;
	typedef Scalar_Type								                      ScalarType;
	typedef Mask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType> MaskType;

protected:
	//-------------------------- input --------------------------------------------------//
	const Image3D<VoxelType>* m_InputImage;

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DPyhsicalPosition;   // evaluate object candidate at each Origin

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DContinuousIndex;      // evaluate object candidate at each Origin

	const ObjectArray<Mask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>* m_MaskList;

	int_max m_MaxNumberOfThread; // max number of threads

	//------------------------- internal data -------------------------------------------//

	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPyhsicalPosition;
	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DContinuousIndex;

	int_max m_TotalCandidateOriginNumber;
	bool m_Flag_ScanWholeImageGrid;
	//-----------------------------------------------------------------------------------//

	// output_0:
	ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>> m_Output;

	// about output:
    // (1) use ObjectArray<SparseVector<ScalarType>>
	//     m_Output[n][k] is the score of object measured by using MaskList[k] at position (x_n, y_n, z_n)
	//     easy to locate each output, but not easy to sort them by score (and then select the top 100 candidates)
	// (2) use ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>
	//    m_Output[m] is the info of the object measured by using a Mask
	//    not easy to locate output by position/index,  but easy to sort them by score

public:
	SingleClassObjectDetectionInSingleImage3D();
	~SingleClassObjectDetectionInSingleImage3D();

	virtual void Clear();

	void SetInputImage(const Image3D<VoxelType>* InputImage);

	void SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList);
	void SetCandidateOriginListOf3DContinuousIndex(const DenseMatrix<ScalarType>* Input3DPositionList);

	void SetMaskList(const ObjectArray<Mask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>* InputMaskList);

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

	ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>& OutputResult();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postrocess() {}
	virtual void BuildMask() {}

	bool WhetherMaskIsInsideImage_AtOrigin_3DContinuousIndex(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);
	bool WhetherMaskIsInsideImage_AtOrigin_3DPyhsicalPosition(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);

	// Evaluate candidate object at Origin (x, y, z), x/y/z is 3DContinuousIndex
	inline virtual void EvaluateCandidateAtOrigin_3DContinuousIndex(EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>& EvaluationResult,
										                            int_max MaskIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex) {}
	// Evaluate candidate object at Origin (x, y, z), x/y/z is 3DPyhsicalPosition
	inline virtual void EvaluateCandidateAtOrigin_3DPyhsicalPosition(EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>& EvaluationResult,
										                             int_max MaskIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex) {}

	int_max GetNumberOfThreadTobeCreated();

private:
	void ComputeRegionOfNOBoundCheck_3DContinuousIndex();
	void ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();

	void EvaluateMultipleCandidate(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex);

private:
	SingleClassObjectDetectionInSingleImage3D(const SingleClassObjectDetectionInSingleImage3D&) = delete;
	void operator=(const SingleClassObjectDetectionInSingleImage3D&) = delete;
};

}// namespace mdk

#include "mdkSingleClassObjectDetectionInSingleImage3D.hpp"

#endif