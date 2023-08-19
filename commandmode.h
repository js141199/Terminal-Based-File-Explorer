
/******************** REMOVE EXTRA SPACES FROM INPUT ***************/
string removeExtraSpaces(string input){

    string res="";
    int size = input.size();

    for(int i=0; i<size; i++){
        
        if(input[i] == ' ' && res == "") { continue; }
        
        else if(input[i] == ' ' && res[res.size()-1] == ' ') { continue; }
        
        else if(input[i] == ' ' && res[res.size()-1] != ' ') { res=res+" "; }
        
        else{ res = res + input[i]; }
    }

    return res;

}

/************************ PARSING THE INPUT ************************/
vector<string> getPaths(string input){

    // for parsing the space seperated input also.
    bool flag = false;
    vector<string> paths;
    string path = "";

    input = removeExtraSpaces(input);

    for(int i = 0;i<input.size();i++){
        char ch = input[i];
        // it is starting of new word
        if(ch == '"' && !flag){
            flag = true;
            continue;
        }
        // it is ending of new word
        else if(ch == '"' && flag){
            paths.push_back(path);
            i++;
            flag = false;
            path = "";
        }
        // still space seperated word continues
        else if(ch == ' ' && flag){
            path += ch;
        }else if(ch == ' ' && !flag){
            paths.push_back(path);
            path = "";
        }else{
            path += ch;
        }
    }

    if(path != "")
        paths.push_back(path);

    return paths;
}

/******************** GET THE MODES OF FILE/DIRCT *******************/
mode_t getPermissions(string src)
{
    struct stat inode;
    stat(src.c_str(), &inode);
    mode_t perms;
    
    // check permissions for other --> here in 0004 first 0 is to represent ocatal notation.
    perms = perms | ((inode.st_mode & S_IROTH)?0004:0);
    perms = perms | ((inode.st_mode & S_IWOTH)?0002:0);
    perms = perms | ((inode.st_mode & S_IXOTH)?0001:0);

    perms = perms | ((inode.st_mode & S_IRUSR)?0400:0);
    perms = perms | ((inode.st_mode & S_IWUSR)?0200:0);
    perms = perms | ((inode.st_mode & S_IXUSR)?0100:0);

    perms = perms | ((inode.st_mode & S_IRGRP)?0040:0);
    perms = perms | ((inode.st_mode & S_IWGRP)?0020:0);
    perms = perms | ((inode.st_mode & S_IXGRP)?0010:0);
    

    return perms ;
}

/************ IS IT POSSIBLE TO DELETE SPECIFIED DIRCT **************/
bool isDeleteValid(string delPath){
    
    // if i am trying to delete myself not possible
    if(CURRENT_PATH == delPath){
        cout << "Invalid path..... You cannot delete yourself.... " 
        << endl << "goto parent first and then try to delete...." << endl;
        return false;
    }

    if(delPath == "/"){
        cout << endl << "You cannot delete root directory" << endl;
        return false;
    }
    
    if(delPath == getHome()){
        cout << endl << "You cannot delete " << getHome() << endl;
        return false;
    }

    if(delPath == "/home"){
        cout << endl << "you cannot delete: /home" << endl;
        return false;
    }
        
    int subIndex = CURRENT_PATH.find(delPath+"/");

    // if del path is parrent of current_path it cannot be deleted.
    if(subIndex != string::npos && subIndex == 0){
        cout << endl << "Inavlid Path...Current and it's Parent directory cannot be deleted....." << endl;
        return false;
    }

    return true;
}

/************* IS IT POSSIBLE TO COPY SPECIFIED DIRCT ***************/
bool isCopyValid(string srcPath,string destPath){
    
    // if i am trying to copy myself into myself it is not spossible
    if(srcPath == destPath){
        cout << endl << "You cannot copy same folder into itself......" << endl;
        return false;
    }

    if(srcPath == "/"){
        cout << endl << "Inavlid Path...Root directory cannot be copied to child......" << endl;
        return false;
    }
    
    // inside child parent cannot be copied.
    int subIndex = destPath.find(srcPath+"/");

    cout << "source: " << srcPath << endl << "dest : " << destPath << endl;
    //usleep(15000000);
    
    // if i am trying to copy my into myself it is not possible
    if(subIndex != string::npos && subIndex == 0){
        cout << endl << "Inavlid Path...Parent directory cannot be copied to child......" << endl;
        return false;
    }

 //   usleep(5000000);

    return true;
}

