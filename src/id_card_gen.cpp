#include "id_card_gen.h"
#include <stdio.h>

#define ASSETS_FOLDER "assets"

/**
 * The image is copied before it is modified so that it can be saved to the death
 * 
 * @param TitoAttendee the attendee to create the id card for.
 * @param std::string the filename for the new id card to be saved as.
 * 
 * @return the return code, 0 is failure and, 1 is success.
 */
static int copyTemplateImage(TitoAttendee attendee,
                              std::string newFileName)
{
    std::string ticketRelease = attendee.getTicket().getTicketRelease();    
    std::string releaseTag = "hacker.png";   
    if (ticketRelease == "Mentor") releaseTag = "mentor.png";
    else if (ticketRelease == "Staff") releaseTag = "staff.png";
    
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
static std::string getFileName(TitoAttendee attendee)
{
    return "id_card_" + std::to_string(attendee.getTicket().getTicketID());
}

IdCard::IdCard(TitoAttendee attendee)
{
    std::string fileName = getFileName(attendee);
    copyTemplateImage(attendee, fileName);
    this->image = CImg<unsigned char> (fileName.c_str());
}

void IdCard::saveIdCard()
{
    
}

void IdCard::printIdCard()
{
    
}
