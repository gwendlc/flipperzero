#pragma once
#include <map>
#include <list>

#include "ibutton-view-manager.h"
#include "scene/ibutton-scene-generic.h"
#include "scene/ibutton-scene-start.h"
#include "scene/ibutton-scene-read.h"
#include "scene/ibutton-scene-cli-read.h"
#include "scene/ibutton-scene-read-crc-error.h"
#include "scene/ibutton-scene-read-not-key-error.h"
#include "scene/ibutton-scene-read-success.h"
#include "scene/ibutton-scene-readed-key-menu.h"
#include "scene/ibutton-scene-write.h"
#include "scene/ibutton-scene-cli-write.h"
#include "scene/ibutton-scene-write-success.h"
#include "scene/ibutton-scene-saved-key-menu.h"
#include "scene/ibutton-scene-delete-confirm.h"
#include "scene/ibutton-scene-delete-success.h"
#include "scene/ibutton-scene-emulate.h"
#include "scene/ibutton-scene-cli-emulate.h"
#include "scene/ibutton-scene-save-name.h"
#include "scene/ibutton-scene-save-success.h"
#include "scene/ibutton-scene-info.h"
#include "scene/ibutton-scene-select-key.h"
#include "scene/ibutton-scene-add-type.h"
#include "scene/ibutton-scene-add-value.h"

#include "helpers/key-worker.h"

#include <sd-card-api.h>
#include <filesystem-api.h>
#include "../cli/cli.h"

#include "one_wire_master.h"
#include "maxim_crc.h"
#include "ibutton-key.h"

#include <notification/notification-messages.h>

class iButtonApp {
public:
    void run(void);

    iButtonApp();
    ~iButtonApp();

    enum class Scene : uint8_t {
        SceneExit,
        SceneStart,
        SceneRead,
        SceneCliRead,
        SceneReadNotKeyError,
        SceneReadCRCError,
        SceneReadSuccess,
        SceneReadedKeyMenu,
        SceneWrite,
        SceneCliWrite,
        SceneWriteSuccess,
        SceneEmulate,
        SceneCliEmulate,
        SceneSavedKeyMenu,
        SceneDeleteConfirm,
        SceneDeleteSuccess,
        SceneSaveName,
        SceneSaveSuccess,
        SceneInfo,
        SceneSelectKey,
        SceneAddType,
        SceneAddValue,
    };

    enum class CliEvent : uint8_t {
        CliReadSuccess,
        CliReadCRCError,
        CliReadNotKeyError,
        CliWriteSuccess,
        CliWriteFail,
        CliTimeout,
        CliInterrupt,
    };

    iButtonAppViewManager* get_view_manager();
    void switch_to_next_scene(Scene index);
    void search_and_switch_to_previous_scene(std::initializer_list<Scene> scenes_list);
    bool switch_to_previous_scene(uint8_t count = 1);
    Scene get_previous_scene();

    const GpioPin* get_ibutton_pin();
    KeyWorker* get_key_worker();
    iButtonKey* get_key();

    void notify_green_blink();
    void notify_yellow_blink();
    void notify_red_blink();

    void notify_error();
    void notify_success();
    void notify_green_on();
    void notify_green_off();
    void notify_red_on();
    void notify_red_off();

    void set_text_store(const char* text...);
    char* get_text_store();
    uint8_t get_text_store_size();

    SdCard_Api* get_sd_ex_api();
    FS_Api* get_fs_api();
    char* get_file_name();
    uint8_t get_file_name_size();

    void cli_cmd_callback(Cli* cli, string_t args, void* context);
    void cli_send_event(CliEvent scene);

    void generate_random_name(char* name, uint8_t max_name_size);

    bool save_key(const char* key_name);
    bool load_key();
    bool delete_key();

private:
    std::list<Scene> previous_scenes_list = {Scene::SceneExit};
    Scene current_scene = Scene::SceneStart;
    iButtonAppViewManager view;
    osMessageQueueId_t cli_event_result;

    std::map<Scene, iButtonScene*> scenes = {
        {Scene::SceneStart, new iButtonSceneStart()},
        {Scene::SceneRead, new iButtonSceneRead()},
        {Scene::SceneCliRead, new iButtonSceneCliRead()},
        {Scene::SceneReadCRCError, new iButtonSceneReadCRCError()},
        {Scene::SceneReadNotKeyError, new iButtonSceneReadNotKeyError()},
        {Scene::SceneReadSuccess, new iButtonSceneReadSuccess()},
        {Scene::SceneReadedKeyMenu, new iButtonSceneReadedKeyMenu()},
        {Scene::SceneWrite, new iButtonSceneWrite()},
        {Scene::SceneCliWrite, new iButtonSceneCliWrite()},
        {Scene::SceneWriteSuccess, new iButtonSceneWriteSuccess()},
        {Scene::SceneEmulate, new iButtonSceneEmulate()},
        {Scene::SceneCliEmulate, new iButtonSceneCliEmulate()},
        {Scene::SceneSavedKeyMenu, new iButtonSceneSavedKeyMenu()},
        {Scene::SceneDeleteConfirm, new iButtonSceneDeleteConfirm()},
        {Scene::SceneDeleteSuccess, new iButtonSceneDeleteSuccess()},
        {Scene::SceneSaveName, new iButtonSceneSaveName()},
        {Scene::SceneSaveSuccess, new iButtonSceneSaveSuccess()},
        {Scene::SceneInfo, new iButtonSceneInfo()},
        {Scene::SceneSelectKey, new iButtonSceneSelectKey()},
        {Scene::SceneAddType, new iButtonSceneAddType()},
        {Scene::SceneAddValue, new iButtonSceneAddValue()},
    };

    KeyWorker* key_worker;

    iButtonKey key;

    SdCard_Api* sd_ex_api;
    FS_Api* fs_api;
    Cli* cli;
    static const uint8_t file_name_size = 100;
    char file_name[file_name_size];

    static const uint8_t text_store_size = 128;
    char text_store[text_store_size + 1];

    NotificationApp* notification;

    bool read_hex_byte(string_t arg, uint8_t* byte);
    void print_key_data(void);

    static const char* app_folder;
    static const char* app_extension;

    void show_file_error_message(const char* error_text);
};