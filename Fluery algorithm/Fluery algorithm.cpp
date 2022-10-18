#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>

/*============================================================================================================*/

int numOfVertexes;

std::vector <std::vector <int> > adjacencyMatrix;

/*============================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile();

std::vector <bool> checkingForConnectivity(int vertex);

bool returnConnectivity(std::vector<bool> visitedVertexes);

bool checkGraphForEvenDegrees(std::vector <std::vector <int> > adjacencyMatrix);

std::stack <int> fleuryAlgorithm(std::vector <std::vector <int> > adjacencyMatrix);

/*============================================================================================================*/

int main()
{
    adjacencyMatrix = inputAdjacencyMatrixFromFile();
    std::vector<bool> visitedVertexes = checkingForConnectivity(0);
    if (returnConnectivity(visitedVertexes))
        if (checkGraphForEvenDegrees(adjacencyMatrix))
        {
            std::stack <int> cycle = fleuryAlgorithm(adjacencyMatrix);
            std::ofstream writer;
            writer.open("output.txt");
            if (writer.is_open())
            {
                writer << "Graph is Eulerian. The path through the graph has the form:\n";
                int cycleSize = cycle.size();
                for (int i = 0; i < cycleSize; ++i)
                {
                    writer << cycle.top() << " ";
                    cycle.pop();
                }
                writer.close();
            }
            else
            {
                writer.close();
                std::cout << "File Alert!";
                std::exit(1);
            }
        }
        else
        {
            std::ofstream writer;
            writer.open("output.txt");
            if (writer.is_open())
            {
                writer << "Not all degrees of graph vertices are even!";
                writer.close();
            }
            else
            {
                writer.close();
                std::cout << "File Alert!";
                std::exit(1);
            }
        }
    else
    {
        std::ofstream writer;
        writer.open("output.txt");
        if (writer.is_open())
        {
            writer << "The graph is not connected!";
            writer.close();
        }
        else
        {
            writer.close();
            std::cout << "File Alert!";
            std::exit(1);
        }
    }
}

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile()
{
    std::ifstream reader("input.txt");
    if (reader.is_open())
    {
        reader >> numOfVertexes;
        std::vector < std::vector <int > > adjacencyMatrix(numOfVertexes, std::vector <int>(numOfVertexes));
        for (int i = 0; i < numOfVertexes; ++i)
            for (int j = 0; j < numOfVertexes; ++j)
                if (!reader.eof())
                    reader >> adjacencyMatrix[i][j];
                else
                    break;
        reader.close();
        return adjacencyMatrix;
    }
    else
    {
        reader.close();
        std::cout << "File Alert!";
        std::exit(1);
    }
}

std::vector<bool> checkingForConnectivity(int vertex)
{
    std::queue <int> lookingVertexes;
    lookingVertexes.push(vertex);
    std::vector<bool> visitedVertexes(numOfVertexes, false);
    visitedVertexes[vertex] = true;
    while (!lookingVertexes.empty())
    {
        int currentVertex = lookingVertexes.front();
        lookingVertexes.pop();
        visitedVertexes[currentVertex] = true;
        for (size_t i = 0; i < adjacencyMatrix[vertex].size(); ++i)
            if (!visitedVertexes[i] && adjacencyMatrix[currentVertex][i])
                lookingVertexes.push(i);
    }
    return visitedVertexes;
}

bool returnConnectivity(std::vector<bool> visitedVertexes)
{
    bool result = all_of(visitedVertexes.begin(), visitedVertexes.end(), [](bool value) {return value; });
    return result;
}

bool checkGraphForEvenDegrees(std::vector<std::vector<int>> adjacencyMatrix)
{
    int counterOfDegree;
    for (int i = 0; i < numOfVertexes; ++i) 
    {
        counterOfDegree = 0;
        for (int j = 0; j < numOfVertexes; ++j)
            if (adjacencyMatrix[i][j] != 0) 
                ++counterOfDegree;
        if (counterOfDegree % 2 == 1)
            return false;
    }
    return true;
}

std::stack <int> fleuryAlgorithm(std::vector<std::vector<int>> adjacencyMatrix)
{
    std::vector <std::vector <int> > resultMatrix = adjacencyMatrix;
    std::stack <int> cycle;
    std::stack <int> checkedVertexes;
    checkedVertexes.push(0);
    while (!checkedVertexes.empty()) 
    {
        bool flag = true;
        int i = checkedVertexes.top();
        for (int j = 0; j < numOfVertexes; ++j)
            if (resultMatrix[i][j] != 0) 
            {
                flag = false;
                checkedVertexes.push(j);
                resultMatrix[i][j] = 0;
                resultMatrix[j][i] = 0;
                break;
            }
        if (flag)
        {
            cycle.push(checkedVertexes.top());
            checkedVertexes.pop();
        }
    }
    return cycle;
}