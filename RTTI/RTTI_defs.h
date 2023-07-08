#pragma once
/***********************************************
* @headerfile rtti.h
* @date 26 / 06 / 2022
* @author Romain Crespel
************************************************/

#include <vector>
#include <memory>
#include <string>
#include <tuple>

class ProtocolExtension;
using ProtocolExtension_Ptr = std::shared_ptr<ProtocolExtension>;



class RTTIDefinition : public std::enable_shared_from_this<RTTIDefinition>
{
private:
	std::vector<std::shared_ptr<RTTIDefinition>> m_vParent;	/*!< parent class definitions*/
	unsigned short m_version;								/*!< version*/
	std::string m_className;								/*!< name of defined class*/
	std::vector<ProtocolExtension_Ptr> m_extensions;		/*!< functional extensions*/

	[[nodiscard]] constexpr bool inheritFrom(const std::shared_ptr<RTTIDefinition>& a_pDef)const
	{
		bool bFound = std::ranges::find(m_vParent, a_pDef) != m_vParent.end();
		if (!bFound)
			bFound = std::ranges::find_if(m_vParent, [&a_pDef](auto&& a_curdef)
				{return a_curdef->inheritFrom(a_pDef); }) != m_vParent.end();

		return bFound;
	}

public:
	RTTIDefinition() = delete;
	RTTIDefinition(const std::string_view& a_name, const unsigned short a_version);
	RTTIDefinition(const std::string_view& a_name, const unsigned short a_version,
		const std::vector<std::shared_ptr<RTTIDefinition>>& a_vParents);

	/*
	* @brief check is defined class is inherited from class defined by pDef
	* @param pDef class definition to test
	* @brief return true if defined class is inherited class defined by pDef
	*/
	[[nodiscard]] virtual bool isKindOf(const std::shared_ptr<RTTIDefinition>& pDef)const noexcept;

	/*
	* @return class name
	*/
	[[nodiscard]] constexpr std::string_view className()const { return m_className; }

	/*
	* @brief check is defined class is same class defined by pDef
	* @param pDef class definition to test
	* @brief return true if defined class is same class defined by pDef
	*/
	[[nodiscard]] inline bool isSame(const std::shared_ptr<RTTIDefinition>& pDef)const noexcept
	{
		return pDef.get() == this;
	}

	/*@brief register a protocol extension*/
	void registerProtocolExt(const std::shared_ptr<ProtocolExtension>& protocol);
	/*@brief unregister protocol extension*/
	bool unregisterProtocolExt(const std::shared_ptr<RTTIDefinition>& pDef);

	/*@brief find  a protocol extension*/
	[[nodiscard]] std::shared_ptr<ProtocolExtension> getProtocolExt(const std::shared_ptr<RTTIDefinition>& pDef)const;

	/*@brief find  a protocol extension*/
	template<typename Protocol>
	[[nodiscard]] inline std::shared_ptr<Protocol> getProtocolExt()const
	{
		return std::dynamic_pointer_cast<Protocol>(getProtocolExt(Protocol::definition()));
	}

	/*@return version number of definition*/
	[[nodiscard]] constexpr unsigned short version()const noexcept { return m_version; }

	/*@brief size of described class*/
	[[nodiscard]] virtual size_t classSize()const noexcept = 0;
};

using RTTIDefinitionPtr = std::shared_ptr<RTTIDefinition>;
