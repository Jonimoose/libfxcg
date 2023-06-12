#include <cstdio>
#include <cstring>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "zx7.h"

void printHelp() {
  std::cout << "Usage: zx7 [OPTION]... INPUT [OUTPUT]" << std::endl
            << "Compress INPUT with zx7 to OUTPUT." << std::endl
            << std::endl
            << "  -d, --decompress\tdecompress INPUT instead of compressing "
               "it, outputting to stdout if OUTPUT not specified"
            << std::endl
            << "  -h, --help\t\tdisplay this help and exit" << std::endl;
}

class InputParser {
public:
  InputParser(int &argc, char **argv) {
    for (int i = 1; i < argc; ++i)
      this->tokens.push_back(std::string(argv[i]));
  }

  const std::string &getCmdOption(const std::string &option, bool pop = true) {
    std::vector<std::string>::const_iterator itr =
        std::find(this->tokens.begin(), this->tokens.end(), option);

    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
      if (pop)
        this->tokens.erase(itr);

      return *itr;
    }

    static const std::string empty_string("");
    return empty_string;
  }

  bool cmdOptionExists(const std::string &option, bool pop = false) {
    std::vector<std::string>::const_iterator itr =
        std::find(this->tokens.begin(), this->tokens.end(), option);

    if (itr == this->tokens.end())
      return false;

    if (pop)
      this->tokens.erase(itr);

    return true;
  }

  std::vector<std::string> getRemainingOptions() const { return this->tokens; }

private:
  std::vector<std::string> tokens;
};

// int main(int argc, char **argv) {
//   InputParser input(argc, argv);
//   if (input.cmdOptionExists("-h")) {
//     // Do stuff
//   }
//   const std::string &filename = input.getCmdOption("-f");
//   if (!filename.empty()) {
//     // Do interesting things ...
//   }
//   return 0;
// }

enum mode { COMPRESS, DECOMPRESS };

int main(int argc, char **argv) {
  InputParser input(argc, argv);

  try {
    if (argc == 1)
      throw "No arguments provided";

    if (input.cmdOptionExists("--help") || input.cmdOptionExists("-h"))
      throw "--help";

    mode compression_mode = COMPRESS;

    // + is the same as ||, but evaluates both (i.e. removes --decompress and -d
    // if both are used)
    if (input.cmdOptionExists("-d", true) +
        input.cmdOptionExists("--decompress", true))
      compression_mode = DECOMPRESS;

    const std::vector<std::string> files = input.getRemainingOptions();

    if (files.size() == 0)
      throw "No input file";

    if (files.size() == 1 && compression_mode == COMPRESS)
      throw "No output file";

    std::ifstream input_file(files[0], std::ios::binary);
    const std::string input_data((std::istreambuf_iterator<char>(input_file)),
                                 std::istreambuf_iterator<char>());
    const int input_size = input_data.size();

    unsigned char *out_data;
    unsigned int out_size;

    if (compression_mode == COMPRESS) {
      out_size = ZX7Compress((unsigned char *)input_data.data(), input_size,
                             &out_data);
    } else {
      out_size = ZX7GetDecompressedSize((unsigned char *)input_data.data());
      out_data = (unsigned char *)malloc(out_size);
      if (out_data == NULL)
        throw std::runtime_error("Could not allocate output memory.");

      ZX7Decompress((unsigned char *)input_data.data(), out_data, out_size);
    }

    if (files.size() == 1) {
      std::cout << out_data;
    } else {
      std::ofstream output_file(files[1], std::ios::binary);

      output_file.write((char *)out_data, out_size);
    }
  } catch (const char *err) {
    if (strcmp(err, "--help") != 0)
      std::cout << "Error: " << err << "." << std::endl;

    printHelp();
  }

  // std::ifstream compress_file;

  // if (argc == 0)

  // compress_file.open()

  //       char buff[]

  //       compress_file = fopen("")

  //       const char *srcData = "Bonjour, j'mappel docteur cahlos!";
  // unsigned char *outData;
  // int compressedSize = ZX7Compress((const unsigned char *)srcData,
  //                                  strlen(srcData) + 1, &outData);

  // for (int i = 0; i < compressedSize; i++) {
  //   printf("%d,", outData[i]);

  //   if (i % 64 == 63)
  //     printf("\n\t");
  // }

  // unsigned char destData[compressedSize];
  // ZX7Decompress(outData, destData, compressedSize);

  // printf("\n%s\n", destData);

  // return 0;
  return 0;
}
