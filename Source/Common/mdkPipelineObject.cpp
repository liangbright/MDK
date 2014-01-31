#include "mdkDebug.h"

#include "mdkPipelineObject.h"

namespace mdk
{

mdkPipelineObject::mdkPipelineObject()
{
	m_MaximumInputNumber = 1000;
	m_MaximumOutputNumber = 1000;
}
	
mdkPipelineObject::~mdkPipelineObject()
{
}

mdkPipelineObject::mdkPipelineObject(const mdkPipelineObject&)
{

}

void mdkPipelineObject::operator=(const mdkPipelineObject&)
{

}

void mdkPipelineObject::SetMaximumInputNumber(uint64 Number)
{
	m_MaximumInputNumber = Number;
}

void mdkPipelineObject::SetMaximumOutputNumber(uint64 Number)
{
	m_MaximumOutputNumber = Number;
}


void mdkPipelineObject::SetInputList(const std::vector<mdkPipelineObject*>& InputList)
{
	m_InputList = InputList;
}


void mdkPipelineObject::SetOutputList(const std::vector<mdkPipelineObject*>& OutputList)
{
	m_OutputList = OutputList;
}


bool mdkPipelineObject::SetInputConnection(uint64 InputPort, mdkPipelineObject* Input)
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


bool mdkPipelineObject::SetInputConnection(mdkPipelineObject* Input)
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


bool mdkPipelineObject::SetOutputConnection(uint64 OutputPort, mdkPipelineObject* Output)
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


bool mdkPipelineObject::SetOutputConnection(mdkPipelineObject* Output)
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


bool mdkPipelineObject::Update()
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
	mdkPipelineObject* Input = nullptr;
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


void mdkPipelineObject::Run()
{
}


void mdkPipelineObject::ClearInputAndOutputList()
{
	m_InputList.clear();

	m_OutputList.clear();

}


bool mdkPipelineObject::IsModified()
{
	// check the last time of upate

	// check the last time of run

	return true;
}


void mdkPipelineObject::Clear()
{

}

}//end namespace mdk