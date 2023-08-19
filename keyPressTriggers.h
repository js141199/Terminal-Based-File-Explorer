
/********** GET PARENT DIRCT PATH OF CURRENT DIRCT ******/
void getParentDirctPath(){

    string filePath = CURRENT_PATH;
    CURRENT_PATH = CURRENT_PATH.substr(0,CURRENT_PATH.find_last_of('/'));
    if(CURRENT_PATH == "")
        CURRENT_PATH = "/";
        
}

/********** PUSH THE PREVIOUSLY VISITED PATH ******/
void backwardStPush(string dirPath){ backwardSt.push(dirPath); }

/********** POP THE PREVIOUSLY VISITED PATH AND RETURN IT ******/
string backwardStPop(){ 
    
    string path = backwardSt.top();
    backwardSt.pop(); 
    return path;

}

/********** FLUSH THE BACKWARD STACK ******/
void backwardStClear(){ while(!backwardSt.empty()) { backwardSt.pop(); } }

/********** PUSH THE CHILD DIR PATH  ******/
void forwardStPush(string dirPath){ forwardSt.push(dirPath); }

/********** POP THE CHILD  PATH AND RETURN IT ******/
string forwardStPop(){ 
    
    string dirPath = forwardSt.top();
    forwardSt.pop(); 
    return dirPath;
}

/********** FLUSH THE FORWARD STACK ******/
void forwardStClear(){ while(!forwardSt.empty()) { forwardSt.pop(); } }

/********** IMPLEMENT WHEN ENTER KEY IS PRESSED ******/
void implementEnterKey(){

    string dirName = dirDetails[CUR_POS][dirDetails[0].size()-2];
    string dirPath = dirDetails[CUR_POS][dirDetails[0].size()-1];
    
    if(dirName == ".."){
        implementBackSpaceKey();
    }

    else if(dirName != "."){

        struct stat dirInfo;

        DIR* dir  = opendir(dirPath.c_str());

        if(errno == EACCES){
            cout << endl << "You don't have read permissions..." << endl;
            return ;
        }

        if(stat((dirPath).c_str(),&dirInfo) != 0){
            cout << "Unable to find information for specified path => " << dirPath << endl;
            return ;
        }

        // check it is a directory
        if(S_ISDIR(dirInfo.st_mode)){

            // make forward stack empty
            forwardStClear();

            backwardStPush(CURRENT_PATH);
            CURRENT_PATH = dirPath;
            listAllDirectories(CURRENT_PATH);

        }else{

            // open file in default user editor
            pid_t pid = fork();
            if(pid == 0){
                execl("/usr/bin/xdg-open","xdg-open",dirPath.c_str(),NULL);
                exit(1);
            }
            
        }

    }

}

/******** IMPLEMENT WHEN BACKSPACE KEY IS PRESSED ****/
void implementBackSpaceKey(){

    // empty the forward stack
    forwardStClear();

    if(backwardSt.empty()){
        if(CURRENT_PATH != "/"){
            // push current path to back stack 
            backwardStPush(CURRENT_PATH);

            getParentDirctPath();
            listAllDirectories(CURRENT_PATH);
        }
    }

    else{   

        if(CURRENT_PATH != "/"){
            // push current path to back stack 
            backwardStPush(CURRENT_PATH);

            getParentDirctPath();
            listAllDirectories(CURRENT_PATH);
        }
    }

}

/******** IMPLEMENT WHEN LEFTARROW KEY IS PRESSED ****/
void implementLeftArrowKey(){

    if(!backwardSt.empty()){

        // store current path to forward stack
        forwardStPush(CURRENT_PATH);

        // remove path from backward stack
        string dirPath = backwardStPop();

        CURRENT_PATH = dirPath;

        listAllDirectories(CURRENT_PATH);
    }

}

/******* IMPLEMENT WHEN RIGHTARROW KEY IS PRESSED ****/
void implementRightArrowKey(){

    if(!forwardSt.empty()){

        // store current path to back stack
        backwardStPush(CURRENT_PATH);
        string dirPath = forwardStPop();
        CURRENT_PATH = dirPath;
        listAllDirectories(CURRENT_PATH);
    }

}

/********* IMPLEMENT WHEN 'h' KEY IS PRESSED *********/
void implementHKey(){

    string homePath = getHome();

    if(CURRENT_PATH != homePath){
        
        // push current path to back stack
        backwardStPush(CURRENT_PATH);
        
        // flush put forward stack
        forwardStClear();
        CURRENT_PATH = homePath;
        listAllDirectories(CURRENT_PATH);
    }
}