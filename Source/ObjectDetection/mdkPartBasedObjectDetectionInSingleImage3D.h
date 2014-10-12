#ifndef __mdkPartBasedObjectDetectionInSingleImage3D_h
#define __mdkPartBasedObjectDetectionInSingleImage3D_h

#include "mdkProcessObject.h"
#include "mdkSimpleObjectArray.h"
#include "mdkObjectArray.h"

namespace mdk
{

// One ObjectClass => many ObjectMask for the ObjectClass
// two ObjectClass may share the same ObjectMask
// ObjectClass <=> ObjectMask: many <=> many
//

template<typename ScalarType = double>
struct ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D
{
	DenseVector<int_max> ClassIndexList; 
	// ClassIndexList[k]: Index of object class that use this Mask

	DenseVector<int_max> MaskIndexInEachClass;
	// SubClassIndexList[k]: SubClassIndex in class #ClassIndexList[k]

	bool Flag_PhysicalPosition;
	// true:  use physical position in PointSet
	// false: use 3D index in PointSet

	DenseMatrix<ScalarType> PointSet;
	// each column is the coordinate of a point
	// use 3D physical Position or 3D index
	// Point_in_PointSet = RotationMatrix*ScaleMatrix*Point_in_the_Object
	// Point_in_PointSet is a point in input image coordinate system (origin is the origin of the object)
	// Point_in_the_Object is a point in the Object Coordinate System (i.e., the databank coordinate system)

	SimpleObjectArray<DenseVector<int_max>> PointSubSetList;
	// PointSubSetList[k]: { Index of point in PointSubSet k}, k is index of PointSubSet
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
	// the coordinate of every point in mask is in input image coordinate system with origin equal to the origin of the object coordinate system
	// so, no need to compute the transform (RotationMatrix*ScaleMatrix) of each point during object evaluation at each possible location/origin
	// Angle and Scale for documentation purpose only

//---------------------------------------------------------------------------------------------
	ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D() {}
	~ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D() {}

