#include "pch.h"
#include "CppUnitTest.h"
#include "rtti_res.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestsRTTI
{
	TEST_CLASS(TestsRTTI)
	{
	public:
		
		TEST_METHOD_INITIALIZE(init_definitions)
		{
			if (nullptr == ProtocolExtension::definition())
				ProtocolExtension::initDef();

			if (nullptr == ParentPE::definition())
				ParentPE::initDef();

			if (nullptr == ChildPE::definition())
				ChildPE::initDef();

			if (nullptr == RTTIParent::definition())
				RTTIParent::initDef();

			if (nullptr == RTTIFirstChild::definition())
				RTTIFirstChild::initDef();

			if (nullptr == RTTISecondChild::definition())
				RTTISecondChild::initDef();

			if (nullptr == RTTIGrandChild::definition())
				RTTIGrandChild::initDef();

			if (nullptr == RTTIParent1::definition())
				RTTIParent1::initDef();

			if (nullptr == RTTISecondChildMult::definition())
				RTTISecondChildMult::initDef();

			if (nullptr == RTTISecondChildMult::definition())
				RTTISecondChildMult::initDef();

			if (nullptr == RTTIDecorator<RTTIParent>::definition())
				RTTIDecorator<RTTIParent>::initDef();
		}

		TEST_METHOD_CLEANUP(release_definitions)
		{
			//
		}

		TEST_METHOD(TestVersion)
		{
			RTTIParent* pparent = new RTTIParent(5);
			RTTIFirstChild* pfirstchild = new RTTIFirstChild(10);
			RTTISecondChild* pSecondChild = new RTTISecondChild(20);
			RTTIGrandChild* pGrandChild = new RTTIGrandChild(30);
			RTTIDecorator<RTTIParent>* pRTTIDecorator = new RTTIDecorator<RTTIParent>();

			Assert::IsTrue(1 == pparent->isA()->version());
			Assert::IsTrue(3 == pfirstchild->isA()->version());
			Assert::IsTrue(4 == pSecondChild->isA()->version());
			Assert::IsTrue(5 == pGrandChild->isA()->version());
			Assert::IsTrue(7 == pRTTIDecorator->isA()->version());

			delete pRTTIDecorator;
			delete pparent;
			delete pfirstchild;
			delete pSecondChild;
			delete pGrandChild;
		}

		TEST_METHOD(TestIsA)
		{

			RTTIParent* pparent = new RTTIParent(5);
			RTTIFirstChild* pfirstchild = new RTTIFirstChild(10);
			RTTISecondChild* pSecondChild = new RTTISecondChild(20);
			RTTIGrandChild* pGrandChild = new RTTIGrandChild(30);

			Assert::IsTrue(RTTIParent::definition() == pparent->isA());
			Assert::IsTrue(RTTIFirstChild::definition() == pfirstchild->isA());
			Assert::IsTrue(RTTISecondChild::definition() == pSecondChild->isA());
			Assert::IsTrue(RTTIGrandChild::definition() == pGrandChild->isA());

			Assert::IsTrue(RTTIParent::definition() != pfirstchild->isA());
			Assert::IsTrue(RTTIParent::definition() != pSecondChild->isA());
			Assert::IsTrue(RTTIParent::definition() != pGrandChild->isA());

			Assert::IsTrue(RTTIFirstChild::definition() != pSecondChild->isA());
			Assert::IsTrue(RTTIFirstChild::definition() != pGrandChild->isA());

			delete pparent;
			delete pfirstchild;
			delete pSecondChild;
			delete pGrandChild;

		}

		TEST_METHOD(TestIsKindOf)
		{
			RTTIParent* pparent = new RTTIParent(5);
			RTTIFirstChild* pfirstchild = new RTTIFirstChild(10);
			RTTISecondChild* pSecondChild = new RTTISecondChild(20);
			RTTIGrandChild* pGrandChild = new RTTIGrandChild(30);

			Assert::IsTrue(pparent->isKindOf(RTTIParent::definition()), L"pparent->isKindOf(RTTIParent::definition())");
			Assert::IsTrue(pfirstchild->isKindOf(RTTIFirstChild::definition()), L"pfirstchild->isKindOf(RTTIFirstChild::definition())");
			Assert::IsTrue(pSecondChild->isKindOf(RTTISecondChild::definition()), L"pSecondChild->isKindOf(RTTISecondChild::definition())");
			Assert::IsTrue(pGrandChild->isKindOf(RTTIGrandChild::definition()), L"pGrandChild->isKindOf(RTTIGrandChild::definition())");

			Assert::IsTrue(pparent->isKindOf(RTTIParent::definition()), L"pparent->isKindOf(RTTIParent::definition())");
			Assert::IsTrue(pfirstchild->isKindOf(RTTIParent::definition()), L"pfirstchild->isKindOf(RTTIParent::definition())");

			Assert::IsTrue(pSecondChild->isKindOf(RTTIParent::definition()), L"pSecondChild->isKindOf(RTTIParent::definition())");
			Assert::IsFalse(pSecondChild->isKindOf(RTTIFirstChild::definition()), L"pSecondChild->isKindOf(RTTIFirstChild::definition())");

			Assert::IsTrue(pGrandChild->isKindOf(RTTIParent::definition()), L"pGrandChild->isKindOf(RTTIParent::definition())");
			Assert::IsTrue(pGrandChild->isKindOf(RTTIFirstChild::definition()), L"pGrandChild->isKindOf(RTTIFirstChild::definition())");
			Assert::IsFalse(pGrandChild->isKindOf(RTTISecondChild::definition()), L"pGrandChild->isKindOf(RTTISecondChild::definition())");


			delete pparent;
			delete pfirstchild;
			delete pSecondChild;
			delete pGrandChild;
		}

		TEST_METHOD(TestProtocolExtension_Get)
		{
			std::shared_ptr<ParentPE> pParentPE = std::make_shared<ParentPE>();
			RTTIParent::definition()->registerProtocolExt(pParentPE);
			auto pExt = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExt.get() == pParentPE.get(), L"pExt.get() == pParentPE.get()");

			auto pExtChild = RTTIFirstChild::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExtChild.get() == pParentPE.get(), L"pExt.get() == pParentPE.get() from child");

			auto pExt1 = RTTIParent1::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExt1.get() == nullptr, L"pExt.get() == nullptr");
		}

		TEST_METHOD(TestProtocolExtension_Change)
		{
			std::shared_ptr<ParentPE> pParentPE = std::make_shared<ParentPE>();
			std::shared_ptr<ChildPE> pChildPE = std::make_shared<ChildPE>();
			RTTIParent::definition()->registerProtocolExt(pParentPE);
			auto pExt = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExt.get() == pParentPE.get(), L"pExt.get() == pParentPE.get()");

			RTTIParent::definition()->registerProtocolExt(pChildPE);
			auto pExtChild = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExtChild.get() == pChildPE.get(), L"pExt.get() == pChildPE.get()");

			RTTIParent::definition()->unregisterProtocolExt(pParentPE->isA());
		}

		TEST_METHOD(TestProtocolExtension_Unregister)
		{
			std::shared_ptr<ParentPE> pParentPE = std::make_shared<ParentPE>();
			RTTIParent::definition()->registerProtocolExt(pParentPE);
			auto pExt = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExt.get() == pParentPE.get(), L"pExt.get() == pParentPE.get()");

			RTTIParent::definition()->unregisterProtocolExt(pParentPE->isA());
			auto pExtChild = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExtChild.get() == nullptr, L"pExt.get() == pChildPE.get()");

			RTTIParent::definition()->unregisterProtocolExt(pParentPE->isA());
		}

		TEST_METHOD(TestProtocolExtension_Child)
		{
			std::shared_ptr<ParentPE> pParentPE = std::make_shared<ParentPE>();
			std::shared_ptr<ChildPE> pChildPE = std::make_shared<ChildPE>();
			RTTIParent::definition()->registerProtocolExt(pParentPE);
			RTTIFirstChild::definition()->registerProtocolExt(pChildPE);

			auto pExt = RTTIParent::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExt.get() == pParentPE.get(), L"pExt.get() == pParentPE.get()");

			auto pExtChild = RTTIFirstChild::definition()->getProtocolExt<ParentPE>();
			Assert::IsTrue(pExtChild.get() == pChildPE.get(), L"pExt.get() == pChildPE.get() from child");

			Assert::AreEqual(pExtChild->testPE(), 10, L"pExtChild->test() == 10");
			Assert::AreEqual(pExt->testPE(), 0, L"pExt->test() == 0");

			RTTIParent::definition()->unregisterProtocolExt(pParentPE->isA());
			RTTIFirstChild::definition()->unregisterProtocolExt(pParentPE->isA());
		}

		TEST_METHOD(TestIsA_Multiple)
		{
			RTTISecondChildMult* pMult = new RTTISecondChildMult(5, 3.2);
			Assert::IsTrue(RTTISecondChildMult::definition() == pMult->isA());
			Assert::IsTrue(RTTIParent::definition() != pMult->isA());
			Assert::IsTrue(RTTIParent1::definition() != pMult->isA());
			delete pMult;
		}

		TEST_METHOD(TestIsKindOf_Multiple)
		{
			RTTISecondChildMult* pMult = new RTTISecondChildMult(5, 3.2);

			Assert::IsTrue(pMult->isKindOf(RTTIParent::definition()), L"pMult->isKindOf(RTTIParent::definition())");
			Assert::IsTrue(pMult->isKindOf(RTTIParent1::definition()), L"pMult->isKindOf(RTTIParent1::definition())");
			Assert::IsTrue(pMult->isKindOf(RTTISecondChildMult::definition()), L"pMult->isKindOf(RTTISecondChildMult::definition())");

			delete pMult;
		}

		TEST_METHOD(TestIsA_RTTIDecoratorTemplateInerit)
		{
			RTTIDecorator<RTTIParent>* pRTTIDecorator = new RTTIDecorator<RTTIParent>();
			Assert::IsTrue(RTTIDecorator<RTTIParent>::definition() == pRTTIDecorator->isA(), L"RTTIDecorator<RTTIParent>->isKindOf(RTTIDecorator::definition()");

			delete pRTTIDecorator;
		}

		TEST_METHOD(TestIsKindOf_RTTIDecoratorTemplateInerit)
		{
			RTTIDecorator<RTTIParent>* pRTTIDecorator = new RTTIDecorator<RTTIParent>();
			Assert::IsTrue(RTTIDecorator<RTTIParent>::definition() == pRTTIDecorator->isA());
			RTTIParent* pParent = static_cast<RTTIParent*>(pRTTIDecorator);

			Assert::IsTrue(RTTIDecorator<RTTIParent>::definition() == pParent->isA());
			Assert::IsTrue(pParent->isKindOf(RTTIDecorator<RTTIParent>::definition()), L"pTemplate->isKindOf(RTTIDecorator<RTTIParent>::definition())");

			Assert::IsTrue(pRTTIDecorator->isKindOf(RTTIParent::definition()), L"pTemplate->isKindOf(RTTIParent::definition())");
			Assert::IsTrue(pRTTIDecorator->isKindOf(RTTIDecorator<RTTIParent>::definition()), L"pTemplate->isKindOf(RTTIDecorator<RTTIParent>::definition())");

			delete pRTTIDecorator;
		}

		TEST_METHOD(TestCast)
		{
			RTTIFirstChild* pfirstchild = new RTTIFirstChild(10);
			RTTIGrandChild* pGrandChild = new RTTIGrandChild(30);
			RTTIDecorator<RTTIParent>* pRTTIDecorator = new RTTIDecorator<RTTIParent>();
			RTTISecondChildMult* pMult = new RTTISecondChildMult(5, 3.2);

			// test cast OK
			RTTIParent* pCasRTTIParent = pfirstchild->cast<RTTIParent>();
			pfirstchild->isKindOf(RTTIParent::definition());
			Assert::IsTrue(pCasRTTIParent != nullptr, L"pfirstchild->cast<RTTIParent>()");

			RTTIParent* pCasRTTIParent1 = pMult->cast<RTTIParent>();
			Assert::IsTrue(pCasRTTIParent1 != nullptr, L"pMult->cast<RTTIParent>()");
			Assert::IsTrue(pCasRTTIParent1->data() == 5, L"pCasRTTIParent1->data() == 5");
			RTTIParent1* pCasRTTIParent2 = pMult->cast<RTTIParent1>();
			Assert::IsTrue(pCasRTTIParent2 != nullptr, L"pMult->cast<RTTIParent>()");
			Assert::IsTrue(pCasRTTIParent2->data1() == 3.2, L"pCasRTTIParent2->data1()");

			RTTIParent* pCasRTTIParent3 = pGrandChild->cast<RTTIParent>();
			Assert::IsTrue(pCasRTTIParent3 != nullptr, L"pGrandChild->cast<RTTIParent>()");
			RTTIFirstChild* pCasRTTIFirstChild = pGrandChild->cast<RTTIFirstChild>();
			Assert::IsTrue(pCasRTTIFirstChild != nullptr, L"pMult->cast<RTTIFirstChild>()");

			// test cast NOK
			RTTISecondChild* pCasRTTISecondChild = pfirstchild->cast<RTTISecondChild>();
			Assert::IsTrue(pCasRTTISecondChild == nullptr, L"pfirstchild->cast<RTTISecondChild>()");
			//
			delete pfirstchild;
			delete pGrandChild;
			delete pRTTIDecorator;
			//delete pMult;
		}
	};
}
