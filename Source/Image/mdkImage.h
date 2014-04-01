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

struct ImageDimension
{
	int_max Lx;
	int_max Ly;
	int_max Lz;
};

struct ImagePhysicalSize
{
    double Sx;
    double Sy;
    double Sz;
};

struct ImagePhysicalOrigin
{
    double x;
    double y;
    double z;
};

struct ImageVoxelSpacing
{
    double Sx;
    double Sy;
    double Sz;
};

typedef enum
{
    NearestNeighbor,
    CubicLinear,
} ImageInterpolationMethodEnum;


struct ImageBoxRegionOf3DIndex
{
    bool IsEmpty;

    int_max x0_Index;
    int_max y0_Index;
    int_max z0_Index;

    int_max x1_Index;
    int_max y1_Index;
    int_max z1_Index;

//-------------------------------------
    ImageBoxRegionOf3DIndex()
    {
        IsEmpty = true;
        x0_Index = 0;
        y0_Index = 0;
        z0_Index = 0;
        x1_Index = 0;
        y1_Index = 0;
        z1_Index = 0;
    };

    int_max Lx() const
    {
        return x1_Index - x0_Index + 1;
    }

    int_max Ly() const
    {
        return y1_Index - y0_Index + 1;
    }

    int_max Lz() const
    {
        return z1_Index - z0_Index + 1;
    }
};


struct ImageBoxRegionOf3DPosition
{
    bool IsEmpty;

    double x0;
    double y0;
    double z0;

    double x1;
    double y1;
    double z1;

//-------------------------------------

    ImageBoxRegionOf3DPosition() 
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
    int_max m_Dimension[3]; // {Lx, Ly, Lz} number of voxels in each direction

    int_max m_VoxelNumberPerZSlice; // total number of voxels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

    double m_PhysicalOrigin[3];    // i.e., Origin in ITK, VTK, {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    double m_VoxelSpacing[3]; // i.e., Spacing in ITK, VTK

    std::vector<VoxelType> m_DataArray;

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

    inline int_max GetLinearIndexBy3DIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

    inline void Get3DIndexByLinearIndex(int_max LinearIndex, int_max* xIndex, int_max* yIndex, int_max* zIndex) const;

    inline void Get3DPositionByLinearIndex(int_max LinearIndex, double* x, double* y, double* z) const;

    inline void Get3DPositionBy3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, double* x, double* y, double* z) const;

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

    VoxelType m_ZeroVoxel;

    VoxelType m_ZeroVoxel_Error_Output;

