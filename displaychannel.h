#ifndef __NOPACITY_DISPLAYCHANNEL_H
#define __NOPACITY_DISPLAYCHANNEL_H

enum eLogoPosition {lpNone = 0, lpLeft, lpRight};
enum eBackgroundStyle {bsTrans = 0, bsFull};

class cNopacityDisplayChannel : public cSkinDisplayChannel, cThread {
private:
    int FrameTime;
    int FadeTime;
    bool withInfo;
    bool initial;
    bool groupSep;
    bool channelChange;
    bool isRadioChannel;
    bool radioIconDrawn;
    cString lastDate;
    int lastSeen;
    time_t lastSignalDisplay;
    int lastSignalStrength;
    int lastSignalQuality;
    int lastScreenWidth;
    int currentLast;
    bool showSignal;
    const cEvent *present;
    cOsd *osd;
    cPixmap *pixmapBackgroundTop;
    cPixmap *pixmapBackgroundMiddle;
    cPixmap *pixmapBackgroundBottom;
    cPixmap *pixmapChannelInfo;
    cPixmap *pixmapDate;
    cPixmap *pixmapLogo;
    cPixmap *pixmapLogoBackground;
    cPixmap *pixmapLogoBackgroundTop;
    cPixmap *pixmapLogoBackgroundBottom;
    cPixmap *pixmapProgressBar;
    cPixmap *pixmapEPGInfo;
    cPixmap *pixmapFooter;
    cPixmap *pixmapStreamInfo;
    cPixmap *pixmapStreamInfoBack;
    cPixmap *pixmapSignalStrength;
    cPixmap *pixmapSignalQuality;
    cPixmap *pixmapSignalMeter;
    cPixmap *pixmapSignalLabel;
    cPixmap *pixmapScreenResolution;
    int height;
    int infoWidth;
    int infoX;
    int channelInfoWidth;
    int channelInfoHeight;
    int dateWidth;
    int progressBarHeight;
    int epgInfoHeight;
    int epgInfoLineHeight;
    int streamInfoHeight;
    int streamInfoY;
    int iconSize;
    int iconsWidth;
    int signalWidth, signalHeight, signalX;
    cFont *fontHeader;
    cFont *fontDate;
    cFont *fontEPG;
    cFont *fontEPGSmall;
    cFont *fontInfoline;
    virtual void Action(void);
    void SetGeometry(void);
    void CreatePixmaps(void);
    void CreateFonts(void);
    void DrawBackground(void);
    void DrawDate(void);
    void DrawProgressBar(int Current, int Total);
    tColor DrawProgressbarBackground(int left, int top, int width, int height);
    void DrawIcons(const cChannel *channel);
    void DrawScreenResolution(void);
    void DrawSignalMeter(void);
    void DrawSignal(void);
public:
    cNopacityDisplayChannel(bool WithInfo);
    virtual ~cNopacityDisplayChannel();
    virtual void SetChannel(const cChannel *Channel, int Number);
    virtual void SetEvents(const cEvent *Present, const cEvent *Following);
    virtual void SetMessage(eMessageType Type, const char *Text);
    virtual void Flush(void);
};
#endif //__NOPACITY_DISPLAYCHANNEL_H