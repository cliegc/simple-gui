#pragma once
#include "base_component.hpp"
#include "deleter.hpp"


namespace SimpleGui {
	class Label final : public BaseComponent {
	public:
		explicit Label(std::string_view text = "label");
		~Label() override = default;

		std::string GetText() const;
		void SetText(std::string_view text);

		TextAlignments GetTextAlignments() const;
		void SetTextAlignments(const TextAlignments& alignments);
		void SetTextAlignments(TextAlignment alignmentH, TextAlignment alignmentV);
		void SetTextAlignmentH(TextAlignment alignmentH);
		void SetTextAlignmentV(TextAlignment alignmentV);

		TTF_Direction GetTextDirection() const;
		void SetTextDirection(TTF_Direction direction) const;

		bool IsWrapEnabled() const { return m_wrapEnabled; }
		void SetWrapEnabled(bool enable) { m_wrapEnabled = enable; }

		bool IsSizeFollowText() const {return m_sizeFollowTextEnabled; }
		void SetSizeFollowTextEnabled(bool enable) { m_sizeFollowTextEnabled = enable; }

		void Update() override;
		void Render(Renderer& renderer) override;

	protected:
		void EnteredComponentTree() override;

	private:
		std::string m_text;
		UniqueTextPtr m_ttfText;
		Rect m_textRect;
		bool m_wrapEnabled;
		bool m_sizeFollowTextEnabled;

		TextAlignments m_textAlignments;

		void Init(std::string_view text);
		void AdjustSize(TTF_Text* ttfText);

		void UpdateTextAlignments();
	};
}