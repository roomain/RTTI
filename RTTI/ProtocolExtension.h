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
    RTTI_DEFINITION(1, ProtocolExtension)

protected:
    ProtocolExtension() {}
};