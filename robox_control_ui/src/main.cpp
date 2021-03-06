// ======================================
// Filename:    main.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        06-05-2022
// ======================================

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MainApplication.h"

int main(int argc, char* argv[])
{
    try
    {
        int result = runGUI(argc, argv);
        return result;
    }
    catch (std::exception& e)
    {
        std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
    }
}
