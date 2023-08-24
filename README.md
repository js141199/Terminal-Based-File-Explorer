# Terminal-Based-File-Explorer

## Application modes:
1. Normal mode (default mode) - used to explore the current directory and navigate the filesystem
2. Command mode - used to enter shell commands


### How to run the application:
- Commands: 
    - g++ main.cpp
    - ./a.out

### Normal mode: The list of functionalities that application provides.

- Display a list of directories and files in the current folder
    - Every file in the directory will be displayed on a new line with 
      the following attributes for each file
        - File Name
        - File Size
        - Ownership (user and group) and Permissions
        - Last modified
    - The file explorer will show entries “.” and “..” for current and 
      parent directory respectively.
    - The file explorer will handle scrolling using the up and 
      down arrow keys.
    - User will be able to navigate up and down in the file list using the  
      corresponding up and down arrow keys. The up and down arrow keys will also handle scrolling during vertical overflow.

- It will open directories and files when enter key is pressed -
    - Directory: It will clear the screen and screen and navigate into the 
      directory and show the directory contents as specified in point 1.
    - File - It will be opening the file in vi editor.
- Traversal
    - Go back - Left arrow key will take the user to the previously visited 
                directory.
    - Go forward - Right arrow key should take the user to the next directory
    - Up one level - Backspace key should take the user up one level
    - Home – h key should take the user to the home folder

### Command Mode: In these mode user needs to type commands as mentioned below.

1. The application will enter the Command button whenever “:” (colon) key is pressed. 
2. In the command mode, the user will be able to enter different commands. 
3. All commands appear in the status bar at the bottom.

- Copy: ‘$ copy <source_file(s)> <destination_directory>’
- Move: ‘$ move <source_file(s)> <destination_directory>’
- Rename: ‘$ rename <old_filename> <new_filename>’

Assumption: The destination directory exists, and we have write permissions.

- Create File: ‘$ create_file <file_name> <destination_path>’
- Create Directory: ‘$ create_dir <dir_name> <destination_path>’
- Delete File: ‘$ delete_file <file_path>’
- Delete Directory: ‘$ delete_dir <dir_path>’
    - On deleting directory, all content present inside the directory will be 
      recursively deleted.
- Goto: ‘$ goto <location>’
- Search: ‘$ search <file_name>’ or ‘$ search <directory_name>’
    - Searching will be done for a given file or folder under the current 
      directory recursively.
    - Output will be True or False depending on whether the file or folder 
      exists.


1. On pressing ESC key, the application will go back to Normal Mode.
2. On pressing q key in normal mode, the application will close. 
3. Similarly, entering the ‘quit’ command in command mode will also close the 
   application.

