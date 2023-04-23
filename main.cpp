#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <chrono>

#include "include/bitarray.hpp"
#include "include/packedint.hpp"

void packedint_queries(std::istream& in, bool timing = false, bool memory = false) {
    std::cout << std::flush;
    std::cerr << std::flush;

    if (!in) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    uint64_t n, k;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    in.read(reinterpret_cast<char*>(&k), sizeof(k));

    pfp::PackedIntegerArray packed_int_arr(n, k);

    const size_t chunk_size = 4;
    uint64_t values[chunk_size];

    auto start_set = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < n; i += chunk_size) {
        size_t count = std::min(chunk_size, n - i);
        in.read(reinterpret_cast<char*>(values), count * sizeof(uint64_t));
        for (size_t j = 0; j < count; ++j) {
            packed_int_arr.set(i + j, values[j]);
        }
    }
    auto end_set = std::chrono::high_resolution_clock::now();

    auto start_get = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < n; i += chunk_size) {
        size_t count = std::min(chunk_size, n - i);
        in.read(reinterpret_cast<char*>(values), count * sizeof(uint64_t));
        for (size_t j = 0; j < count; ++j) {
            std::cout << packed_int_arr.get(values[j]) << '\n';
        }
    }
    auto end_get = std::chrono::high_resolution_clock::now();

    if (timing) {
        auto set_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_set - start_set).count();
        auto get_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_get - start_get).count();
        std::cerr << set_duration << '\n';
        std::cerr << get_duration << '\n';
    }
    
    if (memory) packed_int_arr.memory_usage();

    std::cout << std::flush;
    std::cerr << std::flush;
}

void get_set(std::istream& in,
             bool timing = false, 
             bool sum_queries = false,
             bool location_queries = false,
             bool partial_queries = false,
             bool memory = false) {
    std::cout << std::flush;
    std::cerr << std::flush;

    if (!in) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    uint64_t n, m;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    in.read(reinterpret_cast<char*>(&m), sizeof(m));

    pfp::bit_array bit_array(m, sum_queries || location_queries || partial_queries);

    const size_t chunk_size = 4;
    uint64_t values[chunk_size];

    auto start_set = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < n; i += chunk_size) {
        size_t count = std::min(chunk_size, n - i);
        in.read(reinterpret_cast<char*>(values), count * sizeof(uint64_t));
        for (size_t j = 0; j < count; ++j) {
            bit_array.set(values[j]);
        }
    }
    auto end_set = std::chrono::high_resolution_clock::now();

    auto start_get = std::chrono::high_resolution_clock::now();
    if (sum_queries || location_queries || partial_queries) bit_array.init_partial_sums();
    for (uint64_t i = 0; i < n; i += chunk_size) {
        size_t count = std::min(chunk_size, n - i);
        in.read(reinterpret_cast<char*>(values), count * sizeof(uint64_t));
        for (size_t j = 0; j < count; ++j) {
            if (location_queries) {
            //     std::cout << bit_array.location(values[j]) << '\n';
            // } else if (partial_queries) {
                std::cout << bit_array.location_partial(values[j]) << '\n';
            } else if (sum_queries) {
                std::cout << bit_array.sum(values[j]) << '\n';
            } else {
                std::cout << bit_array.get(values[j]) << '\n';
            }
        }
    }
    auto end_get = std::chrono::high_resolution_clock::now();

    if (timing) {
        auto set_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_set - start_set).count();
        auto get_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_get - start_get).count();
        std::cerr << set_duration << "\n";
        std::cerr << get_duration << "\n";
    }

    if (memory) bit_array.memory_usage();

    std::cout << std::flush;
    std::cerr << std::flush;
}

int main(int argc, char const* argv[]) {
    int input_file = 0;
    bool timing = false;
    bool task1 = false;
    bool sum_queries = false;
    bool location_queries = false;
    bool packed_int = false;
    bool memory = false;
    bool partial_queries = false;

    int i = 1;
    while (i < argc) {
        std::string s(argv[i++]);
        if (s.compare("-t") == 0) {
            timing = true;
        } else if (s.compare("-b") == 0) {
            task1 = true;
        } else if (s.compare("-s") == 0) {
            sum_queries = true;
        } else if (s.compare("-l") == 0) {
            location_queries = true;
        // } else if (s.compare("-p") == 0) {
        //     partial_queries = true;
        } else if (s.compare("-i") == 0) {
            packed_int = true;
        } else if (s.compare("-m") == 0) {
            memory = true;
        } else {
            input_file = i - 1;
        }
    }

    // input_file = 1;
    // argv[input_file] = "b22.txt";
    // location_queries = true;

    std::ifstream in = std::ifstream(argv[input_file], std::ios::binary);
    if (packed_int) {
        if (input_file > 0) {
            packedint_queries(in, timing);
        } else {
            packedint_queries(std::cin, timing, memory);
        }
    } else {
        if (input_file > 0) {
            get_set(in, timing, sum_queries, location_queries, partial_queries, memory);
        } else {
            get_set(std::cin, timing, sum_queries, location_queries, partial_queries, memory);
        }
    }

    return 0;
}