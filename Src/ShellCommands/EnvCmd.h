//EnvCmd.h

#include "ShellCommand.h"

typedef struct _EnvCmd EnvCmd;

typedef struct _EnvCmd
{
	ShellCommand* sBaseObj;
}EnvCmd;

ShellCommand* new_EnvCmd(); // Constructor
void delete_EnvCmd(ShellCommand* const sCommandShellObj); //destructor

// Check if the input given matches this command.
bool EnvCmd_CheckInput(ShellCommand* commandObj, char * commandArg);
// Handle the processing of this command.
void EnvCmd_Process(ShellCommand* commandObj, int argc, char ** args);
