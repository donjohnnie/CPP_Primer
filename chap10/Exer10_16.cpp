#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::size_t;
string make_plural(size_t ctr, const string &word, const string &ending)
{
	return (ctr > 1) ? word + ending : word;
}
void elimDup(vector<string> &words)
{
	sort(words.begin(), words.end());
	auto end_unique = unique(words.begin(), words.end());
	words.erase(end_unique, words.end());
}
void biggie(vector<string> &words, vector<string>::size_type sz)
{
	// put words in alphabetical order and remove duplicate
	elimDup(words);
	// sort words by size, but maintain alphabetical order for words of the same size
	stable_sort(words.begin(), words.end(),
	           [sz](const string &a, const string &b) {return a.size() < b.size();});
	// get an iterator to the first element whose size() is >= sz
	auto wc = find_if(words.begin(), words.end(), 
	           [sz](const string &s) {return s.size() >= sz;});
	// compute the number of elements with size >= sz
	auto count = words.end() - wc;
	cout << count << " " << make_plural(count, "word", "s") << " of length " 
	     << sz << " or longer" << endl;
	// put words of the given size or longer, each one followed by a space
	for_each(wc, words.end(),
	        [](const string &s){cout << s << " ";});
	cout << endl;
}
int main()
{
	vector<string> words = { "the", "quick", "red", "fox", "jump", "over", "the", "slow", "red", "turtle" };
	biggie(words, 3);
	return 0;
}