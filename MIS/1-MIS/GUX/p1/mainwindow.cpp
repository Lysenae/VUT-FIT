// File:        mainwindow.cpp
// Author:      Daniel Klimaj (xklima22)
// Description: Main application window and widgets.

#include "mainwindow.h"

// Init static variables
int MainWindow::m_button_pressed   = 0;
int MainWindow::x1                 = 0;
int MainWindow::x2                 = 0;
int MainWindow::y1                 = 0;
int MainWindow::y2                 = 0;
Widget MainWindow::m_main_win      = 0;
Widget MainWindow::m_shape_point   = 0;
Widget MainWindow::m_shape_line    = 0;
Widget MainWindow::m_shape_rect    = 0;
Widget MainWindow::m_shape_ellipse = 0;
Widget MainWindow::m_border_btn    = 0;
Widget MainWindow::m_fill_btn      = 0;
Widget MainWindow::m_line_width_sc = 0;
Widget MainWindow::m_fg_cb         = 0;
Widget MainWindow::m_bg_cb         = 0;
bool MainWindow::m_quit_dlg_exists = false;
Widget MainWindow::m_quit_dialog   = 0;

///
/// \brief Constructor.
/// \param argc Command line arguments count
/// \param argv Command line arguments
///
MainWindow::MainWindow(int argc, char **argv)
{
  XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

  m_top_level = XtVaAppInitialize(
    &m_app_context,
    "Draw",
    NULL, 0,
    &argc, argv,
    NULL,
    XmNx, 300,
    XmNy, 150,
    XmNminHeight, 300,
    XmNminWidth, 400,
    NULL
  );

  if(!Colors::Init(m_top_level))
  {
    fprintf(stderr, "Failed to initialize colors\n");
    exit(1);
  }

  m_main_win = XtVaCreateManagedWidget(
    "mainWin",
    xmMainWindowWidgetClass,
    m_top_level,
    XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
    NULL
  );

  CreateMenu();

  m_frame = XtVaCreateManagedWidget(
    "frame",
    xmFrameWidgetClass,
    m_main_win,
    NULL
  );

  m_draw_area = XtVaCreateManagedWidget(
    "drawingArea",
    xmDrawingAreaWidgetClass,
    m_frame,
    XmNwidth, 500,
    XmNheight, 300,
    XmNminHeight, 150,
    XmNminWidth, 150,
    XmNbackground, Colors::Background(),
    NULL
  );

  m_tools = XtVaCreateManagedWidget(
    "tools",
    xmRowColumnWidgetClass,
    m_main_win,
    XmNentryAlignment, XmALIGNMENT_BEGINNING,
    XmNorientation, XmVERTICAL,
    XmNpacking, XmPACK_TIGHT,
    NULL
  );

  m_tools_shapes = XtVaCreateManagedWidget(
    "toolsShapes",
    xmRowColumnWidgetClass,
    m_tools,
    XmNentryAlignment, XmALIGNMENT_CENTER,
    XmNorientation, XmHORIZONTAL,
    XmNpacking, XmPACK_COLUMN,
    NULL
  );

  m_tools_border = XtVaCreateManagedWidget(
    "toolsBorder",
    xmRowColumnWidgetClass,
    m_tools,
    XmNentryAlignment, XmALIGNMENT_CENTER,
    XmNorientation, XmHORIZONTAL,
    XmNpacking, XmPACK_COLUMN,
    NULL
  );

  m_tools_style = XtVaCreateManagedWidget(
    "toolsStyle",
    xmRowColumnWidgetClass,
    m_tools,
    XmNentryAlignment, XmALIGNMENT_CENTER,
    XmNorientation, XmHORIZONTAL,
    XmNpacking, XmPACK_TIGHT,
    NULL
  );

  CreateTools();

  XmMainWindowSetAreas(m_main_win, NULL, m_tools, NULL, NULL, m_frame);

  Atom prop = XmInternAtom(XtDisplay(m_top_level),
    const_cast<char*>("WM_PROTOCOLS"), False);
  Atom prot = XInternAtom(XtDisplay(m_top_level),
    const_cast<char*>("WM_DELETE_WINDOW"), True);
  XmAddProtocols(m_top_level, prop, &prot, 1);
  XmAddProtocolCallback(m_top_level, prop, prot, OnQuit, NULL);

  XtAddCallback(m_draw_area, XmNinputCallback, OnDraw, m_draw_area);
  XtAddEventHandler(m_draw_area, ButtonMotionMask, False, HandleDraw, NULL);
  XtAddCallback(m_draw_area, XmNexposeCallback, OnExpose, m_draw_area);

  XtAddCallback(m_menu_clear_btn, XmNactivateCallback, OnClear,
    m_draw_area);
  XtAddCallback(m_menu_exit_btn, XmNactivateCallback, OnQuit, 0);

  m_quit_dlg_exists = false;
  XtRealizeWidget(m_top_level);
}

