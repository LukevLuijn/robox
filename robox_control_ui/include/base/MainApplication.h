// ======================================
// Filename:    MainApplication.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        23-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_MAIN_APPLICATION_H
#define ROBOX_CONTROL_UI_MAIN_APPLICATION_H

#include "wx/wx.h"

#define runGUI wxEntry

namespace Base
{
    class MainApplication : public wxApp
    {
        /**
         * @brief Initializes application.
         * 
         * @return true If the application was successfully executed.
         * @return false If the application was NOT successfully executed.
         */
        bool OnInit() override;
    };
    /**
     * @brief Get a reference to the current application.
     * 
     * @return MainApplication& 
     */
    MainApplication& TheApp();
}



#endif//ROBOX_CONTROL_UI_MAINAPPLICATION_H
