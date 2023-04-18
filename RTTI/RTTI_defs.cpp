#pragma once
#include <format>
#include "RTTI_defs.h"
#include "ProtocolExtension.h"

namespace RTTI
{
	IDefinition::IDefinition(const std::string& a_className, const unsigned short a_usVers) : m_sClassName{ a_className }, m_usVersion{ a_usVers }
	{
		//
	}

	IDefinition::IDefinition(const std::string& a_className, const unsigned short a_usVers, const std::vector<std::shared_ptr<IDefinition>>& a_init) : DefinitionNode(a_init), m_sClassName{ a_className }, m_usVersion{ a_usVers }
	{
		//
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
			auto iter = std::ranges::find_if(m_vProtocoleExt, [&protocol](auto curProt)
				{
					return curProt->isKindOf(protocol->isA()) || protocol->isKindOf(curProt->isA());
				});

			// remove old similar protocole
			if (iter != m_vProtocoleExt.end())
				m_vProtocoleExt.erase(iter);

			m_vProtocoleExt.emplace_back(protocol);
		}
	}

	bool IDefinition::unregisterProtocolExt(const std::shared_ptr<IDefinition>& pDef)
	{
		bool bRet = false;
		using protocolExtIter = std::vector<std::shared_ptr<ProtocolExtension>>::iterator;
		auto iter = std::ranges::find_if(m_vProtocoleExt, [&pDef](auto curProt)
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
		auto iter = std::ranges::find_if(m_vProtocoleExt, [&pDef](const auto curProt)
			{
				return curProt->isKindOf(pDef);
			});

		if (iter != m_vProtocoleExt.cend())
		{
			pProtocol = *iter;
		}
		else
		{
			auto iterParent = std::find_if(cbegin(), cend(), [&pDef](const auto& pParent)
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

	Exception::Exception(Exception::Type errType, std::string_view info) : m_exceptionType{ errType }
	{
		genMessage(info);
	}

	Exception::Type Exception::type()const noexcept
	{
		return m_exceptionType;
	}

	std::string Exception::message()const noexcept
	{
		return m_message;
	}

	void Exception::genMessage(std::string_view info)
	{
		if (info.empty())
		{
			switch (m_exceptionType)
			{
			case Type::parent_not_defined:
				m_message = "Parent Definition does not exist";
				break;

			case Type::null_definition:
				m_message = "Definition is null";
				break;

			case Type::already_defined:
				m_message = "Definition already exists";
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
				m_message = std::format("Parent ({}) definition does not exist", info);
				break;

			case Type::null_definition:
				m_message = std::format("{} definition is null", info);
				break;

			case Type::already_defined:
				m_message = std::format("{} definition already exists", info);
				break;

			default:
				break;
			}
		}
	}

	const char* Exception::what()const
	{
		return m_message.c_str();
	}
}
