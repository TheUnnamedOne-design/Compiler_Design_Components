#include<iostream>
#include<cmath>
#include<regex>
#include<unordered_map>
#include<map>
#include<vector>
#include<iterator>
#include<unordered_set>
#include<iomanip>
#include<set>
#include<stack>
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


unordered_map<string,unordered_set<string>> follow_sets(unordered_map<string,vector<string>> &mp)
{
    
    unordered_map<string,unordered_set<string>> first_map = first_sets(mp);
    
    unordered_map<string,unordered_set<string>> follow_map;
    
    for(auto& nt : non_terminals)
    {
        follow_map[nt] = unordered_set<string>();
    }
    
    follow_map[start].insert("$");
    
    bool changed = true;
    while(changed)
    {
        changed = false;
        
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
                    size_t match_len = 0;
                    string matched_nt = "";
                    
                    for(auto& nt : non_terminals)
                    {
                        if(pos + nt.length() <= production.length() && production.substr(pos, nt.length()) == nt)
                        {
                            found = true;
                            match_len = nt.length();
                            matched_nt = nt;
                            break;
                        }
                    }
                    
                    if(found)
                    {
                        string beta = production.substr(pos + match_len);
                        
                        if(beta.empty())
                        {
                            size_t old_size = follow_map[matched_nt].size();
                            follow_map[matched_nt].insert(follow_map[lhs].begin(), follow_map[lhs].end());
                            if(follow_map[matched_nt].size() > old_size)
                            {
                                changed = true;
                            }
                        }
                        else
                        {
                            bool all_nullable = true;
                            size_t beta_pos = 0;
                            
                            while(beta_pos < beta.length())
                            {
                                string beta_symbol = "";
                                bool beta_is_terminal = false;
                                
                                for(auto& term : terminals)
                                {
                                    if(beta_pos + term.length() <= beta.length() && beta.substr(beta_pos, term.length()) == term)
                                    {
                                        beta_symbol = term;
                                        beta_is_terminal = true;
                                        break;
                                    }
                                }
                                
                                if(!beta_is_terminal)
                                {
                                    for(auto& nt : non_terminals)
                                    {
                                        if(beta_pos + nt.length() <= beta.length() && beta.substr(beta_pos, nt.length()) == nt)
                                        {
                                            beta_symbol = nt;
                                            break;
                                        }
                                    }
                                }
                                
                                if(beta_symbol.empty())
                                {
                                    break;
                                }
                                
                                if(beta_is_terminal)
                                {
                                    if(follow_map[matched_nt].find(beta_symbol) == follow_map[matched_nt].end())
                                    {
                                        follow_map[matched_nt].insert(beta_symbol);
                                        changed = true;
                                    }
                                    all_nullable = false;
                                    break;
                                }
                                else
                                {
                                    if(first_map.find(beta_symbol) != first_map.end())
                                    {
                                        unordered_set<string> first_beta = first_map[beta_symbol];
                                        bool has_null = false;
                                        
                                        for(auto& sym : first_beta)
                                        {
                                            if(sym != "null")
                                            {
                                                if(follow_map[matched_nt].find(sym) == follow_map[matched_nt].end())
                                                {
                                                    follow_map[matched_nt].insert(sym);
                                                    changed = true;
                                                }
                                            }
                                            else
                                            {
                                                has_null = true;
                                            }
                                        }
                                        
                                        if(!has_null)
                                        {
                                            all_nullable = false;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        all_nullable = false;
                                        break;
                                    }
                                }
                                
                                beta_pos += beta_symbol.length();
                            }
                            
                            if(all_nullable)
                            {
                                size_t old_size = follow_map[matched_nt].size();
                                follow_map[matched_nt].insert(follow_map[lhs].begin(), follow_map[lhs].end());
                                if(follow_map[matched_nt].size() > old_size)
                                {
                                    changed = true;
                                }
                            }
                        }
                        
                        pos += match_len;
                    }
                    else
                    {
                        pos++;
                    }
                }
            }
        }
    }
    
    return follow_map;
}

