#include <iostream>
#include <stdlib.h>
#include <string>

#include <windows.h>
#include <shellapi.h>

using namespace std;

string sides[4] = "";	// in order: L, B, R, T
string API_URL_preamble = "https://api.openstreetmap.org/api/0.6/trackpoints?bbox=";
string DEFAULT_RADIUS = "30";	// in meters


string tokenOnComma(string text);
string tokenAfterComma(string text);
void anglesToSides(string locat1, string locat2);
void centerToSides(string center, string r);
string URLConstructor(int curr_page_number);
void openBrowserURL(string URL_text);
void clearSides();


int main(){
	
	cout << "This program will use the coordinates given to download for you the GPX track files uploaded to Openstreetmap.org." << endl;
	cout << "In doing so, it will open your default browser and use the constructed URL to fetch the data." << endl;
	system("pause");
	system("cls");
	
	while(1){
		cout << "\nIn the form \"l.at, l.on\" (WGS84), enter two coordinates of the area of interest.\nAlternatively, enter the center coordinate and radius (recommended).\n";
		string str, str1 = "", str2 = "";
		int pages = 0;
		clearSides();
		
		cout << "Top-left corner (or center):  ";
		for(int it = 0; it < 2; it++){
			getline(cin, str);
			if(it == 0){
				str1 = str;
				cout << "Bottom-right corner (or radius, in meters):   ";
			}
			else  str2 = str;
		}
		if(str == "")  cout << "{Invalid; defaulting to 30m}" << endl;
		cout << endl;
		
		cout << "Ok. How many pages of tracks would you like to download? (hint: start with 1)  ";
		getline(cin, str);
		sscanf(str.c_str(), "%d", &pages);
		if(pages <= 0){
			pages = 1;
			cout << "{Invalid; defaulting to minimum (1).}" << endl;
		}
		cout << endl;
		
		
		if(tokenOnComma(str2) == "0"){
			string radius = (str2 != "")?  str2 : DEFAULT_RADIUS;
			centerToSides(str1, radius);
		}
		else{
			anglesToSides(str1, str2);
		}
		
		//if(pages > 1)		cout << "(From the most recent:)\n";
		for(int j = 0; j < pages; j++){
			string URL_text = URLConstructor(j);
			//cout << URL_text;
			openBrowserURL(URL_text);
		}
		cout << "Done!" << endl;
		
		cout << "Press ENTER to reload this tool; press \"E\" and then ENTER to quit:  ";
		getline(cin, str);
		if(str[0] == 'E'  ||  str[0] == 'e')  break;
		system("cls");
	}
	return 0;
}





string tokenOnComma(string text){
	string new_str = "";
	for(int i = 0; i < text.length(); i++){
		if(text[i] == ',')  return new_str;
		new_str += text[i];
	}
	return "0";
}

string tokenAfterComma(string text){
	string new_str = "";
	int i = 0;
	for(; i < text.length(); i++){
		if(text[i] == ' ')  break;
	}
	for(i = i+1; i < text.length(); i++){
		new_str += text[i];
	}
	
	return (new_str != "")? new_str : "0";
}

void anglesToSides(string locat1, string locat2){
	
	string lat1 = tokenOnComma(locat1);
	string lon1 = tokenAfterComma(locat1);
	
	string lat2 = tokenOnComma(locat2);
	string lon2 = tokenAfterComma(locat2);
	
	string left = lon1;
	string right = lon2;
	string top = lat1;
	string bott = lat2;
	
	sides[0] = left;
	sides[1] = bott;
	sides[2] = right;
	sides[3] = top;
	
	return;
}

void centerToSides(string center, string r){
	
	string latC = tokenOnComma(center);
	string lonC = tokenAfterComma(center);
	
	int radius = 1;
	sscanf(r.c_str(), "%d", &radius);
	
	float latF, lonF;
	sscanf(latC.c_str(), "%f", &latF);
	sscanf(lonC.c_str(), "%f", &lonF);
	
	float offs_deg = (float)(2*radius/100000.0);
	string locat1 = to_string(latF + offs_deg) + ", " + to_string(lonF - offs_deg);
	string locat2 = to_string(latF - offs_deg) + ", " + to_string(lonF + offs_deg);

	anglesToSides(locat1, locat2);
	
	return;
}

string URLConstructor(int curr_page_number){	// no param, global array
	// place coordinates in the form of:   https://api.openstreetmap.org/api/0.6/trackpoints?bbox=0,51.5,0.25,51.75&page=0
	char buffer[5];
  
	string coord_str = sides[0] + ',' + sides[1] + ',' + sides[2] + ',' + sides[3];
	string page = itoa(curr_page_number, buffer, 10);
	string URL_ending = "&page=" + page;
	
	return API_URL_preamble + coord_str + URL_ending + '\n';
}

void openBrowserURL(string URL_text){
	ShellExecuteA(NULL, "open", URL_text.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void clearSides(){
	int size = sizeof(sides)/sizeof(sides[0]);
	for(int k = 0; k < size; k++){
		sides[k] = "";
	}
}

