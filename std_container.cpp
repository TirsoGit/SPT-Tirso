#include <iostream>
#include <map>
#include <string>
#include <string_view>
using namespace std;
 
void print_map(string_view comment, const map<int, int>& m)
{
    cout << comment;
    // iterate using C++17 facilities
    for (const auto& [key, value] : m)
        cout << "[val=" << key << "] : Count=" << value << "; ";
 
// C++11 alternative:
//  for (const auto& n : m)
//      cout << n.first << " = " << n.second << "; ";
//
// C++98 alternative
//  for (map<std::string, int>::const_iterator it = m.begin(); it != m.end(); it++)
//      cout << it->first << " = " << it->second << "; ";
 
    cout << '\n';
}
 
int main()
{
    // Create a map to store values with an associate counter that represents the quantity of this value.
    map<int, int> m{   
        {0/*value 0*/, 10/*qty 10*/}, 
        {1/*value 1*/, 100/*qty 100*/}, 
        {5/*value 5*/, 2/*qty 2*/}
    };
 
    print_map("1) Initial map: ", m);
 
    m[0] = 200; // updates an existing value
    m[4] = 300; // inserts a new value
    print_map("2) Updated map: ", m);
 
    // Using operator[] with non-existent key always performs an insert
    cout << "3) m[val=15] => Count= " << m[15] << '\n';
    print_map("4) Updated map: ", m); // A new value 15 is inserted with counter in 0
 
    m.erase(0); // Erase value 0
    print_map("5) After erase(0): ", m);
 
    erase_if(m, [](const auto& pair){ return pair.second > 100; }); // Erasae values gt 100
    print_map("6) After erase: ", m);
    cout << "7) m.size() = " << m.size() << '\n';
 
    m.clear();  // Clears the map
    cout << boolalpha << "8) Map is empty: " << m.empty() << '\n';
}
