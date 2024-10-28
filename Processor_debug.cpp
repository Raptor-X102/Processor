#include "Processor_debug.h"

void SPU_Dump(const SPU_data * processor, void (* Printf_Format_Func)(const void * value)) {

    int scale = 8;
    printf("-----------------------------------------------------------------------------------------\n");

    printf("code:  ");
    for(int commands_amount = 0; commands_amount < processor->code_size; commands_amount += scale) {
        if(commands_amount >= scale)
            printf("\n       ");
        for(int i = commands_amount; i < commands_amount + scale && i < processor->code_size; i++)
            printf("%8X ", i);
        printf("\n");
        printf("       ");
        for(int i = commands_amount; i < commands_amount + scale && i < processor->code_size; i++) {

            printf("%8X ", processor->cmd_code[i]);
        }
        printf("\n");
        if(processor->IP >= commands_amount && processor->IP < commands_amount + scale) {
            printf("              ");
            for(int k = commands_amount; k < processor->IP; k++) {

                printf("         ");
            }

            printf("^ IP = %2llX\n", processor->IP);

        }


    }







    printf("Stack: ");

    for(size_t commands_amount = 0; commands_amount < processor->stack.size; commands_amount++) {

        Printf_Format_Func((char*)processor->stack.data+commands_amount*processor->stack.element_size);
    }

    printf("\nRegisters: ");
    for(int commands_amount = 0; commands_amount < Registers_amount; commands_amount++)
        printf("%.2lg ", processor->registers[commands_amount]);
    printf("\nRAM: \n");
    int scale_ram = 20;
    /*for(int i = 0; processor->RAM[i] != 0; i++)
        printf("%4.2lg ", processor->RAM[i]); */
    for(int i = 0; i < 400; i+=scale_ram) {
        for(int j = i; j < i + scale_ram; j++)
            printf("%4.2lg ", processor->RAM[j]);
        printf("\n");
    }
    printf("\n-----------------------------------------------------------------------------------------\n");
    getchar();
}
