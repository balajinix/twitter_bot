include ../Makefile.incl
PROJECT = twitterbot
PROJECTFLAGS = -DDEBUG=${DEBUG_LEVEL}# -DTWITTERBOT_DEBUG
ARCHIVE = ar -r

INCLUDE_DIR = -I. -I/usr/include/c++/4.4/ -I../include/json_parser -I../include/twitcurl

LIBS = -l${PROJECT} -ljsonparser -ltwitcurl -lcurl

LIB = ${LIB_DIR}/lib${PROJECT}.so

BINS = ${BIN_DIR}/t_target_finder \
       ${BIN_DIR}/t_spammer

OBJS = twitter_bot.o

all: ${LIB} ${BINS}
	@echo "build done!"

clean:
	@echo "cleaning libraries ..."
	@rm *.o ${BINS} ${LIB}
	@echo "done!"

${LIB}: ${OBJS}
	@echo "creating library file ..."
	@${ARCHIVE} ${LIB} ${OBJS}
	@ranlib ${LIB}
	@echo "done!"

.cc.o:
	@echo "compiling ..." $< "..."
	@${CXX} ${CXXFLAGS} ${PROJECTFLAGS} -shared -fPIC -c ${INCLUDE_DIR} $<

${BIN_DIR}/t_target_finder: t_target_finder.o ${LIB}
	@echo "creating binary:" $@
	@${CXX} ${CXXFLAGS} ${PROJECTFLAGS} $< ${LINK_DIR} ${LIBS} -o ${BIN_DIR}/t_target_finder
	@echo "done!"

${BIN_DIR}/t_spammer: t_spammer.o ${LIB}
	@echo "creating binary:" $@
	@${CXX} ${CXXFLAGS} ${PROJECTFLAGS} $< ${LINK_DIR} ${LIBS} -o ${BIN_DIR}/t_spammer
	@echo "done!"

# DO NOT DELETE THIS LINE
