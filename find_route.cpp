#include<iostream>
#include<fstream>
#include<cctype>
#include<map>
#include<iterator>
#include<set>
#include<vector>
#include<iomanip>
using namespace std;

map<float, map<string, string>>arc;  //multi-dimensional map for storing input1.txt
map<string, float> heuristic;       //map for linking cities
int nodes_gen=0, nodes_popped=0, nodes_exp=0, check=0;      //counters
float dist=0.0, heu_val=0.0;
//Node for adding to the fringe
        class node
        {
            public:
                node* next, *prev;
                string name;
                float cumulative_cost,h_cost;
                int depth;

                node(float cumulative_cost=0, int depth = 0, string name="", float h_cost=0)
                {
                    //all information of a node
                    this->cumulative_cost=cumulative_cost;
                    this->h_cost=h_cost;        //heuristic value
                    this->depth=depth;
                    this->name=name;
                    this->next=nullptr;
                    this->prev=nullptr;     //prev pointer to backtrack route
                }
        };
node *root=nullptr;     //root node to access fringe
string sets[1000];      //closed set for storing already expanded nodes
vector<node*> route;    //vector to store route by backtracking 
int i=0;        //counter for closed set
//adding new node to fringe function
void add_node(node* new_node)
{
    nodes_gen++;
    if(root==nullptr)
    {
        root=new_node;
        return;
    }

    node *ptr=root, *temp=root;
    //priority Q
    while(ptr)
    {
        if(check == 1)
        {
        if((new_node->cumulative_cost+new_node->h_cost)<(ptr->cumulative_cost+ptr->h_cost))
        {
            if(ptr==root)
            {
                new_node->next=root;
                root=new_node;
                break;
            }
            new_node->next=ptr;
            temp->next=new_node;
            break;
        }
        }
        if(new_node->cumulative_cost<ptr->cumulative_cost)
        {
            if(ptr==root)
            {
                new_node->next=root;
                root=new_node;
                break;
            }
            new_node->next=ptr;
            temp->next=new_node;
            break;
        }
        if(ptr->next==nullptr)
        {
            ptr->next=new_node;
            break;
        }
        temp=ptr;
        ptr=ptr->next;
    }

}
//function to pop nodes from fringe
void pop_node()
{
    if(root==nullptr)
    {
        return;
    }
    node* ptr=nullptr;
    ptr=root;
    root=root->next;
    nodes_popped++;
}
//function to expand a node
void expand(node *parent)
{
    float cost;
    int depth;
    string name, parent_name;
    parent_name = parent->name;
    map<float, map<string, string>>::iterator itr;
    itr=arc.begin();
    map<string, string>::iterator m;
    m=itr->second.begin();

    // checking if state already visited from closed set
    for(int j=0;i<sizeof(sets)/sizeof(sets[0]);++j)
    {
        if(sets[j]=="")
            break;
        if(sets[j]==parent->name)
        {
            pop_node();
            return;
        }
    }
    sets[i]=parent->name;       //adding city to closed set
    ++i;

    //checking heuristic to expand node
    // if(check == 1)
    // {

    //adding expanded nodes to fringe
    for(itr=arc.begin();itr!=arc.end();itr++)
    {
        for(m=itr->second.begin();m!=itr->second.end();m++)
        {
            if(m->first == parent_name || m->second == parent_name)
            {
                node* new_node=new node();
                new_node->cumulative_cost=parent->cumulative_cost+itr->first;
                new_node->depth=parent->depth+1;
                if(m->first == parent_name)
                    new_node->name=m->second;
                else
                    new_node->name=m->first;
                new_node->next=nullptr;
                new_node->prev=parent;
                if(check == 1)      //adding heuristic from heuristic file to new node
                {
                    map<string,float>::iterator htr;
                    for(htr=heuristic.begin();htr!=heuristic.end();htr++)
                    {
                        if(htr->first==new_node->name)
                        {
                            new_node->h_cost=htr->second;
                        }
                    }
                }
                //cout<<endl<<new_node->name<<endl;
                add_node(new_node);
                
            }
        }
    }
    ++nodes_exp;
    pop_node();
    return;
}

