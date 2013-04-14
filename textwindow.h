#ifndef __NOPACITY_TEXTWINDOW_H
#define __NOPACITY_TEXTWINDOW_H
class cNopacityTextWindow : public cThread {
private:
    cOsd *osd;
    cFont *font;
    cPixmap *pixmapBackground;
    cPixmap *pixmap;
    cRect *geometry;
    cRect *vidWin;
    cRect oldVidWin;
    bool scaledWindow;
    cString text;
    cTextWrapper twText;
    bool CreatePixmap(int border);
    void DrawText(int border);
    void DoSleep(int duration);
    virtual void Action(void);
public:
    cNopacityTextWindow(cOsd *osd, cFont *font, cRect *vidWin);
    virtual ~cNopacityTextWindow(void);
    void SetGeometry(cRect *geo) {geometry = geo;};
    void SetText(cString Text) {text = Text;};
};

#endif //__NOPACITY_TEXTWINDOW_H