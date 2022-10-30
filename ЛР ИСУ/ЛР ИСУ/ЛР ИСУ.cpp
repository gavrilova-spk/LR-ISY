#include <iostream>
#include <vector>
#include <queue>
#include <list>

std::vector<std::list<int>> bond_graph(14);
static int LastVertex = 1;

//-1 ходить нельзя, 1 начало, 0 свободны для движения
int origin_map[6][6] =  { {-1,-1,-1,-1,-1,-1,},
                        {  -1, 0, 0, 0, 0,-1 },
                        {  -1, 0, 0, 0, 0,-1 },
                        {  -1, 0, 0, 0, 0,-1 },
                        {  -1, 0, 0, 0, 0,-1 },
                        {  -1,-1,-1,-1,-1,-1 }
    };
int map[6][6] = { {-1,-1,-1,-1,-1,-1,},
                {  -1, 0, 0, 0, 0,-1 },
                {  -1, 0, 0, 0, 0,-1 },
                {  -1, 0, 0, 0, 0,-1 },
                {  -1, 0, 0, 0, 0,-1 },
                {  -1,-1,-1,-1,-1,-1 }
};

struct Point {
    int s_x;
    int s_y;
    bool s_final;
    bool s_visited;

    void  SetValue(int x = 0, int y = 0, bool fin = true){
        s_x = x;
        s_y = y;
        s_final = fin;
        s_visited = false;
    };
};
Point control_point[4];

//агент его положение по координатам и по вершине графа
struct Agent {
    int x;
    int y;
    int num_graph;

    void SetValue(int c_x, int c_y) {
        x = c_x;
        y = c_y;
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
void move(Agent* a, bool *target, bool* fin) {
    if (a->num_graph != 1)
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                if (map[i][j] == a->num_graph) {
                    a->x = j;
                    a->y = i;

                    for (int k = 0; k < 4; k++)
                    if (a->x == control_point[k].s_x && a->y == control_point[k].s_y) {
                        if (!control_point[k].s_final && !control_point[k].s_visited) {
                            control_point[k].s_visited = true;
                            *target = true;
                        }
                        else if (control_point[0].s_visited && control_point[1].s_visited && control_point[2].s_visited && control_point[k].s_final)
                            *fin = true;
                    }
                }
}


//алгоритм поиска в ширирну
void BFS(Agent *a, bool *non_fin) {

    bool visited[13];
    for (int j = 0; j < 13; j++) {
        visited[j] = false;
        bond_graph[j].clear();
    }

    bool target = false;
    bool fin_target = false;
    std::queue<int> q;
    std::list<int>::iterator i;
    q.push(a->num_graph);
    visited[0] = true;

    while (!q.empty()) {
        a->num_graph = q.front();
        move(a, &target, &fin_target);

        if (target) {
            return;
        }
        if (fin_target) {
            *non_fin = false;
            return;
        }

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

//очистка прошлого пути
void clear(int x0, int y0) {
    for (int i = 1; i <= 4; i++)
        for (int j = 1; j <= 4; j++) {
            if (i == y0 && j == x0)
                map[i][j] = 1;
            else
                map[i][j] = origin_map[i][j];
        }
    LastVertex = 1;
}

void Output_map(Agent a) {
    for (int i = 1; i <= 4; i++) {
        std::cout << "\n";
        for (int j = 1; j <= 4; j++) {
            if (map[i][j] == -1)
                std::cout << "#" << "\t";
            else if (map[i][j] == a.num_graph)
                std::cout << "[" << map[i][j] << "]\t";

            else if (j == control_point[0].s_x && i == control_point[0].s_y)
                std::cout << "{" << map[i][j] << "}\t";
            else if (j == control_point[1].s_x && i == control_point[1].s_y)
                std::cout << "{" << map[i][j] << "}\t";
            else if (j == control_point[2].s_x && i == control_point[2].s_y)
                std::cout << "{" << map[i][j] << "}\t";
            else if (j == control_point[3].s_x && i == control_point[3].s_y)
                std::cout << "(" << map[i][j] << ")\t";

            else 
            std::cout << map[i][j] << "\t";
        }
    }
    std::cout << "\n------------------------------------------------------------------\n";
}

void Input_map(Agent *a){
    int x, y;
    for (int i = 0; i < 3; i++) {
        std::cout << "Введите координаты точки препятствия: ";
        std::cin >> x >> y;
        origin_map[y][x] = -1;
    }

    for (int i = 0; i < 3; i++) {
        std::cout << "Введите координаты целевой НЕ финальной точки: ";
        std::cin >> x >> y;
        while (origin_map[y][x] == -1) {
            std::cout << "Введите координаты не пересекая препятствие: ";
            std::cin >> x >> y;
        }
        control_point[i].SetValue(x, y, false);
    }
    std::cout << "Введите координаты финальной точки: ";
    std::cin >> x >> y;
    while (origin_map[y][x] == -1) {
        std::cout << "Введите координаты не пересекая препятствие: ";
        std::cin >> x >> y;
    }
    control_point[3].SetValue(x, y, true);

    std::cout << "Введите координаты агента: ";
    std::cin >> x >> y;
    while (origin_map[y][x] == -1) {
        std::cout << "Введите координаты не пересекая препятствие: ";
        std::cin >> x >> y;
    }
    a->SetValue(x, y);

}

int main()
{
    setlocale(LC_ALL, "Russian");

    control_point[0].SetValue(2, 2, false);
    control_point[1].SetValue(1, 3, false);
    control_point[2].SetValue(4, 4, false);
    control_point[3].SetValue(4, 1, true);

    Agent a;
    bool non_fin = true;
    Input_map(&a);

    clear(a.x, a.y);
    Output_map(a);

    for (int i = 0; i < 4; i++) {
        BFS(&a, &non_fin);
        Output_map(a);
    
        clear(a.x, a.y);
        a.num_graph = 1;
    }

    if (non_fin)
        std::cout << "\nNO WAY";

    std::cout << "\n" << control_point[0].s_visited;
    std::cout << "\n" << control_point[1].s_visited;
    std::cout << "\n" << control_point[2].s_visited;

}

