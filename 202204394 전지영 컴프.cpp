#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <map>
#include <memory>
#include <exception>

const int INF = std::numeric_limits<int>::max();

// 추상 클래스: Transport
class Transport {
public:
    virtual void simulate() const = 0; // 순수 가상 함수
    virtual ~Transport() = default; // 가상 소멸자
};

// 상속: Vehicle 클래스
class Vehicle : public Transport {
protected:
    std::string name;
    int current_location;
    int destination;
    std::string type; // "Car" or "Drone"

public:
    Vehicle(const std::string& name, int current_location, int destination, const std::string& type)
        : name(name), current_location(current_location), destination(destination), type(type) {}

    virtual void simulate() const override {
        std::cout << type << " [" << name << "] 이동 시뮬레이션 중...\n";
    }

    virtual int calculateETA(int distance) const { // 함수 중복
        return distance / 50; // 기본 속도: 50km/h
    }

    bool operator==(const Vehicle& other) const { // 연산자 중복
        return name == other.name && type == other.type;
    }
};

// 상속: Car 클래스
class Car : public Vehicle {
public:
    Car(const std::string& name, int current_location, int destination)
        : Vehicle(name, current_location, destination, "Car") {}

    virtual int calculateETA(int distance) const override {
        return distance / 60; // 자동차 속도: 60km/h
    }
};

// 상속: Drone 클래스
class Drone : public Vehicle {
public:
    Drone(const std::string& name, int current_location, int destination)
        : Vehicle(name, current_location, destination, "Drone") {}

    virtual int calculateETA(int distance) const override {
        return distance / 80; // 드론 속도: 80km/h
    }
};

// 그래프 클래스 (제네릭 클래스)
template <typename T>
class Graph {
private:
    int vertices;
    std::vector<std::vector<std::pair<int, T>>> adj;

public:
    Graph(int v) : vertices(v) {
        adj.resize(v);
    }

    void addEdge(int u, int v, T weight) {
        adj[u].emplace_back(v, weight);
        adj[v].emplace_back(u, weight); // 양방향
    }

    std::vector<T> shortestPath(int start) {
        std::vector<T> dist(vertices, INF);
        std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<>> pq;

        dist[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            int u = pq.top().second;
            T d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : adj[u]) {
                int v = edge.first;
                T weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.emplace(dist[v], v);
                }
            }
        }

        return dist;
    }
};

// 예외 처리용 사용자 정의 예외
class InvalidInputException : public std::exception {
public:
    const char* what() const noexcept override {
        return "잘못된 입력입니다. 다시 시도해주세요.";
    }
};

int main() {
    try {
        // 도시 그래프 (10개의 지점)
        Graph<int> city(10);

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

        // 사용자 입력
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

        if (location_names.find(start_location) == location_names.end() || location_names.find(end_location) == location_names.end()) {
            throw InvalidInputException();
        }

        // 차량 또는 드론 생성 (객체 포인터)
        std::unique_ptr<Transport> user_vehicle;
        if (vehicle_type == "자동차") {
            user_vehicle = std::make_unique<Car>(vehicle_name, start_location, end_location);
        } else if (vehicle_type == "드론") {
            user_vehicle = std::make_unique<Drone>(vehicle_name, start_location, end_location);
        } else {
            throw InvalidInputException();
        }

        // 최단 경로 계산
        auto dist = city.shortestPath(start_location);
        if (dist[end_location] == INF) {
            std::cout << "목적지까지의 경로가 없습니다.\n";
            return 0;
        }

        std::cout << "출발지: " << location_names.at(start_location) << "\n";
        std::cout << "목적지: " << location_names.at(end_location) << "\n";
        std::cout << "최단 거리: " << dist[end_location] << " km\n";

        // ETA 계산 및 시뮬레이션 실행
        std::cout << "예상 소요 시간: " << user_vehicle->calculateETA(dist[end_location]) << " 시간\n";
        user_vehicle->simulate();

    } catch (const std::exception& e) {
        std::cerr << "오류: " << e.what() << "\n";
    }

    return 0;
}
