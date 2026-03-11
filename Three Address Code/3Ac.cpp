#include<iostream>
#include<stack>
#include<vector>
#include<unordered_map>
using namespace std;

class threeAC
{
    public:
    string left,o1,o2;
    char op;
    threeAC(string left,string o1,char o, string o2)
    {
        this->left=left;
        this->o1=o1;
        this->o2=o2;
        this->op=o;
    }
};

int priority(char a,char b)
{
    unordered_map<char,int> mp={{'^',10},{'/',8},{'*',8},{'%',8},{'+',5},{'-',5},{'(',1}};
    return mp[a]-mp[b];
}

bool isOp(char ch)
{
    return ch=='+'||ch=='-'||ch=='*'||ch=='^'||ch=='%'||ch=='/';
}


vector<string> postfix(string s)
{
    stack<char> st;

    vector<string> postfix;
    char prev='\0';

    s+="#";

    for(int i=0;i<s.length();i++)
    {
        //cout<<i<<endl;
        char ch=s[i];
        if(ch=='(')
        {
            //cout<<"1";
            st.push(ch);
        }
        else if(isalpha(ch))
        {
            //cout<<"2";
            postfix.push_back(string(1,ch));
        }
        else if(isOp(ch))
        {
            //cout<<"3";
            if(ch=='-' && (isOp(prev)||prev=='\0'))
            {
                postfix.push_back("u-");
            }
            else if(st.empty())
            {
                st.push(ch);
            }
            else if(st.top()=='(')                      
            {
                st.push(ch);        
            }
            else if(priority(ch,st.top())>0)
            {
                st.push(ch);
            }
            else if(priority(ch,st.top())<=0)
            {
                while(!st.empty()&&priority(ch,st.top())<=0&&st.top()!='(')
                {
                    postfix.push_back(string(1,st.top()));
                    st.pop();
                }
                st.push(ch);
            }
        }
        else if(ch==')')
        {
          
            while(st.top()!='(')
            {
                //cout<<"4in"<<endl;
                postfix.push_back(string(1,st.top()));
                st.pop();
            }
            st.pop();                                                
        }
        else if(ch=='#') 
        {
            //cout<<"5";
            while(!st.empty())
            {
                postfix.push_back(string(1,st.top()));
                st.pop();
            }
        }
        prev=ch;
        //cout<<"Stack : "<<st.size()<<endl;
    }

    return postfix;
}


vector<threeAC> EvaluateTo3AC(vector<string> ex,string left)
{
    int mult=0;
    stack<string> st;
    vector<threeAC> arr;
    int tempCtr=1;

    for(auto it:ex)
    {
        if(it=="u-")
        {
            mult=1;
        }
        else
        {
            if(isalpha(it[0]))
            {
                string value=it;
                string temp=it;
                if(mult==1) 
                {
                    value="-"+value;
                    temp="t"+to_string(tempCtr);
                    tempCtr++;
    
                    threeAC obj=threeAC(temp,value,'\0',"");
                    arr.push_back(obj);
                }
                st.push(temp);
            }
            else if(isOp(it[0]))
            {
                string v1=st.top();
                st.pop();
                string v2=st.top();
                st.pop();


                string temp="t"+to_string(tempCtr);
                tempCtr++;

                threeAC obj=threeAC(temp,v2,it[0],v1);
                arr.push_back(obj);
                st.push(temp);
            }
            mult=0;
        }
    }
    threeAC obj=threeAC(left,"t"+to_string(tempCtr),'\0',"");
    arr.push_back(obj);
    return arr;
}

void three_AC(string s)
{
    cout<<"Expression : "<<s<<endl;

    int eqInd=s.find('=');

    string left=s.substr(0,eqInd);
    string right=s.substr(eqInd+1);

    vector<string> ex=postfix(right);
    
    for(auto it:ex)
    {
        cout<<it<<" ";
    }
    cout<<endl;
    vector<threeAC> arr=EvaluateTo3AC(ex,left);
    for(auto it:arr)
    {
        cout<<it.left<<" = "<<it.o1<<" "<<it.op<<" "<<it.o2<<endl;
    }
    cout<<endl;
    //cout<<"HELLO";
}

void Jump3AC(string s)
{
     cout<<"Expression : "<<s<<endl;

}


int main()
{
    three_AC("p=q*-r+q-r+(t*u+v*w)");
    three_AC("x=(m+(n^o) / ((p-q)/(r+(n^o))%s/t))");
}