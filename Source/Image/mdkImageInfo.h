#ifndef __mdkImageInfo_h
#define __mdkImageInfo_h

namespace mdk
{
struct BoxRegionOf3DIndexInImage3D
{ 
    int_max x0;
    int_max y0;
    int_max z0;

    int_max x1; // >= x0
    int_max y1; // >= y0
    int_max z1; // >= z0

//-------------------------------------
    BoxRegionOf3DIndexInImage3D()
    {
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


struct BoxRegionOf3DPhysicalPositionInImage3D
{
	double x0;
	double y0;
	double z0;

	double x1; // >= x0
	double y1; // >= y0
	double z1; // >= z0

//-------------------------------------

    BoxRegionOf3DPhysicalPositionInImage3D()
    {
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


struct Image3DInfo
{
	DenseVector<int_max, 3> Size;       // {Lx, Ly, Lz} number of Pixels in each direction
	DenseVector<double, 3>  Spacing;    // Pixel Spacing of DICOM DenseImage in world coordinate system {Sx, Sy, Sz} (unit: mm)
	DenseVector<double, 3>  Origin;     // Origin of DICOM DenseImage in world coordinate system (x,y,z) (unit: mm)
	DenseMatrix<double> Orientation;    // 3x3 Matrix

//-------------------------------------------
	Image3DInfo() { this->Clear(); }
	~Image3DInfo() {}

	Image3DInfo(const Image3DInfo& Info)
	{
		(*this) = Info;
	}

	void operator=(const Image3DInfo& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = Info.Orientation;
	}

	void Clear()
	{
		Size.Fill(0);
		Spacing.Fill(0);
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(3, 3);
		Orientation.FixSize();
		Orientation.FillDiagonal(1.0);
	}
};

//============================================ 2D =========================================================================

struct BoxRegionOf2DIndexInImage2D
{
	int_max x0;
	int_max y0;

	int_max x1; // >= x0
	int_max y1; // >= y0

	//-------------------------------------
	BoxRegionOf2DIndexInImage2D()
	{
		x0 = 0;
		y0 = 0;
		x1 = 0;
		y1 = 0;
	};

	int_max Lx() const
	{
		return x1 - x0 + 1;
	}

	int_max Ly() const
	{
		return y1 - y0 + 1;
	}
};


struct BoxRegionOf2DPhysicalPositionInImage2D
{
	double x0;
	double y0;

	double x1; // >= x0
	double y1; // >= y0

	//-------------------------------------

	BoxRegionOf2DPhysicalPositionInImage2D()
	{
		x0 = 0;
		y0 = 0;
		x1 = 0;
		y1 = 0;
	};

	double Lx() const
	{
		return x1 - x0;
	}

	double Ly() const
	{
		return y1 - y0;
	}
};

struct Image2DInfo
{
	DenseVector<int_max, 2> Size;       // {Lx, Ly} number of Pixels in each direction
	DenseVector<double, 2>  Spacing;    // Pixel Spacing of DICOM DenseImage in world coordinate system {Sx, Sy} (unit: mm)
	DenseVector<double, 2>  Origin;     // Origin of DICOM DenseImage in world coordinate system (x,y) (unit: mm)
	DenseMatrix<double> Orientation;    // 2x2 Matrix

	//-------------------------------------------
	Image2DInfo() { this->Clear(); }
	~Image2DInfo() {}

	Image2DInfo(const Image2DInfo& Info)
	{
		(*this) = Info;
	}

	void operator=(const Image2DInfo& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = Info.Orientation;
	}

	void Clear()
	{
		Size.Fill(0);
		Spacing.Fill(0);
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(2, 2);
		Orientation.FixSize();
		Orientation.FillDiagonal(1.0);
	}
};

}//namespace mdk


#endif