// File:        mainwindow.h
// Author:      Daniel Klimaj (xklima22)
// Description: Main application window and widgets.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Protocols.h>
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/ComboBox.h>
#include <Xm/Scale.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>

#include <cstdio>
#include <cstdlib>

#include "colors.h"
#include "shape.h"

class MainWindow
{
public:
  MainWindow(int argc, char **argv);
  ~MainWindow();
  int run();

private:
  static int m_button_pressed;
  static int x1, y1, x2, y2;

  Widget m_top_level;
  static Widget m_main_win;
  Widget m_frame;
  Widget m_draw_area;
  Widget m_tools;
  Widget m_tools_shapes;
  Widget m_tools_border;
  Widget m_tools_style;

  // Menu
  Widget   m_menu_bar;
  Widget   m_menu;
  Widget   m_menu_w;
  Widget   m_menu_clear_btn;
  Widget   m_menu_exit_btn;
  Widget   m_menu_cascade;
  XmString m_menu_label;
  XmString m_menu_accel;

  // Tools
  static Widget m_shape_point;
  static Widget m_shape_line;
  static Widget m_shape_rect;
  static Widget m_shape_ellipse;
  static Widget m_border_btn;
  static Widget m_fill_btn;
  static Widget m_line_width_sc;
  static Widget m_fg_cb;
  static Widget m_bg_cb;
  Widget m_fg_label;
  Widget m_bg_label;

  XtAppContext m_app_context;

  static Widget m_quit_dialog;
  static bool m_quit_dlg_exists;

  void CreateMenu();
  void CreateTools();
  static void ShowQuitDialog();

  static void HandleDraw(Widget w, XtPointer client_data, XEvent *event,
    Boolean *cont);
  static void OnQuit(Widget w, XtPointer client_data, XtPointer call_data);
  static void OnClear(Widget w, XtPointer client_data, XtPointer call_data);
  static void OnExpose(Widget w, XtPointer client_data, XtPointer call_data);
  static void OnDraw(Widget w, XtPointer client_data, XtPointer call_data);
  static void OnShapeToggled(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnBorderToggled(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnFillToggled(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnColorChanged(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnLineWidthChanged(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnQdQuit(Widget w, XtPointer client_data,
    XtPointer call_data);
  static void OnQdCancel(Widget w, XtPointer client_data,
    XtPointer call_data);
};

#endif // MAINWINDOW_H
