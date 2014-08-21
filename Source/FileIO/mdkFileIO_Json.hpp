#ifndef __mdkFileIO_Json_hpp
#define __mdkFileIO_Json_hpp

namespace mdk
{

bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName, bool Flag_Append)
{    
	QFile JsonFile;
	if (Flag_Append == false)
	{
		QFile::remove(FilePathAndName + "~temp~");
		JsonFile.setFileName(FilePathAndName + "~temp~");

		if (!JsonFile.open(QIODevice::WriteOnly))
		{
			MDK_Error("Couldn't open file to save result @ SaveNameValuePairListAsJsonFile(...)")
			return false;
		}
	}
	else
	{
		JsonFile.setFileName(FilePathAndName);
		if (!JsonFile.open(QIODevice::Append))
		{
			MDK_Error("Couldn't open file to save result @ SaveNameValuePairListAsJsonFile(...)")
			return false;
		}
	}

    QTextStream out(&JsonFile);

    auto s = int_max(PairList.size());

    out << "{\n";
    for (int_max i = 0; i < s; ++i)
    {
        out << "\"" << PairList[i].Name << "\"" << " : " << "\"" << PairList[i].Value << "\"";

        if (i < s - 1)
        {
            out << "," << "\n";
        }
        else
        {
            out << "\n";
        }
    }
    out << "}\n";

    out.flush();

	if (Flag_Append == false)
	{
		QFile::remove(FilePathAndName);
		JsonFile.rename(FilePathAndName);
	}

    JsonFile.close();

    return true;
}


bool SaveNameValuePairListAsJsonFile(const std::vector<NameValueStdStringPair>& PairList, const std::string& FilePathAndName, bool Flag_Append)
{
	QFile JsonFile;
	QString QFilePathAndName(FilePathAndName.c_str());
	if (Flag_Append == false)
	{		
		QFile::remove(QFilePathAndName + "~temp~");
		JsonFile.setFileName(QFilePathAndName + "~temp~");
		if (!JsonFile.open(QIODevice::WriteOnly))
		{
			MDK_Error("Couldn't open file to save result @ SaveNameValuePairListAsJsonFile(...)")
			return false;
		}
	}
	else
	{
		JsonFile.setFileName(QFilePathAndName);
		if (!JsonFile.open(QIODevice::Append))
		{
			MDK_Error("Couldn't open file to save result @ SaveNameValuePairListAsJsonFile(...)")
			return false;
		}
	}

	QTextStream out(&JsonFile);

	auto s = int_max(PairList.size());

	out << "{\n";
	for (int_max i = 0; i < s; ++i)
	{
		QString QName(PairList[i].Name.c_str());
		QString QValue(PairList[i].Value.c_str());

		out << "\"" << QName << "\"" << " : " << "\"" << QValue << "\"";

		if (i < s - 1)
		{
			out << "," << "\n";
		}
		else
		{
			out << "\n";
		}
	}
	out << "}\n";

	out.flush();

	if (Flag_Append == false)
	{
		QFile::remove(QFilePathAndName);
		JsonFile.rename(QFilePathAndName);
	}
	JsonFile.close();

	return true;
}

}//namespace mdk

#endif