QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    button.cpp \
    changenamedialog.cpp \
    enterdialog.cpp \
    foodform.cpp \
    foodselectdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    material/layouts/qtmaterialsnackbarlayout.cpp \
    material/lib/qtmaterialcheckable.cpp \
    material/lib/qtmaterialcheckable_internal.cpp \
    material/lib/qtmaterialoverlaywidget.cpp \
    material/lib/qtmaterialripple.cpp \
    material/lib/qtmaterialrippleoverlay.cpp \
    material/lib/qtmaterialstatetransition.cpp \
    material/lib/qtmaterialstyle.cpp \
    material/lib/qtmaterialtheme.cpp \
    material/qtmaterialappbar.cpp \
    material/qtmaterialautocomplete.cpp \
    material/qtmaterialautocomplete_internal.cpp \
    material/qtmaterialavatar.cpp \
    material/qtmaterialbadge.cpp \
    material/qtmaterialcheckbox.cpp \
    material/qtmaterialcircularprogress.cpp \
    material/qtmaterialcircularprogress_internal.cpp \
    material/qtmaterialdialog.cpp \
    material/qtmaterialdialog_internal.cpp \
    material/qtmaterialdrawer.cpp \
    material/qtmaterialdrawer_internal.cpp \
    material/qtmaterialfab.cpp \
    material/qtmaterialflatbutton.cpp \
    material/qtmaterialflatbutton_internal.cpp \
    material/qtmaterialiconbutton.cpp \
    material/qtmateriallist.cpp \
    material/qtmateriallistitem.cpp \
    material/qtmaterialmenu.cpp \
    material/qtmaterialmenu_internal.cpp \
    material/qtmaterialpaper.cpp \
    material/qtmaterialprogress.cpp \
    material/qtmaterialprogress_internal.cpp \
    material/qtmaterialradiobutton.cpp \
    material/qtmaterialraisedbutton.cpp \
    material/qtmaterialscrollbar.cpp \
    material/qtmaterialscrollbar_internal.cpp \
    material/qtmaterialslider.cpp \
    material/qtmaterialslider_internal.cpp \
    material/qtmaterialsnackbar.cpp \
    material/qtmaterialsnackbar_internal.cpp \
    material/qtmaterialtable.cpp \
    material/qtmaterialtabs.cpp \
    material/qtmaterialtabs_internal.cpp \
    material/qtmaterialtextfield.cpp \
    material/qtmaterialtextfield_internal.cpp \
    material/qtmaterialtoggle.cpp \
    material/qtmaterialtoggle_internal.cpp \
    materialsdialog.cpp \
    materialsform.cpp \
    menuform.cpp \
    passwordform.cpp \
    pricingform.cpp \
    qjsonwebtoken.cpp

HEADERS += \
    button.h \
    changenamedialog.h \
    enterdialog.h \
    foodform.h \
    foodselectdialog.h \
    mainwindow.h \
    material/layouts/qtmaterialsnackbarlayout.h \
    material/layouts/qtmaterialsnackbarlayout_p.h \
    material/lib/qtmaterialcheckable.h \
    material/lib/qtmaterialcheckable_internal.h \
    material/lib/qtmaterialcheckable_p.h \
    material/lib/qtmaterialoverlaywidget.h \
    material/lib/qtmaterialripple.h \
    material/lib/qtmaterialrippleoverlay.h \
    material/lib/qtmaterialstatetransition.h \
    material/lib/qtmaterialstatetransitionevent.h \
    material/lib/qtmaterialstyle.h \
    material/lib/qtmaterialstyle_p.h \
    material/lib/qtmaterialtheme.h \
    material/lib/qtmaterialtheme_p.h \
    material/qtmaterialappbar.h \
    material/qtmaterialappbar_p.h \
    material/qtmaterialautocomplete.h \
    material/qtmaterialautocomplete_internal.h \
    material/qtmaterialautocomplete_p.h \
    material/qtmaterialavatar.h \
    material/qtmaterialavatar_p.h \
    material/qtmaterialbadge.h \
    material/qtmaterialbadge_p.h \
    material/qtmaterialcheckbox.h \
    material/qtmaterialcheckbox_p.h \
    material/qtmaterialcircularprogress.h \
    material/qtmaterialcircularprogress_internal.h \
    material/qtmaterialcircularprogress_p.h \
    material/qtmaterialdialog.h \
    material/qtmaterialdialog_internal.h \
    material/qtmaterialdialog_p.h \
    material/qtmaterialdrawer.h \
    material/qtmaterialdrawer_internal.h \
    material/qtmaterialdrawer_p.h \
    material/qtmaterialfab.h \
    material/qtmaterialfab_p.h \
    material/qtmaterialflatbutton.h \
    material/qtmaterialflatbutton_internal.h \
    material/qtmaterialflatbutton_p.h \
    material/qtmaterialiconbutton.h \
    material/qtmaterialiconbutton_p.h \
    material/qtmateriallist.h \
    material/qtmateriallist_p.h \
    material/qtmateriallistitem.h \
    material/qtmateriallistitem_p.h \
    material/qtmaterialmenu.h \
    material/qtmaterialmenu_internal.h \
    material/qtmaterialmenu_p.h \
    material/qtmaterialpaper.h \
    material/qtmaterialpaper_p.h \
    material/qtmaterialprogress.h \
    material/qtmaterialprogress_internal.h \
    material/qtmaterialprogress_p.h \
    material/qtmaterialradiobutton.h \
    material/qtmaterialradiobutton_p.h \
    material/qtmaterialraisedbutton.h \
    material/qtmaterialraisedbutton_p.h \
    material/qtmaterialscrollbar.h \
    material/qtmaterialscrollbar_internal.h \
    material/qtmaterialscrollbar_p.h \
    material/qtmaterialslider.h \
    material/qtmaterialslider_internal.h \
    material/qtmaterialslider_p.h \
    material/qtmaterialsnackbar.h \
    material/qtmaterialsnackbar_internal.h \
    material/qtmaterialsnackbar_p.h \
    material/qtmaterialtable.h \
    material/qtmaterialtable_p.h \
    material/qtmaterialtabs.h \
    material/qtmaterialtabs_internal.h \
    material/qtmaterialtabs_p.h \
    material/qtmaterialtextfield.h \
    material/qtmaterialtextfield_internal.h \
    material/qtmaterialtextfield_p.h \
    material/qtmaterialtoggle.h \
    material/qtmaterialtoggle_internal.h \
    material/qtmaterialtoggle_p.h \
    materialsdialog.h \
    materialsform.h \
    menuform.h \
    passwordform.h \
    pricingform.h \
    qjsonwebtoken.h

FORMS += \
    changenamedialog.ui \
    enterdialog.ui \
    foodform.ui \
    foodselectdialog.ui \
    mainwindow.ui \
    materialsdialog.ui \
    materialsform.ui \
    menuform.ui \
    passwordform.ui \
    pricingform.ui

TRANSLATIONS += \
    FoodPricer_fa_IR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    material/resources.qrc \
    reso.qrc

RC_FILE += "foodpricer.rc"

SUBDIRS += \
    material/components.pro
