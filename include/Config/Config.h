#pragma once

#include <filesystem>
#include <cstdlib>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Config {
private:
    json data;
    std::filesystem::path configPath;
    bool firstRun = false;

    static std::filesystem::path getDefaultPath();

public:
    Config();
    ~Config();

    void load();
    void save() const;

    [[nodiscard]] bool isFirstRun() const;

    [[nodiscard]] bool hasField(const std::string& key) const;
    [[nodiscard]] bool fileExists() const;

    [[nodiscard]] std::string getField(const std::string& key) const;
    void setField(const std::string& key, const std::string& value);

};