///
/// \brief Destructor.
///
MainWindow::~MainWindow()
{
  Shape::ClearAll();
}

///
/// \brief Run main loop.
///
int MainWindow::run()
{
  XtAppMainLoop(m_app_context);
  return 0;
}

///
/// \brief Create menubar and menus.
///
void MainWindow::CreateMenu()
{
  Arg args[3];

  m_menu_bar   = XmCreateMenuBar(m_main_win, (char*)"MenuBar", NULL, 0);
  m_menu       = XmCreatePulldownMenu(m_menu_bar, (char*)"FileMenu", NULL, 0);
  m_menu_w     = XmCreatePulldownMenu(m_menu_bar, (char*)"FilePullDown", NULL, 0);
  m_menu_label = XmStringCreateLocalized((char*)"File");

  XtSetArg(args[0], XmNmnemonic, 'F');
  XtSetArg(args[1], XmNlabelString, m_menu_label);
  XtSetArg(args[2], XmNsubMenuId, m_menu_w);

  m_menu_cascade = XmCreateCascadeButton(m_menu_bar, (char*)"File", args, 3);
  XtManageChild(m_menu_cascade);
  XmStringFree(m_menu_label);

  m_menu_accel = XmStringCreateLocalized((char*)"Ctrl+C");
  XtSetArg(args[0], XmNaccelerator, "Ctrl<Key>C");
  XtSetArg(args[1], XmNacceleratorText, m_menu_accel);
  XtSetArg(args[2], XmNmnemonic, 'C');
  m_menu_clear_btn = XmCreatePushButtonGadget(m_menu_w, (char*)"Clear", args, 3);
  XtManageChild(m_menu_clear_btn);

  m_menu_accel = XmStringCreateLocalized((char*)"Ctrl+X");
  XtSetArg(args[0], XmNaccelerator, "Ctrl<Key>X");
  XtSetArg(args[1], XmNacceleratorText, m_menu_accel);
  XtSetArg(args[2], XmNmnemonic, 'x');
  m_menu_exit_btn = XmCreatePushButtonGadget(m_menu_w, (char*)"Exit", args, 3);
  XtManageChild(m_menu_exit_btn);

  XmStringFree(m_menu_accel);
  XtManageChild(m_menu_bar);
}

