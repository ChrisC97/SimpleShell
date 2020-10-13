// SetEnvCmd.c
#include "SetEnvCmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

ShellCommand* new_SetEnvCmd(){
	SetEnvCmd* pEmpObj;
	//calling base class construtor
	ShellCommand* sObj = new_ShellCommand();
	//allocating memory
	pEmpObj = malloc(sizeof(SetEnvCmd));
	if (pEmpObj == NULL)
	{
		sObj->Delete(sObj);
		return NULL;
	}
	sObj->sDerivedObj = pEmpObj; //pointing to derived object
	
	//Changing base class interface to access derived class functions
	//virtual destructor
	//person destructor pointing to destrutor of employee
	sObj->Delete = delete_SetEnvCmd;
	sObj->Process = SetEnvCmd_Process;
	sObj->CheckInput = SetEnvCmd_CheckInput;
	return sObj;
}

void delete_SetEnvCmd(ShellCommand* const sCommandShellObj){
	
}

bool SetEnvCmd_CheckInput(ShellCommand* commandObj, char * commandArg){
	if(strncmp(commandArg, "setenv", 6) == 0){
		return true;
	}
	return false;
}

void SetEnvCmd_Process(ShellCommand* commandObj, int argc, char ** args){
  if(argc == 3){
    setenv(args[1], args[2], 1);
  }else{
    printf("setenv needs two (2) arguments.");
  }
}
