
/******** GET THE USER NAME OF CURRENTLY LOGGED IN USER ********/
string getCurrentUser(){
      
    // get current logged in user_name
    char *currentUser;
    currentUser=(char *)malloc(10*sizeof(currentUser));
    currentUser=getlogin();

    return string(currentUser);
}

/******** GET THE PATH OF HOME/USER DIRCT OF LOGGED IN USER *********/
string getHome(){
    
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL){
        homedir = getpwuid(getuid())->pw_dir;    
    }

    string homePath = homedir;

    return homePath;
}

/****** FLUSH OUT THE SCREEN FOR PRINTING THE NEW CONTENT ******/
void clearScreen(){
    // for flushing/clearing the screen
    cout << "\33c";
}

/*** RESET THE START AND END BEFORE PRINTING THE NEW CONTENT ***/
void reSetCurPointers(){

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    WIN_H = (int)(0.65*size.ws_row) - 1;
    WIN_W = size.ws_col;
    
    PAGE_START = 0;
    PAGE_END = min(WIN_H,TOTAL_ROWS) - 1;
    CUR_POS = 0;

}

/*** THESE FUNCTION IS TRIGGERED WHEN THE TERMINAL IS RESIZED ***/
void resetWindowSize(int sig){

    reSetCurPointers();
    printAllDirDetails(PAGE_START,PAGE_END);

}

/** SORT THE DETAILS ACCORDING TO THE FILENAME BEFORE PRINTING **/
bool sortByFileName(const vector<string>& file1,const vector<string>& file2){

    return file1[file1.size()-1] < file2[file2.size()-1] ; 

}

/************* CONVERT THE FILE/DIRCT SIZE TO BYTES *************/
string convertIntToBytes(long long fileSize){

    long long kb = 1000;
    long long mb = kb*kb;
    long long gb = mb*kb;
    long long tb = gb*kb;

    string size = "";

    // check for Tera bytes
    if(fileSize >= tb){
        size = to_string(fileSize/tb) + "TB";
    }
    else if(fileSize >= gb){
        size = to_string(fileSize/gb) + "GB";
    }
    else if(fileSize >= mb){
        size = to_string(fileSize/mb) + "MB";
    }else if(fileSize >= kb){
        size = to_string(fileSize/kb) + "kB";
    }else{
        size = to_string(fileSize) + "B";
    }

    return size;

}

/************* LIST ALL THE DETAILS OF FILES/DIRCT  *************/
void printAllDirDetails(int startIndex,int endIndex){

    clearScreen();
    for(int i = startIndex;i<=endIndex;i++){
        if(i == CUR_POS)
            cout << "\033[1;107;30m" << left << setw(5) << setfill(' ') << "=>";
        else
            cout << left << setw(5) << setfill(' ') << "";
        cout << left << setw(5) << setfill(' ') << to_string(i+1);
        for(int j = 1;j < dirDetails[i].size()-1;j++){
            // print file permissions
            if(j == 1 && WIN_W > 70)
                cout << left << setw(10) << setfill(' ') << dirDetails[i][j];
            // print user id &&  print group id
            else if((j == 2 || j == 3) && WIN_W > 115)
                cout << right << setw(20) << setfill(' ') << dirDetails[i][j];
            // print size
            else if(j == 4 && WIN_W > 115)
                cout << right << setw(10) << setfill(' ') << dirDetails[i][j];
            // print last modified date and time
            else if(j == 5 && WIN_W > 80)
                cout << right << setw(20) << setfill(' ') << dirDetails[i][j];
            // print file name
            else if(j == 6)
                cout << left << setw(4) << setfill(' ') << "" << dirDetails[i][j];
            
        }
        cout << "\033[0m\t\t";
        cout << endl;
    }

    
    cout << endl << endl << endl << "Current path => " << CURRENT_PATH << endl << endl;
    if (NORMAL_MODE){
        cout << endl << "-------------------------  NORMAL MODE --------------------------" << endl;
    }else{
        cout << "-------------------------  COMMAND MODE --------------------------" << endl;
        string cmd = "Enter command: $ ";
        write(STDOUT_FILENO,cmd.c_str(),cmd.size());
    }
    

}

