#include <TXLib.h>
#include "Processor_Funcs.h"

int main(int argc, char* argv[]) {

    SPU_data processor = {};

    SPU_Ctor(&processor, argv[1]);
    printf("\n1\n");

    /*for(size_t commands_amount = 0; commands_amount < processor.code_size; commands_amount++) {

        printf("%d\n", processor.cmd_code[commands_amount]);
    }*/

    SPU_Run(&processor);
    printf("\n2\n");

    SPU_DUMP(&processor, Printf_Format_Function);
    SPU_Dtor(&processor);
    printf("\n3\n");
}
