//#include "pch.h"
#include "RTTIException.h"
#include <format>

RTTIException::RTTIException(RTTIException::Type errType, std::string_view info) : m_exceptionType{ errType }
{
	genMessage(info);
}

RTTIException::Type RTTIException::type()const noexcept
{
	return m_exceptionType;
}

std::string RTTIException::message()const noexcept
{
	return m_message;
}

void RTTIException::genMessage(std::string_view info)
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

const char* RTTIException::what()const
{
	return m_message.c_str();
}