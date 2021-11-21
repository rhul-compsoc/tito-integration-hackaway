#include "id_card_gen.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "qrcodegen.hpp"

#ifdef DEBUG
#ifndef TEST
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#endif
#endif

#define ASSETS_FOLDER "./assets"
#define FONT_FILE ASSETS_FOLDER "/font.png"
#define GLYPHS_IN_FILE 27
// NOTE: Dear users, please make your templates the same size and change these numbers
#define ID_CARD_WIDTH 2000
#define ID_CARD_HEIGHT 3173
#define TEXT_OPACITY 1
#define TEXT_SIZE_HEIGHT 300
#define TEXT_SIZE_WIDTH TEXT_SIZE_HEIGHT / 2
#define TEXT_Y 1800
#define QR_Y 2700
#define QR_BLOCK_WIDTH 15
unsigned char __TEXT_COLOUR__[] = {0xFF, 0xFF, 0xFF};
#define TEXT_COLOUR __TEXT_COLOUR__
#define MAX_NAME_LEN 11

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

CImgList<unsigned char> font;

void loadFont()
{
    CImg<unsigned char> fontRaw = CImg<unsigned char> (FONT_FILE);
    font = fontRaw.get_split('x', GLYPHS_IN_FILE);
}

CImg<unsigned char> getGlyph(char c)
{
    int index = -1;
    if (c >= 'A' || c <= 'Z') {
        index = c - 'A';
    } else if (c >= 'a' || c <= 'z') {
        index = c - 'a';
    } else if (c == '-') {
        index = 26;
    }
    
    if (index < 0 || index >= (int) font.size()) {
        return CImg<unsigned char>(TEXT_SIZE_WIDTH, TEXT_SIZE_HEIGHT);
    } else {
        return font.at(index);
    }
}

IdCard::IdCard(TitoAttendee attendee)
{
    this->attendee = attendee;
    std::string fileName = getFileName();
    if (!this->copyTemplateImage()) {
        throw ID_CARD_READ_ERROR;
    }
    this->image = CImg<unsigned char> (fileName.c_str());
    this->printName();
    this->printQr();
    
    this->image.save(fileName.c_str());
    
#ifdef DEBUG
#ifndef TEST
    // Ignore this war crime
    int pid = fork();
    if (pid != 0) {
        CImgDisplay main_disp(this->image, "Preview of the id card");
        
        while (!main_disp.is_closed()) {
            main_disp.wait();
        }
        
        kill(pid, SIGSEGV);
    }
#endif
#endif
}

int IdCard::copyTemplateImage()
{
    std::string newFileName = this->getFileName();
    std::string ticketRelease = this->attendee.getTicket().getTicketRelease();    
    std::string releaseTag = "hacker.png";   
    if (ticketRelease == "Mentor") releaseTag = "mentor.png";
    else if (ticketRelease == "Staff") releaseTag = "staff.png";
    else if (ticketRelease == "Committee") releaseTag = "staff.png";
    //TODO: create a comittee ticket template?
    
    std::string source = ASSETS_FOLDER "/" + releaseTag;
    
    FILE *src = fopen(source.c_str(), "r"),
    *dest = fopen(newFileName.c_str(), "wb");
    
    // Handle IO errors
    if (src == NULL) {
        std::cerr << "Error IdCard::copyTemplateImage() : Error copying "
                  << source << " to "
                  << newFileName << ". Source file could not be opened."
        << std::endl;
        return 0;
    }
    
    if (dest == NULL) {
        std::cerr << "Error IdCard::copyTemplateImage() : Error copying "
                  << source << " to "
                  << newFileName << ". Destination file could not be opened."
                  << std::endl;
        return 0;
    }
    
    for (int c; c = fgetc(src), c != EOF;) {
        int t = fputc(c, dest);
        
        // Handle IO errors
        if (t == EOF) {            
            std::cerr << "Error IdCard::copyTemplateImage() : Error copying "
                      << source << " to "
                      << newFileName << ". A write error occurred."
                      << std::endl;
            
            fclose(src);
            fclose(dest);
            return 0;
        }
    }
    
    #ifdef DEBUG
    std::cerr << "Debug IdCard::copyTemplateImage() : Copied "
              << source << " to "
              << newFileName << std::endl;
    #endif
    
    fclose(src);
    fclose(dest);
    return 1;
}

