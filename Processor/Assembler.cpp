#include <TXLib.h>
#include "Assembler.h"

int main(int argc, char* argv[]) {

    Labels labels = {};
    Labels_Ctor(&labels, Labels_amount);
    printf("%d\n", argc);
    for(int i = 0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("start\n");
    Compile_code(argv[1], argv[2], &labels);
    Labels_Ctor(&labels, 20);
    printf("end\n");
}

void Compile_code(const char* compiling_file, const char* out_file, Labels* labels) {

    uint64_t file_size = get_file_size(compiling_file); printf("Size was gotten\n");

    FILE* input_file = fopen(compiling_file, "rb");
    if(!input_file) {

        fprintf(stderr, "ERROR: file was not opened");
        return;
    }

    uint64_t commands_amount = 0;
    //char* output_file = Compiled_filename(compiling_file);
    FILE* output_file = fopen(out_file, "wb+");
    if(!output_file) {

        fprintf(stderr, "ERROR: file was not opened");
        return;
    }

    printf("convertion started\n");
    commands_amount = Convert_txt_to_code(input_file, output_file, file_size, labels);
    printf("commands_amount = %d", commands_amount);
    printf("ended\n");

    fclose(output_file);
    printf("input file was closed\n");

    //free(output_file);
    printf("memory was free\n");

}

// fixme
/*char* Compiled_filename(const char* compiling_file) {

    size_t length = strlen(compiling_file);
    char* result_filename = (char*) calloc(length+5, sizeof(char));
    if(compiling_file[length-2] == 'a' &&
       compiling_file[length-1] == 's' &&
       compiling_file[length-0] == 'm') {

        int i = 0;
        for(; compiling_file[i] != '.'; i++)
            result_filename[i] = compiling_file[i];

        result_filename[i]   = '(';
        result_filename[i+1] = '1';
        result_filename[i+2] = ')';
        result_filename[i+3] = '.';
        result_filename[i+3] = 'a';
        result_filename[i+3] = 's';
        result_filename[i+3] = 'm';
    }
    else {

        int i = 0;
        for(; compiling_file[i] != '.'; i++)
            result_filename[i] = compiling_file[i];

        result_filename[i]   = '.';
        result_filename[i+1] = 'a';
        result_filename[i+2] = 's';
        result_filename[i+3] = 'm';
    }
    printf("\n\nresult_filename = %s\n\n", result_filename);
    return result_filename;
}
*/
uint64_t Convert_txt_to_code(FILE* input_file, FILE* output_file, uint64_t code_length, Labels* labels) {

    int* code_data = (int*) calloc(code_length, sizeof(int));
    uint64_t commands_amount = 0;
    char* input_buffer = NULL;
    char* tmp_ptr = (char*) calloc(code_length+10, sizeof(char));
    if(tmp_ptr) {

    input_buffer = tmp_ptr;
    tmp_ptr = NULL;
    }

    else {

        fprintf(stderr, "ERROR: memory was not allocated\n");
        return 0;
    }


    char signature[signature_len] = {};
    int version = 0;

    fread(signature, sizeof(char), 5, input_file);
    printf("%s\n", signature);
    fwrite(signature, sizeof(char), 5, output_file);
    fseek(input_file, 2L, SEEK_CUR);
    fscanf(input_file, "%d", &version);
    fwrite(&version, sizeof(int), 1, output_file);
    fseek(input_file, 10L, SEEK_SET);
    printf("%d\n", version);

    fread(input_buffer, sizeof(char), code_length-7*sizeof(char)-1*sizeof(int), input_file);
    char* code_str_prev = input_buffer;

    for(int curr_pos = 0, inword_pos = 0; input_buffer[curr_pos] != EOF &&
        curr_pos < code_length-7*sizeof(char)-1*sizeof(int);curr_pos++) { // fixme: scanf
        printf("\n\n");
        while(isspace(input_buffer[curr_pos]))
            curr_pos++;

        if(input_buffer[curr_pos] == ';') {

            while(input_buffer[curr_pos] != '\n')
                curr_pos++;

            curr_pos++;
        }
        inword_pos = curr_pos;

        while(!isspace(input_buffer[inword_pos]))
            inword_pos++;

        input_buffer[inword_pos] = '\0';
        printf("input_buffer + curr_pos = %s\n", input_buffer + curr_pos);

        for(int i = 0; i < sizeof(Commands) / sizeof(Commands[0]); i++) {

            if(!strcmp(input_buffer+curr_pos, Commands[i].command_name)) {

                code_data[commands_amount] = Commands[i].command_num;
                goto end_loop;
            }
        }

        if(strchr(input_buffer+curr_pos, ':')) {

            if(!Check_jmp(code_str_prev)) {
                int64_t label_pos = Find_label(input_buffer+curr_pos, labels);
                if(label_pos >= 0) {

                    code_data[labels->labels_arr[label_pos].label_pos] = commands_amount;
                    code_str_prev = input_buffer+curr_pos;
                    printf("code_str_prev = %s\n", code_str_prev);
                    curr_pos = inword_pos;
                    continue;
                }
                else {

                    Insert_label(labels, input_buffer+curr_pos, commands_amount);
                    code_str_prev = input_buffer+curr_pos;
                    printf("code_str_prev = %s\n", code_str_prev);
                    curr_pos = inword_pos;
                    continue;
                }
            }
            else {

                int64_t label_pos = Find_label(input_buffer+curr_pos, labels);
                if(label_pos >= 0) {

                    code_data[commands_amount] = labels->labels_arr[label_pos].label_pos;
                }
                else {

                    Insert_label(labels, input_buffer+curr_pos, commands_amount);
                }
            }
        }
        else if(!strcmp(code_str_prev, "PUSH") || !strcmp(code_str_prev, "POP")) {

            printf("PUSH params: \n");
            Get_arg(code_data, &commands_amount, input_buffer+curr_pos);
        }

        else{

            code_data[commands_amount] = atoi(input_buffer+curr_pos);
            printf("    [%d]atoi = %d\n", commands_amount, atoi(input_buffer+curr_pos));
        }

        end_loop:
            code_str_prev = input_buffer+curr_pos;
            printf("code_str_prev = %s\n", code_str_prev);
            curr_pos = inword_pos;
            commands_amount++;

        //fprintf(stderr, "ERROR: ")

        /*if(!strcmp(code_str, "HLT")) {
            code_data[commands_amount] = CMD_HLT;
            goto end_loop;
        }

        if(!strcmp(code_str, "PUSH")) {
            code_data[commands_amount] = CMD_PUSH;
            goto end_loop;
        }

        if(!strcmp(code_str, "POP")) {
            code_data[commands_amount] = CMD_POP;
            goto end_loop;
        }

        if(!strcmp(code_str, "ADD")) {
            code_data[commands_amount] = CMD_ADD;
            goto end_loop;
        }

        if(!strcmp(code_str, "SUB")) {
            code_data[commands_amount] = CMD_SUB;
            goto end_loop;
        }

        if(!strcmp(code_str, "MUL")) {
            code_data[commands_amount] = CMD_MUL;
            goto end_loop;
        }

        if(!strcmp(code_str, "DIV")) {
            code_data[commands_amount] = CMD_DIV;
            goto end_loop;
        }

        if(!strcmp(code_str, "OUT")) {
            code_data[commands_amount] = CMD_OUT;
            goto end_loop;
        }

        if(!strcmp(code_str, "JB")) {
            code_data[commands_amount] = CMD_JB;
            goto end_loop;
        }
        */

    }

    txDump(code_data);
    fwrite(&commands_amount, sizeof(uint64_t), 1, output_file);
    fwrite(code_data, sizeof(int), commands_amount, output_file);

    return commands_amount;
}

void Labels_Ctor(Labels* labels, size_t labels_amount) {

    if(!labels) {

        fprintf(stderr, "ERROR: Null ptr\n");
        return;
    }
    labels->labels_amount = labels_amount;
    labels_data* tmp_ptr = (labels_data*) malloc(labels_amount*sizeof(labels_data));
    memset(tmp_ptr, -1, labels_amount*sizeof(labels_data));
    if(tmp_ptr) {

        labels->labels_arr = tmp_ptr;
        tmp_ptr = NULL;
    }
    else
        fprintf(stderr, "ERROR: memory was not allocated\n");

}
void Labels_Dtor(Labels* labels) {

    if(!labels || !labels->labels_arr) {

        fprintf(stderr, "ERROR: Null ptr\n");
        return;
    }

    free(labels->labels_arr);

}

int64_t Find_label(const char* label_name, Labels* labels) {

    for(int label_pos = 0; label_pos < labels->labels_amount; label_pos++) {

        if(!strcmp(label_name, labels->labels_arr[label_pos].label_name))
            return label_pos;

    }
    printf("Label was not found\n");
    return -1;

}

bool Check_jmp(const char* prev_cmd) {

    if(!strcmp(prev_cmd, "JA") || !strcmp(prev_cmd, "JAE") || !strcmp(prev_cmd, "JB") || !strcmp(prev_cmd, "JBE") ||
       !strcmp(prev_cmd, "JE") || !strcmp(prev_cmd, "JNE") || !strcmp(prev_cmd, "JMP"))
        return true;
    return false;

}

void Insert_label(Labels* labels, const char* label_name, size_t label_pos) {

    labels->labels_arr[labels->labels_count].label_pos = label_pos;
    memcpy(labels->labels_arr[labels->labels_count].label_name, label_name, strlen(label_name) + 1);
    labels->labels_count++;

}

int Check_register(const char* code_str) {

    if(strchr(code_str, 'A'))
        return CMD_AX;
    if(strchr(code_str, 'B'))
        return CMD_BX;
    if(strchr(code_str, 'C'))
        return CMD_CX;
    if(strchr(code_str, 'D'))
        return CMD_DX;

    return -1;
}

bool Chech_brackets(const char* code_str) {

    if(code_str[0] == '[' && code_str[strlen(code_str)-1] == ']')
        return true;

    return false;

}

void Get_arg(int* code_data,uint64_t* commands_amount, const char* code_str) {

    char* tmp_ptr = strchr(code_str, '+');

    if(tmp_ptr) {

        code_data[*commands_amount] = NUMBER_MASK + REGISTER_MASK;
        printf("+ \n");
        printf("\n\ntmp_ptr = %s %s %s\n\n", tmp_ptr, tmp_ptr+1,tmp_ptr+2);
        code_data[*commands_amount+1] = Check_register(code_str);
        code_data[*commands_amount+2] = atoi(tmp_ptr+1);
        if(Chech_brackets(code_str)) {

            code_data[*commands_amount] += RAM_MASK;
            printf("[]\n");
        }

        *commands_amount += 2;

    }
    else{

        if(Chech_brackets(code_str)) {

            code_data[*commands_amount] = RAM_MASK;
            printf("[]\n");
            int push_register = Check_register(code_str);
            if(push_register >= 0) {

                printf("%c%c\n", code_str[1], code_str[2]);
                code_data[*commands_amount] += REGISTER_MASK;
                code_data[*commands_amount+1] = push_register;
                (*commands_amount)++;
            }
            else {

                printf("number\n");
                code_data[*commands_amount] += NUMBER_MASK;
                code_data[*commands_amount+1] = atoi(code_str+1);
                (*commands_amount)++;
            }
        }
        else{

            int push_register = Check_register(code_str);
            if(push_register >= 0) {

                printf("%c%c\n", code_str[0], code_str[1]);
                code_data[*commands_amount] = REGISTER_MASK;
                code_data[*commands_amount+1] = push_register;
                (*commands_amount)++;
            }
            else {

                printf("number\n");
                code_data[*commands_amount] = NUMBER_MASK;
                code_data[*commands_amount+1] = atoi(code_str);
                (*commands_amount)++;
            }
        }
    }

}
