/*
 * This file is part of the wawan-ikhwan project.
 * Copyright (c) 2022 Muhammad Ikhwan Perwira <ikhwanperwira@gmail.com>
 *
 * uwc_eol_remover.
 *
 */

#ifndef __UWC_EOL_REMOVER_H__
#define __UWC_EOL_REMOVER_H__

#include "string.h"

/**
 * @brief remove CR or LF or CRLF character in end of string.
 */
void uwc_eol_remover(char* src);

#endif