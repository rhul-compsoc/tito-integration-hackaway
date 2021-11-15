#pragma once
#include "CImg.h"
#include "tito_classes.h"

using namespace cimg_library;

class IdCard {
public:
    IdCard(TitoAttendee);
private:
    void saveIdCard();
    void printIdCard();
    CImg<unsigned char> image;
};
