#include <iostream>
#include <fstream>
#include <cstring>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <chrono>

using namespace CryptoPP;

void UpdateProgressBar(float progress)
{
    std::string bar;
    int barWidth = 40;

    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            bar += "=";
        else if (i == pos)
            bar += ">";
        else
            bar += " ";
    }

    std::cout << "[" << bar << "] " << int(progress * 100.0) << "%\r";
    std::cout.flush();
}

void Log(const std::string& message)
{
    std::cout << message << std::endl;
}

void EncryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password)
{
    Log("Version 1.0");
    Log("AES256Tools");

    Log("File: " + inputFile);
    Log("Request: Encrypt");

    // 读取输入文件
    std::string plaintext;
    std::ifstream ifs(inputFile, std::ios::binary);
    if (ifs)
    {
        ifs.seekg(0, std::ios::end);
        plaintext.resize(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        ifs.read(&plaintext[0], plaintext.size());
    }
    else
    {
        std::cerr << "无法打开输入文件" << std::endl;
        return;
    }

    // 初始化加密器
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    memcpy(key, password.c_str(), AES::DEFAULT_KEYLENGTH);
    CBC_Mode<AES>::Encryption encryption;
    encryption.SetKeyWithIV(key, key);

    // 加密
    std::string ciphertext;

    auto startTime = std::chrono::high_resolution_clock::now();

    StringSource(plaintext, true,
                 new StreamTransformationFilter(encryption, new StringSink(ciphertext),
                                               new FileSink(outputFile.c_str()),
                                               true, // putMessage
                                               [&](int current, int total)
                                               {
                                                   float progress = static_cast<float>(current) / total;
                                                   UpdateProgressBar(progress);
                                               }));

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    Log("加密完成，耗时 " + std::to_string(duration) + " 毫秒");
}

void DecryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& password)
{
    Log("Version 1.0");
    Log("AES256Tools");

    Log("File: " + inputFile);
    Log("Request: Decrypt");

    // 读取输入文件
    std::string ciphertext;
    std::ifstream ifs(inputFile, std::ios::binary);
    if (ifs)
    {
        ifs.seekg(0, std::ios::end);
        ciphertext.resize(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        ifs.read(&ciphertext[0], ciphertext.size());
    }
    else
    {
        std::cerr << "无法打开输入文件" << std::endl;
        return;
    }

    // 初始化解密器
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    memcpy(key, password.c_str(), AES::DEFAULT_KEYLENGTH);
    CBC_Mode<AES>::Decryption decryption;
    decryption.SetKeyWithIV(key, key);

    // 解密
    std::string plaintext;

    auto startTime = std::chrono::high_resolution_clock::now();

    StringSource(ciphertext, true,
                 new StreamTransformationFilter(decryption, new StringSink(plaintext),
                                               new FileSink(outputFile.c_str()),
                                               true, // putMessage
                                               [&](int current, int total)
                                               {
                                                   float progress = static_cast<float>(current) / total;
                                                   UpdateProgressBar(progress);
                                               }));

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    Log("解密完成，耗时 " + std::to_string(duration) + " 毫秒");
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "命令行
