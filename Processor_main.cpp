#include <TXLib.h>
#include "Processor_Funcs.h"
#include "Check_r_w_flags.h"

int main(int argc, char* argv[]) {

    char* input_file_name = NULL;

    if(!Check_r_w_flags(CHECK_R, argv, argc, &input_file_name)) {

        fprintf(stderr, "ERROR: flags check failed\n");
        return 0;
    }

    SPU_data processor = {};

    SPU_Ctor(&processor, input_file_name);
    fprintf(stderr, "\n1\n");

    SPU_Run(&processor);
    fprintf(stderr, "\n2\n");

    SPU_Dump(&processor, Printf_Format_Function);
    SPU_Dtor(&processor);
    fprintf(stderr, "\n3\n");
}