/******************** RETURN THE ABSOLTE PATH ***********************/
string getAbsPath(string input){

    if(input == "/")
        return input;

    string path = "";

//    cout << input << endl;

    stack<string> st;

    if(input[0] == '~'){

        char char_array[getHome().size()+1];
        strcpy(char_array,getHome().c_str());
        
        char *ptr = strtok(char_array,"/");

         while (ptr != NULL)  {  
            if(ptr != ""){
                st.push(ptr);
                cout << ptr << endl;
                ptr = strtok (NULL, "/");  
            }
        }
        // remove ~ and /(if exist) from front
        input = input.size() == 1 ? input.substr(1) : input.substr(2);
    }
    else if(input[0] != '/'){
        char char_array[CURRENT_PATH.size()+1];
        strcpy(char_array,CURRENT_PATH.c_str());
        
        char *ptr = strtok(char_array,"/");

         while (ptr != NULL)  {  
            if(ptr != ""){
                st.push(ptr);
//                cout << ptr << endl;
                ptr = strtok (NULL, "/");  
            }
        }  
    }

    char char_array[input.size()+1];
    strcpy(char_array,input.c_str());

    char *ptr = strtok(char_array,"/");

    while (ptr != NULL)  {  
        string p = ptr;
        if(p != "" && p != "."){
            if(p == ".."){
                if(!st.empty())
                    st.pop();
            }else{
 //               cout << p  << endl;
                st.push(p);
            }
        }
        ptr = strtok (NULL, "/");  
    }  

    while (!st.empty())
    {
        /* code */
        string s = st.top();
        st.pop();
        
        path = (path == "" ? s : s + "/") + path;
    }
    
    path = "/" + path;

    return path;
}

/******************** PARSE THE PATH ********************************/
string getAbsolutePath(string input){

    // valid variations . ./ .. ../ ./name ../name n1/n2 / /n1 /..

    string filteredInput = "";

    for(int i = 0;i<input.size();i++){
        if(i == 0){
            filteredInput += input[i];
        }
        else{
            if(input[i] == '/'){
                filteredInput = (input[i-1] == '/') ? filteredInput : filteredInput + input[i];
            }else{
                filteredInput += input[i];
            }
        }
    }

    string absPath = getAbsPath(filteredInput);

    return absPath;
}

string getCWD(){

    char cwdAbsPath[250];
    getcwd(cwdAbsPath,sizeof(cwdAbsPath));
 //   cout << "cwd : " << cwdAbsPath << endl;
    return string(cwdAbsPath);
}

/******************** CREATE DIRECTORY ******************************/
void createDirectory(string dirName,string dirPath,mode_t perms){

    DIR* dir = opendir(dirPath.c_str());
    if(dir == NULL){
        cout << endl << "Invalid path => " << dirPath << strerror(errno) << endl;
        return ;
    }

    // if read permission is not there
    if(errno == EACCES){
        cout << endl << "Read Permissions denied....." << endl << strerror(errno) << endl;
        return ;
    }

    string path = dirPath == "/" ? dirPath + dirName  : dirPath + "/" + dirName;
    const char* dirctPath = path.c_str();

    if(mkdir(dirctPath,perms) == -1){
        cout << endl << "dirname: " << dirName << endl;
        cout << endl << "dirpath: " << dirctPath << endl;
        cerr << "Error in creating directory :  " << strerror(errno) << endl;
       // usleep(5000000);
    }else{
        cout << endl << "Directory created..." << endl;
    }

}

/******************** CREATE FILE ***********************************/
void createFile(string fileName,string dirPath){

    cout << endl << "Create file called" << endl;

    DIR* dir = opendir(dirPath.c_str());
    if(dir == NULL){
        cout << endl << "Invalid path => " << endl << strerror(errno) << endl;
        return ;
    }

    // if read permission is not there
    if(errno == EACCES){
        cout << endl << "Read Permissions denied....." << strerror(errno) << endl;
        return ;
    }

    string filePath = dirPath == "/" ? dirPath + fileName  : dirPath + "/" + fileName;
    
    // check if file already exists
    int file;
    if((file=open(filePath.c_str(),O_RDONLY))!=-1){
        cout << endl << "file already exixts => " << filePath << endl;
        return;
    }else{
        // creating a new file on specified destination
        if(creat(filePath.c_str(),DEFAULT_FILE_PERM)==-1) 
        { cerr << endl << "Failed to create file => " << filePath << endl 
            << "error: " << strerror(errno) << endl; }
    
        else{ cout << endl << fileName << " File created..." << endl; }    
    }

}

