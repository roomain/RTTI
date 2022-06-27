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

class UndoRedoProtocolExtension;
class ProtocolExtension;

namespace RTTI
{
    class IDefinition;

	template<typename Definition>
    class DefTree : public std::enable_shared_from_this<DefTree<Definition>>
    {
	protected:
		using DefPtr = std::shared_ptr<Definition>;
		std::vector<DefPtr> m_vParentsDef; /*@brief link to metaclasses definitions*/

		inline auto cbegin()const noexcept { return m_vParentsDef.cbegin(); }
		inline auto cend()const noexcept { return m_vParentsDef.cend(); }

        DefTree() = default;
        DefTree(std::initializer_list<DefPtr> a_init) : m_vParentsDef{a_init}
        {
            //
        }

        ~DefTree()
		{
			// get reference count of this
			//assert(weak_from_this().use_count() == 0, "Class is always referenced !");
		}

        /*
		* @brief check if this inherits from pDef (search recursively)
		* @param pDef: RTTI definition
		* @return true if this inherist from pDef
		*/
		inline bool inheritFrom(const DefPtr& pDef)const noexcept
		{
			bool bFound = std::find(m_vParentsDef.begin(), m_vParentsDef.end(), pDef) != m_vParentsDef.end();
			if (!bFound)
			{
				for (auto&& curdef : m_vParentsDef)
				{
					bFound = curdef->inheritFrom(pDef);
					if (bFound)
						break;
				}
			}
			return bFound;
		}
    };
	    
    class IDefinition : public DefTree<IDefinition>
    {
    protected:        
        /*@brief definition version number*/
		unsigned short m_usVersion;

		/*@brief undo / redo protocol extension */
		std::shared_ptr<UndoRedoProtocolExtension> m_pUndoRedoPE;

		/*@brief Links to protocol extension use by defined class*/
		std::vector<std::shared_ptr<ProtocolExtension>>	m_vProtocoleExt;

		/*@brief class name*/
		std::string m_sClassName;

        
        IDefinition() = delete;
		IDefinition(const std::string& a_className, const unsigned short a_usVers);
        IDefinition(const std::string& a_className, const unsigned short a_usVers, const std::vector<std::shared_ptr<IDefinition>>& a_init);

    public:

		/*@brief set undo/redo protocol extension*/
		inline std::shared_ptr<UndoRedoProtocolExtension> undoRedoPE()const noexcept { return m_pUndoRedoPE; }

		/*@brief get undo/redo protocol extension*/
		void setUndoRedoPE(std::shared_ptr<UndoRedoProtocolExtension> protocolExt) { m_pUndoRedoPE = protocolExt; }

		/*
		* @brief check is defined class is inherited from class defined by pDef
		* @param pDef class definition to test
		* @brief return true if defined class is inherited class defined by pDef
		*/
		virtual bool isKindOf(const std::shared_ptr<IDefinition>& pDef)const noexcept;

		/*
		* @return class name
		*/
		inline std::string className()const { return m_sClassName; }

		/*
		* @brief check is defined class is same class defined by pDef
		* @param pDef class definition to test
		* @brief return true if defined class is same class defined by pDef
		*/
		virtual bool isSame(const std::shared_ptr<IDefinition>& pDef)const noexcept;

		void registerProtocolExt(const std::shared_ptr<ProtocolExtension>& protocol);
		bool unregisterProtocolExt(const std::shared_ptr<IDefinition>& pDef);
		std::shared_ptr<ProtocolExtension> getProtocolExt(const std::shared_ptr<IDefinition>& pDef)const;

		template<typename Protocol>
		inline std::shared_ptr<Protocol> getProtocolExt()const
		{
			return std::dynamic_pointer_cast<Protocol>(getProtocolExt(Protocol::definition()));
		}

		/*@return version number of definition*/
		inline unsigned short version()const noexcept { return m_usVersion; }

		/*@brief size of described class*/
		virtual size_t classSize()const noexcept = 0;
    };
	using IDefinitionPtr = std::shared_ptr<IDefinition>;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    /*@brief RTTI Class template definition*/
	template<typename Class>
	class ClassDefinition : public IDefinition
	{

	public:
		ClassDefinition() : m_usVersion{ 0 } {}

		ClassDefinition(const unsigned short a_usVers) : IDefinition{ a_usVers }
		{
			//auto pFun = [&](char* buffer) {return  std::dynamic_pointer_cast<CadKernelObject>(std::shared_ptr<Class>(reinterpret_cast<Class*>(buffer))); };
		}