///
/// \brief Create tool window.
///
void MainWindow::CreateTools()
{
  XmString label;

  m_shape_point = XtVaCreateManagedWidget(
    "Point",
    xmToggleButtonWidgetClass,
    m_tools_shapes,
    NULL
  );
  XtAddCallback(m_shape_point, XmNvalueChangedCallback, OnShapeToggled, NULL);
  XmToggleButtonSetState(m_shape_point, True, False);

  m_shape_line = XtVaCreateManagedWidget(
    "Line",
    xmToggleButtonWidgetClass,
    m_tools_shapes,
    NULL
  );
  XtAddCallback(m_shape_line, XmNvalueChangedCallback, OnShapeToggled, NULL);

  m_shape_rect = XtVaCreateManagedWidget(
    "Rectangle",
    xmToggleButtonWidgetClass,
    m_tools_shapes,
    NULL
  );
  XtAddCallback(m_shape_rect, XmNvalueChangedCallback, OnShapeToggled, NULL);

  m_shape_ellipse = XtVaCreateManagedWidget(
    "Ellipse",
    xmToggleButtonWidgetClass,
    m_tools_shapes,
    NULL
  );
  XtAddCallback(m_shape_ellipse, XmNvalueChangedCallback, OnShapeToggled, NULL);

  m_border_btn = XtVaCreateManagedWidget(
    "Doted line",
    xmToggleButtonWidgetClass,
    m_tools_border,
    NULL
  );
  XtAddCallback(m_border_btn, XmNvalueChangedCallback, OnBorderToggled, NULL);

    m_fill_btn = XtVaCreateManagedWidget(
    "Fill",
    xmToggleButtonWidgetClass,
    m_tools_border,
    NULL
  );
  XtAddCallback(m_fill_btn, XmNvalueChangedCallback, OnFillToggled, NULL);

  Arg args[7];
  label = XmStringCreateSimple((char*)"Line Width");
  XtSetArg(args[0],XmNtitleString, label);
  XtSetArg(args[1],XmNorientation, XmHORIZONTAL);
  XtSetArg(args[2],XmNshowValue, True);
  XtSetArg(args[3],XmNmaximum, 10);
  XtSetArg(args[4],XmNminimum, 0);
  XtSetArg(args[5],XmNhighlightOnEnter, True);
  XtSetArg(args[6],XmNvalue, 1);

  m_line_width_sc = XmCreateScale(m_tools_border, (char*)"lineWidthScale",
    args, 7);
  XtManageChild(m_line_width_sc);
  XtAddCallback(m_line_width_sc, XmNdragCallback, OnLineWidthChanged, NULL);
  XtAddCallback(m_line_width_sc, XmNvalueChangedCallback,
    OnLineWidthChanged, NULL);

  char **col_names = Colors::Names();
  XmString colors[Colors::COLORS];
  for(int i=0; i<Colors::COLORS; ++i)
  {
    colors[i] = XmStringCreateLocalized(col_names[i]);
  }

  label = XmStringCreateSimple((char*)"FG:");
  m_fg_label = XtVaCreateManagedWidget(
    "foregroundLabel",
    xmLabelWidgetClass,
    m_tools_style,
    XmNlabelString, label,
    NULL
  );
  XmStringFree(label);


  m_fg_cb = XtVaCreateManagedWidget(
    "foregroundColor",
    xmComboBoxWidgetClass,
    m_tools_style,
    XmNcomboBoxType, XmDROP_DOWN_LIST,
    XmNitemCount, Colors::COLORS,
    XmNitems, colors,
    XmNselectedPosition, 1,
    NULL
  );
  XtAddCallback(m_fg_cb, XmNselectionCallback, OnColorChanged, NULL);

  label = XmStringCreateSimple((char*)"BG:");
  m_fg_label = XtVaCreateManagedWidget(
    "backgroundLabel",
    xmLabelWidgetClass,
    m_tools_style,
    XmNlabelString, label,
    NULL
  );
  XmStringFree(label);

  m_bg_cb = XtVaCreateManagedWidget(
    "backgroundColor",
    xmComboBoxWidgetClass,
    m_tools_style,
    XmNcomboBoxType, XmDROP_DOWN_LIST,
    XmNitemCount, Colors::COLORS,
    XmNitems, colors,
    NULL
  );
  XtAddCallback(m_bg_cb, XmNselectionCallback, OnColorChanged, NULL);
}

///
/// \brief Show dialog when attempting to quit the application.
///
void MainWindow::ShowQuitDialog()
{
  XmString m_msg;
  XmString m_quit;
  XmString m_cancel;

  if(m_quit_dlg_exists == false)
  {
    m_quit_dialog = XmCreateQuestionDialog(m_main_win, (char*)"quitDialog",
      NULL, 0);
    m_msg         = XmStringCreateSimple((char*)"Quit?");
    m_quit        = XmStringCreateSimple((char*)"Quit");
    m_cancel      = XmStringCreateSimple((char*)"Cancel");

    XtVaSetValues(
      m_quit_dialog,
      XmNmessageString, m_msg,
      XmNokLabelString, m_quit,
      XmNcancelLabelString, m_cancel,
      XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
      NULL
    );

    XtAddCallback(m_quit_dialog, XmNokCallback, OnQdQuit, NULL);
    m_quit_dlg_exists = true;

    XmStringFree(m_msg);
    XmStringFree(m_quit);
    XmStringFree(m_cancel);
  }
  XtManageChild(m_quit_dialog);
}

// *** PRIVATE ***

///
/// \brief Handles draw event (Mouse press).
/// \param w Widget
/// \param client_data XtPointer
/// \param event XEvent*
/// \param cont Boolean*
///
void MainWindow::HandleDraw(Widget w, XtPointer client_data, XEvent *event,
Boolean *cont)
{
  if(m_button_pressed)
  {
    Shape::SetInputGC(w);
    Shape::SetInputStyle(w);

    if(m_button_pressed > 1)
    {
      Shape::Draw(w, x1, y1, x2, y2);
    }
    else
    {
      m_button_pressed = 2;
    }

    x2 = event->xmotion.x;
    y2 = event->xmotion.y;
    Shape::Draw(w, x1, y1, x2, y2);
  }
}

///
/// \brief Draw callback (Mouse move, mouse release).
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnDraw(Widget w, XtPointer client_data, XtPointer call_data)
{
  XmDrawingAreaCallbackStruct *d = (XmDrawingAreaCallbackStruct*)call_data;

  switch (d->event->type)
  {
    case ButtonPress:
    {
      if (d->event->xbutton.button == Button1)
      {
        m_button_pressed = 1;
        x1 = d->event->xbutton.x;
        y1 = d->event->xbutton.y;
      }
      break;
    }
    case ButtonRelease:
    {
      if(d->event->xbutton.button == Button1)
      {
        x2 = d->event->xbutton.x;
        y2 = d->event->xbutton.y;
        Shape::Add(x1, y1, x2, y2);
        m_button_pressed = 0;
        Shape::SetDrawGC(w);
        Shape::SetDrawStyle(w);
        Shape::Draw(w, x1, y1, x2, y2);
        XClearArea(XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True); // Expose
      }
      break;
    }
  }
}