public:		
	
	Image();

	Image(const Image<VoxelType>& InputImage) = delete;

    Image(Image<VoxelType>&& InputImage);

	~Image();

    void operator=(const Image<VoxelType>& InputImage);

    void operator=(Image<VoxelType>&& InputImage);

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (VoxelType_Input) to float (VoxelType), etc

    template<typename VoxelType_Input>
    void Copy(const Image<VoxelType_Input>& InputImage);
    
    template<typename VoxelType_Input>
    bool Copy(const Image<VoxelType_Input>* InputImage);

    template<typename VoxelType_Input>
    bool Copy(const VoxelType_Input* InputVoxelPointer,
              const ImageDimension& Dim, 
              const ImagePhysicalOrigin& Origin,
              const ImageVoxelSpacing& VoxelSize);

    template<typename VoxelType_Input>
    bool Copy(const VoxelType_Input* InputVoxelPointer, 
              int_max Lx, int_max Ly, int_max Lz = 1,
              double PhysicalOrigin_x = 0.0,
              double PhysicalOrigin_y = 0.0,
              double PhysicalOrigin_z = 0.0,
              double VoxelSpacing_x = 1.0,
              double VoxelSpacing_y = 1.0,
              double VoxelSpacing_z = 1.0);

    inline bool Fill(const VoxelType& Voxel);

    //---------------------------------------------------------//
    
    bool Share(Image<VoxelType>& InputImage);

    bool Share(Image<VoxelType>* InputImage);

    // Share InputImage no matter what
    //
    void ForceShare(const Image<VoxelType>& InputImage);

    bool ForceShare(const Image<VoxelType>* InputImage);

    //---------------------------------------------------------//

    void Take(Image<VoxelType>&& InputImage);

    void Take(Image<VoxelType>& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //-----------------------------------------------------------------//
    
    bool ReInitialize(const ImageDimension& Dim, const ImagePhysicalOrigin& Origin, const ImageVoxelSpacing& VoxelSize);

    bool ReInitialize(int_max Lx, int_max Ly, int_max Lz = 1,
		              double PhysicalOrigin_x = 0.0,
		              double PhysicalOrigin_y = 0.0,
		              double PhysicalOrigin_z = 0.0,
                      double VoxelSpacing_x = 1.0,
                      double VoxelSpacing_y = 1.0,
                      double VoxelSpacing_z = 1.0);

	inline bool IsEmpty() const;

    //--------------------------- Get Voxel Pointer ------------------------------//

    inline VoxelType* GetVoxelPointer();

    inline const VoxelType* GetVoxelPointer() const;

    //--------------------------- Get/Set ZeroVoxel ------------------------------//

    inline void SetZeroVoxel(VoxelType ZeroVoxel);

    inline const VoxelType& GetZeroVoxel() const;

    // ------------------------ Get ImageInfo ------------------------------------------------------------------------//

    inline ImageDimension GetDimension() const;

    inline ImagePhysicalSize GetPhysicalSize() const;

    inline ImagePhysicalOrigin GetPhysicalOrigin() const;

    inline ImageVoxelSpacing GetVoxelSpacing() const;

    inline int_max GetVoxelNumber() const;

    // ------------------------ Index and Position ------------------------------------------------------------------------//

	inline int_max GetLinearIndexBy3DIndex(int_max xIndex, int_max yIndex, int_max zIndex = 0) const;

	inline void Get3DIndexByLinearIndex(int_max LinearIndex, int_max* xIndex, int_max* yIndex, int_max* zIndex) const;

    inline void Get3DPositionByLinearIndex(int_max LinearIndex, double* x, double* y, double* z) const;

    inline void Get3DPositionBy3DIndex(int_max xIndex, int_max yIndex, int_max zIndex, double* x, double* y, double* z) const;

	//--------------------------- Get/Set Voxel      ------------------------------//

    inline VoxelType& operator[](int_max LinearIndex);

    inline const VoxelType& operator[](int_max LinearIndex) const;

	inline VoxelType& operator()(int_max LinearIndex);

	inline const VoxelType& operator()(int_max LinearIndex) const;

	inline VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex = 0);

	inline const VoxelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex = 0) const;

	inline VoxelType& at(int_max LinearIndex);

	inline const VoxelType& at(int_max LinearIndex) const;

	inline VoxelType& at(int_max xIndex, int_max yIndex, int_max zIndex = 0);

	inline const VoxelType& at(int_max xIndex, int_max yIndex, int_max zIndex =0) const;

	//-------------------------- Get SubImage -------------------------------//

    Image GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e, int_max zIndex_s = 0, int_max zIndex_e = 0) const;

	//-------------------------- Pad, UnPad -------------------------------//

    Image  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz = 0) const;

    Image  Pad(VoxelType Voxel, int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz = 0) const;

    Image  UnPad(int_max Pad_Lx, int_max Pad_Ly, int_max Pad_Lz = 0) const;

	//------------------------- Get LinearIndex In Region -------------------//

    DenseMatrix<int_max> GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
	                                              int_max yIndex_s,     int_max Region_Ly,
                                                  int_max zIndex_s = 0, int_max Region_Lz = 0) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    VoxelType Sum() const;

    VoxelType Mean() const;

    VoxelType Max() const;

    VoxelType Min() const;

    //-------------------------- Interpolation at 3D Position (x, y, z)-------------------------------//

    inline VoxelType InterpolateAt3DPosition(double x, double y, double z,
                                             ImageInterpolationMethodEnum Method = ImageInterpolationMethodEnum::NearestNeighbor) const;

};


}//end namespace mdk

#include "mdkImage.hpp"

#endif