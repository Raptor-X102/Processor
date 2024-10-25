#ifndef SPU_DBG
#define SPU_DBG

#include "Processor_definitions.h"

#ifdef NO_DEBUG
#define SPU_DUMP(processor, Printf_Format_Func) SPU_dump(processor, Printf_Format_Func)
#else
#define SPU_DUMP(processor, Printf_Format_Func)
#endif
void SPU_dump(const SPU_data * processor, void (* Printf_Format_Func)(const void * value));

#endif
