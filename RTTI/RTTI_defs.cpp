#pragma once
#include "RTTI_defs.h"
#include "ProtocolExtension.h"
#include <ranges>


RTTIDefinition::RTTIDefinition(const std::string_view& a_name, const unsigned short a_version) :
	m_version{ a_version }, m_className{ a_name } {}

RTTIDefinition::RTTIDefinition(const std::string_view& a_name, const unsigned short a_version,
	const std::vector<std::shared_ptr<RTTIDefinition>>& a_vParents) :
	m_version{ a_version }, m_className{ a_name }, m_vParent{ a_vParents } {}

bool RTTIDefinition::isKindOf(const std::shared_ptr<RTTIDefinition>& pDef)const noexcept
{
	bool bIsKindOf = isSame(pDef);
	if (!bIsKindOf)
		bIsKindOf = inheritFrom(pDef);
	return bIsKindOf;
}

bool RTTIDefinition::isSame(const std::shared_ptr<RTTIDefinition>& pDef)const noexcept
{
	return pDef.get() == this;
}

void RTTIDefinition::registerProtocolExt(const std::shared_ptr<ProtocolExtension>& a_protocol)
{
	if (a_protocol)
	{
		std::erase_if(m_extensions, [&a_protocol](const auto& curProt)
			{
				return curProt->isKindOf(a_protocol->isA()) || a_protocol->isKindOf(curProt->isA());
			});

		m_extensions.emplace_back(a_protocol);
	}
}

bool RTTIDefinition::unregisterProtocolExt(const std::shared_ptr<RTTIDefinition>& a_pDef)
{
	return m_extensions.size() != std::erase_if(m_extensions, [&a_pDef](const auto& curProt)
		{
			return curProt->isKindOf(a_pDef);
		});
}

std::shared_ptr<ProtocolExtension> RTTIDefinition::getProtocolExt(const std::shared_ptr<RTTIDefinition>& pDef)const
{
	std::shared_ptr<ProtocolExtension> pProtocolExt;
	auto iter = std::ranges::find_if(m_extensions, [&pDef](auto a_pExt) {return a_pExt->isKindOf(pDef); });
	if (iter != m_extensions.end())
	{
		pProtocolExt = *iter;
	}
	else
	{
		for (const auto& pCurProtocol : m_vParent)
		{
			pProtocolExt = pCurProtocol->getProtocolExt(pDef);
			if (pProtocolExt)
				break;
		}
	}
	return pProtocolExt;
}

bool RTTIDefinition::inheritFrom(const std::shared_ptr<RTTIDefinition>& a_pDef)const
{
	bool bFound = std::ranges::find(m_vParent, a_pDef) != m_vParent.end();
	if (!bFound)
		bFound = std::ranges::find_if(m_vParent, [&a_pDef](auto&& a_curdef)
			{return a_curdef->inheritFrom(a_pDef); }) != m_vParent.end();

	return bFound;
}

