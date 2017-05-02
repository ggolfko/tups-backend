ACLOCAL_AMFLAGS = -I m4 

bin_PROGRAMS = tupsDebug sims sim_java sim_c esandbox judge compile clearQueue
TUPSdir = $(prefix)/data
TUPS_DATA = data/config.sample data/java.policy

clearQueue_SOURCES = \
    src/clearQueue.cpp
clearQueue_CFLAGS = -Wall -O2
clearQueue_LDADD = $(LIBBEANSTALK_LIBS)

sim_java_SOURCES = \
    src/sim/token.c \
    src/sim/lex.c \
    src/sim/stream.c \
    src/sim/text.c \
    src/sim/tokenarray.c \
    src/sim/error.c \
    src/sim/debug.c \
    src/sim/ForEachFile.c \
    src/sim/fname.c \
    src/sim/Malloc.c \
    src/sim/idf.c \
    src/sim/runs.c \
    src/sim/percentages.c \
    src/sim/sim.c \
    src/sim/options.c \
    src/sim/newargs.c \
    src/sim/hash.c \
    src/sim/compare.c \
    src/sim/add_run.c \
    src/sim/pass1.c \
    src/sim/pass2.c \
    src/sim/pass3.c \
    src/sim/algollike.c \
    src/sim/javalang.l 
sim_java_CFLAGS = -DUNIX -DSOLARIS -DMEMLEAK -DMEMCLOBBER -O2 -w

sim_c_SOURCES = \
    src/sim/token.c \
    src/sim/lex.c \
    src/sim/stream.c \
    src/sim/text.c \
    src/sim/tokenarray.c \
    src/sim/error.c \
    src/sim/debug.c \
    src/sim/ForEachFile.c \
    src/sim/fname.c \
    src/sim/Malloc.c \
    src/sim/idf.c \
    src/sim/runs.c \
    src/sim/percentages.c \
    src/sim/sim.c \
    src/sim/options.c \
    src/sim/newargs.c \
    src/sim/hash.c \
    src/sim/compare.c \
    src/sim/add_run.c \
    src/sim/pass1.c \
    src/sim/pass2.c \
    src/sim/pass3.c \
    src/sim/algollike.c \
    src/sim/clang.l 
sim_c_CFLAGS = -DUNIX -DSOLARIS -DMEMLEAK -DMEMCLOBBER -O2 -w

sims_SOURCES = \
    src/config.cpp \
    src/backendSim.cpp \
    src/sim.cpp \
    src/BeanstalkSim.cpp \
    src/odb/solution-odb.cxx \
    src/odb/source_code-odb.cxx \
    src/odb/sim_result-odb.cxx
sims_CPPFLAGS = -Wall -O2 -Wno-unused-result -Wno-unknown-pragmas -DDEBUG
sims_LDFLAGS = 
sims_LDADD = $(LIBODB_LIBS) $(LIBODBMYSQL_LIBS) $(LIBBEANSTALK_LIBS)

judge_SOURCES = \
    src/config.cpp \
	src/backendJudge.cpp \
    src/BeanstalkSim.cpp \
	src/judge.cpp \
	src/BeanstalkJudge.cpp \
    src/odb/solution-odb.cxx \
    src/odb/source_code-odb.cxx \
    src/odb/test_case-odb.cxx \
    src/odb/diff_option-odb.cxx \
    src/odb/problem-odb.cxx \
    src/odb/compile_info-odb.cxx \
    src/odb/diff_result-odb.cxx

judge_CPPFLAGS = -Wall -O2 -Wno-unused-result -Wno-unknown-pragmas -DDEBUG
judge_LDFLAGS = 
judge_LDADD = $(LIBODB_LIBS) $(LIBODBMYSQL_LIBS) $(LIBBEANSTALK_LIBS)

tupsDebug_SOURCES = \
    src/config.cpp \
    src/backendDebug.cpp \
    src/debug.cpp \
    src/BeanstalkClient.cpp \
    src/odb/compile-odb.cxx \
    src/odb/source_code-odb.cxx \
    src/odb/debug-odb.cxx 
tupsDebug_CPPFLAGS = -Wall -O2 -Wno-unused-result -Wno-unknown-pragmas -DDEBUG
tupsDebug_LDFLAGS = 
tupsDebug_LDADD = $(LIBODB_LIBS) $(LIBODBMYSQL_LIBS) $(LIBBEANSTALK_LIBS)

compile_SOURCES = \
    src/config.cpp \
	src/backend.cpp \
	src/compile.cpp \
	src/BeanstalkClient.cpp \
    src/odb/compile-odb.cxx \
    src/odb/source_code-odb.cxx

compile_CPPFLAGS = -Wall -O2 -Wno-unused-result -Wno-unknown-pragmas -DDEBUG
compile_LDFLAGS = 
compile_LDADD = $(LIBODB_LIBS) $(LIBODBMYSQL_LIBS) $(LIBBEANSTALK_LIBS) 

esandbox_SOURCES = \
    src/sandbox/sandbox.c

esandbox_CFLAGS = -O2 
esandbox_LDFLAGS =  
esandbox_LDADD = $(LIBSANDBOX_LIBS) 

