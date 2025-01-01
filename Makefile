VPATH = src
DEPDIR := .deps
TARGET_LIB = lib_a2560k_68040
CALYPSI_INSTALL = /usr/local/Homebrew/lib/calypsi-68000-5.7.1

# debug logging levels: 1=error, 2=warn, 3=info, 4=debug general, 5=allocations
DEBUG_DEF_1=LOG_LEVEL_1
# DEBUG_DEF_2=LOG_LEVEL_2
# DEBUG_DEF_3=LOG_LEVEL_3
DEBUG_DEF_4=LOG_LEVEL_4
# DEBUG_DEF_5=LOG_LEVEL_5
# DEBUG_DEF_1=NO_LOG_LEVEL_1
DEBUG_DEF_2=NO_LOG_LEVEL_2
DEBUG_DEF_3=NO_LOG_LEVEL_3
# DEBUG_DEF_4=NO_LOG_LEVEL_4
DEBUG_DEF_5=NO_LOG_LEVEL_5

# whether disk or serial debug will be used, IF debug is actually on
# defining serial debug means serial will be used, not defining it means disk will be used. 
#DEBUG_VIA_SERIAL=USE_SERIAL_LOGGING
DEBUG_VIA_SERIAL=USE_DISK_LOGGING

# source files
ASM_SRCS =
C_SRCS = sys_test.c general_test.c font_test.c window_test.c bitmap_test.c text_test.c main.c startup.c sys_demo.c font_demo.c window_demo.c bitmap_demo.c text_demo.c bitmap.c control_template.c control.c debug.c event.c font.c general.c list.c menu.c mouse.c sys.c text.c theme.c window.c  startup.c
LIB_SRCS = bitmap.c control_template.c control.c debug.c event.c font.c general.c list.c menu.c mouse.c sys.c text.c theme.c window.c  startup.c
# Test source files (also requires core)
TEST_SRCS = sys_test.c general_test.c font_test.c window_test.c bitmap_test.c text_test.c
DEMO_SRCS = sys_demo.c font_demo.c window_demo.c bitmap_demo.c text_demo.c


MODEL = --code-model=large --data-model=large
LIB_MODEL = lc-ld

# Object files
OBJS = $(ASM_SRCS:%.s=obj/%.o) $(C_SRCS:%.c=obj/%.o)
#OBJS_DEBUG = $(ASM_SRCS:%.s=obj/%-debug.o) $(C_SRCS:%.c=obj/%-debug.o)
LIB_OBJS = $(LIB_SRCS:%.c=obj/%.o)
TEST_OBJS = $(TEST_SRCS:%.c=obj/%.o)
DEMO_OBJS = $(DEMO_SRCS:%.c=obj/%.o)

obj/%.o: %.s
	as68k --core=68040 $(MODEL)  --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%.o: %.c $(DEPDIR)/%.d | $(DEPDIR)
	@cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --core=68040 $(MODEL)  -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --dependencies -MQ$@ >$(DEPDIR)/$*.d $<
	cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL) --core=68040 $(MODEL)  -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.s
	as68k --core=68040 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.c $(DEPDIR)/%-debug.d | $(DEPDIR)
	@cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL)  -core=68040 $(MODEL) --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --dependencies -MQ$@ >$(DEPDIR)/$*-debug.d $<
	cc68k -D_A2560K_ -D$(DEBUG_DEF_1) -D$(DEBUG_DEF_2) -D$(DEBUG_DEF_3) -D$(DEBUG_DEF_4) -D$(DEBUG_DEF_5) -D$(DEBUG_VIA_SERIAL)  --core=68040 $(MODEL) --debug -I$(CALYPSI_INSTALL)/contrib/Foenix-SDK/include --list-file=$(@:%.o=%.lst) -o $@ $<

all: lib tests demos

lib:	$(LIB_OBJS)
	@echo "Building library..."
	nlib $(TARGET_LIB)/a2560_sys.a $(LIB_OBJS)

tests:	$(TEST_OBJS)
	@echo "Building tests..."
	ln68k -o test_general.pgz obj/general_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_general.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000  
	ln68k -o test_sys.pgz obj/sys_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_sys.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000  
	ln68k -o test_font.pgz obj/font_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_font.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000  
	ln68k -o test_window.pgz obj/window_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_window.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000  
	ln68k -o test_bitmap.pgz obj/bitmap_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_bitmap.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000  
	ln68k -o test_text.pgz obj/text_test.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=test_text.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 

demos:	$(DEMO_OBJS)
	@echo "Building demos..."
	ln68k -o demo_sys.pgz obj/sys_demo.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=demo_sys.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 
	ln68k -o demo_font.pgz obj/font_demo.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=demo_font.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 
	ln68k -o demo_window.pgz obj/window_demo.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=demo_window.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 
	ln68k -o demo_bitmap.pgz obj/bitmap_demo.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=demo_bitmap.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 
	ln68k -o demo_text.pgz obj/text_demo.o a2560k_osf.scm $(TARGET_LIB)/a2560_sys.a a2560-68020-lc-sd.a --no-tree-shaking --output-format=pgz --list-file=demo_text.lst --cross-reference --rtattr cstartup=Foenix_user --heap-size=1000000 --stack-size=30000 



osf_debug.elf: $(OBJS_DEBUG)
	ln68k -o $@ $^ a2560k_osf.scm  --list-file=osf-debug.lst --cross-reference  --semi-hosted  --rtattr cstartup=Foenix_user --rtattr stubs=foenix --stack-size=8000 --sstack-size=800

osf.pgz:  $(OBJS)
#	ln68k -o $@ $^ a2560k_osf.scm --output-format=pgz --list-file=osf-pgz.lst --cross-reference --rtattr cstartup=Foenix_user
	ln68k -o osf.pgz obj/main.o a2560k_osf.scm a2560-68020-lc-ld.a $(TARGET_LIB)/a2560_sys.a --output-format=pgz --list-file=osf-pgz.lst --cross-reference --rtattr cstartup=Foenix_user 
	
osf.hex:  $(OBJS)
	ln68k -o $@ $^ a2560k_osf.scm --output-format=intel-hex --list-file=osf-hex.lst --cross-reference --rtattr cstartup=Foenix_morfe --stack-size=8000

clean:
	-rm $(DEPFILES)
	-rm $(TARGET_LIB)/*.a
	-rm $(DEMO_OBJS) obj/*.lst obj/*.o
	-rm $(TEST_OBJS) obj/*.lst obj/*.o
	-rm $(OBJS) $(OBJS:%.o=%.lst) $(OBJS_DEBUG) $(OBJS_DEBUG:%.o=%.lst)
	-rm osf-debug.elf osf.pgz osf-debug.lst osf-Foenix.lst

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(C_SRCS:%.c=$(DEPDIR)/%.d) $(C_SRCS:%.c=$(DEPDIR)/%-debug.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))
