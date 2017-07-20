.AUTODEPEND

#		*Translator Definitions*
CC = bcc +KDREAMS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\Z\BORLANDC\LIB
INCLUDEPATH = C:\Z\BORLANDC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 kdreams.bco \
 kd_demo.bco \
 kd_play.bco \
 kd_keen.bco \
 kd_act1.bco \
 kd_act2.bco \
 gelib.bco \
 jam_io.bco \
 soft.bco \
 lzhuf.bco \
 id_mm.bco \
 id_ca.bco \
 id_vw.bco \
 id_vw_a.bco \
 id_rf.bco \
 id_rf_a.bco \
 id_in.bco \
 id_sd.bco \
 id_us.bco \
 id_us_a.bco \
 kdredict.obj \
 kdrehead.obj \
 kdrmdict.obj \
 kdrmhead.obj \
 kdradict.obj \
 kdrahead.obj \
 context.obj \
 gametext.obj \
 story.obj

#		*Explicit Rules*
kdreams.exe: kdreams.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/L$(LIBPATH) @&&|
c0m.obj+
kdreams.bco+
kd_demo.bco+
kd_play.bco+
kd_keen.bco+
kd_act1.bco+
kd_act2.bco+
gelib.bco+
jam_io.bco+
soft.bco+
lzhuf.bco+
id_mm.bco+
id_ca.bco+
id_vw.bco+
id_vw_a.bco+
id_rf.bco+
id_rf_a.bco+
id_in.bco+
id_sd.bco+
id_us.bco+
id_us_a.bco+
kdredict.obj+
kdrehead.obj+
kdrmdict.obj+
kdrmhead.obj+
kdradict.obj+
kdrahead.obj+
context.obj+
gametext.obj+
story.obj
kdreams
		# no map file
cm.lib
|


#		*Individual File Dependencies*
kdreams.bco: kdreams.cfg kdreams.c 

kd_demo.bco: kdreams.cfg kd_demo.c 

kd_play.bco: kdreams.cfg kd_play.c 

kd_keen.bco: kdreams.cfg kd_keen.c 

kd_act1.bco: kdreams.cfg kd_act1.c 

kd_act2.bco: kdreams.cfg kd_act2.c 

gelib.bco: kdreams.cfg gelib.c 

jam_io.bco: kdreams.cfg jam_io.c 

soft.bco: kdreams.cfg soft.c 

lzhuf.bco: kdreams.cfg lzhuf.c 

id_mm.bco: kdreams.cfg id_mm.c 

id_ca.bco: kdreams.cfg id_ca.c 

id_vw.bco: kdreams.cfg id_vw.c 

id_vw_a.bco: kdreams.cfg id_vw_a.asm 
	$(TASM) /MX /ZI /O ID_VW_A.ASM,ID_VW_A.BCO

id_rf.bco: kdreams.cfg id_rf.c 

id_rf_a.bco: kdreams.cfg id_rf_a.asm 
	$(TASM) /MX /ZI /O ID_RF_A.ASM,ID_RF_A.BCO

id_in.bco: kdreams.cfg id_in.c 

id_sd.bco: kdreams.cfg id_sd.c 

id_us.bco: kdreams.cfg id_us.c 

id_us_a.bco: kdreams.cfg id_us_a.asm 
	$(TASM) /MX /ZI /O ID_US_A.ASM,ID_US_A.BCO

#		*Compiler Configuration File*
kdreams.cfg: kdreams.mak
  copy &&|
-mm
-a
-f-
-K
-v
-G
-O
-Z
-k-
-d
-vi-
-H=KDREAMS.SYM
-wamb
-wpro
-wnod
-weas
-wpre
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| kdreams.cfg


