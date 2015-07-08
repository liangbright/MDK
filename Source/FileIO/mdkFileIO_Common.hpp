#ifndef mdk_FileIO_Common_hpp
#define mdk_FileIO_Common_hpp

namespace mdk
{
inline String ExtractFilePath(const String& FilePathAndName)
{
	String FilePath;

	int_max Index_current = -1;
	for (int_max k = FilePathAndName.GetCharCount() - 1; k >= 0; --k)
	{
		if (FilePathAndName[k] == '/')
		{
			Index_current = k;
			break;
		}
	}
	if (Index_current >= 0)
	{
		FilePath.Resize(Index_current + 1);
		for (int_max k = 0; k <= Index_current; ++k) // "/" will be the end of FilePath
		{
			FilePath[k] = FilePathAndName[k];
		}
	}
	else
	{
		MDK_Warning("Invalid input @ JsonFile::ExtractFilePath(...)")
	}
	return FilePath;
}
//==========================================================================================================================//
inline String ExtractFileName(const String& FilePathAndName)
{
	String FileName;

	int_max Index_current = -1;
	for (int_max k = FilePathAndName.GetCharCount() - 1; k >= 0; --k)
	{
		if (FilePathAndName[k] == '/')
		{
			Index_current = k;
			break;
		}
	}
	if (Index_current >= 0)
	{
		FileName.Resize(FilePathAndName.GetCharCount() - Index_current - 1);
		for (int_max k = Index_current + 1; k < FilePathAndName.GetCharCount(); ++k)
		{
			FileName[k - Index_current - 1] = FilePathAndName[k];
		}
	}
	else
	{
		MDK_Warning("Invalid input @ JsonFile::ExtractFileName(...)")
	}
	return FileName;
}
//==========================================================================================================================//
inline String ExtractFileType(const String& FileNameWithType)
{
	String FileType;

	int_max Index_current = -1;
	for (int_max k = FileNameWithType.GetCharCount() - 1; k >= 0; --k)
	{
		if (FileNameWithType[k] == '.')
		{
			Index_current = k;
			break;
		}
	}
	if (Index_current >= 0)
	{
		FileType.Resize(FileNameWithType.GetCharCount() - Index_current - 1);
		for (int_max k = Index_current + 1; k < FileNameWithType.GetCharCount(); ++k) // "." will not be included
		{
			FileType[k - Index_current - 1] = FileNameWithType[k];
		}
	}
	else
	{
		MDK_Warning("Invalid input @ JsonFile::ExtractFileType(...)")
	}
	return FileType;
}
//==========================================================================================================================//
inline String RemoveFileType(const String& FilePathAndName)
{
	int_max Index_current = -1;
	for (int_max k = FilePathAndName.GetCharCount() - 1; k >= 0; --k)
	{
		if (FilePathAndName[k] == '.')
		{
			Index_current = k;
			break;
		}
	}
	if (Index_current >= 0)
	{
		String FilePathAndNameWithOutType;
		FilePathAndNameWithOutType.Resize(Index_current);
		for (int_max k = 0; k < Index_current; ++k)   // "." will be removed
		{
			FilePathAndNameWithOutType[k] = FilePathAndName[k];
		}

		return FilePathAndNameWithOutType;
	}
	else
	{
		return FilePathAndName;
	}
}
//==========================================================================================================================//
//==========================================================================================================================//
template<typename ScalarType>
bool SaveScalarArrayAsDataFile(const ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName)
{
	int_max ByteCount = GetByteCountOfScalar(ScalarType(0));
	if (ByteCount <= 0)
	{
		MDK_Error("Unknown ScalarType @ SaveScalarArrayAsDataFile(...)")
		return false;
	}

	QFile DataFile(FilePathAndName.StdString().c_str());
	if (!DataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't create file " << FilePathAndName << " @ SaveScalarArrayAsDataFile(...)")
		return false;
	}

	if (ScalarArray != nullptr && ArrayLength > 0)
	{
		DataFile.write((char*)ScalarArray, ArrayLength*ByteCount);
		DataFile.flush();
	}
	DataFile.close();
	return true;
}


template<typename ScalarType>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName, const String& ScalarTypeInDataFile)
{
	if (ScalarTypeInDataFile == "double")
	{
		return LoadScalarArrayFromDataFile<ScalarType, double>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "float")
	{
		return LoadScalarArrayFromDataFile<ScalarType, float>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "int8")
	{
		return LoadScalarArrayFromDataFile<ScalarType, int8>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "int16")
	{
		return LoadScalarArrayFromDataFile<ScalarType, int16>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "int32")
	{
		return LoadScalarArrayFromDataFile<ScalarType, int32>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "int64")
	{
		return LoadScalarArrayFromDataFile<ScalarType, int64>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "uint8")
	{
		return LoadScalarArrayFromDataFile<ScalarType, uint8>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "uint16")
	{
		return LoadScalarArrayFromDataFile<ScalarType, uint16>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "uint32")
	{
		return LoadScalarArrayFromDataFile<ScalarType, uint32>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else if (ScalarTypeInDataFile == "uint64")
	{
		return LoadScalarArrayFromDataFile<ScalarType, uint64>(ScalarArray, ArrayLength, FilePathAndName);
	}
	else
	{
		MDK_Error("unknown ScalarType of data file @ LoadScalarArrayFromDataFile(...) ")
		return false;
	}
}


template<typename ScalarType, typename ScalarTypeInDataFile>
bool LoadScalarArrayFromDataFile(ScalarType* ScalarArray, int_max ArrayLength, const String& FilePathAndName)
{
	if (ScalarArray == nullptr)
	{
		if (ArrayLength == 0)
		{
			return true;
		}
		else
		{
			MDK_Error("Invalid input @ LoadScalarArrayFromDataFile(...)")
			return false;
		}
	}

	QFile DataFile(FilePathAndName.StdString().c_str());
	if (!DataFile.open(QIODevice::ReadOnly))
	{
		MDK_Error("Couldn't open data file:" << FilePathAndName)
		return false;
	}

	int_max BypesofDataFile = DataFile.size();

	int_max ByteCountOfScalarTypeInDataFile = GetByteCountOfScalar(ScalarTypeInDataFile(0));

	if (BypesofDataFile != ArrayLength * ByteCountOfScalarTypeInDataFile)
	{
		MDK_Error("Data file size is not equal to matrix size @ LoadScalarArrayFromDataFile(...)")
		DataFile.close();
		return false;
	}

	for (int_max i = 0; i < ArrayLength; ++i)
	{
		auto tempScalar = ScalarTypeInDataFile(0);

		DataFile.read((char*)(&tempScalar), ByteCountOfScalarTypeInDataFile);

		ScalarArray[i] = ScalarType(tempScalar);
	}
	DataFile.close();

	return true;
}

}//namespace mdk

#include "mdkFileIO_Common.hpp"

#endif
