//EchoCmd.h

#include "ShellCommand.h"

typedef struct _EchoCmd EchoCmd;

typedef struct _EchoCmd
{
	ShellCommand* sBaseObj;
}EchoCmd;

ShellCommand* new_EchoCmd(); // Constructor
void delete_EchoCmd(ShellCommand* const sCommandShellObj); //destructor

// Check if the input given matches this command.
bool EchoCmd_CheckInput(ShellCommand* commandObj, char * commandArg);
// Handle the processing of this command.
void EchoCmd_Process(ShellCommand* commandObj, int argc, char ** args);
