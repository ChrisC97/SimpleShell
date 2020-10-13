// EchoCmd.c
#include "EchoCmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

ShellCommand* new_EchoCmd(){
	EchoCmd* pEmpObj;
	//calling base class construtor
	ShellCommand* sObj = new_ShellCommand();
	//allocating memory
	pEmpObj = malloc(sizeof(EchoCmd));
	if (pEmpObj == NULL)
	{
		sObj->Delete(sObj);
		return NULL;
	}
	sObj->sDerivedObj = pEmpObj; //pointing to derived object
	
	//Changing base class interface to access derived class functions
	//virtual destructor
	//person destructor pointing to destrutor of employee
	sObj->Delete = delete_EchoCmd;
	sObj->Process = EchoCmd_Process;
	sObj->CheckInput = EchoCmd_CheckInput;
	return sObj;
}

void delete_EchoCmd(ShellCommand* const sCommandShellObj){
	
}

bool EchoCmd_CheckInput(ShellCommand* commandObj, char * commandArg){
	if(strncmp(commandArg, "echo", 4) == 0){
		return true;
	}
	return false;
}

void EchoCmd_Process(ShellCommand* commandObj, int argc, char ** args){
  int i = 1;
  for(i = 1; i < argc; i++){
    if(args[i] == NULL || args[i] == '\0'){
      break;
    }
    printf("%s ", args[i]);
  }
  printf("\n");
}
