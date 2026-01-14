#include<iostream>
#include<cmath>
#include<regex>
#include<unordered_map>
#include<vector>
#include<iterator>
#include<unordered_set>
using namespace std;


unordered_set<string> terminals={"+","(",")","id","*","null"};
unordered_set<string> non_terminals={"E","T","F"};
string start="E";

vector<string> splitter(string pattern,string s)
{
    regex delimiter(pattern);
    sregex_token_iterator start(s.begin(),s.end(),delimiter,-1);
    sregex_token_iterator last;

    vector<string> entities(start,last);
    return entities;
}

unordered_map<string,vector<string>> extract_productions(string &s)
{
    string pattern1=",\\s*";
    vector<string> entities=splitter(pattern1,s);
    unordered_map<string,vector<string>> mp;

    for(auto it:entities)
    {
        string pattern2="->";
        vector<string> hold=splitter(pattern2,it);
        
        string character=hold[0];
        string prods=hold[1];

        string pattern3="/";
        vector<string> outs=splitter(pattern3,prods);

        mp[character]=outs;
    }

    return mp;
}


void eliminate_left_recursion(unordered_map<string,vector<string>> &mp)
{
    for(auto it:mp)
    {
        string character=it.first;
        vector<string> productions=it.second;
        //cout<<character<<" "<<productions[0]<<endl;
        if(character[0]==productions[0][0])
        {
            //cout<<character<<" left recursive";

            string first_production=productions[0];
            string second_production=productions[1];
            string character2=character+"'";
            string p1=second_production+character2;

            vector<string> new_productions;

            //cout<<"first "<<first_production;
            new_productions.push_back(first_production.substr(1)+character2);
            new_productions.push_back("null");

            mp[character2]=new_productions;
            (mp[character])[0]=p1;
            (mp[character]).pop_back();
        }
    }
}


unordered_set<string> first(unordered_set<string> terminals,unordered_set<string> non_terminals,string character,unordered_map<string,vector<string>> mp)
{
    vector<string> prods=mp[character];
    unordered_set<string> first_set;
    for(auto it:prods)
    {
        int terminal=0;
        for(auto it2:terminals)
        {
            if(it.find(it2)==0)
            {
                first_set.insert(it2);
                terminal=1;
                break;
            }
        }
        if(terminal==0)
        {
            //cout<<"here "<<string(1, it[0])<<" space "<<it<<endl;
            unordered_set<string> holder=first(terminals,non_terminals,string(1, it[0])+"",mp);
            first_set.insert(holder.begin(),holder.end());
        }
    }
    return first_set;
}


unordered_map<string,unordered_set<string>> first_sets(unordered_map<string,vector<string>> &mp)
{
    
    unordered_map<string,unordered_set<string>> first_map;
    for(auto it:mp)
    {
        string character=it.first;
        vector<string> productions=it.second;
        first_map[character]=first(terminals,non_terminals,character,mp);
    }
    return first_map;
}


