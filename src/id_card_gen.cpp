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

#define ASSETS_FOLDER "assets"
#define QR_BLOCK_WIDTH 5
unsigned char __TEXT_COLOUR__[] = {0xFF, 0xFF, 0xFF};
#define TEXT_COLOUR __TEXT_COLOUR__
#define MAX_NAME_LEN 11

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

IdCard::IdCard(TitoAttendee attendee)
{
    this->attendee = attendee;
    std::string fileName = this->getFileName(".html");
    if (!this->copyTemplateImage()) {
        throw ID_CARD_READ_ERROR;
    }
    
    this->htmlFile = this->printName();
    this->printQr();
    
    FILE *f = fopen(fileName.c_str(), "w");
    if (f == NULL) {
        throw ID_CARD_READ_ERROR;
    }
    
    fprintf(f, "%s", this->htmlFile.c_str());
    fclose(f);
}

int IdCard::copyTemplateImage()
{
    std::string ticketRelease = stripStr(this->attendee.getTicket().getTicketRelease());
    std::string sourceFileName = "default.html";
    this->htmlFile = "";
        
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(ASSETS_FOLDER))) {
        while ((ent = readdir (dir)) != NULL) {
            std::string dir = std::string(ent->d_name);
            size_t index = dir.find_last_of(".html");
            
            if (index != std::string::npos) {
                std::string strippedDir = stripStr(dir.substr(0, index));
                if (strippedDir == ticketRelease) {
                    sourceFileName = dir;
                    break;
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Error IdCard::copyTemplateImage() : Error cannot open ./"
                     ASSETS_FOLDER
                     "/ folder to query image templates."
                  << std::endl;
        return 0;
    }
    
    std::string source = ASSETS_FOLDER "/" + sourceFileName;
    
    FILE *src = fopen(source.c_str(), "r");
    
    // Handle IO errors
    if (src == NULL) {
        std::cerr << "Error IdCard::copyTemplateImage() : Error copying "
                  << source << " to ram. Source file could not be opened."
        << std::endl;
        return 0;
    }    
    
    for (int c; c = fgetc(src), c != EOF;) {
        this->htmlFile += c;
    }
    
#ifdef DEBUG
    std::cerr << "Debug IdCard::copyTemplateImage() : Copied "
              << source << " to ram." << std::endl;
#endif
    
    fclose(src);
    return 1;
}

std::string IdCard::getFileName(std::string extension)
{
    return "id_card_" + std::to_string(this->attendee.getTicket().getTicketID()) 
           + "." + extension;
}

void IdCard::printQr()
{
    const QrCode qr = QrCode::encodeText(this->attendee.getTicket().getTicketSlug().c_str(),
                                         QrCode::Ecc::HIGH);
    std::string filename = this->getFileName(".png");    
    int qrSize = qr.getSize() * QR_BLOCK_WIDTH;
    CImg<unsigned char> image = CImg<unsigned char> (qrSize, qrSize);
    
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            unsigned char colour[] = {0x2E, 0x34, 0x40};
            if (qr.getModule(x, y)) {
                colour[0] = colour[1] = colour[2] = 0xFF;
            }
            
            int printX = x * QR_BLOCK_WIDTH;
            int printY = y * QR_BLOCK_WIDTH;

            image.draw_rectangle(printX,
                                 printY,
                                 printX + QR_BLOCK_WIDTH,
                                 printY + QR_BLOCK_WIDTH,
                                 colour,
                                 1);
        }
    }
    
    image.save(filename.c_str());
}

std::string IdCard::printName()
{
    std::string fname = stripStr(this->attendee.getForename()),
                sname = stripStr(this->attendee.getSurname());
    
    std::string templateTag = "{fname}";
    size_t templateIndex = this->htmlFile.find(templateTag);
    if (templateIndex == std::string::npos) {
        return this->htmlFile;
    }
    
    std::string htmlOut = this->htmlFile.substr(0, templateIndex);
    htmlOut += fname;
    htmlOut += this->htmlFile.substr(templateIndex + templateTag.size(),
                                     this->htmlFile.size() - 1);
    
    templateTag = "{sname}";
    templateIndex = htmlOut.find(templateTag);
    std::string tmp = htmlOut;
    if (templateIndex != std::string::npos) {    
        htmlOut = tmp.substr(0, templateIndex);
        htmlOut += sname;
        htmlOut += tmp.substr(templateIndex + templateTag.size(),
                              htmlOut.size() - 1);
    }
    
    templateTag = "{qr}";
    templateIndex = htmlOut.find(templateTag);
    tmp = htmlOut;
    if (templateIndex != std::string::npos) {    
        htmlOut = tmp.substr(0, templateIndex);
        htmlOut += "<img src=\"" + getFileName(".png")
                + "\" alt=\"qr code\" />";
        htmlOut += tmp.substr(templateIndex + templateTag.size(),
                              htmlOut.size() - 1);
    } 
    
    return htmlOut;
}

std::string IdCard::stripStr(std::string str)
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
    std::string command = "lp \"" + *fileName + "\"";
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
    std::string *name = new std::string(this->getFileName(".html"));
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
