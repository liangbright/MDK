#ifndef __mdkKMeansDictionaryBuilder_hpp
#define __mdkKMeansDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
mdkKMeansDictionaryBuilder<ElementType>::mdkKMeansDictionaryBuilder()
{
}


template<typename ElementType>
mdkKMeansDictionaryBuilder<ElementType>::~mdkKMeansDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
bool mdkKMeansDictionaryBuilder<ElementType>::GenerateDictionary()
{
    return true;
}


}// namespace mdk


#endif