unordered_set<string> follow(unordered_set<string> terminals,unordered_set<string> non_terminals,string character,unordered_map<string,vector<string>> mp, unordered_map<string,unordered_set<string>>& memo, unordered_map<string,unordered_set<string>>& first_map)
{
    
    if(memo.find(character) != memo.end())
    {
        return memo[character];
    }
    
    unordered_set<string> follow_set;
    
    
    if(character == start)
    {
        follow_set.insert("$");
    }
    
    
    for(auto& prod_entry : mp)
    {
        string lhs = prod_entry.first;
        vector<string> productions = prod_entry.second;
        
        for(auto& production : productions)
        {
            
            size_t pos = 0;
            while(pos < production.length())
            {
                
                bool found = false;
                if(pos < production.length() && production.substr(pos, character.length()) == character)
                {
                    
                    if(pos + character.length() == production.length() || 
                       non_terminals.find(string(1, production[pos + character.length()])) != non_terminals.end() ||
                       terminals.find(string(1, production[pos + character.length()])) != terminals.end())
                    {
                        found = true;
                    }
                }
                
                if(found)
                {
                    
                    string beta = production.substr(pos + character.length());
                    
                    if(beta.empty())
                    {
                        
                        if(lhs != character) 
                        {
                            unordered_set<string> follow_lhs = follow(terminals, non_terminals, lhs, mp, memo, first_map);
                            follow_set.insert(follow_lhs.begin(), follow_lhs.end());
                        }
                    }
                    else
                    {
                       
                        string beta_first_symbol = "";
                        
                        
                        bool beta_is_terminal = false;
                        for(auto& term : terminals)
                        {
                            if(beta.find(term) == 0)
                            {
                                beta_first_symbol = term;
                                beta_is_terminal = true;
                                break;
                            }
                        }
                        
                        
                        if(!beta_is_terminal)
                        {
                            beta_first_symbol = string(1, beta[0]);
                        }
                        
                        
                        if(first_map.find(beta_first_symbol) != first_map.end())
                        {
                            unordered_set<string> first_beta = first_map[beta_first_symbol];
                            bool has_null = false;
                            
                            for(auto& sym : first_beta)
                            {
                                if(sym != "null")
                                {
                                    follow_set.insert(sym);
                                }
                                else
                                {
                                    has_null = true;
                                }
                            }
                            
                            
                            if(has_null && lhs != character)
                            {
                                unordered_set<string> follow_lhs = follow(terminals, non_terminals, lhs, mp, memo, first_map);
                                follow_set.insert(follow_lhs.begin(), follow_lhs.end());
                            }
                        }
                        else if(beta_is_terminal)
                        {
                            follow_set.insert(beta_first_symbol);
                        }
                    }
                    
                    pos += character.length();
                }
                else
                {
                    pos++;
                }
            }
        }
    }
    
    memo[character] = follow_set;
    return follow_set;
}



unordered_map<string,unordered_set<string>> follow_sets(unordered_map<string,vector<string>> &mp)
{
    
    unordered_map<string,unordered_set<string>> first_map = first_sets(mp);
    
    unordered_map<string,unordered_set<string>> follow_map;
    unordered_map<string,unordered_set<string>> memo;
    
    for(auto it:mp)
    {
        string character=it.first;
        follow_map[character]=follow(terminals,non_terminals,character,mp,memo,first_map);
    }
    return follow_map;
}



int main()
{
    string productions;
    //cout<<"Input productions in comma seperated form : ";
    //getline(cin,productions);
    //productions="E->E+T/T,T->T*F/F,F->(E)/id";
    //productions="S->(L)/a,L->L.S/S";

    //cout<<"Input string : ";
    //string s;
    //getline(cin,s);


    //unordered_map<string,vector<string>> production_map=extract_productions(productions);
    unordered_map<string,vector<string>> production_map;
    production_map["E"]={"E+T","T"};
    production_map["T"]={"T*F","F"};
    production_map["F"]={"(E)","id"};
    for(auto &it:production_map)
    {
        cout<<it.first<<" -> ";
        for(auto &it2:it.second)
        {
            cout<<it2<<" / ";
        }
        cout<<endl;
    }


    eliminate_left_recursion(production_map);
    map<string,vector<string>> production_map2(production_map.begin(),production_map.end());
    cout<<endl;
    cout<<"eliminated left recursion"<<endl;
    for(auto &it:production_map2)
    {
        cout<<it.first<<" -> ";
        for(auto &it2:it.second)
        {
            cout<<it2<<" / ";
        }
        cout<<endl;
    }

    unordered_map<string,unordered_set<string>> first_sets_productions=first_sets(production_map);
    cout<<endl;

    for(auto &it:first_sets_productions)
    {
        cout<<"first of "<<it.first<<" = ";
        for(auto it2:it.second)
        {
            cout<<it2<<" ";
        }
        cout<<endl;
    }
    
    unordered_map<string,unordered_set<string>> follow_sets_productions=follow_sets(production_map);
    cout<<endl;

    for(auto &it:follow_sets_productions)
    {
        cout<<"follow of "<<it.first<<" = ";
        for(auto it2:it.second)
        {
            cout<<it2<<" ";
        }
        cout<<endl;
    }
    
}