#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include "UI.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Sequence.hpp"
#include "Errors.hpp"

void HandleInputError()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input! Please enter a valid number.\n";
}

void InvalidOption()
{
    std::cout << "Invalid option selected. Please try again.\n";
}

template <typename T>
Sequence<T>* CreateSequenceByOption(int option, const std::vector<T>& items)
{
    switch(option) {
        case 1: return new MutableArraySequence<T>(items.data(), items.size());
        case 2: return new ImmutableArraySequence<T>(items.data(), items.size());
        case 3: return new MutableListSequence<T>(items.data(), items.size());
        case 4: return new ImmutableListSequence<T>(items.data(), items.size());
        default:
            THROW_ERROR(INVALID_ARGUMENT);
            return nullptr;
    }
}

template <typename T>
void CreateSequenceMenu(SequenceManager& manager, const std::string& typeName) {
    if (!manager.CanAddSequence()) {
        std::cout << "Cannot create more sequences. Maximum limit reached.\n";
        return;
    }

    int option;
    do {
        std::cout << "\n=== Create " << typeName << " Sequence ===\n"
                  << "1. Mutable Array\n"
                  << "2. Immutable Array\n"
                  << "3. Mutable List\n"
                  << "4. Immutable List\n"
                  << "0. Back\n"
                  << "Choose option: ";

        if (!(std::cin >> option)) {
            HandleInputError();
            continue;
        }

        if (option == 0) return;

        if (option < 1 || option > 4) {
            InvalidOption();
            continue;
        }

        try {
            std::cout << "Enter number of elements: ";
            int n;
            if (!(std::cin >> n)) {
                HandleInputError();
                continue;
            }

            if (n < 0) {
                std::cout << "Number of elements cannot be negative\n";
                continue;
            }

            std::vector<T> elements(n);
            if (n > 0) {
                std::cout << "Enter " << n << " elements: ";
                for (int i = 0; i < n; i++) {
                    if (!(std::cin >> elements[i])) {
                        HandleInputError();
                        elements.clear();
                        break;
                    }
                }
            }

            if constexpr (std::is_same_v<T, int>) {
                manager.AddIntSequence(CreateSequenceByOption(option, elements));
            } else {
                manager.AddDoubleSequence(CreateSequenceByOption(option, elements));
            }

            std::cout << typeName << " sequence created!\n";
            break;
        } catch (const std::exception& e) {
            std::cout << e.what() << "\n";
        }
    } while (true);
}

