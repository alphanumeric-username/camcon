#include <camcon/constants.hpp>
#include <camcon/init.hpp>
#include <camcon/arg_parse.hpp>
#include <camcon/preset_store.hpp>
#include <camcon/utils.hpp>
#include <camcon/rt_control/select_device_window.hpp>

#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <win32_wrapper/init.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>
#include <win32_wrapper/gdi/gdi_text.hpp>
#include <win32_wrapper/layout/layout_manager.hpp>
#include <win32_wrapper/layout/weighted_divider_layout.hpp>
#include <win32_wrapper/layout/control_container.hpp>
#include <win32_wrapper/layout/label_container.hpp>
#include <win32_wrapper/layout/tiled_layout.hpp>

#include <system/str_tools.hpp>

#include <Dbt.h>
// #include <ks.h>
// #include <ksmedia.h>
#include <stringapiset.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <Shlwapi.h>

#include <algorithm>
#include <iostream>
#include <exception>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

camcon::arg_parse::RTControlArgs processArgs(PWSTR pCmdLine);
std::shared_ptr<win32w::Window> createMainWindow(std::wstring);
void createButtonTiledLayout(win32w::LayoutManager& lm, 
    std::shared_ptr<win32w::Control> btnUp,
    std::shared_ptr<win32w::Control> btnDown,
    std::shared_ptr<win32w::Control> btnLeft,
    std::shared_ptr<win32w::Control> btnRight
);

void createTrackbarLayout(win32w::LayoutManager& lm,
    std::shared_ptr<win32w::GDIText> txtPlus,
    std::shared_ptr<win32w::Control> tbZoom,
    std::shared_ptr<win32w::GDIText> txtMinus
);

