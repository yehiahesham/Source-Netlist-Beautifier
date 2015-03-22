#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include<vector>
#include <sstream>
using namespace std;
struct cell
{
	string node;
	string type;
	int level;
	vector<string> edges;
};
class Graph
{
	int V;    // No. of vertices
	void topologicalSortUtil(int s, int v, vector<cell> &adj, stack<string> &Stack); //part with makes the DFS
public:
	Graph(int v); //construcor
	vector<cell> adj; //this is a vector of a struct which will carry each node's info : outwards edges , name, type, etc
	//bool myfunction(cell i, cell j);
	//void sort();
	void addEdge(string v, string t, string w); //to add a directed edge
	int get_index(string v, vector<cell> adj); //to get a specific index of a node since there is no find dunction in vectors
	void topologicalSort(); //this function loops on all the nodes and do DFS (using topological logic)to get the correct level
};

Graph::Graph(int v = 200) :V(v)
{

}

void Graph::addEdge(string v, string t, string w)
{

	bool s = 0; // this for w , if found
	bool s2 = 0; // this for v, if found

	if (adj.size() != 0) // if it is the first 2/1 nodes
	{
		for (int j = 0; j < adj.size(); j++)
		{
			if (adj[j].node == v)
			{
				s2 = 1; 
				if (adj[j].type == "!" || adj[j].type == "") //since sometimes inside the main function we pass portions of the nodes via another one. so we solve that by implementing this
					adj[j].type = t;
				if (w != "") // if it is a wire, input  or output
				{
					adj[j].edges.push_back(w); 
					
					for (int x = 0; x < adj.size(); x++)
					{
						if (adj[x].node == w) 
						{
							s = 1;
							break;
						}
					}
				}
				else
					s = 1;
				if (s == 0)
				{
					cell temp; temp.node = w; temp.type = t; adj.push_back(temp);
				}

			}
			else if (adj[j].node == w) s = 1;

			if (s2 == 1) break;


		}
		if (!s2)
		{
			cell temp;
			temp.node = v;
			if (w != "")
				temp.edges.push_back(w);
			temp.type = t;
			adj.push_back(temp);
			if (!s && t != "wire" && t != "input" && t != "output") { cell temp2; temp2.node = w; adj.push_back(temp2); }
		}
	}
	else // if it the first two/one input
	{
		cell temp;
		temp.node = v;
		temp.type = t;
		adj.push_back(temp);
		if (t != "wire" &&  t != "input")
		{
			cell temp2;
			temp2.node = w;
			temp.edges.push_back(w);
			adj.push_back(temp2);
		}
	}
}

int Graph::get_index(string v, vector<cell> adj)
{
	//to get the v index in the vecotr adj
	for (int u = 0; u < adj.size(); u++)
	if (v == adj[u].node)
		return u;
}

