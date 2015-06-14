#ifndef MNEMODEFS_H
#define MNEMODEFS_H

enum {
    EL_X = 0, //coord X
    EL_Y = 1, //coord Y
    EL_WIDTH = 2,
    EL_HEIGHT = 3,
    EL_TOOLTIP = 4,
    EL_ID = 5, //unique id
    EL_BLOCK = 6,//blocking element, not movable
    EL_BACKGROUND = 7,
    EL_SIGN_BACKGROUND = 8,
    EL_BORDER_COLOR = 9,
    EL_BORDER_WIDTH = 10,
    EL_BORDER_STYLE = 11,
    EL_ANGLE = 12,
    EL_Z_VALUE = 13,
    EL_LINE_WIDTH = 14,
    EL_TEXT = 15,
    EL_FONT_FAMILY = 16,
    EL_FONT_SIZE = 17,
    EL_FONT_COLOR = 18,
    EL_SCREEN_LINK = 19, //transition to screen
    EL_MESSAGE_TYPE = 20, //accociate message 001-016
    EL_PROPERTY_EMPTY = 21
};

enum {
    SCREEN_ID,
    SCREEN_PRIORITY,
    SCREEN_BACKGROUND,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
};

enum ComponentType {
    NEW_PROJECT,
    NEW_SCREEN
};

enum SaveScreenType {
    SAVE_XML,
    SAVE_BINARY,
    SAVE_IMAGE
};

#endif // MNEMODEFS_H
