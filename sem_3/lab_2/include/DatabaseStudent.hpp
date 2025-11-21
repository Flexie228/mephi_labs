#ifndef DATABASE_STUDENT
#define DATABASE_STUDENT
#define CACHE_SIZE 1000
#define DELETED_COUNT_MAX 1000

#include <vector>
#include <memory>
#include <unordered_map>
#include "Student.hpp"
#include "IDictionary.hpp"
#include "Cache.hpp"
using namespace std;

class DataBaseStudent {
private:
    vector<pair<string, unique_ptr<Student>>> data;
    size_t deletedCount = 0;
    Cache<string, Student> cache{CACHE_SIZE};
    
    // Вторичные индексы
    IDictionaryHT<string, string> nameIndex;           // индекс по имени
    IDictionaryHT<string, string> surnameIndex;        // индекс по фамилии
    IDictionaryHT<Phone, string> phoneIndex;           // индекс по телефону
    unordered_map<string, Student*> idIndex;            // индекс по id (УНИКАЛЬНЫЕ КЛЮЧИ)
    IDictionaryAVL<size_t, string> ageIndex;           // индекс по возрасту
    static const Student DELETED_MARKER;

    static void markAsDeleted(Student* student) { *student = DELETED_MARKER; }
    static bool isDeleted(const Student* student) { return student->age == 0 && student->name == "DELETED"; }
    void lazyDeletion() {
        if (deletedCount < DELETED_COUNT_MAX) return;
        const auto newEnd = std::remove_if(data.begin(), data.end(),
            [](const auto& pair) { return isDeleted(pair.second.get());});

        data.erase(newEnd, data.end());
        deletedCount = 0;

        //rehash
        idIndex.clear();
        for (const auto& pair : data) {
            idIndex[pair.first] = pair.second.get();
        }
    }
    // Вспомогательные методы для работы с индексами
    void addToIndexes(const Student* student, const string& id) {
        nameIndex.Add(student->name, id);
        surnameIndex.Add(student->surname, id);
        phoneIndex.Add(student->phone, id);
        ageIndex.Add(student->age, id);
    }

    void removeFromIndexes(const Student* student, const string& id) {
        nameIndex.Remove(student->name, id);
        surnameIndex.Remove(student->surname, id);
        phoneIndex.Remove(student->phone, id);
        ageIndex.Remove(student->age, id);
    }

    Student* findStudentByID(const string& id) {
        auto it = idIndex.find(id);
        if (it != idIndex.end() && !isDeleted(it->second)) {
            return it->second;
        }
        return nullptr;
    }

public:
    DataBaseStudent() = default;
    explicit DataBaseStudent(vector<pair<string, unique_ptr<Student>>>&& initialData) : data(std::move(initialData)), deletedCount(0) {
        for (const auto& pair : data) {
            const string& id = pair.first;
            Student* student = pair.second.get();

            idIndex[id] = student;
            nameIndex.Add(student->name, id);
            surnameIndex.Add(student->surname, id);
            phoneIndex.Add(student->phone, id);
            ageIndex.Add(student->age, id);
        }
    }
    DataBaseStudent(const DataBaseStudent&) = delete;
    DataBaseStudent& operator=(const DataBaseStudent&) = delete;
    ~DataBaseStudent() = default;
    
    // Добавление студента в базу
    // Генерирует id, сохраняет в основное хранилище, обновляет индексы и кэш
    string addStudent(Student student) {
        string id = generateID();
        auto studentPtr = make_unique<Student>(std::move(student));
        Student* rawPtr = studentPtr.get();

        data.emplace_back(id, std::move(studentPtr));

        idIndex[id] = rawPtr;
        addToIndexes(rawPtr, id);
        cache.Put(id, *rawPtr);
        return id;
    }

    void removeStudent(const string& id) {
        Student* student = findStudentByID(id);
        if (!student) return;

        removeFromIndexes(student, id);
        cache.Remove(id);
        markAsDeleted(student);
        deletedCount++;
        lazyDeletion();
    }
    
    // Обновление данных студента
    // Находит по id, обновляет данные, перестраивает индексы, обновляет кэш
    void updateStudent(const string& id, const Student& newData) {
        Student* student = findStudentByID(id);
        if (!student) return;

        removeFromIndexes(student, id);
        *student = newData;

        addToIndexes(student, id);
        cache.Put(id, *student);
    }

    // Методы поиска по одному параметру
    // Проверяют кэш(если ключи уникальны), ищут в индексах, возвращают копии Student внутри vector, добавляют в кэш(если уникальные ключи)
    vector<Student> findByID(const string& id) {
        auto cached = cache.Get(id);
        if (!cached.empty()) {
            cache.Put(id, cached[0]);
            return cached;
        }

        if (const Student* studentPtr = findStudentByID(id)) {
            cache.Put(id, *studentPtr);
            return vector<Student>{*studentPtr};
        }

        return vector<Student>{};
    }
    vector<Student> findByName(const string& name) {
        const auto resultIDs = nameIndex.Get(name);
        if (resultIDs.empty()) return vector<Student>{};

        vector<Student> result;
        for (const auto& id : resultIDs)
            if (const Student* studentPtr = findStudentByID(id))
                result.push_back(*studentPtr);

        return result;
    }
    vector<Student> findBySurname(const string& surname) {
        const auto resultIDs = surnameIndex.Get(surname);
        if (resultIDs.empty()) return vector<Student>{};

        vector<Student> result;
        for (const auto& id : resultIDs) {
            if (const Student* studentPtr = findStudentByID(id)) {
                cache.Put(id, *studentPtr);
                result.push_back(*studentPtr);
            }
        }
        return result;
    }
    vector<Student> findByPhone(const Phone& phone) {
        const auto resultID = phoneIndex.Get(phone);
        if (resultID.empty()) return vector<Student>{};

        vector<Student> result;
        if (const Student* studentPtr = findStudentByID(resultID[0])) {
            result.push_back(*studentPtr);
            cache.Put(resultID[0], *studentPtr);
        }
        return result;
    }
    vector<Student> findByAge(const size_t age) {
        const auto resultIDs = ageIndex.Get(age);
        if (resultIDs.empty()) return vector<Student>{};

        vector<Student> result;
        for (const auto& id : resultIDs)
            if (const Student* student = findStudentByID(id))
                result.push_back(*student);
        return result;
    }

    vector<Student> findByAgeRange(const size_t minAge, const size_t maxAge) {
        const auto resultIDs = ageIndex.GetRange(minAge, maxAge);
        if (resultIDs.empty()) return vector<Student>{};

        vector<Student> result;
        for (const auto& id : resultIDs)
            if (const Student* student = findStudentByID(id))
                result.push_back(*student);
        return result;
    }

    vector<Student> findByMultipleParams();

    [[nodiscard]] size_t size() const { return data.size() - deletedCount;; };
    static size_t cacheSize() { return CACHE_SIZE; };

    vector<pair<string, Student>> getAllStudents() const {
        vector<pair<string, Student>> result;
        for (const auto& pair : data) {
            if (!isDeleted(pair.second.get())) {
                result.emplace_back(pair.first, *pair.second);
            }
        }
        return result;
    }
    vector<pair<string, Student>> getCacheContents() const {
        return cache.getAll();
    }

    const IDictionaryHT<string, string>& getNameIndex() const { return nameIndex; }
    const IDictionaryHT<string, string>& getSurnameIndex() const { return surnameIndex; }
    const IDictionaryHT<Phone, string>& getPhoneIndex() const { return phoneIndex; }
    const IDictionaryAVL<size_t, string>& getAgeIndex() const { return ageIndex; }
};

#endif