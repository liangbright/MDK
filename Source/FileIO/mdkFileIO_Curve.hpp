#ifndef __mdkFileIO_Curve_hpp
#define __mdkFileIO_Curve_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveSingle3DCurveAsVTKFile(const DenseMatrix<ScalarType>& InputCurve, const std::string& FilePathAndName)
{
	auto VTKCurve = ConvertSingleMDK3DCurveToVTKPolyData(InputCurve);
	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.c_str());
	writer->SetInputData(VTKCurve);
	try
	{
		writer->Write();
	}
	catch (...)
	{
		MDK_Error(" Can not write data @ Save3DCurveAsVTKFile(...) ")
		return false;
	}

	return true;
}


template<typename ScalarType>
bool SaveMultiple3DCurveAsVTKFile(const ObjectArray<DenseMatrix<ScalarType>>& InputCurveList, const std::string& FilePathAndName)
{
	auto VTKCurveData = ConvertMultipleMDK3DCurveToVTKPolyData(InputCurveList);
	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.c_str());
	writer->SetInputData(VTKCurveData);
	try
	{
		writer->Write();
	}
	catch (...)
	{
		MDK_Error(" Can not write data @ SaveMultiple3DCurveAsVTKFile(...) ")
		return false;
	}

	return true;
}


template<typename ScalarType>
DenseMatrix<ScalarType> LoadSingle3DCurveFromVTKFile(const std::string& FilePathAndName)
{
	auto CurveList = LoadMultiple3DCurveFromVTKFile<ScalarType>(FilePathAndName);

	DenseMatrix<ScalarType> OutputCurve;
	if (CurveList.GetLength() > 0)
	{
		OutputCurve = std::move(CurveList[0]);
	}
	return OutputCurve;
}


template<typename ScalarType>
ObjectArray<DenseMatrix<ScalarType>> LoadMultiple3DCurveFromVTKFile(const std::string& FilePathAndName)
{
	ObjectArray<DenseMatrix<ScalarType>> OutputCurveList;

	auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
	Reader->SetFileName(FilePathAndName.c_str());

	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ Load3DCurveFromVTKFile(...) ")
		return OutputCurveList;
	}

	auto VTKCurveData = Reader->GetOutput();
	OutputCurveList = ConvertVTKPolyDataToMDK3DCurve<ScalarType>(VTKCurveData);
	return OutputCurveList;
}

}//namespace mdk

#endif