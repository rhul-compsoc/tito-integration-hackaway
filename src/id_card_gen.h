#pragma once
#include "CImg.h"
#include "tito_classes.h"

using namespace cimg_library;

#define ID_CARD_READ_ERROR 0x05001

class IdCard {
public:
    IdCard(TitoAttendee);
    IdCard(); // Please do not use me
    int print();
private:
    CImg<unsigned char> image;
};
