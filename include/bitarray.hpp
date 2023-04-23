#include <cstring>
#include <vector>
#include <malloc.h>

namespace pfp {
    class bit_array {
    public:
        bit_array(uint64_t size, bool partial = false) {
            _size = size;
            _partial_size = (size + 63) >> 6;
            _bitarray = new uint64_t[_partial_size]();
            if (partial)
                _partial_sums = new uint64_t[_partial_size + 1]();
            else
                _partial_sums = new uint64_t[0];
        }
        ~bit_array() {
            delete[] _bitarray;
            delete[] _partial_sums;
        }

        void set(uint64_t index, bool value = true) {
            if (index > _size) [[unlikely]] {
                throw std::out_of_range("Index out of range");
            }
            if (value)
                _bitarray[index >> 6] |= (1ULL << (index & 63));
            else
                _bitarray[index >> 6] &= ~(1ULL << (index & 63));
        }

        bool get(uint64_t index) const {
            return (_bitarray[index / 64] >> (index % 64)) & 1ULL;
        }

        void init_partial_sums() {
            uint64_t sum = 0;
            for (uint64_t i = 0; i < _partial_size; ++i) {
                sum += __builtin_popcountll(_bitarray[i]);
                if (((i * 64 + 64) % _interval) == 0) {
                    int index = (i * 64 + 64) / _interval;
                    _partial_sums[index] = sum;
                }
            }
        }

        uint64_t sum(uint64_t i) const {
            uint64_t result = _partial_sums[i / _interval];
            for (uint64_t j = (i / _interval) * _interval; j < i; j++) {
                result += get(j);
            }
            return result;
        }
        
        uint64_t location(uint64_t i) const {
            uint64_t left = 0;
            uint64_t right = _size;

            while (left < right) {
                uint64_t mid = left + (right - left) / 2;
                uint64_t mid_sum = sum(mid);

                if (mid_sum < i) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }

            return left;
        }

        uint64_t location_partial(uint64_t i) const {
            uint64_t approx = 0;
    
            if (i <= _partial_sums[0]) [[unlikely]] {
                approx = _partial_sums[0];
            }

            if (i >= _partial_sums[_partial_size]) [[unlikely]] {
                approx = _partial_sums[_partial_size];
            }

            uint64_t left = 0, right = _partial_size, mid = 0;
            while (left < right) {
                mid = (left + right) / 2;
        
                if (_partial_sums[mid] == i) [[unlikely]] {
                    approx = _partial_sums[mid];
                    break;
                }
        
                if (i < _partial_sums[mid]) {
                    if (mid > 0 && i > _partial_sums[mid - 1]) {
                        approx = _partial_sums[mid - 1];
                        break;
                    }
                    right = mid;
                }
                else {
                    if (mid < _partial_size && i < _partial_sums[mid + 1]) {
                        approx = _partial_sums[mid];
                        break;
                    }
                    left = mid + 1;
                }
            }
            approx = mid == 0 ? 0 : mid - 1;
            uint64_t current_sum = _partial_sums[approx];
            approx *= 64;
            while (current_sum < i) {
                current_sum += get(approx++);
            }
            return approx;
        }
        
        void memory_usage() const {
            size_t dynamic_memory_1 = malloc_usable_size(_bitarray);
            size_t dynamic_memory_2 = malloc_usable_size(_partial_sums);
            size_t object_memory = sizeof(*this);
            std::cerr << "Usage of _bitarray: " << dynamic_memory_1 << "\n";
            std::cerr << "Usage of _partial_sums: " << dynamic_memory_2 << "\n";
            std::cerr << "Usage of object: " << object_memory << "\n";
            std::cerr << "Total usage: " << dynamic_memory_1 + dynamic_memory_2 + object_memory << "\n";
        }
    private:
        uint64_t* _bitarray;
        uint64_t _interval = 64;
        uint64_t* _partial_sums;
        uint64_t _size;
        uint64_t _partial_size;
    };
}