# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


# Interclient communication facilities

+USE "x5/lib", "X11/X.h.ts" ;
+USE "x5/lib", "X11/Xlib.h.ts", "types" ;


# Setting and Reading the WM_HINTS Property (14.1.6)

+DEFINE InputHint		%% (1L<<0) %% ;
+DEFINE StateHint		%% (1L<<1) %% ;
+DEFINE IconPixmapHint		%% (1L<<2) %% ;
+DEFINE IconWindowHint		%% (1L<<3) %% ;
+DEFINE IconPositionHint	%% (1L<<4) %% ;
+DEFINE IconMaskHint		%% (1L<<5) %% ;
+DEFINE WindowGroupHint		%% (1L<<6) %% ;
+DEFINE AllHints		%% (InputHint|StateHint|IconPixmapHint|IconWindowHint|IconPosiionHint|IconMaskHint|WindowGroupHint) %% ;

+FIELD ( struct ) XWMHints {
    long flags ;
    Bool input ;
    int initial_state ;
    Pixmap icon_pixmap ;
    Window icon_window ;
    int icon_x, icon_y ;
    Pixmap icon_mask ;
    XID window_group ;
} ;

+DEFINE WithdrawnState		%% 0 %% ;
+DEFINE NormalState		%% 1 %% ;
+DEFINE IconicState		%% 3 %% ;


# Setting and Reading the WM_NORMAL_HINTS Property (14.1.7)

+DEFINE USPosition		%% (1L<<0) %% ;
+DEFINE USSize			%% (1L<<1) %% ;
+DEFINE PPosition		%% (1L<<2) %% ;
+DEFINE PSize			%% (1L<<3) %% ;
+DEFINE PMinSize		%% (1L<<4) %% ;
+DEFINE PMaxSize		%% (1L<<5) %% ;
+DEFINE PResizeInc		%% (1L<<6) %% ;
+DEFINE PAspect			%% (1L<<7) %% ;
+DEFINE PBaseSize		%% (1L<<8) %% ;
+DEFINE PWinGravity		%% (1L<<9) %% ;
+DEFINE PAllHints		%% (PPosition|PSize|PMinSize|PMaxSize|PResizeInc|PAspect) %% ;

+FIELD struct ~XSizeHintsAspect := {
    int x ;
    int y ;
} ;

+FIELD ( struct ) XSizeHints := {
    long flags ;
    int x, y ;
    int width, height ;
    int min_width, min_height ;
    int max_width, max_height ;
    int width_inc, height_inc ;
    struct ~XSizeHintsAspect min_aspect, max_aspect ;
    int base_width, base_height ;
    int win_gravity ;
} ;


# Setting and Reading the WM_CLASS Property (14.1.8)

+FIELD ( struct ) XClassHint := {
    char *res_name ;
    char *res_class ;
} ;


# Setting and Reading the WM_ICON_SIZE Property (14.1.12)

+FIELD ( struct ) XIconSize := {
    int min_width, min_height ;
    int max_width, max_height ;
    int width_inc, height_inc ;
} ;


# Parsing the Window Geometry (16.4)

+CONST int XValue, YValue, WidthValue, HeightValue, XNegative, YNegative ;


# Manipulating Regions (16.5)

+TYPE ~RegionRec ;
+TYPEDEF ~RegionRec *Region ;	# RCA : it must be a pointer


# Determining a Visual Type (16.7)

+DEFINE VisualNoMask		%% 0x0 %% ;
+DEFINE VisualIDMask		%% 0x1 %% ;
+DEFINE VisualScreenMask	%% 0x2 %% ;
+DEFINE VisualDepthMask		%% 0x4 %% ;
+DEFINE VisualClassMask		%% 0x8 %% ;
+DEFINE VisualRedMaskMask	%% 0x10 %% ;
+DEFINE VisualGreenMaskMask	%% 0x20 %% ;
+DEFINE VisualBlueMaskMask	%% 0x40 %% ;
+DEFINE VisualColormapSizeMask	%% 0x80 %% ;
+DEFINE VisualBitsPerRGBMask	%% 0x100 %% ;
+DEFINE VisualAllMask		%% 0x1FF %% ;

+FIELD ( struct ) XVisualInfo := {
    Visual *visual ;
    VisualID visualid ;
    int screen ;
    unsigned int depth ;
    int class ;
    unsigned long red_mask ;
    unsigned long green_mask ;
    unsigned long blue_mask ;
    int colormap_size ;
    int bits_per_rgb ;
} ;


# Manipulating Images (16.8)

+FUNC unsigned long XGetPixel ( XImage *, int, int ) ;
+FUNC void XPutPixel ( XImage *, int, int, unsigned long ) ;
+FUNC XImage *XSubImage ( XImage *, int, int, unsigned int, unsigned int ) ;
+FUNC void XAddPixel ( XImage *, long ) ;


# Using the Context Manager (16.10)

+CONST int XCNOMEM, XCNOENT ;
