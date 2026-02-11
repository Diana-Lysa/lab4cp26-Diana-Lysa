#include "MainForm.h" 

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    OP26Lab4FormDB::MainForm form;
    Application::Run(% form);

    return 0;
}