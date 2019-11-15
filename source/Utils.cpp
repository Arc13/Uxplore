/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint-gcc.h>
#include "Utils.hpp"

uint16_t Utils::swapEndian16(uint16_t n) {
    return (n >> 8) | (n << 8);
}

uint32_t Utils::swapEndian32(uint32_t n) {
    return ((n >> 24) & 0xFF) | ((n << 24) & 0xFF000000) | ((n << 8) & 0xFF0000) | ((n >> 8) & 0xFF00);
}
