
package gui.panels;
import enums.LmMode;
import enums.ResizeSide;
import enums.ShapeState;
import spatial.shapes.*;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import javax.swing.*;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import spatial.shapes.ShapeRectangle;
import enums.ShapeType;
import helpers.ShapeTypeSelector;
import java.awt.Graphics2D;
import models.LayoutManagerModel;
import models.Reservation;
/**
 * @author xskota07
 * @author xklima22
 */
public class LayoutManagerMapPanel
        extends JPanel
        implements MouseListener, MouseMotionListener
{
    public LayoutManagerPanel parent;

    private boolean drawing;
    private boolean moving;
    private boolean resizing;

    private ShapeLine      currentLine;
    private ShapeRectangle currentRect;
    private ShapeCircle    currentCircle;
    private ShapeLinePg    currentPg;

    public Reservation reservation;

    private Point sp;
    private Point ep;

    private LmMode    selectedMode;
    private String    newShapeName;

    private ShapeBase  currentSelection;
    private ShapeBase lastSelection;
    private Point      origin;
    private Dimension  moveOffset;
    private ResizeSide rSide;

    /**
     * Class constructor.
     * @param parent parent panel used for interaction.
     */
    public LayoutManagerMapPanel(LayoutManagerPanel parent)
    {
        this.parent        = parent;
        this.drawing       = false;
        this.moving        = false;
        this.resizing      = false;
        this.currentLine   = null;
        this.currentRect   = null;
        this.currentCircle = null;
        this.currentPg     = null;
        this.sp            = null;
        this.ep            = null;
        this.selectedMode  = LmMode.SELECT;
        this.newShapeName  = "";
        this.origin        = null;
        this.moveOffset    = null;
        this.rSide         = ResizeSide.NONE;
        this.reservation = new Reservation();
        this.configure();
        this.loadAll();
    }

    /**
     *  Set panel properties.
     */
    private void configure()
    {
        LayoutManagerModel.init();
        this.addMouseListener(this);
        this.addMouseMotionListener(this);
        this.setBackground(Color.BLACK);
        this.setVisible(true);
        this.setMinimumSize(new Dimension(300,200));
    }

    /**
     * Get size of workspace.
     * @return size of workspace
     */
    @Override
    public Dimension getPreferredSize()
    {
        return new Dimension(1000, 1000);
    }

    /**
     * Paint component.
     * @param gr graphic context used to draw the component
     */
    @Override
    public void paintComponent(Graphics gr)
    {
        Graphics2D g = (Graphics2D) gr;
        g.clearRect(0, 0, this.getWidth(), this.getHeight());

        // Draw borders
        g.setColor(Color.BLACK);
        g.drawRect(0, 0, this.getPreferredSize().width,
                this.getPreferredSize().height);

        // Draw all saved shapes
        this.drawShapes(g);

        // Draw incomplete object
        if(this.currentLine != null)
            this.currentLine.draw(g, true);
        else if(this.currentRect != null)
            this.currentRect.draw(g, true);
        else if(this.currentCircle != null)
            this.currentCircle.draw(g, true);
        else if(this.currentPg != null)
            this.currentPg.draw(g, true);
    }

    /**
     * Adjusts points coordinates to fit in panels boundaries.
     * @param p input point
     * @return adjusted point
     */
    private Point adjustPoint(Point p)
    {
        if(p.x < 0)
            p.x = 0;
        else if(p.x > this.getPreferredSize().width)
            p.x = this.getPreferredSize().width;

        if(p.y < 0)
            p.y = 0;
        else if(p.y > this.getPreferredSize().height)
            p.y = this.getPreferredSize().height;

        return p;
    }

    /**
     * Sets currently drawn shape.
     */
    private void setCurrentShape()
    {
        switch(ShapeTypeSelector.getType())
        {
            case POINT:
                LayoutManagerModel.addPoint(new ShapePoint(-1, this.sp.x, this.sp.y));
                this.drawing = false;
                break;
            case LINE:
                if(this.currentLine == null)
                {
                    this.currentLine = new ShapeLine(-1, this.sp.x, this.sp.y);
                }
                break;
            case RECTANGLE:
                this.currentRect = new ShapeRectangle(-1, this.newShapeName,
                        this.sp.x, this.sp.y);
                break;
            case CIRCLE:
                this.currentCircle = new ShapeCircle(-1, this.newShapeName,
                        this.sp.x, this.sp.y);
                break;
            case POLYGON:
                if(this.currentPg == null)
                {
                    this.currentPg = new ShapeLinePg(-1, this.newShapeName,
                            this.sp.x, this.sp.y);
                }
                break;
            default:
                break;
        }
    }

    /**
     * Draw all stored shapes.
     * @param g graphic context
     */
    private void drawShapes(Graphics2D g)
    {
        LayoutManagerModel.getComposites().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
        LayoutManagerModel.getPolygons().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
        LayoutManagerModel.getRects().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
        LayoutManagerModel.getCircles().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
        LayoutManagerModel.getLines().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
        LayoutManagerModel.getPoints().forEach((s) ->
        { if(!s.isDeleted()) s.draw(g, false); } );
    }

    /**
     * Finish drawing lines or polygons.
     */
    private void endDrawing()
    {
        if(this.drawing)
        {
            if(ShapeTypeSelector.isLine() && this.currentLine != null)
            {
                if(this.currentLine.isDrawable())
                    LayoutManagerModel.addLine(this.currentLine);
                this.currentLine = null;
            }
            else if(ShapeTypeSelector.isPolygon() && this.currentPg != null)
            {
                if(this.currentPg.isDrawable())
                    LayoutManagerModel.addPolygon(this.currentPg);
                this.currentPg = null;
            }
            this.drawing     = false;
            repaint();
        }
    }

    /**
     * Sets shape type that may be drawn next. Setting a shape type finishes drawing
     * of unfinished lines or polygons.
     * @param s shape type
     */
    public void setSelectedShapeType(ShapeType s)
    {
        this.endDrawing();
        this.setSelectedShape(null);
        ShapeTypeSelector.setType(s);
    }

    /**
     * Sets panel's interaction mode. Setting a mode finishes drawing
     * of unfinished lines or polygons.
     * @param mode panel mode
     */
    public void setSelectedMode(LmMode mode)
    {
        this.endDrawing();
        this.setSelectedShape(null);
        this.selectedMode = mode;
    }

    /**
     * Sets name of next shape.
     * @param s shape name
     */
    public void setNewShapeName(String s)
    {
        this.newShapeName = s;
    }

    /**
     * Checks if clicked position is within some shape. Shapes are searched from
     * the simpliest to most complex in order in which they were loaded or stored.
     * @param p clicked point
     */
    private void findClickedShape(Point p)
    {
        for(ShapePoint s : LayoutManagerModel.getPoints())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                return;
            }
        }

        for(ShapeLine s : LayoutManagerModel.getLines())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                return;
            }
        }

        for(ShapeCircle s : LayoutManagerModel.getCircles())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                this.parent.reservationDetailPanel.updateReservationDetailPanel(s.reservation);
                return;
            }
        }

        for(ShapeRectangle s : LayoutManagerModel.getRects())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                this.parent.reservationDetailPanel.updateReservationDetailPanel(s.reservation);
                return;
            }
        }

        for(ShapeLinePg s : LayoutManagerModel.getPolygons())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                this.parent.reservationDetailPanel.updateReservationDetailPanel(s.reservation);
                return;
            }
        }

        for(ShapeComposite s : LayoutManagerModel.getComposites())
        {
            if(s.contains(p))
            {
                this.setSelectedShape(s);
                this.parent.reservationDetailPanel.updateReservationDetailPanel(s.reservation);
                return;
            }
        }

        this.setSelectedShape(null);
        repaint();
    }

    /**
     * Sets currently selected variable to clicked shape.
     * @param s clicked shape
     */
    private void setSelectedShape(ShapeBase s)
    {
        if(this.currentSelection != null)
        {
            this.lastSelection = this.currentSelection;
            if(this.currentSelection.isResizeable())
            {
                ShapeResizeable rs = (ShapeResizeable) this.currentSelection;
                rs.setResized(false);
            }
            this.currentSelection.setSelected(false);
            this.currentSelection = null;
            this.parent.setEnabledDeleteButton(false);

        }

        if(s != null)
        {
            this.currentSelection = s;
            this.lastSelection = s;
            s.setSelected(true);
            this.parent.setEnabledDeleteButton(true);

        }
        this.parent.updateDetails();
        repaint();
    }

    /**
     * Calculates offset between clicked point and shape's position.
     */
    private void calculateOriginToPositionOffset()
    {
        Point p         = this.currentSelection.getPosition();
        Point o         = this.origin;
        this.moveOffset = new Dimension(o.x - p.x, o.y - p.y);
    }

    /**
     * Gets currently selected shape.
     * @return currently selected shape
     */
    public ShapeBase getCurrentSelection()
    {
        return this.currentSelection;
    }
    
    /**
     * Gets most recently selected shape.
     * @return most recently selected shape
     */
    public ShapeBase getLastSelection()
    {
        return this.lastSelection;
    }

    /**
     * Save all drawn shapes to DB.
     */
    public void saveAll()
    {
        LayoutManagerModel.saveAll();
        LayoutManagerModel.joinOverlappedShapes();
        this.loadAll();
    }

    /**
     * Load all relevant shapes from DB.
     */
    public void loadAll()
    {
        int resCount = LayoutManagerModel.loadAllReservations(this.parent.calendarDate.getDate());
        this.parent.reservationsLabel.setText(String.format("Reservations: %d", resCount));
        int staCount = LayoutManagerModel.loadAllShapes();
        this.parent.stagesLabel.setText((String.format(" Stages: %d", staCount)));
        repaint();
    }

    /**
     * Deletes selected shape.
     */
    public void deleteSelected()
    {
        LayoutManagerModel.removeShapeFromList(this.currentSelection);
        setSelectedShape(null);
        repaint();
    }
    
    // Mouse events responses - INSERT

    /**
     * Handler for mouse release event in insert mode. End drawing.
     * @param e event data
     */
    private void mouseReleasedInsert(MouseEvent e)
    {
        if(this.drawing)
        {
            if(ShapeTypeSelector.isLine() && this.currentLine != null)
            {
                this.currentLine.addPoint(adjustPoint(e.getPoint()));
            }
            else if(ShapeTypeSelector.isPolygon() && this.currentPg != null)
            {
                this.currentPg.setReservation(this.reservation.dup());
                this.currentPg.addPoint(adjustPoint(e.getPoint()));

            }
            else if(ShapeTypeSelector.isRect() || ShapeTypeSelector.isCircle())
            {
                if(ShapeTypeSelector.isRect())
                {
                    this.currentRect.setReservation(this.reservation.dup());
                    LayoutManagerModel.addRect(this.currentRect);
                    this.currentRect = null;
                }
                else
                {
                    this.currentCircle.setReservation(this.reservation.dup());
                    LayoutManagerModel.addCircle(this.currentCircle);
                    this.currentCircle = null;
                }
                this.drawing      = false;
                this.sp           = null;
                this.ep           = null;
            }
        }
        else
        {
            if((ShapeTypeSelector.isMultipoint()) &&
                    e.getButton() == MouseEvent.BUTTON1)
            {
                this.drawing = true;
                this.sp      = this.adjustPoint(e.getPoint());
                this.setCurrentShape();
            }
        }
        this.repaint();
    }

    /**
     * Handler for mouse press event in insert mode. Start drawing shapes.
     * @param e event data
     */
    private void mousePressedInsert(MouseEvent e)
    {
        if(this.drawing)
        {
            if(e.getButton() != MouseEvent.BUTTON1)
            {
                if(ShapeTypeSelector.isLine())
                {
                    if(this.currentLine.isDrawable())
                        LayoutManagerModel.addLine(this.currentLine);
                    this.currentLine = null;
                }
                else if(ShapeTypeSelector.isPolygon())
                {
                    if(this.currentPg.isDrawable())
                        LayoutManagerModel.addPolygon(this.currentPg);
                    this.currentPg = null;
                }
                this.drawing = false;
            }
            this.repaint();
        }
        else
        {
            if(ShapeTypeSelector.isRect() || ShapeTypeSelector.isCircle())
            {
                this.drawing = true;
                this.sp      = this.adjustPoint(e.getPoint());
                this.setCurrentShape();
                this.repaint();
            }
        }
    }

    /**
     * Handler for mouse drag event in insert mode. Draw rectangle or circle.
     * Does nothing in other cases.
     * @param e event data
     */
    private void mouseDraggedInsert(MouseEvent e)
    {
        if(drawing)
        {
            if(ShapeTypeSelector.isRect())
            {
                this.ep = this.adjustPoint(e.getPoint());
                this.currentRect.setGeometry(this.sp, this.ep, false);
            }
            else if(ShapeTypeSelector.isCircle())
            {
                this.ep = this.adjustPoint(e.getPoint());
                this.currentCircle.setGeometry(this.sp, this.ep, false);
            }
            this.repaint();
        }
    }

    // Mouse events responses - MOVE

    /**
     * Handler for mouse press event in move mode. Start moving the shape.
     * @param e event data
     */
    private void mousePressedMove(MouseEvent e)
    {
        this.findClickedShape(e.getPoint());
        if(this.currentSelection != null)
        {
            this.origin = e.getPoint();
            this.moving     = true;
            this.calculateOriginToPositionOffset();
        }
    }

    /**
     * Handler for mouse relese event in move mode. End moving the shape.
     * @param e event data
     */
    private void mouseReleasedMove(MouseEvent e)
    {
        if(this.currentSelection != null)
        {
            this.origin = null;
            this.moving     = false;
            this.moveOffset = null;
        }
    }

    /**
     * Handler for mouse drag event in move mode. Move the shape.
     * @param e event data
     */
    private void mouseDraggedMove(MouseEvent e)
    {
        if(this.currentSelection != null && this.moving)
        {
            Dimension off = this.moveOffset;
            Point endp = e.getPoint();
            Point newPos = new Point(endp.x-off.width, endp.y-off.height);
            currentSelection.moveTo(this.adjustPoint(newPos));
            repaint();
        }
    }

    // Mouse events responses - RESIZE

    /**
     * Handler for mouse press event in resize mode. Start resizing rectangle or
     * circle. Doeas nothing in other cases.
     * @param e event data
     */
    private void mousePressedResize(MouseEvent e)
    {
        this.findClickedShape(e.getPoint());
        if(this.currentSelection != null)
        {
            if(!this.currentSelection.isResizeable())
            {
                this.setSelectedShape(null);
            }
            else
            {
                ShapeResizeable s = (ShapeResizeable) this.currentSelection;
                this.rSide  = s.getResizeSide(e.getPoint());
                this.origin = s.getPosition();
            }
        }
    }

    /**
     * Handler for mouse release event in resize mode. End resizing.
     * @param e event data
     */
    private void mouseReleasedResize(MouseEvent e)
    {

        if(this.currentSelection != null)
        {
            ShapeResizeable s = (ShapeResizeable) this.currentSelection;
            s.setResized(true);
            repaint();
        }

        if(this.rSide != ResizeSide.NONE)
        {
            if(this.currentSelection != null)
            {
                ShapeResizeable s = (ShapeResizeable) this.currentSelection;
                s.setResized(false);
                this.setSelectedShape(null);
            }
            this.rSide  = ResizeSide.NONE;
            this.origin = null;
        }
    }

    /**
     * Handler for mouse drag event in resize mode. Resize.
     * @param e event data
     */
    private void mouseDraggedResize(MouseEvent e)
    {
        if(this.rSide != ResizeSide.NONE)
        {
            ShapeResizeable s = (ShapeResizeable) this.currentSelection;
            s.resize(this.rSide, e.getPoint(), this.origin);
            repaint();
        }
    }

    // Mouse Events

    /**
     * Handler for mouse press event.
     * @param e event data
     */
    @Override
    public void mousePressed(MouseEvent e)
    {
        switch(this.selectedMode)
        {
            case SELECT:
                this.findClickedShape(e.getPoint());
            case INSERT:
                this.mousePressedInsert(e);
                break;
            case MOVE:
                this.mousePressedMove(e);
                break;
            case RESIZE:
                this.mousePressedResize(e);
                break;
            default:
                break;
        }
    }

    /**
     * Handler for mouse release event.
     * @param e event data
     */
    @Override
    public void mouseReleased(MouseEvent e)
    {
        switch(this.selectedMode)
        {
            case INSERT:
                this.mouseReleasedInsert(e);
                break;
            case MOVE:
                this.mouseReleasedMove(e);
                break;
            case RESIZE:
                this.mouseReleasedResize(e);
                break;
            default:
                break;
        }
    }

    /**
     * Handler for mouse drag event.
     * @param e event data
     */
    @Override
    public void mouseDragged(MouseEvent e)
    {
        switch(this.selectedMode)
        {
            case INSERT:
                this.mouseDraggedInsert(e);
                break;
            case MOVE:
                this.mouseDraggedMove(e);
                break;
            case RESIZE:
                this.mouseDraggedResize(e);
                break;
            default:
                break;
        }
    }

    // Unimportant mouse events

    @Override
    public void mouseMoved(MouseEvent e) {}

    @Override
    public void mouseClicked(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}
}
