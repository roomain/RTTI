#pragma once
#include "RTTI_macros.h"
#include "Protocolextension.h"



class ParentPE : public ProtocolExtension
{
	RTTI_DEFINITIONS(1, ParentPE, ProtocolExtension)
public:
	virtual int testPE()const noexcept { return 0; }
};

class ChildPE : public ParentPE
{
	RTTI_DEFINITIONS(1, ChildPE, ParentPE)
public:
	virtual int testPE()const noexcept override { return 10; }
};

class RTTIParent
{
	RTTI_DEFINITION(1, RTTIParent)
private:
	int	m_iData;

public:
	RTTIParent(const int iVal);
	RTTIParent();
	int data()const;
};

class RTTIParent1
{
	RTTI_DEFINITION(2, RTTIParent1)
private:
	double	m_dData;

public:
	RTTIParent1(const double& m_dData);
	RTTIParent1();
	double data1()const;
};

class RTTIFirstChild : public RTTIParent
{
	RTTI_DEFINITIONS(3, RTTIFirstChild, RTTIParent)
public:
	RTTIFirstChild(const int iVal);
	RTTIFirstChild();
};

class RTTISecondChild : public RTTIParent
{
	RTTI_DEFINITIONS(4, RTTISecondChild, RTTIParent)
public:
	RTTISecondChild(const int iVal);
	RTTISecondChild();
};


class RTTIGrandChild : public RTTIFirstChild
{
	RTTI_DEFINITIONS(5, RTTIGrandChild, RTTIFirstChild)
public:
	RTTIGrandChild(const int iVal);
	RTTIGrandChild();
};

class RTTISecondChildMult : public RTTIParent, public RTTIParent1
{
	RTTI_DEFINITIONS(6, RTTISecondChildMult, RTTIParent, RTTIParent1)
public:
	RTTISecondChildMult(const int iVal, const double& dValue);
	RTTISecondChildMult();
};

template<typename Type>
class RTTIDecorator : public Type
{
	RTTI_DEFINITIONS(7, RTTIDecorator<Type>, Type)
public:
	RTTIDecorator() {}
};

