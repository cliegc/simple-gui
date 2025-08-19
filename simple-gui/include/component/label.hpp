#pragma once
#include "base_component.hpp"


namespace SimpleGui {
	class Label final : public BaseComponent {
	public:
		Label(std::string_view text);
		~Label() = default;

		std::string GetText() const;
		void SetText(std::string_view text);

		TextAlignments GetTextAlignments() const;
		void SetTextAlignments(const TextAlignments& alignments);
		void SetTextAlignments(TextAlignment alignmentH, TextAlignment alignmentV);
		void SetTextAlignmentH(TextAlignment alignmentH);
		void SetTextAlignmentV(TextAlignment alignmentV);

		TTF_Direction GetTextDirection() const;
		void SetTextDirection(TTF_Direction direction);

		virtual void Update() override;
		virtual void Render(const Renderer& renderer) override;

	private:
		UniqueTextPtr m_ttfText;
		Rect m_textRect;

		TextAlignments m_textAlignments;

		void Init(std::string_view text);
		void AdjustSize(TTF_Text* ttfText);

		void UpdateTextAlignments();
	};
}