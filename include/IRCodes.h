#ifndef IR_CODES_H
#define IR_CODES_H

#include <stdint.h>
#include <map>

enum IRButton {
    IR_POWER,
    IR_MENU,
    IR_TEST,
    IR_MAS,
    IR_BACK,
    IR_RETROCEDER,
    IR_PLAY,
    IR_ADELANTAR,
    IR_0,
    IR_MENOS,
    IR_C,
    IR_1,
    IR_2,
    IR_3,
    IR_4,
    IR_5,
    IR_6,
    IR_7,
    IR_8,
    IR_9,
    IR_UNKNOWN
};

static std::map<uint32_t, IRButton> IRTable = {
    {0x5DA2FF00, IR_POWER},
    {0x1DE2FF00, IR_MENU},
    {0xDD22FF00, IR_TEST},
    {0xFD02FF00, IR_MAS},
    {0x3DC2FF00, IR_BACK},
    {0x1FE0FF00, IR_RETROCEDER},
    {0x57A8FF00, IR_PLAY},
    {0x6F90FF00, IR_ADELANTAR},
    {0x9768FF00, IR_0},
    {0x6798FF00, IR_MENOS},
    {0x4FB0FF00, IR_C},
    {0xCF30FF00, IR_1},
    {0xE718FF00, IR_2},
    {0x857AFF00, IR_3},
    {0xEF10FF00, IR_4},
    {0xC738FF00, IR_5},
    {0xA55AFF00, IR_6},
    {0xBD42FF00, IR_7},
    {0xB54AFF00, IR_8},
    {0xAD52FF00, IR_9}
};

static IRButton decodeButton(uint32_t code) {
    if (IRTable.count(code)) return IRTable[code];
    return IR_UNKNOWN;
}

#endif
