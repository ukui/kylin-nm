#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_AppSettings(const uint8_t *data, size_t size);

int main(int argc, char *argv[]) {
    const char *inputFile = nullptr;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        fprintf(stderr, "Test type: fuzz_appsettings - 测试AppSettings配置功能\n");
        return 1;
    }
    
    inputFile = argv[1];

    FILE *f = fopen(inputFile, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file: %s\n", inputFile);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buffer = (uint8_t*)malloc(fsize);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(f);
        return 1;
    }

    size_t read_size = fread(buffer, 1, fsize, f);
    fclose(f);

    LLVMFuzzerTestOneInput_AppSettings(buffer, read_size);

    free(buffer);
    return 0;
}
