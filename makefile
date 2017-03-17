#
#	Open Watcom Keen Dreams makefile
#
#
# Possible optimizations for 8088 class processors
#
# -oa   Relax alias checking
# -ob   Try to generate straight line code
# -oe - expand user functions inline (-oe=20 is default, adds lots of code)
# -oh   Enable repeated optimizations
# -oi   generate certain lib funcs inline
# -oi+  Set max inline depth (C++ only, use -oi for C)
# -ok   Flowing of register save into function flow graph
# -ol   loop optimizations
# -ol+  loop optimizations plus unrolling
# -or   Reorder for pipelined (486+ procs); not sure if good to use
# -os   Favor space over time
# -ot   Favor time over space
# -ei   Allocate an "int" for all enum types
# -zp2  Allow compiler to add padding to structs
# -zpw  Use with above; make sure you are warning free!
# -0    8088/8086 class code generation
# -s    disable stack overflow checking

# -zk0u	translate kanji to unicode... wwww
# -zk0	kanji support~
# -zkl		current codepage

#192x144
#wwww will add these
!ifdef __LINUX__
#!ifdef UNIX
to_os_path=\=/
REMOVECOMMAND=rm -f
COPYCOMMAND=cp -f
DIRSEP=/
OBJ=obj
DUMP=cat
!else		#DOS ^^
to_os_path=/=\
REMOVECOMMAND=del
COPYCOMMAND=copy /y
DIRSEP=\
OBJ=obj
DUMP=type
!endif

TARGET_OS = dos

BUILD_ROOT=$+$(%__CWD__)$-
#DATADIR=data$(DIRSEP)


#
# quiet flags
#
WLIBQ=-q
WCLQ=-q
UPXQ=-qqq

#
# compile flags
#
#-zd{f,p} DS floats vs DS pegged to DGROUP
#-zu SS != DGROUP
#-zp{1,2,4,8,16} struct packing align.
#-ei force enums to be type int
#-wo diagnose problems in overlaid code
S_FLAGS=-s -wo
## -zu -zdp
#-sg -st -of+ -zdf -zff -zgf# -k16768# -zt=84
Z_FLAGS=-zk0 -zc -zm -ei -zp16
O_FLAGS=-opnr -oe=24 -oil+ -outback -ohm -okf+
T_FLAGS=-bt=dos -mm -0 -fpi87 -fo=.$(OBJ) -d1

CPPFLAGS=-DTARGET_MSDOS=16 -DMSDOS=1
AFLAGS=$(WCLQ) $(T_FLAGS)
CFLAGS=$(WCLQ) $(T_FLAGS) -wo $(O_FLAGS) $(S_FLAGS) $(Z_FLAGS)
LFLAGS=$(WCLQ) -l=dos -fm=$^&.meh $(S_FLAGS)
LIBFLAGS=$(WLIBQ) -b -n

#
# objects
#
#CONTEXT.OBJ GAMETEXT.OBJ KDRADICT.OBJ KDRAHEAD.OBJ KDRCDICT.OBJ KDRCHEAD.OBJ KDREDICT.OBJ KDREHEAD.OBJ KDRMDICT.OBJ KDRMHEAD.OBJ STORY.OBJ
STATICOBJS = context.obj gametext.obj kdradict.obj kdrahead.obj kdrcdict.obj kdrchead.obj kdredict.obj kdrehead.obj kdrmdict.obj kdrmhead.obj story.obj
IDENGOBJS = id_mm.$(OBJ) id_ca.$(OBJ) id_pm.$(OBJ) id_vw.$(OBJ) id_rf.$(OBJ) id_in.$(OBJ) id_sd.$(OBJ) id_us.$(OBJ)
MOREOBJS = gelib.$(OBJ) jam_io.$(OBJ) soft.$(OBJ) lshuf$(OBJ)
#kdass$(OBJ)
KDOBJS = kd_demo.$(OBJ) kd_play.$(OBJ) kd_keen.$(OBJ) kd_act1.$(OBJ) kd_act2.$(OBJ) $(IDENGOBJS) $(MOREOBJS)
# $(STATICOBJS)
#id_us_a.$(OBJ) id_sd_a.$(OBJ) id_sd_a.$(OBJ)

#
# libraries
#

#
#	Files locations
#
.c : .#$(SRC)

#.asm : $(SRC)

#.lib : .;$(DOSLIB_CPU)/dos86h;$(DOSLIB_DOS)/dos86h;$(DOSLIB_VGA)/dos86h;$(DOSLIB_8250)/dos86h

.obj : .;static/

