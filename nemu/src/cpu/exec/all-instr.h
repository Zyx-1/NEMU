#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/clt.h"
#include "data-mov/leave.h"
#include "data-mov/movzb.h"
#include "data-mov/movzw.h"
#include "data-mov/cmovs.h"
#include "data-mov/movsb.h"
#include "data-mov/movsw.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

#include "new/push.h"
#include "new/call.h"
#include "new/test.h"
#include "new/je.h"
#include "new/pop.h"
#include "new/ret.h"
#include "new/add.h"
#include "new/cmp.h"
#include "new/sub.h"
#include "new/sbb.h"
#include "new/jmp.h"
#include "new/jbe.h"
#include "new/jle.h"
#include "new/jl.h"
#include "new/ja.h"
#include "new/jg.h"
#include "new/js.h"
#include "new/jns.h"
#include "new/jge.h"
#include "new/jne.h"
#include "new/lea.h"
#include "new/adc.h"
#include "new/movs.h"
#include "new/setne.h"
#include "new/nop.h"
#include "new/lods.h"
#include "new/scas.h"
#include "new/stos.h"

