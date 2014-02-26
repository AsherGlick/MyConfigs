#include <ncursesw/ncurses.h>	
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <locale.h>
// #include <ifstream>
#include <fstream>


#include <iostream>



using namespace std;

// Default value for enabling / disabling unicode output
bool UNICODE_ENABLED = true;
bool COLOR_ENABLED = TRUE;
bool VERTICAL_GROUPS = false;
int XSCROLLSPEED = 3;
// vector<string> parseGroup() {
// }

struct UserGroup {
	vector<string> users;
	vector<string> groups;
	vector< vector< string > > mappings;
};

vector<string> explode(string line, string delimiter) {

	vector<string> output;

	int lastSplit = 0;
	for(int i = 0; i <= line.length()-delimiter.length(); i++) {
		if (line.substr(i,delimiter.length()) == delimiter) {
			output.push_back(line.substr(lastSplit, i - lastSplit));
			i += delimiter.length();
			lastSplit = i;
			--i; // off by one
		}	
	}
	output.push_back(line.substr(lastSplit, line.length() - lastSplit));

	return output;
}

UserGroup parseUsers() {

	std::ifstream etcGroups("/etc/group");
	std::ifstream etcPasswd("/etc/passwd");
	// parse the users, save the username and the default group


	// parse the groups, set the


	// username -> {groupname,}
	map< string, set<string> > users;

	// groupid -> groupname
	map< string, string > groupids;

	// username -> groupname
	map<string, string> primaryGroup;

	string line;

	// foreach line in groups
	while (std::getline(etcGroups, line)) {
		string groupName;
		string password;
		string groupid;
		vector<string> userlist;

		vector<string> argumentList = explode(line, ":");

		for(string argument : argumentList) {
			cout << argument << "\t";
		}
		cout << endl;

		groupName = argumentList[0];
		password = argumentList[1];
		groupid = argumentList[2];

		// cout << argumentList.size() << endl;

		userlist = vector<string>(argumentList.begin()+3, argumentList.end());

		// create an object for the group in groupid
		groupids[groupid] = groupName;
		// for each user that exists add this group to their set in users
		for (string user : userlist) {
			if (user.length() > 0) {
				users[user].insert(groupName);
			}
		}
	}
	// foreach line in users
	while (getline(etcPasswd, line)) {
		string userName;
		string password;
		string userID;
		string primaryGroupID;
		string userInfo;
		string homeDirectory;
		string defaultShell;

		vector<string> argumentList = explode(line, ":");

		// for(string argument : argumentList) {
		// 	cout << argument << "\t";
		// }
		// cout << endl;

		userName = argumentList[0];
		password = argumentList[1];
		userID = argumentList[2];
		primaryGroupID = argumentList[3];
		userInfo = argumentList[4];
		homeDirectory = argumentList[5];
		defaultShell = argumentList[6];


		string primaryGroupName;
		if (groupids.find(primaryGroupID) != groupids.end()) {
			primaryGroupName = groupids[primaryGroupID];
			primaryGroup[userName] = primaryGroupName;
		}
		else {
			primaryGroupName = "("+userName+":"+primaryGroupID+")";
			groupids[primaryGroupID] = "("+userName+":"+primaryGroupID+")";
		}
		// create an object in users that contains an empty set if empty
		// add an element to that list containing the name of the primary group the belong to

		users[userName].insert(primaryGroupName);
	}


	UserGroup output;


	for (pair<string, string> group : groupids) {
		output.groups.push_back(group.second);
	}

	for (pair<string, set<string> > user : users) {
		output.users.push_back(user.first);
	}

	// output.users = {"chef", "man", "argalfargalarg"};
	// output.groups = {"root", "sudo", "chef", "man", "arg"};
	// output.mappings = {
	// 	{" ", " ","#"," "," "},
	// 	{" ", "X","X","#"," "},
	// 	{"X", " "," "," ","#"}
	// };


	output.mappings = vector< vector< string > > ( output.users.size(), vector<string> ( output.groups.size(), " " ) );

	for (int i = 0; i < output.users.size(); i++) {
		for (int j = 0; j < output.groups.size(); j++) {
			if (users[output.users[i]].find(output.groups[j]) != users[output.users[i]].end()) {
				output.mappings[i][j] = "X";
			}
			if (primaryGroup[output.users[i]] == output.groups[j]) {
				output.mappings[i][j] = "#";
			}
		}
	}

	return output;
}

