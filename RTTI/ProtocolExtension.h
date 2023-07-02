#pragma once

/***********************************************
* @headerfile ProtocolExtension.h
* @date 26 / 06 / 2022
* @author Romain Crespel
************************************************/
#include "RTTI_macros.h"

/*@brief base class of protocol extension used for RTTI*/
class ProtocolExtension
{
    DECLARE_RTTI(1, ProtocolExtension)
public:
    ProtocolExtension() = default;
    virtual ~ProtocolExtension() = default;
};