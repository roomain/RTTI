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
			std::erase_if(m_vProtocoleExt, [&protocol](const auto& curProt)
				{
					return curProt->isKindOf(protocol->isA()) || protocol->isKindOf(curProt->isA());
				});
			
			m_vProtocoleExt.emplace_back(protocol);
		}
	}

	bool IDefinition::unregisterProtocolExt(const std::shared_ptr<IDefinition>& pDef)
	{
		return m_vProtocoleExt.size() != std::erase_if(m_vProtocoleExt, [&pDef](const auto& curProt)
			{
				return curProt->isKindOf(pDef);
			});
	}

	std::shared_ptr<ProtocolExtension> IDefinition::getProtocolExt(const std::shared_ptr<IDefinition>& pDef)const
	{
		std::shared_ptr<ProtocolExtension> pProtocol;
		auto iterProtocol = std::ranges::find_if(m_vProtocoleExt, [&pDef](const auto& a_pCurProtocol)
			{
				return a_pCurProtocol->isKindOf(pDef);
			});

		if (iterProtocol != m_vProtocoleExt.cend())
		{
			pProtocol = *iterProtocol;
		}
		else
		{
			for (const auto& pCurProtocol : parentDef())
			{
				pProtocol = pCurProtocol->getProtocolExt(pDef);
				if (pProtocol)
					break;
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