/******************** CREATE FUNCTION *******************************/
void implementCreate(string command,vector<string> paths){

    string dirPath = paths[1];

    // call function of parsing to get the absolute path
    dirPath = getAbsolutePath(dirPath) ;

    cout << "dir path : " << dirPath << endl;

    if(command == "create_file") { createFile(paths[0],dirPath); }
    
    else if (command == "create_dir") { createDirectory(paths[0],dirPath,DEFAULT_DIRCT_MODE); }

}

/********* GET THE NEW NAME FOR FILE *************************/
string createReplicaOfFile(string srcPath,string destPath){


    string fileName = srcPath.substr(srcPath.find_last_of("/")+1);

    cout << endl << "filename : " << fileName << endl;

    // if file starts with . then don't make replica
    if(fileName[0] == '.'){
        return "";
    }

    string fileExten = fileName.substr(fileName.find_last_of("."));
    cout << endl << "fileExten : " << fileExten << endl;
    string fileNameWithoutExten = fileName.substr(0,fileName.find_last_of("."));
    cout << endl << "fileNameWithoutExten : " << fileNameWithoutExten << endl;
    int i = 1;
    string duplicateFileName = fileNameWithoutExten + "_" + to_string(i) + fileExten;
    cout << endl << "duplicateFileName : " << duplicateFileName << endl;
    while(searchInSingleDirctory(destPath,duplicateFileName)){
        i++;
        duplicateFileName = fileNameWithoutExten + "_" + to_string(i) + fileExten;
        cout << endl << "duplicateFileName : " << duplicateFileName << endl;
    }

    return duplicateFileName;
    
}

/******************** COPY FILE *************************************/
void copyFile(string srcPath,string destPath){

    string fileName = srcPath.substr(srcPath.find_last_of('/')+1);
    string dpath = destPath == "/" ? destPath + fileName : destPath + "/" + fileName;

    int srcFile,destFile;

    // if i am trying to copy the file at same dest as of src then make replica.
    if(srcPath == dpath){
        cout << endl << "Already exist so making replica of specified file......" << endl;
        string newFileName = createReplicaOfFile(srcPath,destPath);
        // if file starts with . then don't don't do anything
        if(newFileName == "") { return ; }
        dpath = destPath == "/" ? destPath + newFileName : destPath + "/" + newFileName;
    }

    // copy file with same permissions as of source file
    mode_t perms = getPermissions(srcPath);
    cout << "perms: " << perms << endl;
    char ch;
    if((srcFile=open(srcPath.c_str(),O_RDONLY))==-1){
        cout << endl << "Invalid Path => File doesn't exists at => " << srcPath << endl;
        return;
    }

    // if file already exists then delete it and make new one
    // so content will be replaced.
    // in real terminal also using cp command these functionality is implemented.
    if((destFile=open(dpath.c_str(),O_RDONLY))!=-1){
        deleteFile(dpath);
    }

    destFile = open(dpath.c_str(),O_WRONLY|O_CREAT,perms);
    
    while(read(srcFile,&ch,1))
        write(destFile,&ch,1);

    cout << endl << "Successfully copied......" << endl;        

    close(srcFile);
    close(destFile);

}

/******************** COPY DIRECTORY ********************************/
void copyDirectory(string srcPath,string destPath){

    // is src path valid
    DIR* dir = opendir(srcPath.c_str());

    if(dir == NULL){
        cout << "Invalid path => " << srcPath << endl;
        return ;
    }
    
    if(errno == EACCES){
        cout << endl << "You don't have read permissions..." << endl;
        return ;
    }

    string dirName = srcPath.substr(srcPath.find_last_of("/")+1);
    
    mode_t perms = getPermissions(srcPath);
    createDirectory(dirName,destPath,perms);
    
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        string dname = string(entry->d_name);
        string tempPath = srcPath == "/" ? srcPath + dname : srcPath + "/" + dname;
        if(dname != "." && dname != ".."){

            // cout << "forming : " << tempPath << endl;
            struct stat inode;
            if(stat((tempPath).c_str(),&inode) != 0){
                cout << "Unable to find information for specified path" << endl;
            }

            if(S_ISDIR(inode.st_mode)){
                // cout << "It is dir" << endl;
                copyDirectory(tempPath, destPath + "/" + dirName);
            }else{
                // cout << "it is file " << endl;
                copyFile(tempPath, destPath + "/" + dirName);
            }
        }
    }

    return ;
}

