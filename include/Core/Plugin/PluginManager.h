#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <Core/Plugin/Plugin.h>
#include <Core/Plugin/EventManager.h>

#ifdef _WIN32
  #include <windows.h>
  typedef HMODULE LibHandle;
#else
#include <dlfcn.h>
typedef void* LibHandle;
#endif

// Определение типа фабричной функции для создания плагина
extern "C" {
    typedef Plugin* (*CreatePluginFunc)();
}

class PluginManager {
public:
    ~PluginManager();

    // Загружает все плагины из указанной директории.
    // Возвращает true, если хотя бы один плагин был успешно загружен.
    bool loadAllPluginsFrom(const std::filesystem::path& dir, EventManager& eventManager);

    // Загружает плагин по указанному пути.
    // Возвращает true при успешной загрузке.
    bool loadPlugin(const std::string& pluginPath, EventManager& eventManager);

    // Вызывает метод execute() для всех загруженных плагинов.
    void executeAll();

    //... hooks
    std::string getStartupURLOverride(bool firstRun);

private:
    std::vector<Plugin*> plugins;
    std::vector<LibHandle> libraryHandles;
};
