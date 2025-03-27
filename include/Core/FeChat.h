#pragma once

#include <AppCore/AppCore.h>

#include <Core/Plugin/PluginManager.h>
#include <Core/Plugin/EventManager.h>

using namespace ultralight;

class FeChat : public AppListener,
              public WindowListener,
              public LoadListener,
              public ViewListener {
 public:
    FeChat();
    virtual ~FeChat();

    // Запуск основного цикла приложения
    virtual void Run();

    // Вызывается непрерывно из главного цикла приложения
    virtual void OnUpdate() override;

    // Вызывается при закрытии окна
    virtual void OnClose(Window* window) override;

    // Вызывается при изменении размеров окна (значения в пикселях)
    virtual void OnResize(Window* window, uint32_t width, uint32_t height) override;

    // Вызывается, когда фрейм закончил загрузку страницы
    virtual void OnFinishLoading(View* caller,
                                 uint64_t frame_id,
                                 bool is_main_frame,
                                 const String& url) override;

    // Вызывается, когда DOM фрейма загрузился
    virtual void OnDOMReady(View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) override;

    // Вызывается, когда страница меняет курсор
    virtual void OnChangeCursor(View* caller, Cursor cursor) override;

    // Вызывается, когда страница меняет заголовок окна
    virtual void OnChangeTitle(View* caller, const String& title) override;

protected:
    RefPtr<App> app_;
    RefPtr<Window> window_;
    RefPtr<Overlay> overlay_;

    PluginManager pluginManager_;
    EventManager eventManager_;
};