#include <iostream>
#include <fstream>
#include "json.hpp"  // Подключаем библиотеку JSON

using json = nlohmann::json;  // Для удобства

int main() {
    // 1. Загружаем вопросы из файла
    std::ifstream file("questions.json");
    json data = json::parse(file);

    // 2. Выводим первый вопрос
    std::cout << "=== Quiz ===" << std::endl;
    std::cout << data[0]["text"] << std::endl;

    // 3. Показываем варианты ответов
    std::cout << "Options:" << std::endl;
    for (const auto& option : data[0]["options"]) {
        std::cout << "- " << option << std::endl;
    }

    // 4. Проверяем ответ
    std::string userAnswer;
    std::cout << "Your answer: ";
    std::cin >> userAnswer;

    if (userAnswer == data[0]["answer"]) {
        std::cout << "Correct! 🎉" << std::endl;
    } else {
        std::cout << "Wrong! Correct answer: " << data[0]["answer"] << std::endl;
    }

    return 0;
}