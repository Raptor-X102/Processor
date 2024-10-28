#ifndef CDEF
#define CDEF

COMMANDS_DEF({ "HLT",  .command_num = CMD_HLT,  .args_amount = 0, SPU_Hlt)},
COMMANDS_DEF({ "PUSH", .command_num = CMD_PUSH, .args_amount = 3, SPU_Push)},
COMMANDS_DEF({ "POP",  .command_num = CMD_POP,  .args_amount = 3, SPU_Pop)},
COMMANDS_DEF({ "ADD",  .command_num = CMD_ADD,  .args_amount = 0, SPU_Add)},
COMMANDS_DEF({ "SUB",  .command_num = CMD_SUB,  .args_amount = 0, SPU_Sub)},
COMMANDS_DEF({ "MUL",  .command_num = CMD_MUL,  .args_amount = 0, SPU_Mul)},
COMMANDS_DEF({ "DIV",  .command_num = CMD_DIV,  .args_amount = 0, SPU_Div)},
COMMANDS_DEF({ "OUT",  .command_num = CMD_OUT,  .args_amount = 0, SPU_Out)},
COMMANDS_DEF({ "JA",   .command_num = CMD_JA,   .args_amount = 1, SPU_Ja)},
COMMANDS_DEF({ "JAE",   .command_num = CMD_JAE,   .args_amount = 1, SPU_Jae)},
COMMANDS_DEF({ "JB",   .command_num = CMD_JB,   .args_amount = 1, SPU_Jb)},
COMMANDS_DEF({ "JBE",   .command_num = CMD_JBE,   .args_amount = 1, SPU_Jbe)},
COMMANDS_DEF({ "JE",   .command_num = CMD_JE,   .args_amount = 1, SPU_Je)},
COMMANDS_DEF({ "JNE",   .command_num = CMD_JNE,   .args_amount = 1, SPU_Jne)},
COMMANDS_DEF({ "JMP",  .command_num = CMD_JMP,  .args_amount = 1, SPU_Jmp)},
COMMANDS_DEF({ "CALL",  .command_num = CMD_CALL,  .args_amount = 1, SPU_Call)},
COMMANDS_DEF({ "RET",  .command_num = CMD_RET,  .args_amount = 0, SPU_Ret)},
COMMANDS_DEF({ "DRAW",  .command_num = CMD_DRAW,  .args_amount = 2, SPU_RAM_Draw)}

#endif
