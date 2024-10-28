#ifndef ASM
#define ASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Processor_definitions.h"
#include "GetFileSize2.h"

const size_t Labels_amount = 50;
const size_t Cmd_name_length = 32;
const size_t signature_len = 8;

struct labels_data {

    uint64_t label_pos;
    char label_name[Cmd_name_length];
};

struct Labels {

    labels_data* labels_arr;
    size_t labels_count;
    size_t labels_amount;
};

#define COMMANDS_DEF(name, cmd, arg_n, ...) name, cmd, arg_n
const unsigned int Command_len = 8;

struct Commands_data {

    char command_name[Command_len];
    char command_num;
    int args_amount;

};

Commands_data Commands[] =
{

#include "Commands_data.h"

};
#undef COMMANDS_DEF
void Compile_code(const char* compiling_file, const char* output_file, Labels* labels);
//char * Compiled_filename(const char* compiling_file);
uint64_t Convert_txt_to_code(FILE* input_file, FILE* output_file, uint64_t code_length, Labels* labels);
void ASM_dump(const char* code_data, uint64_t curr_code_byte);
void Labels_Ctor(Labels* labels, size_t labels_amount);
void Labels_Dtor(Labels* labels);
void Insert_label(Labels* labels, const char* label_name, size_t label_pos);
int64_t Find_label(const char* label_name, Labels* labels);
bool Check_jmp_func(const char* prev_cmd);
char Check_register(const char* code_str);
bool Chech_brackets(const char* code_str);
void Get_arg(char* code_data, uint64_t* commands_amount, uint64_t* curr_code_byte, const char* code_str);
void Get_draw_arg(char* input_buffer, uint64_t* height, uint64_t* width, uint64_t* inword_pos);

#ifdef NO_DEBUG
#define ASM_DUMP(code_data, curr_code_byte) ASM_dump(code_data, curr_code_byte)
#else
#define ASM_DUMP(code_data, curr_code_byte)
#endif
#endif