/************ SEARCH IN CURRENT DIRCT ONLY **************************/
bool searchInSingleDirctory(string destPath,string duplicateName){

    DIR* dir = opendir(destPath.c_str());

    struct dirent* entry;

    while((entry = readdir(dir)) != NULL){
        string dname = string(entry->d_name);
        if(dname != "." && dname != ".." && dname == duplicateName)
            return true;
    }

    return false;
}

/********* CREATE SAME FOLDER WITH NEW NAME *************************/
void createReplicaOfDirectory(string srcPath,string destPath){

    string dirctName = srcPath.substr(srcPath.find_last_of("/")+1);
    int i = 1;
    string duplicateName = dirctName + "_" + to_string(i);
    
    // go to the parent and search there
    //destPath = destPath.substr(0,destPath.find_last_of("/"));

    while(searchInSingleDirctory(destPath,duplicateName)){
        i++;
        duplicateName = dirctName + "_" + to_string(i);
    }

    cout << "duplicateName: " << duplicateName << endl;
    createDirectory(duplicateName,destPath,DEFAULT_DIRCT_MODE);

    // new destination after creating the replica
    destPath = destPath == "/" ? destPath + duplicateName : destPath + "/" + duplicateName;

    // now apply BFS and copy dirct and files of srcPath
    DIR* dir = opendir(srcPath.c_str());
    struct dirent* entry;

    if(errno == EACCES){
        cout << endl << "Read Permissions denied...." << endl;
        return;
    }

    while ((entry = readdir(dir)) != NULL){
        string dname = string(entry->d_name);

        if(dname != "." && dname != ".."){

            string newPath = srcPath == "/" ? srcPath + dname : srcPath + "/" + dname;

            cout << endl << "newpath: " << newPath << endl;

            DIR* dir = opendir(newPath.c_str());

            if(errno == EACCES){
                cout << endl << "Read Permissions denied...." << endl;
                continue;
            }

            struct stat inode;
            if(stat((newPath).c_str(),&inode) != 0){
                cout << "Unable to find information for specified path" << endl;
            }

            if(S_ISDIR(inode.st_mode)) { copyDirectory(newPath,destPath); }
            
            else { copyFile(newPath,destPath); }
        }   
    }
}

/******************** COPY FUNCTION *********************************/
void implementCopy(vector<string> paths){

    string destPath = paths[paths.size()-1];
    paths.pop_back();

    destPath = getAbsolutePath(destPath);

    DIR* dir = opendir(destPath.c_str());

    if(dir == NULL || errno == EACCES){
        cerr << "Invalid path => " << destPath << " " << strerror(errno) << endl;
        return ;
    }

    int fd = access(destPath.c_str(), (R_OK & W_OK));
    if(fd == -1){
        cout<<endl;
        perror("Error Description:");
        cout<<endl;
        return;
    }

    for(int i = 0;i<paths.size();i++){
        
        string srcPath = paths[i];

        srcPath = getAbsolutePath(srcPath);

        // check if path is directory
        struct stat inode;
        if(stat((srcPath).c_str(),&inode) != 0){
            cout << "Unable to find information for : " << srcPath << endl;
            continue;
        }

        // if dirct found
        string srcParent = srcPath.substr(0,srcPath.find_last_of("/"));
        if(S_ISDIR(inode.st_mode)){ 
            cout << "srcParent :" << srcParent << endl;
            cout << "destPath: " << destPath << endl;
          //  usleep(20000000);
            // if dirct is to be added to same srcPath and destPath
            if((srcParent == "" ? "/" : srcParent) == destPath){
                cout << endl << "Already exist so making replica of specified directory......" << endl;
                createReplicaOfDirectory(srcPath,destPath);
            }
            
            else if(isCopyValid(srcPath,destPath)){ copyDirectory(srcPath,destPath); }
        }
        
        // if it is file
        else { 
            copyFile(srcPath,destPath);
    }

    }
}

