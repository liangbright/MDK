#include <iostream>
#include <string>

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "mdkJsonValue.h"
#include "mdkJsonArray.h"
#include "mdkJsonObject.h"

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

	Value = true;
	Value = 1;
	Value = 1.0;
	Value = "123";
	Value = JArray;
	Value = JObject;
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