//function for graph search
node* graph_search(string origin_city, string destination_city)
{
    node* new_node=new node(0, 0, origin_city,0);
    if(check == 1)
    {
        map<string,float>::iterator htr;
        for(htr=heuristic.begin();htr!=heuristic.end();htr++)
        {
            if(htr->first==origin_city)
            {
                new_node->h_cost=htr->second;       //adding heuristic to root node
            }
        }
    }
    add_node(new_node);
    heu_val = new_node->h_cost + new_node->cumulative_cost;     //f(n) function

    while(root)
    {
        if(root->name==destination_city)
        {
            dist=root->cumulative_cost;
            node* backtrack=nullptr;
            backtrack=root;
            //backtracking for route
            while(backtrack)
            {
                //cout<<backtrack->name<<" ";           //to display all names of cities visited 
                route.insert(route.begin(),backtrack);
                backtrack=backtrack->prev;
            }
            pop_node();
            return root;
        }
        expand(root);       
    }
    if(root==nullptr)
    {
        dist=-1;
        return root;
    }
    return root;
}

int main(int argc, char* argv[])       //main function to pass arguments
{
    string filename = argv[1];
    if(argc == 4 || argc == 5)      //accept only if 4 or 5 arguments
    {

        fstream fout,h_read;
        //string filename = "D:\\desktop\\prac prog\\AI CSE 5360 004\\Assignment 1\\input1.txt";
        fout.open(filename.c_str());
        string city,city1="",city2="";
        int count=0;  
        while(fout>>city)       //reading input file and saving to map
        {
           // getline(fout,city);
            //cout<<city<<endl; 
            if(city=="END")
                break;
            if(count == 0)
            {
                city1=city;
        }
            if(count == 1)
            {
                city2=city;
            }
            if(count == 2)
            {
                float temp=stof(city);
                arc.insert(make_pair(temp,map<string,string>()));
                arc[temp].insert(make_pair(city1, city2));
                
                count=-1;
            }
            count++;
        }
        fout.close();

        if(argc == 5)
        {
            check=1;
            string temp;
            float h_value;
            string h_file = "D:\\desktop\\prac prog\\AI CSE 5360 004\\Assignment 1\\h_kassel.txt";
            h_read.open(h_file.c_str());
            while(h_read>>city)     //reading heuristic file and saving to map
            {
                if(city == "END")
                    break;
                    temp=city;
                if(h_read>>city)
                {
                    h_value=stof(city);
                    heuristic.insert(pair<string,float>(temp,h_value));
                }
            }
        }
        h_read.close();
        string init_state = argv[2];
        string goal_state = argv[3];
            // string init_state = "Bremen";
            // string goal_state = "Kassel";
   
    root=graph_search(init_state, goal_state);      

    cout<<endl<<"Nodes Popped: "<<nodes_popped<<endl;
    cout<<"Nodes Expanded: "<<nodes_exp<<endl;
    cout<<"Nodes Generated: "<<nodes_gen<<endl;
    if(dist==-1)        //dist=-1 if no route
    {
        cout<<"Distance: Infinity"<<endl;
        cout<<"Route: "<<endl<<"None";  
    }
    else
    {
        cout<<"Distance: "<<fixed<<setprecision(1)<<dist<<" km"<<endl;
        cout<<"Route: ";
    for(int i=0;i<route.size()-1;++i)          //print route
    {
        cout<<route[i]->name<<" to "<<route[i+1]->name<<", "<<fixed<<setprecision(1)<<route[i+1]->cumulative_cost-route[i]->cumulative_cost<<" km"<<endl;
    }
    }
    }

    else{
        cout<<"Enter 4 to 5 arguments only";
        return 0;
    }

    return 0;
}