/********** Search for file or directory in current directory *******/
bool isDirctOrFilePresent(string dirPath,string searchName){

    struct dirent *entry;
    DIR *dir = opendir(dirPath.c_str());

    // if dirPath is a file and not a dirct
    if (dir == NULL){
        cout << "Invalid path => " << dirPath << endl;
        return false;
    }
    
    while ((entry = readdir(dir)) != NULL) {

        string dname = string(entry->d_name);
        string path = dirPath + "/" + dname;
        if(dname != "." && dname != ".."){
//            cout << "path=>" << path << endl;
            if(dname == searchName) { return true; }    

            struct stat inode;

            if(stat((path).c_str(),&inode) != 0){
//                cout << "Unable to find information for specified path=>" << path << endl;
                return false;
            }
            // check if directory
            if(S_ISDIR(inode.st_mode)){
                if(isDirctOrFilePresent(path,searchName)){
                    return true;
                }
            }
        }
    }

    closedir(dir);

    return false;

}

/******************** SEARCH FUNCTION *******************************/
void implementSearch(string searchName){

    string status[2] = {"False","True"};

    string dirPath = CURRENT_PATH;

    bool isPresent = isDirctOrFilePresent(dirPath,searchName);

    cout << status[isPresent] << endl;

}

/************ RENAME FILE NAME ANYWHERE IN SYSTEM ************/
void implementRename(string oldFileNamePath,string newFileName){

    // int rename( const char *oldname, const char *newname );

    string oldFilePath = getAbsolutePath(oldFileNamePath);

    // check if given path is of any directory
    DIR* dir = opendir(oldFilePath.c_str());

    if(dir != NULL){
        cout << endl << "Given old name is of directory...Try with file name..." << endl;
        return ;
    }

    string newPath = oldFilePath.substr(0,oldFilePath.find_last_of("/"));

    string newFilePath = newPath == "/" ? newPath + newFileName : newPath + "/" + newFileName;

    // if file name already exists same as new name.
    int newFile;
    if((newFile = open(newFilePath.c_str(),O_RDONLY)) != -1){
        cout << endl << "File with " << newFileName << " already exists. Try with another name..." << endl;
        return ;
    }    
    
    int status = rename(oldFilePath.c_str(),newFilePath.c_str());

    if(status != 0){
        cout << "No file exist at " << oldFilePath << endl;
    }else{
        cout << "renamed successfully....." << endl;
    }

}

/******************* GOTO SPECIFIC DIRECTORY ************************/
void implementGoTo(string destPath){

    destPath = getAbsolutePath(destPath);

    DIR* dir = opendir(destPath.c_str());

    if(dir == NULL){
        cout << "Invalid path => " << destPath << endl;
        return ;
    }else{

        // clear the forward stack
        forwardStClear();
        
        if(CURRENT_PATH != destPath)
            // push current path to back stack
            backwardStPush(CURRENT_PATH);

        CURRENT_PATH = destPath;
    }

}

/******************** DELETE FILE ***********************************/
void deleteFile(string filePath){

    cout << endl << "delete File : " << filePath << endl;

    int status = unlink(filePath.c_str());

    if(status == -1) { cout << endl << "Invalid Path! Deletion failed for => " << filePath << endl; }
    
    else{ cout << endl << endl << filePath << "File Deleted successfully....." << endl; }

}

/******************** DELETE SINGLE DIRECTORY ***********************/
void deleteSingleDirectory(string dirPath){

    // int rmdir(const char *path);

    int status = rmdir(dirPath.c_str());

    if(status == -1){
        cerr << " Error in deleteing dirct for => " << dirPath << " " << strerror(errno) << endl;
    }else{
        cout << "directory removed...." << endl;
    }
}