template <typename T>
void OperationMenu(SequenceManager& manager, const std::string& typeName) {
    size_t count = std::is_same_v<T, int> ? manager.GetIntSequenceCount() : manager.GetDoubleSequenceCount();
    
    if (count == 0) {
        std::cout << "No " << typeName << " sequences available. Please create one first.\n";
        return;
    }

    size_t seqIndex;
    do {
        std::cout << "\nSelect " << typeName << " sequence index (0-" << count-1 << "): ";
        if (!(std::cin >> seqIndex)) {
            HandleInputError();
            continue;
        }

        if (seqIndex >= count) {
            std::cout << "Invalid sequence index. Please try again.\n";
            continue;
        }
        break;
    } while (true);

    Sequence<T>* seq = nullptr;
    if constexpr (std::is_same_v<T, int>) {
        seq = manager.GetIntSequence(seqIndex);
    } else {
        seq = manager.GetDoubleSequence(seqIndex);
    }

    int option;
    do {
        std::cout << "\n=== " << typeName << " Sequence Operations ===\n"
                  << "1. Show sequence\n"
                  << "2. Append element\n"
                  << "3. Prepend element\n"
                  << "4. Insert element at index\n"
                  << "5. Get element by index\n"
                  << "6. Concatenate with another sequence\n"
                  << "7. Get subsequence\n"
                  << "8. Remove element by index\n"
                  << "0. Back\n"
                  << "Choose option: ";

        if (!(std::cin >> option)) {
            HandleInputError();
            continue;
        }

        if (option == 0) break;

        try {
            switch(option) {
                case 1:
                    manager.ShowSequence(seq);
                    break;
                case 2:
                {
                    T elem;
                    std::cout << "Enter element to append: ";
                    if (!(std::cin >> elem)) {
                        HandleInputError();
                        continue;
                    }
                    auto* newSeq = seq->Append(elem);
                    manager.ShowSequence(newSeq);
                    if (newSeq != seq && manager.CanAddSequence()) {
                        if constexpr (std::is_same_v<T, int>) {
                            manager.AddIntSequence(newSeq);
                        } else {
                            manager.AddDoubleSequence(newSeq);
                        }
                    }
                    break;
                }
                case 3:
                {
                    T elem;
                    std::cout << "Enter element to prepend: ";
                    if (!(std::cin >> elem)) {
                        HandleInputError();
                        continue;
                    }
                    auto* newSeq = seq->Prepend(elem);
                    manager.ShowSequence(newSeq);
                    if (newSeq != seq && manager.CanAddSequence()) {
                        if constexpr (std::is_same_v<T, int>) {
                            manager.AddIntSequence(newSeq);
                        } else {
                            manager.AddDoubleSequence(newSeq);
                        }
                    }
                    break;
                }
                case 4:
                {
                    T elem;
                    std::cout << "Enter element to insert: ";
                    if (!(std::cin >> elem)) {
                        HandleInputError();
                        continue;
                    }

                    size_t index;
                    std::cout << "Enter index to insert by: ";
                    if (!(std::cin >> index)) {
                        HandleInputError();
                        continue;
                    }

                    auto* newSeq = seq->InsertAt(elem, index);
                    manager.ShowSequence(newSeq);
                    if (newSeq != seq && manager.CanAddSequence()) {
                        if constexpr (std::is_same_v<T, int>) {
                            manager.AddIntSequence(newSeq);
                        } else {
                            manager.AddDoubleSequence(newSeq);
                        }
                    }
                    break;
                }
                case 5:
                {
                    size_t index;
                    std::cout << "Enter index of element to get: ";
                    if (!(std::cin >> index)) {
                        HandleInputError();
                        continue;
                    }
                    auto elem = seq->Get(index);
                    std::cout << elem;
                    break;
                }
                case 6: { // Concatenate
                    if (count < 2) {
                        std::cout << "Need at least 2 sequences for concatenation.\n";
                        break;
                    }
                
                    size_t otherIndex;
                    std::cout << "Enter index of sequence to concatenate with (0-" << count-1 << "): ";
                    if (!(std::cin >> otherIndex) || otherIndex >= count) {
                        HandleInputError();
                        break;
                    }
                
                    try {
                        // Получаем вторую последовательность
                        Sequence<T>* otherSeq = nullptr;
                        if constexpr (std::is_same_v<T, int>) {
                            otherSeq = manager.GetIntSequence(otherIndex);
                        } else {
                            otherSeq = manager.GetDoubleSequence(otherIndex);
                        }
                
                        // Выполняем конкатенацию
                        Sequence<T>* newSeq = seq->Concat(otherSeq);
                        
                        // Проверяем результат
                        if (!newSeq) {
                            std::cout << "Error: Concatenation returned null\n";
                            break;
                        }
                
                        // Выводим результат
                        std::cout << "Concatenation result: ";
                        manager.ShowSequence(newSeq);
                        std::cout << std::endl;  // Явный flush вывода
                
                        // Сохраняем результат если есть место
                        if (manager.CanAddSequence()) {
                            if constexpr (std::is_same_v<T, int>) {
                                manager.AddIntSequence(newSeq);
                            } else {
                                manager.AddDoubleSequence(newSeq);
                            }
                            std::cout << "Result saved as sequence #" 
                                      << (std::is_same_v<T, int> 
                                          ? manager.GetIntSequenceCount()-1 
                                          : manager.GetDoubleSequenceCount()-1)
                                      << "\n";
                        } else {
                            std::cout << "Cannot save result - maximum sequences reached.\n";
                            delete newSeq;
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Concatenation failed: " << e.what() << "\n";
                    }
                    break;
                }

                case 7: { // GetSubsequence
                    size_t start, end;
                    std::cout << "Enter start index (0-" << seq->GetLength()-1 << "): ";
                    if (!(std::cin >> start) || start >= seq->GetLength()) {
                        HandleInputError();
                        break;
                    }

                    std::cout << "Enter end index (" << start << "-" << seq->GetLength()-1 << "): ";
                    if (!(std::cin >> end) || end < start || end >= seq->GetLength()) {
                        HandleInputError();
                        break;
                    }

                    Sequence<T>* subSeq = seq->GetSubsequence(start, end);
                    std::cout << "Subsequence [" << start << ".." << end << "]: ";
                    manager.ShowSequence(subSeq);

                    if (manager.CanAddSequence()) {
                        if constexpr (std::is_same_v<T, int>) {
                            manager.AddIntSequence(subSeq);
                        } else {
                            manager.AddDoubleSequence(subSeq);
                        }
                    } else {
                        std::cout << "Cannot save subsequence - maximum sequences reached.\n";
                        delete subSeq;
                    }
                    break;
                }
                case 8:
                {
                    T elem;
                    std::cout << "Enter index of element to remove: ";
                    if (!(std::cin >> elem)) {
                        HandleInputError();
                        continue;
                    }
                    auto* newSeq = seq->DeleteByIndex(elem);
                    manager.ShowSequence(newSeq);
                    if (newSeq != seq && manager.CanAddSequence()) {
                        if constexpr (std::is_same_v<T, int>) {
                            manager.AddIntSequence(newSeq);
                        } else {
                            manager.AddDoubleSequence(newSeq);
                        }
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    InvalidOption();
                    break;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    } while (option != 0);
}

void IntMenu(SequenceManager& manager) {
    int option;
    do {
        std::cout << "\n=== Integer Sequences ===\n"
                  << "1. Create new sequence\n"
                  << "2. Perform operation on sequence\n"
                  << "3. Show all integer sequences\n"
                  << "0. Back\n"
                  << "Choose option: ";

        if (!(std::cin >> option)) {
            HandleInputError();
            continue;
        }

        switch(option) {
            case 1: CreateSequenceMenu<int>(manager, "integer"); break;
            case 2: OperationMenu<int>(manager, "integer"); break;
            case 3: manager.ShowAllSequences(manager.GetIntSequences(), "integer"); break;
            case 0: break;
            default: InvalidOption(); break;
        }
    } while (option != 0);
}

void DoubleMenu(SequenceManager& manager) {
    int option;
    do {
        std::cout << "\n=== Double Sequences ===\n"
                  << "1. Create new sequence\n"
                  << "2. Perform operation on sequence\n"
                  << "3. Show all double sequences\n"
                  << "0. Back\n"
                  << "Choose option: ";

        if (!(std::cin >> option)) {
            HandleInputError();
            continue;
        }

        switch(option) {
            case 1: CreateSequenceMenu<double>(manager, "double"); break;
            case 2: OperationMenu<double>(manager, "double"); break;
            case 3: manager.ShowAllSequences(manager.GetDoubleSequences(), "double"); break;
            case 0: break;
            default: InvalidOption(); break;
        }
    } while (option != 0);
}

void MainMenu() {
    SequenceManager manager;
    int option;
    do {
        std::cout << "\n=== Main Menu ===\n"
                  << "1. Integer sequences\n"
                  << "2. Double sequences\n"
                  << "0. Exit\n"
                  << "Choose option: ";

        if (!(std::cin >> option)) {
            HandleInputError();
            continue;
        }

        switch(option) {
            case 1: IntMenu(manager); break;
            case 2: DoubleMenu(manager); break;
            case 0: 
                manager.ClearAll();
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default: InvalidOption(); break;
        }
    } while (option != 0);
}