#ifndef __mdkSparseImage3D_h
#define __mdkSparseImage3D_h

#include <vector>
#include <memory>
#include <cstdlib>
#include <unordered_map>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkImageInterpolation3D.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Sparse Image Class
// used in SingleClassObjectDetectionInSingleImage3D
//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_3DSparseImage_Operator_CheckBound
#endif
//------------------------------------------------------

//===================================================================================================================//
//--------------------------------------------------- SparseImageData3D struct --------------------------------------------//

template<typename PixelType>
struct SparseImageData3D
{
	DenseVector<int_max, 3> m_Size;     // {Lx, Ly, Lz} number of Pixels in each direction

    int_max m_PixelNumberPerZSlice;     // total number of Pixels in each z-slice  = m_Size[2]*m_Size[1]

	DenseVector<double, 3> m_Spacing;   // Pixel Spacing of DICOM image in world coordinate system {Sx, Sy, Sz} (unit: mm)

	DenseVector<double, 3> m_Origin;    // Origin of DICOM image in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 3x3 Matrix

	std::unordered_map<int_max, PixelType> m_DataMap; // map LinearIndex to Pixel

	PixelType m_Pixel_OutsideImage;

//----------------------------------------------------------------

	SparseImageData3D();

	~SparseImageData3D();

    inline void Clear();

    //---------------------------------

	inline const PixelType& GetPixel(int_max LinearIndex) const;
   
	inline const PixelType& GetPixel(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline void SetPixel(int_max LinearIndex, const PixelType& Pixel);

	inline void SetPixel(int_max xIndex, int_max yIndex, int_max zIndex, const PixelType& Pixel);

	//---------------------------------

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const;
	
	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

private:
//deleted:
	SparseImageData3D(const SparseImageData3D&) = delete;
	SparseImageData3D(SparseImageData3D&&) = delete;
	void operator=(const SparseImageData3D&) = delete;
	void operator=(SparseImageData3D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class SparseImage3D : public Object
{
public:
	typedef Pixel_Type PixelType;

	typedef Option_Of_Image3DInterpolation<PixelType>    InterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation   InterpolationBoundaryOptionEnum;

protected:
    std::shared_ptr<SparseImageData3D<PixelType>> m_ImageData;

public:
    SparseImage3D();

    SparseImage3D(const SparseImage3D& InputImage);

    SparseImage3D(SparseImage3D&& InputImage);

    ~SparseImage3D();

    void operator=(const SparseImage3D& InputImage);

    void operator=(SparseImage3D&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (Type_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const SparseImage3D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	bool Copy(const SparseImage3D<PixelType_Input>* InputImage);

	template<typename PixelType_Input>
	void CopyPixelData(const SparseImage3D<PixelType_Input>& InputImage);

    //---------------------------------------------------------//
    bool Share(SparseImage3D& InputImage);
    bool Share(SparseImage3D* InputImage);

    void ForceShare(const SparseImage3D& InputImage);
    bool ForceShare(const SparseImage3D* InputImage);

    //---------------------------------------------------------//
    void Take(SparseImage3D&& InputImage);
    void Take(SparseImage3D& InputImage);
    bool Take(SparseImage3D* InputImage);

    //-----------------------------------------------------------------//

    inline bool IsEmpty() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

	inline DenseVector<int_max, 3> GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const;

	inline bool SetSize(const DenseVector<int_max, 3>& Size);

    inline bool SetSize(int_max Lx, int_max Ly, int_max Lz);

	inline DenseVector<double, 3> GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const;

	inline void SetSpacing(const DenseVector<double, 3>& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z);

	inline DenseVector<double, 3> GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const;

	inline void SetOrigin(const DenseVector<double, 3>& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y, double Origin_z);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

	inline DenseVector<double, 3> GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const;

    inline int_max GetRecordedPixelNumber() const;

    //------------------------ LinearIndex, 3DIndex and 3DPhyscialPosition ------------------------------------------------------------------------//
	// 3DIndex can be continuous or discrete

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline int_max Transform3DIndexToLinearIndex(const DenseVector<int_max, 3>& Index3D) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPhysicalPosition(const DenseVector<ScalarType_Index, 3>& Index3D) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPhysicalPositionTo3DIndex(const DenseVector<ScalarType, 3>& Position) const;

	//--------------------------- Get/Set Pixel ------------------------------//

	inline const PixelType& operator[](int_max LinearIndex) const;

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline const PixelType& GetPixelNearestTo3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline const PixelType& GetPixelNearestTo3DIndex(const DenseVector<int_max, 3>& Index3D) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position) const;

	inline bool SetPixelAtLinearIndex(int_max LinearIndex, const PixelType& Pixel);

	inline bool SetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const PixelType& Pixel);

};


}//end namespace mdk

#include "mdkSparseImage3D.hpp"

#endif