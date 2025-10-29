#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int a = 1234454;
    char bytes[sizeof a];
    std::copy(static_cast<const char*>(static_cast<const void*>(&a)),
          static_cast<const char*>(static_cast<const void*>(&a)) + sizeof a,
          bytes);
    cout<<"Original: "<<a<<endl;
    cout<<"Numbers are"<<endl;
    for (const char &word : bytes)
    {
        cout << (int)word << " ";
    }
    int b = int((unsigned char)(bytes[3]) << 24 |
            (unsigned char)(bytes[2]) << 16 |
            (unsigned char)(bytes[1]) << 8 |
            (unsigned char)(bytes[0]));
    cout<<endl;
    cout<<"Converted back: "<<b<<endl;
    cout << endl;
}
