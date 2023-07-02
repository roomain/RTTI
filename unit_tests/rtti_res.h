#pragma once
#include "RTTI_macros.h"
#include "Protocolextension.h"

class RTTIPureVirtualClass
{
	DECLARE_RTTI(1, RTTIPureVirtualClass)
public:
	explicit RTTIPureVirtualClass() = default;
	virtual ~RTTIPureVirtualClass() = default;
	virtual float data()const = 0;
};

class RTTIDerivedFromPureVirtual : public RTTIPureVirtualClass
{
	DECLARE_RTTI_DERIVED(1, RTTIDerivedFromPureVirtual, RTTIPureVirtualClass)
public:
	explicit RTTIDerivedFromPureVirtual() = default;
	~RTTIDerivedFromPureVirtual() = default;
	float data()const override{ return 1.5f; }
};

class RTTIParent
{
	DECLARE_RTTI(1, RTTIParent)
private:
	int	m_iData;

public:
	explicit RTTIParent(const int iVal);
	RTTIParent();
	int data()const;
};


class RTTIParent1
{
	DECLARE_RTTI(2, RTTIParent1)
private:
	double	m_dData;

public:
	explicit RTTIParent1(const double& m_dData);
	RTTIParent1();
	double data1()const;
};

class ParentPE : public ProtocolExtension
{
	DECLARE_RTTI_DERIVED(1, ParentPE, ProtocolExtension)
public:
	virtual int testPE()const noexcept { return 0; }
};

class ChildPE : public ParentPE
{
	DECLARE_RTTI_DERIVED(1, ChildPE, ParentPE)
public:
	virtual int testPE()const noexcept override { return 10; }
};



class RTTIFirstChild : public RTTIParent
{
	DECLARE_RTTI_DERIVED(3, RTTIFirstChild, RTTIParent)
public:
	explicit RTTIFirstChild(const int iVal);
	RTTIFirstChild();
};

class RTTISecondChild : public RTTIParent
{
	DECLARE_RTTI_DERIVED(4, RTTISecondChild, RTTIParent)
public:
	explicit RTTISecondChild(const int iVal);
	RTTISecondChild();
};


class RTTIGrandChild : public RTTIFirstChild
{
	DECLARE_RTTI_DERIVED(5, RTTIGrandChild, RTTIFirstChild)
public:
	explicit RTTIGrandChild(const int iVal);
	RTTIGrandChild();
};

class RTTISecondChildMult : public RTTIParent, public RTTIParent1
{
	DECLARE_RTTI_DERIVED(6, RTTISecondChildMult, RTTIParent, RTTIParent1)
public:
	explicit RTTISecondChildMult(const int iVal, const double& dValue);
	RTTISecondChildMult();
};

template<typename Type>
class RTTIDecorator : public Type
{
	DECLARE_RTTI_DERIVED(7, RTTIDecorator<Type>, Type)
public:
	RTTIDecorator() {}
};

