#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "movzw-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movzw-template.h"
#undef DATA_BYTE

make_helper_v(movzw_rm2r)