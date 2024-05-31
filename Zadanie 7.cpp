#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class Route {
private:
    vector<int> stops; // список остановок на маршруте
public:
    Route() {}
    void addStop(int stop) {
        stops.push_back(stop);
    }
    vector<int>& getStops() {
        return stops;
    }
};

class Graph {
private:
    vector<Route> routes; // список всех маршрутов
    int numStops; // общее количество остановок
public:
    Graph() {}
    void create() {
        int k;
        cout << "Введите количество маршрутов: ";
        cin >> k;

        cout << "Введите количество остановок: ";
        cin >> numStops;

        for (int i = 0; i < k; i++) {
            Route route;
            int stopsCount;
            cout << "Введите количество остановок на " << i + 1 << "-ом маршруте: ";
            cin >> stopsCount;
            for (int j = 0; j < stopsCount; j++) {
                int stop;
                cout << "Введите номер остановки " << j + 1 << ": ";
                cin >> stop;
                route.addStop(stop);
            }
            routes.push_back(route);
        }
    }
    void addRoute(Route* newRoute) {

        routes.push_back(*newRoute);
    }
    vector<Route>& getRoutes() {
        return routes;
    }

    void printRoutes() {
        for (size_t i = 0; i < routes.size(); ++i) {
            cout << i + 1 << " Маршрут: [";
            bool first = true;
            for (int stop : routes[i].getStops()) {
                if (!first) {
                    cout << " -> ";
                }
                else {
                    first = false;
                }
                cout << stop;
            }
            cout << "]" << endl;
        }
    }

    // Проверка возможности добраться от start до end с заданным количеством пересадок
    bool canTransfer(int start, int end, int transfers) {
        vector<vector<int>> adjList(numStops); // список смежности для графа
        for (Route route : routes) {
            const auto& stops = route.getStops();
            for (size_t i = 0; i < stops.size(); ++i) {
                for (size_t j = 0; j < stops.size(); ++j) {
                    if (i != j) {
                        adjList[stops[i]].push_back(stops[j]); // добавление ребра между остановками
                    }
                }
            }
        }
        return bfs(adjList, start, end, transfers);
    }

    bool bfs(const vector<vector<int>>& adjList, int start, int end, int maxTransfers) {
        vector<int> distance(numStops, -1);
        queue<int> q;
        q.push(start);
        distance[start] = 0;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == end) {
                return true;
            }

