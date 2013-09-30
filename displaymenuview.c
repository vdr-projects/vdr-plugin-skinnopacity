#include "displaymenuview.h"

cNopacityDisplayMenuView::cNopacityDisplayMenuView(cImageCache *imgCache) {
    this->imgCache = imgCache;
    diskUsageAlert = 95;
    pixmapStatus = NULL;
    pixmapHeaderIcon = NULL;
}

cNopacityDisplayMenuView::~cNopacityDisplayMenuView(void) {
    osd->DestroyPixmap(pixmapHeader);
    osd->DestroyPixmap(pixmapHeaderLogo);
    osd->DestroyPixmap(pixmapHeaderLabel);
    osd->DestroyPixmap(pixmapDate);
    osd->DestroyPixmap(pixmapFooter);
    osd->DestroyPixmap(pixmapButtonsText);
    osd->DestroyPixmap(pixmapContent);
    osd->DestroyPixmap(pixmapScrollbar);    
    osd->DestroyPixmap(pixmapDiskUsage);
    osd->DestroyPixmap(pixmapDiskUsageIcon);
    osd->DestroyPixmap(pixmapDiskUsageLabel);
    if (pixmapHeaderIcon)
        osd->DestroyPixmap(pixmapHeaderIcon);
    if (pixmapRssFeed)
        osd->DestroyPixmap(pixmapRssFeed);
    if (pixmapRssFeedBackground)
        osd->DestroyPixmap(pixmapRssFeedBackground);
    if (pixmapRssFeedIcon)
        osd->DestroyPixmap(pixmapRssFeedIcon);
}

cOsd *cNopacityDisplayMenuView::createOsd(void) {
    osd = CreateOsd(geoManager->osdLeft, geoManager->osdTop, geoManager->osdWidth, geoManager->osdHeight);
    return osd;
}

void cNopacityDisplayMenuView::SetDescriptionTextWindowSize(void) {
    int xSchedules, xRecordings, xChannels;
    int widthSchedules, widthRecordings, widthChannels;
    if (config.menuAdjustLeft) {
        xSchedules =    2 * geoManager->menuSpace 
                        + geoManager->menuContentWidthSchedules 
                        + geoManager->menuWidthScrollbar;
        xRecordings =   2 * geoManager->menuSpace 
                        + geoManager->menuContentWidthRecordings 
                        + geoManager->menuWidthScrollbar;
        xChannels =     2 * geoManager->menuSpace 
                        + geoManager->menuContentWidthChannels 
                        + geoManager->menuWidthScrollbar;
        widthSchedules = geoManager->osdWidth - xSchedules - geoManager->menuSpace;
        widthRecordings = geoManager->osdWidth - xRecordings - geoManager->menuSpace;
        widthChannels = geoManager->osdWidth - xChannels - geoManager->menuSpace;
    } else {
        xSchedules = geoManager->menuSpace;
        xRecordings = geoManager->menuSpace;
        xChannels = geoManager->menuSpace;
        widthSchedules =  geoManager->osdWidth 
                          - geoManager->menuContentWidthSchedules 
                          - geoManager->menuWidthScrollbar 
                          - 2 * geoManager->menuSpace;
        widthRecordings = geoManager->osdWidth 
                          - geoManager->menuContentWidthRecordings 
                          - geoManager->menuWidthScrollbar 
                          - 2 * geoManager->menuSpace;
        widthChannels =   geoManager->osdWidth 
                          - geoManager->menuContentWidthChannels 
                          - geoManager->menuWidthScrollbar 
                          - 2 * geoManager->menuSpace;
    }
    int heightFull = geoManager->menuContentHeight - 2*geoManager->menuSpace;
    int height = config.menuHeightInfoWindow * heightFull / 100;
    int y = geoManager->menuHeaderHeight + (geoManager->menuContentHeight - height - geoManager->menuSpace);
    int yFullScreen = geoManager->menuHeaderHeight + geoManager->menuSpace;

    if (config.menuSchedulesWindowMode == 0)
        textWindowSizeSchedules = cRect(xSchedules,y,widthSchedules,height);
    else
        textWindowSizeSchedules = cRect(xSchedules,yFullScreen,widthSchedules,heightFull);
    if (config.menuRecordingsWindowMode == 0)
        textWindowSizeRecordings = cRect(xRecordings,y,widthRecordings,height);
    else
        textWindowSizeRecordings = cRect(xRecordings,yFullScreen,widthRecordings,heightFull);
    textWindowSizeChannels = cRect(xChannels,y,widthChannels,height);
}

cRect *cNopacityDisplayMenuView::GetDescriptionTextWindowSize(eMenuCategory menuCat) {
    switch (menuCat) {
        case mcSchedule:
            return &textWindowSizeSchedules;
        case mcRecording:
            return &textWindowSizeRecordings;
        case mcChannel:
            return &textWindowSizeChannels;
        default:
            return NULL;
    }
    return NULL;
}

