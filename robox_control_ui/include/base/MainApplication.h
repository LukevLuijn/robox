//
// Created by luke on 23-05-22.
//

#ifndef ROBOX_UI_TESTS_MAIN_APPLICATION_H
#define ROBOX_UI_TESTS_MAIN_APPLICATION_H

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



#endif//ROBOX_UI_TESTS_MAINAPPLICATION_H
