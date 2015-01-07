#ifndef __mdkString_h
#define __mdkString_h

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <ostream>

#include "mdkDebugConfig.h"
#include "mdkCommonType.h"
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

    // deep-copy or shared-copy constructor
	inline String(const String& InputString, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
	inline String(String&& InputString) noexcept;

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

    inline void Copy(const String* InputString);

    inline void Copy(const char* InputElementPointer);

    inline void Fill(char Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline void Share(String& InputString);

    inline void Share(String* InputString);

    inline void ForceShare(const String& InputString);

    inline void ForceShare(const String* InputString);

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(String&& InputString);

    inline void Take(String& InputString);

    inline void Take(String* InputString);

    //------------------------- Swap shared_ptr m_StringData -------------------------------------------//

    inline void Swap(String& InputString);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

	inline void ReleaseUnusedCapacity();

    inline bool IsEmpty() const;

	inline bool IsPureEmpty() const;

    inline bool IsShared() const;

    inline int_max GetCharNumber() const;

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

    inline const char& operator[](int_max Index) const;

    inline char& operator()(int_max Index);

    inline const char& operator()(int_max Index) const;
    
    // at(): bound check

    inline char& at(int_max Index);

    inline const char& at(int_max Index) const;

    //-------------------------------------------------------------------------------

	inline bool Append(char Element);

	inline bool Append(const char* InputString);

	inline bool Append(const std::basic_string<char>& InputString);

	inline bool Append(const String& InputString);

	inline bool Delete(int_max Index);

	inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const int_max* IndexList, int_max ListLength);

	inline bool Delete(int_max Index_start, int_max Index_end);

	inline bool Insert(int_max Index, char Element);

	inline bool Insert(int_max Index, const char* InputString);

	inline bool Insert(int_max Index, const std::basic_string<char>& InputString);

	inline bool Insert(int_max Index, const String& InputString);

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

#endif