void createRightLayout(win32w::LayoutManager& lm, 
    std::shared_ptr<win32w::GDIText> txtPreset,
    std::shared_ptr<win32w::Control> cbPreset,
    std::shared_ptr<win32w::GDIText> txtSave,
    std::shared_ptr<win32w::Control> editSave,
    std::shared_ptr<win32w::Control> btnSave
);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    camcon::init();

    // auto hinstDll = LoadLibrary(L"C:\\Windows\\System32\\ComCtl32.dll");
    // auto DllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hinstDll, "DllGetVersion");

    // DLLVERSIONINFO dvi{};
    // ZeroMemory(&dvi, sizeof(dvi));
    // dvi.cbSize = sizeof(dvi);
    // (*DllGetVersion)(&dvi);

    auto args = processArgs(pCmdLine);
    
    auto devIdx = args.index;
    if(!args.indexWasProvided)
    {
        devIdx = camcon::promptDeviceIndexWindow();
        if(devIdx < 0)
        {
            return 0;
        }
    }

    mfw::VideoDeviceEnumerator vde{};

    try {
        vde.checkIndexInBounds(devIdx);
    } catch(std::exception e)
    {
        MessageBox(nullptr, L"There is no device with the provided index", L"Error", MB_OK);
        return 2;
    }

    auto devSymbLink {vde.getSymbolicLink(devIdx)};
    auto devFriendlyName { vde.getDeviceName(devIdx) };
    auto devSrc {vde.getDeviceFromSymbolicLink(devSymbLink)};
    mfw::CameraController cc{};
    cc.setDevice(devSrc);

    auto win = createMainWindow(devFriendlyName);

    auto leRoot { std::make_shared<win32w::WeightedDividerLayout>() };
    leRoot->ratios = { 0.4f, 0.05f, 0.05f, 0.5f };
    leRoot->margin = 10;

    win32w::LayoutManager lm{};
    lm.win = win;
    lm.root = std::static_pointer_cast<win32w::LayoutElement>(leRoot);

    win32w::ControlFactory cf{};
    
    auto btnLeft = cf.createButton(win, L"<", 0, 0, 0, 0);
    auto btnRight = cf.createButton(win, L">", 0, 0, 0, 0);
    auto btnUp = cf.createButton(win, L"^", 0, 0, 0, 0);
    auto btnDown = cf.createButton(win, L"v", 0, 0, 0, 0);

    auto txtPreset = std::make_shared<win32w::GDIText>();
    auto cbPreset = cf.createComboBox(win, 0, 0, 0, 0);
    auto txtSave = std::make_shared<win32w::GDIText>();
    auto editSave = cf.createEdit(win, 0, 0, 0, 0);
    auto btnSave = cf.createButton(win, L"Save Preset", 0, 0, 0, 0);

    auto zoomRange = cc.getPropertyRange(tagCameraControlProperty::CameraControl_Zoom);
    // zoomRange.pMin = 0;
    // zoomRange.pMax = 500;

    auto txtPlus { std::make_shared<win32w::GDIText>() };
    auto txtMinus { std::make_shared<win32w::GDIText>() };
    auto tbZoom = cf.createTrackBar(win, L"Zoom", 0, 0, 0, 0, zoomRange.pMin, zoomRange.pMax,
        WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_DOWNISLEFT );
    tbZoom->setTrackPosition(zoomRange.pMax);
    
    createButtonTiledLayout(lm, btnUp, btnDown, btnLeft, btnRight);
    createTrackbarLayout(lm, txtPlus, tbZoom, txtMinus);
    leRoot->addChild(std::make_shared<win32w::LayoutElement>());
    createRightLayout(lm, txtPreset, cbPreset, txtSave, editSave, btnSave);
    lm.update();
    
    txtPlus->contents = L"+";
    txtPlus->fontBuilder.size = 16;
    txtPlus->alignment = win32w::MIDDLE | win32w::CENTER;
    txtMinus->contents = L"-";
    txtMinus->fontBuilder.size = 16;
    txtMinus->alignment = win32w::MIDDLE | win32w::CENTER;

    txtPreset->contents = L"Current preset:";
    txtPreset->fontBuilder.size = 16;
    txtPreset->alignment = win32w::LEFT | win32w::MIDDLE;

    txtSave->contents = L"New preset:";
    txtSave->fontBuilder.size = 16;
    txtSave->alignment = win32w::LEFT | win32w::MIDDLE;

    win->addGDIText(txtPlus);
    win->addGDIText(txtMinus);
    win->addGDIText(txtPreset);
    win->addGDIText(txtSave);
    
    cbPreset->setText(L"<Custom>");
    Edit_LimitText(editSave->hwnd, camcon::MAX_EDIT_TEXT);
    
    camcon::PresetStore ps{};

    ps.root = camcon::getExePath();
    auto presets = ps.loadPresets();
    
    for(auto & p : presets)
    {
        cbPreset->addItem(p.name);
    }

    auto varyCamProp = [&](tagCameraControlProperty ccp, int dir)
    {
        auto prop = cc.getProperty(ccp);

        int sign = dir > 0 ? 1
                 : dir < 0 ? -1
                 : 0;

        auto delta = sign * (GetKeyState(VK_SHIFT) & 0x8000 ? 10 : 1);

        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + delta);
        }

        cbPreset->setText(L"<Custom>");
    };

    btnLeft->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        varyCamProp(tagCameraControlProperty::CameraControl_Pan, -1);
    });
    
    btnRight->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        varyCamProp(tagCameraControlProperty::CameraControl_Pan, 1);
    });
    
    btnUp->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        varyCamProp(tagCameraControlProperty::CameraControl_Tilt, 1);
    });
    
    btnDown->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        varyCamProp(tagCameraControlProperty::CameraControl_Tilt, -11);
    });

    btnSave->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if(!cc.deviceIsSet()) {
            return;
        }

        std::vector<mfw::CameraControlPropertyValue> ccprops {
            cc.getProperty(tagCameraControlProperty::CameraControl_Pan),
            cc.getProperty(tagCameraControlProperty::CameraControl_Tilt),
            cc.getProperty(tagCameraControlProperty::CameraControl_Zoom),
        };

        camcon::Preset p{};
        p.name = sys::str_tools::trim(editSave->getText());

        if(p.name.empty())
        {
            return;
        }

        p.config = ccprops;

        bool found = false;
        int i;
        for(i = 0; i < presets.size(); i++)
        {
            auto& p2 { presets.at(i) };
            if(p2.name.compare(p.name) == 0)
            {
                p2.config = p.config;
                found = true;
                break;
            }
        }

        if(found) {
            ComboBox_SetCurSel(cbPreset->hwnd, i);
        } else {
            presets.emplace_back(p);
            cbPreset->addItem(p.name);
            ComboBox_SetCurSel(cbPreset->hwnd, presets.size() - 1);
        }

        editSave->setText(L"");
        
        ps.savePreset(p);
    });

    cbPreset->setCallback(CBN_SELCHANGE, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if(!cc.deviceIsSet()) {
            return;
        }

        auto currIdx = ComboBox_GetCurSel(cbPreset->hwnd);
        if(currIdx == CB_ERR)
        {
            return;
        }

        auto p = presets.at(currIdx);
        for(auto& prop : p.config)
        {
            cc.setProperty(prop.prop, prop.lvalue);
            if(prop.prop == tagCameraControlProperty::CameraControl_Zoom)
            {
                int pos 
                {
                    zoomRange.pMin + (zoomRange.pMax - prop.lvalue)
                };

                tbZoom->setTrackPosition(pos);
            }
        }
    });

    tbZoom->setCallback(WM_VSCROLL, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        int pos 
        { 
            zoomRange.pMin + (zoomRange.pMax - tbZoom->getTrackPosition())
        };

        cc.setProperty(tagCameraControlProperty::CameraControl_Zoom, pos);
        cbPreset->setText(L"<Custom>");
    });


    auto windHdevNotify = camcon::registerForDeviceNotification(win->hwnd);
    win->setCallback(WM_DEVICECHANGE, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVICEARRIVAL)
        {
            auto dbh = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);

            if(dbh == nullptr || dbh->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE)
            {
                return;
            }
            
            auto dbdi = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(dbh);

            std::wstring rightSymbLink { dbdi->dbcc_name };

            auto llsl = sys::str_tools::tolower(devSymbLink).substr(0, 50);
            auto lrsl = sys::str_tools::tolower(rightSymbLink).substr(0, 50);

            if(llsl.compare(lrsl) == 0)
            {
                auto is_DBT_DEVICEREMOVECOMPLETE = wParam == DBT_DEVICEREMOVECOMPLETE;
                auto is_DBT_DEVICEARRIVAL = wParam == DBT_DEVICEARRIVAL;

                auto state = is_DBT_DEVICEREMOVECOMPLETE ? L"Disconnected" :
                             is_DBT_DEVICEARRIVAL ? L"Connected" : L"Unknow state";

                SetWindowText(hwnd, sys::str_tools::join({camcon::TITLE, camcon::VERSION_STR, L"-", devFriendlyName + L":", state}).c_str());

                if(is_DBT_DEVICEARRIVAL)
                {
                    cc.setDevice(vde.getDeviceFromSymbolicLink(devSymbLink));
                    win->enableControls(true);
                } else {
                    cc.setDevice(nullptr);
                    win->enableControls(false);
                }
            }
        } 
    });

    ShowWindow(win->hwnd, nCmdShow);

    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    windHdevNotify && UnregisterDeviceNotification(windHdevNotify);

    vde.release();

    camcon::finalize();
    return 0;
}

