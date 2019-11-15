#include <iostream>
#include <fstream>
#include <string>

// Parameter default values

int getDefaultNote (int controlIndex)    { return 60 + controlIndex; }      // can depend on control index

// Object lifetime based script decoration

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

// Updater for the type-dependent parameters

void updateControlType(std::ofstream& file, int ind)
{
//====== BUTTON ================================================================
    file
        << "\tif (ParamControlType" << ind << ".value == 0)\n"
        << "\t{\n";

    // Name
    file
        << "\t\tParamControlCCA" << ind << ".displayName = \"Button CC\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = false;\n"
        << "\t\tParamControlCCA" << ind << ".visible = (" << ind << " < amount.value);\n";

    file
        << "\t}\n";

//====== NOTE ==================================================================
    file
        << "\telse if (ParamControlType" << ind << ".value == 1)\n"
        << "\t{\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = (" << ind << " < amount.value);\n"
        << "\t\tParamControlCCA" << ind << ".visible = false;\n";

    file
        << "\t}\n";

//====== FADER =================================================================
    file
        << "\telse if (ParamControlType" << ind << ".value == 2)\n"
        << "\t{\n";

    // Name
    file
        << "\t\tParamControlCCA" << ind << ".displayName = \"Fader CC\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = false;\n"
        << "\t\tParamControlCCA" << ind << ".visible = (" << ind << " < amount.value);\n";

    file
        << "\t}\n";
}

int main()
{
    const int controlCount = 25;

    std::string fileName {"OnChangeScripts" + std::to_string (controlCount)
                          + ".littlefoot"};
    std::ofstream outFile;
    outFile.open (fileName);

    // onChange="individualBrightness"
    {
        OnChangeScript script (outFile, "individualBrightness");

        outFile << "\tbrightness.visible = ! individualBrightness.value;\n";
    }

    // onChange="ParamControlType"
    for (int i = 0; i < controlCount; ++i)
    {
        outFile << "\n";
        OnChangeScript script (outFile, "ParamControlType" + std::to_string (i));

        updateControlType(outFile, i);
    }

    // onChange="amount"
    {
        outFile << "\n";
        OnChangeScript script (outFile, "amount");

        for (int i = 0; i < controlCount; ++i)
        {
            outFile
                << "\tParamControlType"   << i << ".visible = (" << i << " < amount.value);\n"
                << "\tParamControlColour" << i << ".visible = (" << i << " < amount.value);\n"
                << "\tParamControlMode"   << i << ".visible = (" << i << " < amount.value);\n";

            outFile
                << "\tControlPosX"        << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlPosY"        << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlWidth"       << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlHeight"      << i << ".visible = (" << i << " < amount.value);\n";

            updateControlType(outFile, i);

            outFile << "\n";
        }
    }

    std::cout << "Written to " << fileName << "\n";
}