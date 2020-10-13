//SetEnvCmd.h

#include "ShellCommand.h"

typedef struct _SetEnvCmd SetEnvCmd;

typedef struct _SetEnvCmd
{
	ShellCommand* sBaseObj;
}SetEnvCmd;

ShellCommand* new_SetEnvCmd(); // Constructor
void delete_SetEnvCmd(ShellCommand* const sCommandShellObj); //destructor

// Check if the input given matches this command.
bool SetEnvCmd_CheckInput(ShellCommand* commandObj, char * commandArg);
// Handle the processing of this command.
void SetEnvCmd_Process(ShellCommand* commandObj, int argc, char ** args);
