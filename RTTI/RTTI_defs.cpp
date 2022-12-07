#pragma once
#include "RTTI_defs.h"
#include "ProtocolExtension.h"

namespace RTTI
{
	IDefinition::IDefinition(const std::string& a_className, const unsigned short a_usVers) : m_sClassName{ a_className }, m_usVersion{ a_usVers }
	{
		//
	}

	IDefinition::IDefinition(const std::string& a_className, const unsigned short a_usVers, const std::vector<std::shared_ptr<IDefinition>>& a_init) : m_sClassName{ a_className }, m_usVersion{ a_usVers }
	{
		for (auto parentDef : a_init)
			m_vParentsDef.push_back(parentDef);
	}


	bool IDefinition::isKindOf(const std::shared_ptr<IDefinition>& pDef)const noexcept
	{
		if (this == pDef.get())
			return true;
		return inheritFrom(pDef);
	}

	bool IDefinition::isSame(const std::shared_ptr<IDefinition>& pDef)const noexcept
	{
		return this == pDef.get();
	}

	void IDefinition::registerProtocolExt(const std::shared_ptr<ProtocolExtension>& protocol)
	{
		if (protocol)
		{
			using protocolExtIter = std::vector<std::shared_ptr<ProtocolExtension>>::iterator;
			protocolExtIter iter = std::find_if(m_vProtocoleExt.begin(), m_vProtocoleExt.end(), [&](auto curProt)
				{
					return curProt->isKindOf(protocol->isA()) || protocol->isKindOf(curProt->isA());
				});

			if (iter != m_vProtocoleExt.end())
			{
				m_vProtocoleExt.erase(iter);
			}

			m_vProtocoleExt.push_back(protocol);
		}
	}

	bool IDefinition::unregisterProtocolExt(const std::shared_ptr<IDefinition>& pDef)
	{
		bool bRet = false;
		using protocolExtIter = std::vector<std::shared_ptr<ProtocolExtension>>::iterator;
		protocolExtIter iter = std::find_if(m_vProtocoleExt.begin(), m_vProtocoleExt.end(), [&](auto curProt)
			{
				return curProt->isKindOf(pDef);
			});

		if (iter != m_vProtocoleExt.end())
		{
			m_vProtocoleExt.erase(iter);
			bRet = true;
		}
		return bRet;
	}

	std::shared_ptr<ProtocolExtension> IDefinition::getProtocolExt(const std::shared_ptr<IDefinition>& pDef)const
	{
		std::shared_ptr<ProtocolExtension> pProtocol = nullptr;

		using protocolExtIter = std::vector<std::shared_ptr<ProtocolExtension>>::const_iterator;
		protocolExtIter iter = std::find_if(m_vProtocoleExt.cbegin(), m_vProtocoleExt.cend(), [&](const auto curProt)
			{
				return curProt->isKindOf(pDef);
			});

		if (iter != m_vProtocoleExt.cend())
		{
			pProtocol = *iter;
		}
		else
		{
			auto iterParent = std::find_if(cbegin(), cend(), [&](const auto& pParent)
				{
					return pParent->getProtocolExt(pDef);
				});

			if (iterParent != cend())
			{
				pProtocol = (*iterParent)->getProtocolExt(pDef);
			}
		}
		return pProtocol;
	}

	//----------------------------------------------------------------------------------------------------------------------

	Exception::Exception(Exception::Type errType, const std::string& info) : m_exceptionType{ errType }, m_info{ info }
	{
		//
	}

	Exception::Type Exception::type()const noexcept
	{
		return m_exceptionType;
	}

	std::string Exception::message()const noexcept
	{
		std::string errMes;
		if (m_info.empty())
		{
			switch (m_exceptionType)
			{
			case Type::parent_not_defined:
				errMes = "Parent Definition does not exist";
				break;

			case Type::null_definition:
				errMes = "Definition is null";
				break;

			case Type::already_defined:
				errMes = "Definition already exists";
				break;

			default:
				break;
			}
		}
		else
		{
			switch (m_exceptionType)
			{
			case Type::parent_not_defined:
				errMes = "Parent (" + m_info + ") definition does not exist";
				break;

			case Type::null_definition:
				errMes = m_info + " definition is null";
				break;

			case Type::already_defined:
				errMes = m_info + " definition already exists";
				break;

			default:
				break;
			}
		}

		return errMes;
	}

	const char* Exception::what()const
	{
		// NOT USED
		return nullptr;
	}
}