std::string IdCard::getFileName()
{
    return "id_card_" + std::to_string(this->attendee.getTicket().getTicketID()) + ".png";
}

void IdCard::printName()
{
    std::string fname = stripAttendeeName(this->attendee.getForename()),
                sname = stripAttendeeName(this->attendee.getSurname());
    this->printText(fname, TEXT_Y);
    this->printText(sname, TEXT_Y + TEXT_SIZE_HEIGHT + 30);
}

void IdCard::printText(std::string text, int yOffset)
{
    int xOffset = (ID_CARD_WIDTH - (TEXT_SIZE_WIDTH * text.size())) / 2;
    
    for (size_t i = 0; i < text.size(); i ++) {
        char c = text.c_str()[i];
        if ((c >= 'A' && c <= 'Z') || c == ' ') {
            CImg<unsigned char> glyph = getGlyph(c);
            int printX = (i * TEXT_SIZE_WIDTH) + xOffset;
            for (int x = 0; x < TEXT_SIZE_WIDTH; x++) {
                for (int y = 0; y < TEXT_SIZE_HEIGHT; y++) {
                    if (glyph.atXY(x, y) == 0xFF) {
                        this->image.draw_rectangle(printX + x, y + yOffset,
                                                   printX + x, y + yOffset,
                                                   TEXT_COLOUR);
                    }
                }
            }
        }
    }
}

void IdCard::printQr()
{
    std::string slugStr = this->attendee.getTicket().getTicketSlug();
    QrCode qr = QrCode::encodeText(slugStr.c_str(),
                                   QrCode::Ecc::HIGH);
    int xOffset = (ID_CARD_WIDTH - (qr.getSize() * QR_BLOCK_WIDTH)) / 2;
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            unsigned char colour[] = {0x2E, 0X34, 0X40};
            if (qr.getModule(x, y)) {
                colour[0] = colour[1] = colour[2] = 0xFF;
            }

            int printX = xOffset + (x * QR_BLOCK_WIDTH);
            int printY = QR_Y + (y * QR_BLOCK_WIDTH);

            this->image.draw_rectangle(printX,
                                       printY,
                                       printX + QR_BLOCK_WIDTH,
                                       printY + QR_BLOCK_WIDTH,
                                       colour,
                                       1);
        }
    }
}

std::string IdCard::stripAttendeeName(std::string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    std::string ret = "";
    
    for (char c : str) {
        if ((c >= '0' && c <'9') || (c >= 'A' || c <= 'Z') || c == ' ') {
            ret += c;
        }
    }
    
    std::string retOut = ret;
    
    /*
     * Truncate big names after their first name then add a '.' after the first
     * letter of their surname.
     */
    if (retOut.size() > MAX_NAME_LEN) {
        size_t index = ret.find(" ");
        if (index != std::string::npos) {
            retOut = ret.substr(0, index + 2);
            retOut += ".";
        }
    }
    
    /*
     * Truncates big names after the end of their first name
     */
    if (retOut.size() > MAX_NAME_LEN) {
        size_t index = ret.find(" ");
        if (index != std::string::npos) {
            retOut = ret.substr(0, index - 1);
        }
    }
    
    // Bruh - just cut the name off here you know
    if (retOut.size() > MAX_NAME_LEN) {
        retOut = ret.substr(0, MAX_NAME_LEN - 3);
        retOut += "...";
    }
    
    return retOut;
}

static void *print_image_thread(void *name)
{    
    std::string *fileName = (std::string *) name;
    std::string command = "lp -o media=a6 \"" + *fileName + "\"";
    int status = system(command.c_str());
    if (!status) {
        std::cerr << "Error IdCard::print_image_thread : lp returned non-zero value "
                  << std::to_string(status)
                  << std::endl;
    }
    delete fileName;
    pthread_exit(0);    
}

void IdCard::print()
{
    std::string *name = new std::string(this->getFileName());
    pthread_t thread;
    pthread_attr_t *attr = NULL;
    int r = pthread_create(&thread, attr, &print_image_thread, (void *) name);
    
    if (r != 0) {
        delete name;
        std::cerr << "Critical Failure: ";
        std::cerr << "Error IdCard::print() : Cannot start a thread to print the id card."
                  << std::endl;
        exit(13);
    }
}
