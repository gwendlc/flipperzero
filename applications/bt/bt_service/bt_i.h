#pragma once

#include "bt.h"

#include <furi.h>
#include <furi-hal.h>

#include <gui/gui.h>
#include <gui/view_port.h>
#include <gui/view.h>

#include <dialogs/dialogs.h>
#include <power/power_service/power.h>
#include <applications/rpc/rpc.h>

#include "../bt_settings.h"

#define BT_API_UNLOCK_EVENT (1UL << 0)

typedef enum {
    BtStatusOff,
    BtStatusAdvertising,
    BtStatusConnected,
} BtStatus;

typedef enum {
    BtMessageTypeUpdateStatusbar,
    BtMessageTypeUpdateBatteryLevel,
    BtMessageTypePinCodeShow,
    BtMessageTypeKeysStorageUpdated,
    BtMessageTypeSetProfile,
} BtMessageType;

typedef union {
    uint32_t pin_code;
    uint8_t battery_level;
    BtProfile profile;
} BtMessageData;

typedef struct {
    BtMessageType type;
    BtMessageData data;
    bool* result;
} BtMessage;

struct Bt {
    uint8_t* bt_keys_addr_start;
    uint16_t bt_keys_size;
    uint16_t max_packet_size;
    BtSettings bt_settings;
    BtStatus status;
    BtProfile profile;
    osMessageQueueId_t message_queue;
    Gui* gui;
    ViewPort* statusbar_view_port;
    DialogsApp* dialogs;
    DialogMessage* dialog_message;
    Power* power;
    Rpc* rpc;
    RpcSession* rpc_session;
    osEventFlagsId_t rpc_event;
    osEventFlagsId_t api_event;
};
