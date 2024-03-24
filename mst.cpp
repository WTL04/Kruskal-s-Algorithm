#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Edge
{
    int start;
    int end;
    float weight;

    Edge(int a, int b, float c) : start(a), end(b), weight(c) {} // consturctor, similar to self. in python
};

// data structure Union-find (did not learn this in 274), also known as Disjoint-Set
class UnionFind
{
    private:
        vector<int> parent;

    public:
        // constructor, sets each vertex to be it's own parent
        UnionFind(int n) 
        {
            parent.resize(n);
            for (int i = 0; i < n; ++i)
            {
                parent[i] = i; // Each vertex is its own parent initially
            }
            
        }

        // finds parent of a vector v
        int find(int v)
        {
            if(parent[v] == v)
            {
                return v; // if v is its own parent, return v
            }
            return parent[v] = find(parent[v]); // path comparision, recursivly calls to find representative of parent v
        }

        // merge a and b under one parent
        void unite(int a, int b)
        {
            parent[find(a)] = find(b);
        }

};


//takes each line of file --> gets integers and stores into 2d vector --> returns 2d vector
vector<vector<float>> parseFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        exit(1);
    }

    vector<vector<float>> matrix;
    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        char ch;
        float num;
        vector<float> row;

        while (ss >> ch)
        {
            if (isdigit(ch) || ch == '-') {
                ss.putback(ch);
                ss >> num;
                row.push_back(num);
            }
            else if (ch == ',' || ch == '{')
            {
                continue; //skip commas and {
            }
            else if (ch == '}')
            {
                //if consecutive closing brackets --> break
                char next_char;
                if (!(ss >> next_char))
                {
                    break;
                }

                matrix.push_back(row);
                row.clear();
            }
        }
    }
    file.close();
    return matrix;
}

// important: graph is undirected
void kruskal(vector<vector<float>> graph)
{
    int n = graph.size();

    // create union-find object
    UnionFind uf(n); 

    vector<Edge> edges;

    for (vector<float> & row : graph)
    {
        edges.push_back(Edge(row[0], row[1], row[2]));
    }
    
    // sort using lambda function, used to make custom sorting
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // minimum spanning tree
    vector<Edge> mst;

    // kruskal's algorithm
    for (Edge & edge : edges)
    {
        if (uf.find(edge.start) != uf.find(edge.end))
        {
            mst.push_back(edge);
            uf.unite(edge.start, edge.end);
        }
    }

    // output minimum spanning tree
    float totalSum = 0;
    for (const Edge& edge : mst) {
        totalSum += edge.weight;
    }

    cout << "MST weight : " << totalSum << endl;
}




int main() {

    // File paths
    string filename;

    cout << "Enter file name (.txt): ";
    cin >> filename;
    cout << endl;

    vector<vector<float>> graph = parseFile(filename);
    kruskal(graph);


    return 0;

}