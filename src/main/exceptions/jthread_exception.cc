#include <iostream>
#include <thread>
#include <stop_token>
#include <stdexcept>

int main() {
    std::jthread jt([](std::stop_token st) {
        try {
            if (!st.stop_requested()) {
                throw std::runtime_error("Exception in jthread");
            }
        } catch (const std::exception& e) {
            std::cerr << "Caught in jthread: " << e.what() << '\n';
        }
    });
    // jt is auto-joined on destruction
    return 0;
}