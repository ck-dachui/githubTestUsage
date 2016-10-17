#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<set>
#include<memory>

using namespace std;

typedef vector<string>::size_type line_no;

class queryResult;
class textQuery{
public:
	queryResult query(string &str);
	textQuery(ifstream&);
private:
	shared_ptr<vector<string>>file;
	map<string,shared_ptr<set<line_no>>>matchedLines;
};
textQuery::textQuery(ifstream &inputFile):file(new vector<string>)
{
	string text;
	while(getline(inputFile,text)){
		string word;
		file->push_back(text);
		int n = file->size() - 1;
		cout<<" n  "<<n<<endl;
		istringstream line(text);
		while(line>>word){
			auto &res = matchedLines[word];
			if(!res)
				res.reset(new set<line_no>);
			res->insert(n);
		}
	}
	/*for(vector<string>::iterator iter = file->begin();iter != file->end();++iter)
		cout<<*iter<<endl;*/
	map<string,shared_ptr<set<line_no>>>::iterator iter = matchedLines.begin();
	while(iter!= matchedLines.end()){
		set<line_no>temp = *(iter->second);
		cout<<iter->first<<"   "<<endl;
		for(auto li : *iter->second)
			cout<<li<<"   ";
		cout<<endl;
		++iter;
	}
}

void print(const queryResult&);
class queryResult{
	friend void print(const queryResult&);
public:
	queryResult(string &str,shared_ptr<vector<string>>fi,shared_ptr<set<line_no>>line):
		soughtString(str),file(fi),lines(line){}
private:
	string soughtString;
	shared_ptr<vector<string>>file;
	shared_ptr<set<line_no>>lines;
};

void print(const queryResult&res)
{
	cout<<res.soughtString<<"   occurs   " <<res.lines->size()
		<<endl;
	for(auto lin : *(res.lines)){
		cout<<"\t(line"<<lin + 1<<"): "<<*(res.file->begin() + lin )<<endl;
	}
}

queryResult textQuery::query(string &str)
{
	shared_ptr<set<line_no>>noData(new set<line_no>());
	auto res = matchedLines.find(str);
	if(res == matchedLines.end())
		return queryResult(str,file,noData);
	else
		return queryResult(str,file,res->second);
}

void runQuery(ifstream & file)
{
	textQuery tq(file);
	string str;
	while(true){
		cout<<"enter query word q to quit"<<endl;
		if(!(cin>>str) || str == "q")break;
		else print(tq.query(str));
	}
}

int main(void)
{
	ifstream file("book.txt");
	runQuery(file);
	return 0;
}