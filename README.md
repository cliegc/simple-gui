# SimpleGui（dev）

## 介绍
基于SDL3实现的简单易用的GUI框架。

## 支持平台
Windows

## 组件
- Label: 文本显示
- Button: 命令按钮
- CheckBox: 带有文本标签的复选框
- ComboBox: 带有下拉列表的选择框
- DraggablePanel: 可自由移动的展示面板
- LineEdit: 单行文本编辑器
- ProgressBar: 水平或垂直进度条
- ScrollBar: 水平或垂直滚动条
- ScrollPanel: 可滚动显示内容超过显示大小的面板
- Slider: 水平或垂直滑块
- TextureRect: 显示图片的矩形区域
- ...

## 布局
- BoxLayout: 水平或垂直排列子组件
- AnchorPointLayout: 任意固定子组件的显示位置
- ...

## 示例

```c++
#include <simple_gui.hpp>
using namespace SimpleGui;


int main(int argc, char** argv) {
    // 初始化simple-gui程序
    GuiManager::Init(argc, argv, R"(C:\WINDOWS\Fonts\msyh.ttc)");

    // 创建一个窗口
    auto& win = SG_GuiManager.GetWindow("simple gui", 300, 180);

    // 使用垂直盒子布局，并设置大小占据整个窗口
    auto layout = win.AddComponent<BoxLayout>(Direction::Vertical);
    layout->SetSizeConfigs(ComponentSizeConfig::Expanding, ComponentSizeConfig::Expanding);

    // 在布局中添加一个标签
    auto lbl = layout->AddChild<Label>("hello world");

    // 设置标签文字的对齐方式，水平居中，垂直居中
    lbl->SetTextAlignments(TextAlignment::Center, TextAlignment::Center);

    // 设置标签高度为扩充模式
    lbl->SetSizeConfigH(ComponentSizeConfig::Expanding);

    // 在布局中添加一个按钮
    auto btn = layout->AddChild<Button>("click me");

    static int clickedCount = 0;

    // 为按钮的点击操作添加一个回调函数
    btn->clicked.Connect("on_clicked", [lbl] {
       clickedCount++;
       lbl->SetText(std::format("hello world: {}", clickedCount));
    });

    // 启用窗口垂直同步。启用失败，则设置显示帧率的60帧
    if (win.EnableVsync(true)) {
        SG_GuiManager.SetTargetFrameRate(60);
    }
    
    // 运行simple-gui程序
    SG_GuiManager.Run();
    
    // 程序退出，释放所有内存
    GuiManager::Quit();
}
```

![](.\screenshot\example1.png)

## 第三方库

- SDL3: [libsdl-org/SDL: Simple DirectMedia Layer](https://github.com/libsdl-org/SDL)
- SDL_ttf: [libsdl-org/SDL_ttf: Support for TrueType (.ttf) font files with Simple Directmedia Layer.](https://github.com/libsdl-org/SDL_ttf)
- SDL_image: [libsdl-org/SDL_image: Image decoding for many popular formats for Simple Directmedia Layer.](https://github.com/libsdl-org/SDL_image)
- utf8cpp: [nemtrif/utfcpp: UTF-8 with C++ in a Portable Way](https://github.com/nemtrif/utfcpp)
- tinyxml2: [leethomason/tinyxml2: TinyXML2 is a simple, small, efficient, C++ XML parser that can be easily integrated into other programs.](https://github.com/leethomason/tinyxml2)