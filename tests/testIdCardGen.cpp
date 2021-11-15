#include "testIdCardGen.h"
#include "../src/id_card_gen.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestIdCardGen);

TestIdCardGen::TestIdCardGen() : CppUnit::TestCase("id_card_gen.h tests")
{
    
}

void TestIdCardGen::test()
{
    std::string email = "email",
                phone = "phone";
    
    TitoTicket hacker = TitoTicket(1, "slug", "Hacker");
    TitoTicket staff = TitoTicket(2, "slug", "Staff");
    TitoTicket committee = TitoTicket(3, "slug", "Committee");
    TitoTicket mentor = TitoTicket(4, "slug", "Mentor");
    
    TitoAttendee longBoy = TitoAttendee("MMMMMMMMMMMMMMMMMMM",
                                        email,
                                        phone,
                                        TitoTicket(5, "slug", "Mentor"));
    IdCard lCard = IdCard(longBoy);
    
    TitoAttendee h = TitoAttendee("Carlos Matos", email, phone, hacker);
    IdCard hCard = IdCard(h);
    
    TitoAttendee s = TitoAttendee("Gregory Gutin", email, phone, staff);
    IdCard sCard = IdCard(s);
    
    TitoAttendee c = TitoAttendee("Dave Cohen", email, phone, committee);
    IdCard cCard = IdCard(c);
    
    TitoAttendee m = TitoAttendee("Leondro Lio", email, phone, mentor);
    IdCard mCard = IdCard(m);
}