int cNopacityDisplayMenuView::GetContentWidth(eMenuCategory menuCat) {
    switch (menuCat) {
        case mcMain:
            if (config.narrowMainMenu)
                return geoManager->menuContentWidthMain;
            else
                return geoManager->menuContentWidthFull;
        case mcSetup:
            if (config.narrowSetupMenu)
                return geoManager->menuContentWidthSetup;
            else
                return geoManager->menuContentWidthFull;
        case mcSchedule:
        case mcScheduleNow:
        case mcScheduleNext:
            if (config.narrowScheduleMenu)
                return geoManager->menuContentWidthSchedules;
            else
                return geoManager->menuContentWidthFull;
        case mcChannel:
            if (config.narrowChannelMenu)
                return geoManager->menuContentWidthChannels;
            else
                return geoManager->menuContentWidthFull;
        case mcTimer:
            if (config.narrowTimerMenu)
                return geoManager->menuContentWidthTimers;
            else
                return geoManager->menuContentWidthFull;
        case mcRecording:
            if (config.narrowRecordingMenu)
                return geoManager->menuContentWidthRecordings;
            else
                return geoManager->menuContentWidthFull;
        default:
            return geoManager->menuContentWidthFull;     
    }
    return geoManager->menuContentWidthFull;
}


void cNopacityDisplayMenuView::CreatePixmaps(void) {
    pixmapHeader = osd->CreatePixmap(1, cRect(0, 0, geoManager->osdWidth, geoManager->menuHeaderHeight));
    int dateX = (config.menuAdjustLeft) ? (geoManager->osdWidth - geoManager->menuDateWidth) : 0;
    pixmapDate = osd->CreatePixmap(2, cRect(dateX, 0, geoManager->menuDateWidth, geoManager->menuHeaderHeight));
    int logoX = (config.menuAdjustLeft) ? 0 : (geoManager->osdWidth - config.menuHeaderLogoWidth);
    pixmapHeaderLogo = osd->CreatePixmap(-1, cRect(logoX, 0, config.menuHeaderLogoWidth, config.menuHeaderLogoHeight));
    int labelX = (config.menuAdjustLeft) ? 0 : geoManager->menuDateWidth;
    pixmapHeaderLabel = osd->CreatePixmap(2, cRect(labelX, 0, geoManager->osdWidth - geoManager->menuDateWidth, geoManager->menuHeaderHeight));
    pixmapFooter = osd->CreatePixmap(1, cRect(0, geoManager->osdHeight - geoManager->menuRssFeedHeight - geoManager->menuFooterHeight, geoManager->osdWidth, geoManager->menuFooterHeight));
    pixmapButtonsText = osd->CreatePixmap(1, cRect(0, geoManager->osdHeight - geoManager->menuRssFeedHeight - geoManager->menuFooterHeight, geoManager->osdWidth, geoManager->menuFooterHeight));
    int drawPortWidth = geoManager->osdWidth + geoManager->menuContentWidthFull - geoManager->menuContentWidthMinimum;
    pixmapContent = osd->CreatePixmap(1, cRect(0, geoManager->menuHeaderHeight, geoManager->osdWidth, geoManager->menuContentHeight),
                                         cRect(0, 0, drawPortWidth, geoManager->menuContentHeight));
    int diskUsageX = (config.menuAdjustLeft) ? (geoManager->osdWidth - geoManager->menuDiskUsageWidth - 10) : 10;
    pixmapDiskUsage = osd->CreatePixmap(2, cRect(diskUsageX, geoManager->menuHeaderHeight + geoManager->menuSpace, geoManager->menuDiskUsageWidth, geoManager->menuDiskUsageHeight));
    pixmapDiskUsageIcon = osd->CreatePixmap(3, cRect(diskUsageX + 2, geoManager->menuHeaderHeight + geoManager->menuSpace + 2, geoManager->menuDiskUsageWidth - 4, geoManager->menuDiskUsageHeight - 4));
    pixmapDiskUsageLabel = osd->CreatePixmap(4, cRect(diskUsageX + 2, geoManager->menuHeaderHeight + geoManager->menuSpace + 2, geoManager->menuDiskUsageWidth - 4, geoManager->menuDiskUsageHeight - 4));
    int scrollbarX = (config.menuAdjustLeft) ? geoManager->menuContentWidthMain : (geoManager->osdWidth - geoManager->menuContentWidthMain);
    pixmapScrollbar = osd->CreatePixmap(2, cRect(scrollbarX, geoManager->menuHeaderHeight + geoManager->menuSpace, geoManager->menuWidthScrollbar, geoManager->menuContentHeight - 2 * geoManager->menuSpace));

    if (config.displayRSSFeed) {
        pixmapRssFeedBackground = osd->CreatePixmap(1, cRect(0, geoManager->menuHeaderHeight + geoManager->menuContentHeight + geoManager->menuFooterHeight, geoManager->osdWidth, geoManager->menuRssFeedHeight));
        pixmapRssFeed = osd->CreatePixmap(2, cRect(0, geoManager->menuHeaderHeight + geoManager->menuContentHeight + geoManager->menuFooterHeight, geoManager->osdWidth, geoManager->menuRssFeedHeight));
        pixmapRssFeedIcon = osd->CreatePixmap(3, cRect(0, geoManager->menuHeaderHeight + geoManager->menuContentHeight + geoManager->menuFooterHeight, geoManager->menuRssFeedHeight, geoManager->menuRssFeedHeight));
    } else {
        pixmapRssFeedBackground = NULL;
        pixmapRssFeed = NULL;
        pixmapRssFeedIcon = NULL;
    }
    pixmapHeaderLogo->Fill(clrTransparent);
    pixmapHeaderLabel->Fill(clrTransparent);
    pixmapDiskUsage->Fill(clrTransparent);
    pixmapDiskUsageIcon->Fill(clrTransparent);
    pixmapDiskUsageLabel->Fill(clrTransparent);
    if (config.menuFadeTime) {
        pixmapHeader->SetAlpha(0);
        pixmapHeaderLogo->SetAlpha(0);
        pixmapHeaderLabel->SetAlpha(0);
        pixmapDate->SetAlpha(0);
        pixmapFooter->SetAlpha(0);
        pixmapButtonsText->SetAlpha(0);
        pixmapContent->SetAlpha(0);
        pixmapScrollbar->SetAlpha(0);
        pixmapDiskUsage->SetAlpha(0);
        pixmapDiskUsageIcon->SetAlpha(0);
        pixmapDiskUsageLabel->SetAlpha(0);
    }
}

