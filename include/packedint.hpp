namespace pfp {
    class PackedIntegerArray {
    public:
        PackedIntegerArray(size_t n, size_t k) : _n(n), _k(k) {
            if (_k > 64) [[unlikely]] {
                throw std::invalid_argument("k must be less than or equal to 64");
            }
            _size = (_n * _k + 63) >> 6;
            _data = new uint64_t[_size]();

            _mask = (1ULL << _k) - 1;
            _offset_mask = 63;
        }
        ~PackedIntegerArray() {
            delete[] _data;
        }

        uint64_t get(size_t index) const {
            if (index >= _n) [[unlikely]] {
                throw std::out_of_range("Index out of range");
            }

            size_t bit_pos = index * _k;
            size_t data_index = bit_pos >> 6;
            size_t offset = bit_pos & _offset_mask;

            uint64_t value = (_data[data_index] >> offset) & _mask;
            if (offset + _k > 64 && data_index + 1 < _size) {
                value |= (_data[data_index + 1] << (64 - offset)) & _mask;
            }
            return value;
        }

        void set(size_t index, uint64_t value) {
            if (index >= _n) [[unlikely]] {
                throw std::out_of_range("Index out of range");
            }
            if (value >= (1ULL << _k)) [[unlikely]] {
                throw std::invalid_argument("Value out of range");
            }

            size_t bit_pos = index * _k;
            size_t data_index = bit_pos >> 6;
            size_t offset = bit_pos & _offset_mask;

            _data[data_index] &= ~(_mask << offset);
            _data[data_index] |= (value << offset) & (_mask << offset);
            if (offset + _k > 64 && data_index + 1 < _size) {
                _data[data_index + 1] &= ~((1ULL << (offset + _k - 64)) - 1);
                _data[data_index + 1] |= (value >> (64 - offset)) & ((1ULL << (offset + _k - 64)) - 1);
            }
        }

        void memory_usage() const {
            size_t dynamic_memory = malloc_usable_size(_data);
            size_t object_memory = sizeof(*this);
            std::cerr << "Usage of _data: " << dynamic_memory << "\n";
            std::cerr << "Usage of object: " << object_memory << "\n";
            std::cerr << "Total usage: " << dynamic_memory + object_memory << "\n";
        }

    private:
        size_t _n;
        size_t _k;
        uint64_t* _data;
        uint64_t _size;
        uint64_t _mask;
        uint64_t _offset_mask;
    };
}
