#include <iostream>
#include <vector>
#include <queue>
#include <list>
using pair = std::pair<int, int>;

std::vector<std::list<int>> bond_graph(14);

//-1 ходить нельзя, 1 начало, 0 свободны для движения
int map[6][6] = { {-1,-1,-1,-1,-1,-1,},
                 { -1,0,-1,0,0,-1 },
                 { -1,0,0,-1,0,-1 },
                 { -1,0,1,0,0,-1 },
                 { -1,0,-1,0,0,-1 },
                 { -1,-1,-1,-1,-1,-1 }
    };

struct Point {
    int x;
    int y;
    bool fin;
    bool visited;
};

//агент его положение по координатам и по вершине графа
struct Agent {
    int x;
    int y;
    int num_graph;
    Agent() {
        x = 2;
        y = 3;
        num_graph = 1;
    }
};

//запись значения вершины по алгоритму и установление связи соседних вершин
//(они могут повторяться но в функции BFS это не мешает и все пока верно)
void add_connect(int i, int *j) {
    static int N = 1;
    if (*j == 0) {
        *j = N + 1;
        N++;
    }

    bond_graph[i].push_back(*j);
    bond_graph[*j].push_back(i);
}

// из-за особености записи массива сначала координата по вертикали потом по горизонтали
//проверка возможности хода в 4 направления
void graph(Agent a) {
    if (map[a.y][a.x + 1] >= 0)
        add_connect(a.num_graph, &map[a.y][a.x + 1]);
    if (map[a.y][a.x - 1] >= 0)
        add_connect(a.num_graph, &map[a.y][a.x - 1]);
    if (map[a.y + 1][a.x] >= 0)
        add_connect(a.num_graph, &map[a.y + 1][a.x]);
    if (map[a.y - 1][a.x] >= 0)
        add_connect(a.num_graph, &map[a.y - 1][a.x]);

}
//передвижение агента на следующий шаг по графу
void move(Agent* a) {
    if ((*a).num_graph != 1)
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                if (map[i][j] == (*a).num_graph) {
                    (*a).x = j;
                    (*a).y = i;
                }
}

void BFS(Agent *a) {

    bool visited[13];
    for (int j = 0; j < 13; j++)
        visited[j] = false;
    std::queue<int> q;
    std::list<int>::iterator i;
    q.push((*a).num_graph);

    while (!q.empty()) {
        (*a).num_graph = q.front();
        move(a);

        graph(*a);
        q.pop();
        for (i = bond_graph[(*a).num_graph].begin(); i != bond_graph[(*a).num_graph].end(); ++i)
        {
            int adjVertex = *i;
            if (!visited[adjVertex-1])
            {
                visited[adjVertex-1] = true;
                q.push(adjVertex);
            }
        }
    }
}




int main()
{
    Agent a;
    BFS(&a);

}