		ClassDefinition(const std::string& a_classname, const unsigned short a_usVers) : IDefinition{ a_classname, a_usVers }
		{
			//auto pFun = [&](char* buffer) {return  std::dynamic_pointer_cast<CadKernelObject>(std::shared_ptr<Class>(reinterpret_cast<Class*>(buffer))); };
		}

		ClassDefinition(const std::string& a_classname, const unsigned short a_usVers, const std::vector<std::shared_ptr<IDefinition>>& a_init) : IDefinition(a_classname, a_usVers, a_init)
		{
			//auto pFun = [&](char* buffer) {return  std::dynamic_pointer_cast<CadKernelObject>(std::shared_ptr<Class>(reinterpret_cast<Class*>(buffer))); };
		}


		inline std::shared_ptr<Class> createObject()
		{
			return std::make_shared<Class>();
		}

		size_t classSize()const noexcept final
		{
			return sizeof(Class);
		}
	};

	/* @brief RTTI exception*/
	class Exception : public std::exception
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
		std::string		m_info;

	public:
		Exception(Type errType, const std::string& info = "");
		Type type()const noexcept;
		std::string message()const noexcept;
		const char* what()const override;
	};

	template<typename Type, typename ...others>
	void checkAllreadyDefined();

	inline void checkAllreadyDefinedAux(std::tuple<>&& a_tuple) {}

	template<typename ...others>
	void checkAllreadyDefinedAux(std::tuple<others...>&& a_tuple)
	{
		return checkAllreadyDefined<others...>();
	}

    /*@brief Check if RTTI definition of classname already exist => produce an exception if not*/
    template<typename Type, typename ...others>
    void checkAllreadyDefined()
    {
        if(Type::definition())
            throw Exception(Exception::Type::already_defined, Type::definition()->className());
		checkAllreadyDefinedAux(std::tuple<others...>());
    }


	//------------------------------------------------------------------------------------------------------------------------
    /*@brief Check if RTTI definition of classname not exist => produce an exception if not*/
	template<typename Type, typename ...others>
	void checkNotDefined();

	inline void checkNotDefinedAux(std::tuple<>&& a_tuple) {}

	template<typename ...others>
	void checkNotDefinedAux(std::tuple<others...>&& a_tuple)
	{
		return checkNotDefined<others...>();
	}

	/*@brief Check if RTTI definition of classname already exist => produce an exception if not*/
	template<typename Type, typename ...others>
	void checkNotDefined()
	{
		if (nullptr == Type::definition().get())
			throw Exception(Exception::Type::parent_not_defined, typeid(Type).name());
		checkNotDefinedAux(std::tuple<others...>());
	}

	//------------------------------------------------------------------------------------------------------------------------

	template<typename A, typename ...Types>
	class DefinitionList
	{
	private:
		std::vector<IDefinitionPtr> m_vDefs;

		static inline void internfillVecAux(std::vector<IDefinitionPtr>& vec, std::tuple<>&& empty) { }

		template<typename... args>
		static void internfillVecAux(std::vector<IDefinitionPtr>& vec, std::tuple<args...>&& empty)
		{
			internfillVec<args...>(vec);
		}

		template<typename A, typename... args>
		static void internfillVec(std::vector<IDefinitionPtr>& vec)
		{
			vec.push_back(A::definition());
			internfillVecAux(vec, std::tuple<args...>());
		}

	public:
		DefinitionList()
		{
			internfillVec<A, Types...>(m_vDefs);
		}

		inline const std::vector<IDefinitionPtr>& vector()const noexcept
		{
			return m_vDefs;
		}
	};

	template<typename Type, typename ...others>
	void listDefinitions(std::vector<IDefinitionPtr>& vec);

	inline void listDefinitionsAux(std::vector<IDefinitionPtr>& vec, std::tuple<>&& a_tuple) {}

	template<typename ...others>
	void listDefinitionsAux(std::vector<IDefinitionPtr>& vec, std::tuple<others...>&& a_tuple)
	{
		listDefinitions<others...>(vec);
	}

	/*@brief Check if RTTI definition of classname already exist => produce an exception if not*/
	template<typename Type, typename ...others>
	void listDefinitions(std::vector<IDefinitionPtr>& vec)
	{
		if (Type::definition())
			throw Exception(Exception::Type::parent_not_defined, typeid(Type).name());
		vec.push_back(Type::definition());
		listDefinitionsAux<others...>(vec, std::tuple<others...>());
	}

}