            if (distance[current] <= maxTransfers) {
                for (int neighbor : adjList[current]) { // если не превышено количество пересадок для всех соседей текущей остановки
                    if (distance[neighbor] == -1) {
                        distance[neighbor] = distance[current] + 1; // если сосед еще не посещен обновляем расстояние
                        q.push(neighbor);
                    }
                }
            }
        }
        return false;
    }

    // Проверка возможности добраться с каждой остановки до любой другой
    bool canReachAnyStop() {
        for (int i = 0; i < numStops; ++i) {
            for (int j = 0; j < numStops; ++j) {
                if (i != j && !canTransfer(i, j, numStops)) {
                    return false;
                }
            }
        }
        return true;
    }

    void zadanie5(int start, int end, int maxTransfers) {
        int index = 0;
        cout << "Введите номер маршрута для удаления: "; cin >> index;
        Graph newGraph;
        vector<int>stops;
        int k = 0;
        for (int i = 0; i < routes.size(); i++) {
            if (i != index - 1) {
                newGraph.getRoutes().push_back(routes[i]);
                for (int j = 0; j < routes[i].getStops().size(); j++) {
                    bool in = false;
                    for (int n = 0; n < stops.size(); n++) {
                        if (stops[n] == routes[i].getStops()[j])in = true;
                    }
                    if (!in) {
                        stops.push_back(routes[i].getStops()[j]);
                        k++;
                    }
                }
            }
        }

        newGraph.setnumstops(numStops);
        newGraph.printRoutes();
        if (newGraph.canTransfer(start, end, maxTransfers)) {
            cout << "Можно" << endl;
        }
        else cout << "Нельзя" << endl;

    }

    // Проверка возможности добраться с каждой остановки до любой другой после удаления маршрута
    bool canReachAnyStopAfterRemovingRoute(int routeIndex) {
        vector<Route> newRoutes;
        for (size_t i = 0; i < routes.size(); ++i) {
            if (i != routeIndex) {
                newRoutes.push_back(routes[i]);
            }
        }

        Graph g;
        g.routes = newRoutes; // создание нового графа без удаленного маршрута
        g.numStops = numStops;

        return g.canReachAnyStop(); // проверка связности нового графа
    }

    // Поиск минимального количества маршрутов, удаление которых нарушает связность
    int findMinRoutesToRemove() {
        int minRoutes = routes.size();
        for (size_t i = 0; i < routes.size(); ++i) {
            if (!canReachAnyStopAfterRemovingRoute(i)) {
                minRoutes = (i + 1 < minRoutes) ? i + 1 : minRoutes;
            }
        }
        return minRoutes;
    }

    // Печать минимального количества маршрутов, удаление которых нарушает связность
    void printMinRoutesToRemove() {
        int minRoutes = findMinRoutesToRemove();
        cout << "Минимальное количество маршрутов для удаления: " << minRoutes << endl;
    }

    void setnumstops(int newnum) {
        numStops = newnum;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    int num;
    int flag = 0;

    while (flag == 0) {
        cout << "Меню:" << endl;
        cout << "1 - Прогнать по готовым тестовым вариантам" << endl;
        cout << "2 - Вбить данные для теста вручную" << endl;
        cin >> num;
        if (num == 1) {
            flag = 1;
            cout << "Тест 1" << endl;
            Graph first;
            first.setnumstops(5);
            Route route;
            route.addStop(0);
            route.addStop(1);
            route.addStop(2);
            route.addStop(3);
            route.addStop(4);
            first.getRoutes().push_back(route);
            first.printRoutes();

            cout << "Проверка возможности попасть с остановки 0 на остановку 4 без пересадок:" << endl;
            if (first.canTransfer(0, 4, 0)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (first.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            first.printMinRoutesToRemove();
            cout << endl;
            first.printMinRoutesToRemove();
            first.zadanie5(0, 4, 1);


            cout << endl;
            cout << "******************************************************" << endl;
            cout << endl;


            cout << "Тест 2" << endl;
            Graph second;
            second.setnumstops(5);
            Route route2;
            route2.addStop(0);
            route2.addStop(1);
            route2.addStop(2);
            Route route3;
            route3.addStop(2);
            route3.addStop(3);
            route3.addStop(4);
            second.getRoutes().push_back(route2);
            second.getRoutes().push_back(route3);
            second.printRoutes();


            cout << "Проверка возможности попасть с остановки 0 на остановку 4 c 1 пересадкой:" << endl;
            if (second.canTransfer(0, 4, 1)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (second.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            second.printMinRoutesToRemove();
            cout << endl;
            second.printMinRoutesToRemove();
            second.zadanie5(0, 4, 1);


            cout << endl;
            cout << "******************************************************" << endl;
            cout << endl;


            cout << "Тест 3" << endl;
            Graph tri;
            tri.setnumstops(6);
            Route route4;
            route4.addStop(0);
            route4.addStop(1);
            route4.addStop(2);
            Route route5;
            route5.addStop(3);
            route5.addStop(4);
            route5.addStop(5);
            tri.getRoutes().push_back(route4);
            tri.getRoutes().push_back(route5);
            tri.printRoutes();

            cout << "Проверка возможности попасть с остановки 0 на остановку 5 с одной пересадкой:" << endl;
            if (tri.canTransfer(0, 5, 1)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (tri.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            tri.printMinRoutesToRemove();
            cout << endl;
            tri.printMinRoutesToRemove();
            tri.zadanie5(0, 5, 1);


            cout << endl;
            cout << "******************************************************" << endl;
            cout << endl;


            cout << "Тест 4" << endl;
            Graph four;
            four.setnumstops(4);
            Route route6;
            route6.addStop(0);
            route6.addStop(1);
            route6.addStop(2);
            route6.addStop(3);
            four.getRoutes().push_back(route6);
            four.printRoutes();

            cout << "Проверка возможности попасть с остановки 0 на остановку 3 без пересадок:" << endl;
            if (four.canTransfer(0, 3, 0)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (four.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            four.printMinRoutesToRemove();
            cout << endl;
            four.printMinRoutesToRemove();
            four.zadanie5(0, 3, 0);


            cout << endl;
            cout << "******************************************************" << endl;
            cout << endl;


            cout << "Тест 5" << endl;
            Graph five;
            five.setnumstops(4);
            Route route7;
            route7.addStop(0);
            route7.addStop(1);
            Route route8;
            route8.addStop(1);
            route8.addStop(2);
            route8.addStop(3);
            five.getRoutes().push_back(route7);
            five.getRoutes().push_back(route8);
            five.printRoutes();

            cout << "Проверка возможности попасть с остановки 0 на остановку 3 с 1 пересадкой:" << endl;
            if (five.canTransfer(0, 3, 1)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (five.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            five.printMinRoutesToRemove();
            cout << endl;
            five.printMinRoutesToRemove();
            five.zadanie5(0, 3, 1);


            cout << endl;
            cout << "******************************************************" << endl;
            cout << endl;


            cout << "Тест 6" << endl;
            Graph six;
            six.setnumstops(6);
            Route route9;
            route9.addStop(0);
            route9.addStop(1);
            route9.addStop(2);
            Route route10;
            route10.addStop(2);
            route10.addStop(3);
            route10.addStop(4);
            Route route11;
            route11.addStop(4);
            route11.addStop(5);

            six.getRoutes().push_back(route9);
            six.getRoutes().push_back(route10);
            six.getRoutes().push_back(route11);
            six.printRoutes();

            cout << "Проверка возможности попасть с остановки 0 на остановку 5 с 2 пересадками:" << endl;
            if (six.canTransfer(0, 5, 2)) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            cout << "Проверка возможности добраться с любой остановки до любой другой:" << endl;
            if (six.canReachAnyStop()) {
                cout << "Можно" << endl;
            }
            else {
                cout << "Нельзя" << endl;
            }
            cout << endl;
            six.printMinRoutesToRemove();
            six.zadanie5(0, 5, 2);

        }
        else if (num == 2) {
            flag = 1;
            Graph un;
            un.create();
            un.printRoutes();

            cout << "Введите остановку, с которой вам нужно уехать: ";
            int startStop;
            cin >> startStop;
            cout << "Введите остановку, на которую вам нужно приехать: ";
            int endStop;
            cin >> endStop;
            cout << "Введите количество пересадок: ";
            int transfers;
            cin >> transfers;

            if (un.canTransfer(startStop, endStop, transfers)) {
                cout << "Возможен путь с " << transfers << " пересадками." << endl;
            }
            else {
                cout << "Путь невозможен." << endl;
            }

            if (un.canReachAnyStop()) {
                cout << "Можно добраться с любой остановки до любой другой остановки." << endl;
            }
            else {
                cout << "Нельзя добраться с каждой остановки до любой другой остановки." << endl;
            }

            un.printMinRoutesToRemove();
            cout << endl;
            un.printMinRoutesToRemove();
            un.zadanie5(startStop, endStop, transfers);
        }
        else {
            cout << "Нет такого выбора, выберите 1 или 2" << endl;
        };
    };

    return 0;
}
