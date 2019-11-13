#include <iostream>
#include <fstream>
#include <string>

// Parameter default values

int getDefaultNote (int controlIndex)    { return 60 + controlIndex; }      // can depend on control index

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

        outFile
        //==== BUTTON ==========================================================
            << "\tif (ParamControlType" << i << ".value == 0)\n"
            << "\t{\n"
            << "\t\tParamControlCCA" << i << ".displayName = \"Button CC\";\n"

            << "\t\tMidiNote" << i << ".visible = false;\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCA" << i << ".visible = true;\n"
            << "\t}\n"

        //==== NOTE ============================================================
            << "\telse if (ParamControlType" << i << ".value == 1)\n"
            << "\t{\n"
            << "\t\tMidiNote" << i << ".value = " << getDefaultNote (i) << ";\n"

            << "\t\tParamControlCCA" << i << ".visible = false;\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tMidiNote" << i << ".visible = true;\n"
            << "\t}\n"

        //==== FADER ===========================================================
            << "\telse if (ParamControlType" << i << ".value == 2)\n"
            << "\t{\n"
            << "\t\tParamControlCCA" << i << ".displayName = \"Fader CC\";\n"

            << "\t\tMidiNote" << i << ".visible = false;\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCA" << i << ".visible = true;\n"
            << "\t}\n";
    }

    // onChange="amount"
    {
        outFile << "\n";
        OnChangeScript script (outFile, "amount");

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

                outFile << "\t\tControlPosX"        << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlPosY"        << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlWidth"       << i << ".visible = " << state << ";\n";
                outFile << "\t\tControlHeight"      << i << ".visible = " << state << ";\n";

                if (isVisible)
                {
                    // Show MIDI Note parameter only for Note-type controls
                    outFile
                        << "\t\tif (ParamControlType" << i << ".value == 1)\n"
                        << "\t\t\tMidiNote" << i << ".visible = true;\n";

                    // Show CC A parameter for all controls except Note-type
                    outFile
                        << "\t\telse\n"
                        << "\t\t\tParamControlCCA" << i << ".visible = true;\n";
                }
                else
                {
                    outFile << "\t\tMidiNote"        << i << ".visible = false;\n";
                    outFile << "\t\tParamControlCCA" << i << ".visible = false;\n";
                }

                outFile << "\n";
            }

            outFile << "\t}\n";
        }
    }

    std::cout << "Written to " << fileName << "\n";
}