#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "ShellCommand.h"

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128
#define SEPARATORS " \t\n"

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
char * cwd;
extern char **environ;

int commandArraySize = 5;
pid_t childPID = -100;

int saved_stdout;
int saved_stdin;
int saved_stderr;

void ClearArgs(char ** args);
void SetCommands(ShellCommand** cmdArray);
void ProcessCommands(ShellCommand** cmdArray, int argc, char ** args);
void ForkExternalProcess(bool isBackground, char * outputFile,
                         int argc, char ** args);
bool CheckIfBackgroundProcess(int argCount, char ** args);
char* CheckForOutputRedirection(int argCount, char ** args);
int RedirectOutput(bool isBackground, char* outputFile);
void Cleanup();

void HandleExitCall(int signum);
void KillLongRunningChild(int signum);

// Handles the exit call (ctrl+c) while a child is running
// in the foreground. 
void HandleExitCall(int signum){
  // Check if a child process is running.
  if(childPID > 0){
    // Kill it.
    kill(childPID, SIGINT);
  }else{
    // No child was running, end the terminal.
    exit(0);
  }
}

// Handles killing a child process if it's ran for at least 10 seconds.
void KillLongRunningChild(int signum){
  if(childPID > 0){
    kill(childPID, SIGINT);
  }
}

int main() {
  // Save file descriptors.
  saved_stdin = dup(0);
  saved_stdout = dup(1);
  saved_stderr = dup(2);
  
  // Fill the command array.
  ShellCommand *commandArray[commandArraySize];
	SetCommands(commandArray);
  
  // Stores the string typed into the command line.
  char command_line[MAX_COMMAND_LINE_LEN];
  char cmd_bak[MAX_COMMAND_LINE_LEN];

  // Stores the tokenized command line input.
  char *arguments[MAX_COMMAND_LINE_ARGS];
  
  while (true) {
    // Check if a child has terminated, but don't block.
    if(childPID > 0){
      int* status;
      waitpid(-1, status, WNOHANG);
    }
    
    int argCount = 0;
		// Prompt the user with "cwd>".
		do{
      // Make all the arguments NULL.
      ClearArgs(arguments);
      argCount = 0;
      
      // Get the current working directory.
      cwd = getcwd(NULL, 0);
			// Print the shell prompt.
			printf("%s%s", cwd, prompt);
			fflush(stdout);

			// Read input from stdin and store it in command_line. If there's an
			// error, exit immediately. (If you want to learn more about this line,
			// you can Google "man fgets")
			if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
					fprintf(stderr, "fgets error");
					exit(0);
			}

			// Tokenize command line args.
			// https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
			char * splitArg = strtok(command_line, SEPARATORS);
      while (splitArg != NULL && argCount != MAX_COMMAND_LINE_ARGS) { 
        // By default, just put the string in our args.
        arguments[argCount] = splitArg;
        // Check if the string could be a env variable.
        if (splitArg != NULL
          && splitArg[0] == '$') {
          // Try getting the env variable and putting it in our args.
          char * ecArg = (splitArg + 1);
          char * convertedArg;
          if(((convertedArg = getenv(ecArg)) != NULL)){
            arguments[argCount] = convertedArg;
          }
        }
        // Get the next argument.
				splitArg = strtok(NULL, SEPARATORS); 
        argCount++;
			}

      // Process the given command if args are given.
      if(argCount > 0){
        ProcessCommands(commandArray, argCount, arguments);
      }
		}while(command_line[0] == 0x0A);  // while just ENTER pressed


		// If the user input was EOF (ctrl+d) or "exit", exit the shell.
		if (feof(stdin) || strncmp(arguments[0], "exit", 4) == 0) {
				printf("\n");
				fflush(stdout);
				fflush(stderr);
				return 0;
		}
  }
	// This should never be reached.
	return -1;
}

// Set all arguments to NULL. This is done everytime the user presses enter.
void ClearArgs(char ** args){
  int i = 0;
  for(i = 0; i < MAX_COMMAND_LINE_ARGS; i++){
    args[i] = '\0';
  }
}

// Fill the command array with our commands.
void SetCommands(ShellCommand** cmdArray){
	cmdArray[0] = new_ChangeDirectory();
  cmdArray[1] = new_EchoCmd();
  cmdArray[2] = new_PwdCmd();
  cmdArray[3] = new_EnvCmd();
  cmdArray[4] = new_SetEnvCmd();
}

// Check if the process should run in the background.
bool CheckIfBackgroundProcess(int argCount, char ** args){
  bool isBackground = true;
  int j = 0;
  // Not enough arguments to bother checking.
  if(argCount <= 1){
    return false;
  }
  // Check if the process should run in the background.
  if(strncmp(args[argCount-1], "&", 1) != 0){
    isBackground = false;
    for(j = argCount-1; j >= 0; j--){
      if(args[j] == NULL 
        && strncmp(args[j-1], "&", 1) == 0){
        // Remove the '&' symbol.
        args[j-1] = '\0';
        isBackground = true;
        break;
      }
    }
  }else{
    // Last argument was an '&'.
    args[argCount-1] = '\0';
    isBackground = true;
  }
  return isBackground;
}