void cNopacityDisplayMenuView::SetPixmapAlpha(int Alpha) {
    pixmapHeader->SetAlpha(Alpha);
    pixmapHeaderLogo->SetAlpha(Alpha);
    pixmapHeaderLabel->SetAlpha(Alpha);
    pixmapDate->SetAlpha(Alpha);
    pixmapContent->SetAlpha(Alpha);
    pixmapFooter->SetAlpha(Alpha);
    pixmapButtonsText->SetAlpha(Alpha);
    pixmapScrollbar->SetAlpha(Alpha);
    pixmapDiskUsage->SetAlpha(Alpha);
    pixmapDiskUsageIcon->SetAlpha(Alpha);
    pixmapDiskUsageLabel->SetAlpha(Alpha);
}

void cNopacityDisplayMenuView::GetMenuItemSize(eMenuCategory menuCat, cPoint *itemSize) {
    int itemWidth = 0;
    int itemHeight = 0;
    switch (menuCat) {
        case mcUnknown:
            itemWidth = geoManager->menuItemWidthDefault;
            itemHeight = geoManager->menuItemHeightDefault;
            break;
        case mcMain:
            itemWidth = geoManager->menuItemWidthMain;
            itemHeight = geoManager->menuItemHeightMain;
            break;
        case mcSetup:
            itemWidth = geoManager->menuItemWidthSetup;
            itemHeight = geoManager->menuItemHeightMain;
            break;
        case mcSchedule:
        case mcScheduleNow:
        case mcScheduleNext:
            itemWidth = geoManager->menuItemWidthSchedule;
            itemHeight = geoManager->menuItemHeightSchedule;
            break;
        case mcChannel:
            itemWidth = geoManager->menuItemWidthChannel;
            itemHeight = geoManager->menuItemHeightSchedule;
            break;
        case mcTimer:
            itemWidth = geoManager->menuItemWidthTimer;
            itemHeight = geoManager->menuItemHeightSchedule;
            break;
        case mcRecording:
            itemWidth = geoManager->menuItemWidthRecording;
            itemHeight = geoManager->menuItemHeightRecordings;
            break;
        default:
            itemWidth = geoManager->menuItemWidthDefault;
            itemHeight = geoManager->menuItemHeightDefault;
            break;
    }
    itemSize->Set(itemWidth, itemHeight);
}

int cNopacityDisplayMenuView::GetMaxItems(eMenuCategory menuCat) {
    int maxItems = 0;
    switch (menuCat) {
        case mcMain:
        case mcSetup:
            maxItems = geoManager->menuContentHeight / (geoManager->menuItemHeightMain + geoManager->menuSpace);
            break;
        case mcSchedule:
        case mcScheduleNow:
        case mcScheduleNext:
        case mcChannel:
        case mcTimer:
            maxItems = geoManager->menuContentHeight / (geoManager->menuItemHeightSchedule + geoManager->menuSpace);
            break;
        case mcRecording:
            maxItems = geoManager->menuContentHeight / (geoManager->menuItemHeightRecordings + geoManager->menuSpace);
            break;
        default:
            maxItems = config.numDefaultMenuItems;      
    }
    return maxItems;
}

int cNopacityDisplayMenuView::GetMenuTop(int numItems, int itemHeight) {
    return geoManager->menuHeaderHeight + (geoManager->menuContentHeight - numItems*(itemHeight + geoManager->menuSpace))/2;
}

int cNopacityDisplayMenuView::GetMenuItemLeft(int itemWidth) {
    int menuLeft = geoManager->menuSpace;
    if (!config.menuAdjustLeft)
        menuLeft = geoManager->osdWidth - itemWidth - geoManager->menuSpace;
    return menuLeft;
}

void cNopacityDisplayMenuView::SetAvrgFontWidth(void) {
    avrgFontWidth = fontManager->menuItemDefault->Width("x")+3;
}

void cNopacityDisplayMenuView::SetTabs(int Tab1, int Tab2, int Tab3, int Tab4, int Tab5) {
    mytabs[0] = 1;
    mytabs[1] = Tab1 ? mytabs[0] + Tab1 : 0;
    mytabs[2] = Tab2 ? mytabs[1] + Tab2 : 0;
    mytabs[3] = Tab3 ? mytabs[2] + Tab3 : 0;
    mytabs[4] = Tab4 ? mytabs[3] + Tab4 : 0;
    mytabs[5] = Tab5 ? mytabs[4] + Tab5 : 0;
    if (Tab2) {
        for (int i = 1; i < cSkinDisplayMenu::MaxTabs; i++)
        mytabs[i] *= avrgFontWidth;
    } else if (Tab1) {
        mytabs[1] = GetEditableWidth();
    }
}

int cNopacityDisplayMenuView::GetEditableWidth(void) {
    return geoManager->menuContentWidthFull*0.5;
}

int cNopacityDisplayMenuView::GetTextAreaWidth(void) {
    return geoManager->menuContentWidthFull - 2*geoManager->menuSpace;
}

const cFont *cNopacityDisplayMenuView::GetTextAreaFont(bool FixedFont) {
    return cFont::CreateFont(config.fontName, geoManager->menuContentHeight / 25 + config.fontDetailView);
}

