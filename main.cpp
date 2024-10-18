#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <mutex>

int M; // Cantidad de metros
int N; // Cantidad de autos
std::vector<std::pair<int, int>> resultados; // Vector para almacenar el lugar y el auto
std::mutex mtx; // Mutex para proteger el acceso a 'resultados'
int lugar = 1; // Variable que indica el lugar de llegada

// Definición de carreraAuto
void carreraAuto(int id) {
    int distancia = 0;
    while (distancia < M) {
        int avance = std::rand() % 10 + 1; // Avanza entre 1 y 10 metros
        distancia += avance;
        std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distancia << " metros)" << std::endl;
        int espera = 100 + std::rand() % 401; // Espera entre 100 y 500 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(espera));
    }
    
    // Al terminar la carrera, usar un mutex para asignar el lugar de llegada
    {
        std::lock_guard<std::mutex> guard(mtx); // Bloquear el acceso mientras se modifica el lugar
        std::cout << "Auto" << id << " termina la carrera en el lugar " << lugar << "!" << std::endl;
        resultados.push_back({lugar, id}); // Guardar el lugar y el ID del auto
        lugar++; // Incrementar el lugar para el próximo auto
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia total> <número de autos>" << std::endl;
        return 1;
    }

    // Convertir argumentos a enteros
    M = std::atoi(argv[1]);
    N = std::atoi(argv[2]);

    // Validación: Al menos 2 autos y una distancia mínima de 10 metros
    if (N < 2) {
        std::cerr << "Error: Debe haber al menos 2 autos en la carrera." << std::endl;
        return 1;
    }

    if (M < 10) {
        std::cerr << "Error: La distancia de la carrera debe ser de al menos 10 metros." << std::endl;
        return 1;
    }

    std::cout << "La carrera de " << M << " metros comenzará ahora: \n------------------------------" << std::endl;

    std::vector<std::thread> autos;
    
    // Crear un hilo por cada auto
    for (int i = 0; i < N; ++i) {
        autos.push_back(std::thread(carreraAuto, i));
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; ++i) {
        autos[i].join();
    }

    // Imprimir la tabla de resultados al final de la carrera
    std::cout << "\nLugar  Auto\n";
    std::cout << "−−−−−−−−−−−−\n";
    for (const auto& resultado : resultados) {
        std::cout << resultado.first << "       Auto" << resultado.second << std::endl;
    }

    return 0;
}