// Check if the output of the shell should be redirected somewhere else.
char* CheckForOutputRedirection(int argCount, char ** args){
  char* outputFile;
  outputFile = (char *)malloc(MAX_COMMAND_LINE_LEN*1);
  if(argCount <= 2){
    return NULL;
  }
  int a = 0;
  for(a = argCount; a >= 0; a--){
    // Output file would have to be the last argument.
    if(args[a] != NULL){
      // Argument before it should be '>'.
      if(strncmp(args[a-1], ">", 1) == 0){
        strcpy(outputFile, args[a]);
        args[a] = '\0';
        args[a-1] = '\0';
        return outputFile;
      }
      return NULL;
    }
  }
  return NULL;
}

// Handles redirecting input. We have to do this if the user
// specifies an output file, if the process runs in the background,
// or both.
int RedirectOutput(bool isBackground, char* outputFile){
  // If it's a bg process, don't print to console.
  // Need to printf before redirection for some reason,
  // as otherwise the console breaks.
  if(isBackground == true){
    printf("(BG) Output redirected.");
    // By default we want to output background programs
    // to /dev/null so they don't use the terminal.
    if(outputFile == NULL){
      outputFile = "/dev/null";
    }
    // Redirect stdout/in/err.
    int devNull = open(outputFile, O_WRONLY | O_CREAT);
    dup2(devNull, STDIN_FILENO);
    dup2(devNull, STDOUT_FILENO);
    dup2(devNull, STDERR_FILENO);
    return devNull;
  }else if(outputFile != NULL){
    printf("Output redirected.");
    // Redirect stdout/in/err.
    int devNull = open(outputFile, O_WRONLY | O_CREAT);
    dup2(devNull, STDIN_FILENO);
    dup2(devNull, STDOUT_FILENO);
    dup2(devNull, STDERR_FILENO);
    return devNull;
  }
  return -1;
}

// Check if we have a command handler for what the user's entered.
// If we don't, then we'll try using an external program.
void ProcessCommands(ShellCommand** cmdArray, int argCount, char ** args){
  // No command matched, try forking a process.
  bool isBackground = CheckIfBackgroundProcess(argCount, args);
  // Check if process should redirect output (>) to a file.
  char* outputFile = CheckForOutputRedirection(argCount, args);
  // Check for first command that matches our conditions.
  int i = 0;
	for(i = 0; i < commandArraySize; i++){
		if(cmdArray[i]->CheckInput(cmdArray[i], args[0])){
      // Redirect output if needed.
      int openFile = RedirectOutput(isBackground, outputFile);
      // Carry out command.
      cmdArray[i]->Process(cmdArray[i], argCount, args);
      // Close output file if we opened one.
      if(openFile != -1){
        close(openFile);
      }
      Cleanup();
			return;
		}
	}
  // No matching process in our program, try finding it outside.
  ForkExternalProcess(isBackground, outputFile, argCount, args);
}

// Try running an external process.
void ForkExternalProcess(bool isBackground, char* outputFile,
                         int argc, char ** args){
  
  childPID = fork();
  // Forking error.
  if(childPID == -1){
    Cleanup();
    printf("Error while forking child process.\n");
    return;
  }else if (childPID == 0){
    // CHILD PROCESS //
    int openFile = RedirectOutput(isBackground, outputFile);
    // Try to execute program.
    if(execvp(args[0], args) == -1){
      perror("execvp() failed: ");
      // Error occured, exit the program.
      close(openFile);
      exit(1);
    }
    close(openFile);
  } else{
    // PARENT PROCESS //
    // Child isn't a background process, so we have to wait on it.
    if(isBackground == false){
      // Handle the exit signal call outselves while the child runs.
      signal(SIGINT,HandleExitCall);
      // IN 10 seconds, kill off the child if it's still running.
      signal(SIGALRM, KillLongRunningChild);
      alarm(10); //Schedule a SIGALRM for 10 seconds.
      // Wait for child.
      pid_t cProc = wait(NULL);
      // No child anymore.
      childPID = -100;
    }
    Cleanup();
  }
}

// Cleans up anything that we changed while 
// running a command.
void Cleanup(){
  // Return to using standard signal for ctrl-c.
  signal(SIGINT, SIG_DFL);
  // Cancel the alarm and return the standard signal.
  signal(SIGALRM, SIG_DFL);
  alarm(0);
  // Restore proper file descriptors.
  dup2(saved_stdin, 0);
  dup2(saved_stdout, 1);
  dup2(saved_stderr, 2);
  //close(saved_stdin);
  //close(saved_stdout);
  //close(saved_stderr);
  // Clear buffers.
  fflush(stdout);
  fflush(stdin);
  fflush(stderr);
}
