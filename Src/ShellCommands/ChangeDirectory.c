// ChangeDirectory.c
#include "ChangeDirectory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

ShellCommand* new_ChangeDirectory(){
	ChangeDirectory* pEmpObj;
	//calling base class construtor
	ShellCommand* sObj = new_ShellCommand();
	//allocating memory
	pEmpObj = malloc(sizeof(ChangeDirectory));
	if (pEmpObj == NULL)
	{
		sObj->Delete(sObj);
		return NULL;
	}
	sObj->sDerivedObj = pEmpObj; //pointing to derived object
	
	//Changing base class interface to access derived class functions
	//virtual destructor
	//person destructor pointing to destrutor of employee
	sObj->Delete = delete_ChangeDirectory;
	sObj->Process = ChangeDirectory_Process;
	sObj->CheckInput = ChangeDirectory_CheckInput;
	return sObj;
}

void delete_ChangeDirectory(ShellCommand* const sCommandShellObj){
	
}

bool ChangeDirectory_CheckInput(ShellCommand* commandObj, char * commandArg){
	if(strncmp(commandArg, "cd", 2) == 0){
		return true;
	}
	return false;
}

void ChangeDirectory_Process(ShellCommand* commandObj, int argc, char ** args){
  // Given argument is NULL, assume they want the default.
  if(args[1] == NULL){
    if ((args[1] = getenv("HOME")) == NULL) {
      args[1] = "/";
    } 
  }
  // Directory change failed, print the given error.
  if(chdir(args[1]) == -1){
    char* errString;
    errString = malloc(strlen(args[1])+1+4);
    strcpy(errString, "cd: ");
    strcat(errString, args[1]);
    // Print error.
    perror(errString);
    // Free up string memory.
    free(errString);
  };
}
