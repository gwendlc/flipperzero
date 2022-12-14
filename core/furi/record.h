#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize record storage
 * For internal use only.
 */
void furi_record_init();

/**
 * Create record
 * @param name - record name
 * @param data - data pointer
 * @note Thread safe. Create and destroy must be executed from the same thread.
 */
void furi_record_create(const char* name, void* data);

/**
 * Destroy record
 * @param name - record name
 * @return true if successful, false if still have holders or thread is not owner.
 * @note Thread safe. Create and destroy must be executed from the same thread.
 */
bool furi_record_destroy(const char* name);

/**
 * Open record
 * @param name - record name
 * @return pointer to the record
 * @note Thread safe. Open and close must be executed from the same thread.
 * Suspends caller thread till record appear
 */
void* furi_record_open(const char* name);

/**
 * Close record
 * @param name - record name
 * @note Thread safe. Open and close must be executed from the same thread.
 */
void furi_record_close(const char* name);

#ifdef __cplusplus
}
#endif
