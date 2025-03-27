#include <cstdint>
#include <Ultralight/platform/Platform.h>
#include <AppCore/AppCore.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>

#include <Core/FeChat.h>
#include <Config/Config.h>


static const uint32_t WINDOW_WIDTH  = 600;
static const uint32_t WINDOW_HEIGHT = 400;

using namespace ultralight;

FeChat::FeChat() {
    Settings settings;
    settings.force_cpu_renderer = false;

    app_ = App::Create(settings);

    ::Config config;
    const char* appData = std::getenv("APPDATA");
    auto pluginDir = std::filesystem::path(appData) / "ChatKursUltralight/plugins/";
    pluginManager_.loadAllPluginsFrom(pluginDir, eventManager_);



    window_ = Window::Create(app_->main_monitor(),
                           WINDOW_WIDTH,
                           WINDOW_HEIGHT,
                           false,
                           kWindowFlags_Titled | kWindowFlags_Resizable | kWindowFlags_Maximizable);
    window_->SetTitle("FeChat");
    window_->set_listener(this);

    overlay_ = Overlay::Create(window_, window_->width(), window_->height(), 0, 0);

    FeChat::OnResize(window_.get(), window_->width(), window_->height());

    bool firstRun = config.isFirstRun();
    auto embeddedHtml = pluginManager_.getStartupURLOverride(firstRun);
    if (!embeddedHtml.empty()) {
        overlay_->view()->LoadHTML(embeddedHtml.c_str());
    } else {
        // Обычная логика
        if (firstRun)
            overlay_->view()->LoadURL("file:///welcome.html");
        else
            overlay_->view()->LoadURL("file:///app.html");
    }

    app_->set_listener(this);
    overlay_->view()->set_load_listener(this);
    overlay_->view()->set_view_listener(this);
}

FeChat::~FeChat() = default;

void FeChat::Run() {
    app_->Run();
}

void FeChat::OnUpdate() {

}

void FeChat::OnClose(Window* window) {
    app_->Quit();
}

void FeChat::OnResize(Window* window, uint32_t width, uint32_t height) {
    overlay_->Resize(width, height);
}

void FeChat::OnFinishLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {

}

void FeChat::OnDOMReady(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {

}

void FeChat::OnChangeCursor(View* caller, Cursor cursor) {
    window_->SetCursor(cursor);
}

void FeChat::OnChangeTitle(View* caller, const String& title) {
    window_->SetTitle(title.utf8().data());
}
