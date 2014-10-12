#ifndef __mdkImage3D_h
#define __mdkImage3D_h

#include <vector>
#include <memory>
#include <cstdlib>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkImageInterpolation3D.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Image Class
// Pixel is Scalar or vector
// Lx: number of Pixel in x-direction
// Ly: number of Pixel in y-direction
// Lz: number of Pixel in z-direction
//
// set Lz = 1, it can be used as 2D Image Class
//
// PixelPhysicalSize (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
// same resolution => image filters do not need to be re-sampled with different Pixel shape
// reslice the input image using ITK/VTK, then feed it into this class.
//
// ok to store image with non-isotropic Pixel
// but such image is not good for analysis
// if the size is 0.5/0.5/6, then it is really bad and useless for 3D analysis
//
// ITK can register images with non-isotropic Pixel
//
// note: 
// use std::array as PixelType if Pixel is a  with known length, and do not use std::
//
// --------------------------------------------------------------------------------------------------------//
//
// ImageOrientation is a 3x3 double DenseMatrix

//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_3DImage_Operator_CheckBound
#endif
//------------------------------------------------------

struct Image3DBoxRegionOf3DIndex
{
    bool IsEmpty;

    int_max x0;
    int_max y0;
    int_max z0;

    int_max x1;
    int_max y1;
    int_max z1;

//-------------------------------------
    Image3DBoxRegionOf3DIndex()
    {
        IsEmpty = true;
        x0 = 0;
        y0 = 0;
        z0 = 0;
        x1 = 0;
        y1 = 0;
        z1 = 0;
    };

    int_max Lx() const
    {
        return x1 - x0 + 1;
    }

    int_max Ly() const
    {
        return y1 - y0 + 1;
    }

    int_max Lz() const
    {
        return z1 - z0 + 1;
    }
};

template<typename ScalarType = double>
struct Image3DBoxRegionOf3DPhysicalPosition
{
    bool IsEmpty;

	ScalarType x0;
	ScalarType y0;
	ScalarType z0;

	ScalarType x1;
	ScalarType y1;
	ScalarType z1;

//-------------------------------------

    Image3DBoxRegionOf3DPhysicalPosition()
    {
        IsEmpty = true;
        x0 = 0;
        y0 = 0;
        z0 = 0;
        x1 = 0;
        y1 = 0;
        z1 = 0;
    };

	ScalarType Lx() const
    {
        return x1 - x0;
    }

	ScalarType Ly() const
    {
        return y1 - y0;
    }

	ScalarType Lz() const
    {
        return z1 - z0;
    }
};


struct Image3DInfo
{
	DenseVector<int_max, 3> Size;       // {Lx, Ly, Lz} number of Pixels in each direction
	DenseVector<double, 3>  Spacing;    // Pixel Spacing of DICOM image in world coordinate system {Sx, Sy, Sz} (unit: mm)
	DenseVector<double, 3>  Origin;     // Origin of DICOM image in world coordinate system (x,y,z) (unit: mm)
	DenseMatrix<double> m_Orientation;  // 3x3 Matrix
};

//===================================================================================================================//
//--------------------------------------------------- ImageData3D struct --------------------------------------------//

template<typename PixelType>
struct ImageData3D
{
	DenseVector<int_max, 3> m_Size;     // {Lx, Ly, Lz} number of Pixels in each direction

    int_max m_PixelNumberPerZSlice;     // total number of Pixels in each z-slice  = m_Size[2]*m_Size[1]

	DenseVector<double, 3> m_Spacing;   // Pixel Spacing of DICOM image in world coordinate system {Sx, Sy, Sz} (unit: mm)

	DenseVector<double, 3> m_Origin;    // Origin of DICOM image in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 3x3 Matrix

    std::vector<PixelType> m_DataArray;

    PixelType m_NaNPixel;

	Option_Of_Image3DInterpolation<PixelType> InterpolationOption;
//----------------------------------------------------------------

    ImageData3D();

    ~ImageData3D();

    inline void Clear();

    //---------------------------------

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

