#include <stdio.h>
#include <stdlib.h>
#include "src/load.h"
#include "src/shims.h"
#include "src/vm.h"

static uint8_t memory_pool[MRBC_MEMORY_SIZE];
static mrbc_vm vm;
static mrbc_value mrbc_regs[MAX_REGS];

void run_ruby(char *mrb_path)
{
  FILE *file = fopen(mrb_path, "rb");
  if (!file) {
    printf("Error: Could not open file %s\n", mrb_path);
    return;
  }
  
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  uint8_t *p = malloc(file_size);
  if (!p) {
    printf("Error: Could not allocate memory\n");
    fclose(file);
    return;
  }
  
  fread(p, 1, file_size, file);
  fclose(file);
  
  vm.cur_regs = mrbc_regs;
  mrbc_load_mrb(&vm, p);
  
  free(p);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    puts("usage: nones_test foo.mrb");
    return 1;
  }
  run_ruby(argv[1]);
  return 0;
}
