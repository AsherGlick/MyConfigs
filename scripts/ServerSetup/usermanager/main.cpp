#include <ncursesw/ncurses.h>	
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <locale.h>
#include <algorithm>
// #include <ifstream>
#include <fstream>
#include <iostream>
#include <cstdarg>




using namespace std;

// Default value for enabling / disabling unicode output
bool UNICODE_ENABLED = true;
bool COLOR_ENABLED = true;
bool VERTICAL_GROUPS = false;


enum Mode { GROUP_MODE, USER_MODE };
// vector<string> parseGroup() {
// }

struct UserGroup {
	vector<string> users;
	vector<string> groups;
	vector< vector< string > > mappings;
};


vector<string> explode(string line, string delimiter) {

	if (line.length() < delimiter.length()) {
		return vector<string>();
	}

	vector<string> output;

	int lastSplit = 0;
	for(unsigned int i = 0; i <= line.length()-delimiter.length(); i++) {
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

		userlist = explode(argumentList[3], ",");

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

	for (unsigned int i = 0; i < output.users.size(); i++) {
		for (unsigned int j = 0; j < output.groups.size(); j++) {
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

// void redrawWindows(WINDOW * mapping, WINDOW * groupnames, WINDOW * usernames, WINDOW * userdata, WINDOW * menubar) {

// }


// This fucntion draws the entire table of mappings given upper and lower bounds of the grid to display
void drawMappingTable(
	WINDOW * mapping,
	const vector<string> & mappingCashe,
	const unsigned int & xOffset,
	const unsigned int & yOffset,
	const unsigned int & ncols,
	const unsigned int & nlines,
	const unsigned int & rowSelected,
	const unsigned int & columnSelectedMin,
	const unsigned int & columnSelectedMax
) {
	// Draw Mappings
	for(unsigned int i = 0 ; i < nlines; i++) {

		unsigned int index = i + yOffset;

		if (index < mappingCashe.size()) {
			wmove(mapping, i ,0);


			if (index == rowSelected) wattron(mapping,A_REVERSE);
			for (unsigned int j = xOffset; j < xOffset + ncols; j++) {
				if (index != rowSelected && j > columnSelectedMin && j < columnSelectedMax) wattron(mapping,A_REVERSE);
				if ( j < mappingCashe[index].length()) {
					char character = mappingCashe[index][j];

					if (character == '|' && UNICODE_ENABLED) {
						waddstr(mapping, "│");
					}
					else if (character == '#' && COLOR_ENABLED){
						wattron(mapping, COLOR_PAIR(2));
						wattron(mapping, A_BOLD);
						if (UNICODE_ENABLED) waddstr(mapping, "▌");
						else waddch(mapping, character);
						wattroff(mapping, A_BOLD);
						wattroff(mapping, COLOR_PAIR(2));
					}
					else if (character == 'X' && COLOR_ENABLED){
						wattron(mapping, COLOR_PAIR(3));
						wattron(mapping, A_BOLD);
						if (UNICODE_ENABLED) waddstr(mapping, "▌");
						else waddch(mapping, character);
						wattroff(mapping, A_BOLD);
						wattroff(mapping, COLOR_PAIR(3));
					}
					else {
						waddch(mapping, character);
					}
				}
				if (index != rowSelected && j > columnSelectedMin && j < columnSelectedMax) wattroff(mapping,A_REVERSE);
			}
			if (index == rowSelected) wattroff(mapping,A_REVERSE);
		}
	}
	wrefresh(mapping);
}

void drawCommandList (WINDOW * commandList, unsigned int screenWidth) {
	wmove(commandList, 1, (screenWidth * 0) / 5 + 2);
	waddstr(commandList, " Exit");
	wmove(commandList, 1, (screenWidth * 1) / 5 + 2);
	waddstr(commandList, " New");
	wmove(commandList, 1, (screenWidth * 2) / 5 + 2);
	waddstr(commandList, " Delete");
	wmove(commandList, 1, (screenWidth * 3) / 5 + 2);
	waddstr(commandList, " Toggle ");
		wattron(commandList,COLOR_PAIR(3));
		wattron(commandList, A_BOLD);
			waddstr(commandList, "Secondary");
		wattroff(commandList, A_BOLD);
		wattroff(commandList,COLOR_PAIR(3));
	wmove(commandList, 1, (screenWidth * 4) / 5 + 2);
	waddstr(commandList, " Set ");
		wattron(commandList,COLOR_PAIR(2));
		wattron(commandList, A_BOLD);
			waddstr(commandList, "Primary");
		wattroff(commandList, A_BOLD);
		wattroff(commandList,COLOR_PAIR(2));


	wattron(commandList, A_REVERSE);
	wmove(commandList, 1, (screenWidth * 0) / 5);
	waddstr(commandList, "^C");
	wmove(commandList, 1, (screenWidth * 1) / 5);
	waddstr(commandList, "^N");
	wmove(commandList, 1, (screenWidth * 2) / 5);
	waddstr(commandList, "^D");
	wmove(commandList, 1, (screenWidth * 3) / 5);
	waddstr(commandList, "  ");
	wmove(commandList, 1, (screenWidth * 4) / 5);
	waddstr(commandList, "^P");
	wattroff(commandList, A_REVERSE);


	wrefresh(commandList);
}

template<typename... Arguments>
int executeFunction(Arguments... args) {
	// Grab all the arguments as strings
	// int size = sizeof...(Arguments);
	vector<string> arguments = { (args)... };

	// Assemble the command on a single line
	string command;
	for (string d : arguments) {
		command += d + ' ';
	}

	// Run the command
	cerr << "Running:" << command << endl;

    return 0;
}


/************************************ MAIN ************************************\
| This function is in charge of handling the main window for ncurses. It       |
| delegates window size and reacts to user input                               |
\******************************************************************************/
int main() {

// 	parseUsers();
// }

// int dub() {
	Mode viewMode = GROUP_MODE;

	// X and Y offsets for viewing data
	unsigned int xOffset = 0;
	unsigned int yOffset = 0;

	unsigned int rowSelected = 0;
	unsigned int columnSelected = 0;

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
		init_pair(1, COLOR_RED, -1); // Group Names
		init_pair(2, COLOR_BLUE, -1); // Primary Group ID Color
		init_pair(3, COLOR_GREEN, -1); // Seconday Group ID Color
	}


	// Parse the usergroup file
	UserGroup usergroup = parseUsers();


	// Get the longest length of usernames
	unsigned int longestUsername = 0;
	for (string username : usergroup.users) {
		if (longestUsername < username.length()) {
			longestUsername = username.length();
		}
	}

	// Get the longest length of groups
	unsigned int longestGroupname = 0;
	for (string groupname : usergroup.groups) {
		if (longestGroupname < groupname.length()) {
			longestGroupname = groupname.length();
		}
	}


	// Used to determine which screen columns should be hilighted starting with the leftmost bound of 0
	vector<unsigned int> columnBounds = {0};
	// Used to cashe the display for the group names
	vector<string> groupnameCashe;

	if (!VERTICAL_GROUPS) {

		groupnameCashe = vector<string>(1,"| ");
		for (string groupname : usergroup.groups) {
			groupnameCashe[0] += groupname + " | ";

			// add the new bound to the list, -2 because there are 2 extra characters "| " after the end of the label
			columnBounds.push_back(groupnameCashe[0].length()-2);
		}
		longestGroupname = 1;
	}
	else {
		groupnameCashe = vector<string>(longestGroupname,"| ");
		for (unsigned int i = 0; i < longestGroupname; i++) {
			// wmove(grouplist, i, 0);
			// waddstr(grouplist, "| ");

			for (unsigned int j = 0; j < usergroup.groups.size(); j++) {
				unsigned int offset = longestGroupname - usergroup.groups[j].size();

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
				if (i == 0) {
					columnBounds.push_back(groupnameCashe[0].length()-2);
				}
			}
		}
	}


	// Create the mapping buffers
	vector<string> mappingCashe(usergroup.users.size(), "|");
	for (unsigned int i = 0; i < usergroup.groups.size(); i++) {
		for (unsigned int j = 0; j < usergroup.users.size(); j++) {
			if (!VERTICAL_GROUPS){
				mappingCashe[j] += usergroup.mappings[j][i];
				for (unsigned int k = 0; k < usergroup.groups[i].length(); k++){
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

	int nlines = h - longestGroupname -2;
	int ncols = w - longestUsername - 2;

	WINDOW * mapping = newwin(nlines, ncols, longestGroupname, longestUsername);
	WINDOW * userlist = newwin(nlines, longestUsername, longestGroupname , 0);
	WINDOW * grouplist = newwin(longestGroupname, ncols, 0, longestUsername);
	WINDOW * commandList = newwin(2, w, h-2,0);
	refresh();


  //////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////// DRAW LOOP ////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////  

	while(true) {
		// Draw usernames
		for(int i = 0 ; i < nlines; i++) {
			wmove(userlist, i, 0);

			unsigned int index = i + yOffset;

			if (index < usergroup.users.size()) {

				if (index == rowSelected) wattron(userlist,A_REVERSE);
				waddstr(userlist, usergroup.users[index].c_str());
				if (index == rowSelected) wattroff(userlist,A_REVERSE);

				// Padding to overwrite the ghosting letters from longer usernames
				waddstr(userlist, string(longestUsername-usergroup.users[index].length(), ' ').c_str());

			}			
		}
		wrefresh(userlist);

		// Draw groupnames
		for (unsigned int i = 0; i < groupnameCashe.size(); i++) {
			wmove(grouplist, i , 0);
			for (unsigned int j = xOffset; j < xOffset + ncols; j++) {
				if (j < groupnameCashe[i].size()) {
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
		}
		wrefresh(grouplist);

		drawMappingTable(mapping, mappingCashe, xOffset, yOffset, ncols, nlines, rowSelected, columnBounds[columnSelected], columnBounds[columnSelected+1]);

		drawCommandList(commandList,w);

		// Refresh the entire screen
		refresh();

		// Handle Keypresses
		int inputkey = getch();
		move(0 , 0);
		// addstr(to_string(inputkey).c_str() );
		addstr(to_string(columnBounds.size()).c_str());
		refresh();

		bool quit = false;
		switch(inputkey) {
			case 113:  // q
			case 81: // Q
				quit=true;
				break;
			case 261: // right arrow
				if (columnSelected < columnBounds.size() - 2 ) {
					columnSelected += 1;
				}

				break;
			case 260: // left arrow
				if (columnSelected > 0) {
					columnSelected -= 1;
				}

				break;
			case 258: // down arrow
				rowSelected+= 1;
				break;
			case 259: // up arrow
				if (rowSelected > 0) {
					rowSelected -= 1;
				}
				break;
			case 10: // enter
				// Toggle between edit and view modes

				break;
			case 32: // Space (toggle settings)
				if (usergroup.mappings[rowSelected][columnSelected] == " ")
					executeFunction("/usr/sbin/adduser", usergroup.users[rowSelected], usergroup.groups[columnSelected]);
				else if (usergroup.mappings[rowSelected][columnSelected] == "X")
					executeFunction("/usr/sbin/deluser", usergroup.users[rowSelected], usergroup.groups[columnSelected]);
				else
					cerr << "You cant change the primary group of " << usergroup.users[rowSelected] << " yet" << endl;
				break;

			case 4: // ^D
				//
				break;
			case 14: // ^N
				break;
			case 16: // ^P
				break;
			case 9: // Tab
				// Switch views
				if (viewMode == GROUP_MODE) viewMode = USER_MODE;
				else viewMode = GROUP_MODE;
				// redrawstuff();
				break;
		}


		// Check Bounds LR
		unsigned int maxIndex = columnBounds.size()-1;
		unsigned int upperIndex = min(maxIndex,columnSelected+3);
		if (columnBounds[upperIndex] > xOffset + ncols) {

			xOffset = columnBounds[upperIndex]-ncols+1;
			;
		}

		unsigned int lowerIndex = columnSelected;
		if (lowerIndex >= 2) lowerIndex -= 2;
		if (xOffset > columnBounds[lowerIndex]) {
			int newPosition =columnBounds[lowerIndex];
			xOffset = max(0, newPosition);
		}

		// Check Bounds UD
		if (rowSelected > yOffset + nlines - 3) {
			++yOffset;
		}
		if (yOffset + nlines > usergroup.users.size()) {
			int newPosition = usergroup.users.size() - nlines;
			yOffset = max(0, newPosition);
		}
		if (rowSelected >= usergroup.users.size()){
			rowSelected = usergroup.users.size() -1;
		}
		if (rowSelected < yOffset + 2) {
			if (yOffset > 0 ) {
				--yOffset;
			}
		}

		if (quit) {
			break;
		}

	}

	//  U - create new user
	//  G - greate new group
	//  P - set selected group as user's primary group (GROUP EDIT MODE)
	//  RETURN - Enter Edit mode for the selected user
	//  SPACE - toggle group on / off (GROUP EDIT MODE)
	//  ESC - Return to view mode
	// 	Tab - Switch between user/group view and user/settings view
	//  q - Exit
	//  S - Save
	

	// Clean Up
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