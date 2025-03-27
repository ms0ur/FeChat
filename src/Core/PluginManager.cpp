#include <Core/Plugin/PluginManager.h>
#include <iostream>
#include <filesystem>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

// Внутренние функции для работы с динамическими библиотеками
namespace {
    LibHandle loadLibrary(const std::string& path) {
    #ifdef _WIN32
        return LoadLibraryA(path.c_str());
    #else
        return dlopen(path.c_str(), RTLD_LAZY);
    #endif
    }

    void* getSymbol(LibHandle lib, const std::string& symbol) {
    #ifdef _WIN32
        return reinterpret_cast<void*>(GetProcAddress(lib, symbol.c_str()));
    #else
        return dlsym(lib, symbol.c_str());
    #endif
    }

    void unloadLibrary(LibHandle lib) {
    #ifdef _WIN32
        FreeLibrary(lib);
    #else
        dlclose(lib);
    #endif
    }
} // anonymous namespace

// Деструктор: удаляет плагины и выгружает библиотеки
PluginManager::~PluginManager() {
    for (size_t i = 0; i < plugins.size(); ++i) {
        delete plugins[i];
        unloadLibrary(libraryHandles[i]);
    }
}

// Загружает все плагины из указанной директории.
// Возвращает true, если хотя бы один плагин был успешно загружен.
bool PluginManager::loadAllPluginsFrom(const std::filesystem::path& dir, EventManager& eventManager) {
    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
        std::cerr << "Директория для плагинов не существует: " << dir << std::endl;
        return false;
    }
    bool anyLoaded = false;
    for (auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            // Проверяем расширение: .dll для Windows, .so для Linux
#ifdef _WIN32
            if (entry.path().extension() == ".dll") {
#else
            if (entry.path().extension() == ".so") {
#endif
                if (loadPlugin(entry.path().string(), eventManager)) {
                    anyLoaded = true;
                }
            }
        }
    }
    return anyLoaded;
}

// Загружает плагин по указанному пути.
// При успешной загрузке плагина возвращает true.
bool PluginManager::loadPlugin(const std::string& pluginPath, EventManager& eventManager) {
    LibHandle lib = loadLibrary(pluginPath);
    if (!lib) {
#ifdef _WIN32
        std::cerr << "Не удалось загрузить плагин: " << pluginPath << std::endl;
#else
        std::cerr << "Ошибка загрузки плагина: " << dlerror() << std::endl;
#endif
        return false;
    }
    auto createFunc = reinterpret_cast<CreatePluginFunc>(getSymbol(lib, "create_plugin"));
    if (!createFunc) {
        std::cerr << "Не удалось найти функцию create_plugin в " << pluginPath << std::endl;
        unloadLibrary(lib);
        return false;
    }
    if (Plugin* plugin = createFunc()) {
        std::cout << "Загружен плагин: " << plugin->name() << std::endl;
        // Инициализируем плагин, передавая ему ссылку на EventManager
        plugin->initialize(eventManager);
        plugins.push_back(plugin);
        libraryHandles.push_back(lib);
        return true;
    }
    unloadLibrary(lib);
    return false;
}

// Вызывает метод execute() для всех загруженных плагинов.
void PluginManager::executeAll() {
    for (auto plugin : plugins) {
        plugin->execute();
    }
}

std::string PluginManager::getStartupURLOverride(bool firstRun) {
        for (auto plugin : plugins) {
            std::string html = getStartupURLOverride(plugin);
            if (!html.empty()) {
                return html; // Взяли первую же непустую строку
            }
        }
        return std::string();
}