#include "id_card_gen.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#ifdef DEBUG
#ifndef TEST
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#endif
#endif

#define ASSETS_FOLDER "assets"
// NOTE: Dear users, please make your templates the same size and change these numbers
#define ID_CARD_WIDTH 2000 
#define ID_CARD_HEIGHT 3173
// Enjoy tuning the text size, turn on DEBUG mode to make it easier for you
#define TEXT_OPACITY 1
#define TEXT_SIZE_HEIGHT 300
#define TEXT_Y 2600
unsigned char __TEXT_COLOUR__[] = {0xFF, 0xFF, 0xFF};
#define TEXT_COLOUR __TEXT_COLOUR__
// Enjoy playing with fonts, they are rather crap
#define TEXT_X 80
#define MAX_NAME_LEN 11

IdCard::IdCard(TitoAttendee attendee)
{
    this->attendee = attendee;
    std::string fileName = getFileName();
    if (!this->copyTemplateImage()) {
        throw ID_CARD_READ_ERROR;
    }    
}

int IdCard::copyTemplateImage()
{
    std::string newFileName = this->getFileName();
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
                std::string strippedDir = stripStr(dir.substr(0, index - 1));
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
        this->htmlFile += c;
        
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
    return "id_card_" + std::to_string(this->attendee.getTicket().getTicketID()) + ".html";
}

std::string IdCard::printName()
{
    std::string fname, sname;    
    size_t nameIndex = this->attendee.getName().find(" ");
    if (nameIndex != std::string::npos) {        
        fname = stripStr(this->attendee.getName()).substr(0, nameIndex - 1);
        sname = stripStr(this->attendee.getName().substr(nameIndex + 1,
                                                         this->attendee.getName().size() - 1));
    } else {
        fname = stripStr(this->attendee.getName());        
    }
    
    std::string templateTag = "{fname}";
    size_t templateIndex = this->htmlFile.find(templateTag);
    if (templateIndex == std::string::npos) {
        return this->htmlFile;
    }
    
    std::string htmlOut = this->htmlFile.substr(0, templateIndex - 1);
    htmlOut += fname;
    htmlOut += this->htmlFile.substr(templateIndex + templateTag.size(),
                                     this->htmlFile.size() - 1);
    
    templateTag = "{sname}";
    templateIndex = htmlOut.find(templateTag);
    if (templateIndex != std::string::npos) {    
        htmlOut = htmlOut.substr(0, templateIndex - 1);
        htmlOut += sname;
        htmlOut += htmlOut.substr(templateIndex + templateTag.size(),
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
