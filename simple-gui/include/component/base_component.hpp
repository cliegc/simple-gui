#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "renderer.hpp"
#include "style.hpp"
#include "math.hpp"
#include "timer.hpp"
#include "extended_functions.hpp"
#include "common/types.hpp"


namespace SimpleGui {

#define SG_CMP_HANDLE_EVENT_CONDITIONS_FALSE if (!m_visible) return false;\
										  if (m_disabled) return false;\
										  /*if (m_visibleRect.size.IsZeroApprox()) return false;*/

#define SG_CMP_UPDATE_CONDITIONS if (!m_visible) return

#define SG_CMP_RENDER_CONDITIONS if (!m_visible) return;\
								 if (m_visibleGRect.size.IsZeroApprox()) return

	class Window;
	class Event;

	class BaseComponent {
	public:
		BaseComponent();
		virtual ~BaseComponent() = default;

		virtual bool HandleEvent(Event* event);
		virtual void Update();
		virtual void Render(Renderer& renderer);

		Rect GetRect() const { return Rect{ m_position, m_size}; }
		Rect GetGlobalRect() const { return Rect{ GetGlobalPosition(), m_size}; }
		Rect GetVisibleGlobalRect() const { return m_visibleGRect; }
		Rect GetContentGlobalRect() const;

		// 基于global_position的坐标系统，无法在组件被添加到父组件之前有效设置position(局部)
		// 采用基于position(局部)的坐标系统，通过计算获得global_position

		Vec2 GetPosition() const { return m_position; }
		void SetPosition(const Vec2 pos) { m_position = pos; }
		void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; }
		void SetPositionX(float x) { m_position.x = x; }
		void SetPositionY(float y) { m_position.y = y; }

		Vec2 GetGlobalPosition() const;
		void SetGlobalPosition(const Vec2& pos);
		void SetGlobalPosition(float x, float y);
		void SetGlobalPositionX(float x);
		void SetGlobalPositionY(float y);

		Vec2 MapPositionToGlobal(const Vec2& pos) const;
		Vec2 MapGlobalPositionToLocal(const Vec2& pos) const;

		Vec2 GetSize() const { return m_size; }
		void SetSize(const Vec2& size);
		void SetSize(float w, float h);
		void SetWidth(float w);
		void SetHeight(float h);

		Vec2 GetMinSize() const { return m_minSize; }
		void SetMinSize(const Vec2& size);
		void SetMinSize(float w, float h);
		void SetMinWidth(float w);
		void SetMinHeight(float h);

		ComponentPadding GetPadding() const { return m_padding; }
		void SetPadding(const ComponentPadding& padding) { m_padding = padding; }
		void SetPadding(int left, int top, int right, int bottom) {
			m_padding.left = left; m_padding.top = top; m_padding.right = right; m_padding.bottom = bottom; }

		ComponentSizeConfigs GetSizeConfigs() const { return m_sizeConfigs; }
		void SetSizeConfigs(const ComponentSizeConfigs& config) { m_sizeConfigs = config; }
		void SetSizeConfigs(ComponentSizeConfig wConfig, ComponentSizeConfig hConfig) {
			m_sizeConfigs.first = wConfig; m_sizeConfigs.second = hConfig; }
		void SetSizeConfigW(ComponentSizeConfig config) { m_sizeConfigs.first = config; }
		void SetSizeConfigH(ComponentSizeConfig config) { m_sizeConfigs.second = config; }

		bool IsVisible() const { return m_visible; }
		void SetVisible(bool visible);

		bool IsDisabled() const { return m_disabled; }
		void SetDisabled(bool disabled) { m_disabled = disabled; }

		template<typename T, typename...Args>
		T* AddChild(Args&& ...args) {
			static_assert(std::is_base_of_v<BaseComponent, T>, "T 必须继承自 BaseComponent");
			auto cmp = std::make_unique<T>(std::forward<Args>(args)...);
			auto cmp_ptr = cmp.get();
			AddChild(std::move(cmp));
			return cmp_ptr;
		}

		template<typename T, typename...Args>
		T* AddChildDeferred(Args&& ...args) {
			static_assert(std::is_base_of_v<BaseComponent, T>, "T 必须继承自 BaseComponent");
			auto cmp = std::make_unique<T>(std::forward<Args>(args)...);
			auto cmp_ptr = cmp.get();
			AddChildDeferred(std::move(cmp));
			return cmp_ptr;
		}

		BaseComponent* GetParent() const { return m_parent; }
		Window* GetWindow() const { return m_window; }

		//void PresetChildrenCount(size_t count);

		void AddChild(std::unique_ptr<BaseComponent> child);
		void AddChildDeferred(std::unique_ptr<BaseComponent> child);
		BaseComponent* GetChildAt(size_t idx) const;
		virtual std::unique_ptr<BaseComponent> RemoveChild(BaseComponent* cmp);
		virtual std::unique_ptr<BaseComponent> RemoveChildDeferred(BaseComponent* cmp);

		std::vector<std::unique_ptr<BaseComponent>>& GetChildren() { return m_children; };
		size_t GetChildrenCount() const;
		bool HasChild(BaseComponent* cmp);
		void ClearAllChildren();
		void ClearAllChildrenDeferred() const;
		void ForEachChild(const std::function<void(BaseComponent*)>& fn);

		// size_t GetIndex() const { return m_index; };
		// void SetIndex(size_t idx);

		virtual void SetFont(std::unique_ptr<Font> font) { m_font = std::move(font); };
		virtual void SetFont(std::string_view path, int size);
		virtual Font& GetFont();

