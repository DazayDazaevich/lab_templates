#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>
#include <utility>
#include <stdexcept>

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
        try {
            data[0] = t;
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    Grid(size_type y_size, size_type x_size) : y_size(y_size), x_size(x_size) {
        if (y_size == 0 || x_size == 0) {
            throw std::invalid_argument("размеры сетки не могут быть нулевыми");
        }
        
        data = new T[y_size * x_size];
        try {
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = T();
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    Grid(size_type y_size, size_type x_size, T const &t) : y_size(y_size), x_size(x_size) {
        if (y_size == 0 || x_size == 0) {
            throw std::invalid_argument("размеры сетки не могут быть нулевыми");
        }
        
        data = new T[y_size * x_size];
        try {
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = t;
            }
        } catch (...) {
            delete[] data;
            throw;
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
        try {
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = other.data[i];
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            T* new_data = new T[other.y_size * other.x_size];
            try {
                for (size_type i = 0; i < other.y_size * other.x_size; ++i) {
                    new_data[i] = other.data[i];
                }
            } catch (...) {
                delete[] new_data;
                throw;
            }
            
            delete[] data;
            data = new_data;
            y_size = other.y_size;
            x_size = other.x_size;
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept 
        : data(std::exchange(other.data, nullptr)), 
          y_size(std::exchange(other.y_size, 0)), 
          x_size(std::exchange(other.x_size, 0)) {}

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            Grid temp(std::move(other));
            swap(temp);
        }
        return *this;
    }

    // теперь тут swap
    void swap(Grid& other) noexcept {
        std::swap(data, other.data);
        std::swap(y_size, other.y_size);
        std::swap(x_size, other.x_size);
    }

    // операторы доступа 
    T const& operator()(size_type y_idx, size_type x_idx) const {
        if (y_idx >= y_size || x_idx >= x_size) {
            throw std::out_of_range("индексы выходят за границы сетки");
        }
        return data[y_idx * x_size + x_idx];
    }   

    T& operator()(size_type y_idx, size_type x_idx) {
        if (y_idx >= y_size || x_idx >= x_size) {
            throw std::out_of_range("индексы выходят за границы сетки");
        }
        return data[y_idx * x_size + x_idx];
    }

    // оператор присваивания 
    Grid& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    // геттеры
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
            if (x_idx >= grid.x_size) {
                throw std::out_of_range("индекс столбца выходит за границы");
            }
            return grid.data[y_idx * grid.x_size + x_idx];
        }
    };

    class ConstRowProxy {
    private:
        const Grid& grid;
        size_type y_idx;
    public:
        ConstRowProxy(const Grid& grid, size_type y_idx) : grid(grid), y_idx(y_idx) {}
        T const& operator[](size_type x_idx) const {
            if (x_idx >= grid.x_size) {
                throw std::out_of_range("индекс столбца выходит за границы");
            }
            return grid.data[y_idx * grid.x_size + x_idx];
        }
    };

    RowProxy operator[](size_type y_idx) {
        if (y_idx >= y_size) {
            throw std::out_of_range("индекс строки выходит за границы");
        }
        return RowProxy(*this, y_idx);
    }

    ConstRowProxy operator[](size_type y_idx) const {
        if (y_idx >= y_size) {
            throw std::out_of_range("индекс строки выходит за границы");
        }
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
    // конструкторы с обработкой исключений
    Grid(T const &t) : size(1) {
        data = new T[1];
        try {
            data[0] = t;
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    Grid(size_type size) : size(size) {
        if (size == 0) {
            throw std::invalid_argument("размер сетки не может быть нулевым");
        }
        
        data = new T[size];
        try {
            for (size_type i = 0; i < size; ++i) {
                data[i] = T();
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    Grid(size_type size, T const &t) : size(size) {
        if (size == 0) {
            throw std::invalid_argument("размер сетки не может быть нулевым");
        }
        
        data = new T[size];
        try {
            for (size_type i = 0; i < size; ++i) {
                data[i] = t;
            }
        } catch (...) {
            delete[] data;
            throw;
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
        try {
            for (size_type i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            T* new_data = new T[other.size];
            try {
                for (size_type i = 0; i < other.size; ++i) {
                    new_data[i] = other.data[i];
                }
            } catch (...) {
                delete[] new_data;
                throw;
            }
            
            delete[] data;
            data = new_data;
            size = other.size;
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept 
        : data(std::exchange(other.data, nullptr)), 
          size(std::exchange(other.size, 0)) {}

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            Grid temp(std::move(other));
            swap(temp);
        }
        return *this;
    }

    void swap(Grid& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
    }

    // операторы доступа с проверкой границ
    T const& operator()(size_type i) const {
        if (i >= size) {
            throw std::out_of_range("индекс выходит за границы сетки");
        }
        return data[i];
    }

    T& operator()(size_type i) {
        if (i >= size) {
            throw std::out_of_range("индекс выходит за границы сетки");
        }
        return data[i];
    }

    T const& operator[](size_type i) const {
        if (i >= size) {
            throw std::out_of_range("индекс выходит за границы сетки");
        }
        return data[i];
    }

    T& operator[](size_type i) {
        if (i >= size) {
            throw std::out_of_range("индекс выходит за границы сетки");
        }
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
    // конструкторы с обработкой исключений
    Grid(size_type z_size, size_type y_size, size_type x_size) 
        : z_size(z_size), y_size(y_size), x_size(x_size) {
        if (z_size == 0 || y_size == 0 || x_size == 0) {
            throw std::invalid_argument("размеры сетки не могут быть нулевыми");
        }
        
        data = new T[z_size * y_size * x_size];
        try {
            for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
                data[i] = T();
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    Grid(size_type z_size, size_type y_size, size_type x_size, T const &t) 
        : z_size(z_size), y_size(y_size), x_size(x_size) {
        if (z_size == 0 || y_size == 0 || x_size == 0) {
            throw std::invalid_argument("размеры сетки не могут быть нулевыми");
        }
        
        data = new T[z_size * y_size * x_size];
        try {
            for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
                data[i] = t;
            }
        } catch (...) {
            delete[] data;
            throw;
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
        try {
            for (size_type i = 0; i < z_size * y_size * x_size; ++i) {
                data[i] = other.data[i];
            }
        } catch (...) {
            delete[] data;
            throw;
        }
    }

    // 3. оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            T* new_data = new T[other.z_size * other.y_size * other.x_size];
            try {
                for (size_type i = 0; i < other.z_size * other.y_size * other.x_size; ++i) {
                    new_data[i] = other.data[i];
                }
            } catch (...) {
                delete[] new_data;
                throw;
            }
            
            delete[] data;
            data = new_data;
            z_size = other.z_size;
            y_size = other.y_size;
            x_size = other.x_size;
        }
        return *this;
    }

    // 4. конструктор перемещения
    Grid(Grid &&other) noexcept 
        : data(std::exchange(other.data, nullptr)),
          z_size(std::exchange(other.z_size, 0)),
          y_size(std::exchange(other.y_size, 0)),
          x_size(std::exchange(other.x_size, 0)) {}

    // 5. оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            Grid temp(std::move(other));
            swap(temp);
        }
        return *this;
    }

    void swap(Grid& other) noexcept {
        std::swap(data, other.data);
        std::swap(z_size, other.z_size);
        std::swap(y_size, other.y_size);
        std::swap(x_size, other.x_size);
    }

    // операторы доступа с проверкой границ
    T const& operator()(size_type z_idx, size_type y_idx, size_type x_idx) const {
        if (z_idx >= z_size || y_idx >= y_size || x_idx >= x_size) {
            throw std::out_of_range("индексы выходят за границы сетки");
        }
        return data[z_idx * y_size * x_size + y_idx * x_size + x_idx];
    }

    T& operator()(size_type z_idx, size_type y_idx, size_type x_idx) {
        if (z_idx >= z_size || y_idx >= y_size || x_idx >= x_size) {
            throw std::out_of_range("индексы выходят за границы сетки");
        }
        return data[z_idx * y_size * x_size + y_idx * x_size + x_idx];
    }

    // оператор присваивания значения
    Grid& operator=(T const &t) {
        for (auto it = data, end = data + z_size * y_size * x_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    // геттеры
    size_type get_z_size() const { return z_size; }
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

    // оператор индексирования для 3d сетки
    Grid<T, 2> operator[](size_type z_idx) const {
        if (z_idx >= z_size) {
            throw std::out_of_range("индекс z выходит за границы");
        }
        
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
    try {
        std::cout << "проверка задания 1" << std::endl;
        
        // разные сетки
        Grid<int, 2> small(5);
        print_grid(small, "одинарная сетка");
        
        Grid<int, 2> empty(3, 2);
        print_grid(empty, "пустая ");
        
        Grid<float, 2> filled(2, 3, 4242.42f);
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

    } catch (const std::exception& e) {
        std::cerr << "ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}