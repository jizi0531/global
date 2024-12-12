#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <map>

const int INF = std::numeric_limits<int>::max();

// 구조체: 차량 및 드론
struct Vehicle {
    std::string name;
    int current_location;
    int destination;
    std::string type; // "Car" or "Drone"
};

// 그래프 클래스
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
        adj[v].push_back({ u, weight }); // 양방향
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

// 차량 이동 시뮬레이션
void simulateVehicle(Vehicle& vehicle, Graph& graph, const std::map<int, std::string>& location_names) {
    std::cout << vehicle.type << " [" << vehicle.name << "] 시뮬레이션 시작\n";
    std::cout << "출발지: " << location_names.at(vehicle.current_location) << "\n";
    std::cout << "목적지: " << location_names.at(vehicle.destination) << "\n";

    auto dist = graph.shortestPath(vehicle.current_location);
    if (dist[vehicle.destination] == INF) {
        std::cout << "목적지까지의 경로가 없습니다.\n";
        return;
    }

    std::cout << "목적지까지의 거리: " << dist[vehicle.destination] << " km\n";
    std::cout << "이동 시뮬레이션 중...\n";

    vehicle.current_location = vehicle.destination;
    std::cout << vehicle.type << " [" << vehicle.name << "]이(가) 목적지에 도착했습니다: " << location_names.at(vehicle.destination) << "\n";
    std::cout << "-------------------------------\n";
}

int main() {
    // 도시 그래프 (10개의 지점)
    Graph city(10);

    // 경로 추가 (거리 단위: km)
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

    // 지점 이름 매핑
    std::map<int, std::string> location_names = {
        {0, "도시 중심"}, {1, "북부 역"}, {2, "대학교"}, {3, "쇼핑몰"},
        {4, "공항"}, {5, "남부 역"}, {6, "병원"}, {7, "기술 공원"},
        {8, "주거 지역"}, {9, "산업 지역"}
    };

    // 사용자 입력을 받아 차량 생성
    std::string vehicle_name;
    std::string vehicle_type;
    int start_location, end_location;

    std::cout << "차량 또는 드론의 이름을 입력하세요: ";
    std::cin >> vehicle_name;

    std::cout << "수단의 유형을 선택하세요 (자동차/드론): ";
    std::cin >> vehicle_type;

    std::cout << "출발지 번호를 입력하세요:\n";
    for (const auto& loc : location_names) {
        std::cout << loc.first << ": " << loc.second << "\n";
    }
    std::cin >> start_location;

    std::cout << "목적지 번호를 입력하세요: ";
    std::cin >> end_location;

    // 차량 또는 드론 생성
    Vehicle user_vehicle = { vehicle_name, start_location, end_location, vehicle_type };

    // 시뮬레이션 실행
    simulateVehicle(user_vehicle, city, location_names);

    return 0;
}