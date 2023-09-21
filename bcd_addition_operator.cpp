#include <iostream>
using namespace std;

class Bcd {
    typedef uint32_t bcdDigits_t; // Holds up to 8-BCD digits
private:
    bcdDigits_t v;
public:
    Bcd(bcdDigits_t i = 0){
        v = i;
    }
    // The global operator function
    Bcd operator+(Bcd const& bcd)
    {
        bcdDigits_t t1 = v + 0x06666666;
        bcdDigits_t t2 = t1 + bcd.v;
        bcdDigits_t t3 = t1 ^ bcd.v;
        bcdDigits_t t4 = t2 ^ t3;
        bcdDigits_t t5 = ~t4 & 0x11111110;
        bcdDigits_t t6 = (t5 >> 2) | (t5 >> 3);
        Bcd res(t2-t6); 
        return res;
    }
    void show() { cout << "bcd=" << hex << static_cast<int>(v); }
};

int main()
{
    Bcd c1(0x10), c2(0x20); // Remember to enter decimals digits coded in binary
    c1.show();
    cout << " + "; 
    c2.show();
    cout << " = ";
    Bcd c3 = c1 + c2; // An example call to "operator+"
    c3.show();
    cout << endl;
    return 0;
}