///
/// \brief Expose callback.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnExpose(Widget w, XtPointer client_data, XtPointer call_data)
{
  Shape::InitDrawGC(w);
  Shape::DrawAll(w);
}

///
/// \brief Clear callback. Remove all shapes from draw area.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnClear(Widget w, XtPointer client_data, XtPointer call_data)
{
  Widget wcd = (Widget) client_data;
  Shape::ClearAll();
  XClearWindow(XtDisplay(wcd), XtWindow(wcd));
}

///
/// \brief Quit callback. Show confirmation dialog.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnQuit(Widget w, XtPointer client_data, XtPointer call_data)
{
  ShowQuitDialog();
}

///
/// \brief Change currenlty selected shape to draw.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnShapeToggled(Widget w, XtPointer client_data,
XtPointer call_data)
{
  if(w == m_shape_point)
  {
    XmToggleButtonSetState(m_shape_point, True, False);
    XmToggleButtonSetState(m_shape_line, False, False);
    XmToggleButtonSetState(m_shape_rect, False, False);
    XmToggleButtonSetState(m_shape_ellipse, False, False);
    Shape::SetShape(Shape::POINT);
  }
  else if(w == m_shape_line)
  {
    XmToggleButtonSetState(m_shape_point, False, False);
    XmToggleButtonSetState(m_shape_line, True, False);
    XmToggleButtonSetState(m_shape_rect, False, False);
    XmToggleButtonSetState(m_shape_ellipse, False, False);
    Shape::SetShape(Shape::LINE);
  }
  else if(w == m_shape_rect)
  {
    XmToggleButtonSetState(m_shape_point, False, False);
    XmToggleButtonSetState(m_shape_line, False, False);
    XmToggleButtonSetState(m_shape_rect, True, False);
    XmToggleButtonSetState(m_shape_ellipse, False, False);
    Shape::SetShape(Shape::RECT);
  }
  else if(w == m_shape_ellipse)
  {
    XmToggleButtonSetState(m_shape_point, False, False);
    XmToggleButtonSetState(m_shape_line, False, False);
    XmToggleButtonSetState(m_shape_rect, False, False);
    XmToggleButtonSetState(m_shape_ellipse, True, False);
    Shape::SetShape(Shape::ELLIPSE);
  }
  else
  {
    fprintf(stderr, "Invalid shape button ID\n");
  }
}

///
/// \brief Change currently selected border type of the shape.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnBorderToggled(Widget w, XtPointer client_data,
XtPointer call_data)
{
  if(XmToggleButtonGetState(w) == True)
  {
    Shape::SetBorder(Shape::BORDER_DOTTED);
  }
  else
  {
    Shape::SetBorder(Shape::BORDER_FULL);
  }
}

///
/// \brief Change property whether ellipses and rectangles should be filled.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnFillToggled(Widget w, XtPointer client_data,
XtPointer call_data)
{
  if(XmToggleButtonGetState(w) == True)
  {
    Shape::SetFill(true);
  }
  else
  {
    Shape::SetFill(false);
  }
}

///
/// \brief Change current foreground or background color.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnColorChanged(Widget w, XtPointer client_data,
XtPointer call_data)
{
  XmComboBoxCallbackStruct *cbcs = (XmComboBoxCallbackStruct*)call_data;
  unsigned int item = (unsigned int)cbcs->item_position;
  if(w == m_fg_cb)
  {
    Colors::SetForeground(item);
  }
  else if(w == m_bg_cb)
  {
    Colors::SetBackground(item);
  }
  else
  {
    fprintf(stderr, "Invalid color type changed\n");
  }
}

///
/// \brief Change line width (or border width in case of ellipses and rectangles).
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnLineWidthChanged(Widget w, XtPointer client_data,
XtPointer call_data)
{
  XmScaleCallbackStruct *cbcs = (XmScaleCallbackStruct*)call_data;
  Shape::SetLineWidth(cbcs->value);
}

///
/// \brief Quit application.
/// \param w Widget
/// \param client_data XtPointer
/// \param call_data XtPointer
///
void MainWindow::OnQdQuit(Widget w, XtPointer client_data,
XtPointer call_data)
{
  exit(0);
}