/************* FETCH THE FILE/DIRCT INFORMATION  *************/
void fetchFileInformation(string fileName){

    vector<string> info;

    // adding cursor
    info.push_back("");

    string filePath = CURRENT_PATH == "/" ? CURRENT_PATH + fileName : CURRENT_PATH + "/" + fileName;

    time_t s_time;
    char mtime[50];
    struct stat fileInfo;
    struct tm local_time;

    //int stat(const char *path, struct stat *buf);
    // these line is compulsory to assign output obj to fileInfo
    // c_str() is used to convert string to const char*
    if(stat((filePath).c_str(),&fileInfo) != 0){
        cout << "Unable to find information for specified path" << endl;
    }
    
    string perms = "";
    perms += (S_ISDIR(fileInfo.st_mode) ? "d" : "-");  // check for is it a directory
    perms += (fileInfo.st_mode & S_IRUSR ? "r" : "-"); // check read permission for user 
    perms += (fileInfo.st_mode & S_IWUSR ? "w" : "-"); // check write permission for user
    perms += (fileInfo.st_mode & S_IXUSR ? "x" : "-"); // check executable permission for user
    perms += (fileInfo.st_mode & S_IRGRP ? "r" : "-"); // check read permission for group
    perms += (fileInfo.st_mode & S_IWGRP ? "w" : "-"); // check write permission for group
    perms += (fileInfo.st_mode & S_IXGRP ? "x" : "-"); // check executable permission for group
    perms += (fileInfo.st_mode & S_IROTH ? "r" : "-"); // check read permission for other
    perms += (fileInfo.st_mode & S_IWOTH ? "w" : "-"); // check read permission for other
    perms += (fileInfo.st_mode & S_IXOTH ? "x" : "-"); // check read permission for other

    info.push_back(perms);
    
    struct group *grp;
    struct passwd *pwd;

    string uid,pid;

    pwd = getpwuid(fileInfo.st_uid);

    if(pwd != NULL)
        pid = pwd->pw_name;
    else
        pid = to_string(fileInfo.st_uid);

    grp = getgrgid(fileInfo.st_gid);
    
    if(grp != NULL)
        uid = grp->gr_name;
    else
        uid = fileInfo.st_gid;

    info.push_back(pid);
    info.push_back(uid);

    string fileSize = convertIntToBytes((int) fileInfo.st_size);

    info.push_back(fileSize);

    s_time = fileInfo.st_mtime;
    localtime_r(&s_time, &local_time);
    strftime(mtime, sizeof mtime, "%b %d %H:%M", &local_time);

    info.push_back(mtime);

    info.push_back(fileName);

    info.push_back(filePath);

    dirDetails.push_back(info);

}

void listAllDirectories(string dircPath){

    dirDetails.clear();

    struct dirent *entry;
    
    // char cwdAbsPath[250];
    // getcwd(cwdAbsPath,sizeof(cwdAbsPath));

    DIR *dir = opendir(dircPath.c_str());
   
    if (dir == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        fetchFileInformation(entry->d_name);
    }

    closedir(dir);

    TOTAL_ROWS = dirDetails.size();

    sort(dirDetails.begin(),dirDetails.end(),sortByFileName);

    reSetCurPointers();
    printAllDirDetails(PAGE_START,PAGE_END);

}
 
/************* FETCH THE INPUT OF USER  *************/
char readKey(){

    char key;
    read(STDIN_FILENO, &key, 1);

    if(key == '\x1b'){
        char seq[3];
        if( read(STDIN_FILENO, &seq[0], 1) != 1 ) return '-';
        if(seq[0] == KEY_COLON){
            return KEY_COLON;
        }
        if( read(STDIN_FILENO, &seq[1], 1) != 1 ) return '-';
        if(seq[1] == KEY_COLON){
            return KEY_COLON;
        }
        if(seq[0] == '['){
            switch (seq[1]){
                case KEY_UP :  return KEY_UP ;
                case KEY_DOWN :  return KEY_DOWN ;
                case KEY_LEFT : return KEY_LEFT ;
                case KEY_RIGHT : return KEY_RIGHT ;
            }
        }
    }
    
    return key;
}

void moveCursorUp(){ 

    if(CUR_POS != 0){
        if(CUR_POS == PAGE_START){
            CUR_POS--;
            PAGE_START = CUR_POS;
            PAGE_END--;
            printAllDirDetails(CUR_POS,PAGE_END);
        }else{
            CUR_POS--;
            printAllDirDetails(PAGE_START,PAGE_END);
        }
    }

 }

void moveCursorDown(){ 

    if(CUR_POS < TOTAL_ROWS-1){
        if(CUR_POS == PAGE_END){
            CUR_POS++;
            PAGE_END = CUR_POS;
            PAGE_START++;
            printAllDirDetails(PAGE_START,CUR_POS);
        }else{
            CUR_POS++;
            printAllDirDetails(PAGE_START,PAGE_END);
        }
    }

 }

void switchToNormalMode(){

    NORMAL_MODE = true;

    // start application with /home/user
    CURRENT_PATH = getHome();

    reSetCurPointers();
    listAllDirectories(CURRENT_PATH);    

    // implementation of key-press input.
    char key;

    while(1){   
            
        key = readKey();
        
        switch(key){

            case KEY_UP :  moveCursorUp(); break;
            
            case KEY_DOWN :  moveCursorDown(); break;
            
            case KEY_LEFT :  implementLeftArrowKey(); break;
            
            case KEY_RIGHT :  implementRightArrowKey(); break;

            case KEY_ESCAPE: break;

            case KEY_BACKSPACE : implementBackSpaceKey();  break;

            case KEY_COLON : 
            
            startCommandMode();
            
            NORMAL_MODE = true;
            
            listAllDirectories(CURRENT_PATH);   
            
            break;

            case KEY_ENTER : implementEnterKey();  break;

            case KEY_HOME : implementHKey();  break;

            case KEY_EXIT :  exit(1);  break;

            default :  break;
        }
    }
}

