VPATH = src
DEPDIR := .deps
TARGET_LIB = lib_a2560k_68040
CALYPSI_INSTALL = /usr/local/Homebrew/lib/calypsi-68000-5.7.1

# debug logging levels: 1=error, 2=warn, 3=info, 4=debug general, 5=allocations
# DEBUG_DEF_1=LOG_LEVEL_1
# DEBUG_DEF_2=LOG_LEVEL_2
# DEBUG_DEF_3=LOG_LEVEL_3
# DEBUG_DEF_4=LOG_LEVEL_4
# DEBUG_DEF_5=LOG_LEVEL_5
DEBUG_DEF_1=NO_LOG_LEVEL_1
DEBUG_DEF_2=NO_LOG_LEVEL_2
DEBUG_DEF_3=NO_LOG_LEVEL_3
DEBUG_DEF_4=NO_LOG_LEVEL_4
DEBUG_DEF_5=NO_LOG_LEVEL_5

# whether disk or serial debug will be used, IF debug is actually on
# defining serial debug means serial will be used, not defining it means disk will be used. 
#DEBUG_VIA_SERIAL=USE_SERIAL_LOGGING
DEBUG_VIA_SERIAL=USE_DISK_LOGGING

# source files
ASM_SRCS =
#C_SRCS = main.c bitmap.c control.c control_template.c debug.c event.c font.c general.c list.c menu.c mouse.c  sys.c text.c theme.c window.c
C_SRCS = sys_test.c general_test.c main.c
LIB_SRCS = bitmap.c control_template.c control.c debug.c event.c font.c general.c list.c menu.c mouse.c sys.c text.c theme.c window.c 
# Test source files (also requires core)
TEST_SRCS = sys_test.c general_test.c
#bitmap_test.c font_test.c text_test.c window_test.c general_test.c 
# Demo source files (also requires core)
TEXT_DEMO_SRCS = sys.c theme.c control_template.c font.c window.c control.c general.c bitmap.c text.c list.c event.c mouse.c menu.c text_demo.c
SYS_DEMO_SRCS = sys.c theme.c control_template.c font.c window.c control.c general.c bitmap.c text.c list.c event.c mouse.c menu.c sys_demo.c
BITMAP_DEMO_SRCS = bitmap_demo.c


MODEL = --code-model=large --data-model=small
LIB_MODEL = lc-sd

# Object files
OBJS = $(ASM_SRCS:%.s=obj/%.o) $(C_SRCS:%.c=obj/%.o)
#OBJS_DEBUG = $(ASM_SRCS:%.s=obj/%-debug.o) $(C_SRCS:%.c=obj/%-debug.o)
#C_OBJS = $(C_SRCS:%.c=obj/%.o)
LIB_OBJS = $(LIB_SRCS:%.c=obj/%.o)
TEST_OBJS = $(TEST_SRCS:%.c=obj/%.o)
DEMO_OBJS = $(DEMO_SRCS:%.c=obj/%.o)
#TUTORIAL_OBJS = $(TUTORIAL_SRCS:%.c=obj/%.o)
TEXT_DEMO_OBJS = $(TEXT_DEMO_SRCS:%.c=obj/%.o)
SYS_DEMO_OBJS = $(SYS_DEMO_SRCS:%.c=obj/%.o)
BITMAP_DEMO_OBJS = $(BITMAP_DEMO_SRCS:%.c=obj/%.o)

obj/%.o: %.s
	as68k --core=68040 $(MODEL) --target=Foenix --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%.o: %.c $(DEPDIR)/%.d | $(DEPDIR)
	@cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --core=68040 $(MODEL) --target=Foenix --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --dependencies -MQ$@ >$(DEPDIR)/$*.d $<
	cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --core=68040 $(MODEL) --target=Foenix --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.s
	as68k --core=68040 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.c $(DEPDIR)/%-debug.d | $(DEPDIR)
	@cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --target=Foenix -core=68040 $(MODEL) --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --dependencies -MQ$@ >$(DEPDIR)/$*-debug.d $<
	cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --target=Foenix --core=68040 $(MODEL) --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --list-file=$(@:%.o=%.lst) -o $@ $<

all: lib tests osf.pgz

lib:	$(LIB_OBJS)
	@echo "Building library..."
#	ln68k -o $@ $^ $(FOENIX_LINKER_RULES) clib-68000-$(LIB_MODEL)-foenix.a --output-format=raw --list-file=a2560_sys.lst --cross-reference --rtattr printf=float --rtattr cstartup=Foenix_user
	nlib $(TARGET_LIB)/a2560_sys.a $(LIB_OBJS)

tests:	$(TEST_OBJS)
	@echo "Building tests..."
	ln68k -o sys_test.pgz obj/sys_test.o a2560k.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --output-format=pgz --list-file=osf-sys-test.lst --cross-reference --rtattr cstartup=Foenix_user 
	ln68k -o general_test.pgz obj/general_test.o a2560k.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --output-format=pgz --list-file=osf-general-test.lst --cross-reference --rtattr cstartup=Foenix_user 

#--no-tree-shaking

osf_debug.elf: $(OBJS_DEBUG)
	ln68k --debug -o $@ $^ a2560k.scm  --list-file=osf-debug.lst --cross-reference  --semi-hosted --target=Foenix --rtattr cstartup=Foenix_user --rtattr stubs=foenix --stack-size=2000 --sstack-size=800

osf.pgz:  $(OBJS)
#	ln68k -o $@ $^ a2560k.scm --output-format=pgz --list-file=osf-pgz.lst --cross-reference --rtattr cstartup=Foenix_user
	ln68k -o osf.pgz obj/main.o a2560k.scm a2560-68020-lc-sd.a $(TARGET_LIB)/a2560_sys.a --output-format=pgz --list-file=osf-pgz.lst --cross-reference --rtattr cstartup=Foenix_user 
	
osf.hex:  $(OBJS)
	ln68k -o $@ $^ a2560k.scm --output-format=intel-hex --list-file=osf-hex.lst --cross-reference --rtattr cstartup=Foenix_morfe --stack-size=2000

clean:
	-rm $(DEPFILES)
	-rm $(TARGET_LIB)/*.a
	-rm $(SYS_DEMO_OBJS) obj/*.lst obj/*.o
	-rm $(TEXT_DEMO_OBJS) obj/*.lst obj/*.o
	-rm $(OBJS) $(OBJS:%.o=%.lst) $(OBJS_DEBUG) $(OBJS_DEBUG:%.o=%.lst)
	-rm osf-debug.elf osf.pgz osf-debug.lst osf-Foenix.lst

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(C_SRCS:%.c=$(DEPDIR)/%.d) $(C_SRCS:%.c=$(DEPDIR)/%-debug.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))
