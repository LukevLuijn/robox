// ======================================
// Filename:    Bitmap.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_BITMAP_H
#define ROBOX_CONTROL_UI_BITMAP_H

#include "wx/bitmap.h"
#include "global_config.h"

namespace Utils
{
    class Bitmap
    {
    public:
        /**
         * @brief Create a Bitmap object
         * 
         * @param filename 
         * @param size 
         * @param fromAssets True if bitmap location is in local directory 'assets'
         * @return wxBitmap 
         */
        static wxBitmap CreateBitmap(const std::string& filename, const wxSize& size, bool fromAssets = true)
        {
            wxBitmap bitmap((fromAssets) ? std::string(ASSETS_PATH) + filename : filename);
            wxImage image = bitmap.ConvertToImage();
            image.Rescale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH);
            return wxBitmap(image);
        }
        /**
         * @brief Create a Bitmap object
         * 
         * @param filename 
         * @param width 
         * @param height 
         * @param fromAssets True if bitmap location is in local directory 'assets'
         * @return wxBitmap 
         */
        static wxBitmap CreateBitmap(const std::string& filename, uint16_t width, uint16_t height, bool fromAssets = true)
        {
            return CreateBitmap(filename, wxSize(width, height), fromAssets);
        }

    };
}

#endif//ROBOX_CONTROL_UI_BITMAP_H
