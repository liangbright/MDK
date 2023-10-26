#include <iostream>
#include <string>

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "mdkJsonValue.h"
#include "mdkJsonObject.h"
#include "mdkJsonFile.h"
#include "mdkQTDataStructureConversion.h"

namespace mdk
{

void Test_QJson()
{
	QString QFilePathAndName("C:/Research/MDK/MDK_Build/Test/Test_QJson/TestData/Test_A.json");
	QFile HeaderFile(QFilePathAndName);
	if (!HeaderFile.open(QIODevice::ReadOnly))
	{	
		std::cout << "Couldn't open Header File" << '\n';
	}

	//----------------------------------------------------------//
	QByteArray HeaderContent = HeaderFile.readAll();
	QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
	QJsonObject HeaderObject = HeaderDoc.object();
	//-----------------------------------------------------------//	
	std::string ObjectType;
	{
		auto it = HeaderObject.find("ObjectType");
		if (it != HeaderObject.end())
		{
			ObjectType = it.value().toString().toStdString();
		}
	}
	QJsonObject MemberLocation;
	{
		auto it = HeaderObject.find("MemberLocation");
		if (it != HeaderObject.end())
		{
			MemberLocation = it.value().toObject();
		}
	}	
	//-----------------------------------------------------------//
	double m_A1 = 0;
	{
		auto it = MemberLocation.find("m_A1");
		if (it != MemberLocation.end())
		{
			auto Location_m_A1 = it.value().toString();
			if (Location_m_A1 == "Inside")
			{
				auto it_m_A1 = HeaderObject.find("m_A1");
				if (it_m_A1 != HeaderObject.end())
				{
					m_A1 = it_m_A1.value().toDouble();
				}
			}
			
		}
	}

	//==============================================================//
	QJsonObject OutputObject;
	//OutputObject.insert("m_A1", QJsonValue(m_A1));
	//OutputObject.insert("MemberLocation", QJsonValue(MemberLocation));
	OutputObject["m_A1"] = QJsonValue(m_A1);

}

void Test_JsonValue()
{
	JsonValue Value("abcd");
	JsonArray JArray;
	JsonObject JObject;
	DenseMatrix<double> A(1, 10);
	DenseVector<String> B;
	B.Resize(2);
	B[0] = "B0";
	B[1] = "B1";

	Value = true;
	Value = 1;
	Value = 1.0;
	Value = "123";
	Value = JArray;
	Value = JObject;
	Value = A;
	Value = B;
}

void Test_JsonObject()
{
	DenseMatrix<double> A(1, 10);

	JsonObject JObject;
	JObject["1"] = 1;
	JObject["2.0"]=2.0;
	JObject["Array"] = A;
}

void Test_JsonFile_1()
{
	DenseMatrix<double> A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	DenseVector<String> B = { "B0", "B1" };

	JsonObject JObject;
	JObject["ObjectType"] = "DenseMatrix";
	JObject["RowNumber"] = int(1);
	JObject["ColNumber"] = float(2.0);
	JObject["ScalarType"] = "double";
	JObject["ScalarArray"] = A;
	JObject["ScalarArray_json"] = "A.json";
	JObject["StringArray"] = B;	
	JObject["Empty"] = A;
	JObject["Empty"].Clear();
	JObject["bool"] = true;

	String FileName_a = "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/testJsonFile1a.json";
	JsonFile::Save(JObject, FileName_a);

	JsonObject JObject2;
	JsonFile::Load(JObject2, FileName_a);

	String FileName_b = "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/testJsonFile1b.json";
	JsonFile::Save(JObject2, FileName_b);
}

void Test_UniquePtr()
{
	std::unique_ptr<String> m_OtherData;
	
	auto tempSPtr = std::make_unique<String>();
	*tempSPtr = "abc";
	m_OtherData.reset(tempSPtr.release());

	auto SPtr = static_cast<String*>(m_OtherData.get());

	String s = std::move(*SPtr);
}

void Test_JsonFile_2()
{
	DenseMatrix<double> A = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0, 
		                      1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0,
							  1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0 };

	DenseVector<String> B = {"B0", "B1"};

	JsonObject JObjectA, JObjectB;

	JObjectA["Empty"] = A;
	JObjectA["Empty"].Clear();
	JObjectA["ObjectType"] = "DenseMatrix";
	JObjectA["RowNumber"] = 1;
	JObjectA["ColNumber"] = 2;
	JObjectA["ScalarType"] = "double";
	JObjectA["ScalarArray"] = A;
	JObjectA["ScalarArray_json"] = "A.json";
	JObjectA["Empty"] = A;
	JObjectA["Empty"].Clear();
		
	JObjectB = JObjectA;

	JsonArray JArray;
	JArray.Resize(5);
	JArray[0] = A;
	JArray[1] = JObjectA;
	JArray[2] = JObjectB;
	JArray[3] = A;
	JArray[4] = B;

	JsonObject JObjectC, JObjectD;

	JObjectC["ObjectB"] = JObjectB;
	JObjectC["ObjectA"] = JObjectA;
	JObjectC["A"] = A;
	JObjectC["B"] = JArray;

	JObjectD = JObjectC;
	
	JsonObject JObject;
	//JObject["ObjectD"] = JObjectD;
	//JObject["ObjectC"] = JObjectC;
	JObject["JArray"] = JArray;

