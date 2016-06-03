#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <ostream>

#include "mdkDebugConfig.h"
#include "mdkCommonType.h"
#include "mdkSymbol.h"
#include "mdkObject.h"

namespace mdk
{

// assume UTF-8 in string
class String : public Object
{
private:     
	std::shared_ptr<std::basic_string<char>> m_StringData;

public:			
	//------------------- constructor and destructor ------------------------------------//

	inline String();

    inline String(char Element);

	inline String(const char* InputString);

	inline String(const std::basic_string<char>& InputString);

    // copy constructor
	inline String(const String& InputString);

    // move constructor
	inline String(String&& InputString);

	inline ~String();

    //----------------------  operator=  ----------------------------------------//

    inline void operator=(const String& InputString);

    inline void operator=(String&& InputString);

    inline void operator=(char Element);

    inline void operator=(const char* InputString);

    inline void operator=(const std::basic_string<char>& InputString);

    //----------------------  Get/Set Std string  ----------------------------------------//
    inline std::basic_string<char>& StdString();
    inline const std::basic_string<char>& StdString() const;

    //----------------------  Copy  ----------------------------------------//
    inline void Copy(const String& InputString);
	inline void Copy(String&& InputString);
    inline void Copy(const char* InputElementPointer);

    inline void Fill(char Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //
    inline void Share(String& InputString);
    inline void ForceShare(const String& InputString);

    //------------------------- Swap shared_ptr m_StringData -------------------------------------------//
    inline void Swap(String& InputString);

    //------------------------- Clear -------------------------------------------//
    inline void Clear();
	inline void Clear(MDK_Symbol_PureEmpty&);
	
	//------------------------- Recreate -------------------------------------------//
	inline void Recreate();
	//---------------------- Set/get Size ----------------------------------------//

    inline void Resize(int_max InputLength); // try to keep the old data

	inline void FastResize(int_max InputLength); // do not care about old data

	inline void ReserveCapacity(int_max InputElementCount); // reserve memory, current Length does not change

	inline void ReleaseUnusedCapacity();

    inline bool IsEmpty() const;

	inline bool IsPureEmpty() const;

    inline bool IsShared() const;

	inline bool IsSharedWith(const String& InputString) const;

    inline int_max GetCharCount() const;

    //--------------------- Get Data Pointer -----------------------------//

    inline char* GetCharPointer(); //  the position of the first char

	inline const char* GetCharPointer() const;

	inline char* begin(); //  the position of the first char

	inline const char* begin() const;

	inline char* end(); //  the position of the first char

	inline const char* end() const;

	//----------- Get/Set by Index -----------------------------------//

    // operator[] or () : no bound check

    inline char& operator[](int_max Index);

    inline char operator[](int_max Index) const;

    inline char& operator()(int_max Index);

    inline char operator()(int_max Index) const;
    
    // at(): bound check

    inline char& at(int_max Index);

    inline char at(int_max Index) const;

    //-------------------------------------------------------------------------------

	inline void Append(char Element);

	inline void Append(const char* InputString);

	inline void Append(const std::basic_string<char>& InputString);

	inline void Append(const String& InputString);

	inline void Delete(int_max Index);

	inline void Delete(const std::initializer_list<int_max>& IndexList);

	inline void Delete(const int_max* IndexList, int_max ListLength);

	inline void Delete(int_max Index_start, int_max Index_end);

	inline void Insert(int_max Index, char Element);

	inline void Insert(int_max Index, const char* InputString);

	inline void Insert(int_max Index, const std::basic_string<char>& InputString);

	inline void Insert(int_max Index, const String& InputString);

    //------------------------------------------------------------------

	inline void operator+=(char Element);

	inline void operator+=(const char* InputString);

    inline void operator+=(const std::basic_string<char>& InputString);

	inline void operator+=(const String& InputString);

private:

};

}//end namespace mdk

#include "mdkString.hpp"
#include "mdkStringOperator.h"

