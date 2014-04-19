#ifndef __mdkImage_h
#define __mdkImage_h

#include <vector>
#include <memory>
#include <cstdlib>

#include "mdkObject.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 3D Image Class
// Voxel = an array of scalars
// Lx: number of voxels in x-direction
// Ly: number of voxels in y-direction
// Lz: number of voxels in z-direction
//
// set Lz = 1, it can be used as 2D Image Class
//
// VoxelPhysicalSize (unit: mm), e.g., 0.5mm x 0.5mm x 0.5mm,  same resolution in x, y, z directions 
// same resolution => image filters do not need to be re-sampled with different voxel shape
// reslice the input image using ITK/VTK, then feed it into this class.
//
// ok to store image with non-isotropic voxels
// but such image is not good for analysis
// if the size is 0.5/0.5/6, then it is really bad and useless for 3D analysis
//
// ITK can register images with non-isotropic voxels
//
// note: 
// use std::array as VoxelType if voxel is a vector with known length, and do not use std::vector
//
// --------------------------------------------------------------------------------------------------------//

struct ImageSize
{
	int_max Lx;
	int_max Ly;
	int_max Lz;
};

struct ImageVoxelSpacing
{
    double Sx;
    double Sy;
    double Sz;
};

struct ImagePhysicalSize
{
    double Lx;
    double Ly;
    double Lz;
};

struct ImagePhysicalOrigin
{
    double x;
    double y;
    double z;
};

// ImageOrientation is a 3x3 double DenseMatrix

struct ImageBoxRegionOf3DIndex
{
    bool IsEmpty;

    int_max x0;
    int_max y0;
    int_max z0;

    int_max x1;
    int_max y1;
    int_max z1;

//-------------------------------------
    ImageBoxRegionOf3DIndex()
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


struct ImageBoxRegionOf3DPhysicalPosition
{
    bool IsEmpty;

    double x0;
    double y0;
    double z0;

    double x1;
    double y1;
    double z1;

//-------------------------------------

    ImageBoxRegionOf3DPhysicalPosition()
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
//--------------------------------------------------- ImageData struct --------------------------------------------//

template<typename VoxelType>
struct ImageData
{
    int_max m_Size[3];                  // {Lx, Ly, Lz} number of voxels in each direction

    int_max m_VoxelNumberPerZSlice;     // total number of voxels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

    double m_VoxelSpacing[3];           // i.e., Spacing in DICOM image (ITK, VTK)

    double m_PhysicalOrigin[3];         // i.e., Origin in DICOM image (ITK, VTK) {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 3x3 Matrix

    std::vector<VoxelType> m_DataArray;

    VoxelType m_NaNVoxel;
//-----------------------------------------------------------

    ImageData();

    ~ImageData();

    void Clear();

    inline VoxelType& operator[](int_max LinearIndex);

    inline const VoxelType& operator[](int_max LinearIndex) const;

    inline VoxelType& operator()(int_max LinearIndex);

    inline const VoxelType& operator()(int_max LinearIndex) const;
   
    inline VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

    inline const VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline void TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const;

    inline void TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const;

    inline void Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const;

    inline void Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, double& xIndex, double& yIndex, double& zIndex) const;

private:
//deleted:
    ImageData(const ImageData&) = delete;
    ImageData(ImageData&&) = delete;
    void operator=(const ImageData&) = delete;
    void operator=(ImageData&&) = delete;
};

//===================================================================================================================//

template<typename VoxelType>
class Image : public Object
{
private:

    std::shared_ptr<ImageData<VoxelType>> m_ImageData;

    VoxelType* m_VoxelPointer; // keep tracking m_ImageData->m_DataArray.data();

public:		
	
	Image();

    Image(const Image<VoxelType>& InputImage) = delete;

    Image(Image<VoxelType>&& InputImage);

	~Image();

    void operator=(const Image<VoxelType>& InputImage) = delete;

