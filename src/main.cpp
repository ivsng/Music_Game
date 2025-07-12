#include <iostream>
#include <cstdlib> // для system()
#include "json.hpp"  
#include <fstream>

using json = nlohmann::json;

void playBackgroundMusic() {
    #ifdef _WIN32
        system("start assets/global/audio/theme.m4a");
    #else
        system("xdg-open assets/global/audio/theme.mp3");
    #endif
}

int main() {
    playBackgroundMusic(); // Запуск музыки
    
    // Загрузка вопросов
    std::ifstream file("questions.json");
    json data = json::parse(file);
    
    // Простой вывод вопросов
    for (auto& question : data) {
        std::cout << question["question"] << std::endl;
        
        // Открываем медиафайл
        #ifdef _WIN32
            system(("start " + question["file"].get<std::string>()).c_str());
        #else
            system(("xdg-open " + question["file"].get<std::string>()).c_str());
        #endif
        
        // Вывод вариантов
        for (size_t i = 0; i < question["options"].size(); i++) {
            std::cout << i+1 << ". " << question["options"][i] << std::endl;
        }
        
        // Проверка ответа
        std::string userAnswer;
        std::cin >> userAnswer;
        
        if (userAnswer == question["answer"]) {
            std::cout << "Correct!\n";
        } else {
            std::cout << "Wrong! Answer: " << question["answer"] << "\n";
        }
    }
    return 0;
}