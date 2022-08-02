#include "uwc_eolremover.h"

void uwc_eol_remover(char* src) {
  while (src[strlen(src) - 1] == '\r' || src[strlen(src) - 1] == '\n') {
    src[strlen(src) - 1] = '\0';  // remove unnecessary EOL
  }
}
