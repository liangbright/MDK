#ifndef __mdkSymbol_h
#define __mdkSymbol_h

#include  <string>

#include "mdkDebugConfig.h"

namespace mdk
{
//------------------------------------------- Symbol ALL  --------------------------------------------------------------------------//
struct InputStruct_For_MDK_Symbol_ALL
{
    std::string Name = "InputStruct_For_MDK_Symbol_ALL";
};

struct MDK_Symbol_ALL
{
    MDK_Symbol_ALL(const InputStruct_For_MDK_Symbol_ALL& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_MDK_Symbol_ALL")
        {
            MDK_Error("ALL Symbol error @ MDK_Symbol_ALL")
        }
    }

    ~MDK_Symbol_ALL() {}

private:
    MDK_Symbol_ALL() = delete;
    MDK_Symbol_ALL(const MDK_Symbol_ALL&) = delete;
    MDK_Symbol_ALL(MDK_Symbol_ALL&&) = delete;
    void operator=(const MDK_Symbol_ALL&) = delete;
    void operator=(MDK_Symbol_ALL&&) = delete;
};

static InputStruct_For_MDK_Symbol_ALL This_Is_InputStruct_For_MDK_Symbol_ALL;
static MDK_Symbol_ALL StaticGlobalObject_MDK_Symbol_ALL(This_Is_InputStruct_For_MDK_Symbol_ALL);
#define ALL StaticGlobalObject_MDK_Symbol_ALL
//-----------------------------------------------------------------------------------------------------------------------------------------//

//------------------------------- Symbol PureEmpty to Construct Empty Data Object (with Empty internal shared_ptr)  ------------------------//

struct InputStruct_For_MDK_Symbol_PureEmpty
{
    std::string Name = "InputStruct_For_MDK_Symbol_PureEmpty";
};

struct MDK_Symbol_PureEmpty
{
    MDK_Symbol_PureEmpty(const InputStruct_For_MDK_Symbol_PureEmpty& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_MDK_Symbol_PureEmpty")
        {
            MDK_Error("Symbol error @ MDK_Symbol_PureEmpty")
        }
    }

    ~MDK_Symbol_PureEmpty() {}

private:
    MDK_Symbol_PureEmpty() = delete;
    MDK_Symbol_PureEmpty(const MDK_Symbol_PureEmpty&) = delete;
    MDK_Symbol_PureEmpty(MDK_Symbol_PureEmpty&&) = delete;
    void operator=(const MDK_Symbol_PureEmpty&) = delete;
    void operator=(MDK_Symbol_PureEmpty&&) = delete;
};

static InputStruct_For_MDK_Symbol_PureEmpty This_Is_InputStruct_For_MDK_Symbol_PureEmpty;
static MDK_Symbol_PureEmpty StaticGlobalObject_MDK_Symbol_PureEmpty(This_Is_InputStruct_For_MDK_Symbol_PureEmpty);
#define MDK_PURE_EMPTY StaticGlobalObject_MDK_Symbol_PureEmpty
//--------------------------------------------------------------------------------------------------------------------------------//

//------------------------------- Symbol Empty to Construct Empty Data Object (with Nonempty internal shared_ptr) -------------------//

struct InputStruct_For_MDK_Symbol_Empty
{
    std::string Name = "InputStruct_For_MDK_Symbol_Empty";
};

struct MDK_Symbol_Empty
{
    MDK_Symbol_Empty(const InputStruct_For_MDK_Symbol_Empty& InputStruct)
    {
        if (InputStruct.Name != "InputStruct_For_MDK_Symbol_Empty")
        {
            MDK_Error("Symbol error @ MDK_Symbol_Empty")
        }
    }

    ~MDK_Symbol_Empty() {}

private:
    MDK_Symbol_Empty() = delete;
    MDK_Symbol_Empty(const MDK_Symbol_Empty&) = delete;
    MDK_Symbol_Empty(MDK_Symbol_Empty&&) = delete;
    void operator=(const MDK_Symbol_Empty&) = delete;
    void operator=(MDK_Symbol_PureEmpty&&) = delete;
};

static InputStruct_For_MDK_Symbol_Empty This_Is_InputStruct_For_MDK_Symbol_Empty;
static MDK_Symbol_Empty StaticGlobalObject_MDK_Symbol_Empty(This_Is_InputStruct_For_MDK_Symbol_Empty);
#define MDK_EMPTY StaticGlobalObject_MDK_Symbol_Empty
//--------------------------------------------------------------------------------------------------------------------------------//


}// namespace mdk

#endif