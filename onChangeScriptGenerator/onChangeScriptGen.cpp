#include <iostream>
#include <fstream>
#include <string>

// Parameter default values

int getDefaultYAxisCC (int controlIndex) { return 102 + controlIndex; }     // can depend on control index
int getDefaultNote (int controlIndex)    { return 60 + controlIndex; }      // can depend on control index
const int defaultButtonOnValue = 127;
const int defaultNoteVelocity = 127;

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

void updateControlType(std::ofstream& file, int ind, bool areValuesUpdated)
{
//====== BUTTON ================================================================
    file
        << "\tif (ParamControlType" << ind << ".value == 0)\n"
        << "\t{\n";

    // Values
    if (areValuesUpdated)
    {
        file
            << "\t\tParamControlCCB" << ind << ".value = " << defaultButtonOnValue << ";\n";
    }

    // Names and tooltips
    file
        << "\t\tParamControlCCA" << ind << ".displayName = \"Button CC\";\n"
        << "\t\tParamControlCCA" << ind << ".tooltip = \"Select the MIDI message sent by the control.\\n\\nRange: [0-127]\";\n"

        << "\t\tParamControlCCB" << ind << ".displayName = \"On Value\";\n"
        << "\t\tParamControlCCB" << ind << ".tooltip = \"Set the value sent when the control is activated. When the control is deactivated, [0] is always transmitted.\\n\\nRange: [0-127]\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = false;\n"
        << "\t\tParamControlCCA" << ind << ".visible = (" << ind << " < amount.value);\n"
        << "\t\tParamControlCCB" << ind << ".visible = (" << ind << " < amount.value);\n";

    file
        << "\t}\n";

//====== NOTE ==================================================================
    file
        << "\telse if (ParamControlType" << ind << ".value == 1)\n"
        << "\t{\n";

    // Values
    if (areValuesUpdated)
    {
        file
            << "\t\tMidiNote" << ind << ".value = " << getDefaultNote (ind) << ";\n"
            << "\t\tParamControlCCB" << ind << ".value = " << defaultNoteVelocity << ";\n";
    }

    // Names and tooltips
    file
        << "\t\tParamControlCCB" << ind << ".displayName = \"Velocity Value\";\n"
        << "\t\tParamControlCCB" << ind << ".tooltip = \"Set the velocity of the MIDI note sent when sliding or gliding to the control, and when striking the control if [Fixed Note Velocity] is enabled.\\n\\nRange: [0-127]\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = (" << ind << " < amount.value);\n"
        << "\t\tParamControlCCA" << ind << ".visible = false;\n"
        << "\t\tParamControlCCB" << ind << ".visible = (" << ind << " < amount.value);\n";

    file
        << "\t}\n";

//====== FADER =================================================================
    file
        << "\telse if (ParamControlType" << ind << ".value == 2)\n"
        << "\t{\n";

    // Names and tooltips
    file
        << "\t\tParamControlCCA" << ind << ".displayName = \"Fader CC\";\n"
        << "\t\tParamControlCCA" << ind << ".tooltip = \"Select the MIDI message sent by the control.\\n\\nRange: [0-127]\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = false;\n"
        << "\t\tParamControlCCA" << ind << ".visible = (" << ind << " < amount.value);\n"
        << "\t\tParamControlCCB" << ind << ".visible = false;\n";

    file
        << "\t}\n";

//====== XY PAD ================================================================
    file
        << "\telse if (ParamControlType" << ind << ".value == 3)\n"
        << "\t{\n";

    // Values
    if (areValuesUpdated)
    {
        file
            << "\t\tParamControlCCB" << ind << ".value = " << getDefaultYAxisCC (ind) << ";\n";
    }

    // Names and tooltips
    file
        << "\t\tParamControlCCA" << ind << ".displayName = \"X Axis CC\";\n"
        << "\t\tParamControlCCA" << ind << ".tooltip = \"Select the MIDI message sent by the horizontal axis.\\n\\nRange: [0-127]\";\n"

        << "\t\tParamControlCCB" << ind << ".displayName = \"Y Axis CC\";\n"
        << "\t\tParamControlCCB" << ind << ".tooltip = \"Select the MIDI message sent by the vertical axis.\\n\\nRange: [0-127]\";\n";

    // Visibility
    file
        << "\t\tMidiNote"        << ind << ".visible = false;\n"
        << "\t\tParamControlCCA" << ind << ".visible = (" << ind << " < amount.value);\n"
        << "\t\tParamControlCCB" << ind << ".visible = (" << ind << " < amount.value);\n";

    file
        << "\t}\n";
}

int main()
{
    const int controlCount = 16;

    std::string fileName {"OnChangeScripts.littlefoot"};
    std::ofstream outFile;
    outFile.open (fileName);

    // onChange="individualBrightness"
    {
        OnChangeScript script (outFile, "individualBrightness");

        outFile << "\tbrightness.visible = ! individualBrightness.value;\n";
    }

    // onChange="fixedVelocity"
    {
        outFile << "\n";
        OnChangeScript script (outFile, "fixedVelocity");

        outFile << "\tvelocitySensitivity.visible = ! fixedVelocity.value;\n";
    }

    // onChange="ParamControlType"
    for (int i = 0; i < controlCount; ++i)
    {
        outFile << "\n";
        OnChangeScript script (outFile, "ParamControlType" + std::to_string (i));

        updateControlType(outFile, i, false);
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
                << "\tParamControlMode"   << i << ".visible = (" << i << " < amount.value);\n"
                << "\tParamMidiChannel"   << i << ".visible = (" << i << " < amount.value);\n";

            outFile
                << "\tControlPosX"        << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlPosY"        << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlWidth"       << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlHeight"      << i << ".visible = (" << i << " < amount.value);\n";

            outFile
                << "\tSendPress"          << i << ".visible = (" << i << " < amount.value);\n"
                << "\tControlCCP"         << i << ".visible = (" << i << " < amount.value);\n";

            updateControlType(outFile, i, false);

            outFile << "\n";
        }
    }

    std::cout << "Written to " << fileName << "\n";
}