void cNopacityDisplayMenuView::DrawBorderDecoration() {
    if (config.doBlending) {
        cImage *headerImage = imgCache->GetBackground(btMenuHeader);
        if (headerImage)
            pixmapHeader->DrawImage(cPoint(0, 0), *headerImage);
    } else 
        pixmapHeader->Fill(Theme.Color(clrMenuBack));
    pixmapFooter->Fill(Theme.Color(clrMenuBack));
    pixmapButtonsText->Fill(clrTransparent);
    
    int borderWidth = 2;
    int radius = 10;

    pixmapContent->Fill(clrTransparent);

    if (config.menuAdjustLeft) {
        //Background
        pixmapContent->DrawRectangle(cRect(0, 0, geoManager->menuContentWidthFull - radius, geoManager->menuContentHeight), Theme.Color(clrMenuBack));
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull - radius, 0, geoManager->menuWidthScrollbar + geoManager->menuSpace + radius, geoManager->menuContentHeight), Theme.Color(clrMenuScrollBarBase));
        //Upper and lower Corner Square
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull - radius, 0, radius, radius), Theme.Color(clrMenuBack));
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull - radius, geoManager->menuContentHeight - radius, radius, radius), Theme.Color(clrMenuBack));
        //Upper line
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull, 0, geoManager->osdWidth - geoManager->menuContentWidthMinimum, borderWidth), Theme.Color(clrMenuBorder));
        //Lower Line
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull, geoManager->menuContentHeight - borderWidth, geoManager->osdWidth - geoManager->menuContentWidthMinimum, borderWidth), Theme.Color(clrMenuBorder));
        //Vertical line
        pixmapContent->DrawRectangle(cRect(geoManager->menuContentWidthFull-radius, radius, borderWidth, geoManager->menuContentHeight - 2*radius), Theme.Color(clrMenuBorder));
        if (radius-borderWidth > 2) {
            //Upper Ellipse
            pixmapContent->DrawEllipse(cRect(geoManager->menuContentWidthFull - radius, 0, radius, radius), Theme.Color(clrMenuBorder),2);
            pixmapContent->DrawEllipse(cRect(geoManager->menuContentWidthFull - radius + borderWidth, borderWidth, radius-borderWidth, radius-borderWidth), Theme.Color(clrMenuScrollBarBase), 2);
            //Lower Ellipse
            pixmapContent->DrawEllipse(cRect(geoManager->menuContentWidthFull - radius, geoManager->menuContentHeight - radius, radius, radius), Theme.Color(clrMenuBorder),3);
            pixmapContent->DrawEllipse(cRect(geoManager->menuContentWidthFull - radius + borderWidth, geoManager->menuContentHeight - radius, radius-borderWidth, radius-borderWidth), Theme.Color(clrMenuScrollBarBase), 3);
        }
    } else {
        //Background
        pixmapContent->DrawRectangle(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum + radius, 0, geoManager->menuContentWidthFull - radius, geoManager->menuContentHeight), Theme.Color(clrMenuBack));
        pixmapContent->DrawRectangle(cRect(0, 0, geoManager->menuWidthScrollbar + geoManager->menuSpace + radius, geoManager->menuContentHeight), Theme.Color(clrMenuScrollBarBase));
        //Upper and lower Corner Square
        pixmapContent->DrawRectangle(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, 0, radius, radius), Theme.Color(clrMenuBack));
        pixmapContent->DrawRectangle(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, geoManager->menuContentHeight - radius, radius, radius), Theme.Color(clrMenuBack));
        //Upper line
        pixmapContent->DrawRectangle(cRect(0, 0, geoManager->osdWidth - geoManager->menuContentWidthMinimum, borderWidth), Theme.Color(clrMenuBorder));
        //Lower Line
        pixmapContent->DrawRectangle(cRect(0, geoManager->menuContentHeight - borderWidth, geoManager->osdWidth - geoManager->menuContentWidthMinimum, borderWidth), Theme.Color(clrMenuBorder));
        //Vertical line
        pixmapContent->DrawRectangle(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum + radius - borderWidth, radius, borderWidth, geoManager->menuContentHeight - 2*radius), Theme.Color(clrMenuBorder));
        if (radius-borderWidth > 2) {
            //Upper Ellipse
            pixmapContent->DrawEllipse(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, 0, radius, radius), Theme.Color(clrMenuBorder),1);
            pixmapContent->DrawEllipse(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, borderWidth, radius-borderWidth, radius-borderWidth), Theme.Color(clrMenuScrollBarBase), 1);
            //Lower Ellipse
            pixmapContent->DrawEllipse(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, geoManager->menuContentHeight - radius, radius, radius), Theme.Color(clrMenuBorder),4);
            pixmapContent->DrawEllipse(cRect(geoManager->osdWidth - geoManager->menuContentWidthMinimum, geoManager->menuContentHeight - radius, radius-borderWidth, radius-borderWidth), Theme.Color(clrMenuScrollBarBase), 4);
        }
    }
}

