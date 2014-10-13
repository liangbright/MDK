#ifndef __mdkPartBasedObjectDetectionInSingleImage3D_h
#define __mdkPartBasedObjectDetectionInSingleImage3D_h

#include "mdkProcessObject.h"
#include "mdkDataArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

// One ObjectClass => many ObjectMask for the ObjectClass
// two ObjectClass may share the same ObjectMask
// ObjectClass <=> ObjectMask: many <=> many
//

// ObjectMask is only for reference
/*
template<typename ScalarType = double>
struct ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D
{
	bool Flag_PhysicalPosition;
	// true:  use physical position in PointSet
	// false: use 3D index in PointSet

	DenseMatrix<ScalarType> PointSet;
	// each column is the coordinate of a point
	// use 3D physical Position or 3D index
	// Point_in_PointSet = RotationMatrix*ScaleMatrix*Point_in_the_Object
	// Point_in_PointSet is a point in input image coordinate system (origin is the origin of the object)
	// Point_in_the_Object is a point in the Object Coordinate System (i.e., the databank coordinate system)

	DataArray<DenseVector<int_max>> PointSubSetList;
	// PointSubSetList[k]: { ColIndex in PointSet}, k is index of PointSubSet
	// PointSubSet can overlap each other

	DenseVector<ScalarType> PartOriginList;
	// number of parts = PartOriginList.GetLength()
	// PartOriginList(:, k): origin of the part k
	// each Origin is a point in input image coordinate system (origin is the origin of the object)
	// Origin = RotationMatrix*ScaleMatrix*Origin_in_the_Object_Coordinate_System

	DenseVector<ScalarType, 3> Angle;
	// Angle[0]: angle about axes x
	// Angle[1]: angle about axes y
	// Angle[2]: angle about axes z

	DenseVector<ScalarType, 3> Scale;
	// Scale[0]: Scale about axes x
	// Scale[1]: Scale about axes y
	// Scale[2]: Scale about axes z

	// Note:
	// the coordinate of every point in mask is in world coordinate system with origin equal to the origin of the object coordinate system
	// so, no need to compute the transform (RotationMatrix*ScaleMatrix) of each point during object evaluation at each possible location/origin
	// Angle and Scale for documentation purpose only

//---------------------------------------------------------------------------------------------
	ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D() {}
	~ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D() {}

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
*/

template<typename ObjectImagePixel_Type, typename ObjectPartMembershipImagePixel_Type, typename ObjectMembershipImagePixel_Type, typename Scalar_Type = double>
class PartBasedObjectDetectionInSingleImage3D : public ProcessObject
{
public:
	typedef ObjectImagePixel_Type								 ObjectImagePixelType;
	typedef ObjectPartMembershipImagePixel_Type					 ObjectPartMembershipImagePixelType;
	typedef ObjectMembershipImagePixel_Type						 ObjectMembershipImagePixelType;
	typedef Scalar_Type											 ScalarType;

	typedef Option_Of_Image3DInterpolation<ObjectImagePixelType> ObjectImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation                   ObjectImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation           ObjectImageInterpolationBoundaryOptionEnum;

protected:
	//-------------------------- input --------------------------------------------------//

	const Image3D<ObjectImagePixelType>* m_ObjectImage;

	ObjectImageInterpolationOptionType m_ObjectImageInterpolationOption;

	int_max m_ObjectPartNumber;

	const DataArray<DenseImage3D<ObjectPartMembershipImagePixelType>*> m_ObjectPartMembershipDenseImageList;

	const DataArray<SparseImage3D<ObjectPartMembershipImagePixelType>*> m_ObjectPartMembershipSparseImageList;

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DPyhsicalPosition;   // evaluate object candidate at each Origin

	DenseMatrix<int_max> m_CandidateOriginList_3DIndex;                        // evaluate object candidate at each Origin

	int_max m_MaxNumberOfThread; // max number of threads

	//------------------------- internal data -------------------------------------------//
	int_max m_TotalCandidateOriginNumber;
	bool m_Flag_ScanWholeImageGrid;
	
	bool m_Flag_StoreObjectMembershipInSparseImage;
	// true: use m_ObjectMembershipSparseImage
	// false: use m_ObjectMembershipImage
	//-----------------------------------------------------------------------------------//

	// output_0:
	DenseImage3D<ObjectMembershipImagePixelType> m_ObjectMembershipDenseImage;

	// output_1:
	SparseImage3D<ObjectMembershipImagePixelType> m_ObjectMembershipSparseImage;

protected:
	PartBasedObjectDetectionInSingleImage3D();
	virtual ~PartBasedObjectDetectionInSingleImage3D();

public:
	virtual void Clear();
	virtual void ClearOutput();

	void SetObjectImage(const Image3D<ObjectImagePixelType>* ObjectImage);
	
	void SetObjectImageInterpolationOption(const ObjectImageInterpolationOptionType& InputOption);

	ObjectImageInterpolationOptionType GetObjectImageInterpolationOption();

	void SetupObjectMembershipDenseImage(const DenseVector<ScalarType, 3>& Origin,
										 const DenseVector<ScalarType, 3>& Spacing,
										 const DenseVector<ScalarType, 3>& Size);

	void SetupObjectMembershipSparseImage(const DenseVector<ScalarType, 3>& Origin,
										  const DenseVector<ScalarType, 3>& Spacing,
										  const DenseVector<ScalarType, 3>& Size);

	void SetObjectPartNumber(int_max PartNumber);
	void SetObjectPartMembershipDenseImage(int_max PartIndex, const DenseImage3D<ObjectPartMembershipImagePixelType>* MembershipImage);
	void SetObjectPartMembershipSparseImage(int_max PartIndex, const SparseImage3D<ObjectPartMembershipImagePixelType>* MembershipImage);

	void SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList);
	void SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList);

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

	DenseImage3D<ObjectMembershipImagePixelType>* GetObjectMembershipDenseImage();

	SparseImage3D<ObjectMembershipImagePixelType>* GetObjectMembershipSparseImage();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postrocess() { return true; }

	// Evaluate candidate object at Origin (x0, y0, z0): 3DIndex
	inline virtual void EvaluateCandidateAtOriginOf3DIndex(int_max x0, int_max y0, int_max z0, int_max ThreadIndex) = 0;

	template<typename PixelTypeForMask = ObjectImagePixelType>
	DataArray<PixelTypeForMask> GetObjectImagePixelSetByPointMaskOf3DIndex_AtOriginOf3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = ObjectImagePixelType>
	DataArray<PixelTypeForMask> GetObjectImagePixelSetByPointMaskOf3DPyhsicalPosition_AtOriginOf3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	int_max GetNumberOfThreadTobeCreated();

private:
	void EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex);

private:
	PartBasedObjectDetectionInSingleImage3D(const PartBasedObjectDetectionInSingleImage3D&) = delete;
	void operator=(const PartBasedObjectDetectionInSingleImage3D&) = delete;
};

}// namespace mdk

#include "mdkPartBasedObjectDetectionInSingleImage3D.hpp"

#endif