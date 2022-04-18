#include <iostream>
#include <conio.h>
#include <cstring>
#include <fstream>
#include <fileapi.h>
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

char folderpath[256] = {0};

bool manifestpatcher(char path[256], string manifestid)
{
	char altpath[256];
	string curline;
	strcpy(altpath, path);
	strcat(altpath, ".tmp");
	fstream mainfile(path, ios::in | ios::out);
	ofstream tempfile(altpath);
	if(!mainfile.is_open() || !tempfile.is_open())
	{
		cout<<"Permission error - access denied. Grant yourself full permissions to the folder or try running this software as administrator and try again."<<endl;
		getch();
		exit(1);
	}
	for(int i=1; i<=6; i++)
	{
		getline(mainfile, curline);
		tempfile<<curline<<endl;
	}
	tempfile<<"\t\"StateFlags\"\t\t\"4\""<<endl;
	getline(mainfile, curline);
	for(int i=8; i<=24; i++)
	{
		getline(mainfile, curline);
		tempfile<<curline<<endl;
	}
	tempfile<<"\t\t\t\"manifest\"\t\t\""<<manifestid<<"\""<<endl;
	getline(mainfile, curline);
	while(getline(mainfile, curline))
	{
		tempfile<<curline<<endl;
	}
	tempfile.close();
	mainfile.close();
	remove(path);
	rename(altpath, path);
	return true;
}

void getGameName(string manifestid)
{
	string curline;
	// cout<<"funkcjasciezka"<<endl<<folderpath<<"test"<<endl;
	char id[30] = {0};
	// cout<<endl<<endl;
	for(int i=0; i<manifestid.length(); i++)
	{
		id[i] = manifestid[i];
		//cout<<id[i]<<"ID I"<<endl<<endl;
	}
	char path[256] = {0};
	strcpy(path, folderpath);
	strcat(path, id);
	ifstream gamename(path);
	if(!gamename.is_open())
	{
		cout<<manifestid<<" could not be opened!"<<endl;
		getch();
		exit(1);
	}
	for(int i=1; i<6; i++)
	{
		getline(gamename, curline);
	}
	getline(gamename, curline);
	curline = curline.substr(10, curline.length() - 10);
	curline = curline.substr(0, curline.length() - 1);
	cout<<curline<<endl;
	gamename.close();
}

void trimGameId(char filename[30])
{
	char manifestid[8] = {0};
	for(int i=12; i<=18; i++)
	{
		manifestid[i-12] = filename[i];
		if(!isdigit(filename[i+1]))
		{
			break;
		}
	}
	cout<<manifestid<<" - ";
	// getGameName(manifestid);
	for(int i=0; i<8; i++)
	{
		manifestid[i] = 0;
	}
}

void getFileList(char path[256])
{
	//string output = "", temp;
	WIN32_FIND_DATA searchres;
	char search[256] = {0}, manifestid[7] = {0};
	strcpy(search, path);
	strcat(search, "appmanifest_*.acf");
	// cout<<search<<endl;
	HANDLE filelist = NULL;
	filelist = FindFirstFileA(search, &searchres);
	// output = trimGameId(searchres.cFileName);
	trimGameId(searchres.cFileName);
	getGameName(searchres.cFileName);
	while(FindNextFile(filelist, &searchres))
	{
		trimGameId(searchres.cFileName);
		getGameName(searchres.cFileName);
		// temp = trimGameId(searchres.cFileName);
		// output = output + temp;
	}
	FindClose(filelist);
}

