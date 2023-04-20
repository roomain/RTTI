#pragma once

/***********************************************
* @headerfile rttiMacro.h
* @date 26 / 06 / 2022
* @author Romain Crespel
************************************************/
#include "RTTI_defs.h"

using namespace RTTI;


//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief Generates RTTI inline functions
//----------------------------------------------------------------------------------------------------------------------------------------
#define RTTI_STATIC_FUNCTIONS(classname)\
	friend class ClassDefinition<classname>; \
private:\
	static inline std::shared_ptr<ClassDefinition<classname>> m_definition = nullptr;\
\
public: \
	[[nodiscard]] static inline std::shared_ptr<IDefinition> const definition()noexcept\
	{\
		return m_definition; \
	}\
	\
	[[nodiscard]] static constexpr size_t classMemorySize() noexcept \
	{\
		return sizeof(classname);\
	}\

#define RTTI_BASE_FUNCTIONS(classname)\
	\
	[[nodiscard]] virtual inline std::shared_ptr<IDefinition> const isA()const noexcept\
	{\
		return classname::m_definition;\
	}\
	[[nodiscard]] virtual inline bool isKindOf(const std::shared_ptr<IDefinition>& def)const\
	{\
		checkNotDefined<classname>();\
		return classname::m_definition->isKindOf(def);\
	}\
	\
	template<typename Type> \
	[[nodiscard]] bool isKindOf()const\
	{\
		return isKindOf(Type::definition());\
	}\
	\
	template<typename Type>\
	[[nodiscard]] Type* const cast()const\
	{\
		Type* pRet = nullptr; \
		if (isKindOf(Type::definition()))\
			pRet = const_cast<Type*>(dynamic_cast<const Type*>(this));\
		return pRet;\
	}\


#define RTTI_BASE_OVERRIDED_FUNCTIONS(classname)\
	\
	[[nodiscard]] virtual inline std::shared_ptr<IDefinition> const isA()const noexcept override\
	{\
		return classname::m_definition;\
	}\
	[[nodiscard]] virtual inline bool isKindOf(const std::shared_ptr<IDefinition>& def)const override\
	{\
		checkNotDefined<classname>();\
		return classname::m_definition->isKindOf(def);\
	}\
	\
	template<typename Type> \
	[[nodiscard]] bool isKindOf()const\
	{\
		return isKindOf(Type::definition());\
	}\
	\
	template<typename Type>\
	[[nodiscard]] Type* const cast()const\
	{\
		Type* pRet = nullptr; \
		if (isKindOf(Type::definition()))\
			pRet = const_cast<Type*>(dynamic_cast<const Type*>(this));\
		return pRet;\
	}\

//---------------------------------------------------------------------------------------------
/// @brief RTTI definition for a class
//---------------------------------------------------------------------------------------------
#define RTTI_DEFINITION(Version, classname)\
RTTI_STATIC_FUNCTIONS(classname)\
RTTI_BASE_FUNCTIONS(classname)\
public: \
	static inline void initDef() \
	{\
		checkAllreadyDefined<classname>(); \
        	classname::m_definition = std::make_shared<ClassDefinition<classname>>(std::string(#classname), Version);\
	}



#define RTTI_DEFINITIONS(Version, classname, ...)\
RTTI_STATIC_FUNCTIONS(classname)\
RTTI_BASE_OVERRIDED_FUNCTIONS(classname)\
public: \
	static inline void initDef() \
	{\
		checkAllreadyDefined<classname>(); \
		checkNotDefined<__VA_ARGS__>(); \
		classname::m_definition = std::make_shared<ClassDefinition<classname>>(std::string(#classname), Version, DefinitionList<__VA_ARGS__>().vector()); \
	}