/******************** DELETE DIRECTORY *****************************/
void deleteDirectory(string dirPath){

     if(dirPath == "/" || dirPath == getHome() || dirPath == "/home"){
        cout << endl << "Invalid Path... You cannot delete /home/username or root(/) or /home directories...." << endl;
        return ;
    }

    cout << endl << "dir path inside delete dir :" << dirPath << endl;

    DIR* dir = opendir(dirPath.c_str());
    struct dirent *entry;
    
    if(dir == NULL){
        cout << "No such dir path exits => " << dirPath << endl;
        return ;
    }

    while ((entry = readdir(dir)) != NULL){
        
        string dname = entry->d_name;

        if(dname != "." && dname != ".."){

            string tempPath = dirPath == "/" ? dirPath + dname : dirPath + "/" + dname;
            struct stat inode;

            if(stat((tempPath).c_str(),&inode) != 0){
                cout << "Unable to find information for specified path" << endl;
                return ;
            }

            // check if directory
            if(S_ISDIR(inode.st_mode)){
                deleteDirectory(tempPath);
            }

            else{
                cout << endl << "deleting file : " << tempPath << endl;            
                deleteFile(tempPath);
            }
        }
    }

    // once all the files/folder of current directory is deleted 
    // delete current empty directory
    deleteSingleDirectory(dirPath);
    cout << endl << "deleting folder : " << dirPath << endl;

    return ;

}

/******************** DELETE FUNCTION *******************************/
void implementDelete(string command,vector<string> paths){

    string delPath = paths[0];

    delPath = getAbsolutePath(delPath);

    if(delPath == "/" || delPath == getHome() || delPath == "/home"){
        cout << endl << "Invalid Path... You cannot delete /home/username or root(/) or /home directories...." << endl;
        return ;
    }

    cout << "del path : " << delPath << endl;

    if (command == "delete_file") { deleteFile(delPath); }

    else if(command == "delete_dir" && isDeleteValid(delPath)) { deleteDirectory(delPath); }

}

/******************** MOVE DIRECTORY ********************************/
void moveDirectory(string srcPath,string destPath){

    DIR* dir = opendir(srcPath.c_str());

    if(dir == NULL){
        cout << endl << "Invalid Path => " << destPath << endl;
        return ;
    }

    if(errno == EACCES){
        cout << endl << "You don't have read permissions..." << endl;
        return ;
    }

    string srcParent = srcPath.substr(0,srcPath.find_last_of("/"));
    
    if(!isCopyValid(srcPath,destPath) || (srcParent == "" ? "/" : srcParent) == destPath){
        cout << endl << "Move is invalid operation for specified " << srcPath << endl;
        //usleep(3000000);
        return ;
    }

    // first copy directory
    copyDirectory(srcPath,destPath);

    // now delete directory
    deleteDirectory(srcPath);

    //usleep(20000000);

}

/******************** MOVE FILE *************************************/
void moveFile(string srcPath,string destPath){

    // move = first copy then delete
    
    string fileName = srcPath.substr(srcPath.find_last_of('/')+1);
    string dpath = destPath == "/" ? destPath + fileName : destPath + "/" + fileName;

    // if destPath == srcPath then don't do anything
    if(srcPath == dpath){
        cout << endl << "File already exist on speified destination....." << endl;
        return ;
    }

    // first copy file
    copyFile(srcPath,destPath);

    // delete file
    deleteFile(srcPath);

}

/******************** MOVE FUNCTION *********************************/
void implementMove(vector<string> paths){

    string destPath = paths[paths.size()-1];
    paths.pop_back();

    destPath = getAbsolutePath(destPath);

    DIR* dir = opendir(destPath.c_str());

    if(dir == NULL){
        cout << endl << "Invalid Path => " << destPath << endl;
        return ;
    }

    if(errno == EACCES){
        cout << endl << "You don't have read permissions..." << endl;
        return ;
    }

    for(int i = 0;i<paths.size();i++){
        
        string srcPath = paths[i];

        srcPath = getAbsolutePath(srcPath);

        // check if dirct is to be moved
        struct stat inode;
        if(stat((srcPath).c_str(),&inode) != 0){
            cout << "Unable to find information for specified path" << endl;
            return ;
        }

        if(S_ISDIR(inode.st_mode) ){
            if(isDeleteValid(srcPath)) { moveDirectory(srcPath,destPath); }
            else { cout << endl << "Move cannot be done for => " << srcPath << endl; }
        }else{
            moveFile(srcPath,destPath);
        }
        
    }

}

