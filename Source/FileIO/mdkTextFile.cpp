#ifndef mdk_TextFile_cpp
#define mdk_TextFile_cpp

#include "mdkTextFile.h"

namespace mdk
{

TextFile::TextFile(const String& FilePathAndName)
{
	m_QFile = std::make_unique<QFile>();
	m_QFile->setFileName(FilePathAndName.StdString().c_str());
	if (!m_QFile->open(QIODevice::ReadWrite))
	{
		MDK_Error("Couldn't open file @ TextFile::TextFile(...)")
		return;
	}
	m_QTextStream = std::make_unique<QTextStream>(m_QFile.get());
}

TextFile::~TextFile()
{
}

bool TextFile::Clear()
{
	return m_QFile->resize(0);
}

TextFile& TextFile::operator<<(char InputChar)
{
	*m_QTextStream << InputChar;
	return *this;
}

TextFile& TextFile::operator<<(const char* InputString)
{
	*m_QTextStream << InputString;
	return *this;
}

TextFile& TextFile::operator<<(const String& InputString)
{
	*m_QTextStream << InputString.StdString().c_str();
	return *this;
}

void TextFile::Close()
{
	if (m_QTextStream)
	{
		m_QTextStream->flush();
		m_QFile->close();
	}
}

//==============================================================================================================================//
//-------------------------------------- public static function -----------------------------------------------------------------//
//===============================================================================================================================//
void TextFile::Save(const String& InputText, const String& FilePathAndName)
{
	TextFile OutputFile(FilePathAndName);
	OutputFile << InputText;
	OutputFile.Close();
}
//==========================================================================================================================//
bool TextFile::Load(String& OutputText, const String& FilePathAndName)
{
	QFile JFile_in(FilePathAndName.StdString().c_str());
	if (!JFile_in.open(QIODevice::ReadOnly))
	{
		MDK_Error("Can not open file to read @ JsonFile::Load(...) " << FilePathAndName);
		return false;
	}
	QByteArray QBArray_in = JFile_in.readAll();
	OutputText = QBArray_in.toStdString();
	return true;
}

}

#endif