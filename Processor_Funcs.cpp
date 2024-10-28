#include "Processor_Funcs.h"

//debug

void Printf_Format_Function(const void * value) {

    double tmp = 0;
    memcpy(&tmp, value, sizeof(double));
    printf("%.2lg ", tmp);
}

Commands_data_struct Commands_data[] =
{

#include "Commands_data.h"

};

#undef COMMANDS_DEF
/*********************************************************** */

void SPU_Ctor(SPU_data * processor, const char* filename) {

    const int poison = -666;
    STACK_CTOR(&(processor->stack), 10, sizeof(double), &poison);
    STACK_CTOR(&(processor->ret_val_stack), 10, sizeof(uint64_t), &poison);
    Read_code_file(processor, filename);
    double* tmp_ptr = (double*) calloc(RAM_Size, sizeof(double));
    if(tmp_ptr) {

        processor->RAM = tmp_ptr;
        tmp_ptr = NULL;
    }
    else
        fprintf(stderr, "ERROR: RAM was not allocated\n");
}

void SPU_Run(SPU_data * processor) { // TODO: rename (prefix)

    while(true) {

        if(processor->cmd_code[processor->IP] == CMD_HLT)
            break;

        else
            Commands_data[processor->cmd_code[processor->IP]].SPU_Func(processor);
            SPU_DUMP(processor, Printf_Format_Function);

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

        char* tmp = (char*) calloc(processor->code_size+10, sizeof(char));
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

    fread(processor->cmd_code, sizeof(char), processor->code_size, file);
    printf("read_code_file\n");
    for(int i = 0; i < processor->code_size; i++)
        printf("%x ", processor->cmd_code[i]);
    printf("\n");
}

void SPU_Push(SPU_data* processor) {

    double tmp = Get_push_arg(processor);
    StackPush(&processor->stack, &tmp);
}

double Get_push_arg(SPU_data* processor) {

    int option_byte = processor->cmd_code[processor->IP+1];
    processor->IP += 2;

    if(option_byte & RAM_MASK) {

        int64_t result = 0;

        if(option_byte & REGISTER_MASK) {

            result += (int64_t) processor->registers[processor->cmd_code[processor->IP]];
            processor->IP++;
        }

        if(option_byte & NUMBER_MASK) {

            int64_t temp_int = 0;
            memcpy(&temp_int, &processor->cmd_code[processor->IP], sizeof(int64_t));
            result += temp_int;
            processor->IP += sizeof(int64_t);
        }

        return (processor->RAM[result]);
    }

    else {

        double result = 0;

        if(option_byte & REGISTER_MASK) {

            result += processor->registers[processor->cmd_code[processor->IP]];
            processor->IP++;
        }

        if(option_byte & NUMBER_MASK) {

                double temp_int = 0;
                memcpy(&temp_int, &processor->cmd_code[processor->IP], sizeof(double));
                result += temp_int;
                processor->IP += sizeof(double);
            }

        return result;
    }
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
    int64_t result = 0;
    if(option_byte & RAM_MASK) {

        if(option_byte & REGISTER_MASK) {

            result += (int64_t)processor->registers[processor->cmd_code[processor->IP]];
            processor->IP++;
        }

        if(option_byte & NUMBER_MASK) {

            int64_t temp_int = 0;
            memcpy(&temp_int, &processor->cmd_code[processor->IP], sizeof(int64_t));
            result += temp_int;
            processor->IP += sizeof(int64_t);
        }

        return &processor->RAM[result];

    }

    else {

        processor->IP++;
        return &processor->registers[processor->cmd_code[processor->IP-1]];
    }
}

void SPU_Add(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    double sum = pop2 + pop1;
    StackPush(&(processor->stack), &sum);

    processor->IP++;
}

void SPU_Sub(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    double difference = pop2 - pop1;
    StackPush(&(processor->stack), &difference);

    processor->IP++;
}

void SPU_Mul(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    double product = pop2 * pop1;
    StackPush(&(processor->stack), &product);

    processor->IP++;
}

void SPU_Div(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    double quotient = pop2 / pop1;
    StackPush(&(processor->stack), &quotient);

    processor->IP++;
}

void SPU_Out(SPU_data* processor) {

    double out = 0;
    StackPop(&(processor->stack), &out);
    printf("\nOUT: %lg\n\n", out);
    processor->IP++;
}

void SPU_Ja(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(pop1 < pop2)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Jae(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(pop1 < pop2)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Jb(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(pop1 > pop2)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Jbe(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(pop1 >= pop2)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Je(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(fabs(pop1 - pop2) < Epsilon)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Jne(SPU_data* processor) {

    double pop1 = 0;
    double pop2 = 0;

    StackPop(&(processor->stack), &pop1);
    StackPop(&(processor->stack), &pop2);

    if(fabs(pop1 - pop2) > Epsilon)
        memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));

    else
        processor->IP += 1 + sizeof(uint64_t);
}

void SPU_Jmp(SPU_data* processor) {

    memcpy(&processor->IP, &processor->cmd_code[processor->IP+1], sizeof(uint64_t));
}

void SPU_Call(SPU_data* processor) {

    uint64_t tmp_int = processor->IP + 1 + sizeof(uint64_t);
    StackPush(&processor->ret_val_stack, &tmp_int);
    SPU_Jmp(processor);
}

void SPU_Ret(SPU_data* processor) {

    uint64_t tmp_addr = 0;
    StackPop(&processor->ret_val_stack, &tmp_addr);
    processor->IP = tmp_addr;
}

void SPU_RAM_Draw(SPU_data* processor) {

    processor->IP++;
    uint64_t height = 0, width = 0;
    memcpy(&height, &processor->cmd_code[processor->IP], sizeof(uint64_t));
    processor->IP += sizeof(uint64_t);
    memcpy(&width, &processor->cmd_code[processor->IP], sizeof(uint64_t));
    processor->IP += sizeof(uint64_t);

    for(uint64_t y = 0; y < height*width; y += width) {
        for(uint64_t x = y; x < y + width; x++)
            printf("%c", (char) processor->RAM[x]);

        printf("\n");
    }
}
void SPU_Dtor(SPU_data* processor) {

    StackDtor(&processor->stack);
    processor->IP = 0;
    processor->code_size = 0;
    free(processor->cmd_code);
}
