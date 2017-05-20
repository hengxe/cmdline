CC :=$(CROSS_COMPILE)gcc
LD :=$(CROSS_COMPILE)ld
AR :=$(CROSS_COMPILE)ar
RM :=/bin/rm -rf

$(shell mkdir -p obj)
OBJ_PATH := $(PWD)/obj

vpath %.c $(SRC_PATH)

SRC := $(foreach x,$(SRC_PATH),$(wildcard $(x)/*.c)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o,$(notdir $(basename $(SRC)))))
DEP := $(addprefix $(OBJ_PATH)/, $(addsuffix .d,$(notdir $(basename $(SRC)))))

$(TARGET): $(OBJ)
	@echo "Generating executable file..." $(notdir $(TARGET))
	@$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@

sinclude $(DEP)	
$(OBJ_PATH)/%.d: %.c
	@set -e; $(RM) $@; \
	$(CC) $(CPPFLAGS) -MM  $< > $@.$$$$; \
	/bin/sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$	
	
$(OBJ_PATH)/%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $<)))
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@	
	
.PHONY : all clean

all : $(TARGET)
clean:
	@echo "cleaning..."
	@$(RM) $(OBJ) $(DEP) $(TARGET)

distclean: clean
	@$(RM) $(OBJ_PATH) 
