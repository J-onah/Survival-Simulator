# CFLAGS = -Wall -Werror -ansi -pedantic -std=c99
CFLAGS = -g -lm

# Source and Object directory
SRCDIR = src
OBJDIR = obj
REPLAY_MENU_SRCDIR = ${SRCDIR}/replay_menu
REPLAY_MENU_OBJDIR = ${OBJDIR}/replay_menu
SETTINGS_MENU_SRCDIR = ${SRCDIR}/game_settings
SETTINGS_MENU_OBJDIR = ${OBJDIR}/game_settings
COMMON_SRCDIR = ${SRCDIR}/common
COMMON_OBJDIR = ${OBJDIR}/common
SIGNUP_SRCDIR = ${SRCDIR}/signup
SIGNUP_OBJDIR = ${OBJDIR}/signup
LOGIN_SRCDIR = ${SRCDIR}/login
LOGIN_OBJDIR = ${OBJDIR}/login
LEADERBOARD_SRCDIR = ${SRCDIR}/leaderboard
LEADERBOARD_OBJDIR = ${OBJDIR}/leaderboard
GAME_STATS_SAVE_LOAD_SRCDIR = ${SRCDIR}/game_stats_save_load
GAME_STATS_SAVE_LOAD_OBJDIR = ${OBJDIR}/game_stats_save_load
INVENTORY_SAVE_LOAD_SRCDIR = ${SRCDIR}/inventory_save_load
INVENTORY_SAVE_LOAD_OBJDIR = ${OBJDIR}/inventory_save_load
FILE_READ_WRITE_SRCDIR = ${SRCDIR}/file_read_write
FILE_READ_WRITE_OBJDIR = ${OBJDIR}/file_read_write
ATTEMPT_RESTORE_SRCDIR = ${SRCDIR}/attempt_restore
ATTEMPT_RESTORE_OBJDIR = ${OBJDIR}/attempt_restore
GAME_FILES_SRCDIR = ${SRCDIR}/game_files
GAME_FILES_OBJDIR = ${OBJDIR}/game_files

REPLAY_MENU_OFILES = ${REPLAY_MENU_OBJDIR}/replayMenu.o

SETTINGS_OFILES = ${SETTINGS_MENU_OBJDIR}/gameSettingsMenu.o \
					${SETTINGS_MENU_OBJDIR}/changePassword.o

COMMON_OFILES = ${COMMON_OBJDIR}/memAllocChecks.o 	\
				${COMMON_OBJDIR}/charCommonFuncs.o

SIGN_UP_OFILES = $(SIGNUP_OBJDIR)/signUpProcess.o 				\
				$(SIGNUP_OBJDIR)/signUpPasswordProcessing.o 	\
				$(SIGNUP_OBJDIR)/signUpUsernameProcessing.o 	\
				$(SIGNUP_OBJDIR)/encryption.o 					\
				$(SIGNUP_OBJDIR)/signUpProcessTest.o

LOGIN_OFILES = $(LOGIN_OBJDIR)/loginProcess.o

LEADERBOARD_OFILES = $(LEADERBOARD_OBJDIR)/leaderboardProcess.o							\
						$(LEADERBOARD_OBJDIR)/leaderboardProcessTest.o

