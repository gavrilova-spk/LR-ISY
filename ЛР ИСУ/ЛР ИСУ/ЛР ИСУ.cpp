#include <iostream>
#include <vector>
#include <queue>
#include <list>
using pair = std::pair<int, int>;

std::vector<std::list<int>> bond_graph(14);

//-1 ходить нельзя, 1 начало, 0 свободны для движения
const int origin_map[6][6] = { {-1,-1,-1,-1,-1,-1,},
                             { -1,0,-1,0,0,-1 },
                             { -1,0,0,-1,0,-1 },
                             { -1,0,0,0,0,-1 },
                             { -1,0,-1,0,0,-1 },
                             { -1,-1,-1,-1,-1,-1 }
    };

int map[6][6] = { {-1,-1,-1,-1,-1,-1,},
                 { -1,0,-1,0,0,-1 },
                 { -1,0,0,-1,0,-1 },
                 { -1,0,0,0,0,-1 },
                 { -1,0,-1,0,0,-1 },
                 { -1,-1,-1,-1,-1,-1 }
};

static int LastVertex = 1;

struct Point {
    int c_x;
    int c_y;
    bool c_fin;
    int c_visited;

    void  SetValue(int x = 0, int y = 0, bool fin = false){
        c_x = x;
        c_y = y;
        c_fin = fin;
        c_visited = 0;
    };
};
Point p[3];

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
    if (*j == 0) {
        *j = LastVertex + 1;
        LastVertex++;
    }

    bond_graph[i-1].push_back(*j);
    bond_graph[*j-1].push_back(i);
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
    if (a->num_graph != 1)
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                if (map[i][j] == a->num_graph) {
                    a->x = j;
                    a->y = i;

                    for (int k = 0; k < 3; k++)
                    if (a->x == p[k].c_x && a->y == p[k].c_y) {
                        p[k].c_visited = a->num_graph;
                    }
                }
}

void BFS(Agent *a) {

    bool visited[13];
    for (int j = 0; j < 13; j++) {
        visited[j] = false;
        bond_graph[j].clear();
    }

    std::queue<int> q;
    std::list<int>::iterator i;
    q.push(a->num_graph);

    while (!q.empty()) {
        a->num_graph = q.front();
        move(a);

        graph(*a);
        q.pop();
        for (i = bond_graph[a->num_graph-1].begin(); i != bond_graph[a->num_graph-1].end(); ++i)
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

void clear_map(int x0, int y0) {
    for (int i = 1; i <= 4; i++)
        for (int j = 1; j <= 4; j++) {
            if (i == y0 && j == x0)
                map[i][j] = 1;
            else
                map[i][j] = origin_map[i][j];
        }
    LastVertex = 1;
}


int main()
{
    p[0].SetValue(1, 1, false);
    p[1].SetValue(1, 3, false);
    p[2].SetValue(4, 4, false);

    Agent a;
    clear_map(a.x, a.y);
    BFS(&a);
    for (int i = 1; i <= 4; i++) {
        std::cout << "\n";
        for (int j = 1; j <= 4; j++)
            if (map[i][j] == -1)
                std::cout << "*" << "\t";
            else
                std::cout << map[i][j] << "\t";
        
    }
    std::cout << "\n" << p[1].c_visited;
    std::cout << "\n" << p[2].c_visited;
    std::cout << "\n" << p[3].c_visited;

    for (int i = 0; i < 3; i++)
    {
        a.x = p[i].c_x;
        a.y = p[i].c_y;
        a.num_graph = 1;
        clear_map(a.x, a.y);
        BFS(&a);
    

        for (int i = 1; i <= 4; i++) {
            std::cout << "\n";
            for (int j = 1; j <= 4; j++)
                if (map[i][j] == -1)
                    std::cout << "*" << "\t";
                else
                    std::cout << map[i][j] << "\t";
        }
        std::cout << "\n------------------------";
    }
}