/************************************ MAIN ************************************\
| This function is in charge of handling the main window for ncurses. It       |
| delegates window size and reacts to user input                               |
\******************************************************************************/
int main() {
// 	parseUsers();
// }

// int dub() {


	// X and Y offsets for viewing data
	int xOffset = 0;
	int yOffset = 1;

	int rowSelected = 0;

	setlocale(LC_ALL, "");



	// Initilize Screen
	initscr(); 

	// Set some global terminal settings
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);


	// Get the height and width of the screen
	int h, w;
	getmaxyx(stdscr, h, w);


	if (COLOR_ENABLED) {
		start_color();
		use_default_colors();
		init_pair(1, COLOR_RED, -1);
		int newcolor;
		init_pair(2, COLOR_BLUE, -1);
		init_pair(3, COLOR_GREEN, -1);
	}


	// Parse the usergroup file
	UserGroup usergroup = parseUsers();


	// Get the longest length of usernames
	int longestUsername = 0;
	for (string username : usergroup.users) {
		if (longestUsername < username.length()) {
			longestUsername = username.length();
		}
	}

	// Get the longest length of groups
	int longestGroupname = 0;
	for (string groupname : usergroup.groups) {
		if (longestGroupname < groupname.length()) {
			longestGroupname = groupname.length();
		}
	}


	vector<string> groupnameCashe;

	if (!VERTICAL_GROUPS) {
		groupnameCashe = vector<string>(1,"| ");
		for (string groupname : usergroup.groups) {
			groupnameCashe[0] += groupname + " | ";
		}
		longestGroupname = 1;
	}
	else {
		groupnameCashe = vector<string>(longestGroupname,"| ");
		for (int i = 0; i < longestGroupname; i++) {
			// wmove(grouplist, i, 0);
			// waddstr(grouplist, "| ");

			for (int j = 0; j < usergroup.groups.size(); j++) {
				int offset = longestGroupname - usergroup.groups[j].size();

				if (offset <= i) {

					// char character = 
					// waddstr(grouplist, "\033[1m")



					// waddch(grouplist,usergroup.groups[j][i-offset]);
					groupnameCashe[i] += usergroup.groups[j][i-offset];


					// waddstr(grouplist, "\033[0m");
				}
				else {

					groupnameCashe[i] += " ";
				}


				// if (UNICODE_ENABLED) waddstr(grouplist, " │ ");
				// else waddstr(grouplist, " | ");
				groupnameCashe[i] += " | ";
			}
		}
	}


	// Create the mapping buffers
	vector<string> mappingCashe(usergroup.users.size(), "|");
	for (int i = 0; i < usergroup.groups.size(); i++) {
		for (int j = 0; j < usergroup.users.size(); j++) {
			if (!VERTICAL_GROUPS){
				mappingCashe[j] += usergroup.mappings[j][i];
				for (int k = 0; k < usergroup.groups[i].length(); k++){
					mappingCashe[j] += usergroup.mappings[j][i];
				}
				mappingCashe[j] += usergroup.mappings[j][i];
			}
			else {
				mappingCashe[j] += usergroup.mappings[j][i];
				mappingCashe[j] += usergroup.mappings[j][i];
				mappingCashe[j] += usergroup.mappings[j][i];
			}
			mappingCashe[j] += "|";
		}
	}

	int nlines = h - longestGroupname -1;
	int ncols = w - longestUsername - 2;

	WINDOW * mapping = newwin(nlines, ncols, longestGroupname, longestUsername);
	WINDOW * userlist = newwin(nlines, longestUsername, longestGroupname , 0);
	WINDOW * grouplist = newwin(longestGroupname, ncols, 0, longestUsername);
	refresh();


	// Draw vector map of data
	// wmove(mapping, 0, 0);
	// for (int i = 0; i < 1000; i++){
	// 	wattron(mapping, COLOR_PAIR(2));
	// 	waddstr(mapping, "#");
	// 	wattroff(mapping, COLOR_PAIR(2));
	// }
	// wrefresh(mapping);


	while(true) {
		// Draw usernames
		for(int i = 0 ; i < nlines; i++) {
			wmove(userlist, i, 0);

			int index = i + yOffset;

			if (index < usergroup.users.size()) {
				waddstr(userlist, usergroup.users[index].c_str());
			}


			// rowSelected
			
		}
		wrefresh(userlist);

		// Draw groupnames
		// if (VERTICAL_GROUPS) {
			for (int i = 0; i < groupnameCashe.size(); i++) {
				wmove(grouplist, i , 0);
				for (int j = xOffset; j < xOffset + ncols; j++) {

					char character = groupnameCashe[i][j];

					if (character != '|') {
						wattron(grouplist, COLOR_PAIR(1));
						wattron(grouplist, A_BOLD);
					}

					if (character == '|' && UNICODE_ENABLED) {
						waddstr(grouplist, "│");
					}
					else {
						waddch(grouplist, character);
					}



					if (character != '|') {
						wattroff(grouplist, A_BOLD);
						wattroff(grouplist, COLOR_PAIR(1));
					}
				}
			}
			/// INSERT VERTICAL GROUP PRINTING (POSSIBLY MERGE WIHT OTHER)
		// }
		// else {
		// 	wmove (grouplist, 0 ,0);
		// 	for (int i = xOffset; i < xOffset + ncols; i++) {
		// 		waddch(grouplist, groupnameCashe[0][i]);
		// 	}
		// }
		wrefresh(grouplist);

		// Draw Mappings
		for (int i = 0; i < usergroup.mappings.size(); i ++) {
			wmove(mapping, i ,0);


			if (i == rowSelected) wattron(mapping,A_REVERSE);
			for (int j = xOffset; j < xOffset + ncols; j++) {
				char character = mappingCashe[i][j];

				if (character == '|' && UNICODE_ENABLED) {
					waddstr(mapping, "│");
				}
				else if (character == '#' && UNICODE_ENABLED && COLOR_ENABLED){
					wattron(mapping, COLOR_PAIR(2));
					wattron(mapping, A_BOLD);
					waddstr(mapping, "█");
					wattroff(mapping, A_BOLD);
					wattroff(mapping, COLOR_PAIR(2));
				}
				else if (character == 'X' && UNICODE_ENABLED && COLOR_ENABLED){
					wattron(mapping, COLOR_PAIR(3));
					wattron(mapping, A_BOLD);
					waddstr(mapping, "█");
					wattroff(mapping, A_BOLD);
					wattroff(mapping, COLOR_PAIR(3));
				}
				else {
					waddch(mapping, character);
				}
			}
			if (i == rowSelected) wattroff(mapping,A_REVERSE);

		}
		wrefresh(mapping);


		// printw("Hello world");
		refresh();

		// string helloworld = "Hello World";
		// printw( helloworld.c_str() );


		int inputkey = getch();
		move(0 , 0);
		addstr(to_string(inputkey).c_str() );
		refresh();

		bool quit = false;
		switch(inputkey) {
			case 113:  // Q
				quit=true;
				break;
			case 261: // right arrow
				xOffset += XSCROLLSPEED;
				break;
			case 260: // left arrow
				xOffset -= XSCROLLSPEED;
				break;
			case 258: // down arrow
				rowSelected+= 1;
				break;
			case 259: // up arrow
				rowSelected -= 1;
				break;
			case 108: // enter
				break;
		}


		// Check Bounds LR
		if (xOffset + ncols > mappingCashe[0].size()) {
			xOffset = mappingCashe[0].size() - ncols;
		}
		if (xOffset < 0) {
			xOffset = 0;
		}

		// Check Bounds UD
		if (rowSelected > yOffset + nlines-5) {
			++yOffset;
		}

		if (quit) {
			break;
		}

	}

	// Pasre Groups
	

	// Parse users



	//  ^U - create new user
	//  ^G - greate new group
	//  ^P - set selected group as user's primary group
	//  SPACE - toggle group on / off
	// 	^C - Exit
	//  ^O - Save
	

	delwin(mapping);
	delwin(grouplist);
	delwin(userlist);



	endwin();

	return 0;
}



// // hash example
// #include <iostream>
// #include <functional>
// #include <string>

// int main ()
// {
//   char nts1[] = "Test";
//   char nts2[] = "Test";
//   std::string str1 (nts1);
//   std::string str2 (nts2);

//   std::hash<char*> ptr_hash;
//   std::hash<std::string> str_hash;

//   std::cout << "same hashes:\n" << std::boolalpha;
//   std::cout << "nts1 and nts2: " << (ptr_hash(nts1)==ptr_hash(nts2)) << '\n';
//   std::cout << "str1 and str2: " << (str_hash(str1)==str_hash(str2)) << '\n';

//   return 0;
// }