GAME_STATS_SAVE_LOAD_OFILES = $(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcess.o		\
						$(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcessTest.o

INVENTORY_SAVE_LOAD_OFILES = $(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcess.o		\
						$(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcessTest.o

FILE_READ_WRITE_OFILES = $(FILE_READ_WRITE_OBJDIR)/fileReadWriteMainProcess.o	\
						$(FILE_READ_WRITE_OBJDIR)/fileReadWriteCommon.o			\
						$(FILE_READ_WRITE_OBJDIR)/fileAppendProcess.o			\
						$(FILE_READ_WRITE_OBJDIR)/fileClearProcess.o			\
						$(FILE_READ_WRITE_OBJDIR)/fileOverwriteProcess.o		\
						$(FILE_READ_WRITE_OBJDIR)/fileReadProcess.o				\
						$(FILE_READ_WRITE_OBJDIR)/fileReadWriteProcessTest.o	\
						$(FILE_READ_WRITE_OBJDIR)/fileStatementProcessTest.o		

ATTEMPT_RESTORE_OFILES = ${ATTEMPT_RESTORE_OBJDIR}/attemptRestoreProcess.o

GAME_OFILES = ${GAME_FILES_OBJDIR}/helper.o 		\
				${GAME_FILES_OBJDIR}/random.o 		\
				${GAME_FILES_OBJDIR}/inventory.o 	\
				${GAME_FILES_OBJDIR}/item.o 		\
				${GAME_FILES_OBJDIR}/animals.o 		\
				${GAME_FILES_OBJDIR}/actions.o 		\
				${GAME_FILES_OBJDIR}/game.o

OFILES = $(OBJDIR)/mainMenu.o				\
			${REPLAY_MENU_OFILES}			\
			${SETTINGS_OFILES}				\
			${COMMON_OFILES} 				\
			${SIGN_UP_OFILES} 				\
			$(LOGIN_OFILES) 				\
			$(LEADERBOARD_OFILES)			\
			$(GAME_STATS_SAVE_LOAD_OFILES)	\
			$(INVENTORY_SAVE_LOAD_OFILES)	\
			${FILE_READ_WRITE_OFILES}		\
			${ATTEMPT_RESTORE_OFILES}		\
			${GAME_OFILES}

.PHONY: clean

main: $(OFILES)
	gcc $(OFILES) -o SurvivalSim.exe $(CFLAGS)
	gcc $(OFILES) -o SurvivalSim.o $(CFLAGS)


##############################################################
################# MAKING MAIN MENU OBJ FILES #################
##############################################################
$(OBJDIR)/mainMenu.o: $(SRCDIR)/mainMenu.c
	gcc -c $(SRCDIR)/mainMenu.c -o $(OBJDIR)/mainMenu.o $(CFLAGS)
##############################################################
##############################################################
##############################################################


################################################################
################# MAKING REPLAY MENU OBJ FILES #################
################################################################
$(REPLAY_MENU_OBJDIR)/replayMenu.o: $(REPLAY_MENU_SRCDIR)/replayMenu.c
	gcc -c $(REPLAY_MENU_SRCDIR)/replayMenu.c -o $(REPLAY_MENU_OBJDIR)/replayMenu.o $(CFLAGS)
################################################################
################################################################
################################################################


##################################################################
################# MAKING SETTINGS MENU OBJ FILES #################
##################################################################
$(SETTINGS_MENU_OBJDIR)/gameSettingsMenu.o: $(SETTINGS_MENU_SRCDIR)/gameSettingsMenu.c
	gcc -c $(SETTINGS_MENU_SRCDIR)/gameSettingsMenu.c -o $(SETTINGS_MENU_OBJDIR)/gameSettingsMenu.o $(CFLAGS)

$(SETTINGS_MENU_OBJDIR)/changePassword.o: $(SETTINGS_MENU_SRCDIR)/changePassword.c
	gcc -c $(SETTINGS_MENU_SRCDIR)/changePassword.c -o $(SETTINGS_MENU_OBJDIR)/changePassword.o $(CFLAGS)
##################################################################
##################################################################
##################################################################


#################################################################
################# MAKING COMMON FUNCS OBJ FILES #################
#################################################################
${COMMON_OBJDIR}/memAllocChecks.o: ${COMMON_SRCDIR}/memAllocChecks.c
	gcc -c $(COMMON_SRCDIR)/memAllocChecks.c -o $(COMMON_OBJDIR)/memAllocChecks.o $(CFLAGS)

${COMMON_OBJDIR}/charCommonFuncs.o: ${COMMON_SRCDIR}/charCommonFuncs.c
	gcc -c $(COMMON_SRCDIR)/charCommonFuncs.c -o $(COMMON_OBJDIR)/charCommonFuncs.o $(CFLAGS)
#################################################################
#################################################################
#################################################################


###########################################################
################# MAKING SIGNUP OBJ FILES #################
###########################################################
$(SIGNUP_OBJDIR)/signUpProcess.o: $(SIGNUP_SRCDIR)/signUpProcess.c
	gcc -c $(SIGNUP_SRCDIR)/signUpProcess.c -o $(SIGNUP_OBJDIR)/signUpProcess.o $(CFLAGS)

$(SIGNUP_OBJDIR)/signUpPasswordProcessing.o: $(SIGNUP_SRCDIR)/signUpPasswordProcessing.c
	gcc -c $(SIGNUP_SRCDIR)/signUpPasswordProcessing.c -o $(SIGNUP_OBJDIR)/signUpPasswordProcessing.o $(CFLAGS)

$(SIGNUP_OBJDIR)/signUpUsernameProcessing.o: $(SIGNUP_SRCDIR)/signUpUsernameProcessing.c
	gcc -c $(SIGNUP_SRCDIR)/signUpUsernameProcessing.c -o $(SIGNUP_OBJDIR)/signUpUsernameProcessing.o $(CFLAGS)

$(SIGNUP_OBJDIR)/encryption.o: $(SIGNUP_SRCDIR)/encryption.c
	gcc -c $(SIGNUP_SRCDIR)/encryption.c -o $(SIGNUP_OBJDIR)/encryption.o $(CFLAGS)

$(SIGNUP_OBJDIR)/signUpProcessTest.o: $(SIGNUP_SRCDIR)/signUpProcessTest.c
	gcc -c $(SIGNUP_SRCDIR)/signUpProcessTest.c -o $(SIGNUP_OBJDIR)/signUpProcessTest.o $(CFLAGS)
###########################################################
###########################################################
###########################################################


##########################################################
################# MAKING LOGIN OBJ FILES #################
##########################################################
$(LOGIN_OBJDIR)/loginProcess.o: $(LOGIN_SRCDIR)/loginProcess.c
	gcc -c $(LOGIN_SRCDIR)/loginProcess.c -o $(LOGIN_OBJDIR)/loginProcess.o $(CFLAGS)
##########################################################
##########################################################
##########################################################


################################################################
################# MAKING LEADERBOARD OBJ FILES #################
################################################################
$(LEADERBOARD_OBJDIR)/leaderboardProcess.o: $(LEADERBOARD_SRCDIR)/leaderboardProcess.c
	gcc -c $(LEADERBOARD_SRCDIR)/leaderboardProcess.c -o $(LEADERBOARD_OBJDIR)/leaderboardProcess.o $(CFLAGS)

$(LEADERBOARD_OBJDIR)/leaderboardProcessTest.o: $(LEADERBOARD_SRCDIR)/leaderboardProcessTest.c
	gcc -c $(LEADERBOARD_SRCDIR)/leaderboardProcessTest.c -o $(LEADERBOARD_OBJDIR)/leaderboardProcessTest.o $(CFLAGS)
################################################################
################################################################
################################################################


#########################################################################
################# MAKING SAVE LOAD GAME STATS OBJ FILES #################
#########################################################################
$(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcess.o: $(GAME_STATS_SAVE_LOAD_SRCDIR)/saveLoadStatsProcess.c
	gcc -c $(GAME_STATS_SAVE_LOAD_SRCDIR)/saveLoadStatsProcess.c -o $(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcess.o $(CFLAGS)

$(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcessTest.o: $(GAME_STATS_SAVE_LOAD_SRCDIR)/saveLoadStatsProcessTest.c
	gcc -c $(GAME_STATS_SAVE_LOAD_SRCDIR)/saveLoadStatsProcessTest.c -o $(GAME_STATS_SAVE_LOAD_OBJDIR)/saveLoadStatsProcessTest.o $(CFLAGS)
#########################################################################
#########################################################################
#########################################################################


#############################################################################
################# MAKING SAVE LOAD GAME INVENTORY OBJ FILES #################
#############################################################################
$(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcess.o: $(INVENTORY_SAVE_LOAD_SRCDIR)/saveLoadInventoryProcess.c
	gcc -c $(INVENTORY_SAVE_LOAD_SRCDIR)/saveLoadInventoryProcess.c -o $(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcess.o $(CFLAGS)

$(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcessTest.o: $(INVENTORY_SAVE_LOAD_SRCDIR)/saveLoadInventoryProcessTest.c
	gcc -c $(INVENTORY_SAVE_LOAD_SRCDIR)/saveLoadInventoryProcessTest.c -o $(INVENTORY_SAVE_LOAD_OBJDIR)/saveLoadInventoryProcessTest.o $(CFLAGS)
#############################################################################
#############################################################################
#############################################################################


####################################################################
################# MAKING FILE READ-WRITE OBJ FILES #################
####################################################################
$(FILE_READ_WRITE_OBJDIR)/fileReadWriteMainProcess.o: $(FILE_READ_WRITE_SRCDIR)/fileReadWriteMainProcess.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileReadWriteMainProcess.c -o $(FILE_READ_WRITE_OBJDIR)/fileReadWriteMainProcess.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileReadWriteCommon.o: $(FILE_READ_WRITE_SRCDIR)/fileReadWriteCommon.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileReadWriteCommon.c -o $(FILE_READ_WRITE_OBJDIR)/fileReadWriteCommon.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileAppendProcess.o: $(FILE_READ_WRITE_SRCDIR)/fileAppendProcess.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileAppendProcess.c -o $(FILE_READ_WRITE_OBJDIR)/fileAppendProcess.o $(CFLAGS)
	
$(FILE_READ_WRITE_OBJDIR)/fileClearProcess.o: $(FILE_READ_WRITE_SRCDIR)/fileClearProcess.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileClearProcess.c -o $(FILE_READ_WRITE_OBJDIR)/fileClearProcess.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileOverwriteProcess.o: $(FILE_READ_WRITE_SRCDIR)/fileOverwriteProcess.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileOverwriteProcess.c -o $(FILE_READ_WRITE_OBJDIR)/fileOverwriteProcess.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileReadProcess.o: $(FILE_READ_WRITE_SRCDIR)/fileReadProcess.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileReadProcess.c -o $(FILE_READ_WRITE_OBJDIR)/fileReadProcess.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileReadWriteProcessTest.o: $(FILE_READ_WRITE_SRCDIR)/fileReadWriteProcessTest.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileReadWriteProcessTest.c -o $(FILE_READ_WRITE_OBJDIR)/fileReadWriteProcessTest.o $(CFLAGS)

$(FILE_READ_WRITE_OBJDIR)/fileStatementProcessTest.o: $(FILE_READ_WRITE_SRCDIR)/fileStatementProcessTest.c
	gcc -c $(FILE_READ_WRITE_SRCDIR)/fileStatementProcessTest.c -o $(FILE_READ_WRITE_OBJDIR)/fileStatementProcessTest.o $(CFLAGS)
####################################################################
####################################################################
####################################################################


####################################################################
################# MAKING ATTEMPT-RESTORE OBJ FILES #################
####################################################################
$(ATTEMPT_RESTORE_OBJDIR)/attemptRestoreProcess.o: $(ATTEMPT_RESTORE_SRCDIR)/attemptRestoreProcess.c
	gcc -c $(ATTEMPT_RESTORE_SRCDIR)/attemptRestoreProcess.c -o $(ATTEMPT_RESTORE_OBJDIR)/attemptRestoreProcess.o $(CFLAGS)
####################################################################
####################################################################
####################################################################


#########################################################
################# MAKING GAME OBJ FILES #################
#########################################################
${GAME_FILES_OBJDIR}/helper.o : ${GAME_FILES_SRCDIR}/helper.c
	gcc -c ${GAME_FILES_SRCDIR}/helper.c -o ${GAME_FILES_OBJDIR}/helper.o $(CFLAGS)

${GAME_FILES_OBJDIR}/random.o: ${GAME_FILES_SRCDIR}/random.c
	gcc -c ${GAME_FILES_SRCDIR}/random.c -o ${GAME_FILES_OBJDIR}/random.o $(CFLAGS)

${GAME_FILES_OBJDIR}/inventory.o: ${GAME_FILES_SRCDIR}/inventory.c
	gcc -c ${GAME_FILES_SRCDIR}/inventory.c -o ${GAME_FILES_OBJDIR}/inventory.o $(CFLAGS)

${GAME_FILES_OBJDIR}/item.o: ${GAME_FILES_SRCDIR}/item.c
	gcc -c ${GAME_FILES_SRCDIR}/item.c -o ${GAME_FILES_OBJDIR}/item.o $(CFLAGS)

${GAME_FILES_OBJDIR}/game.o: ${GAME_FILES_SRCDIR}/game.c
	gcc -c ${GAME_FILES_SRCDIR}/game.c -o ${GAME_FILES_OBJDIR}/game.o $(CFLAGS)

${GAME_FILES_OBJDIR}/actions.o: ${GAME_FILES_SRCDIR}/actions.c
	gcc -c ${GAME_FILES_SRCDIR}/actions.c -o ${GAME_FILES_OBJDIR}/actions.o $(CFLAGS)

${GAME_FILES_OBJDIR}/animals.o: ${GAME_FILES_SRCDIR}/animals.c
	gcc -c ${GAME_FILES_SRCDIR}/animals.c -o ${GAME_FILES_OBJDIR}/animals.o $(CFLAGS)
#########################################################
#########################################################
#########################################################

clean:
	rm $(OFILES)
	rm SurvivalSim.*
	