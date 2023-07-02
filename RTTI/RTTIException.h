#pragma once
#include <exception>
#include <string>

/* @brief RTTI exception*/
class RTTIException : public std::exception
{
public:
	enum class Type : int
	{
		parent_not_defined, /* @brief parent class is not RTTI defined*/
		null_definition,	/* @brief class is not defined*/
		already_defined		/* @brief class RTTI is already defined*/
	};

private:
	Type			m_exceptionType;
	std::string		m_message;

	void genMessage(std::string_view info);

public:
	explicit RTTIException(Type errType, std::string_view info = "");
	RTTIException() = delete;
	virtual ~RTTIException() = default;
	[[nodiscard]] Type type()const noexcept;
	[[nodiscard]] std::string message()const noexcept;
	const char* what()const override;/*!< NOT USED*/
};