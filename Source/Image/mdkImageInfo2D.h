#ifndef mdk_ImageInfo2D_h
#define mdk_ImageInfo2D_h

namespace mdk
{

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


struct BoxRegionOf2DWorldPositionInImage2D
{
	double x_min;
	double y_min;

	double x_max;
	double y_max;

	//-------------------------------------

	BoxRegionOf2DWorldPositionInImage2D()
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