#ifndef __NOPACITY_TIMERS_H
#define __NOPACITY_TIMERS_H

class cNopacityTimer : public cListObject {
    private:
        cOsd *osd;
        cPixmap *pixmap;
        cPixmap *pixmapLogo;
        const cTimer *timer;
        const cFont *font;
        const cFont *fontLarge;
        int width;
        int height;
        int y;
        cString Date;
        cTextWrapper showName;
        void DrawLogo(void);
    public:
        cNopacityTimer(cOsd *osd, const cTimer *timer, const cFont *font, const cFont *fontLarge);
        virtual ~cNopacityTimer(void);
        void SetGeometry(int width, int y);
        void CreateDate(void);
        void CreateShowName(void);
        void CalculateHeight(int space);
        void CreatePixmaps(int x);
        void SetAlpha(int alpha);
        void Show(void);
        void Hide(void);
        int GetHeight(void) {return pixmap->ViewPort().Height();}
        void Render(void);
};

#endif //__NOPACITY_TIMERS_H
