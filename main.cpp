#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <mutex>

std::vector<std::pair<int, int>> resultados; // Vector para almacenar el lugar y el auto
std::mutex mtx; // Mutex para proteger el acceso a 'resultados'
int lugar = 1; // Variable que indica el lugar de llegada

// Definición de función carreraAuto
void carreraAuto(int id, int M) {
    int distancia = 0;
    while (distancia < M) {
        int avance = std::rand() % 10 + 1; // Avanza entre 1 y 10 metros
        distancia += avance;
        // Si el auto termina la carrera
        if (distancia >= M){
            {
                std::lock_guard<std::mutex> guard(mtx); // Bloquear la salida a la terminal y proteger 'resultados' 
                std::cout << "Auto" << id << " avanza " << avance << " metros y termina la carrera en el lugar " << lugar << "!" << std::endl;
                resultados.push_back({lugar, id}); // Guardar el lugar y el ID del auto
            }
            lugar++; // Incrementar el lugar para el próximo auto
        // Si el auto no ha terminado la carrera
        }else {
            {
                std::lock_guard<std::mutex> guard(mtx); // Bloquear la salida a la terminal
                std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distancia << " metros)" << std::endl;
            }
            int espera = 100 + std::rand() % 401; // Espera entre 100 y 500 ms
            std::this_thread::sleep_for(std::chrono::milliseconds(espera));
        }
    }
}

int main(int argc, char* argv[]) {
    // Validar argumentos
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia total> <número de autos>" << std::endl;
        return 1;
    }
    // Validación: Distancia mínima de 10 metros
    if (std::atoi(argv[1]) <= 10){
        std::cerr << "Error: La distancia de la carrera debe ser de al menos 10 metros." << std::endl;
        return 1;
    }
    // Validación: Al menos 2 autos
    if (std::atoi(argv[2]) < 2){
        std::cerr << "Error: Debe haber al menos 2 autos en la carrera." << std::endl;
        return 1;
    }
    // Convertir argumentos a enteros
    int M = std::atoi(argv[1]); // Cantidad de metros
    int N = std::atoi(argv[2]); // Cantidad de autos

    std::cout << "\nLa carrera de " << M << " metros comenzará ahora: \n----------------------------------------" << std::endl;

    std::vector<std::thread> autos;
    
    // Crear un hilo por cada auto
    for (int i = 0; i < N; ++i) {
        autos.push_back(std::thread(carreraAuto, i, M));
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
