#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <furi.h>
#include "irda_i.h"


struct IrdaHandler {
    void** ctx;
};

typedef struct {
    IrdaAlloc alloc;
    IrdaDecode decode;
    IrdaFree free;
} IrdaDecoders;

typedef struct {
    IrdaEncode encode;
} IrdaEncoders;

typedef struct {
    IrdaProtocol protocol;
    const char* name;
    IrdaDecoders decoder;
    IrdaEncoders encoder;
} IrdaProtocolImplementation;


// TODO: replace with key-value, Now we refer by enum index, which is dangerous.
static const IrdaProtocolImplementation irda_protocols[] = {
    // #0
    { .protocol = IrdaProtocolSamsung32,
      .name ="Samsung32",
      .decoder = {
          .alloc = irda_decoder_samsung32_alloc,
          .decode = irda_decoder_samsung32_decode,
          .free = irda_decoder_samsung32_free},
      .encoder = {
          .encode = irda_encoder_samsung32_encode}
    },
    // #1
    { .protocol = IrdaProtocolNEC,
      .name = "NEC",
      .decoder = {
          .alloc = irda_decoder_nec_alloc,
          .decode = irda_decoder_nec_decode,
          .free = irda_decoder_nec_free},
      .encoder = {
          .encode = irda_encoder_nec_encode}
    },
};


const IrdaMessage* irda_decode(IrdaHandler* handler, bool level, uint32_t duration) {
    furi_assert(handler);

    IrdaMessage* message = NULL;
    IrdaMessage* result = NULL;

    for (int i = 0; i < COUNT_OF(irda_protocols); ++i) {
        message = irda_protocols[i].decoder.decode(handler->ctx[i], level, duration);
        if (!result && message) {
            message->protocol = irda_protocols[i].protocol;
            result = message;
        }
    }

    return result;
}

IrdaHandler* irda_alloc_decoder(void) {
    IrdaHandler* handler = furi_alloc(sizeof(IrdaHandler));
    handler->ctx = furi_alloc(sizeof(void*) * COUNT_OF(irda_protocols));

    for (int i = 0; i < COUNT_OF(irda_protocols); ++i) {
        handler->ctx[i] = irda_protocols[i].decoder.alloc();
        furi_check(handler->ctx[i]);
    }

    return handler;
}

void irda_free_decoder(IrdaHandler* handler) {
    furi_assert(handler);
    furi_assert(handler->ctx);

    for (int i = 0; i < COUNT_OF(irda_protocols); ++i) {
        irda_protocols[i].decoder.free(handler->ctx[i]);
    }

    free(handler->ctx);
    free(handler);
}

void irda_send(const IrdaMessage* message, int times) {
    furi_assert(message);

    for (int i = 0; i < times; ++i) {
        osKernelLock();
        __disable_irq();
        irda_protocols[message->protocol].encoder.encode(message->address, message->command, !!i);
        __enable_irq();
        osKernelUnlock();
    }
}

const char* irda_get_protocol_name(IrdaProtocol protocol) {
    return irda_protocols[protocol].name;
}

