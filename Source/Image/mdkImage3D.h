#ifndef __mdkImage3D_h
#define __mdkImage3D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImageConfig.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Image Class
// Pixel is Scalar or vector
// Lx: number of Pixels in x-direction
// Ly: number of Pixels in y-direction
// Lz: number of Pixels in z-direction
//
// set Lz = 1, it can be used as 2D Image Class
//
// PixelPhysicalSize (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
// same resolution => image filters do not need to be re-sampled with different Pixel shape
// reslice the input image using ITK/VTK, then feed it into this class.
//
// ok to store image with non-isotropic Pixels
// but such image is not good for analysis
// if the size is 0.5/0.5/6, then it is really bad and useless for 3D analysis
//
// ITK can register images with non-isotropic Pixels
//
// note: 
// use std::array as PixelType if Pixel is a  with known length, and do not use std::
//
// --------------------------------------------------------------------------------------------------------//

struct ImageSize3D
{
	int_max Lx;
	int_max Ly;
	int_max Lz;
};

struct ImageSpacing3D
{
    double Sx;
    double Sy;
    double Sz;
};

struct ImagePhysicalSize3D
{
    double Lx;
    double Ly;
    double Lz;
};

struct ImageOrigin3D
{
    double x;
    double y;
    double z;
};

// ImageOrientation is a 3x3 double DenseMatrix

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


struct Image3DBoxRegionOf3DPhysicalPosition
{
    bool IsEmpty;

    double x0;
    double y0;
    double z0;

    double x1;
    double y1;
    double z1;

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

    double Lx() const
    {
        return x1 - x0;
    }

    double Ly() const
    {
        return y1 - y0;
    }

    double Lz() const
    {
        return z1 - z0;
    }
};

//===================================================================================================================//
//--------------------------------------------------- ImageData3D struct --------------------------------------------//

template<typename PixelType>
struct ImageData3D
{
    int_max m_Size[3];                  // {Lx, Ly, Lz} number of Pixels in each direction

    int_max m_PixelNumberPerZSlice;     // total number of Pixels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

    double m_Spacing[3];                // i.e., Pixel Spacing in DICOM image (ITK, VTK)

    double m_Origin[3];                // i.e., Origin in DICOM image (ITK, VTK) {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 3x3 Matrix

    std::vector<PixelType> m_DataArray;

    PixelType m_NaNPixel;
//-----------------------------------------------------------

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

    inline void TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const;

    inline void TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const;

    inline void Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const;

    inline void Transform3DPhysicalPositionTo3DContinuousIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const;

private:
//deleted:
    ImageData3D(const ImageData3D&) = delete;
    ImageData3D(ImageData3D&&) = delete;
    void operator=(const ImageData3D&) = delete;
    void operator=(ImageData3D&&) = delete;
};

//===================================================================================================================//

template<typename PixelType>
class Image3D : public Object
{
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

    template<typename Type_Input>
    void Copy(const Image3D<Type_Input>& InputImage);

    template<typename Type_Input>
    bool Copy(const Image3D<Type_Input>* InputImage);

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

    inline ImageSize3D GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const;

    inline bool SetSize(const ImageSize3D& Size);

    inline bool SetSize(int_max Lx, int_max Ly, int_max Lz);

    inline ImageSpacing3D GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const;

    inline void SetSpacing(const ImageSpacing3D& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z);

    inline ImageOrigin3D GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const;

    inline void SetOrigin(const ImageOrigin3D& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y, double Origin_z);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

    inline ImagePhysicalSize3D GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const;

    inline int_max GetPixelNumber() const;

    template<typename Type_Input>
    bool CopyData(const Type_Input* InputPixelPointer, int_max InputPixelNumber);

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();

    inline const PixelType* GetPixelPointer() const;

    // ------------------------ Index and Position ------------------------------------------------------------------------//

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline void TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const;

    inline void TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const;

    inline void Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const;

    inline void Transform3DPhysicalPositionTo3DContinuousIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const;

	//--------------------------- Get/Set Pixel      ------------------------------//

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