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
    int controlCount = 0;

    std::cout << "Number of controls: ";

    while (! (std::cin >> controlCount) || controlCount < 1)
    {
        std::cin.clear();

        while (std::cin.get() != '\n')
            continue;

        std::cout << "Invalid, try again: ";
    }

    std::string fileName {"OnChangeScripts" + std::to_string (controlCount)
                          + ".littlefoot"};
    std::ofstream outFile;
    outFile.open (fileName);

    // onChange="amount"
    {
        OnChangeScript script(outFile, "amount");

        for (int amount = 1; amount <= controlCount; ++amount)
        {
            outFile << "\t";

            if (amount > 1)
                outFile << "else ";

            outFile << "if (amount.value == " << amount << ")"
                    << "\n\t{\n";

            for (int i = 0; i < controlCount; ++i)
            {
                bool isVisible {i < amount};
                std::string state = isVisible ? "true" : "false";

                outFile << "\t\tParamControlType"   << i << ".visible = " << state << ";\n";
                outFile << "\t\tParamControlColour" << i << ".visible = " << state << ";\n";
                outFile << "\t\tParamControlMode"   << i << ".visible = " << state << ";\n";
                outFile << "\t\tParamMidiChannel"   << i << ".visible = " << state << ";\n";

                outFile << "\t\tControlPosX"        << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlPosY"        << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlWidth"       << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlHeight"      << i << ".visible = " << state << ";\n";

                outFile << "\t\tSendPress"          << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlCCP"         << i << ".visible = " << state << ";\n";

                if (isVisible)
                {
                    // Show MIDI Note parameter only for Note-type controls
                    outFile
                        << "\t\tif (ParamControlType" << i << ".value == 1)\n"
                        << "\t\t\tMidiNote" << i << ".visible = true;\n";

                    // Show CC A parameter for all controls except Note-type
                    outFile
                        << "\t\tif (ParamControlType" << i << ".value != 1)\n"
                        << "\t\t\tParamControlCCA" << i << ".visible = true;\n";

                    // Show CC B parameter for all controls except Fader-type
                    outFile
                        << "\t\tif (ParamControlType" << i << ".value != 2)\n"
                        << "\t\t\tParamControlCCB" << i << ".visible = true;\n";
                }
                else
                {
                    outFile << "\t\tMidiNote"        << i << ".visible = false;\n";
                    outFile << "\t\tParamControlCCA" << i << ".visible = false;\n";
                    outFile << "\t\tParamControlCCB" << i << ".visible = false;\n";
                }

                outFile << "\n";
            }

            outFile << "\t}\n";
        }
    }

    std::cout << "Written to " << fileName << "\n";
}