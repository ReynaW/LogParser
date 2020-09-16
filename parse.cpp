#include "parse.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include "ConsoleColor.h";
#include <map>
using namespace std;

/*
<-- LOG PARSER -->

Author: Reyna Wood 
Contact: reynawood.atx@gmail.com
Last updated: 6/1/2020


*/

vector<string> logF;
vector<string> sectTit;
vector<int> titCost;
vector<string> zeroT;
vector<string> oneT;
vector<string> twoT;
vector<string> viewed;

/*
* 
* Possible settings:
* 
0:
--Move Updates--
--Removing Trade Evolutions--
--Condensed Level Evolutions--
--Random 2-Evolution Starters--
--Custom Starters--

1:

--In-Game Trades--
--TM Moves--
--Move Tutor Moves--

2:
--Pokemon Base Stats & Types--
--Pokemon Movesets--
--Wild Pokemon--
--Static Pokemon--
--Trainers Pokemon--


*/

//Splits up a string input into separate parts. Used for file parsing.
vector<string> split(string strToSplit, char delimeter)
{
	stringstream ss(strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

//Initializes the vector of randomization settings and text. Loads in previous data of viewed items by the user. Throws errors if files are unable to be opened.
void initArr()
{
	ifstream myfile;
	myfile.open("log.txt");

	string line;
	string total;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.compare("") == 0) {
				total += "~";
			}
			else {
				total += line + "\n";
			}
		}
		myfile.close();
	}
	else {
		cout << "Error: File unable to be opened.";
	}

	logF = split(total, '~');
	logF.pop_back();

	zeroT.push_back("--Move Updates--");
	zeroT.push_back("--Removing Trade Evolutions--");
	zeroT.push_back("--Condensed Level Evolutions--");
	zeroT.push_back("--Random 2-Evolution Starters--");
	zeroT.push_back("--Custom Starters--");


	oneT.push_back("--In-Game Trades--");
	oneT.push_back("--TM Moves--");
	oneT.push_back("--Move Tutor Moves--");

	twoT.push_back("--Pokemon Base Stats & Types--");
	twoT.push_back("--Pokemon Movesets--");
	twoT.push_back("--Wild Pokemon--");
	twoT.push_back("--Static Pokemon--");
	twoT.push_back("--Trainers Pokemon--");


	myfile.close();

	ifstream viewf;
	viewf.open("viewed.txt");

	if (viewf.is_open())
	{
		while (getline(viewf, line))
		{
			viewed.push_back(line);
		}
		viewf.close();
	}
	else {
		cout << "Error: File unable to be opened.";
	}
	viewf.close();

}

//Checks to see if vector s contains a certain value.
boolean contains(vector<string> s, string o) {
	for (int i = 0; i < s.size(); i++) {
		if (s.at(i).compare(o) == 0) {
			return true;
		}
	}
	return false;
}


//After initializing the randomization settings into a vector, loads the section headers for each setting.
void sectionTitles() {
	for (int i = 0; i < logF.size(); i++) {
		string temp = logF.at(i).substr(0, logF.at(i).find_first_of("\n"));
		sectTit.push_back(temp);
		if (contains(viewed, temp)) {
			titCost.push_back(0);
		}
		else {
			if (contains(zeroT, temp)) {
				titCost.push_back(0);
				continue;
			}
			if (contains(oneT, temp)) {
				titCost.push_back(1);
				continue;
			}
			if (contains(twoT, temp)) {
				titCost.push_back(2);
				continue;
			}
			titCost.push_back(0);

		}

	}

}

//Refreshes after viewed data has changed.
void reloadViewed() {
	ofstream view("viewed.txt");
	for (int i = 0; i < viewed.size(); i++) {
		view << viewed.at(i) << "\n";
	}
	sectTit.clear();
	titCost.clear();
	sectionTitles();
}