		Color GetThemeColor(ThemeColorFlags flag);
		virtual void CustomThemeColor(ThemeColorFlags flag, const Color& color);
		virtual void ClearCustomThemeColor(ThemeColorFlags flag);
		virtual void ClearCustomThemeColors();

		bool ToolTipEnabled() const { return m_toolTip->enabled; };
		void SetToolTipEnabled(bool enabled) const { m_toolTip->enabled = enabled; };

		template<typename T, typename...Args>
		T* SetToolTip(Args&& ...args) {
			static_assert(std::is_base_of_v<BaseComponent, T>, "T 必须继承自 BaseComponent");
			auto cmp = std::make_unique<T>(std::forward<Args>(args)...);
			auto ptr = cmp.get();
			m_toolTip->cmp = std::move(cmp);
			return ptr;
		}

		BaseComponent* GetToolTipComponent() const { return m_toolTip->cmp.get(); }

		template<typename T, typename ...Args>
		T* AddExtendedFunctions(Args&& ...args) {
			return m_extFunctionsManager->AddExtendedFunctions<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename ...Args>
		T* AddExtendedFunctionsDeferred(Args&& ...args) {
			return m_extFunctionsManager->AddExtendedFunctionsDeferred<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		std::unique_ptr<ExtendedFunctions> RemoveExtendedFunctions() {
			return m_extFunctionsManager->RemoveExtendedFunctions<T>();
		}

		template<typename T>
		std::unique_ptr<ExtendedFunctions> RemoveExtendedFunctionsDeferred() {
			return m_extFunctionsManager->RemoveExtendedFunctionsDeferred<T>();
		}

		void ClearAllExtendedFunctions() const { m_extFunctionsManager->Clear(); }
		void ClearAllExtendedFunctionsDeferred() const { m_extFunctionsManager->ClearDeferred(); }

	public:
		Signal<bool> visibleChanged;
		Signal<> enteredTree;
		Signal<> exitedTree;
		Signal<> resized;

	protected:
		class ToolTip final {
		public:
			const float MOUSE_SATY_DURATION = 1.0f;

			BaseComponent *target{};
			std::unique_ptr<BaseComponent> cmp{};
			std::unique_ptr<Timer> m_timer{};
			bool enabled{};
			bool isFirstShown{};

			explicit ToolTip(BaseComponent *target);
			~ToolTip() = default;

			void HandleEvent(Event *event) const;
			void Update() const;
			void Render(Renderer &renderer) const;
			void SetSafePositionForComponent() const;
		};

	protected:
		Vec2 m_position;			// 局部坐标
		Vec2 m_size;
		Vec2 m_minSize;
		Rect m_visibleGRect;		// 使用全局坐标

		ComponentPadding m_padding{};
		ComponentSizeConfigs m_sizeConfigs;

		bool m_visible = true;
		bool m_disabled = false;
		bool m_needRemove = false;

		std::unique_ptr<Font> m_font;
		std::unordered_map<ThemeColorFlags, Color> m_themeColorCaches;

		Window* m_window = nullptr;
		BaseComponent* m_parent = nullptr;
		std::unique_ptr<ToolTip> m_toolTip{};
		std::vector<std::unique_ptr<BaseComponent>> m_children;
		std::vector<std::unique_ptr<BaseComponent>> m_childCaches;
		std::unique_ptr<ExtendedFunctionsManager> m_extFunctionsManager;

	protected:
		virtual void EnteredComponentTree() {};
		virtual void ExitedComponentTree() {};

		void SetComponentOwner(BaseComponent* cmp, Window* window, BaseComponent* parent) const {
			cmp->m_window = window;
			cmp->m_parent = parent;
		}

		// 局部坐标的原点位置就是内容矩形的左上角位置
		// 若派生组件的局部坐标原点位置不是组件的左上角，则需要重写该函数
		inline virtual Vec2 GetLocalCoordinateOriginOffset() const;
		// 若派生组件的内容矩形大小不为组件大小减去padding大小，则需要重写该函数 （局部坐标的原点位置就是内容矩形的左上角位置）
		inline virtual Vec2 GetContentSize() const;

		// 方便在该组件内获取获取其他组件的所有子组件
		std::vector<std::unique_ptr<BaseComponent>>& GetComponentChildren(BaseComponent* cmp) const { return cmp->m_children; }

		// 方便在该组件内获取/设置其他组件的受保护数据，获取内容矩形的大小
		Vec2 GetComponentContentSize(BaseComponent* cmp) const { return cmp->GetContentSize(); }

		// 方便在该组件内获取/设置其他组件的受保护数据，设置可见矩形大小，在cmp调用CalcVisibleGlobalRect更新可见矩形之后调用该函数才生效
		void SetComponentVisibleGlobalRect(BaseComponent* cmp, const Rect& rect) const { cmp->m_visibleGRect = rect; }

		void EnteredComponentTree(BaseComponent* cmp) const { cmp->EnteredComponentTree(); }
		void ExitedComponentTree(BaseComponent* cmp) const { cmp->ExitedComponentTree(); }

		void PreparationOfUpdateChildren();
		void UpdateChildSizeConfigs(BaseComponent* cmp) const;
		void CalcVisibleGlobalRect(BaseComponent* parent, BaseComponent* target) const;
		Rect CalcVisibleGlobalRect(const Rect& parentVisibleGRect, const Rect& parentContentGRect, const Rect& targetGRect) const;

		// 计算包含所有子组件的最小矩形
		// 在PreparationOfUpdateChildren之后调用
		Rect CalcChildrenBoundaryGlobalRect(BaseComponent* cmp) const;
	};
}