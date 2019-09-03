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
    const int controlCount = 16;

    std::string fileName {"OnChangeScripts" + std::to_string (controlCount)
                          + ".littlefoot"};
    std::ofstream outFile;
    outFile.open (fileName);

    // onChange="overrideBrightness"
    {
        OnChangeScript script (outFile, "overrideBrightness");

        outFile << "\tbrightness.visible = overrideBrightness.value;\n";
    }

    // onChange="ParamControlType"
    for (int i = 0; i < controlCount; ++i)
    {
        outFile << "\n";
        OnChangeScript script (outFile, "ParamControlType" + std::to_string (i));

        // TODO: consider not checking the control amount for control 0

        outFile
            << "\tif (ParamControlType" << i << ".value == 0)\n"        // button
            << "\t{\n"
            << "\t\tMidiNote" << i << ".visible = false;\n"

            << "\t\tParamControlCCA" << i << ".displayName = \"MIDI CC\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCA" << i << ".visible = true;\n"

            << "\t\tParamControlCCB" << i << ".displayName = \"ON Value\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCB" << i << ".visible = true;\n"
            << "\t}\n"

            << "\telse if (ParamControlType" << i << ".value == 1)\n"   // note
            << "\t{\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tMidiNote" << i << ".visible = true;\n"

            << "\t\tParamControlCCA" << i << ".visible = false;\n"

            << "\t\tParamControlCCB" << i << ".displayName = \"Velocity\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCB" << i << ".visible = true;\n"
            << "\t}\n"

            << "\telse if (ParamControlType" << i << ".value == 2)\n"   // fader
            << "\t{\n"
            << "\t\tMidiNote" << i << ".visible = false;\n"

            << "\t\tParamControlCCA" << i << ".displayName = \"MIDI CC\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCA" << i << ".visible = true;\n"

            << "\t\tParamControlCCB" << i << ".visible = false;\n"
            << "\t}\n"

            << "\telse if (ParamControlType" << i << ".value == 3)\n"   // XYZ Pad
            << "\t{\n"
            << "\t\tMidiNote" << i << ".visible = false;\n"

            << "\t\tParamControlCCA" << i << ".displayName = \"MIDI CC X\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCA" << i << ".visible = true;\n"

            << "\t\tParamControlCCB" << i << ".displayName = \"MIDI CC Y\";\n"
            << "\t\tif (amount.value > " << i << ")\n"
            << "\t\t\tParamControlCCB" << i << ".visible = true;\n"
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