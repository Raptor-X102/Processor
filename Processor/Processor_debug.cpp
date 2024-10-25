#include "Processor_debug.h"

void SPU_dump(const SPU_data * processor, void (* Printf_Format_Func)(const void * value)) {

    printf("-----------------------------------------------------------------------------------------\n");

    printf("code:  ");
    for(int commands_amount = 0; commands_amount < processor->code_size; commands_amount++)
        printf("%02X ", commands_amount);

    printf("\n       ");

    for(int commands_amount = 0; commands_amount < processor->code_size; commands_amount++) {

        printf("%02X ", processor->cmd_code[commands_amount]);
    }

    printf("\n       ");

    for(int commands_amount = 0; commands_amount < processor->IP; commands_amount++) {

        printf("   ");
    }

    printf("^ IP = %d\n", processor->IP);
    printf("Stack: ");

    for(size_t commands_amount = 0; commands_amount < processor->stack.size; commands_amount++) {

        Printf_Format_Func((char*)processor->stack.data+commands_amount*processor->stack.element_size);
    }

    printf("\nRegisters: ");
    for(int commands_amount = 0; commands_amount < Registers_amount; commands_amount++)
        printf("%02X ", processor->registers[commands_amount]);

    printf("\n-----------------------------------------------------------------------------------------\n");
    getchar();
}