camcon::arg_parse::RTControlArgs processArgs(PWSTR pCmdLine)
{
    int argc;
    auto argv {CommandLineToArgvW(pCmdLine, &argc)};

    auto args { camcon::arg_parse::parseRTControlArgs(argc, argv)};

    if(!args.syntaxIsValid)
    {
        MessageBox(nullptr, L"Usage:\n\trt-control <device_index>", L"Syntax Error", MB_OK);
        args.indexWasProvided = false;
        return args;
    }

    return args;
}

std::shared_ptr<win32w::Window> createMainWindow(std::wstring devFriendlyName)
{
    win32w::WindowBuilder wb{};
    wb.generateClassName();
    wb.createClass();
    auto style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    auto win_title = sys::str_tools::join({camcon::TITLE, camcon::VERSION_STR, L"-", devFriendlyName + L":", L"Connected"});

    return wb.build(win_title, CW_USEDEFAULT, CW_USEDEFAULT, camcon::W_WIDTH, camcon::W_HEIGHT, style);
}

void createButtonTiledLayout(win32w::LayoutManager & lm, 
    std::shared_ptr<win32w::Control> btnUp, 
    std::shared_ptr<win32w::Control> btnDown, 
    std::shared_ptr<win32w::Control> btnLeft, 
    std::shared_ptr<win32w::Control> btnRight)
{
    auto tl { std::make_shared<win32w::TiledLayout>() };
    tl->itemWidth = 64;
    tl->itemHeight = 64;
    tl->padding = 10;
    tl->columns = 3;

    tl->addChild(std::make_shared<win32w::LayoutElement>());
    tl->addChild(std::make_shared<win32w::ControlContainer>(btnUp));
    tl->addChild(std::make_shared<win32w::LayoutElement>());
    tl->addChild(std::make_shared<win32w::ControlContainer>(btnLeft));
    tl->addChild(std::make_shared<win32w::LayoutElement>());
    tl->addChild(std::make_shared<win32w::ControlContainer>(btnRight));
    tl->addChild(std::make_shared<win32w::LayoutElement>());
    tl->addChild(std::make_shared<win32w::ControlContainer>(btnDown));
    tl->addChild(std::make_shared<win32w::LayoutElement>());

    lm.root->addChild(tl);
}


