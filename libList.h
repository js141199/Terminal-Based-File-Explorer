#include<bits/stdc++.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include<dirent.h>
#include<iostream>
#include <stdio.h>
#include <time.h>
#include<fcntl.h>
#include <ctype.h>
#include<signal.h>
#include <errno.h>
#include<pwd.h>
#include<grp.h>


using namespace std;

#define DEFAULT_SLEEP 5000000
#define DEFAULT_FILE_PERM 0664
#define DEFAULT_DIRCT_MODE 0755
#define KEY_ESCAPE 27
#define KEY_ENTER 10
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67 
#define KEY_LEFT 68
#define KEY_EXIT 113
#define KEY_BACKSPACE 127
#define KEY_HOME 104
#define KEY_COLON 58

//------------------------ NORMAL MODE FUNCTIONS -----------------------

// functions of listing and staaring normal mode.
string convertIntToBytes(long long);
void fetchFileInformation(string);
void printAllDirDetails(int,int);
void listAllDirectories(string);
void switchToNormalMode();
void reSetCurPointers();
void moveCursorDown();
void moveCursorUp();
char readKey();


// functions of key press events in normal mode.
void implementRightArrowKey();
void implementBackSpaceKey();
void implementLeftArrowKey();
void backwardStPush(string);
void forwardStPush(string);
void updateCurrentPath();
void implementEnterKey();
string backwardStPop();
void backwardStClear();
string forwardStPop();
void forwardStClear();
void implementHKey();
void clearScreen();


//------------------------ COMMAND MODE FUNCTIONS -----------------------

void createReplicaOfDirectory(string,string);
void implementCreate(string,vector<string>);
void implementDelete(string,vector<string>);
void createDirectory(string,string,mode_t);
bool searchInSingleDirctory(string,string);
bool isDirctOrFilePresent(string,string);
void implementRename(string,string);
void implementCopy(vector<string>);
void deleteSingleDirectory(string);
void implementMove(vector<string>);
void implementMove(vector<string>);
void moveDirectory(string,string);
void copyDirectory(string,string);
string removeExtraSpaces(string);
void switchToCommandMode(string);
vector<string> getPaths(string);
bool isCopyValid(string,string);
string getAbsolutePath(string);
void createFile(string,string);
mode_t getPermissions(string);
void implementSearch(string);
void copyFile(string,string);
void moveFile(string,string);
void deleteDirectory(string);
void implementGoTo(string);
bool isDeleteValid(string);
string getAbsPath(string);
void deleteFile(string);
void startCommandMode();
void die(const char *);
void disableRawMode();
void enableRawMode();

// GLOBAL VARIABLE DECLARATIONS

string month[12] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Nov","Dec"};

static struct termios term, oterm;
struct termios orig_termios;

vector<vector<string>> dirDetails;

stack<string> forwardSt;
stack<string> backwardSt;


string CURRENT_PATH;
bool NORMAL_MODE;
int TOTAL_ROWS;
int PAGE_START;
int PAGE_END;
int CUR_POS;
int WIN_H;
int WIN_W;