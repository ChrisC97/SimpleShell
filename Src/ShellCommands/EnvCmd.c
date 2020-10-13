// EnvCmd.c
#include "EnvCmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

ShellCommand* new_EnvCmd(){
	EnvCmd* pEmpObj;
	//calling base class construtor
	ShellCommand* sObj = new_ShellCommand();
	//allocating memory
	pEmpObj = malloc(sizeof(EnvCmd));
	if (pEmpObj == NULL)
	{
		sObj->Delete(sObj);
		return NULL;
	}
	sObj->sDerivedObj = pEmpObj; //pointing to derived object
	
	//Changing base class interface to access derived class functions
	//virtual destructor
	//person destructor pointing to destrutor of employee
	sObj->Delete = delete_EnvCmd;
	sObj->Process = EnvCmd_Process;
	sObj->CheckInput = EnvCmd_CheckInput;
	return sObj;
}

void delete_EnvCmd(ShellCommand* const sCommandShellObj){
	
}

bool EnvCmd_CheckInput(ShellCommand* commandObj, char * commandArg){
	if(strncmp(commandArg, "env", 3) == 0){
		return true;
	}
	return false;
}

void EnvCmd_Process(ShellCommand* commandObj, int argc, char ** args){
  int i = 0;
  while(environ[i]) {
    printf("%s\n", environ[i]); // prints in form of "variable=value"
    i++;
  }
}
