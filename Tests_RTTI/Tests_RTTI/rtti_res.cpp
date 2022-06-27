#include "rtti_res.h"


RTTIParent::RTTIParent(const int iVal) : m_iData{ iVal } {}
RTTIParent::RTTIParent() : m_iData{ 0 } {}

int RTTIParent::data()const
{
	return m_iData;
}


RTTIParent1::RTTIParent1(const double& dVal) : m_dData{ dVal } {}
RTTIParent1::RTTIParent1() : m_dData{ 0 } {}

double RTTIParent1::data1()const
{
	return m_dData;
}
//----------------------------------------------------------------------------
RTTIFirstChild::RTTIFirstChild(const int iVal) : RTTIParent{ iVal } {}
RTTIFirstChild::RTTIFirstChild() : RTTIParent{ 0 } {}

RTTISecondChild::RTTISecondChild(const int iVal) : RTTIParent{ iVal } {}
RTTISecondChild::RTTISecondChild() : RTTIParent{ 0 } {}

RTTIGrandChild::RTTIGrandChild(const int iVal) : RTTIFirstChild{ iVal } {}
RTTIGrandChild::RTTIGrandChild() : RTTIFirstChild{ 0 } {}

RTTISecondChildMult::RTTISecondChildMult(const int iVal, const double& dVal) : RTTIParent{ iVal }, RTTIParent1{ dVal } {}
RTTISecondChildMult::RTTISecondChildMult() : RTTIParent{ 0 }, RTTIParent1{ 0 } {}