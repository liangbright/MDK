﻿#pragma once

#include <memory>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkImageInterpolation3D.h"

// must be the last to include
#include "mdkImageInfo3D.h" 

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D DenseImage Class
// Pixel is Scalar or vector
// Lx: number of Pixel in x-direction
// Ly: number of Pixel in y-direction
// Lz: number of Pixel in z-direction
//
// Spacing (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm same resolution in x, y, z directions 
// same resolution => DenseImage filters do not need to be re-sampled with different Pixel shape
// reslice the input DenseImage using ITK/VTK, then feed it into this class.
//
//
// ok to store DenseImage with non-isotropic Pixel
// but such DenseImage is not good for analysis
// if the size is 0.5/0.5/6, then it is really bad and useless for 3D analysis
//
// ITK can register DenseImages with non-isotropic Pixel
//
// note: 
// use std::array as PixelType if Pixel is a  with known length, and do not use std::vector
//
//------------------- coordinate system -----------------------------------------------------
// 3DIndex:            Pixel index in image space, example:				   (0,0,0),                         (1,1,1)
// 3DPosition:         Pixel Physical Position in image space			   (0,0,0),                         (spacing_x,spacing_y,spacing_z) 
// 3DWorldPosition:    Pixel physical position in world coordinate system  (Origin[0],Origin[1],Origin[2]), (x,y,z)
// --------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_DenseImage3D_Operator_CheckBound
#endif
//------------------------------------------------------

//===================================================================================================================//	
//--------------------------------------------------- DenseImageData3D struct --------------------------------------------//

template<typename PixelType>
struct DenseImageData3D
{
	ImageInfo3D m_Info;

	ObjectArray<PixelType> m_PixelArray;

	PixelType m_Pixel_OutsideImage;
//----------------------------------------------------------------

    DenseImageData3D();

    ~DenseImageData3D();

    inline void Clear();

    //---------------------------------

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

    inline PixelType& operator()(int_max LinearIndex);

    inline const PixelType& operator()(int_max LinearIndex) const;
   
    inline PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

    inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DIndex(int_max LinearIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DPosition(int_max LinearIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const;

	//3DIndex is discrete
	inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	// 3DIndex may be continuous
	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z) const;

private:
//deleted:
    DenseImageData3D(const DenseImageData3D&) = delete;
    DenseImageData3D(DenseImageData3D&&) = delete;
    void operator=(const DenseImageData3D&) = delete;
    void operator=(DenseImageData3D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class DenseImage3D : public Object
{
public:
	typedef Pixel_Type PixelType;

	typedef Option_Of_Image3DInterpolation<PixelType>    InterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation   InterpolationBoundaryOptionEnum;

protected:
    std::shared_ptr<DenseImageData3D<PixelType>> m_ImageData;

public:
    DenseImage3D();
    DenseImage3D(const DenseImage3D& InputImage);
    DenseImage3D(DenseImage3D&& InputImage);
    ~DenseImage3D();
    void operator=(const DenseImage3D& InputImage);
    void operator=(DenseImage3D&& InputImage);
    //---------------------------------------------------------//

    // Copy can be used to convert an DenseImage from double (Type_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const DenseImage3D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	void CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelCount);

	void Copy(DenseImage3D&& InputImage);

    inline void Fill(const PixelType& Pixel);

    //---------------------------------------------------------//
    void Share(DenseImage3D& InputImage);
	void ForceShare(const DenseImage3D& InputImage);
	
	//special share, after this, size can not be changed
	void Share(PixelType* InputImage, const ImageInfo3D& InputImageInfo);
	void ForceShare(const PixelType* InputImage, const ImageInfo3D& InputImageInfo);

    //-----------------------------------------------------------------//
	// difficult to add Load / Save as member function
	// bool Load(const String& FilePathAndName);
	// bool Save(const String& FilePathAndName);
    // because no such thing as
	//		if (PixelType == double) Load_double
	//		if (PixelType == DenseVector<double>) Load_DenseVector
	// unless std::enable_if to conditionally compile a member function
    // specialization ?

	//---------------------------------------------------------//
	void ReCreate();
	void Clear();
    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

	inline bool IsPixelDataInInternalArray() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

	inline ImageInfo3D GetInfo() const;

	inline void SetInfo(const ImageInfo3D& Info, bool Flag_AllocateMemory = true);

	inline void AllocateMemory(); // the same as SetSize(this->GetSize());

	inline DenseVector<int_max, 3> GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const;

	inline void SetSize(const DenseVector<int_max, 3>& Size);// Memory is allocated inside the function

	inline void SetSize(int_max Lx, int_max Ly, int_max Lz);// Memory is allocated inside the function

	inline DenseVector<double, 3> GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const;

	inline void SetSpacing(const DenseVector<double, 3>& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z);

	inline DenseVector<double, 3> GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const;

	inline void SetOrigin(const DenseVector<double, 3>& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y, double Origin_z);

    inline DenseMatrix<double> GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

	inline DenseMatrix<double> GetTransformMatrix_3DIndexTo3DWorld() const;

	inline DenseMatrix<double> GetTransformMatrix_3DWorldTo3DIndex() const;

	inline DenseVector<double, 3> GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const;

    inline int_max GetPixelCount() const;

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();
    inline const PixelType* GetPixelPointer() const;

	inline PixelType* GetPixelPointerOfZSlice(int_max ZSliceIndex);
	inline const PixelType* GetPixelPointerOfZSlice(int_max ZSliceIndex) const;

	inline PixelType* begin();
	inline const PixelType* begin() const;

	inline PixelType* end();
	inline const PixelType* end() const;

    //------------------------ LinearIndex, 3DIndex and 3DWorldPosition ------------------------------------------------------------------------//
	// 3DIndex can be continuous or discrete

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DIndex(int_max LinearIndex) const;

	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DPosition(int_max LinearIndex) const;

	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const;

	inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline int_max Transform3DIndexToLinearIndex(const DenseVector<int_max, 3>& Index3D) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPosition(const DenseVector<ScalarType_Index, 3>& Index3D) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DWorldPosition(const DenseVector<ScalarType_Index, 3>& Index3D) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DIndex(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DPositionToNearest3DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DPositionToNearest3DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DPositionToNearest3DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DPositionToNearest3DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPositionTo3DWorldPosition(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DIndex(const DenseVector<ScalarType, 3>& Position) const;
	
	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DWorldPositionToNearest3DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DWorldPositionToNearest3DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DWorldPositionToNearest3DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 3> Transform3DWorldPositionToNearest3DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const;
	
	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DWorldPositionTo3DPosition(const DenseVector<ScalarType, 3>& Position) const;

	//------------------- check if 3D Index is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf3DIndexIsInsideImage(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const;

	template<typename ScalarType>
	inline bool CheckIf3DIndexIsInsideImage(const DenseVector<ScalarType, 3>& Index3D) const;

	//------------------- check if 3DPosition is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf3DPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline bool CheckIf3DPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const;

	//------------------- check if 3DWorldPosition is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf3DWorldPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline bool CheckIf3DWorldPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const;

	//--------------------------- Set/Get Pixel  ------------------------------//

	inline void SetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, PixelType Pixel);

	inline void SetPixelAt3DIndex(DenseVector<int_max, 3> Index3D, PixelType Pixel);

	inline void SetPixelAtLinearIndex(int_max LinearIndex, PixelType Pixel);

	inline const PixelType& GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline const PixelType& GetPixelAt3DIndex(DenseVector<int_max, 3> Index3D) const;

	inline const PixelType& GetPixelAtLinearIndex(int_max LinearIndex) const;

	//--------------------------- Get/Set Pixel by using operator -------------//

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

	inline PixelType& operator()(int_max LinearIndex);

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	//------------------ get nearest pixel --------------------//

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DIndex(const DenseVector<ScalarType, 3>& Index3D) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DPosition(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DWorldPosition(const DenseVector<ScalarType, 3>& Position) const;

	//------------------ get Pixel by using interpolation method --------------------//

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	PixelType GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt3DIndex(int xIndex, int yIndex, int zIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt3DIndex(long xIndex, long yIndex, long zIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DIndex(const DenseVector<ScalarType, 3>& Index3D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	PixelType GetPixelAt3DIndex(const DenseVector<int_max, 3>& Index3D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt3DIndex(const DenseVector<int, 3>& Index3D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt3DIndex(const DenseVector<long, 3>& Index3D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DWorldPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DWorldPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	//------------------------- Get LinearIndex or 3DIndex In Region -------------------//
    
	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf3DIndexInImage3D& RegionInfo) const;

	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf3DPositionInImage3D& RegionInfo) const;

	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf3DWorldPositionInImage3D& RegionInfo) const;
    
	DenseMatrix<int_max> Get3DIndexListInRegion(const BoxRegionOf3DIndexInImage3D& RegionInfo) const;

	DenseMatrix<int_max> Get3DIndexListInRegion(const BoxRegionOf3DPositionInImage3D& RegionInfo) const;

	DenseMatrix<int_max> Get3DIndexListInRegion(const BoxRegionOf3DWorldPositionInImage3D& RegionInfo) const;

    //-------------------------- Get SubImage -------------------------------//

	DenseImage3D GetSubImage(const BoxRegionOf3DIndexInImage3D& RegionInfo) const;

	DenseImage3D GetSubImage(const BoxRegionOf3DPositionInImage3D& RegionInfo) const;

	DenseImage3D GetSubImage(const BoxRegionOf3DWorldPositionInImage3D& RegionInfo) const;

    //-------------------------- Pad, UnPad (3DIndex) -------------------------------//

    DenseImage3D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    DenseImage3D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    DenseImage3D  UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

};


}//end namespace mdk

#include "mdkDenseImage3D.hpp"
