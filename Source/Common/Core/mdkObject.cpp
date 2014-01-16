#include "mdkDebug.h"

#include "mdkObject.h"

namespace mdk
{

mdkObject::mdkObject()
{
	m_MaximumInputNumber = 1000;
	m_MaximumOutputNumber = 1000;
}
	
mdkObject::~mdkObject()
{
}


void mdkObject::SetMaximumInputNumber(uint64 Number)
{
	m_MaximumInputNumber = Number;
}

void mdkObject::SetMaximumOutputNumber(uint64 Number)
{
	m_MaximumOutputNumber = Number;
}


void mdkObject::SetInputList(const std::vector<mdkObject*>& InputList)
{
	m_InputList = InputList;
}


void mdkObject::SetOutputList(const std::vector<mdkObject*>& OutputList)
{
	m_OutputList = OutputList;
}


bool mdkObject::SetInputConnection(uint64 InputPort, mdkObject* Input)
{
	if (InputPort < 0 || Input == nullptr)
	{
		return false;
	}

	if (InputPort > m_MaximumInputNumber)
	{
		return false;
	}

	auto ListSize = uint64(m_InputList.size());

	if (InputPort > ListSize)
	{
		for (uint64 i = ListSize; i < InputPort; ++i)
		{
			m_InputList.push_back(nullptr);
		}
		m_InputList.push_back(Input);
	}
	else
	{
		m_InputList[InputPort] = Input;
	}

	return true;
}


bool mdkObject::SetInputConnection(mdkObject* Input)
{
	auto ListSize = uint64(m_InputList.size());

	if (ListSize == 0)
	{
		m_InputList.push_back(Input);
	}
	else
	{
		m_InputList[0] = Input;
	}

	return true;
}


bool mdkObject::SetOutputConnection(uint64 OutputPort, mdkObject* Output)
{
	if (OutputPort < 0 || Output == nullptr)
	{
		return false;
	}

	auto ListSize = uint64(m_OutputList.size());

	if (OutputPort > ListSize)
	{
		for (uint64 i = ListSize; i < OutputPort; ++i)
		{
			m_OutputList.push_back(nullptr);
		}
		m_OutputList.push_back(Output);		
	}
	else
	{
		m_OutputList[OutputPort] = Output;
	}

	return true;
}


bool mdkObject::SetOutputConnection(mdkObject* Output)
{
	auto ListSize = uint64(m_OutputList.size());

	if (ListSize == 0)
	{
		m_OutputList.push_back(Output);
	}
	else
	{
		m_OutputList[0] = Output;
	}

	return true;
}


bool mdkObject::Update()
{
    // handle feedback loop
	if (m_SelfUpdateCounter > 1)
	{
		mdkWarning << "A feedback loop is detected";

		return false;
	}

	m_SelfUpdateCounter += 1;

	//---------------------------------------------------
	auto ListSize = uint64(m_InputList.size());

	//---- this is a source object, no input connection ------------------//
	if (ListSize == 0)
	{
		//return true if some parameters are changed
	    return this->IsModified();
	}

	//------------------- Update Input ---------------------------------//
	bool IsInputChanged = false;
	mdkObject* Input = nullptr;
	for (uint64 i = 0; i < ListSize; ++i)
	{
		Input = m_InputList[i];
		if (Input != nullptr)
		{
			auto IsChanged = Input->Update();
			if (IsChanged == true)
			{
				IsInputChanged = true;
			}
		}
	}

	//--- clear the counter
	m_SelfUpdateCounter = 0;

	if (IsInputChanged == true)
	{
		this->Run();

		// do something here		

		return true;
	}

	return false;
}


void mdkObject::Run()
{
}


void mdkObject::ClearInputAndOutputList()
{
	m_InputList.clear();

	m_OutputList.clear();

}


bool mdkObject::IsModified()
{
	// check the last time of upate

	// check the last time of run

	return true;
}


void mdkObject::Clear()
{

}

}//end namespace mdk