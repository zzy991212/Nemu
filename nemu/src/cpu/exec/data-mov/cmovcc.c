#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

//make_helper_v(cmovo)
//make_helper_v(cmovno)
//make_helper_v(cmovb)
//make_helper_v(cmovae)
make_helper_v(cmove)
//make_helper_v(cmovne)
//make_helper_v(cmovbe)
//make_helper_v(cmova)
//make_helper_v(cmovs)
//make_helper_v(cmovns)
//make_helper_v(cmovp)
//make_helper_v(cmovnp)
//make_helper_v(cmovl)
//make_helper_v(cmovge)
make_helper_v(cmovle)
//make_helper_v(cmovg)
