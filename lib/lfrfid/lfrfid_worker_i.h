/**
 * @file lfrfid_worker_i.h
 * 
 * lfrfid worker, internal definitions 
 */

#pragma once
#include <furi.h>
#include "lfrfid_worker.h"
#include "lfrfid_raw_worker.h"
#include "protocols/lfrfid_protocols.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*const process)(LFRFIDWorker* worker);
} LFRFIDWorkerModeType;

typedef enum {
    LFRFIDWorkerIdle,
    LFRFIDWorkerRead,
    LFRFIDWorkerWrite,
    LFRFIDWorkerEmulate,
    LFRFIDWorkerReadRaw,
    LFRFIDWorkerEmulateRaw,
} LFRFIDWorkerMode;

struct LFRFIDWorker {
    char* raw_filename;
    // LFRFIDWriter* writer;

    LFRFIDWorkerMode mode_index;
    void* mode_storage;

    FuriEventFlag* events;
    FuriThread* thread;

    LFRFIDWorkerReadType read_type;

    LFRFIDWorkerReadCallback read_cb;
    LFRFIDWorkerWriteCallback write_cb;
    LFRFIDWorkerEmulateCallback emulate_cb;
    LFRFIDWorkerReadRawCallback read_raw_cb;
    LFRFIDWorkerEmulateRawCallback emulate_raw_cb;

    void* cb_ctx;

    ProtocolDict* protocols;
    LFRFIDProtocol protocol_to_encode;

    LFRFIDRawWorker* raw_worker;
};

extern const LFRFIDWorkerModeType lfrfid_worker_modes[];

bool lfrfid_worker_check_for_stop(LFRFIDWorker* worker);

#ifdef __cplusplus
}
#endif
