#include "wavsaver.h"

void saveWav(const std::string &filename, const std::vector<double> &samples, int sampleRate) {

    WAVHeader header;
    header.sampleRate = sampleRate;
    header.byteRate = sampleRate * header.numChannels * (header.bitsPerSample / 8);
    header.blockAlign = header.numChannels * (header.bitsPerSample / 8);

    std::vector<int16_t> intSamples;
    intSamples.reserve(samples.size());

    // Преобразование double [-1,1] в int16 [-32768,32767]
    for (double sample : samples) {
        int16_t intSample = static_cast<int16_t>(sample * 32767.0);
        intSamples.push_back(intSample);
    }

    // Заполнение размеров
    header.dataSize = intSamples.size() * sizeof(int16_t);
    header.fileSize = header.dataSize + sizeof(WAVHeader) - 8;

    // Запись в файл
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
    file.write(reinterpret_cast<const char*>(intSamples.data()), header.dataSize);
    file.close();
}