#
#	Default make rules
#
.c.obj:
	*wcl $(CPPFLAGS) $(CFLAGS) $(extra_$^&_obj_opts) -c $[@

.asm.obj:
	*wcl $(AFLAGS) $(extra_$^&_obj_opts) -c $[@

#CFLAGS is neccessary here
.obj.exe :
	*wcl $(LFLAGS) $(extra_$^&_exe_opts)-fe=$@ $<

.obj.lib :
	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

#
# List of executables to build
#
EXEC = &
	kdreams.exe

all: $(STATICOBJS) $(EXEC)

#
# game executables
#
kdreams.exe:	kdreams.$(OBJ) $(KDOBJS) $(STATICOBJS)

#
# Test Executables!
#
#kdreamste.exe:	kdreamste.$(OBJ) $(KDOBJS)# $(TESTOBJS)

#
# executable's objects
#
kdreams.$(OBJ):	kdreams.c
#kdreamste.$(OBJ):	kdreamste.c
#sega.$(OBJ):	sega.c

#
# non executable objects libraries
#
$(STATICOBJS): .symbolic
	@cd static
!ifdef __LINUX__
	@. ./make.sh
!else
	@make.bat
!endif
	@cd ..

#kdass.lib: .symbolic
#	@if exist src/KDASS.LIB	cp src/KDASS.LIB ./kdass.lib

#gfx.lib: $(GFXLIBOBJS)
#	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

id_pm.$(OBJ):	id_pm.c
id_mm.$(OBJ):	id_mm.c
id_ca.$(OBJ):	id_ca.c
id_vw.$(OBJ):	id_vw.c
id_in.$(OBJ):	id_in.c
id_sd.$(OBJ):	id_sd.c
id_sd_a.$(OBJ):	id_sd_a.c
id_us_1.$(OBJ):	id_us_1.c
id_us_a.$(OBJ):	id_us_a.c
kd_act1.$(OBJ):	kd_act1.c
kd_act2.$(OBJ):	kd_act2.c
kd_play.$(OBJ):	kd_play.c
kd_demo.$(OBJ):	kd_demo.c
kdass.$(OBJ):	kdass.c
#id_exter.$(OBJ):	id_exter.c
#id_vl_a.$(OBJ):	id_vl_a.asm

#
#other~
#
clean: .symbolic
	@for %f in ($(EXEC)) do @if exist %f $(REMOVECOMMAND) %f
!ifdef __LINUX__
	@rm *.LIB
	@rm *.EXE
	#++@if exist src/obj/*.EXE	mv src/obj/*.EXE bcwolf.exe
	@if exist *.OBJ $(REMOVECOMMAND) *.OBJ
	#@wmake -h bomb
!endif
	@for %f in ($(KDOBJS)) do @if exist %f $(REMOVECOMMAND) %f
	@if exist *.LIB $(REMOVECOMMAND) *.LIB
	@if exist *.lnk $(REMOVECOMMAND) *.lnk
	@if exist *.LNK $(REMOVECOMMAND) *.LNK
	@if exist *.smp $(REMOVECOMMAND) *.smp
	@if exist *.SMP $(REMOVECOMMAND) *.SMP
	@if exist *.hed $(REMOVECOMMAND) *.hed
	@if exist *.mah $(REMOVECOMMAND) *.mah
	@if exist *.MAH $(REMOVECOMMAND) *.MAH
	@if exist *.err $(REMOVECOMMAND) *.err

bomb: .symbolic
	#@wmake -h clean
	#@if exist src/obj/*.*	$(REMOVECOMMAND) src/obj/*.*
!ifdef __LINUX__
	#@if exist src/obj/*.OBJ	$(REMOVECOMMAND) src/obj/*.OBJ
	@if exist TC*.SWP	$(REMOVECOMMAND) TC*.SWP
!endif

backupconfig: .symbolic
	@$(COPYCOMMAND) .git$(DIRSEP)config git_con.fig
#	@$(COPYCOMMAND) .gitmodules git_modu.les
	@$(COPYCOMMAND) .gitignore git_igno.re

comp: .symbolic
	@*upx -9 $(EXEC)

comq: .symbolic
	@*upx -9 $(UPXQ) $(EXEC)

vomitchan: .symbolic
	@if exist *.err $(DUMP) *.err

##
##	External library management~ ^^
##
#git submodule add <repo>
reinitlibs: .symbolic
	@wmake -h initlibs

initlibs: .symbolic
	@cp git_con.fig .git/config
#	@cp git_modu.les .gitmodules
	@cp git_igno.re .gitignore
