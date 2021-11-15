#pragma once
#include "CImg.h"
#include "tito_classes.h"

using namespace cimg_library;

#define ID_CARD_READ_ERROR 0x05001

class IdCard {
public:
    IdCard(TitoAttendee);
    IdCard() {}; // Please do not use me
    void print();
private:
    void printName();    
    /**
     * Generates the file name for the image for the id card to be saved as.
     * 
     * Concatenate the ticket id to id_card, this will be fine as there are no
     * duplicate ticket ids. The name of the file is derived from the ticket id to
     * make sure that there are no bad characters in the filename.
     * 
     * @param TitoAttendee the attendee to generate the ticket for.
     * @return the filename for the generated id card to be saved as.
     */
    std::string getFileName();    
    /**
     * The image is copied before it is modified so that it can be saved to the death
     * NOTE: If you @ me on discord or something I will make the tickets not hard
     * coded.
     * 
     * @param TitoAttendee the attendee to create the id card for.
     * @param std::string the filename for the new id card to be saved as.
     * 
     * @return the return code, 0 is failure and, 1 is success.
     */
    int copyTemplateImage();    
    CImg<unsigned char> image;
    TitoAttendee attendee;
};
