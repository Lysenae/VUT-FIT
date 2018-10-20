/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package spatial.shapes;

import enums.ShapeState;
import enums.ShapeType;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Point;

import models.Reservation;
import oracle.spatial.geometry.JGeometry;

/**
 * Abstract base class for shapes
 */
public abstract class ShapeBase
{
    private int id;
    protected String name;
    protected ShapeType type;
    private Point position;
    private ShapeState state;
    private boolean selected;
    private boolean isOwnable;
    public boolean isValid;

    public Reservation reservation;

    protected static final int PT_OFFSET = 3;
    public static final BasicStroke DASH = new BasicStroke(
            3, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 0, new float[]{9}, 0);

    /**
     * Class constructor.
     * @param id shape id
     * @param name shape name
     * @param x shape position - x coordinate
     * @param y shape position - y coordinate
     */
    public ShapeBase(int id, String name, int x, int y)
    {
        this.id       = id;
        this.type     = ShapeType.NONE;
        this.position = new Point(x, y);
        this.state    = ShapeState.NEW;
        this.name     = name;
        this.selected = false;

        this.reservation = new Reservation();
        this.isOwnable = false;
    }

    /**
     * Gets shape id.
     * @return shape id
     */
    public int getId()
    {
        return this.id;
    }

    /**
     * Checks if shape is resizeable.
     * @return true if shape is resizeable
     */
    public boolean isResizeable()
    {
        return false;
    }

    /**
     * Checks if type is point or line.
     * @return true of shape is point or line
     */
    public boolean isMultipoint()
    {
        return this.type == ShapeType.POINT || this.type == ShapeType.LINE;
    }

    /**
     * Sets shape position.
     * @param x x coordinate
     * @param y y coordinate
     */
    protected void setPosition(int x, int y)
    {
        this.position.x = x;
        this.position.y = y;
    }

    /**
     * Sets shape position.
     * @param p position
     */
    protected void setPosition(Point p)
    {
        this.position = p;
    }

    /**
     * Gets shape type.
     * @return shape type
     */
    public ShapeType getType()
    {
        return this.type;
    }

    /**
     * Gets shape position.
     * @return shape position
     */
    public Point getPosition()
    {
        return this.position;
    }

    /**
     * Gets x position coordinate.
     * @return x
     */
    public int getX()
    {
        return this.position.x;
    }

    /**
     * Gets y position coordinate.
     * @return y
     */
    public int getY()
    {
        return this.position.y;
    }
    public ShapeState getState()
    {
        return this.state;
    }

    /**
     * Sets shape state.
     * @param state state
     */
    public void setState(ShapeState state)
    {
        this.state = state;
    }

    /**
     * Gets shape name.
     * @return name
     */
    public String getName()
    {
        return this.name;
    }
    
    /**
     * Sets shape name.
     * @param s new name
     */
    public void setName(String s)
    {
        this.name = s;
    }
    
    /**
     * Checks if shape is selected.
     * @return true if is selected
     */
    public boolean isSelected()
    {
        return this.selected;
    }

    /**
     * Checks if shape is ownable.
     * @return true if is ownable
     */
    public boolean isOwnable()
    {
        return this.isOwnable;
    }
    
    /**
     * Sets ownalbe flag to true.
     */
    public void setOwnable()
    {
        this.isOwnable=true;
    }

    /**
     * Sets selected flag to given value.
     * @param s value
     */
    public void setSelected(boolean s)
    {
        this.selected = s;
    }
    
    /**
     * Sets reservation.
     * @param reservation reservation
     */
    public void setReservation(Reservation reservation)
    {
        this.reservation = reservation;
    }

    /**
     * Gets color for given state. Returns different colors for fill and outline.
     * @param outline specifies if color is for fill or outline
     * @return color
     */
    protected Color getColorForState(boolean outline)
    {
        Color c;
        if(this.isSelected())
        {
            c = outline ? Color.GREEN : Color.YELLOW;
        }
        else
        {
            switch(this.getState())
            {
                case NEW:      c = outline ? Color.RED       : Color.PINK;   break;
                case MODIFIED: c = outline ? Color.MAGENTA   : Color.ORANGE; break;
                case DELETED:  c = outline ? Color.DARK_GRAY : Color.GRAY;   break;
                case OK:       c = outline ? Color.BLUE      : Color.CYAN;   break;
                case MODIFIEDINFO: c = outline ? Color.DARK_GRAY : Color.LIGHT_GRAY; break;
                default:       c = Color.WHITE; break; // Something wrong!
            }
        }
        return c;
    }

    /**
     * Checks if shape is new.
     * @return true if is new
     */
    public boolean isNew()
    {
        return this.state == ShapeState.NEW;
    }

    /**
     * Checks if shape is updated.
     * @return true if is updated
     */
    public boolean isUpdated()
    {
        return this.state == ShapeState.MODIFIED;
    }

    /**
     * Checks if shape is deleted.
     * @return true if is deleted
     */
    public boolean isDeleted()
    {
        return this.state == ShapeState.DELETED;
    }

    /**
     * Checks if shape has modified info.
     * @return true if has modified info
     */
    public boolean isInfoUpdated()
    {
        return this.state == ShapeState.MODIFIEDINFO;
    }

    /**
     * Checks if shape is in consistent state.
     * @return true if is consisten
     */
    public boolean isConsistent()
    {
        return this.state == ShapeState.OK;
    }

    /**
     * Sets state to updated.
     */
    protected void setUpdated()
    {
        if(this.state == ShapeState.OK)
            this.state = ShapeState.MODIFIED;
    }
    
    /**
     * Sets state to updated info.
     */
    public void setInfoUpdated()
    {
        if(this.state == ShapeState.OK)
        {
            this.state = ShapeState.MODIFIEDINFO;
        }
    }

    public abstract boolean contains(Point p);
    public abstract void moveTo(Point p);
    public abstract void draw(Graphics2D g, boolean sketch);
    public abstract JGeometry toJGeometry();
}