void Graph::topologicalSortUtil(int s, int v, vector<cell> &adj, stack<string> &Stack)
{
	// Mark the current node as visited.
	//adj[v].visted = true;
	// i removed the visted bool, cause we need a better calulation for the levels. we need to access the nodes with different parents and choose the highest level value

	if (adj[v].type == "wire")
		adj[v].level = adj[s].level; // same level value if it was a wire
	else
	{
		if (adj[v].type != "input")
		{
			int u = adj[s].level + 1; // if it isn;t a wire nor a  input, increase it level value
			if (u > adj[v].level) // if the old value is less than that of calulated 
				adj[v].level = u; //replace it
		}
	}

	// Recur for all the vertices adjacent to this vertex

	for (int j = 0; j < adj[v].edges.size(); j++)
	{
		int x = get_index(adj[v].edges.at(j), adj);
		topologicalSortUtil(v, x, adj, Stack);
	}
	// Push current vertex to stack which stores result
	Stack.push(adj[v].node);


}
void Graph::topologicalSort()
{
	stack<string> Stack;

	for (int i = 0; i < adj.size(); i++)
		topologicalSortUtil(0, i, adj, Stack);

	cout << endl;

	//JSON file generation (manual, without using libraries)
	//___________PLZ TAKE CARE OF THE LOCATION_____________//
	ofstream jsonout("C:\\Users\\yehia\\Desktop\\booth.json");                   //destination file to export JSON data to
	vector<string> temp;
	jsonout << "{" << endl;

	//////////////////////////////////////////////////////////////////////Inputs
	jsonout << " \"inputs\": {" << endl;

	for (int u = 0; u < adj.size(); u++)
	{
		if (adj[u].type == "input")
		{
			temp.push_back(adj[u].node);
		}
	}
	for (int u = 0; u < temp.size() - 1; u++)
	{
		jsonout << "  \"" << temp[u] << "\"," << endl;
	}
	jsonout << "  \"" << temp[temp.size() - 1] << "\"" << endl;
	jsonout << " }," << endl;

	//////////////////////////////////////////////////////////////////////Outputs
	vector<string>temp2;
	vector<int> templevel;
	jsonout << " \"outputs\": [" << endl;
	for (int u = 0; u < adj.size(); u++)
	{
		if (adj[u].type == "output")
		{
			temp2.push_back(adj[u].node);
			templevel.push_back(adj[u].level);
		}
	}
	for (int u = 0; u < temp2.size() - 1; u++)
	{
		jsonout << "  \{" << endl;
		jsonout << "   \"name\":   \"" << temp2[u] << "\"," << endl;
		jsonout << "   \"level\":   " << templevel[u] << endl;
		jsonout << "  \}," << endl;
	}
	jsonout << "  \{" << endl;
	jsonout << "   \"name\":   \"" << temp2[temp2.size() - 1] << "\"," << endl;
	jsonout << "   \"level\":   " << templevel[temp2.size() - 1] << endl;
	jsonout << "  \}," << endl;

	jsonout << " ]" << endl;

	//////////////////////////////////////////////////////////////////////Modules
	vector<string>temp3;
	vector<string>temptype;
	vector<int>templevel2;
	jsonout << " \"modules\": [" << endl;
	for (int u = 0; u < adj.size(); u++)
	{
		if ((adj[u].type != "output") && (adj[u].type != "input") && (adj[u].type != "wire"))
		{
			temp3.push_back(adj[u].node);
			templevel2.push_back(adj[u].level);
			temptype.push_back(adj[u].type);
		}
	}
	for (int u = 0; u < temp3.size() - 1; u++)
	{
		jsonout << "  \{" << endl;
		jsonout << "   \"type\":   \"" << temptype[u] << "\"," << endl;
		jsonout << "   \"name\":   \"" << temp3[u] << "\"," << endl;
		jsonout << "   \"level\":   " << templevel2[u] << "," << endl;
		jsonout << "  \}," << endl;
	}
	jsonout << "  \{" << endl;
	jsonout << "   \"type\":   \"" << temptype[temp3.size() - 1] << "\"," << endl;
	jsonout << "   \"name\":   \"" << temp3[temp3.size() - 1] << "\"," << endl;
	jsonout << "   \"level\":   " << templevel2[temp3.size() - 1] << endl;
	jsonout << "  \}," << endl;
	jsonout << " ]" << endl;

	jsonout << "}" << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct module{
	string name;
	string type;
	vector<string> inputs;
	string output;
};
string convert_to_string(int  i)
{ // convert from int to string
	stringstream out;
	out << i;
	return (out.str());
}
void main()
{

	vector<int>wires;
	ifstream myfile;
	//___________PLZ TAKE CARE OF THE LOCATION_____________//
	myfile.open("C:\\Users\\Ahmad Adel\\Desktop\\3input.g.v");              //source file to retrieve netlist from
	string line;
	string line2 = "NOR2X4 U111(.A(n118), .B(n172), .Y(n116));";
	smatch m;

	//regular expressions for different line formats

	regex modulename("\\s*([[:w:]]+)\\s+([[:w:]]+)\\s+\\(");
	regex mline1("\\s*\\.([[:w:]]+)\\(([[:w:]]+\\[[[:d:]]+\\])\\s*\\)\\s*,\\s*"); //input with [] and with comma
	regex mline11("\\s*\\.([[:w:]]+)\\(([[:w:]]+)\\s*\\)\\s*,\\s*"); //correct ,input without  [] and with comma
	regex mline42("\\s*\\.([[:w:]]+)\\(([[:w:]]+\\[[[:d:]]+\\])\\s*\\)\\s*"); //output with []
	regex mline442("\\s*\\.([[:w:]]+)\\(([[:w:]]+)\\s*\\)\\s*"); //output without  []
	regex wire2("\\s*wire\\s+([[:w:]]+)\\s*\\;"); //WIRE without []
	regex wire("\\s*wire\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;"); //WIRE with []
	regex input("\\s*input\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;"); //input with []
	regex input2("\\s*input\\s+([[:w:]]+)\\s*\\;"); //input without []
	regex output("\\s*output\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;"); //output with []
	regex output2("\\s*output\\s+([[:w:]]+)\\s*\\;"); //output without []
	regex assign("\\s*assign\\s+([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\=\\s*([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\;"); // assign x[d]=y[d]
	regex assign2("\\s*assign\\s+([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*\\=\\s*([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\;"); // assign x[d:d]=y[d:d]
	regex assign3("\\s*assign\\s+([[:w:]]+)\\s*\\=\\s*([[:w:]]+)\\s*\\;"); //assign x=y
	regex endofnetlist("\\s*\\)\\;"); // to check the end of module

	vector<module> nodes;

	//start reading file
	if (!myfile.fail())
	{
		while (!myfile.eof())
		{
			getline(myfile, line);
			if (regex_match(line, m, modulename))          //if line is module type and name, ex. "NOR2X2 _2_ ("
			{
				int incount;
				if (m.str(1) == "INVX1")
					incount = 1;
				else incount = 2;
				module temp;
				temp.name = m.str(2);
				temp.type = m.str(1);
				cout << "Name: " << temp.name << endl;

				///////////////////////////////////////////////////////////////////////////////////////////////////
				//inserting inputs and output
				bool o = false;
				while (!o)
				{
					getline(myfile, line);
					cout << "Line: " << line << endl;
					if (regex_match(line, m, mline1)) //.A(md[0]),
					{//input
						temp.inputs.push_back(m.str(2));
						cout << "Input: " << temp.inputs.back() << endl;
					}
					else if (regex_match(line, m, mline11)) //.A(md),
					{//input
						temp.inputs.push_back(m.str(2));
						cout << "Input: " << temp.inputs.back() << endl;
					}
					else if (regex_match(line, m, mline42)) //.A(md[0]) //for output
					{
						temp.output = m.str(2);
						cout << "Output: " << temp.output << endl << "----------------------\n";
						o = true;
					}
					else if (regex_match(line, m, mline442))//.A(md) //for output 
					{
						temp.output = m.str(2);
						cout << "Output: " << temp.output << endl << "----------------------\n";
						o = true;
					}
				}
				nodes.push_back(temp);
			}

			else if (regex_match(line, m, input))               //if line is netlist input, ex. ".A(md[0]),"
			{
				module temp;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "input";
					nodes.push_back(temp);
				}
			}
			else if ((regex_match(line, m, input2)))            //if line is netlist input, ex. ".A(_0_),"
			{
				module temp;
				temp.name = m.str(1);
				temp.type = "input";
				nodes.push_back(temp);
			}
			else if (regex_match(line, m, output))              //if line is netlist output, ex. ".Y(md[0]),"
			{
				module temp;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "output";
					nodes.push_back(temp);
				}
			}
			else if (regex_match(line, m, output2))             //if line is netlist output, ex. ".Y(_0_),"
			{
				module temp;
				temp.type = "output";
				temp.name = m.str(1);
				nodes.push_back(temp);
			}
			else if (regex_match(line, m, wire))                 //if line is wire declaration, ex. "f[0]"
			{
				module temp;
				cout << "Type: wires(BUS)" << endl;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "wire";
					nodes.push_back(temp);
					cout << "Name: " << temp.name << endl;
					cout << "----------------------\n";
				}
			}
			else if (regex_match(line, m, wire2))                 //if line is netlist input, ex. "_0_"
			{
				module temp;
				temp.type = "wire";
				temp.name = m.str(1);
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Type: " << temp.type << endl;
				cout << "----------------------\n";
			}
			else if (regex_match(line, m, assign)) //assign x[2]=b[1]
			{
				module temp;
				temp.name = m.str(1) + '[' + m.str(2) + ']';

				temp.type = "assignment";
				temp.output = m.str(3) + '[' + m.str(4) + ']';
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Type: " << temp.type << endl;
				cout << "Input: " << temp.output << endl << "----------------------\n";
			}
			else if (regex_match(line, m, assign2)) //assign x[d:d] = y[d:d]
			{
				int def = atoi(m.str(3).c_str()) - atoi(m.str(6).c_str());

				module temp;
				temp.type = "assigment";
				for (int c = atoi(m.str(3).c_str()); c <= atoi(m.str(2).c_str()); c++)
				{
					cout << "Name: " << temp.name << endl;
					cout << "Type: " << temp.type << endl;
					temp.name = m.str(1) + '[' + convert_to_string(c) + ']';
					temp.output = m.str(4) + '[' + convert_to_string(c + def) + ']';
					cout << "Input: " << temp.output;
					nodes.push_back(temp);
				}
			}
			else if (regex_match(line, m, assign3)) //assign x=y
			{
				module temp;
				temp.name = m.str(1);
				temp.type = "assigment";
				temp.output = m.str(2);
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Input: " << m.str(2);
				cout << "Type: " << temp.type << endl;
			}

		}
	}
	myfile.close();

	//graph creation process
	Graph g(nodes.size());
	for (int y = 0; y < nodes.size(); y++)
	{
		if (nodes[y].type != "wire") // cause we need to insert the wire at the end , to correctly create the best level accuracy
		{
			g.addEdge(nodes[y].name, nodes[y].type, nodes[y].output);
			int f = g.get_index(nodes[y].name, g.adj);
			if (nodes[y].inputs.size() != 0)
			{
				for (int c = 0; c < nodes[y].inputs.size(); c++)
					g.addEdge(nodes[y].inputs[c], "!", nodes[y].name); // this for compeleting the missed part in the nodes with type !
			}
			else
			{
				if (nodes[y].type != "output" && nodes[y].type != "wire")
					g.adj[f].level = 1;
			}
		}
		else
		{
			wires.push_back(y);
		}
	}
	for (int y = 0; y < wires.size(); y++) // add the wires now
	{
		g.addEdge(nodes[wires[y]].name, nodes[wires[y]].type, nodes[wires[y]].output);
		if (nodes[wires[y]].inputs.size() != 0)
		{
			for (int c = 0; c < nodes[wires[y]].inputs.size(); c++)
				g.addEdge(nodes[wires[y]].inputs[c], "!", nodes[y].name);
		}

	}
	g.topologicalSort();                //topological sorting and JSON file generation
	system("pause");
}