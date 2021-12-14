#include "serial.hpp"
#include "incubator.hpp"
#include "filedummy.hpp"

[[noreturn]] int main() {
    Incubator<Serial> inc{"/dev/ttyAMA0"};

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
