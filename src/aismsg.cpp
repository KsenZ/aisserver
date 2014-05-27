#include "aismsg.h"

using namespace std;

aismsg::aismsg()
{
    string out_s;
    ifstream f("/usr/local/www/apache22/data/message/message.txt");

    while(!f.eof())
    {
        getline(f, out_s);
    }
    return out_s;
}
