#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

// основной шаблон для n-мерной сетки
template <typename T, size_t N = 2>
class Grid;

// задание 1

template <typename T>
class Grid<T, 2> {
public:
    using value_type = T;
    using size_type = unsigned;

private:
    T* data;
    size_type y_size, x_size;

public:
    // конструкторы
    Grid(T const &t) : y_size(1), x_size(1) {
        data = new T[1];
        data[0] = t;
    }

    Grid(size_type y_size, size_type x_size) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = T();
        }
    }

    Grid(size_type y_size, size_type x_size, T const &t) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }

    // правило пяти

    // 1. деструктор
    ~Grid() {
        delete[] data;
    }

    // 2. конструктор копирования
    Grid(Grid const &other) : y_size(other.y_size), x_size(other.x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            y_size = other.y_size;
            x_size = other.x_size;
            data = new T[y_size * x_size];
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    // операторы доступа
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    // оператор присваивания значения
    Grid& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    // ге отееры
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

    // задание 2
    class RowProxy {
    private:
        Grid& grid;
        size_type y_idx;
    public:
        RowProxy(Grid& grid, size_type y_idx) : grid(grid), y_idx(y_idx) {}
        T& operator[](size_type x_idx) {
            return grid.data[y_idx * grid.x_size + x_idx];
        }
    };

    class ConstRowProxy {
    private:
        const Grid& grid;
        size_type y_idx;
    public:
        ConstRowProxy(const Grid& grid, size_type y_idx) : grid(grid), y_idx(y_idx) {}
        T operator[](size_type x_idx) const {
            return grid.data[y_idx * grid.x_size + x_idx];
        }
    };

    RowProxy operator[](size_type y_idx) {
        return RowProxy(*this, y_idx);
    }

    ConstRowProxy operator[](size_type y_idx) const {
        return ConstRowProxy(*this, y_idx);
    }
};

// задание 3


// специализация для 1d сетки
template <typename T>
class Grid<T, 1> {
public:
    using value_type = T;
    using size_type = unsigned;

private:
    T* data;
    size_type size;

public:
    // конструкторы
    Grid(T const &t) : size(1) {
        data = new T[1];
        data[0] = t;
    }

    Grid(size_type size) : size(size) {
        data = new T[size];
        for (size_type i = 0; i < size; ++i) {
            data[i] = T();
        }
    }

    Grid(size_type size, T const &t) : size(size) {
        data = new T[size];
        for (size_type i = 0; i < size; ++i) {
            data[i] = t;
        }
    }

    // правило пяти


    // 1. деструктор
    ~Grid() {
        delete[] data;
    }

    // 2. конструктор копирования
    Grid(Grid const &other) : size(other.size) {
        data = new T[size];
        for (size_type i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (size_type i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // операторы доступа
    T operator()(size_type i) const {
        return data[i];
    }

    T& operator()(size_type i) {
        return data[i];
    }

    T operator[](size_type i) const {
        return data[i];
    }

    T& operator[](size_type i) {
        return data[i];
    }

    // оператор присваивания значения
    Grid& operator=(T const &t) {
        for (size_type i = 0; i < size; ++i) {
            data[i] = t;
        }
        return *this;
    }

    // геттеры
    size_type get_size() const { return size; }
};

// специализация для 3d сетки
template <typename T>
class Grid<T, 3> {
public:
    using value_type = T;
    using size_type = unsigned;

private:
    T* data;
    size_type z_size, y_size, x_size;

public:
    // конструкторы
    Grid(size_type z_size, size_type y_size, size_type x_size) 
        : z_size(z_size), y_size(y_size), x_size(x_size) {
        data = new T[z_size * y_size * x_size];
        for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
            data[i] = T();
        }
    }

    Grid(size_type z_size, size_type y_size, size_type x_size, T const &t) 
        : z_size(z_size), y_size(y_size), x_size(x_size) {
        data = new T[z_size * y_size * x_size];
        for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
            data[i] = t;
        }
    }

    // правило пяти



    // 1. деструктор
    ~Grid() {
        delete[] data;
    }

    // 2. конструктор копирования
    Grid(Grid const &other) 
        : z_size(other.z_size), y_size(other.y_size), x_size(other.x_size) {
        data = new T[z_size * y_size * x_size];
        for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            z_size = other.z_size;
            y_size = other.y_size;
            x_size = other.x_size;
            data = new T[z_size * y_size * x_size];
            for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept 
        : data(other.data), z_size(other.z_size), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.z_size = 0;
        other.y_size = 0;
        other.x_size = 0;
    }

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            z_size = other.z_size;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.z_size = 0;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    // операторы доступа
    T operator()(size_type z_idx, size_type y_idx, size_type x_idx) const {
        return data[z_idx * y_size * x_size + y_idx * x_size + x_idx];
    }

    T& operator()(size_type z_idx, size_type y_idx, size_type x_idx) {
        return data[z_idx * y_size * x_size + y_idx * x_size + x_idx];
    }

    // оператор присваивания значения
    Grid& operator=(T const &t) {
        for (auto it = data, end = data + z_size * y_size * x_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    // гетте ры
    size_type get_z_size() const { return z_size; }
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

    // оператор индексирования для 3d сетки
    Grid<T, 2> operator[](size_type z_idx) const {
        Grid<T, 2> slice(y_size, x_size);
        for (size_type y = 0; y < y_size; ++y) {
            for (size_type x = 0; x < x_size; ++x) {
                slice(y, x) = (*this)(z_idx, y, x);
            }
        }
        return slice;
    }
};

// функция для печати 2d сетки
template<typename T>
void print_grid(const Grid<T, 2>& grid, const std::string& name = "Grid") {
    std::cout << name << " (" << grid.get_y_size() << "x" << grid.get_x_size() << "):" << std::endl;
    
    for (unsigned y = 0; y < grid.get_y_size(); ++y) {
        std::cout << "| ";
        for (unsigned x = 0; x < grid.get_x_size(); ++x) {
            std::cout << std::setw(6) << grid(y, x) << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "проверка задания 1" << std::endl;
    
    // разные сетки
    Grid<int, 2> small(5);  // 1x1 с числом 5
    print_grid(small, "одинарная сетка");
    
    Grid<int, 2> empty(3, 2);  // 3x2 с нулями
    print_grid(empty, "пустая ");
    
    Grid<float, 2> filled(2, 3, 4242.42f);  // 2x3 
    print_grid(filled, "заполненная");
    
    Grid<int, 2> matrix(3, 3);
    
    int counter = 1;
    for (unsigned y = 0; y < matrix.get_y_size(); ++y) {
        for (unsigned x = 0; x < matrix.get_x_size(); ++x) {
            matrix(y, x) = counter++;
        }
    }
    
    print_grid(matrix);
    
    // массовое присваивание
    Grid<char, 2> char_grid(2, 4, 'E');
    print_grid(char_grid, "сетка символов");
    char_grid = 'Z'; 
    print_grid(char_grid,"после массового присваивания");

    std::cout << "проверка задания 2" << std::endl;
    
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<float>::size_type;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));

    std::cout << "проверка задания 3" << std::endl;
    
    Grid<float, 3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));

    Grid<float, 2> g2(2, 5, 2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));

    return 0;
}