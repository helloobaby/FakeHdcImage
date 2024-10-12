 SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE); // Call it at the beginning of DllMain

BOOL WINAPI HookedBitBlt(
    HDC   hdc,
    int   x,
    int   y,
    int   cx,
    int   cy,
    HDC   hdcSrc,
    int   x1,
    int   y1,
    DWORD rop
) {    
    LOG(DEBUG) << "x -> " << x;
    LOG(DEBUG) << "y -> " << y;
    LOG(DEBUG) << "cx -> " << cx;
    LOG(DEBUG) << "cy -> " << cy;
    LOG(DEBUG) << "x1 -> " << x1;
    LOG(DEBUG) << "y1 -> " << y1;

    int   cxScreen = GetSystemMetrics(SM_CXSCREEN); 
    int   cyScreen = GetSystemMetrics(SM_CYSCREEN); 
    LOG(DEBUG) << std::dec << "cxScreen " << cxScreen;
    LOG(DEBUG) << std::dec << "cyScreen " << cyScreen;
    
    if ((cx == cxScreen && cy == cyScreen)) { 
        auto hBitmap = (HBITMAP)LoadImageA(NULL, (std::filesystem::path(DllPath).parent_path().string() + + "\\2560_1440.bmp").c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hBitmap) {
        }
        else {
            hBitmap = (HBITMAP)LoadImageA(NULL, (std::filesystem::path(DllPath).parent_path().string() + + "\\1920_1080.bmp").c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        }
        if (!hBitmap) { // 都没有
            LOG(ERROR) << "LoadImage fail";
            return false;   // 黑色屏幕
        }
        HWND win = GetDesktopWindow();
        HDC dc = GetDC(win);

        HDC hdcMem = CreateCompatibleDC(dc); 
        HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap); 
        bool r = ((T_BitBlt)_BitBlt)(hdc, 0, 0, cx, cy, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(win, dc);
        DeleteDC(hdcMem);
        return r;
    }

    LOG(INFO) << "go origin BitBlt";
    return ((T_BitBlt)_BitBlt)(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}