void createTrackbarLayout(win32w::LayoutManager& lm,
    std::shared_ptr<win32w::GDIText> txtPlus,
    std::shared_ptr<win32w::Control> tbZoom,
    std::shared_ptr<win32w::GDIText> txtMinus
) {
    auto leTrackbar { std::make_shared<win32w::WeightedDividerLayout>() };
    leTrackbar->orientation = win32w::VERTICAL;
    leTrackbar->ratios = { 0.1f, 0.8f, 0.1f };

    leTrackbar->addChild(std::make_shared<win32w::LabelContainer>(txtPlus));
    leTrackbar->addChild(std::make_shared<win32w::ControlContainer>(tbZoom));
    leTrackbar->addChild(std::make_shared<win32w::LabelContainer>(txtMinus));
    
    lm.root->addChild(leTrackbar);
}

void createRightLayout(win32w::LayoutManager& lm, 
    std::shared_ptr<win32w::GDIText> txtPreset,
    std::shared_ptr<win32w::Control> cbPreset,
    std::shared_ptr<win32w::GDIText> txtSave,
    std::shared_ptr<win32w::Control> editSave,
    std::shared_ptr<win32w::Control> btnSave
) {
    auto wlRight { std::make_shared<win32w::WeightedDividerLayout>() };
    auto wlBtnSave { std::make_shared<win32w::WeightedDividerLayout>() };

    wlBtnSave->ratios = {0.5f, 0.5f};
    wlBtnSave->margin = 0;
    wlBtnSave->addChild(std::make_shared<win32w::LayoutElement>());
    wlBtnSave->addChild(std::make_shared<win32w::ControlContainer>(btnSave));

    wlRight->orientation = win32w::WeightedDividerLayoutOrientation::VERTICAL;
    wlRight->ratios = {1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 4.0f};

    wlRight->addChild(std::make_shared<win32w::LabelContainer>(txtPreset));
    wlRight->addChild(std::make_shared<win32w::ControlContainer>(cbPreset));
    wlRight->addChild(std::make_shared<win32w::LabelContainer>(txtSave));
    wlRight->addChild(std::make_shared<win32w::ControlContainer>(editSave));
    // wlRight->addChild(std::make_shared<win32w::ControlContainer>(btnSave));
    wlRight->addChild(wlBtnSave);
    wlRight->addChild(std::make_shared<win32w::LayoutElement>());

    lm.root->addChild(wlRight);
}