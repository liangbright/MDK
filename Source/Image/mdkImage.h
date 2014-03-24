#ifndef __mdkImage_h
#define __mdkImage_h

#include <vector>
#include <memory>

#include "mdkObject.h"
#include "mdkMatrix.h"

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
	int64 Lx;
	int64 Ly;
	int64 Lz;
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

struct ImageVoxelPhysicalSize
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

//===================================================================================================================//
//--------------------------------------------------- ImageData struct --------------------------------------------//

template<typename VoxelType>
struct ImageData
{
    int64 m_ImageDimension[3]; // {Lx, Ly, Lz} number of voxels in each direction

    int64 m_VoxelNumberPerZSlice; // total number of voxels in each z-slice  = m_ImageSize[2]*m_ImageSize[1]

    double m_PhysicalOrigin[3];    // i.e., Origin in ITK, VTK, {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    double m_VoxelPhysicalSize[3]; // i.e., Spacing in ITK, VTK

    std::vector<VoxelType> m_DataArray;

//-----------------------------------------------------------

    ImageData();

    ~ImageData();

    void Clear();

    inline VoxelType& operator[](int64 LinearIndex);

    inline const VoxelType& operator[](int64 LinearIndex) const;

    inline VoxelType& operator()(int64 LinearIndex);

    inline const VoxelType& operator()(int64 LinearIndex) const;
   
    inline VoxelType& operator()(int64 xIndex, int64 yIndex, int64 zIndex);

    inline const VoxelType& operator()(int64 xIndex, int64 yIndex, int64 zIndex) const;

    inline int64 GetLinearIndexBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex) const;

    inline void Get3DIndexByLinearIndex(int64 LinearIndex, int64* xIndex, int64* yIndex, int64* zIndex) const;

    inline void Get3DPositionByLinearIndex(int64 LinearIndex, double* x, double* y, double* z) const;

    inline void Get3DPositionBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex, double* x, double* y, double* z) const;

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
    bool Copy(const Image<VoxelType_Input>& InputImage);
    
    template<typename VoxelType_Input>
    bool Copy(const Image<VoxelType_Input>* InputImage);

    template<typename VoxelType_Input>
    bool Copy(const VoxelType_Input* InputVoxelPointer, int64 Lx, int64 Ly, int64 Lz = 1);

    inline bool Fill(const VoxelType& Voxel);

    //---------------------------------------------------------//
    
    // only an empty image A can use A.Share(B)
    //
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

    bool ReInitialize(int64 Lx, int64 Ly, int64 Lz = 1,
		              double PhysicalOrigin_x = 0.0,
		              double PhysicalOrigin_y = 0.0,
		              double PhysicalOrigin_z = 0.0,
		              double VoxelPhysicalSize_x = 1.0,
		              double VoxelPhysicalSize_y = 1.0,
		              double VoxelPhysicalSize_z = 1.0);

	inline bool IsEmpty() const;

    inline VoxelType* GetVoxelPointer();

    inline const VoxelType* GetVoxelPointer() const;

    //--------------------------- Get/Set ZeroVoxel ------------------------------//

    inline void SetZeroVoxel(VoxelType ZeroVoxel);

    inline const VoxelType& GetZeroVoxel() const;

    // ------------------------ Get ImageInfo ------------------------------------------------------------------------//

    inline ImageDimension GetImageDimension() const;

    inline ImagePhysicalSize GetImagePhysicalSize() const;

    inline ImagePhysicalOrigin GetPhysicalOrigin() const;

    inline ImageVoxelPhysicalSize GetVoxelPhysicalSize() const;

    inline int64 GetVoxelNumber() const;

    // ------------------------ Index and Position ------------------------------------------------------------------------//

	inline int64 GetLinearIndexBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex = 0) const;

	inline void Get3DIndexByLinearIndex(int64 LinearIndex, int64* xIndex, int64* yIndex, int64* zIndex) const;

    inline void Get3DPositionByLinearIndex(int64 LinearIndex, double* x, double* y, double* z) const;

    inline void Get3DPositionBy3DIndex(int64 xIndex, int64 yIndex, int64 zIndex, double* x, double* y, double* z) const;

	//--------------------------- Get/Set Voxel      ------------------------------//

    inline VoxelType& operator[](int64 LinearIndex);

    inline const VoxelType& operator[](int64 LinearIndex) const;

	inline VoxelType& operator()(int64 LinearIndex);

	inline const VoxelType& operator()(int64 LinearIndex) const;

	inline VoxelType& operator()(int64 xIndex, int64 yIndex, int64 zIndex = 0);

	inline const VoxelType& operator()(int64 xIndex, int64 yIndex, int64 zIndex = 0) const;

	inline VoxelType& at(int64 LinearIndex);

	inline const VoxelType& at(int64 LinearIndex) const;

	inline VoxelType& at(int64 xIndex, int64 yIndex, int64 zIndex = 0);

	inline const VoxelType& at(int64 xIndex, int64 yIndex, int64 zIndex =0) const;

	//-------------------------- Get SubImage -------------------------------//

    Image GetSubImage(int64 xIndex_s, int64 xIndex_e, int64 yIndex_s, int64 yIndex_e, int64 zIndex_s = 0, int64 zIndex_e = 0) const;

	//-------------------------- Pad, UnPad -------------------------------//

    Image  Pad(const std::string& Option, int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const;

    Image  Pad(VoxelType Voxel, int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const;

    Image  UnPad(int64 Pad_Lx, int64 Pad_Ly, int64 Pad_Lz = 0) const;

	//------------------------- Get LinearIndex In Region -------------------//

    mdkMatrix<int64> GetLinearIndexListOfRegion(int64 xIndex_s,     int64 Region_Lx,
		                                        int64 yIndex_s,     int64 Region_Ly,
                                                int64 zIndex_s = 0, int64 Region_Lz = 0) const;

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