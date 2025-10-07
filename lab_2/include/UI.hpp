#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <vector>
#include "Sequence.hpp"
#include "Errors.hpp"

class SequenceManager {
    private:
        static const size_t MAX_SEQ_COUNT = 15;
        std::vector<Sequence<int>*> intSeqs;
        std::vector<Sequence<double>*> doubleSeqs;
    
    public:
        size_t GetIntSequenceCount() const { return intSeqs.size(); }
        size_t GetDoubleSequenceCount() const { return doubleSeqs.size(); }
    
        const std::vector<Sequence<int>*>& GetIntSequences() const { return intSeqs; }
        const std::vector<Sequence<double>*>& GetDoubleSequences() const { return doubleSeqs; }
    
        template <typename T>
        void ShowSequence(const Sequence<T>* seq) const {
            if (!seq) {
                std::cout << "Null sequence\n";
                return;
            }
            std::cout << "[ ";
            for (int i = 0; i < seq->GetLength(); i++)
                std::cout << seq->Get(i) << " ";
            std::cout << "]\n";
        }
    
        template <typename T>
        void ShowAllSequences(const std::vector<Sequence<T>*>& sequences, const std::string& typeName) const {
            if (sequences.empty()) {
                std::cout << "No " << typeName << " sequences available\n";
                return;
            }
            std::cout << "\n=== " << typeName << " Sequences ===\n";
            for (size_t i = 0; i < sequences.size(); i++) {
                std::cout << "Sequence #" << i << ": ";
                ShowSequence(sequences[i]);
            }
        }
    
        bool CanAddSequence() const {
            return intSeqs.size() + doubleSeqs.size() < MAX_SEQ_COUNT;
        }
    
        void AddIntSequence(Sequence<int>* seq) {
            if (intSeqs.size() >= MAX_SEQ_COUNT) THROW_ERROR(INDEX_OUT_OF_RANGE);
            intSeqs.push_back(seq);
        }
    
        void AddDoubleSequence(Sequence<double>* seq) {
            if (doubleSeqs.size() >= MAX_SEQ_COUNT) THROW_ERROR(INDEX_OUT_OF_RANGE);
            doubleSeqs.push_back(seq);
        }
    
        Sequence<int>* GetIntSequence(size_t index) {
            if (index >= intSeqs.size()) THROW_ERROR(INDEX_OUT_OF_RANGE);
            return intSeqs[index];
        }
    
        Sequence<double>* GetDoubleSequence(size_t index) {
            if (index >= doubleSeqs.size()) THROW_ERROR(INDEX_OUT_OF_RANGE);
            return doubleSeqs[index];
        }
    
        void ClearAll() {
            intSeqs.clear();
            doubleSeqs.clear();
            std::cout << "All sequences cleared\n";
        }
};

void MainMenu();

#endif // UI_HPP