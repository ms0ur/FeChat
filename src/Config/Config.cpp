#include <iostream>
#include <Config/Config.h>


Config::Config() {
    this->configPath = getDefaultPath();
    this->load();
}

std::filesystem::path Config::getDefaultPath() {
    const char* appData = std::getenv("APPDATA");
    return std::filesystem::path(appData) / "ChatKursUltralight/config.json";
}

bool Config::isFirstRun() const {
    return this->firstRun;
}



void Config::load() {
    if (std::filesystem::exists(configPath)) {
        std::ifstream file(configPath);
        try {
            this->data = json::parse(file);
            this->firstRun = false;
        } catch (const json::parse_error& e) {
            std::cerr << "Ошибка разбора JSON: " << e.what() << "\n";
            this->data = json{};
            this->firstRun = true;
        }
    } else {
        this->data = json{
            {"version", "1.0"},
            {"openConfigScreenNextTime", false},
            {"server", {
                {"hostname", ""},
                {"password", ""}
                }
            },
            {"user", {
                {"username", ""},
                {"token", ""}
                }
            }
            };
        std::filesystem::create_directories(configPath.parent_path());
        save();
        this->firstRun = true;
    }
}


void Config::save() const {
    std::ofstream file(this->configPath);
    file << this->data;
}

bool Config::fileExists() const {
    return std::filesystem::exists(configPath);
}

bool Config::hasField(const std::string& key) const {
    return data.contains(key);
}

std::string Config::getField(const std::string& key) const {
    return this->data.value(key, "");
}

void Config::setField(const std::string& key, const std::string& value) {
    this->data[key] = value;
}

Config::~Config() {
    this->save();
}