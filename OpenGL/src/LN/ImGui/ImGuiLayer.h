#pragma once

namespace LN {

	class ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event& event) override;
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void Begin();
		void End();

	private:
		bool m_BlockEvents = true;
	};
}