map<pair<string,string>,string> create_parsing_table(unordered_map<string,vector<string>> &mp, unordered_map<string,unordered_set<string>> &first_map, unordered_map<string,unordered_set<string>> &follow_map)
{
    map<pair<string,string>,string> parse_table;
    
    for(auto& prod_entry : mp)
    {
        string non_term = prod_entry.first;
        vector<string> productions = prod_entry.second;
        
        for(auto& production : productions)
        {
            unordered_set<string> first_prod;
            
            if(production == "null")
            {
                first_prod.insert("null");
            }
            else
            {
                size_t prod_pos = 0;
                bool found_first = false;
                
                while(prod_pos < production.length() && !found_first)
                {
                    string prod_symbol = "";
                    bool is_terminal = false;
                    
                    for(auto& term : terminals)
                    {
                        if(prod_pos + term.length() <= production.length() && production.substr(prod_pos, term.length()) == term)
                        {
                            prod_symbol = term;
                            is_terminal = true;
                            break;
                        }
                    }
                    
                    if(!is_terminal)
                    {
                        for(auto& nt : non_terminals)
                        {
                            if(prod_pos + nt.length() <= production.length() && production.substr(prod_pos, nt.length()) == nt)
                            {
                                prod_symbol = nt;
                                break;
                            }
                        }
                    }
                    
                    if(prod_symbol.empty())
                    {
                        break;
                    }
                    
                    if(is_terminal)
                    {
                        first_prod.insert(prod_symbol);
                        found_first = true;
                    }
                    else
                    {
                        if(first_map.find(prod_symbol) != first_map.end())
                        {
                            bool has_null_in_first = false;
                            for(auto& sym : first_map[prod_symbol])
                            {
                                if(sym != "null")
                                {
                                    first_prod.insert(sym);
                                }
                                else
                                {
                                    has_null_in_first = true;
                                }
                            }
                            
                            if(!has_null_in_first)
                            {
                                found_first = true;
                            }
                        }
                        else
                        {
                            found_first = true;
                        }
                    }
                    
                    prod_pos += prod_symbol.length();
                }
                
                if(!found_first)
                {
                    first_prod.insert("null");
                }
            }
            
            bool has_null = false;
            for(auto& symbol : first_prod)
            {
                if(symbol != "null")
                {
                    parse_table[{non_term, symbol}] = production;
                }
                else
                {
                    has_null = true;
                }
            }
            
            if(has_null)
            {
                if(follow_map.find(non_term) != follow_map.end())
                {
                    for(auto& follow_symbol : follow_map[non_term])
                    {
                        parse_table[{non_term, follow_symbol}] = production;
                    }
                }
            }
        }
    }
    
    return parse_table;
}

void display_parsing_table(map<pair<string,string>,string> &parse_table, unordered_map<string,vector<string>> &mp)
{
    set<string> all_terminals;
    for(auto& term : terminals)
    {
        if(term != "null")
        {
            all_terminals.insert(term);
        }
    }
    all_terminals.insert("$");
    
    vector<string> ordered_non_terminals;
    map<string,vector<string>> ordered_map(mp.begin(), mp.end());
    for(auto& entry : ordered_map)
    {
        ordered_non_terminals.push_back(entry.first);
    }
    
    cout << setw(10) << "";
    for(auto& term : all_terminals)
    {
        cout << setw(15) << term;
    }
    cout << endl;
    
    cout << string(10 + all_terminals.size() * 15, '-') << endl;
    
    for(auto& non_term : ordered_non_terminals)
    {
        cout << setw(10) << non_term;
        for(auto& term : all_terminals)
        {
            if(parse_table.find({non_term, term}) != parse_table.end())
            {
                string production = non_term + "->" + parse_table[{non_term, term}];
                cout << setw(15) << production;
            }
            else
            {
                cout << setw(15) << "";
            }
        }
        cout << endl;
    }
}

