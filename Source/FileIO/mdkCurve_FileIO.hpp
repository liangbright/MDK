#ifndef mdk_Curve_FileIO_hpp
#define mdk_Curve_FileIO_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveSingle3DCurveAsVTKFile(const DenseMatrix<ScalarType>& InputCurve, const String& FilePathAndName)
{
	auto VTKCurve = ConvertSingleMDK3DCurveToVTKPolyData(InputCurve);
	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.StdString().c_str());
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
bool SaveMultiple3DCurveAsVTKFile(const ObjectArray<DenseMatrix<ScalarType>>& InputCurveList, const String& FilePathAndName)
{
	auto VTKCurveData = ConvertMultipleMDK3DCurveToVTKPolyData(InputCurveList);
	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.StdString().c_str());
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
bool LoadSingle3DCurveFromVTKFile(DenseMatrix<ScalarType>& OutputCurve, const String& FilePathAndName)
{
	ObjectArray<DenseMatrix<ScalarType>> OutputCurveList;
	if (LoadMultiple3DCurveFromVTKFile(OutputCurveList, FilePathAndName) == false)
	{
		return false;
	}

	if (OutputCurveList.GetLength() > 0)
	{
		OutputCurve = std::move(OutputCurveList[0]);
	}
	else
	{
		OutputCurve.Clear();
	}
	return true;
}


template<typename ScalarType>
bool LoadMultiple3DCurveFromVTKFile(ObjectArray<DenseMatrix<ScalarType>>& OutputCurveList, const String& FilePathAndName)
{
	auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
	Reader->SetFileName(FilePathAndName.StdString().c_str());

	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ Load3DCurveFromVTKFile(...) ")
		return false;
	}

	auto VTKCurveData = Reader->GetOutput();
	ConvertVTKPolyDataToMDK3DCurve(VTKCurveData, OutputCurveList);
	return true;
}

}//namespace mdk

#endif