    inline PixelType& operator()(int_max LinearIndex);

    inline const PixelType& operator()(int_max LinearIndex) const;
   
    inline PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

    inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex) const;
	
	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform3DIndexTo3DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform3DPhysicalPositionTo3DIndex(ScalarType x, ScalarType y, ScalarType z) const;

private:
//deleted:
    ImageData3D(const ImageData3D&) = delete;
    ImageData3D(ImageData3D&&) = delete;
    void operator=(const ImageData3D&) = delete;
    void operator=(ImageData3D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class Image3D : public Object
{
public:
	typedef Pixel_Type PixelType;

protected:

    std::shared_ptr<ImageData3D<PixelType>> m_ImageData;

    PixelType* m_PixelPointer; // keep tracking m_ImageData->m_DataArray.data();

public:
    Image3D();

    Image3D(const Image3D& InputImage);

    Image3D(Image3D&& InputImage);

    ~Image3D();

    void operator=(const Image3D& InputImage);

    void operator=(Image3D&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (Type_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const Image3D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	bool Copy(const Image3D<PixelType_Input>* InputImage);

    inline bool Fill(const PixelType& Pixel);

    //---------------------------------------------------------//
    bool Share(Image3D& InputImage);
    bool Share(Image3D* InputImage);

    void ForceShare(const Image3D& InputImage);
    bool ForceShare(const Image3D* InputImage);

    //---------------------------------------------------------//
    void Take(Image3D&& InputImage);
    void Take(Image3D& InputImage);
    bool Take(Image3D* InputImage);

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

    inline int_max GetPixelNumber() const;

	template<typename PixelType_Input>
	bool CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber);

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();
    inline const PixelType* GetPixelPointer() const;

	inline PixelType* begin();
	inline const PixelType* begin() const;

	inline PixelType* end();
	inline const PixelType* end() const;

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

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

	inline PixelType& operator()(int_max LinearIndex);

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline PixelType& at(int_max LinearIndex);

	inline const PixelType& at(int_max LinearIndex) const;

	inline PixelType& at(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const PixelType& at(int_max xIndex, int_max yIndex, int_max zIndex) const;

	//------------------ get Pixel by using interpolation method --------------------//

	const Option_Of_Image3DInterpolation<PixelType>& GetInterpolationOption() const;

	void SetInterpolationOption(const Option_Of_Image3DInterpolation<PixelType>& Option);

	template<typename ScalarType>
	PixelType GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, const Option_Of_Image3DInterpolation<PixelType>& Option) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position, const Option_Of_Image3DInterpolation<PixelType>& Option) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const;

	PixelType GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;
	PixelType GetPixelAt3DIndex(int xIndex, int yIndex, int zIndex) const;
	PixelType GetPixelAt3DIndex(long xIndex, long yIndex, long zIndex) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DIndex(const DenseVector<ScalarType, 3>& Index3D) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const Option_Of_Image3DInterpolation<PixelType>& Option) const;

	PixelType GetPixelAt3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const Option_Of_Image3DInterpolation<PixelType>& Option) const;
	PixelType GetPixelAt3DIndex(int xIndex, int yIndex, int zIndex, const Option_Of_Image3DInterpolation<PixelType>& Option) const;
	PixelType GetPixelAt3DIndex(long xIndex, long yIndex, long zIndex, const Option_Of_Image3DInterpolation<PixelType>& Option) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DIndex(const DenseVector<ScalarType, 3>& Index3D, const Option_Of_Image3DInterpolation<PixelType>& Option) const;

	//------------------------- Get LinearIndex In Region -------------------//

    DenseMatrix<int_max> GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
	                                                int_max yIndex_s,     int_max Region_Ly,
                                                    int_max zIndex_s,     int_max Region_Lz) const;
    
    //-------------------------- Get SubImage -------------------------------//

    Image3D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const;

    //-------------------------- Pad, UnPad -------------------------------//

    Image3D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    Image3D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    Image3D  UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

};


}//end namespace mdk

#include "mdkImage3D.hpp"

#endif