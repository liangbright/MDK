#ifndef mdk_ImageInfo_h
#define mdk_ImageInfo_h

namespace mdk
{
struct BoxRegionOf3DIndexInImage3D
{ 
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;

//-------------------------------------
    BoxRegionOf3DIndexInImage3D()
    {
		x_min = 0;
		y_min = 0;
		z_min = 0;
		x_max = 0;
		y_max = 0;
		z_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min + 1;
    }

	double Ly() const
    {
		return y_max - y_min + 1;
    }

	double Lz() const
    {
		return z_max - z_min + 1;
    }
};


struct BoxRegionOf3DPhysicalPositionInImage3D
{
	double x_min;
	double y_min;
	double z_min;

	double x_max;
	double y_max;
	double z_max;

//-------------------------------------

    BoxRegionOf3DPhysicalPositionInImage3D()
    {
		x_min = 0;
		y_min = 0;
		z_min = 0;
		x_max = 0;
		y_max = 0;
		z_max = 0;
    };

	double Lx() const
    {
		return x_max - x_min;
    }

	double Ly() const
    {
		return y_max - y_min;
    }

	double Lz() const
    {
		return z_max - z_min;
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
	double x_min;
	double y_min;

	double x_max;
	double y_max;

	//-------------------------------------
	BoxRegionOf2DIndexInImage2D()
	{
		x_min = 0;
		y_min = 0;
		x_max = 0;
		y_max = 0;
	};

	double Lx() const
	{
		return x_max - x_min + 1;
	}

	double Ly() const
	{
		return y_max - y_min + 1;
	}
};


struct BoxRegionOf2DPhysicalPositionInImage2D
{
	double x_min;
	double y_min;

	double x_max;
	double y_max;

	//-------------------------------------

	BoxRegionOf2DPhysicalPositionInImage2D()
	{
		x_min = 0;
		y_min = 0;
		x_max = 0;
		y_max = 0;
	};

	double Lx() const
	{
		return x_max - x_min;
	}

	double Ly() const
	{
		return y_max - y_min;
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