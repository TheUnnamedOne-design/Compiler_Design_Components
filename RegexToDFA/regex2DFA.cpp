#include<iostream>
#include<unordered_set>
#include<unordered_map>
#include<stack>
#include<vector>
using namespace std;

class Node
{
public:
    bool nullable=false;
    unordered_set<int> fp;
    unordered_set<int> lp;
    bool isleaf=false;
    int number=0;
    char ch;
    Node *leftChild=nullptr;
    Node *rightChild=nullptr;
};

unordered_map<int, unordered_set<int>> followpos;

bool isLeaf(Node *n)
{
    return n->leftChild==nullptr && n->rightChild==nullptr;
}

void AssignData(Node *node)
{
    if(node==nullptr) return;

    if(isLeaf(node))
    {
        node->fp.insert(node->number);
        node->lp.insert(node->number);
        node->nullable=false;
        return;
    }

    AssignData(node->leftChild);
    AssignData(node->rightChild);

    if(node->ch=='|')
    {
        node->nullable=node->leftChild->nullable || node->rightChild->nullable;
        node->fp=node->leftChild->fp;
        node->fp.insert(node->rightChild->fp.begin(),node->rightChild->fp.end());
        node->lp=node->leftChild->lp;
        node->lp.insert(node->rightChild->lp.begin(),node->rightChild->lp.end());
    }
    else if(node->ch=='.')
    {
        node->nullable=node->leftChild->nullable && node->rightChild->nullable;

        if(node->leftChild->nullable)
        {
            node->fp=node->leftChild->fp;
            node->fp.insert(node->rightChild->fp.begin(),node->rightChild->fp.end());
        }
        else node->fp=node->leftChild->fp;

        if(node->rightChild->nullable)
        {
            node->lp=node->leftChild->lp;
            node->lp.insert(node->rightChild->lp.begin(),node->rightChild->lp.end());
        }
        else node->lp=node->rightChild->lp;
    }
    else if(node->ch=='*')
    {
        node->nullable=true;
        node->fp=node->leftChild->fp;
        node->lp=node->leftChild->lp;
    }
}

void computeFollowpos(Node* node)
{
    if(node==nullptr) return;

    computeFollowpos(node->leftChild);
    computeFollowpos(node->rightChild);

    if(node->ch=='.')
    {
        for(int i: node->leftChild->lp)
            followpos[i].insert(node->rightChild->fp.begin(),
                                 node->rightChild->fp.end());
    }
    if(node->ch=='*')
    {
        for(int i: node->lp)
            followpos[i].insert(node->fp.begin(),node->fp.end());
    }
}

void printNode(Node *node)
{
    cout<<"Node "<<node->ch<<" "<<node->number<<"\n";
    cout<<"Nullable: "<<node->nullable<<"\n";
    cout<<"Firstpos: { ";
    for(int x: node->fp) cout<<x<<" ";
    cout<<"}\nLastpos: { ";
    for(int x: node->lp) cout<<x<<" ";
    cout<<"}\n\n";
}

void preorder(Node *node)
{
    if(node==nullptr) return;
    printNode(node);
    preorder(node->leftChild);
    preorder(node->rightChild);
}

int precedence(char c)
{
    if(c=='*') return 3;
    if(c=='.') return 2;
    if(c=='|') return 1;
    return 0;
}

string insertConcat(string s)
{
    string r;
    for(int i=0;i<s.length();i++)
    {
        r+=s[i];
        if((s[i]=='a'||s[i]=='b'||s[i]==')'||s[i]=='*'||s[i]=='#') &&
           (i+1<s.length() && (s[i+1]=='a'||s[i+1]=='b'||s[i+1]=='('||s[i+1]=='#')))
            r+='.';
    }
    return r;
}

string infixToPostfix(string infix)
{
    stack<char> st;
    string postfix;

    for(char c: infix)
    {
        if(c=='a'||c=='b'||c=='#')
            postfix+=c;
        else if(c=='(')
            st.push(c);
        else if(c==')')
        {
            while(!st.empty() && st.top()!='(')
            {
                postfix+=st.top();
                st.pop();
            }
            st.pop();
        }
        else
        {
            while(!st.empty() && precedence(st.top())>=precedence(c))
            {
                postfix+=st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while(!st.empty())
    {
        postfix+=st.top();
        st.pop();
    }
    return postfix;
}

Node* makeLeaf(char c,int num)
{
    Node* n=new Node();
    n->ch=c;
    n->number=num;
    n->isleaf=true;
    return n;
}

Node* makeNode(char c,Node* l,Node* r=nullptr)
{
    Node* n=new Node();
    n->ch=c;
    n->leftChild=l;
    n->rightChild=r;
    return n;
}

Node* buildSyntaxTree(string postfix)
{
    stack<Node*> st;
    int pos=1;

    for(char c: postfix)
    {
        if(c=='a'||c=='b'||c=='#')
            st.push(makeLeaf(c,pos++));
        else if(c=='|')
        {
            Node* r=st.top(); st.pop();
            Node* l=st.top(); st.pop();
            st.push(makeNode('|',l,r));
        }
        else if(c=='.')
        {
            Node* r=st.top(); st.pop();
            Node* l=st.top(); st.pop();
            st.push(makeNode('.',l,r));
        }
        else if(c=='*')
        {
            Node* l=st.top(); st.pop();
            st.push(makeNode('*',l));
        }
    }
    return st.top();
}

void printFollowpos()
{
    cout<<"\nFOLLOWPOS\n";
    for(auto &p: followpos)
    {
        cout<<p.first<<" : { ";
        for(int x: p.second) cout<<x<<" ";
        cout<<"}\n";
    }
}

int main()
{
    vector<string> regexes = {
        "(x|z)*(zy|zz)(x|y|z)*(x|z)#"
    };

    for(int i=0;i<regexes.size();i++)
    {
        followpos.clear();

        cout<<"\n=============================\n";
        cout<<"REGEX "<<i+1<<" : "<<regexes[i]<<"\n";
        cout<<"=============================\n";

        string infix = insertConcat(regexes[i]);
        string postfix = infixToPostfix(infix);

        cout<<"Postfix : "<<postfix<<"\n\n";

        Node* root = buildSyntaxTree(postfix);

        AssignData(root);
        computeFollowpos(root);

        preorder(root);
        printFollowpos();
    }
}
