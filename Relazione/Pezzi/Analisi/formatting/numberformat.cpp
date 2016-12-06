#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string eraseSpaces(string s)
{
	for(int i = 0; i<s.size(); ++i)
	{
		if(s.at(i)==' ') s.erase(i,1);
		else if (s.at(i)=='\n') s.erase(i,1);
	}
	if(s.at(0)== '.') s = '0' + s;
	return s;
}
int main(int argc ,  char* argv[])
{
	int col, row;
	
	ifstream inFile;
	inFile.open("data.txt");
	ofstream outFile;
	outFile.open("formatted.txt");
	cout<<"Columns?: ";
	cin>>col;
	cout<<"Rows?: ";
	cin>>row;
	string s;
	for(int i = 0; i<row ; ++i)
	{
		for(int j = 0; j<col - 1; ++j)
		{
			getline(inFile,s,',');

			s=eraseSpaces(s);

			outFile<<"\\num\{"<<s<<'\}'<<'\t';
			
			s.clear();
		}
		getline(inFile,s,',');

		s=eraseSpaces(s);
		outFile<<"\\num\{"<<s<<'\}'<<endl;
		
		s.clear();
	}
	
	return 0;
}
