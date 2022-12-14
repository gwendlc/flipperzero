#include <api-hal-bt.h>
#include <app_entry.h>
#include <ble.h>
#include <stm32wbxx.h>
#include <shci.h>
#include <cmsis_os2.h>
#include <app_ble.h>

void api_hal_bt_init() {
    // Explicitly tell that we are in charge of CLK48 domain
    HAL_HSEM_FastTake(CFG_HW_CLK48_CONFIG_SEMID);
    // Start Core2, init HCI and start GAP/GATT
    APPE_Init();
}

bool api_hal_bt_start_app() {
    return APP_BLE_Start();
}

void api_hal_bt_dump_state(string_t buffer) {
    BleGlueStatus status = APPE_Status();
    if (status == BleGlueStatusStarted) {
        uint8_t HCI_Version;
        uint16_t HCI_Revision;
        uint8_t LMP_PAL_Version;
        uint16_t Manufacturer_Name;
        uint16_t LMP_PAL_Subversion;

        tBleStatus ret = hci_read_local_version_information(
            &HCI_Version, &HCI_Revision, &LMP_PAL_Version, &Manufacturer_Name, &LMP_PAL_Subversion
        );

        string_cat_printf(buffer,
            "Ret: %d, HCI_Version: %d, HCI_Revision: %d, LMP_PAL_Version: %d, Manufacturer_Name: %d, LMP_PAL_Subversion: %d",
            ret, HCI_Version, HCI_Revision, LMP_PAL_Version, Manufacturer_Name, LMP_PAL_Subversion
        );
    } else {
        string_cat_printf(buffer, "BLE not ready");
    }
}

bool api_hal_bt_is_alive() {
    return APPE_Status() == BleGlueStatusStarted;
}

bool api_hal_bt_wait_transition() {
    uint8_t counter = 0;
    while (APPE_Status() == BleGlueStatusStartup) {
        osDelay(10);
        counter++;
        if (counter > 1000) {
            return false;
        }
    }
    return true;
}

bool api_hal_bt_lock_flash() {
    if (!api_hal_bt_wait_transition()) {
        return false;
    }
    if (APPE_Status() == BleGlueStatusUninitialized) {
        HAL_FLASH_Unlock();
    } else {
        while (HAL_HSEM_FastTake(CFG_HW_FLASH_SEMID) != HAL_OK) {
            osDelay(1);
        }
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON);
        HAL_FLASH_Unlock();
        while(LL_FLASH_IsOperationSuspended()) {};
    }
    return true;
}

void api_hal_bt_unlock_flash() {
    if (APPE_Status() == BleGlueStatusUninitialized) {
        HAL_FLASH_Lock();
    } else {
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF);
        HAL_FLASH_Lock();
        HAL_HSEM_Release(CFG_HW_FLASH_SEMID, HSEM_CPU1_COREID);
    }
}

void api_hal_bt_start_tone_tx(uint8_t tx_channel, uint8_t power) {
    aci_hal_set_tx_power_level(0, power);
    aci_hal_tone_start(tx_channel, 0);
}

void api_hal_bt_stop_tone_tx() {
    aci_hal_tone_stop();
}

void api_hal_bt_start_packet_tx(uint8_t frequency, uint8_t datarate) {
    hci_le_enhanced_transmitter_test(frequency, 0x25, 2, datarate);
}

void api_hal_bt_stop_packet_tx() {
    uint16_t num_of_packets;
    hci_le_test_end(&num_of_packets);
}

void api_hal_bt_start_rx(uint8_t frequency) {
    aci_hal_rx_start(frequency);
}

void api_hal_bt_stop_rx() {
    aci_hal_rx_stop();
}