	String FileName = "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/testJsonFile2.json";
	JsonFile::Save(JObject, FileName);
}

void Test_MDK_QT_JsonConversion()
{
	DenseMatrix<double> A = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0,
		1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0,
		1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0 };

	JsonObject JObjectA, JObjectB;

	JObjectA["Empty"] = A;
	JObjectA["Empty"].Clear();
	JObjectA["ObjectType"] = "DenseMatrix";
	JObjectA["RowNumber"] = 1;
	JObjectA["ColNumber"] = 2;
	JObjectA["ScalarType"] = "double";
	JObjectA["ScalarArray"] = A;
	JObjectA["ScalarArray_json"] = "A.json";

	JObjectB = JObjectA;

	JsonArray JArray;
	JArray.Resize(4);
	JArray[0] = A;
	JArray[1] = JObjectA;
	JArray[2] = JObjectB;
	JArray[3] = A;

	JsonObject JObjectC, JObjectD;

	JObjectC["ObjectB"] = JObjectB;
	JObjectC["ObjectA"] = JObjectA;
	JObjectC["A"] = A;
	JObjectC["JArray"] = JArray;

	JObjectD = JObjectC;

	JsonObject JObject;
	JObject["ObjectD"] = JObjectD;
	JObject["ObjectC"] = JObjectC;
	JObject["JArray"] = JArray;
	//-----------------------------------------------------------------	
	JsonObject JObject_out = JObject;
	//-----------------------------------------------------------------	
	JsonFile::Save(JObject_out, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_MDK_QT_JsonConversion_1.json");
	//-----------------------------------------------------------------
	auto QJObject_out = ConvertMDKJsonObjectToQTJsonObject(JObject_out);
	QJsonDocument QJDoc(QJObject_out);
	auto QBArray = QJDoc.toJson();
	QFile JFile_out("C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_MDK_QT_JsonConversion_2.json");
	if (!JFile_out.open(QIODevice::WriteOnly))
	{
		std::cout << "something is wrong here" << '\n';
		return;
	}
	JFile_out.write(QBArray);
	JFile_out.close();
	//-----------------------------------------------------------------
	QFile JFile_in("C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_MDK_QT_JsonConversion_1.json");
	if (!JFile_in.open(QIODevice::ReadOnly))
	{
		std::cout << "something is wrong here" << '\n';
		return;
	}
	QByteArray QBArray_in = JFile_in.readAll();
	QJsonDocument JDoc_in(QJsonDocument::fromJson(QBArray_in));
	QJsonObject QJObject_in = JDoc_in.object();
	//-----------------------------------------------------------//
	auto JObject_in = ConvertQTJsonObjectToMDKJsonObject(QJObject_in);
	JsonFile::Save(JObject_in, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_MDK_QT_JsonConversion_3.json");
}

void Test_JsonFile_NullValue()
{
	JsonValue A;
	JsonObject JObject1, JObject2;
	JObject1["A"] = A;
	JsonFile::Save(JObject1, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_JsonFile_NullValue.json");

	JsonFile::Load(JObject2, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_JsonFile_NullValue.json");
}

void Test_JsonFile_EmptyArray()
{
	DenseMatrix<double> A;
	JsonObject JObject1, JObject2;
	JObject1["A"] = A;
	JsonFile::Save(JObject1, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_JsonFile_EmptyArray.json");

	JsonFile::Load(JObject2, "C:/Research/MDK/MDK_Build/Test/Test_Json/TestData/Test_JsonFile_EmptyArray.json");
}

/*
struct myStruct
{
public:
	union
	{
		std::unique_ptr<float> upFloat;
		std::unique_ptr<int> upInt;
	};
	enum class unionType { f, i, none } type = unionType::none; // Keep it sane
	myStruct(){}
	myStruct(std::unique_ptr<float> p)
	{
		new (&upFloat) std::unique_ptr < float > {std::move(p)};
		type = unionType::f;
	}
	myStruct(std::unique_ptr<int> p)
	{
		new (&upInt) std::unique_ptr < int > {std::move(p)};
		type = unionType::i;
	}
	~myStruct()
	{
		switch (type)
		{
		case unionType::f: upFloat.~unique_ptr<float>(); break;
		case unionType::i: upInt.~unique_ptr<int>(); break;
		}
	}
};

int test_main()
{
	std::vector<std::unique_ptr<myStruct>> structVec;
	structVec.push_back(std::make_unique<myStruct>(std::make_unique<float>(3.14f)));
	structVec.push_back(std::make_unique<myStruct>(std::make_unique<int>(739)));
	structVec.push_back(std::make_unique<myStruct>());
	structVec.push_back(std::make_unique<myStruct>(std::make_unique<float>(8.95f)));
	structVec.push_back(std::make_unique<myStruct>(std::make_unique<int>(3)));
	structVec.push_back(std::make_unique<myStruct>());

	for (auto &a : structVec)
	{
		if (a->type == myStruct::unionType::none)
		{
			std::cout << "Struct Has Unallocated Union" << std::endl;
		}
		else if (a->type == myStruct::unionType::f)
		{
			std::cout << "Struct float = " << *a->upFloat << std::endl;
		}
		else
		{
			std::cout << "Struct int = " << *a->upInt << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}
*/

}//end of namespace