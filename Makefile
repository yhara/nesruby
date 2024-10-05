PROJECT  := $(shell basename $(CURDIR))

SRC_DIR  := src
BIN_DIR  := bin
OBJ_DIR  := obj

TARGET   := $(BIN_DIR)/$(PROJECT).nes

CFILES   := $(wildcard $(SRC_DIR)/*.c)
SFILES   := $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*.asm)
LIBS     := nes.lib atmos.lib

OBJS     := \
	$(foreach file, $(CFILES), $(OBJ_DIR)/$(basename $(notdir $(file))).o) \
	$(foreach file, $(SFILES), $(OBJ_DIR)/$(basename $(notdir $(file))).o)

DEPENDS  := $(wildcard $(OBJ_DIR)/*.dep)

ASFLAGS  := -t nes
CFLAGS   := -t nes -j
LDFLAGS  := -t nes

CC       := cl65
AS       := cl65
LD       := ld65
RM       := rm -rf
MKDIR    := mkdir

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) --create-dep $@.dep -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) --create-dep $@.dep -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) $(ASFLAGS) --create-dep $@.dep -c -o $@ $<

.PHONY: all
all: init $(TARGET)

.PHONY: clean
clean:
	- $(RM) $(BIN_DIR)
	- $(RM) $(OBJ_DIR)

.PHONY: init
init:
	@$(if $(shell ls | grep $(BIN_DIR)) , , $(MKDIR) $(BIN_DIR))
	@$(if $(shell ls | grep $(OBJ_DIR)) , , $(MKDIR) $(OBJ_DIR))

.PHONY: run
run: all
	$(VM_PATH) $(TARGET)

$(TARGET): $(OBJS)
	@ echo Create $@
	$(LD) $(LDFLAGS) -m $@.map -o $@ --obj $(OBJS) --lib $(LIBS)
	@ echo Done.

-include $(DEPENDS)
