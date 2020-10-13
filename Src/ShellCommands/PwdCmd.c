// PwdCmd.c
#include "PwdCmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

ShellCommand* new_PwdCmd(){
	PwdCmd* pEmpObj;
	//calling base class construtor
	ShellCommand* sObj = new_ShellCommand();
	//allocating memory
	pEmpObj = malloc(sizeof(PwdCmd));
	if (pEmpObj == NULL)
	{
		sObj->Delete(sObj);
		return NULL;
	}
	sObj->sDerivedObj = pEmpObj; //pointing to derived object
	
	//Changing base class interface to access derived class functions
	//virtual destructor
	//person destructor pointing to destrutor of employee
	sObj->Delete = delete_PwdCmd;
	sObj->Process = PwdCmd_Process;
	sObj->CheckInput = PwdCmd_CheckInput;
	return sObj;
}

void delete_PwdCmd(ShellCommand* const sCommandShellObj){
	
}

bool PwdCmd_CheckInput(ShellCommand* commandObj, char * commandArg){
	if(strncmp(commandArg, "pwd", 3) == 0){
		return true;
	}
	return false;
}

void PwdCmd_Process(ShellCommand* commandObj, int argc, char ** args){
  char * cwd = getcwd(NULL, 0);
  printf("%s\n", cwd);
}