int main(int argc, char** argv) {
	unsigned int choice = 1;
	bool installmain = false, validpath = true;
	const char defaultpath[256] = "C:\\Program Files (x86)\\Steam\\steamapps\\libraryfolders.vdf";
	char libpath[256] = {0}, gameid[8] = {0}, filepath[256] = {0};
	string manifest;
	cout<<"*****************************************************"<<endl;
	cout<<"*************** STEAM UPDATE BYPASSER ***************"<<endl;
	cout<<"*****************************************************"<<endl<<endl;
	cout<<"Choose path mode"<<endl;
	cout<<"1 - Automatic (recommended)"<<endl;
	cout<<"2 - Manual"<<endl;
	cout<<"0 - Quit"<<endl<<endl;
	cin>>choice;
	cout<<endl;
	cin.ignore();
	if(choice == 1)
	{
		cout<<"Automatic mode selected, scanning for Steam install..."<<endl<<endl;
		ifstream mainfolder(defaultpath);
		if(mainfolder.is_open())
		{
			cout<<"Steam install folder found at "<<defaultpath<<endl<<endl;
			mainfolder.close();
			strncpy(folderpath, defaultpath, 39);
			getFileList(folderpath);
		}
		else
		{
			cout<<"Automatic mode failed - cannot find Steam at default install directory. Manual mode must be used."<<endl<<endl;
			getch();
			exit(1);
		}
	}
	else if(choice == 2)
	{
		char userpath[256] = {0};
		string inputpath;
		cout<<"Manual mode selected. Drag and drop or paste the steam\\steamapps folder or steamlibrary folder."<<endl<<endl;
		getline(cin, inputpath);
		for(int i=0; i<inputpath.length(); i++)
		{
			userpath[i] = inputpath[i];
		}
		cout<<"Checking if this is a valid Steam library folder..."<<endl;
		if(userpath[0] == '\"')
		{
			userpath[0] = 0;
			for(int i=255; i>2; i--)
			{
				if(userpath[i] == '\"')
				{
					userpath[i] = 0;
					validpath = false;
					break;
				}
			}
			if(!validpath)
			{
				for(int i=0; i<255; i++)
				{
					userpath[i] = userpath[i+1];
				}
			}
		}
		for(int i=255; i>2; i--)
		{
			if(userpath[i] == '\\')
			{
				userpath[i] = 0;
				break;
			}
			if(userpath[i] != 0)
			{
				if(userpath[i] != 's')
				{
					cout<<"Pasting files is not supported!"<<endl;
					getch();
					exit(1);
				}
				break;
			}
		}
		strcpy(folderpath, userpath);
		strcpy(libpath, userpath);
		strcat(libpath, "\\..\\libraryfolder.vdf");
		strcat(folderpath, "\\libraryfolders.vdf");
		ifstream mainfolder(folderpath);
		ifstream libfolder(libpath);
		if(!mainfolder.is_open() && !libfolder.is_open())
		{
			cout<<"This is not a valid Steam library folder!"<<endl;
			getch();
			exit(1);
		}
		if(mainfolder.is_open())
		{
			mainfolder.close();
		}
		else
		{
			libfolder.close();
		}
		cout<<userpath<<" is a valid Steam folder! Loading games list..."<<endl<<endl;
		for(int i=0; i<256; i++)
		{
			folderpath[i] = 0;
		}
		strcpy(folderpath, userpath);
		strcat(folderpath, "\\");
		getFileList(folderpath);
	}
	else
	{
		getch();
		exit(0);
	}
	while(true)
	{
		cout<<endl<<"Input the ID of the game you'd like to bypass updates of!"<<endl;
		cin>>gameid;
		cin.ignore();
		cout<<"Checking if the file exists..."<<endl;
		strcpy(filepath, folderpath);
		strcat(filepath, "appmanifest_");
		strcat(filepath, gameid);
		strcat(filepath, ".acf");
		ifstream patcher(filepath);
		if(!patcher.is_open())
		{
			cout<<"File doesn't exist or permission denied!"<<endl<<endl;
		}
		else
		{
			patcher.close();
			cout<<"Found the file at "<<filepath<<endl<<endl;
			cout<<"Input the manifest number"<<endl;
			cin>>manifest;
			cin.ignore();
			if(manifest.length() != 19)
			{
				cout<<"Incorrect manifest length!"<<endl<<endl;
			}
			else
			{
				manifestpatcher(filepath, manifest);
				cout<<"Patching successful! Exiting..."<<endl;
				getch();
				break;
			}
		}
		for(int i=0; i<256; i++)
		{
			filepath[i] = 0;
		}
	}
	return 0;
}
