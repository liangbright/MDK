#ifndef __mdkFileIO_FeatureDictionaryForSpareCoding_hpp
#define __mdkFileIO_FeatureDictionaryForSpareCoding_hpp

namespace mdk
{

template<typename ElementType>
bool SaveFeatureDictionaryForSparseCodingAsJsonDataFile(const FeatureDictionaryForSparseCoding<ElementType>& Dictionary, const std::string& JsonFilePathAndName)
{
    ElementType ReferenceScalar = ElementType(0);

    int_max ByteNumber = GetByteNumberOfScalar(ReferenceScalar);

    if (ByteNumber <= 0)
    {
        MDK_Error("Unknown type of image @ SaveFeatureDictionaryForSparseCoding(...)")
        return false;
    }

    auto ElementTypeName = GetScalarTypeName(ReferenceScalar);
    QString QElementTypeName(ElementTypeName.c_str());

    //-------------------------------------------------------------------------------------

    std::vector<NameValueQStringPair> PairList(5);

    PairList[0].Name = "ObjectType";
    PairList[0].Value = "FeatureDictionaryForSparseCoding";

    PairList[1].Name = "Name";
    PairList[1].Value = Dictionary.GetName().c_str();

    PairList[2].Name = "ElementType";
    PairList[2].Value = QElementTypeName;

    auto Size = Dictionary.GetSize();

    PairList[3].Name = "RowNumber";
    PairList[3].Value = QString::number(Size.RowNumber);

    PairList[4].Name = "ColNumber";
    PairList[4].Value = QString::number(Size.ColNumber);

    // write header file (json) --------------------------------------------------

	QString QFilePathAndName(JsonFilePathAndName.c_str());

    SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);

    // write data file  --------------------------------------------------

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        MDK_Error("Couldn't open file to write data @ SaveFeatureDictionaryForSparseCoding(...)")
        return false;
    }

    // step 1 : write BasisMatrix
    int_max L = Size.RowNumber * Size.ColNumber;
    auto RawPointer = (char*)Dictionary.BasisMatrix().GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    // step 2 : write VarianceOfL1Distance
    auto L = Dictionary.VarianceOfL1Distance().GetElementNumber();
    RawPointer = (char*)Dictionary.VarianceOfL1Distance().GetElementPointer();
    DataFile.write(RawPointer, L*ByteNumber);

    DataFile.flush();
    DataFile.close();

    return true;
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType> LoadFeatureDictionaryForSparseCodingFromJsonDataFile(const std::string& FilePathAndName)
{
    FeatureDictionaryForSparseCoding<ElementType> Dictionary;

    //----------------------------------------------------------

    auto ReferenceScalar = ElementType(0);

    auto OutputElementTypeName = GetScalarTypeName(ReferenceScalar);

    int_max OutputByteNumber = GetByteNumberOfScalar(ReferenceScalar);

    //---------------------------------------------- Read header --------------------------------------------------------//

    QString QFilePathAndName(FilePathAndName.c_str());

    QFile HeaderFile(QFilePathAndName);

    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open header file:" << FilePathAndName)
        return Dictionary;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

    std::string InputElementTypeName;

    auto it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ElementType @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    //---------------------------------------------------

    std::string Name;

    it = HeaderObject.find("Name");
    if (it != HeaderObject.end())
    {
         Name = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get Name @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    //---------------------------------------------------

    int_max RowNumber = 0;

    it = HeaderObject.find("RowNumber");
    if (it != HeaderObject.end())
    {
        RowNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get RowNumber @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    int_max ColNumber = 0;

    it = HeaderObject.find("ColNumber");
    if (it != HeaderObject.end())
    {
        ColNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get ColNumber @ LoadFeatureDictionaryForSparseCoding(...)")
        HeaderFile.close();
        return Dictionary;
    }

    HeaderFile.close();

    //-------------------------------------------------- Read data ---------------------------------------------------------//

    QFile DataFile(QFilePathAndName + ".data");

    if (!DataFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open data file:" << FilePathAndName)
        return Dictionary;
    }

    //----------------------------------------------------------------------------------------------

    Dictionary.SetName(Name);
    Dictionary.BasisMatrix().FastResize(RowNumber, ColNumber);
    Dictionary.VarianceOfL1Distance().FastResize(1, ColNumber);
    Dictionary.VarianceOfL2Distance().FastResize(1, ColNumber);
    Dictionary.VarianceOfKLDivergence().FastResize(1, ColNumber);
    Dictionary.VarianceOfReconstruction().FastResize(1, ColNumber);

    if (OutputElementTypeName == InputElementTypeName)
    {
        LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(Dictionary.BasisMatrix(), DataFile, OutputByteNumber);
        LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(Dictionary.VarianceOfL1Distance(), DataFile, OutputByteNumber);
        LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(Dictionary.VarianceOfL2Distance(), DataFile, OutputByteNumber);
        LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(Dictionary.VarianceOfKLDivergence(), DataFile, OutputByteNumber);
        LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(Dictionary.VarianceOfReconstruction(), DataFile, OutputByteNumber);
    }
    else
    {
        MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ LoadFeatureDictionaryForSparseCoding(...)")

        LoadDenseMatrixFromJsonDataFile_Data(Dictionary.BasisMatrix(), DataFile, InputElementTypeName);
        LoadDenseMatrixFromJsonDataFile_Data(Dictionary.VarianceOfL1Distance(), DataFile, InputElementTypeName);
        LoadDenseMatrixFromJsonDataFile_Data(Dictionary.VarianceOfL2Distance(), DataFile, InputElementTypeName);
        LoadDenseMatrixFromJsonDataFile_Data(Dictionary.VarianceOfKLDivergence(), DataFile, InputElementTypeName);
        LoadDenseMatrixFromJsonDataFile_Data(Dictionary.VarianceOfReconstruction(), DataFile, InputElementTypeName);
    }

    DataFile.close();

    return Dictionary;
}


}//namespace mdk

#endif