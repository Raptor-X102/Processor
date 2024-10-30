#include <TXLib.h>
#include "Assembler.h"
#include "Check_r_w_flags.h"

int main(int argc, char* argv[]) {

    char* input_file_name = NULL;
    char* output_file_name = NULL;

    if(!Check_r_w_flags(CHECK_R_W, argv, argc, &input_file_name, &output_file_name)) {

        fprintf(stderr, "ERROR: flags check failed\n");
        return 0;
    }

    Labels labels = {};
    Labels_Ctor(&labels, Labels_amount);

    fprintf(stderr, "start\n");

    Compile_code(input_file_name, output_file_name, &labels);
    Labels_Ctor(&labels, 20);

    fprintf(stderr, "end\n");

    Labels_Dtor(&labels);
}