#include <iostream>
#include <vector>
#include <string>
#include "Deque/Deque.hpp"
#include "Deque/ArrayDeque.hpp"
#include "Deque/ListDeque.hpp"
#include "Complex.hpp"
#include "Student.hpp"

class DequeUI {
private:
    std::vector<Deque<int>*> intDeques;
    std::vector<Deque<Complex>*> complexDeques;
    std::vector<Deque<Student>*> studentDeques;

public:
    ~DequeUI() {
        for (auto ptr : intDeques) delete ptr;
        for (auto ptr : complexDeques) delete ptr;
        for (auto ptr : studentDeques) delete ptr;
    }

    void Run() {
        while (true) {
            std::cout << "\nВыберите тип данных:\n"
                      << "1 - int\n"
                      << "2 - Complex\n"
                      << "3 - Student\n"
                      << "0 - Выход\n"
                      << "Ваш выбор: ";
            int typeChoice;
            std::cin >> typeChoice;

            if (typeChoice == 0) break;

            std::cout << "Выберите тип дека: 1 - ArrayDeque, 2 - ListDeque: ";
            int dequeChoice;
            std::cin >> dequeChoice;

            Deque<int>* intDeque = nullptr;
            Deque<Complex>* complexDeque = nullptr;
            Deque<Student>* studentDeque = nullptr;

            if (dequeChoice == 1) {
                if (typeChoice == 1) {
                    intDeque = new ArrayDeque<int>();
                    intDeques.push_back(intDeque);
                } else if (typeChoice == 2) {
                    complexDeque = new ArrayDeque<Complex>();
                    complexDeques.push_back(complexDeque);
                } else if (typeChoice == 3) {
                    studentDeque = new ArrayDeque<Student>();
                    studentDeques.push_back(studentDeque);
                }
            } else if (dequeChoice == 2) {
                if (typeChoice == 1) {
                    intDeque = new ListDeque<int>();
                    intDeques.push_back(intDeque);
                } else if (typeChoice == 2) {
                    complexDeque = new ListDeque<Complex>();
                    complexDeques.push_back(complexDeque);
                } else if (typeChoice == 3) {
                    studentDeque = new ListDeque<Student>();
                    studentDeques.push_back(studentDeque);
                }
            } else {
                std::cout << "Неверный выбор типа дека." << std::endl;
                continue;
            }

            if (intDeque) {
                ManageDeque(intDeque);
            } else if (complexDeque) {
                ManageDeque(complexDeque);
            } else if (studentDeque) {
                ManageDeque(studentDeque);
            }
        }
    }

private:
    template <typename T>
    void ManageDeque(Deque<T>* deque) {
        while (true) {
            std::cout << "\nВыберите операцию:\n"
                      << "1 - PushFront\n"
                      << "2 - PushBack\n"
                      << "3 - PopFront\n"
                      << "4 - PopBack\n"
                      << "5 - GetFront\n"
                      << "6 - GetBack\n"
                      << "7 - GetSize\n"
                      << "8 - IsEmpty\n"
                      << "9 - Печать дека\n"
                      << "0 - Назад\n"
                      << "Ваш выбор: ";
            int op;
            std::cin >> op;

            if (op == 0) break;

            switch (op) {
                case 1: {
                    T value;
                    std::cout << "Введите элемент: ";
                    std::cin >> value;
                    deque->PushFront(value);
                    break;
                }
                case 2: {
                    T value;
                    std::cout << "Введите элемент: ";
                    std::cin >> value;
                    deque->PushBack(value);
                    break;
                }
                case 3: {
                    if (deque->IsEmpty()) {
                        std::cout << "Дек пуст." << std::endl;
                    } else {
                        T val = deque->PopFront();
                        std::cout << "Удалён элемент с начала: " << val << std::endl;
                    }
                    break;
                }
                case 4: {
                    if (deque->IsEmpty()) {
                        std::cout << "Дек пуст." << std::endl;
                    } else {
                        T val = deque->PopBack();
                        std::cout << "Удалён элемент с конца: " << val << std::endl;
                    }
                    break;
                }
                case 5: {
                    if (deque->IsEmpty()) {
                        std::cout << "Дек пуст." << std::endl;
                    } else {
                        std::cout << "Первый элемент: " << deque->GetFront() << std::endl;
                    }
                    break;
                }
                case 6: {
                    if (deque->IsEmpty()) {
                        std::cout << "Дек пуст." << std::endl;
                    } else {
                        std::cout << "Последний элемент: " << deque->GetBack() << std::endl;
                    }
                    break;
                }
                case 7:
                    std::cout << "Размер дека: " << deque->GetSize() << std::endl;
                    break;
                case 8:
                    std::cout << (deque->IsEmpty() ? "Дек пуст" : "Дек не пуст") << std::endl;
                    break;
                case 9:
                    deque->Print(); // Вызываем метод Print
                    break;
                default:
                    std::cout << "Неверная операция." << std::endl;
            }
        }
    }
};
