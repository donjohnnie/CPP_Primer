#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
string& address(string& name, const string& prefix, const string& suffix)
{
	if(name.empty() && prefix.empty() && suffix.empty())
		return name;
	name.insert(0, prefix);
	name.insert(name.size(),suffix);
	return name;
}
int main()
{
	string lady = "Christie";
	string gentleman;
	address(gentleman, "Mr.", "");
	cout << lady << endl;
	cout << gentleman << endl;
	return 0;
}