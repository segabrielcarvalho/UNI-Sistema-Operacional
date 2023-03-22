#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

std::mutex mtx;
std::vector<std::pair<int, int>> ranking;

void sapo(int id, int max_saltos, int distancia) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    int saltos = 0;
    int distancia_percorrida = 0;

    while (saltos < max_saltos && distancia_percorrida < distancia) {
        int salto = dis(gen);
        distancia_percorrida += salto;
        saltos++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Sapo " << id << " saltou " << salto << " cm e já percorreu " << distancia_percorrida << " cm." << std::endl;
        lock.unlock();
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        ranking.push_back(std::make_pair(id, saltos));
        std::cout << "Sapo " << id << " finalizou a corrida!" << std::endl;
    }
}

void exibir_ranking() {
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second < b.second;
    });

    std::cout << "\nRanking da corrida de sapos:\n";
    std::cout << std::setw(10) << "Sapo" << std::setw(15) << "Saltos" << std::endl;

    for (const auto &sapo : ranking) {
        std::cout << std::setw(10) << sapo.first << std::setw(15) << sapo.second << std::endl;
    }
}

int main() {
    int distancia, num_sapos, max_saltos;

    std::cout << "Informe a distância máxima em cm: ";
    std::cin >> distancia;

    std::cout << "Informe a quantidade de sapos participantes da corrida: ";
    std::cin >> num_sapos;

    std::cout << "Informe a quantidade máxima de saltos para completar a corrida: ";
    std::cin >> max_saltos;

    std::vector<std::thread> sapos;
    for (int i = 1; i <= num_sapos; ++i) {
        sapos.push_back(std::thread(sapo, i, max_saltos, distancia));
    }

    for (auto &t: sapos) {
        t.join();
    }

    std::cout << "Corrida de sapos finalizada!" << std::endl;

    exibir_ranking();

    return 0;
}