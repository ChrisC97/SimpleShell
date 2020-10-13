//ChangeDirectory.h

#include "ShellCommand.h"

typedef struct _ChangeDirectory ChangeDirectory;

typedef struct _ChangeDirectory
{
	ShellCommand* sBaseObj;
}ChangeDirectory;

ShellCommand* new_ChangeDirectory(); // Constructor
void delete_ChangeDirectory(ShellCommand* const sCommandShellObj); //destructor

// Check if the input given matches this command.
bool ChangeDirectory_CheckInput(ShellCommand* commandObj, char * commandArg);
// Handle the processing of this command.
void ChangeDirectory_Process(ShellCommand* commandObj, int argc, char ** args);
