#include "Processor_debug.h"

void SPU_Dump(const SPU_data * processor, void (* Printf_Format_Func)(const void * value)) {

    int scale = 8;
    fprintf(stderr, "-----------------------------------------------------------------------------------------\n");

    fprintf(stderr, "code:  ");
    for(int commands_amount = 0; commands_amount < processor->code_size; commands_amount += scale) {
        if(commands_amount >= scale)
            fprintf(stderr, "\n       ");
        for(int i = commands_amount; i < commands_amount + scale && i < processor->code_size; i++)
            fprintf(stderr, "%8X ", i);
        fprintf(stderr, "\n");
        fprintf(stderr, "       ");
        for(int i = commands_amount; i < commands_amount + scale && i < processor->code_size; i++) {

            fprintf(stderr, "%8X ", processor->cmd_code[i]);
        }
        fprintf(stderr, "\n");
        if(processor->IP >= commands_amount && processor->IP < commands_amount + scale) {
            fprintf(stderr, "              ");
            for(int k = commands_amount; k < processor->IP; k++) {

                fprintf(stderr, "         ");
            }

            fprintf(stderr, "^ IP = %2llX\n", processor->IP);
        }
    }

    fprintf(stderr, "Stack: ");

    for(size_t commands_amount = 0; commands_amount < processor->stack.size; commands_amount++) {

        Printf_Format_Func((char*)processor->stack.data+commands_amount*processor->stack.element_size);
    }

    fprintf(stderr, "\nRegisters: ");

    for(int commands_amount = 0; commands_amount < Registers_amount; commands_amount++)
        fprintf(stderr, "%.2lg ", processor->registers[commands_amount]);

    fprintf(stderr, "\nRAM: \n");

    int scale_ram = 20;

    for(int i = 0; i < 400; i+=scale_ram) {
        for(int j = i; j < i + scale_ram; j++)
            fprintf(stderr, "%4.2lg ", processor->RAM[j]);

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n-----------------------------------------------------------------------------------------\n");
    getchar();
}
