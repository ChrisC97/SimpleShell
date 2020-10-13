// ShellCommand.c
#include "ShellCommand.h"
#include <stdio.h>
#include <stdlib.h>

// Constructor
ShellCommand* new_ShellCommand()
{
  ShellCommand* pObj = NULL;
  //allocating memory
  pObj = (ShellCommand*)malloc(sizeof(ShellCommand));
  if (pObj == NULL)
  {
    return NULL;
  }
	
	//pointing to itself as we are creating base class object
	pObj->sDerivedObj = pObj;
  
	//Initializing interface for access to functions
	//destructor pointing to destrutor of itself
	pObj->Delete = Delete_ShellCommand;
	pObj->CheckInput = ShellCommand_CheckInput;
	pObj->Process = ShellCommand_Process;
	
  return pObj;
}

// Destructor
void Delete_ShellCommand(ShellCommand* const shellCmdObj){
	
}

bool ShellCommand_CheckInput(ShellCommand* commandObj, char * commandArg){
	return false;
}

void ShellCommand_Process(ShellCommand* commandObj, int argc, char ** args){
	
}
