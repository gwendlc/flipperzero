#include <furi.h>
#include <furi_hal.h>
#include "lfrfid_worker_i.h"
#include <stream_buffer.h>

void lfrfid_worker_mode_idle_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_idle_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_idle_stop(LFRFIDWorker* worker);

void lfrfid_worker_mode_emulate_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_emulate_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_emulate_stop(LFRFIDWorker* worker);

void lfrfid_worker_mode_read_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_read_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_read_stop(LFRFIDWorker* worker);

void lfrfid_worker_mode_write_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_write_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_write_stop(LFRFIDWorker* worker);

void lfrfid_worker_mode_read_raw_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_read_raw_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_read_raw_stop(LFRFIDWorker* worker);

void lfrfid_worker_mode_emulate_raw_start(LFRFIDWorker* worker);
void lfrfid_worker_mode_emulate_raw_tick(LFRFIDWorker* worker);
void lfrfid_worker_mode_emulate_raw_stop(LFRFIDWorker* worker);

const LFRFIDWorkerModeType lfrfid_worker_modes[] = {
    [LFRFIDWorkerIdle] =
        {
            .quant = osWaitForever,
            .start = lfrfid_worker_mode_idle_start,
            .tick = lfrfid_worker_mode_idle_tick,
            .stop = lfrfid_worker_mode_idle_stop,
        },
    [LFRFIDWorkerRead] =
        {
            .quant = 100,
            .start = lfrfid_worker_mode_read_start,
            .tick = lfrfid_worker_mode_read_tick,
            .stop = lfrfid_worker_mode_read_stop,
        },
    [LFRFIDWorkerWrite] =
        {
            .quant = 1000,
            .start = lfrfid_worker_mode_write_start,
            .tick = lfrfid_worker_mode_write_tick,
            .stop = lfrfid_worker_mode_write_stop,
        },
    [LFRFIDWorkerEmulate] =
        {
            .quant = 1000,
            .start = lfrfid_worker_mode_emulate_start,
            .tick = lfrfid_worker_mode_emulate_tick,
            .stop = lfrfid_worker_mode_emulate_stop,
        },
    [LFRFIDWorkerReadRaw] =
        {
            .quant = 1000,
            .start = lfrfid_worker_mode_read_raw_start,
            .tick = lfrfid_worker_mode_read_raw_tick,
            .stop = lfrfid_worker_mode_read_raw_stop,
        },
    [LFRFIDWorkerEmulateRaw] =
        {
            .quant = 1000,
            .start = lfrfid_worker_mode_emulate_raw_start,
            .tick = lfrfid_worker_mode_emulate_raw_tick,
            .stop = lfrfid_worker_mode_emulate_raw_stop,
        },
};

/*********************** IDLE ***********************/

void lfrfid_worker_mode_idle_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_idle_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_idle_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}

/*********************** READ ***********************/

void lfrfid_worker_mode_read_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_read_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_read_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}

/*********************** EMULATE ***********************/

void lfrfid_worker_mode_emulate_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_emulate_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_emulate_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}

/*********************** WRITE ***********************/

void lfrfid_worker_mode_write_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_write_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_write_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}

/*********************** READ RAW ***********************/

void lfrfid_worker_mode_read_raw_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_read_raw_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_read_raw_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}

/*********************** EMULATE RAW ***********************/

void lfrfid_worker_mode_emulate_raw_start(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_emulate_raw_tick(LFRFIDWorker* worker) {
    UNUSED(worker);
}

void lfrfid_worker_mode_emulate_raw_stop(LFRFIDWorker* worker) {
    UNUSED(worker);
}
