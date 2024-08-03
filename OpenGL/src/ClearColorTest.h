#pragma once
#include "TestBase.h"

namespace Test
{
	class ClearColorTest : public TestBase
	{
	public:
		ClearColorTest();
		~ClearColorTest();

		void OnUpdata(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_Color[4];
	};
}