vector<string> tokenize_input(string input)
{
    vector<string> tokens;
    string current = "";
    
    for(int i = 0; i < input.length(); i++)
    {
        string ch(1, input[i]);
        
        if(terminals.find(ch) != terminals.end())
        {
            if(!current.empty())
            {
                tokens.push_back(current);
                current = "";
            }
            tokens.push_back(ch);
        }
        else if(input[i] != ' ')
        {
            current += input[i];
        }
        else
        {
            if(!current.empty())
            {
                tokens.push_back(current);
                current = "";
            }
        }
    }
    
    if(!current.empty())
    {
        tokens.push_back(current);
    }
    
    return tokens;
}

bool parse_string(string input, map<pair<string,string>,string> &parse_table, unordered_map<string,vector<string>> &mp)
{
    vector<string> tokens = tokenize_input(input);
    tokens.push_back("$");
    
    stack<string> parse_stack;
    parse_stack.push("$");
    parse_stack.push(start);
    
    int token_index = 0;
    
    cout << endl << "Parsing Steps:" << endl;
    cout << setw(20) << "Stack" << " | " << setw(20) << "Input" << " | " << setw(30) << "Action" << endl;
    cout << string(80, '-') << endl;
    
    while(!parse_stack.empty())
    {
        string stack_display = "";
        stack<string> temp_stack = parse_stack;
        vector<string> stack_elements;
        while(!temp_stack.empty())
        {
            stack_elements.push_back(temp_stack.top());
            temp_stack.pop();
        }
        for(int i = stack_elements.size() - 1; i >= 0; i--)
        {
            stack_display += stack_elements[i];
        }
        
        string input_display = "";
        for(int i = token_index; i < tokens.size(); i++)
        {
            input_display += tokens[i];
        }
        
        string top = parse_stack.top();
        string current_token = tokens[token_index];
        
        if(top == "$" && current_token == "$")
        {
            cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << "Accept" << endl;
            return true;
        }
        else if(terminals.find(top) != terminals.end() || top == "$")
        {
            if(top == current_token)
            {
                cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << "Match " + top << endl;
                parse_stack.pop();
                token_index++;
            }
            else
            {
                cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << "Error: Mismatch" << endl;
                return false;
            }
        }
        else if(non_terminals.find(top) != non_terminals.end() || non_terminals.find(string(1, top[0])) != non_terminals.end())
        {
            if(parse_table.find({top, current_token}) != parse_table.end())
            {
                string production = parse_table[{top, current_token}];
                cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << top + "->" + production << endl;
                
                parse_stack.pop();
                
                if(production != "null")
                {
                    for(int i = production.length() - 1; i >= 0; i--)
                    {
                        bool found = false;
                        
                        for(auto& non_term : non_terminals)
                        {
                            if(i >= non_term.length() - 1)
                            {
                                string substr = production.substr(i - non_term.length() + 1, non_term.length());
                                if(substr == non_term)
                                {
                                    parse_stack.push(non_term);
                                    i -= (non_term.length() - 1);
                                    found = true;
                                    break;
                                }
                            }
                        }
                        
                        if(!found)
                        {
                            for(auto& term : terminals)
                            {
                                if(i >= term.length() - 1)
                                {
                                    string substr = production.substr(i - term.length() + 1, term.length());
                                    if(substr == term)
                                    {
                                        parse_stack.push(term);
                                        i -= (term.length() - 1);
                                        found = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << "Error: No production" << endl;
                return false;
            }
        }
        else
        {
            cout << setw(20) << stack_display << " | " << setw(20) << input_display << " | " << setw(30) << "Error: Invalid symbol" << endl;
            return false;
        }
    }
    
    return false;
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
    
    for(auto &it:production_map)
    {
        non_terminals.insert(it.first);
    }
    
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
    
    map<pair<string,string>,string> parsing_table = create_parsing_table(production_map, first_sets_productions, follow_sets_productions);
    cout<<endl;
    cout<<"Parsing Table:"<<endl;
    display_parsing_table(parsing_table, production_map);
    
    cout<<endl;
    string input_string;
    cout<<"Enter input string to parse: ";
    getline(cin, input_string);
    
    bool result = parse_string(input_string, parsing_table, production_map);
    
    cout<<endl;
    if(result)
    {
        cout<<"String is ACCEPTED"<<endl;
    }
    else
    {
        cout<<"String is REJECTED"<<endl;
    }
    
}