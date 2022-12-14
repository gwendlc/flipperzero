#include <furi.h>
#include <api-hal.h>
#include "notification.h"
#include "notification-messages.h"

#define NOTIFICATION_LED_COUNT 3
#define NOTIFICATION_EVENT_COMPLETE 0x00000001U

typedef enum {
    NotificationLayerMessage,
    InternalLayerMessage,
} NotificationAppMessageType;

typedef struct {
    const NotificationSequence* sequence;
    NotificationAppMessageType type;
    osEventFlagsId_t back_event;
} NotificationAppMessage;

typedef enum {
    LayerInternal = 0,
    LayerNotification = 1,
    LayerMAX = 2,
} NotificationLedLayerIndex;

typedef struct {
    uint8_t value[LayerMAX];
    NotificationLedLayerIndex index;
    Light light;
} NotificationLedLayer;

typedef struct {
    float display_brightness;
    float led_brightness;
    uint32_t display_off_delay_ms;
} NotificationSettings;

struct NotificationApp {
    osMessageQueueId_t queue;
    osTimerId_t display_timer;

    NotificationLedLayer display;
    NotificationLedLayer led[NOTIFICATION_LED_COUNT];

    NotificationSettings settings;
};