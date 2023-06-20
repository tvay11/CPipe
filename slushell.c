int inputValid(char* input){
    int i=0;
    while(input[i]!='\n'){
        if(isspace(input[i])){
            return 1;
        }
        i++;
    }
    return 0;
}
void run(char input[]){
    char* cmd[16];
    cmd[0] = strtok(input, " ");
    char* first;
    first = cmd[0];

    if (strcmp(first, "cd") == 0){
        cmd[1] = strtok(NULL, "\n");
        if (chdir(cmd[1])== -1){
            perror("ERROR CHANGING DIRECTORY");
            exit(1);
        }
    } else {
        int i = 1;
        while(1){
            cmd[i] = strtok(NULL, " ");
            i++;
            if (cmd[i] == NULL) {
                break;
            }
        }
        if (execvp(first, cmd) == -1){
            perror("PROGRAM NOT FOUND");
            exit(1);
        }
    }
}
void handler(int num){
    printf("SIGNORED\n");
}

int main (int argc, char* argv[]) {

    signal(SIGINT, handler);
    char input_string[256];
    while (1){
        printf("INPUT: ");

        if(fgets(input_string, 256, stdin)==NULL){
            break;
        }

        //COUNT PIPE
        int c=0;
        int pipeCount=0;
        while((input_string[c]!='\n')){
            if(input_string[c]=='('){
                pipeCount++;
                if(input_string[c+1]=='('){
                    perror("SYNTAX ERROR");
                    exit(1);
                }
                else if(input_string[c+1]=='\n'){
                    perror("SYNTAX ERROR");
                    exit(1);
                }
            }
            c++;
        }


        strtok(input_string, "\n");

        if(pipeCount == 0){
            pid_t child = fork();
            if (child == 0) {
                run(input_string);
            }
            waitpid(child, NULL, 0);
        } else {
            // PIPELINE
            int pipefd[2];
            int writeSide;
            int readSide;
            int oldfd;
            int pChild;
            char* parse_string[15];
            parse_string[0] = strtok(input_string, "(\n");
            int p = 0;
            while(parse_string[p] != NULL) {
                p++;
                parse_string[p] = strtok(NULL, "(\n");

            }

            parse_string[p+1]=NULL;
            p=p-1;
            while ((p) >= 0){
                if (inputValid(parse_string[p])== 0){
                    perror("INVALID NULL COMMAND");
                    exit(1);
                }

                if((p) == pipeCount){
                    int pipeCount = pipe(pipefd);
                    if (pipeCount == -1){
                        perror("PIPE ERROR");
                        exit(1);
                    }
                    writeSide = pipefd[1];
                    oldfd = pipefd[0];
                }
                if((p)>0){
                    int pipeCount = pipe(pipefd);
                    if (pipeCount == -1){
                        perror("PIPE ERROR");
                        exit(1);
                    }
                    writeSide = pipefd[1];
                    readSide = oldfd;
                    oldfd = pipefd[0];
                }
                pChild = fork();
                if (pChild == 0){
                    if ((p) == pipeCount){
                        int ret1 = dup2( writeSide, STDOUT_FILENO );
                        if( ret1 == -1 ){
                            perror("DUP2 CAN'T READ PIPE");
                            exit(-1);
                        }
                        close(readSide);
                    } else if ((p) == 0) {
                        if( dup2( oldfd, STDIN_FILENO )== -1 ){
                            perror("DUP2 CAN'T READ PIPE2");
                            exit(-1);
                        }
                    }  else {

                        if( dup2( writeSide, STDOUT_FILENO )== -1 ){
                            perror("DUP2 CAN'T READ PIPE");
                            exit(-1);
                        }
                        if( dup2( readSide, STDIN_FILENO ) == -1 ){
                            perror(" DUP2 CAN'T READ PIPE2");
                            exit(-1);
                        }
                        close(readSide);
                    }
                    run(parse_string[p]);
                }
                // PARENT
                if ((p) == pipeCount){
                    close(writeSide);
                } else if ((p) == 0) {
                    close(oldfd);
                } else {
                    close(readSide);
                    close(writeSide);
                }
                p--;
                waitpid(pChild, NULL, 0);
            }
        }
    }
    return 0;
}
