#include <iostream>
#include <fstream>
#include <string>

class OnChangeScript
{
public:
    OnChangeScript (std::ofstream& file, const std::string& parameterName)
        : mFile (file)
    {
        mFile
            << "<script onChange=\"" << parameterName << "\">\n"
            << "<![CDATA[\n";
    }

    ~OnChangeScript()
    {
        mFile
            << "]]>\n"
            << "</script>\n";
    }

private:
    std::ofstream& mFile;
};

int main()
{
    int count = 0;

    std::cout << "Number of controls: ";

    while (! (std::cin >> count) || count < 1)
    {
        std::cin.clear();

        while (std::cin.get() != '\n')
            continue;

        std::cout << "Invalid, try again: ";
    }

    std::string fileName {"OnChangeScripts" + std::to_string (count)
                          + ".littlefoot"};
    std::ofstream outFile;
    outFile.open (fileName);

    // onChange="amount"
    {
        OnChangeScript script(outFile, "amount");

        for (int i = 1; i <= count; ++i)
        {
            outFile << "\t";

            if (i > 1)
                outFile << "else ";

            outFile << "if (amount.value == " << i << ")";
                    << "\n\t{\n";



            outFile << "\t}\n";
        }
    }

    std::cout << "Written to " << fileName << "\n";
}