/******************** START COMMAND MODE ****************************/
void startCommandMode(){

    bool flag = false; 
    while(!flag){
        
        clearScreen();
        NORMAL_MODE = false;
        listAllDirectories(CURRENT_PATH);

        char key;
        char prevKey = '\0';        
        string input = "";
        int size = 0;

        bool tempStatus = false;

        while(1){
            
            key = getchar();

            if(key == '\t'){
                continue;
            }

            // for clearing the buffer of previously pressed arrows.
            if (key == '['){
                tempStatus = true;
                continue;
            }

            if ((key == 'A' || key == 'B' || key == 'C' || key == 'D') && tempStatus){
                tempStatus = false;
                continue;
            }

            if(key == '\n'){
                break;
            }else if(key == '\x1b'){
                flag = true;
                input = "";
                break;
            }else if(key == KEY_BACKSPACE){
                if(size > 0){
                    cout << "\b \b";
                    input.pop_back();
                    size--;
                }
            }
            else{
                input += key;
                size++;
                
            }
            cout << key;
            //prevKey = key;
        }
        if(!flag){
            if(prevKey == ' ')
                input.pop_back();
            cout << endl;
            switchToCommandMode(input);  
        }else{
            //cout << "switching to normal mode" << endl;
        }
        usleep(3000000);
    }
 
}

/******************** MODE CHANGE TO COMMAND ************************/
void switchToCommandMode(string input){

    string command = "";

    if(input == ""){ cout << "Invalid Input.... " << endl; return ; }

    vector<string> path = getPaths(input);

    command = path[0];

    path.erase(path.begin());

    //cout << endl << command << endl; 

    if(command == "create_file" || command == "create_dir") { 
        
        if(path.size() != 2) { cout << endl << "Invalid command.... Try with.." << endl
                                << "create_file <file_name> <destination_path>" << endl
                                << "create_dir <dir_name> <destination_path>" << endl;
                                usleep(DEFAULT_SLEEP); }
        
        else { implementCreate(command,path); }
    }


    else if(command == "copy") { 

        if (path.size() < 2 ) { cout << endl << "Invalid command.... Try with..." << endl
                                << "copy <source_file(s)> <destination_directory>" << endl; usleep(DEFAULT_SLEEP); }

        else { implementCopy(path); }
    }


    else if (command == "search") { 
    
        if(path.size() != 1) { cout << endl << "Invalid command.... Try with..." << endl
                                << "search <file_name>" << endl
                                << "search <directory_name>" << endl; usleep(DEFAULT_SLEEP); }
       
        else { implementSearch(path[0]); }
    }


    else if (command == "rename") { 
        
        if(path.size() != 2) { cout << endl << "Invalid command.... Try with..." << endl
                                << "rename <old_filename> <new_filename>" << endl; usleep(DEFAULT_SLEEP); }
        
        else { implementRename(path[0],path[1]); }
    }


    else if (command == "goto") { 
        
        if(path.size() != 1) { cout << endl << "Invalid command.... Try with..." << endl
                                << "goto <directory_path>" << endl; usleep(DEFAULT_SLEEP); }
        
        else { implementGoTo(path[0]); }
    }


    else if (command == "delete_file" || command == "delete_dir") { 
        
        if(path.size() != 1) { cout << endl << "Invalid command.... Try with..." << endl
                                << "delete_file <file_path>" << endl
                                << "delete_dir <dir_path>" << endl; usleep(DEFAULT_SLEEP);}

        else { implementDelete(command,path); }
    }


    else if (command == "move") { 
        
        if (path.size() < 2 ) { cout << endl << "Invalid command.... Try with..." << endl
                                << "move <source_file(s)> <destination_directory>" << endl; usleep(DEFAULT_SLEEP); }
        
        else { implementMove(path); }
    }

    else if (command == "quit" ) { exit(1); }

    else {  clearScreen();
            cout << endl << "Invalid command name !! Try again with any of these \n"
            << "copy <source_file(s)> <destination_directory>" << endl
            << "move <source_file(s)> <destination_directory>" << endl
            << "rename <old_filename> <new_filename>" << endl
            << "create_file <file_name> <destination_path>" << endl
            << "create_dir <dir_name> <destination_path>" << endl
            << "delete_file <file_path>" << endl
            << "delete_dir <dir_path>" << endl
            << "goto <directory_path>" << endl
            << "search <file_name>" << endl
            << "search <directory_name>" << endl
            << "quit" << endl; usleep(1500000); }

}
