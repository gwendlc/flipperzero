#pragma once
#include <furi.h>
#include <gui/canvas.h>
#include "m-string.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************** COMMON ******************/

typedef struct DialogsApp DialogsApp;

/****************** FILE BROWSER ******************/

/**
 * Shows and processes the file browser dialog
 * @param context api pointer
 * @param result_path selected file path string pointer
 * @param path preselected file path string pointer
 * @param extension file extension to be offered for selection
 * @param skip_assets true - do not show assets folders
 * @param icon file icon pointer, NULL for default icon
 * @param hide_ext true - hide extensions for files
 * @return bool whether a file was selected
 */
bool dialog_file_browser_show(
    DialogsApp* context,
    string_ptr result_path,
    string_ptr path,
    const char* extension,
    bool skip_assets,
    const Icon* icon,
    bool hide_ext);

/****************** MESSAGE ******************/

/**
 * Message result type
 */
typedef enum {
    DialogMessageButtonBack,
    DialogMessageButtonLeft,
    DialogMessageButtonCenter,
    DialogMessageButtonRight,
} DialogMessageButton;

/**
 * Message struct
 */
typedef struct DialogMessage DialogMessage;

/**
 * Allocate and fill message
 * @return DialogMessage* 
 */
DialogMessage* dialog_message_alloc();

/**
 * Free message struct
 * @param message message pointer
 */
void dialog_message_free(DialogMessage* message);

/**
 * Set message text
 * @param message message pointer
 * @param text text, can be NULL if you don't want to display the text
 * @param x x position
 * @param y y position
 * @param horizontal horizontal alignment
 * @param vertical vertical alignment
 */
void dialog_message_set_text(
    DialogMessage* message,
    const char* text,
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical);

/**
 * Set message header
 * @param message message pointer
 * @param text text, can be NULL if you don't want to display the header
 * @param x x position
 * @param y y position
 * @param horizontal horizontal alignment
 * @param vertical vertical alignment
 */
void dialog_message_set_header(
    DialogMessage* message,
    const char* text,
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical);

/**
 * Set message icon
 * @param message message pointer
 * @param icon icon pointer, can be NULL if you don't want to display the icon
 * @param x x position
 * @param y y position
 */
void dialog_message_set_icon(DialogMessage* message, const Icon* icon, uint8_t x, uint8_t y);

/**
 * Set message buttons text, button text can be NULL if you don't want to display and process some buttons
 * @param message message pointer
 * @param left left button text, can be NULL if you don't want to display the left button
 * @param center center button text, can be NULL if you don't want to display the center button
 * @param right right button text, can be NULL if you don't want to display the right button
 */
void dialog_message_set_buttons(
    DialogMessage* message,
    const char* left,
    const char* center,
    const char* right);

/**
 * Show message from filled struct
 * @param context api pointer
 * @param message message struct pointer to be shown
 * @return DialogMessageButton type
 */
DialogMessageButton dialog_message_show(DialogsApp* context, const DialogMessage* message);

/**
 * Show SD error message (with question sign)
 * @param context 
 * @param error_text 
 */
void dialog_message_show_storage_error(DialogsApp* context, const char* error_text);

#ifdef __cplusplus
}
#endif
