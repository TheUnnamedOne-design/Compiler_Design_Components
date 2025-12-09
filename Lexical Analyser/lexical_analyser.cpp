# include <iostream>
# include <unordered_map>
# include <unordered_set>
# include <vector>
# include <fstream>
#include<regex>
# include <cstring>
using namespace std;

int isNumber(string hold)
{
    int i;
    for(i=0;i<hold.length();i++)
    {
        if(!(isdigit(hold[i]))) return 0;
    }
    return 1;
}

vector<string> token_extractor(string s)
{
   vector<string> tokens;

   unordered_set<string> keywords={"int","float","long","string","while","if","for","void","using","include","return","main","cout","namespace"};
   string pattern1="";
   for(auto it=keywords.begin();it!=keywords.end();it++)
   {
    if(!pattern1.empty()) pattern1+="|";
        pattern1+=*it;
   }
   string pattern2="[A-Za-z_][A-Za-z0-9_]*";
   string pattern3="[0-9]+";
   string pattern4="<<|>>|==|<=|>=|=|\\+|\\-|\\*|/";
   string pattern5=";|\\(|\\)|\\{|\\}";
   string pattern6="\"[^\"]*\"";

   regex token_pattern("("+pattern1+")"
                    "|("+pattern2+")"
                    "|("+pattern3+")"
                    "|("+pattern4+")"
                    "|("+pattern5+")"
                    "|("+pattern6+")");

    auto begin=sregex_iterator(s.begin(),s.end(),token_pattern);
    auto end=sregex_iterator();

     for (auto it = begin; it != end; it++) {
        tokens.push_back(it->str());
    }

    return tokens;

   
}

void classifier(unordered_map<string,string> &mp, string hold)
{
    unordered_set<string> keywords = {
        "int","float","long","string","while","if","for","void",
        "using","include","return","main","cout","namespace"
    };

    unordered_set<string> operators = {
        "+","-","*","/","^","<<",">>","==","<=",">=","="
    };

    unordered_set<string> braces = {"{","}"};
    unordered_set<string> parentheses = {"(",")"};
    unordered_set<string> delimiters = {";"}; 
  


    if (mp.find(hold) != mp.end()) return;


    if (keywords.find(hold) != keywords.end()) {
        mp[hold] = "keyword";
    }
    else if (operators.find(hold) != operators.end()) {
        mp[hold] = "operator";
    }
    else if (braces.find(hold) != braces.end()) {
        mp[hold] = "brace";
    }
    else if (parentheses.find(hold) != parentheses.end()) {
        mp[hold] = "parenthesis";
    }
    else if (delimiters.find(hold) != delimiters.end()) {
        mp[hold] = "delimiter";
    }
    else if (isNumber(hold)) {
        mp[hold] = "number";
    }
    else if (hold.size() >= 2 && hold.front()=='"' && hold.back()=='"') {
        mp[hold] = "string_literal";
    }
    else {
        mp[hold] = "identifier";
    }
}


void lexical_analyser(string s)
{
    int i;
    ifstream inputFile(s);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    string line;
    string complete;
    while (getline(inputFile, line)) {
        complete+=line+" ";
    }
    inputFile.close();

    unordered_map<string,string> mp;


    vector<string> tokens = token_extractor(complete);
        for (string &tok : tokens) {
            classifier(mp, tok);
        }

    for(auto it=mp.begin();it!=mp.end();it++)
    {
        string s1=it->first;
        string s2=it->second;

        cout<<s1<<" "<<s2<<endl;

    }

}

int main()
{
    string s;
    s="prog.txt";
    lexical_analyser(s);
}