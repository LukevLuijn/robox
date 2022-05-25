//
// Created by luke on 23-05-22.
//

#include "MainApplication.h"
#include "Logger.h"
#include "Controller.h"

namespace Base
{
    wxIMPLEMENT_APP_NO_MAIN(MainApplication);

    MainApplication& TheApp()
    {
        return wxGetApp();
    }

    bool MainApplication::OnInit()
    {
        Logger::Logger::SetLogType(Logger::LogType_e::WX_PANEL);

        auto* frame = new Base::Controller();
        SetTopWindow(frame);
        frame->Show(true);
        return true;
    }
}// namespace Base