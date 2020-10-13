// ShellCommand.h
// https://www.codeproject.com/Articles/108830/Inheritance-and-Polymorphism-in-C
#include <stdbool.h>

typedef struct _ShellCommand ShellCommand;

// Function Pointers
typedef bool    (*fptrCheckInput)(ShellCommand *, char * commandArg);
typedef void    (*fptrProcess)(ShellCommand *, int argc, char ** args);
typedef void    (*fptrDelete)(ShellCommand *);

typedef struct _ShellCommand{
	void* sDerivedObj;
  //interface for function
	fptrCheckInput CheckInput;
	fptrProcess Process;
  fptrDelete Delete;
}ShellCommand;

ShellCommand* new_ShellCommand(); // Constructor
void Delete_ShellCommand(ShellCommand* const shellCmdObj); // Destructor

// Check if the input given matches this command.
bool ShellCommand_CheckInput(ShellCommand* commandObj, char * commandArg);
// Handle the processing of this command.
void ShellCommand_Process(ShellCommand* commandObj, int argc, char ** args);
