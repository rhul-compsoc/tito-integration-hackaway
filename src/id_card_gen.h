#pragma once
#include "CImg.h"
#include "tito_classes.h"

using namespace cimg_library;

#define ID_CARD_READ_ERROR 0x05001
#define wkhtmltopdf_ERROR 0x05002

class IdCard {
public:
    IdCard(TitoAttendee);
    IdCard() {}; // Please do not use me
    void print();
private:
    /**
     * Converts the html file to a pdf file which can then be sent to the printer.
     *
     * @param htmlFile the html file to convert
     * @param pdfFile the output file name
     * @return the return code of wkhtmltopdf
     */
    int convertToPdf(std::string /*htmlFile*/, std::string /*pdfFile*/);
    /**
     * Puts the name into the file in the correct place
     */
    std::string printName();
    /**
     * Generates the qr code for the ticket
     */
    void printQr();
    /**
     * Generates the file name for the image for the id card to be saved as.
     * 
     * Concatenate the ticket id to id_card, this will be fine as there are no
     * duplicate ticket ids. The name of the file is derived from the ticket id to
     * make sure that there are no bad characters in the filename.
     * 
     * @param std::string the file extension
     * @return the filename for the generated id card to be saved as.
     */
    std::string getFileName(std::string /*extension*/);    
    /**
     * The image is copied before it is modified so that it can be saved to the death
     * NOTE: If you @ me on discord or something I will make the tickets not hard
     * coded.
     * 
     * @return the return code, 0 is failure and, 1 is success.
     */
    int copyTemplateImage();
    std::string stripStr(std::string);
    std::string htmlFile;
    TitoAttendee attendee;
};