void cNopacityDisplayMenuView::AdjustContentBackground(eMenuCategory menuCat, eMenuCategory menuCatLast, cRect & vidWin) {
    int contentWidth = GetContentWidth(menuCat);
    int contentWidthLast = GetContentWidth(menuCatLast);
    int drawportX;
    if ((contentWidth != contentWidthLast)||(menuCatLast == mcUndefined)) {
        if (contentWidth == geoManager->menuContentWidthFull) {
            drawportX = (config.menuAdjustLeft)?0:(geoManager->menuContentWidthMinimum - geoManager->menuContentWidthFull);
            pixmapContent->SetDrawPortPoint(cPoint(drawportX, 0));
            if (config.scalePicture) {
                // ask output device to restore full size
                vidWin = cDevice::PrimaryDevice()->CanScaleVideo(cRect::Null);
            }
        } else {
            drawportX = (config.menuAdjustLeft)?(contentWidth - geoManager->menuContentWidthFull):(geoManager->menuContentWidthMinimum - contentWidth);
            pixmapContent->SetDrawPortPoint(cPoint(drawportX, 0));
            if (config.scalePicture) {
                // ask output device to scale down
                int windowX = (config.menuAdjustLeft)?(geoManager->osdLeft + contentWidth + geoManager->menuWidthScrollbar + 2 * geoManager->menuSpace)
                                                     :(geoManager->osdLeft + 2 * geoManager->menuSpace);
                cRect availableRect(
                    windowX,
                    geoManager->osdTop + geoManager->menuHeaderHeight,
                    geoManager->menuContentWidthFull - geoManager->osdLeft - contentWidth - geoManager->menuWidthScrollbar - 4 * geoManager->menuSpace,
                    geoManager->menuContentHeight);
                vidWin = cDevice::PrimaryDevice()->CanScaleVideo(availableRect);
            }
        }
    }
    osd->DestroyPixmap(pixmapScrollbar);
    int scrollbarX = (config.menuAdjustLeft)?(contentWidth):(geoManager->osdWidth - contentWidth - geoManager->menuWidthScrollbar);
    pixmapScrollbar = osd->CreatePixmap(2, cRect(scrollbarX , geoManager->menuHeaderHeight + geoManager->menuSpace, geoManager->menuWidthScrollbar, geoManager->menuContentHeight - 2*geoManager->menuSpace));
    pixmapScrollbar->Fill(clrTransparent);
}

void cNopacityDisplayMenuView::DrawHeaderLogo(void) {
    cImage *imgIcon = imgCache->GetSkinIcon("skinIcons/vdrlogo");
    if (imgIcon)
        pixmapHeaderLogo->DrawImage(cPoint(0,0), *imgIcon);
}

int cNopacityDisplayMenuView::ShowHeaderLogo(bool show) {
    if (show) {
        pixmapHeaderLogo->SetLayer(2);
    } else {
        pixmapHeaderLogo->SetLayer(-1);
    }
    return config.menuHeaderLogoWidth + geoManager->menuSpace;
}

int cNopacityDisplayMenuView::DrawHeaderIcon(eMenuCategory menuCat) {
    cString icon;
    bool drawIcon = true;
    switch (menuCat) {
        case mcScheduleNow:
        case mcScheduleNext:
            icon = "menuIcons/Schedule";
            break;
        case mcChannel:
            icon = "menuIcons/Channels";
            break;
        case mcTimer:
            icon = "menuIcons/Timers";
            break;
        case mcRecording:
            icon = "menuIcons/Recordings";
            break;
        case mcSetup:
            icon = "menuIcons/Setup";
            break;
        case mcCommand:
            icon = "menuIcons/Commands";
            break;
        default:
            drawIcon = false;
    }
    
    int left = 0;
    if (drawIcon) {
        int iconX = (config.menuAdjustLeft) ? 0 : (geoManager->osdWidth - config.headerIconHeight);
        pixmapHeaderIcon = osd->CreatePixmap(2, cRect(iconX, 0, config.headerIconHeight, config.headerIconHeight));
        pixmapHeaderIcon->Fill(clrTransparent);
        cImage *imgIcon = imgCache->GetSkinIcon(*icon);
        if (imgIcon) {
            pixmapHeaderIcon->DrawImage(cPoint(0,0), *imgIcon);
            left = config.headerIconHeight + geoManager->menuSpace;
        }
    }
    return left;
}

int cNopacityDisplayMenuView::ShowHeaderIconChannelLogo(const char *Title) {
    int left = 0;
    int iconX = (config.menuAdjustLeft) ? 0 : (geoManager->osdWidth - config.menuItemLogoWidth);
    pixmapHeaderIcon = osd->CreatePixmap(2, cRect(iconX, 0, config.menuItemLogoWidth, config.menuItemLogoHeight));
    pixmapHeaderIcon->Fill(clrTransparent);
    std::string channel = Title;
    if (channel.length() == 0)
        return left;
    std::string remove = trVDR("Schedule");
    try {
        remove.append(" - ");
        channel.erase(0, remove.length());
    } catch (...) {}
    cImageLoader imgLoader;
    if (imgLoader.LoadLogo(channel.c_str(), config.menuItemLogoWidth, config.menuItemLogoHeight)) {
        pixmapHeaderIcon->DrawImage(cPoint(0, 0), imgLoader.GetImage());
        left =  config.menuItemLogoWidth + geoManager->menuSpace;
    }
    return left;
}

void cNopacityDisplayMenuView::DestroyHeaderIcon(void) {
    if (pixmapHeaderIcon) {
        osd->DestroyPixmap(pixmapHeaderIcon);
        pixmapHeaderIcon = NULL;
    }
}

void cNopacityDisplayMenuView::DrawHeaderLabel(int left, cString label) {
    pixmapHeaderLabel->Fill(clrTransparent);
    int labelW = fontManager->menuHeader->Width(label);
    int labelX = (config.menuAdjustLeft) ? (left) : (geoManager->osdWidth - geoManager->menuDateWidth - labelW - left - 2*geoManager->menuSpace);
    pixmapHeaderLabel->DrawText(cPoint(labelX, ((geoManager->menuHeaderHeight - 10) - fontManager->menuHeader->Height()) / 2), *label, Theme.Color(clrMenuFontHeader), clrTransparent, fontManager->menuHeader);
}