int main() {
	//Initialization
	initArr();
	sectionTitles();
	
	cout << blue << "<--- LOG PARSER --->\n\n";
	cout << white << "--Pokemon Base Stats & Types-- includes held item search.\n";

	string line;

	ifstream myfile;
	myfile.open("tokendata.txt");
	getline(myfile, line);

	myfile.close();

	int tokens = stoi(line);


	int n;
	while (true) {
		//Menu setup: runs until valid choice is made
		while (true) {
			cout << purple << "\n\n1: Exit.\n";
			cout << purple << "2: Redeem tokens.\n";

			for (int i = 0; i < sectTit.size(); i++) {
				cout << i + 3 << ": " << sectTit.at(i) << " " << titCost.at(i) << " token(s).\n";
			}
			cout << green << "\n\nYou have " << tokens << " token(s).\n";
			cout << white << "\n\nEnter the number of the selection you would like to go to.\n";
			string inp;
			cin >> inp;
			regex r("\\d+");
			if (regex_match(inp, r)) {
				n = stoi(inp);
				if (n > 0 && n < (3+sectTit.size())) {
					break;
				}
				else {
					cout << "Number not in range.\n";
				}
			}
			else {
				cout << "Input not a number.\n";
			}

		}
		//Exits program
		if (n == 1) {
			break;
		}
		//Adds in a custom number of tokens
		if (n == 2) {
			int gym;
			while (true) {
				cout << white << "\n\nHow many tokens do you want to add? Enter a number between 1-1000\n";
				string inp;
				cin >> inp;
				regex r("\\d+");
				if (regex_match(inp, r)) {
					gym = stoi(inp);
					if (gym > 0 && gym < 1001) {
						break;
					}
					else {
						cout << "Number not in range.\n";
					}
				}
				else {
					cout << "Input not a number.\n";
				}

			}



			tokens += gym;

			ofstream td("tokendata.txt");
			td << tokens;

			cout << green << "\n\nYou now have " << tokens << " tokens.\n";
		}

		//Main body of randomization data. (For clarity, minimize if-statements you are currently not working with. Code can be kind of bulky.)
		//Depending on randomizer settings, some options will not be viewable if they have not been changed from the base game.
		else {
			
			//Zero tokens, dumps text data.
			if (sectTit.at(n - 3).compare("--Move Updates--") == 0 || sectTit.at(n - 3).compare("--Removing Trade Evolutions--") == 0 || sectTit.at(n - 3).compare("--Condensed Level Evolutions--") == 0 || sectTit.at(n - 3).compare("--Random 2-Evolution Starters--") == 0 || sectTit.at(n - 3).compare("--Custom Starters--") == 0) {
				cout << yellow << "\n" << logF.at(n - 3) << "\n";
			}

			//Shows in game trade data. If viewed once, will be available for zero tokens on subsequent viewings. Dumps all trade text.
			if (sectTit.at(n - 3).compare("--In-Game Trades--") == 0) {
				if (tokens < titCost.at(n - 3)) {
					cout << red << "\nNot enough tokens.";
				}
				else {
					cout << yellow << "\n" << logF.at(n - 3) << "\n";
					tokens -= titCost.at(n-3);
					ofstream td("tokendata.txt");
					td << tokens;
					if (!contains(viewed, "--In-Game Trades--")) {
						viewed.push_back("--In-Game Trades--");
					}
					reloadViewed();
				}
			}

			//Shows in game trade data. If viewed once, will be available for zero tokens on subsequent viewings. Dumps all move tutor text.
			if (sectTit.at(n - 3).compare("--Move Tutor Moves--") == 0) {
				if (tokens < titCost.at(n-3)) {
					cout << red << "\nNot enough tokens.";
				}
				else {
					cout << yellow << "\n" << logF.at(n - 3) << "\n";
					tokens -= titCost.at(n - 3);
					ofstream td("tokendata.txt");
					td << tokens;
					if (!contains(viewed, "--Move Tutor Moves--")) {
						viewed.push_back("--Move Tutor Moves--");
					}
					reloadViewed();
				}
			}

			//Shows TM move data. Only one move and number combination will be shown per token use. Saves after viewing, can view a list of seen TMs.
			if (sectTit.at(n - 3).compare("--TM Moves--") == 0) {
				if (tokens < titCost.at(n - 3)) {
					cout << red << "\nNot enough tokens.";
				}
				else {
					string MTV;
					int ch;
					while (true) {
						cout << "\nWould you like to\n1: See a TM move\n2: Search for a TM move\n3: View old searches\n";
						string inp;
						cin >> inp;
						regex r("\\d+");
						if (regex_match(inp, r)) {
							ch = stoi(inp);
							if (ch > 0 && ch < 4) {
								break;
							}
							else {
								cout << "Number not in range.\n";
							}
						}
						else {
							cout << "Input not a number.\n";
						}
					}
					if (ch == 1) {
						int tmn;
						while (true) {
							cout << "\nEnter the number of the TM. (1-95)\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								tmn = stoi(inp);
								if (tmn > 0 && tmn < 96) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						string numb;
						if (tmn > 9) {
							numb = "TM" + to_string(tmn);

						}
						else {
							numb = "TM0" + to_string(tmn);
						}
						MTV = logF.at(n - 3).substr(logF.at(n - 3).find(numb), logF.at(n - 3).find("\n", logF.at(n - 3).find(numb)) - logF.at(n - 3).find(numb));
						cout << yellow << "\n" << MTV << "\n";
						if (!contains(viewed, "MT " + MTV)) {
							tokens -= titCost.at(n - 3);
							ofstream td("tokendata.txt");
							td << tokens;
							viewed.push_back("MT " + MTV);
							reloadViewed();
						}


					}
					if (ch == 2) {
						cout << "\nEnter the name of the move you want to search for.\nNote: Tokens will be lost if the move is not found or misspelled. Capitalization matters. (EX: Take Down, DoubleSlap)\n";
						string search;
						cin >> ws;
						getline(cin, search);
						int find = logF.at(n - 3).find(search);
						if (find < search.npos) {
							MTV = logF.at(n - 3).substr(logF.at(n - 3).find(search) - 5, logF.at(n - 3).find("\n", logF.at(n - 3).find(search)) - logF.at(n - 3).find(search) + 5);
							cout << yellow << "\n" << MTV << "\n";
							if (!contains(viewed, "MT " + MTV)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("MT " + MTV);
								reloadViewed();
							}

						}
						else {
							cout << red << "\nNot found.\n";
						}

					}
					if (ch == 3) {
						for (int i = 0; i < viewed.size(); i++) {
							if (viewed.at(i).find("MT") == 0) {
								cout << yellow << "\n" << viewed.at(i).substr(2) << "\n";
							}
						}

					}

				}
			}
				
			//Shows pokemon base stat and type data, including held items. Various search criteria is available, whatever is viewed will be saved.
			if (sectTit.at(n - 3).compare("--Pokemon Base Stats & Types--") == 0) {
					if (tokens < titCost.at(n - 3)) {
						cout << red << "\nNot enough tokens.";
					}
					else {
						string BT;
						int ch;
						while (true) {
							cout << "\nWould you like to\n1: View all information about one Pokemon (including held items)\n2: Search by held items\n3: Search by ability\n4: View old searches\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								ch = stoi(inp);
								if (ch > 0 && ch < 5) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						if (ch == 1) {
							int op;
							while (true) {
								cout << "\Please enter the pokedex number of the Pokemon you wish to view data for (1-649)\n";
								string inp;
								cin >> inp;
								regex r("\\d+");
								if (regex_match(inp, r)) {
									op = stoi(inp);
									if (op > 0 && op < 650) {
										break;
									}
									else {
										cout << "Number not in range.\n";
									}
								}
								else {
									cout << "Input not a number.\n";
								}
							}
							string result = to_string(op);

							BT = logF.at(n - 3).substr(logF.at(n - 3).find(result), logF.at(n - 3).find("\n", logF.at(n - 3).find(result)) - logF.at(n - 3).find(result));
							cout << yellow << "\n" << BT << "\n";
							if (!contains(viewed, "BST " + BT)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("BST " + BT);
								reloadViewed();
							}

						}
						if (ch == 2) {
							cout << "\nEnter the name of the item you want to search for.\nNote: Tokens will be lost if the item is not found or misspelled. Capitalization matters. (EX: Moon Stone, BlackGlasses)\nPokedex numbers of Pokemon holding the requested item will be given\n";
							string search;
							cin >> ws;
							getline(cin, search);
							int find = logF.at(n - 3).find(search);
							string BT = search + ": ";
							if (find < search.npos) {
								int pos = 0;
								while (true) {
									BT += logF.at(n - 3).substr(logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) + 1, 3) + ", ";
									pos = logF.at(n - 3).find(search, pos) + 1;
									if (pos == 0) {
										break;
									}
								}

							}
							else {
								cout << red << "\nNot found.\n";
							}
							BT = BT.substr(0, BT.size() - 4);
							cout << yellow << "\n" << BT << "\n";
							if (!contains(viewed, "BST " + BT)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("BST " + BT);
								reloadViewed();
							}
						}
						if (ch == 3) {
							cout << "\nEnter the name of the ability you want to search for.\nNote: Tokens will be lost if the ability is not found or misspelled. Capitalization matters. (EX: Flash Fire)\nPokedex numbers of Pokemon with the requested ability will be given.\nIt is possible for the requested ability to be a Pokemon's hidden ability.\n";
							string search;
							cin >> ws;
							getline(cin, search);
							int find = logF.at(n - 3).find(search);
							string BT = search + ": ";
							if (find < search.npos) {
								int pos = 0;
								while (true) {
									BT += logF.at(n - 3).substr(logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) + 1, 3) + ", ";
									pos = logF.at(n - 3).find(search, pos) + 1;
									if (pos == 0) {
										break;
									}
								}

							}
							else {
								cout << red << "\nNot found.\n";
							}
							BT = BT.substr(0, BT.size() - 4);
							cout << yellow << "\n" << BT << "\n";
							if (!contains(viewed, "BST " + BT)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("BST " + BT);
								reloadViewed();
							}
						}
						if (ch == 4) {
							for (int i = 0; i < viewed.size(); i++) {
								if (viewed.at(i).find("BST") == 0) {
									cout << yellow << "\n" << viewed.at(i).substr(3) << "\n";
								}
							}
						}

					}
				}
				
			//Shows all moves available for a searched pokemon. Viewed data will be saved.
			if (sectTit.at(n - 3).compare("--Pokemon Movesets--") == 0) {
					if (tokens < titCost.at(n - 3)) {
						cout << red << "\nNot enough tokens.";
					}
					else {
						string MV;
						int ch;
						while (true) {
							cout << "\nWould you like to\n1: Search for a Pokemon's moveset\n2: View old searches\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								ch = stoi(inp);
								if (ch > 0 && ch < 3) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						if (ch == 1) {
							int op;
							while (true) {
								cout << "\Please enter the pokedex number of the Pokemon you wish to view data for (1-649)\n";
								string inp;
								cin >> inp;
								regex r("\\d+");
								if (regex_match(inp, r)) {
									op = stoi(inp);
									if (op > 0 && op < 650) {
										break;
									}
									else {
										cout << "Number not in range.\n";
									}
								}
								else {
									cout << "Input not a number.\n";
								}
							}
							string result = to_string(op);

							MV = logF.at(n - 3).substr(logF.at(n - 3).find(result), logF.at(n - 3).find("\n", logF.at(n - 3).find(result)) - logF.at(n - 3).find(result));
							cout << yellow << "\n" << MV << "\n";
							if (!contains(viewed, "MS " + MV)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("MS " + MV);
								reloadViewed();
							}

						}
						if (ch == 2) {
							for (int i = 0; i < viewed.size(); i++) {
								if (viewed.at(i).find("MS") == 0) {
									cout << yellow << "\n" << viewed.at(i).substr(2) << "\n";
								}
							}
						}

					}
					
				}

			//Search by wild pokemon or view all wild pokemon in an area. Viewed data will be saved.
			if (sectTit.at(n - 3).compare("--Wild Pokemon--") == 0) {
					if (tokens < titCost.at(n - 3)) {
						cout << red << "\nNot enough tokens.";
					}
					else {
						string WP;
						int ch;
						while (true) {
							cout << "\nWould you like to\n1: Search for a Pokemon\n2: View all Pokemon in a region\n3: View old searches\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								ch = stoi(inp);
								if (ch > 0 && ch < 4) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						if (ch == 1) {

							cout << "\nEnter the name of the Pokemon you want to search for.\nNote: Tokens will be lost if the Pokemon is not found or misspelled. Capitalization matters. (EX: Mime Jr.)\n";
							string search;
							cin >> ws;
							getline(cin, search);
							search = search + " ";
							int find = logF.at(n - 3).find(search);
							string WP = search + ": ";
							if (find < search.npos) {
								int pos = 0;
								while (true) {
									WP += logF.at(n - 3).substr(logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) + 1, logF.at(n - 3).rfind(")", logF.at(n - 3).find(search, pos)) - logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos))) + " ";
									pos = logF.at(n - 3).find(search, pos) + 1;
									if (pos == 0) {
										break;
									}
								}

							}
							else {
								cout << red << "\nNot found.\n";
							}
							cout << yellow << "\n" << WP << "\n";
							if (!contains(viewed, "WP " + WP)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("WP " + WP);
								reloadViewed();
							}

						}
						if (ch == 2) {

							cout << "\nEnter the name of the route/location you want to search for.\nNote: Tokens will be lost if the place is not found or misspelled. Capitalization matters. (EX: Wellspring Cave)\n";
							string search;
							cin >> ws;
							getline(cin, search);
							search = search + " ";
							int find = logF.at(n - 3).find(search);
							string WP = search + ": ";
							if (find < search.npos) {
								int pos = 0;
								while (true) {
									WP += logF.at(n - 3).substr(logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) + 1, logF.at(n - 3).find("\n", logF.at(n - 3).find(search, pos))  - logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) ) + " ";
									pos = logF.at(n - 3).find(search, pos) + 1;
									if (pos == 0) {
										break;
									}
								}

							}
							else {
								cout << red << "\nNot found.\n";
							}
							cout << yellow << "\n" << WP << "\n";
							if (!contains(viewed, "WP " + WP)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("WP " + WP);
								reloadViewed();
							}

						}
						if (ch == 3) {
							for (int i = 0; i < viewed.size(); i++) {
								if (viewed.at(i).find("WP") == 0) {
									cout << yellow << "\n" << viewed.at(i).substr(2) << "\n";
								}
							}
						}

					}

				}

			//Shows one static pokemon randomization. Viewed data will be saved.
			if (sectTit.at(n - 3).compare("--Static Pokemon--") == 0) {
					if (tokens < titCost.at(n - 3)) {
						cout << red << "\nNot enough tokens.";
					}
					else {
						string SP;
						int ch;
						while (true) {
							cout << "\nWould you like to\n1: View a static Pokemon\n2: View old searches\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								ch = stoi(inp);
								if (ch > 0 && ch < 3) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						if (ch == 1) {
							cout << "\nEnter the name of the Pokemon you want to search for.\nNote: Tokens will be lost if the Pokemon is not found or misspelled. Capitalization matters. (EX: Mime Jr.)\n";
							string search;
							cin >> ws;
							getline(cin, search);
							int find = logF.at(n - 3).find(search);
							string SP = search + ": ";
							if (find < search.npos) {
								SP = logF.at(n - 3).substr(logF.at(n - 3).find(search), logF.at(n - 3).find("\n", logF.at(n - 3).find(search)) - logF.at(n - 3).find(search));
							}
							else {
								cout << red << "\nNot found.\n";
							}
							cout << yellow << "\n" << SP << "\n";
							if (!contains(viewed, "SP " + SP)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("SP " + SP);
								reloadViewed();
							}

						}
						if (ch == 2) {
							for (int i = 0; i < viewed.size(); i++) {
								if (viewed.at(i).find("SP") == 0) {
									cout << yellow << "\n" << viewed.at(i).substr(2) << "\n";
								}
							}
						}


					}

				}

			//Shows all of a selected trainer's pokemon. This will NOT work with randomized trainer names (yet). Viewed data will be saved.
			if (sectTit.at(n - 3).compare("--Trainers Pokemon--") == 0) {
					if (tokens < titCost.at(n - 3)) {
						cout << red << "\nNot enough tokens.";
					}
					else {
						string TP;
						int ch;
						while (true) {
							cout << "\nWould you like to\n1: View a trainer's Pokemon\n2: View old searches\n";
							string inp;
							cin >> inp;
							regex r("\\d+");
							if (regex_match(inp, r)) {
								ch = stoi(inp);
								if (ch > 0 && ch < 3) {
									break;
								}
								else {
									cout << "Number not in range.\n";
								}
							}
							else {
								cout << "Input not a number.\n";
							}
						}
						if (ch == 1) {
							cout << "\nEnter the name of the trainer you want to search for.\nNote: Tokens will be lost if the trainer is not found or misspelled. Capitalization matters. (EX: Botanist Bob.)\n";
							string search;
							cin >> ws;
							getline(cin, search);
							int find = logF.at(n - 3).find(search);
							string TP = search + ": ";
							if (find < search.npos) {
								int pos = 0;
								while (true) {
									TP += logF.at(n - 3).substr(logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos)) + 1, logF.at(n - 3).find("\n", logF.at(n - 3).find(search, pos)) - logF.at(n - 3).rfind("\n", logF.at(n - 3).find(search, pos))) + " ";
									pos = logF.at(n - 3).find(search, pos) + 1;
									if (pos == 0) {
										break;
									}
								}

							}
							else {
								cout << red << "\nNot found.\n";
							}
							cout << yellow << "\n" << TP << "\n";
							if (!contains(viewed, "TP " + TP)) {
								tokens -= titCost.at(n - 3);
								ofstream td("tokendata.txt");
								td << tokens;
								viewed.push_back("TP " + TP);
								reloadViewed();
							}
						}
						if (ch == 2) {
							for (int i = 0; i < viewed.size(); i++) {
								if (viewed.at(i).find("TP") == 0) {
									cout << yellow << "\n" << viewed.at(i).substr(2) << "\n";
								}
							}
						}


					}

				}
		}

	}
	return 0;
}
