#include <unistd.h>

int main(int argc, char* argv[]) {
    return execl("/bin/sh", nullptr);
}