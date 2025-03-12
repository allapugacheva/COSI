#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

#pragma pack(push, 1) // Отключаем выравнивание структур

struct WAVHeader {
    char riff[4] = {'R', 'I', 'F', 'F'};
    uint32_t fileSize = 0;  // Общий размер файла - 8 байт
    char wave[4] = {'W', 'A', 'V', 'E'};

    char fmt[4] = {'f', 'm', 't', ' '};
    uint32_t fmtSize = 16;  // Размер подзаголовка fmt (16 байт для PCM)
    uint16_t audioFormat = 1; // PCM (без сжатия)
    uint16_t numChannels = 1; // 1 канал (моно)
    uint32_t sampleRate = 44100; // Частота дискретизации (Гц)
    uint32_t byteRate = 44100 * 2; // sampleRate * numChannels * bitsPerSample / 8
    uint16_t blockAlign = 2; // numChannels * bitsPerSample / 8
    uint16_t bitsPerSample = 16; // 16 бит на семпл

    char data[4] = {'d', 'a', 't', 'a'};
    uint32_t dataSize = 0; // Размер аудиоданных (заполняем позже)
};

#pragma pack(pop) // Включаем обратно выравнивание структур

void saveWav(const std::string &filename, const std::vector<double> &samples, int sampleRate = 44100);
