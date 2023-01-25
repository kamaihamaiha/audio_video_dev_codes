#include <cstdlib>
#include <iostream>
#include <string>

#include "io_data.h"
#include "video_decoder_core.h"

using namespace std;

static void usage(const char *program_name) {
  cout << "usage: " << string(program_name) << " input_file output_file" << endl;
}

int main(int argc, char** argv){

  if (argc < 3) {
    usage(argv[0]);
    return -1;
  }

  char *input_file_name = argv[1];
  char *output_file_name = argv[2];

  cout << "Input file: " << string (input_file_name) << endl;
  cout << "Output file: " << string (output_file_name) << endl;

  int32_t ret = open_input_output_files(input_file_name, output_file_name);
  if (ret < 0) return showError(ret, "")

  // ...
  ret = init_video_decoder();
  if (ret < 0) return showError(-1, "");

  ret = decoding();
  if (ret < 0) return showError(ret, "");


  destroy_video_decoder();
  close_input_output_files();
  
  return 0;
}