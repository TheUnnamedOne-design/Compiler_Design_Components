#include<string>
#include<iostream>
#include<cmath>
#include<regex>
using namespace std;


class StateMachine5
{
    public:
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a') return 0;
        else if(state==0&&ch=='b') return 1;
        else if(state==1&&ch=='a') return 0;
        else if(state==1&&ch=='b') return 1;

        return -1;
    }

    bool checker(int ind,int state,string s)
    {
        if(ind==s.length())
        {
            if(state==1)
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};


class StateMachine6
{
    public:
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a') return 1;
        else if(state==0&&ch=='b') return -1;
        else if(state==1&&ch=='a') return 1;
        else if(state==1&&ch=='b') return 1;

        return -1;
    }

    bool checker(int ind,int state,string s)
    {
        if(ind==s.length())
        {
            if(state==1)
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};



class StateMachine7
{
    public:
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a') return 1;
        else if(state==0&&ch=='b') return -1;
        else if(state==1&&ch=='a') return 1;
        else if(state==1&&ch=='b') return 2;
        else if(state==2&&ch=='a') return -1;
        else if(state==2&&ch=='b') return 2;

        return -1;
    }

    bool checker(int ind,int state,string s)
    {
        if(ind==s.length())
        {
            if(state==2)
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};


class StateMachine8
{
    public:
    vector<char> stack;
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a'&&stack.empty())
        {
            stack.push_back('a');
            return 1;
        } 
        else if(state==0) return -1;
        else if(state==1 && stack.empty()) return -1;
        else if(state==1&&ch=='a'&&stack.back()=='a') 
        {
            stack.push_back('a');
            return 1;
        }
        else if(state==1&&ch=='b'&&stack.back()=='a') 
        {
            stack.pop_back();
            return 2;
        }
        else if(state==1) return -1;
        else if(state==2&&stack.empty()) return -1;
        else if(state==2&&stack.back()=='a')
        {
            stack.pop_back();
            return 2;
        }
        return -1;
    }




    bool checker(int ind,int state,string s)
    {
        if(ind==s.length())
        {
            if(state==2 && stack.empty())
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};






    class StateMachine9
    {
    public:
    vector<char> stack;

    bool checker(int ind,int state,string s)
    {
        //cout<<state<<endl;
        if(ind==s.length())
        {
            if(state==1 && stack.empty())
            {
                return true;
            }
            else return false;
        }

        char ch=s[ind];

        if(state==0&&ch=='a'&&stack.empty())
        {
            stack.push_back('a');
            return checker(ind+1,1,s);
        } 
        else if(state==0&&ch=='b'&&stack.empty())
        {
            stack.push_back('b');
            return checker(ind+1,1,s);
        }
        else if(state==0) return false;
        else if(state==1 && stack.empty()) return false;
        else if(state==1&&ch=='a'&&stack.back()=='a') 
        {

            int ns=1;
            vector<char> st_buffer=stack;
            stack.push_back('a');
            bool option1=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            stack.pop_back();
            bool option2=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            bool option3=checker(ind+1,ns,s);
            stack=st_buffer;

            return option1||option2||option3;
        }
        else if(state==1&&ch=='b'&&stack.back()=='a') 
        {
            int ns=1;
            vector<char> st_buffer=stack;
            stack.push_back('b');
            bool option1=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            bool option2=checker(ind+1,ns,s);
            stack=st_buffer;


            return option1||option2;
        }
        else if(state==1&&ch=='a'&&stack.back()=='b') 
        {
            int ns=1;
            vector<char> st_buffer=stack;
            stack.push_back('a');
            bool option1=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            bool option2=checker(ind+1,ns,s);
            stack=st_buffer;


            return option1||option2;
        }
        else if(state==1) return false;
        else if(state==2&&stack.empty()) return false;
        else if(state==2&&ch=='b'&&stack.back()=='b')
        {
            int ns=1;
            vector<char> st_buffer=stack;
            stack.push_back('b');
            bool option1=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            stack.pop_back();
            bool option2=checker(ind+1,ns,s);
            stack=st_buffer;


            st_buffer=stack;
            bool option3=checker(ind+1,ns,s);
            stack=st_buffer;

            return option1||option2||option3;
        }


        return false;
    }
};




    class StateMachine10
    {
    public:

    vector<char> stack1;
    vector<char> stack2;
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a'&&stack1.empty())
        {
            stack1.push_back('a');
            return 1;
        } 
        else if(state==0) return -1;
        else if(state==1 && stack1.empty()) return -1;
        else if(state==1&&ch=='a'&&stack1.back()=='a') 
        {
            stack1.push_back('a');
            return 1;
        }
        else if(state==1&&ch=='b'&&stack1.back()=='a'&&stack2.empty()) 
        {
            stack1.pop_back();
            stack2.push_back('b');
            return 2;
        }
        else if(state==1) return -1;
        else if(state==2&&ch=='b'&&stack1.back()=='a'&&stack2.back()=='b')
        {
            stack1.pop_back();
            stack2.push_back('b');
            return 2;
        }
        else if (state==2&&ch=='c'&&stack1.empty()&&stack2.back()=='b')
        {
            stack2.pop_back();
            return 3;
        }
        else if(state==2) return -1;
        else if(state=3&&stack2.empty()) return -1;
        else if(state==3&&ch=='c'&&stack2.back()=='b')
        {
            stack2.pop_back();
            return 3;
        }
        return -1;
    }


    bool checker(int ind,int state,string s)
    {
        //cout<<state<<endl;
        if(ind==s.length())
        {
            if(state==3 && stack2.empty())
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};



class StateMachine11
{
    public:
    int delta(int state,char ch)
    {
        if(state==0&&ch=='a') return 1;
        else if(state==0&&(ch=='b'||ch=='c')) return -1;
        else if(state==1&&ch=='a') return 1;
        else if(state==1&&ch=='b') return 2;
        else if(state==1&&ch=='c') return -1;
        else if(state==2&&ch=='a') return -1;
        else if(state==2&&ch=='b') return 2;
        else if(state==2&&ch=='c') return 3;
        else if(state==3&&ch=='c') return 3;
        else if(state==3&&(ch=='a'||ch=='b')) return -1;

        return -1;
    }

    bool checker(int ind,int state,string s)
    {
        if(ind==s.length())
        {
            if(state==3)
            {
                return true;
            }
            else return false;
        }
        return checker(ind+1,delta(state,s[ind]),s);
    }
};

void question1()
{
    string s;
    getchar();
    getline(cin,s);
    regex pattern1("([0-9]+)");
    auto begin=sregex_iterator(s.begin(),s.end(),pattern1);
    auto end=sregex_iterator();

    for(auto it=begin;it!=end;it++)
    {
        cout<<it->str()<<" ";
    }
}


void question2()
{
    string s;
    getchar();
    getline(cin,s);
    regex pattern2("(^[A-Z][A-Za-z]*)");
    if(regex_match(s,pattern2))
    {
        cout<<"String is accepted";
    }
    else
    {
        cout<<"String is rejected";

    }
}

void question3()
{
     string s;
    getchar();
    getline(cin,s);
    regex pattern3_1(".*[A-Z]+.*");
    regex pattern3_2(".*[a-z]+.*");
    regex pattern3_3(".*[0-9]+.*");
    regex pattern3_4(".\\;|\\(|\\)|\\{|\\}|\\[|\\]|\\,|\\#\\.|\\=|\\!|\\*|\\+.*");
    if(regex_search(s,pattern3_1)&&regex_search(s,pattern3_2)&&regex_search(s,pattern3_3)&&regex_search(s,pattern3_4))
    {
        cout<<"Password is accepted";
    }
    else
    {
        cout<<"Password is rejected";
    }
}

void question4()
{
    string s;
    getchar();
    getline(cin,s);
    regex pattern4("(#)+");
    auto begin=sregex_iterator(s.begin(),s.end(),pattern4);
    auto end=sregex_iterator();

    for(auto it=begin;it!=end;it++)
    {
        cout<<it->str()<<" ";
    }
}
void question5()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine5 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";

}
void question6()
{   
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine6 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}
void question7()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine7 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}
void question8()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine8 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}
void question9()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine9 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}
void question10()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine10 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}
void question11()
{
    string s;
    getchar();
    getline(cin,s);

    int state=0;

    StateMachine11 obj;
    if(obj.checker(0,state,s)) cout<<"Accepted";
    else cout<<"Rejected";
}


int main()
{
    cout<<"Enter your choice : ";
    int choice;
    cin>>choice;

    switch(choice)
    {
        case 1:
        {
            question1();
            break;
        }
        case 2:
        {
            question2();
            break;
        }
        case 3:
        {
            question3();
            break;
        }
        case 4:
        {
            question4();
            break;
        }
        case 5:
        {
            question5();
            break;
        }
        case 6:
        {
            question6();
            break;
        }
        case 7:
        {
            question7();
            break;
        }
        case 8:
        {
            question8();
            break;
        }
        case 9:
        {
            question9();
            break;
        }
        case 10:
        {
            question10();
            break;
        }
        case 11:
        {
            question11();
            break;
        }
        default:
        {
            cout<<"Invalid option";
            break;
        }
    }

}