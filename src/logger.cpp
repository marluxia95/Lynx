#include <stdio.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <cstring>

#include "logger.h"


static char* COL_NORM    = "\x1B[0m";   // Normal
static char* COL_FATAL   = "\x1B[31m";  // Red
static char* COL_ERROR   = "\x1B[91m";  // Light Red
static char* COL_INFO    = "\x1B[37m";  // White
static char* COL_WARN    = "\x1B[33m";  // Yellow
static char* COL_DEBUG   = "\x1B[94m";  // Light Blue
static char* COL_VERBOSE = "\x1B[36m";  // Cyan
static char* COL_LOGGER  = "\x1B[90m";  // Dark Grey
static char* COL_TRACE   = "\x1B[95m";  // Light Magenta

Logger::Logger(const char* logFile, LogLevel level, bool isSilent) {
	filePath = logFile;
	Level = level;
	silent = isSilent;
}

void Logger::log(LogLevel loglvl, const char* str) {


	//int logFile = open(filePath, O_CREAT | O_APPEND | O_RDWR, 0664 );

	char* date = getDateString();

	int stringSize = strlen( date ) + strlen(str) + 10;

	int msgSize = stringSize + 50;  // 60 char buffer to prevent overflow

	char* msg = (char*)malloc( msgSize + 8); // 8 for colors

	char *outColor = COL_NORM; // Default : Normal

	if(loglvl < LOG_INFO){
		if(loglvl == LOG_FATAL){
			outColor = COL_FATAL;
            sprintf( msg, "%s\tFATAL   : ", date); 
		}else if(loglvl == LOG_ERROR){
			outColor = COL_ERROR;
            sprintf( msg, "%s\tERROR   : ", date); 
		}

		sprintf( msg + strlen( msg ), "%s\n", str); // Write args msg
#ifdef __linux__
		printf("%s%s%s", outColor ,msg, COL_NORM);
#else
		printf("%s", msg);
#endif
		//write( logFile , msg, strlen( msg ) );
	}else{
		if(loglvl == LOG_INFO){
			outColor = COL_INFO;
			sprintf( msg, "%s\tINFO    : ", date ); 
		}else if(loglvl == LOG_WARN){
			outColor = COL_WARN;
			sprintf( msg, "%s\tWARNING : ", date ); 
		}else if(loglvl == LOG_DEBUG){
			outColor = COL_DEBUG;
			sprintf( msg, "%s\tDEBUG   : ", date ); 
		}
		sprintf( msg + strlen( msg ), "%s\n", str ); // Write args msg
#ifdef __linux__
		printf("%s%s%s", outColor ,msg, COL_NORM);
#else
		printf("%s", msg);
#endif
		//write( logFile , msg, strlen( msg ) );
	}


	//close(logFile);

	free(date);
	free(msg);
}

char* Logger::getDateString() {
    // Initialize and get current time
    time_t t = time( NULL );

    // Allocate space for date string
    char* date = (char*)malloc( 100 );

    // Format the time correctly
    strftime(date, 100, "[%F %T]", localtime(&t));

    return date;
}