void cNopacityDisplayMenuView::DrawDate(bool initial) {
    cString date = DayDateTime();
    if (initial || strcmp(date, lastDate)) {
        pixmapDate->Fill(clrTransparent);
        int dateW = fontManager->menuDate->Width(date);
        int dateX = (config.menuAdjustLeft) ? (geoManager->menuDateWidth - dateW - 2*geoManager->menuSpace) : (geoManager->menuSpace);
        pixmapDate->DrawText(cPoint(dateX, (geoManager->menuHeaderHeight - fontManager->menuDate->Height()) / 2), date, Theme.Color(clrMenuFontDate), clrTransparent, fontManager->menuDate);
        lastDate = date;
    }
}

void cNopacityDisplayMenuView::DrawDiskUsage(void) {
    int iconWidth = geoManager->menuDiskUsageWidth * 3/4;
    int DiskUsage = cVideoDiskUsage::UsedPercent();
    bool DiskAlert = DiskUsage > diskUsageAlert;
    tColor bgColor = DiskAlert ? Theme.Color(clrDiskAlert) : Theme.Color(clrMenuBack);
    pixmapDiskUsage->Fill(Theme.Color(clrMenuBorder));
    pixmapDiskUsage->DrawRectangle(cRect(2,2,geoManager->menuDiskUsageWidth-4, geoManager->menuDiskUsageHeight-4), bgColor);
    cImage *imgDisc = imgCache->GetSkinIcon("skinIcons/DiskUsage");
    if (imgDisc)
        pixmapDiskUsageIcon->DrawImage(cPoint((geoManager->menuDiskUsageWidth - iconWidth)/2,0), *imgDisc);
    cImage *imgDiscPerc = imgCache->GetSkinIcon("skinIcons/discpercent");
    if (imgDiscPerc)
        pixmapDiskUsageIcon->DrawImage(cPoint(0,4*geoManager->menuDiskUsageHeight/5), *imgDiscPerc);
    pixmapDiskUsageLabel->Fill(clrTransparent);
    cString usage = cString::sprintf("%d%%", DiskUsage);
    cString rest;
    if (config.discUsageStyle == 0)
        rest = cString::sprintf("%02d:%02dh %s", cVideoDiskUsage::FreeMinutes() / 60, cVideoDiskUsage::FreeMinutes() % 60, tr("free"));
    else
        rest = cString::sprintf("%d GB %s", cVideoDiskUsage::FreeMB() / 1024, tr("free"));
    pixmapDiskUsageLabel->DrawRectangle(cRect((geoManager->menuDiskUsageWidth - 4)*DiskUsage/100,4*geoManager->menuDiskUsageHeight/5, (geoManager->menuDiskUsageWidth - 4) - (geoManager->menuDiskUsageWidth - 4)*DiskUsage/100, geoManager->menuDiskUsageHeight/5), 0xDD000000);
    pixmapDiskUsageLabel->DrawText(cPoint((geoManager->menuDiskUsageWidth - fontManager->menuDiskUsagePercent->Width(*usage))/2, 4*geoManager->menuDiskUsageHeight/5), *usage, Theme.Color(clrMenuFontDiscUsage), clrTransparent, fontManager->menuDiskUsagePercent);
    pixmapDiskUsageLabel->DrawText(cPoint((geoManager->menuDiskUsageWidth - fontManager->menuDiskUsage->Width(*rest))/2, (geoManager->menuDiskUsageHeight - fontManager->menuDiskUsage->Height())/2), *rest, Theme.Color(clrMenuFontDiscUsage), clrTransparent, fontManager->menuDiskUsage);

    pixmapDiskUsage->SetLayer(2);
    pixmapDiskUsageIcon->SetLayer(3);
    pixmapDiskUsageLabel->SetLayer(4);
}

void cNopacityDisplayMenuView::ShowDiskUsage(bool show) {
    if (show) {
        pixmapDiskUsage->SetLayer(2);
        pixmapDiskUsageIcon->SetLayer(3);
        pixmapDiskUsageLabel->SetLayer(4);
    } else {
        pixmapDiskUsage->SetLayer(-1);
        pixmapDiskUsageIcon->SetLayer(-1);
        pixmapDiskUsageLabel->SetLayer(-1);
    }
}

