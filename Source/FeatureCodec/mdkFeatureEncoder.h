#ifndef __mdkFeatureEncoder_h
#define __mdkFeatureEncoder_h

#include <string>

#include "mdkFeatureCodebook.h"


namespace mdk
{

template<typename ElementType>
class mdkFeatureEncoder : public mdkObject
{
protected:

    mdkFeatureCodebook<ElementType> m_Codebook;

    mdkMatrix<ElementType> m_FeatureData;

    mdkMatrix<ElementType> m_FeatureCode;

public:

    mdkFeatureEncoder();

    ~mdkFeatureEncoder();

    //-----------------------------------------

    void Reset();

    void Clear();
    
    //-----------------------------------------

    bool SetInputFeatureData_Shared(const mdkMatrix<ElementType>& InputFeatureData);

    bool LoadFeatureData(const std::string& FilePathAndName);

    bool SetCodebook_Shared(const mdkFeatureCodebook<ElementType>& Codebook);

    bool LoadCodebook(const std::string& FilePathAndName);

    void SetOutputFeatureCode(mdkMatrix<ElementType>& FeatureCode);

    //-----------------------------------------

    virtual bool Run();

    //----------------------------------------------------//

    mdkMatrix<ElementType>& GetOutputFeatureCode();


protected:

    virtual bool GenerateCode();


private:
//deleted:
    mdkFeatureEncoder(const mdkFeatureEncoder&) = delete;

    void operator=(const mdkFeatureEncoder&) = delete;

};

}

#include "mdkFeatureEncoder.hpp"

#endif