    void operator=(Image<VoxelType>&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (VoxelType_Input) to float (VoxelType), etc

    template<typename VoxelType_Input>
    void Copy(const Image<VoxelType_Input>& InputImage);

    template<typename VoxelType_Input>
    bool Copy(const Image<VoxelType_Input>* InputImage);

    inline bool Fill(const VoxelType& Voxel);

    //---------------------------------------------------------//

    bool Share(Image<VoxelType>& InputImage);

    bool Share(Image<VoxelType>* InputImage);

    void ForceShare(const Image<VoxelType>& InputImage);

    bool ForceShare(const Image<VoxelType>* InputImage);

    //---------------------------------------------------------//

    void Take(Image<VoxelType>&& InputImage);

    void Take(Image<VoxelType>& InputImage);

    //-----------------------------------------------------------------//

    bool Initialize(const ImageSize& Size, const ImageVoxelSpacing& Spacing, const ImagePhysicalOrigin& Origin, const DenseMatrix<double>& Orientation = {});

    bool Initialize(int_max Lx, int_max Ly, int_max Lz,
                    double VoxelSpacing_x = 1.0,
                    double VoxelSpacing_y = 1.0,
                    double VoxelSpacing_z = 1.0,
                    double PhysicalOrigin_x = 0.0,
                    double PhysicalOrigin_y = 0.0,
                    double PhysicalOrigin_z = 0.0,                    
                    const DenseMatrix<double>& Orientation = {});

    inline bool IsEmpty() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

    inline ImageSize GetSize() const;

    inline bool SetSize(const ImageSize& Size);

    inline bool SetSize(int_max Lx, int_max Ly, int_max Lz);

    inline ImageVoxelSpacing GetVoxelSpacing() const;

    inline void SetVoxelSpacing(const ImageVoxelSpacing& VoxelSpacing);

    inline void SetVoxelSpacing(double VoxelSpacing_x, double VoxelSpacing_y, double VoxelSpacing_z);

    inline ImagePhysicalOrigin GetPhysicalOrigin() const;

    inline void SetPhysicalOrigin(const ImagePhysicalOrigin& Origin);

    inline void SetPhysicalOrigin(double Origin_x, double Origin_y, double Origin_z);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

    inline ImagePhysicalSize GetPhysicalSize() const;

    inline int_max GetVoxelNumber() const;

    template<typename VoxelType_Input>
    bool CopyData(const VoxelType_Input* InputVoxelPointer, int_max InputVoxelNumber);

    //--------------------------- Get Voxel Pointer ------------------------------//

    inline VoxelType* GetVoxelPointer();

    inline const VoxelType* GetVoxelPointer() const;

  
    // ------------------------ Index and Position ------------------------------------------------------------------------//

    inline int_max Transform3DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline void TransformLinearIndexTo3DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex, int_max& zIndex) const;

    inline void TransformLinearIndexTo3DPhysicalPosition(int_max LinearIndex, double& x, double& y, double& z) const;

    inline void Transform3DIndexTo3DPhysicalPosition(int_max xIndex, int_max yIndex, int_max zIndex, double& x, double& y, double& z) const;

    inline void Transform3DPhysicalPositionToContinuous3DIndex(double x, double y, double z, int_max& xIndex, int_max& yIndex, int_max& zIndex) const;

	//--------------------------- Get/Set Voxel      ------------------------------//

    inline VoxelType& operator[](int_max LinearIndex);

    inline const VoxelType& operator[](int_max LinearIndex) const;

	inline VoxelType& operator()(int_max LinearIndex);

	inline const VoxelType& operator()(int_max LinearIndex) const;

	inline VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline VoxelType& at(int_max LinearIndex);

	inline const VoxelType& at(int_max LinearIndex) const;

	inline VoxelType& at(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const VoxelType& at(int_max xIndex, int_max yIndex, int_max zIndex) const;

	//-------------------------- Get SubImage -------------------------------//

    Image GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s, int_max zIndex_e) const;

	//-------------------------- Pad, UnPad -------------------------------//

    Image  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    Image  Pad(VoxelType Voxel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

    Image  UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz) const;

	//------------------------- Get LinearIndex In Region -------------------//

    DenseMatrix<int_max> GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
	                                                int_max yIndex_s,     int_max Region_Ly,
                                                    int_max zIndex_s,     int_max Region_Lz) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    VoxelType Sum() const;

    VoxelType Mean() const;

    VoxelType Max() const;

    VoxelType Min() const;

};


}//end namespace mdk

#include "mdkImage.hpp"

#endif