	void Clear()
	{
		ClassIndexList.Clear();
		SubClassIndexList.Clear();
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


template<typename ScalarType = double>
struct Element_Of_ObjectProbabilityImagePixel_Of_PartBasedObjectDetectionInSingleImage3D
{// Element k in ObjectProbabilityImagePixelType,
 // one Element <-> one Class
	ScalarType MaskIndex_in_ObjectMaskList;   // MaskIndex in m_ObjectMaskList
	ScalarType Probability;                   // Probability of being class k
};


template<typename Pixel_Type, typename Scalar_Type = double>
class PartBasedObjectDetectionInSingleImage3D : public ProcessObject
{
public:
	typedef Pixel_Type												               PixelType;
	typedef Scalar_Type												               ScalarType;
	typedef ObjectMask_Of_PartBasedObjectDetectionInSingleImage3D<ScalarType>	   ObjectMaskType;	
	typedef SparseVector<Element_Of_ObjectProbabilityImagePixel_Of_PartBasedObjectDetectionInSingleImage3D>  ObjectProbabilityImagePixelType;

protected:
	//-------------------------- input --------------------------------------------------//

	const Image3D<PixelType>* m_ObjectImage;

	int_max m_ObjectPartNumber;

	ObjectArray<PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>*> m_ObjectPartDetectorList;

	const Image3D<ObjectProbabilityImagePixelType>* m_ObjectPartProbabilityImage;

	const DenseMatrix<ScalarType>* m_CandidateOriginList_3DPyhsicalPosition;   // evaluate object candidate at each Origin

	DenseMatrix<int_max> m_CandidateOriginList_3DIndex;                        // evaluate object candidate at each Origin

	const ObjectArray<ObjectMaskType>* m_ObjectMaskList;

	int_max m_MaxNumberOfThread; // max number of threads

	//------------------------- internal data -------------------------------------------//

	ObjectArray<Image3DBoxRegionOf3DPhysicalPosition> m_NOBoundCheckRegionList_3DPyhsicalPosition;
	ObjectArray<Image3DBoxRegionOf3DIndex> m_NOBoundCheckRegionList_3DIndex;

	int_max m_TotalCandidateOriginNumber;
	bool m_Flag_ScanWholeImageGrid;
	
	bool m_Flag_Pipeline;
	//-----------------------------------------------------------------------------------//

	// output_0:
	Image3D<ObjectProbabilityImagePixelType> m_ObjectProbabilityImage;

	// about m_Output:
    // (1) use ObjectArray<SparseVector<ScalarType>>
	//     m_Output[n][k] is the score of object measured by using MaskList[k] at position (x_n, y_n, z_n)
	//     easy to locate each output, but not easy to sort them by score (and then select the top 100 candidates)
	// (2) use ObjectArray<EvaluationResult_Of_SingleClassObjectDetectionInSingleImage3D<ScalarType>>
	//    m_Output[m] is the info of the object measured by using a Mask
	//    not easy to locate output by position/index,  but easy to sort them by score

protected:
	PartBasedObjectDetectionInSingleImage3D();
	virtual ~PartBasedObjectDetectionInSingleImage3D();

public:
	virtual void Clear();
	virtual void ClearOutput();

	void SetObjectImage(const Image3D<PixelType>* ObjectImage);
	void SetObjectPartNumber(int_max PartNumber);
	
	void SetObjectPartDetector(int_max PartIndex, PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>* PartDetector);
	PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>* GetObjectPartDetector(int_max PartIndex);
	const ObjectArray<PartBasedObjectDetectionInSingleImage3D<PixelType, ScalarType>*>& GetObjectPartDetectorList();

	void SetObjectPartProbabilityImage(int_max PartIndex, const Image3D<SparseVector<ScalarType>>* PartProbabilityImage);

	void SetCandidateOriginListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* Input3DPositionList);
	void SetCandidateOriginListOf3DIndex(DenseMatrix<int_max> Input3DIndexList);

	void SetObjectMaskList(const ObjectArray<ObjectMaskType>* InputMaskList);
	const ObjectArray<ObjectMaskType>* GetObjectMaskList();

	void SetThreadNumber(int_max MaxNumber);

	void EnablePipeline();
	void DisablePipeline();

	virtual bool Update();

	Image3D<ObjectProbabilityImagePixelType>* GetObjectProbabilityImage();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postrocess() {}
	virtual void BuildMask() {}

	bool WhetherMaskIsInsideImage_AtOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);
	bool WhetherMaskIsInsideImage_AtOrigin_3DPyhsicalPosition(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex);

	// Evaluate candidate object at Origin (x0, y0, z0): 3DIndex
	inline virtual void EvaluateCandidateAtOrigin_3DIndex(ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) = 0;

	ObjectArray<PixelType> GetPixelSetByObjectMaskAtOrigin_3DIndex(const ObjectMaskType& Mask, ScalarType x0, ScalarType y0, ScalarType z0);

	ObjectArray<PixelType> GetPixelSetByObjectMaskAtOrigin_3DPyhsicalPosition(const ObjectMaskType& Mask, ScalarType x0, ScalarType y0, ScalarType z0);

	int_max GetNumberOfThreadTobeCreated();

private:
	void ComputeRegionOfNOBoundCheck_3DIndex();
	void ComputeRegionOfNOBoundCheck_3DPyhsicalPosition();

	void EvaluateCandidateAtMultipleOrigin_in_a_thread(int_max OriginIndex_start, int_max OriginIndex_end, int_max ThreadIndex);

private:
	PartBasedObjectDetectionInSingleImage3D(const PartBasedObjectDetectionInSingleImage3D&) = delete;
	void operator=(const PartBasedObjectDetectionInSingleImage3D&) = delete;
};

}// namespace mdk

#include "mdkPartBasedObjectDetectionInSingleImage3D.hpp"

#endif