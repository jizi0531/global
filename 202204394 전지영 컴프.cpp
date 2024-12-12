#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <map>

const int INF = std::numeric_limits<int>::max();

// ����ü: ���� �� ���
struct Vehicle {
    std::string name;
    int current_location;
    int destination;
    std::string type; // "Car" or "Drone"
};

// �׷��� Ŭ����
class Graph {
private:
    int vertices;
    std::vector<std::vector<std::pair<int, int>>> adj;

public:
    Graph(int v) : vertices(v) {
        adj.resize(v);
    }

    void addEdge(int u, int v, int weight) {
        adj[u].push_back({ v, weight });
        adj[v].push_back({ u, weight }); // �����
    }

    std::vector<int> shortestPath(int start) {
        std::vector<int> dist(vertices, INF);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

        dist[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({ dist[v], v });
                }
            }
        }

        return dist;
    }
};

// ���� �̵� �ùķ��̼�
void simulateVehicle(Vehicle& vehicle, Graph& graph, const std::map<int, std::string>& location_names) {
    std::cout << vehicle.type << " [" << vehicle.name << "] �ùķ��̼� ����\n";
    std::cout << "�����: " << location_names.at(vehicle.current_location) << "\n";
    std::cout << "������: " << location_names.at(vehicle.destination) << "\n";

    auto dist = graph.shortestPath(vehicle.current_location);
    if (dist[vehicle.destination] == INF) {
        std::cout << "������������ ��ΰ� �����ϴ�.\n";
        return;
    }

    std::cout << "������������ �Ÿ�: " << dist[vehicle.destination] << " km\n";
    std::cout << "�̵� �ùķ��̼� ��...\n";

    vehicle.current_location = vehicle.destination;
    std::cout << vehicle.type << " [" << vehicle.name << "]��(��) �������� �����߽��ϴ�: " << location_names.at(vehicle.destination) << "\n";
    std::cout << "-------------------------------\n";
}

int main() {
    // ���� �׷��� (10���� ����)
    Graph city(10);

    // ��� �߰� (�Ÿ� ����: km)
    city.addEdge(0, 1, 5);
    city.addEdge(1, 2, 10);
    city.addEdge(2, 3, 20);
    city.addEdge(3, 4, 15);
    city.addEdge(0, 5, 7);
    city.addEdge(5, 6, 8);
    city.addEdge(6, 7, 6);
    city.addEdge(7, 8, 10);
    city.addEdge(8, 9, 12);
    city.addEdge(1, 7, 14);

    // ���� �̸� ����
    std::map<int, std::string> location_names = {
        {0, "���� �߽�"}, {1, "�Ϻ� ��"}, {2, "���б�"}, {3, "���θ�"},
        {4, "����"}, {5, "���� ��"}, {6, "����"}, {7, "��� ����"},
        {8, "�ְ� ����"}, {9, "��� ����"}
    };

    // ����� �Է��� �޾� ���� ����
    std::string vehicle_name;
    std::string vehicle_type;
    int start_location, end_location;

    std::cout << "���� �Ǵ� ����� �̸��� �Է��ϼ���: ";
    std::cin >> vehicle_name;

    std::cout << "������ ������ �����ϼ��� (�ڵ���/���): ";
    std::cin >> vehicle_type;

    std::cout << "����� ��ȣ�� �Է��ϼ���:\n";
    for (const auto& loc : location_names) {
        std::cout << loc.first << ": " << loc.second << "\n";
    }
    std::cin >> start_location;

    std::cout << "������ ��ȣ�� �Է��ϼ���: ";
    std::cin >> end_location;

    // ���� �Ǵ� ��� ����
    Vehicle user_vehicle = { vehicle_name, start_location, end_location, vehicle_type };

    // �ùķ��̼� ����
    simulateVehicle(user_vehicle, city, location_names);

    return 0;
}