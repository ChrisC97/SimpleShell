//PwdCmd.h

#include "ShellCommand.h"

typedef struct _PwdCmd PwdCmd;

typedef struct _PwdCmd
{
	ShellCommand* sBaseObj;
}PwdCmd;

ShellCommand* new_PwdCmd(); // Constructor
void delete_PwdCmd(ShellCommand* const sCommandShellObj); //destructor

// Check if the input given matches this command.
bool PwdCmd_CheckInput(ShellCommand* commandObj, char * commandArg);
// Prints the current working directory.
void PwdCmd_Process(ShellCommand* commandObj, int argc, char ** args);
