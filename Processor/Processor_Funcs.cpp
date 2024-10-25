#include "Processor_Funcs.h"

//debug

void Printf_Format_Function(const void * value) {

    int tmp = 0;
    memcpy(&tmp, value, sizeof(int));
    printf("%02X ", tmp);
}

Commands_data_struct Commands_data[] =
{

#include "Commands_data.h"

};

#undef COMMANDS_DEF
/*********************************************************** */

void SPU_Ctor(SPU_data * processor, const char* filename) {

    const int poison = -666;
    STACK_CTOR(&(processor->stack), 10, sizeof(int), &poison);
    Read_code_file(processor, filename);
    int* tmp_ptr = (int*) calloc(RAM_Size, sizeof(char));
    if(tmp_ptr) {

        processor->RAM = tmp_ptr;
        tmp_ptr = NULL;
    }
    else
        fprintf(stderr, "ERROR: RAM was not allocated\n");
}

void SPU_Run(SPU_data * processor) { // TODO: rename (prefix)

    bool isn_hlt = true;
    while(isn_hlt) {

        if(processor->cmd_code[processor->IP] == CMD_HLT)
            break;

        else
            Commands_data[processor->cmd_code[processor->IP]].SPU_Func(processor);
            SPU_DUMP(processor, Printf_Format_Function);
        /*switch(processor->cmd_code[processor->IP]) {

            case CMD_HLT:
                SPU_Hlt( processor);
                isn_hlt = false;
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_PUSH:
                SPU_Push(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_POP:
                SPU_Pop(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_ADD:
                SPU_Add(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_SUB:
                SPU_Sub(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_MUL:
                SPU_Mul(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_DIV:
                SPU_Div(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_OUT:
                SPU_Out(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            case CMD_JB:
                SPU_Jb(processor);
                SPU_DUMP(processor, Printf_Format_Function);
                break;

            default:
                fprintf(stderr, "ERROR: unknown command\n");
                SPU_DUMP(processor, Printf_Format_Function);
                return; */

    }
}

void Read_code_file(SPU_data * processor, const char* file_name) {

    FILE * file = fopen(file_name, "rb");
    if(!file) {

        fprintf(stderr, "ERROR: file was not opened\n");
        return;
    }
    char signature[10] = {};
    int version = 0;

    fread(signature, sizeof(char), 5, file);
    printf("%s\n", signature);

    fread(&version, sizeof(int), 1, file);
    printf("%d\n", version);

    fread(&(processor->code_size), sizeof(uint64_t), 1, file);
    printf("(processor->code_size) = %d\n", (processor->code_size));
    //getchar();

    if(processor->code_size) {

        int* tmp = (int*) calloc(processor->code_size, sizeof(int));
        if(tmp) {

            processor->cmd_code = tmp;
            tmp = NULL;
        }
        else {

            fprintf(stderr, "ERROR: memory was not allocated\n");
            return;
        }
    }
    else {

        fprintf(stderr, "ERROR: null size\n");
        return;
    }

    fread(processor->cmd_code, sizeof(int), processor->code_size, file);

}

void SPU_Push(SPU_data* processor) {

    int64_t tmp = Get_push_arg(processor);
    StackPush(&processor->stack, &tmp);
}

int64_t Get_push_arg(SPU_data* processor) {

    int option_byte = processor->cmd_code[processor->IP+1];
    int result = 0;
    processor->IP += 2;

    if(option_byte & NUMBER_MASK) {

        result = processor->cmd_code[processor->IP];
        processor->IP++;
    }

    if(option_byte & REGISTER_MASK) {

        result += processor->registers[processor->cmd_code[processor->IP]];
        processor->IP++;
    }

    if(option_byte & RAM_MASK)
        return (processor->RAM[result]);

    else
        return result;
}

void SPU_Hlt(SPU_data* processor) {

    processor->IP++;
}

void SPU_Pop(SPU_data* processor) {

    void* tmp = Get_pop_arg(processor);
    StackPop(&processor->stack, tmp);
}

void* Get_pop_arg(SPU_data* processor) {

    int option_byte = processor->cmd_code[processor->IP+1];
    processor->IP += 2;
    int result = 0;
    if(option_byte & RAM_MASK) {

        if(option_byte & NUMBER_MASK) {

            result = processor->cmd_code[processor->IP];
            processor->IP++;
        }
        if(option_byte & REGISTER_MASK) {

            result += processor->registers[processor->cmd_code[processor->IP]];
            processor->IP++;
        }

        return &processor->RAM[result];

    }

    else {

        processor->IP++;
        return &processor->registers[processor->cmd_code[processor->IP-1]];
    }
}

void SPU_Add(SPU_data* processor) {

    int pop1 = 0;
    int pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    int sum = pop2 + pop1;
    StackPush(&(processor->stack), &sum);

    processor->IP++;
}

void SPU_Sub(SPU_data* processor) {

    int pop1 = 0;
    int pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    int difference = pop2 - pop1;
    StackPush(&(processor->stack), &difference);

    processor->IP++;
}

void SPU_Mul(SPU_data* processor) {

    int pop1 = 0;
    int pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    int product = pop2 * pop1;
    StackPush(&(processor->stack), &product);

    processor->IP++;
}

void SPU_Div(SPU_data* processor) {

    int pop1 = 0;
    int pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    int quotient = (int)(pop2 / pop1);
    StackPush(&(processor->stack), &quotient);

    processor->IP++;
}

void SPU_Out(SPU_data* processor) {

    int out = 0;
    StackPop(&(processor->stack), &out);
    printf("OUT: %d\n", out);
    processor->IP++;
}

void SPU_Jb(SPU_data* processor) {

    int pop1 = 0;
    int pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(pop1 > pop2)
        processor->IP = processor->cmd_code[processor->IP+1];

    else
        processor->IP += 2;
}

void SPU_Dtor(SPU_data* processor) {

    StackDtor(&processor->stack);
    processor->IP = 0;
    processor->code_size = 0;
    free(processor->cmd_code);
}
