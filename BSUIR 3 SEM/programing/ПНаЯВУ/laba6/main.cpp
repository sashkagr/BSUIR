#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct PGMImage {
    std::string format;
    int width;
    int height;
    int maxGrayValue;
    std::vector<int> pixels;
};
void writePGM(const char* filename,const unsigned char* data, int width, int height) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    // Заголовок PGM файла
    outFile << "P5\n" << width << " " << height << "\n255\n";

    // Запись данных яркости пикселей
    outFile.write(reinterpret_cast<const char*>(data), width * height);

    // Закрытие файла
    outFile.close();
}
PGMImage readPGM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    PGMImage img;

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return img;
    }

    std::string line;
    std::getline(file, img.format); // Считываем формат PGM (P2 или P5)

    // Пропускаем комментарии, начинающиеся с #
    while (std::getline(file, line) && line[0] == '#') {}

    std::istringstream iss(line);
    iss >> img.width >> img.height;

    int maxVal;
    file >> maxVal; // Считываем максимальное значение оттенков серого
    img.maxGrayValue = maxVal;

    int pixel;
    while (file >> pixel) {
        img.pixels.push_back(pixel);
    }

    file.close();

    return img;
}


unsigned char* Bradley_threshold(const unsigned char* src, unsigned char* res, int width, int height) {
    const int S = width/8;
    int s2 = S/2;
    const float t = 0.15;
    unsigned long* integral_image = 0;
    long sum=0;
    int count=0;
    int index;
    int x1, y1, x2, y2;

    //рассчитываем интегральное изображение
    integral_image = new unsigned long [width*height*sizeof(unsigned long*)];

    for (int i = 0; i < width; i++) {
        sum = 0;
        for (int j = 0; j < height; j++) {
            index = j * width + i;
            sum += src[index];
            if (i==0)
                integral_image[index] = sum;
            else
                integral_image[index] = integral_image[index-1] + sum;
        }
    }

//находим границы для локальные областей
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            index = j * width + i;

            x1=i-s2;
            x2=i+s2;
            y1=j-s2;
            y2=j+s2;

            if (x1 < 0)
                x1 = 0;
            if (x2 >= width)
                x2 = width-1;
            if (y1 < 0)
                y1 = 0;
            if (y2 >= height)
                y2 = height-1;

            count = (x2-x1)*(y2-y1);

            sum = integral_image[y2*width+x2] - integral_image[y1*width+x2] -
                  integral_image[y2*width+x1] + integral_image[y1*width+x1];
            if ((long)(src[index]*count) < (long)(sum*(1.0-t)))
                res[index] = 0;
            else
                res[index] = 255;
        }
    }

    delete[] integral_image;
    return res;
}
int main() {
    std::string filename = "image.pgm";

    PGMImage image = readPGM(filename);

    const unsigned char* src = reinterpret_cast<const unsigned char*>(image.pixels.data());

    unsigned char* res = new unsigned char[image.width * image.height];
    res = Bradley_threshold(src, res, image.width, image.height);
    const char* filename1 = "output.pgm";

    writePGM(filename1, res, image.width, image.height);

    delete[] res;
}