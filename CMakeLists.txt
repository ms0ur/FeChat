cmake_minimum_required(VERSION 3.30)
set(APP_NAME ChatKursUltralight)


project(${APP_NAME})

set(CMAKE_CXX_STANDARD 20)


find_package(nlohmann_json CONFIG REQUIRED)


# Указываем пути к Ultralight SDK
set(ULTRALIGHT_SDK_DIR "C:/UltralightSDK")
set(ULTRALIGHT_BINARY_DIR "${ULTRALIGHT_SDK_DIR}/bin")
set(ULTRALIGHT_RESOURCES_DIR "${ULTRALIGHT_SDK_DIR}/resources")

include_directories("${ULTRALIGHT_SDK_DIR}/include")
link_directories("${ULTRALIGHT_SDK_DIR}/lib")

include_directories("include")



link_libraries(UltralightCore AppCore Ultralight WebCore)
file(GLOB_RECURSE ASSET_FILES "assets/*")
file(GLOB_RECURSE ASSET_FILES_S "assets/**/*")
add_executable(${APP_NAME} WIN32 MACOSX_BUNDLE "src/main.cpp"
        ${ASSET_FILES}
        ${ASSET_FILES_S}
        src/Config/Config.cpp
        src/Core/FeChat.cpp
        src/Core/PluginManager.cpp
)

target_link_libraries(${APP_NAME} PRIVATE
        nlohmann_json::nlohmann_json
)



if (APPLE)
    set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in)
endif()

if (MSVC)
    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

# Создаем целевую директорию для ресурсов
add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_PATH}/resources"
)

# Копируем бинарные файлы Ultralight
add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ULTRALIGHT_BINARY_DIR}"
        "$<TARGET_FILE_DIR:${APP_NAME}>"
)

# Устанавливаем путь для ассетов
if (APPLE)
    set(ASSETS_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/../Resources/assets")
else()
    set(ASSETS_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/assets")
endif()

# Копируем пользовательские ассеты
add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "${ASSETS_PATH}"
)

# Копируем ресурсы Ultralight
add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ULTRALIGHT_RESOURCES_DIR}"
        "${ASSETS_PATH}/resources"
)