void cNopacityDisplayMenuView::DrawButton(const char *text, eBackgroundType bgButton, tColor buttonColor, tColor borderColor, tColor fontColor, int num) {
    if (num < 0)
        return;
    int top = 2*geoManager->menuButtonsBorder;
    int left = num * geoManager->menuButtonWidth + (2*num + 1) * geoManager->menuButtonsBorder;
    if (config.doBlending) {
        pixmapFooter->DrawRectangle(cRect(left, top, geoManager->menuButtonWidth, geoManager->menuButtonHeight), borderColor);
	cImage *back = imgCache->GetBackground(bgButton);
        if (back)
            pixmapFooter->DrawImage(cPoint(left+2, top+2), *back);
    } else {
        pixmapFooter->DrawRectangle(cRect(left, top, geoManager->menuButtonWidth, geoManager->menuButtonHeight), buttonColor);
    }
    if (config.roundedCorners) {
        int radius = config.cornerRadius;
        if (radius > 2) {
            pixmapFooter->DrawEllipse(cRect(left,top,radius,radius), borderColor, -2);
            pixmapFooter->DrawEllipse(cRect(left-2,top-2,radius,radius), Theme.Color(clrMenuBack), -2);

            pixmapFooter->DrawEllipse(cRect(left + geoManager->menuButtonWidth -radius, top,radius,radius), borderColor, -1);
            pixmapFooter->DrawEllipse(cRect(left + geoManager->menuButtonWidth -radius+2,top-2,radius,radius), Theme.Color(clrMenuBack), -1);

            pixmapFooter->DrawEllipse(cRect(left,top + geoManager->menuButtonHeight -radius,radius,radius), borderColor, -3);
            pixmapFooter->DrawEllipse(cRect(left - 2, top + geoManager->menuButtonHeight - radius + 2,radius,radius), Theme.Color(clrMenuBack), -3);

            pixmapFooter->DrawEllipse(cRect(left + geoManager->menuButtonWidth -radius, top + geoManager->menuButtonHeight -radius,radius,radius), borderColor, -4);
            pixmapFooter->DrawEllipse(cRect(left + geoManager->menuButtonWidth -radius + 2, top + geoManager->menuButtonHeight -radius + 2,radius,radius), Theme.Color(clrMenuBack), -4);
        }
    }
    
    int textWidth = fontManager->menuButtons->Width(text);
    int textHeight = fontManager->menuButtons->Height();
    pixmapButtonsText->DrawRectangle(cRect(left, top, geoManager->menuButtonWidth, geoManager->menuButtonHeight), clrTransparent);
    pixmapButtonsText->DrawText(cPoint(left + (geoManager->menuButtonWidth-textWidth)/2, top + (geoManager->menuButtonHeight-textHeight)/2), text, fontColor, clrTransparent, fontManager->menuButtons);
}

void cNopacityDisplayMenuView::ClearButton(int num) {
    if (num < 0)
        return;
    int top = 2*geoManager->menuButtonsBorder;
    int left = num * geoManager->menuButtonWidth + (2*num + 1) * geoManager->menuButtonsBorder;
    pixmapFooter->DrawRectangle(cRect(left, top, geoManager->menuButtonWidth, geoManager->menuButtonHeight), Theme.Color(clrMenuBack));
    pixmapButtonsText->DrawRectangle(cRect(left, top, geoManager->menuButtonWidth, geoManager->menuButtonHeight), clrTransparent);
}

int cNopacityDisplayMenuView::GetTimersInitHeight(void) {
    int initHeight = geoManager->menuHeaderHeight + 2*geoManager->menuSpace;
    if (config.showDiscUsage)
        initHeight += geoManager->menuDiskUsageHeight;
    return initHeight;
}

int cNopacityDisplayMenuView::GetTimersMaxHeight(void) {
    return geoManager->menuHeaderHeight + geoManager->menuContentHeight;
}

cNopacityTimer *cNopacityDisplayMenuView::DrawTimerConflict(int numConflicts, int y) {
    cNopacityTimer *t = new cNopacityTimer(osd, imgCache, numConflicts, fontManager->menuTimers, fontManager->menuTimersHead);
    t->SetGeometry(geoManager->menuTimersWidth, y);
    t->CreateConflictText();
    t->CalculateHeight(geoManager->menuSpace);
    int timerX = (config.menuAdjustLeft) ? (geoManager->osdWidth - geoManager->menuTimersWidth - 10) : 10;
    t->CreatePixmaps(timerX);
    t->Render();
    return t;
}

cNopacityTimer *cNopacityDisplayMenuView::DrawTimer(const cTimer *Timer, int y) {
    cNopacityTimer *t = new cNopacityTimer(osd, imgCache, Timer, fontManager->menuTimers, fontManager->menuTimersHead);
    t->SetGeometry(geoManager->menuTimersWidth, y);
    t->CreateDate();
    t->CreateShowName();
    t->CalculateHeight(geoManager->menuSpace);
    int timerX = (config.menuAdjustLeft) ? (geoManager->osdWidth - geoManager->menuTimersWidth - 10) : 10;
    t->CreatePixmaps(timerX);
    t->Render();
    return t;
}

void cNopacityDisplayMenuView::DrawScrollbar(double Height, double Offset) {
    int totalHeight = pixmapScrollbar->ViewPort().Height() - 6;
    int height = Height * totalHeight;
    int offset = Offset * totalHeight;
    pixmapScrollbar->Fill(Theme.Color(clrMenuScrollBar));
    pixmapScrollbar->DrawRectangle(cRect(2,2,geoManager->menuWidthScrollbar-4,totalHeight+2), Theme.Color(clrMenuScrollBarBack));
    pixmapScrollbar->DrawRectangle(cRect(3,3 + offset,geoManager->menuWidthScrollbar-6,height), Theme.Color(clrMenuScrollBar));
}

void cNopacityDisplayMenuView::ClearScrollbar(void) {
    pixmapScrollbar->Fill(clrTransparent);
}


