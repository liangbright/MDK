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

	DenseMatrix<ScalarType> PointSet;
	// each column is (x, y, z)
	// use 3D physical Position or 3D continuous index
	// Point_in_PointSet = RotationMatrix*ScaleMatrix*Point_in_the_databank
	// Point_in_PointSet is a point in input image coordinate system
	// Point_in_the_databank is a point in the databank coordinate system

	SimpleObjectArray<DenseVector<int_max>> RegionList;
	// RegionList[k]: { Index of point in 3PointSet}, k is index of region/PointSet
	// region can overlap each other

	DenseVector<ScalarType, 3> Angle;
	// Angle[0]: angle about axes x
	// Angle[1]: angle about axes y
	// Angle[2]: angle about axes z

	DenseVector<ScalarType, 3> Scale;
	// Scale[0]: Scale about axes x
	// Scale[1]: Scale about axes y
	// Scale[2]: Scale about axes z

//---------------------------------------------------------------------------------------------
	Mask_Of_SingleClassObjectDetectionInSingleImage3D() {}
	~Mask_Of_SingleClassObjectDetectionInSingleImage3D() {}

	DenseVector<ScalarType, 3> GetPointPositionByIndex(int_max Index) const
	{
		DenseVector<ScalarType, 3> Position;
		PointSet.GetCol(Index, Position);
	}

};

template<typename ScalarType = double>
struct EvaluationInfo_Of_SingleClassObjectDetectionInSingleImage3D  // Object Candidate Evaluation Result
{
	DenseVector<ScalarType, 3> Origin; // Origin of Mask, 3D physical Position or 3D continuous Index (determined by m_Flag_3DPositionInMask)
	int_max OriginIndex;
	int_max MaskIndex;                 // index in m_MaskList
	ScalarType  Score;                 // could be probability or energy (log(p))
};

template<typename Pixel_Type, typename Scalar_Type = double>
class SingleClassObjectDetectionInSingleImage3D : public ProcessObject
{
public:
	typedef Pixel_Type																  PixelType;
	typedef Scalar_Type																  ScalarType;
	typedef Mask_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>			  MaskType;
	typedef EvaluationInfo_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>   EvaluationInfoType;

protected:
	//-------------------------- input --------------------------------------------------//
	const Image3D<PixelType>* m_InputImage;

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DPyhsicalPosition;   // evaluate object candidate at each Origin

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DContinuousIndex;      // evaluate object candidate at each Origin

	const ObjectArray<MaskType>* m_MaskList;

	int_max m_MaxNumberOfThread; // max number of threads

	//------------------------- internal data -------------------------------------------//

	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPyhsicalPosition;
	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DContinuousIndex;

	int_max m_TotalCandidateOriginNumber;
	bool m_Flag_ScanWholeImageGrid;
	//-----------------------------------------------------------------------------------//

	// output_0:
	ObjectArray<EvaluationResultType> m_Output;

	// about output:
    // (1) use ObjectArray<SparseVector<ScalarType>>
	//     m_Output[n][k] is the score of object measured by using MaskList[k] at position (x_n, y_n, z_n)
	//     easy to locate each output, but not easy to sort them by score (and then select the top 100 candidates)
	// (2) use ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>
	//    m_Output[m] is the info of the object measured by using a Mask
	//    not easy to locate output by position/index,  but easy to sort them by score

protected:
	SingleClassObjectDetectionInSingleImage3D();
	virtual ~SingleClassObjectDetectionInSingleImage3D();

public:
	virtual void Clear();

	void SetInputImage(const Image3D<PixelType>* InputImage);

	void SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList);
	void SetCandidateOriginListOf3DContinuousIndex(const DenseMatrix<ScalarType>* Input3DPositionList);

	void SetMaskList(const ObjectArray<MaskType>* InputMaskList);

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

	ObjectArray<EvaluationInfoType>& OutputResult();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postrocess() {}
	virtual void BuildMask() {}

	bool WhetherMaskIsInsideImage_AtOrigin_3DContinuousIndex(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);
	bool WhetherMaskIsInsideImage_AtOrigin_3DPyhsicalPosition(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);

	// Evaluate candidate object at Origin (x, y, z), x/y/z is 3DContinuousIndex
	inline virtual void EvaluateCandidateAtOrigin_3DContinuousIndex(EvaluationInfoType& EvaluationInfo, int_max ThreadIndex) = 0;

	// Evaluate candidate object at Origin (x, y, z), x/y/z is 3DPyhsicalPosition
	inline virtual void EvaluateCandidateAtOrigin_3DPyhsicalPosition(EvaluationInfoType& EvaluationInfo, int_max ThreadIndex) = 0;

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