#pragma once

#include <memory>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkObjectArray.h"
#include "mdkImageInterpolation2D.h"

// must be the last to include
#include "mdkImageInfo2D.h" 

namespace mdk
{
//---------------------------------------------------------------------------------------------------------------------------------//
// 2D DenseImage Class:  2D Image on a Flat Plane in 3D Space, e.g., 2D slice of 3D image
// Pixel is scalar or vector
// Lx: number of Pixel in x-direction of image space
// Ly: number of Pixel in y-direction of image space
//------------------- coordinate system -------------------------------------------------------------------------------------------
// 2DIndex:            Pixel index in image space, example:				   (0,0),                           (1,1)
// 2DPosition:         Pixel Physical Position in image space			   (0,0),                           (spacing_x,spacing_y) 
// 3DWorldPosition:    Pixel physical position in world coordinate system  (Origin[0],Origin[1],Origin[2]), (x,y,z)
// --------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_DenseImage2D
#endif
//------------------------------------------------------

//===================================================================================================================//	
//--------------------------------------------------- DenseImageData2D struct --------------------------------------------//

template<typename PixelType>
struct DenseImageData2D
{
	ImageInfo2D m_Info;

	ObjectArray<PixelType> m_PixelArray;

	PixelType m_Pixel_OutsideImage;
//----------------------------------------------------------------

    DenseImageData2D();

    ~DenseImageData2D();

    inline void Clear();

    //---------------------------------

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

    inline PixelType& operator()(int_max LinearIndex);

    inline const PixelType& operator()(int_max LinearIndex) const;
   
    inline PixelType& operator()(int_max xIndex, int_max yIndex);

    inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DPosition(int_max LinearIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const;

	//2DIndex is discrete
	inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

	// 2DIndex may be continuous
	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPositionTo2DIndex(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform2DPositionTo3DWorldPosition(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z) const;

private:
//deleted:
    DenseImageData2D(const DenseImageData2D&) = delete;
    DenseImageData2D(DenseImageData2D&&) = delete;
    void operator=(const DenseImageData2D&) = delete;
    void operator=(DenseImageData2D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class DenseImage2D : public Object
{
public:
	typedef Pixel_Type PixelType;

	typedef Option_Of_Image2DInterpolation<PixelType>    InterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation   InterpolationBoundaryOptionEnum;

protected:
    std::shared_ptr<DenseImageData2D<PixelType>> m_ImageData;

public:
    DenseImage2D();
    DenseImage2D(const DenseImage2D& InputImage);
    DenseImage2D(DenseImage2D&& InputImage);
    ~DenseImage2D();
    void operator=(const DenseImage2D& InputImage);
    void operator=(DenseImage2D&& InputImage);
    //---------------------------------------------------------//

    // Copy can be used to convert an DenseImage from double (Type_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const DenseImage2D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	void CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelCount);

	void Copy(DenseImage2D&& InputImage);

    inline void Fill(const PixelType& Pixel);

    //---------------------------------------------------------//
    void Share(DenseImage2D& InputImage);
	void ForceShare(const DenseImage2D& InputImage);
	
	//special share, after this, size can not be changed
	void Share(PixelType* InputImage, const ImageInfo2D& InputImageInfo);
	void ForceShare(const PixelType* InputImage, const ImageInfo2D& InputImageInfo);

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
	void Clear();

    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

	inline bool IsPixelDataInInternalArray() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

	inline ImageInfo2D GetInfo() const;

	inline void SetInfo(const ImageInfo2D& Info, bool Flag_AllocateMemory = true);

	inline void AllocateMemory(); // the same as SetSize(this->GetSize());

	inline DenseVector<int_max, 2> GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly) const;

	inline void SetSize(const DenseVector<int_max, 2>& Size);// Memory is allocated inside the function

	inline void SetSize(int_max Lx, int_max Ly);// Memory is allocated inside the function

	inline DenseVector<double, 2> GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y) const;

	inline void SetSpacing(const DenseVector<double, 2>& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y);

	inline DenseVector<double, 3> GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const;//origin in 3D

	inline void SetOrigin(const DenseVector<double, 3>& Origin);//origin in 3D

    inline void SetOrigin(double Origin_x, double Origin_y, double Origin_z);//origin in 3D

    inline DenseMatrix<double> GetOrientation() const;//orientation in 3D

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

	inline DenseMatrix<double> GetTransformMatrix_2DIndexTo3DWorld() const;

	inline DenseMatrix<double> GetTransformMatrix_3DWorldTo2DIndex() const;

	inline DenseVector<double, 2> GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const;

    inline int_max GetPixelCount() const;

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();
    inline const PixelType* GetPixelPointer() const;

	inline PixelType* begin();
	inline const PixelType* begin() const;

	inline PixelType* end();
	inline const PixelType* end() const;

    //------------------------ LinearIndex, 2DIndex and 3DWorldPosition ------------------------------------------------------------------------//
	// 2DIndex can be continuous or discrete

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;

	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DPosition(int_max LinearIndex) const;

	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DWorldPosition(int_max LinearIndex) const;

	inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

	inline int_max Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform2DIndexTo3DWorldPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform2DIndexTo3DWorldPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPositionTo2DIndex(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPositionTo2DIndex(const DenseVector<ScalarType, 2>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform2DPositionToNearest2DDiscreteIndex(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform2DPositionToNearest2DDiscreteIndex(const DenseVector<ScalarType, 2>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform2DPositionToNearest2DDiscreteIndexInsideImage(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform2DPositionToNearest2DDiscreteIndexInsideImage(const DenseVector<ScalarType, 2>& Position) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform2DPositionTo3DWorldPosition(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform2DPositionTo3DWorldPosition(const DenseVector<ScalarType, 2>& Position) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DIndex(const DenseVector<ScalarType, 3>& Position) const;
	
	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform3DWorldPositionToNearest2DDiscreteIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform3DWorldPositionToNearest2DDiscreteIndex(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform3DWorldPositionToNearest2DDiscreteIndexInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<int_max, 2> Transform3DWorldPositionToNearest2DDiscreteIndexInsideImage(const DenseVector<ScalarType, 3>& Position) const;
	
	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform3DWorldPositionTo2DPosition(const DenseVector<ScalarType, 3>& Position) const;

	//------------------- check if 3D Index is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf2DIndexIsInsideImage(ScalarType xIndex, ScalarType yIndex) const;

	template<typename ScalarType>
	inline bool CheckIf2DIndexIsInsideImage(const DenseVector<ScalarType, 2>& Index2D) const;

	//------------------- check if 2DPosition is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf2DPositionIsInsideImage(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline bool CheckIf2DPositionIsInsideImage(const DenseVector<ScalarType, 2>& Position) const;

	//------------------- check if 3DWorldPosition is inside Image ---------------------//
	template<typename ScalarType>
	inline bool CheckIf3DWorldPositionIsInsideImage(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline bool CheckIf3DWorldPositionIsInsideImage(const DenseVector<ScalarType, 3>& Position) const;

	//--------------------------- Set/Get Pixel  ------------------------------//

	inline void SetPixelAt2DIndex(int_max xIndex, int_max yIndex, PixelType Pixel);

	inline void SetPixelAt2DIndex(DenseVector<int_max, 2> Index2D, PixelType Pixel);

	inline void SetPixelAtLinearIndex(int_max LinearIndex, PixelType Pixel);

	inline const PixelType& GetPixelAt2DIndex(int_max xIndex, int_max yIndex) const;

	inline const PixelType& GetPixelAt2DIndex(DenseVector<int_max, 2> Index2D) const;

	inline const PixelType& GetPixelAtLinearIndex(int_max LinearIndex) const;

	//--------------------------- Get/Set Pixel by using operator -------------//

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

	inline PixelType& operator()(int_max LinearIndex);

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline PixelType& operator()(int_max xIndex, int_max yIndex);

	inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

	//------------------ get nearest pixel --------------------//

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(ScalarType xIndex, ScalarType yIndex) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(const DenseVector<ScalarType, 2>& Index2D) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPosition(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPosition(const DenseVector<ScalarType, 2>& Position) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DWorldPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DWorldPosition(const DenseVector<ScalarType, 3>& Position) const;

	//------------------ get Pixel by using interpolation method --------------------//

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	PixelType GetPixelAt2DIndex(int_max xIndex, int_max yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt2DIndex(int xIndex, int yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt2DIndex(long xIndex, long yIndex, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(const DenseVector<ScalarType, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	PixelType GetPixelAt2DIndex(const DenseVector<int_max, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt2DIndex(const DenseVector<int, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;
	PixelType GetPixelAt2DIndex(const DenseVector<long, 2>& Index2D, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPosition(ScalarType x, ScalarType y, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPosition(const DenseVector<ScalarType, 2>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DWorldPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt3DWorldPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	//------------------------- Get LinearIndex or 2DIndex In Region -------------------//
    
	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf2DIndexInImage2D& RegionInfo) const;

	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf2DPositionInImage2D& RegionInfo) const;

	DenseVector<int_max> GetLinearIndexListInRegion(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const;
    
	DenseMatrix<int_max> Get2DIndexListInRegion(const BoxRegionOf2DIndexInImage2D& RegionInfo) const;

	DenseMatrix<int_max> Get2DIndexListInRegion(const BoxRegionOf2DPositionInImage2D& RegionInfo) const;

	DenseMatrix<int_max> Get2DIndexListInRegion(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const;

    //-------------------------- Get SubImage -------------------------------//

	DenseImage2D GetSubImage(const BoxRegionOf2DIndexInImage2D& RegionInfo) const;

	DenseImage2D GetSubImage(const BoxRegionOf2DPositionInImage2D& RegionInfo) const;

	DenseImage2D GetSubImage(const BoxRegionOf3DWorldPositionInImage2D& RegionInfo) const;

    //-------------------------- Pad, UnPad (2DIndex) -------------------------------//

    DenseImage2D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const;

    DenseImage2D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const;

    DenseImage2D  UnPad(int_max Pad_Lx, int_max Pad_Ly) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

};


}//end namespace mdk

#include "mdkDenseImage2D.hpp"