void cNopacityDisplayMenuView::DrawMessage(eMessageType Type, const char *Text) {
    tColor col = Theme.Color(clrMessageStatus);
    tColor colFont = Theme.Color(clrMessageFontStatus);
    switch (Type) {
        case mtStatus: 
            col = Theme.Color(clrMessageStatus);
            colFont = Theme.Color(clrMessageFontStatus);
            break;
        case mtInfo:
            col = Theme.Color(clrMessageInfo);
            colFont = Theme.Color(clrMessageFontInfo);
            break;
        case mtWarning:
            col = Theme.Color(clrMessageWarning);
            colFont = Theme.Color(clrMessageFontWarning);
            break;
        case mtError:
            col = Theme.Color(clrMessageError);
            colFont = Theme.Color(clrMessageFontError);
            break;
    }
    if (pixmapStatus) {
        ClearMessage();
    }
    pixmapStatus = osd->CreatePixmap(7, cRect(0.1*geoManager->osdWidth, 0.8*geoManager->osdHeight, geoManager->menuMessageWidth, geoManager->menuMessageHeight));
    pixmapStatus->Fill(col);
    if (config.doBlending) {
        cImage imgBack = imgCache->GetBackground(Theme.Color(clrMenuBack), col, geoManager->menuMessageWidth-2, geoManager->menuMessageHeight-2, true);
        pixmapStatus->DrawImage(cPoint(1, 1), imgBack);
    }
    if (config.roundedCorners) {
        DrawRoundedCornersWithBorder(pixmapStatus, col, config.cornerRadius, geoManager->menuMessageWidth, geoManager->menuMessageHeight);
    }
    int textWidth = fontManager->menuMessage->Width(Text);
    tColor clrFontBack = (config.doBlending)?(clrTransparent):col;
    pixmapStatus->DrawText(cPoint((geoManager->menuMessageWidth - textWidth) / 2, (geoManager->menuMessageHeight - fontManager->menuMessage->Height()) / 2), Text, colFont, clrFontBack, fontManager->menuMessage);
}

void cNopacityDisplayMenuView::ClearMessage(void) {
    if (pixmapStatus) {
        osd->DestroyPixmap(pixmapStatus);
        pixmapStatus = NULL;
    }
}

void cNopacityDisplayMenuView::SetDetailViewSize(eDetailViewType detailViewType, cNopacityMenuDetailView *detailView) {
    int x = (config.menuAdjustLeft) ? 0 : geoManager->osdWidth - geoManager->menuContentWidthFull + 2*geoManager->menuSpace;
    int width = 0;
    int height = 0;
    int top = 0;
    int contentBorder = 30;
    int detailHeaderHeight = 0;
    
    switch (detailViewType) {
        case dvEvent:
            detailHeaderHeight = max(config.detailViewLogoHeight, config.epgImageHeight)+4;
            contentBorder = config.borderDetailedEPG;
            break;
        case dvRecording:
            detailHeaderHeight = config.epgImageHeight + 4;
            contentBorder = config.borderDetailedRecordings;
            break;
        case dvText:
            detailHeaderHeight = 0;         
            break;
        default:
            break;
    }
    width = geoManager->menuContentWidthFull - 2*geoManager->menuSpace;
    height = geoManager->menuContentHeight;
    top = geoManager->menuHeaderHeight;
    detailView->SetGeometry(x, width, height, top, contentBorder, detailHeaderHeight);
}

void cNopacityDisplayMenuView::DrawRssFeed(std::string feedName) {
    pixmapRssFeedBackground->Fill(clrBlack);
    pixmapRssFeed->Fill(clrTransparent);
    feedNameLength = fontManager->menuRssFeed->Width(feedName.c_str());
    int labelWidth = 2 + geoManager->menuRssFeedHeight + 2 + feedNameLength + 6;
    pixmapRssFeed->Fill(Theme.Color(clrRSSFeedBorder));
    if (config.doBlending) {
        cImage imgBack = imgCache->GetBackground(Theme.Color(clrRSSFeedHeaderBack), Theme.Color(clrRSSFeedHeaderBackBlend), labelWidth, geoManager->menuRssFeedHeight - 4);
        pixmapRssFeed->DrawImage(cPoint(2,2), imgBack);
        cImage imgBack2 = imgCache->GetBackground(Theme.Color(clrRSSFeedBack), Theme.Color(clrRSSFeedBackBlend), geoManager->osdWidth - labelWidth - 2, geoManager->menuRssFeedHeight - 4);
        pixmapRssFeed->DrawImage(cPoint(labelWidth,2), imgBack2);
    } else {
        pixmapRssFeed->DrawRectangle(cRect(2, 2, labelWidth, geoManager->menuRssFeedHeight - 4), Theme.Color(clrRSSFeedHeaderBack));
        pixmapRssFeed->DrawRectangle(cRect(labelWidth, 2, geoManager->osdWidth - labelWidth - 2, geoManager->menuRssFeedHeight - 4), Theme.Color(clrRSSFeedBack));        
    }
    pixmapRssFeed->DrawText(cPoint(geoManager->menuRssFeedHeight + 2, (geoManager->menuRssFeedHeight - fontManager->menuRssFeed->Height()) / 2), feedName.c_str(), Theme.Color(clrRSSFeedHeaderText), clrTransparent, fontManager->menuRssFeed);
    pixmapRssFeedIcon->Fill(clrTransparent);
    cImage *imgIcon = imgCache->GetSkinIcon("skinIcons/rss");
    if (imgIcon)
        pixmapRssFeedIcon->DrawImage(cPoint(2,2), *imgIcon);
}

cPoint cNopacityDisplayMenuView::GetRssFeedPosition(void) {
    int x = geoManager->menuRssFeedHeight + feedNameLength + 10;
    int y = geoManager->osdHeight - geoManager->menuRssFeedHeight + 2;
    return cPoint(x, y);
}

cPoint cNopacityDisplayMenuView::GetRssFeedSize(void) {
    int width = geoManager->osdWidth - (geoManager->menuRssFeedHeight + feedNameLength + 12);
    int height = geoManager->menuRssFeedHeight - 4;
    return cPoint(width, height);
}
