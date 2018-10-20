// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ResizeSide;
import enums.ShapeState;
import enums.ShapeType;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Shape;
import oracle.spatial.geometry.JGeometry;

/**
 * Circle
 */
public class ShapeCircle extends ShapeResizeable
{
    private int   radius;
    private int   width;
    private int   height;
    private Point center;
    
    /**
     * Class constructor
     * @param id shape id
     * @param name shape name
     * @param x position x
     * @param y position y
     */
    public ShapeCircle(int id, String name, int x, int y)
    {
        super(id, name, x, y);
        this.width  = 0;
        this.height = 0;
        this.center = new Point(x, y);
        this.type   = ShapeType.CIRCLE;
        this.setOwnable();
    }
    
    /**
     * Gets radius.
     * @return radius
     */
    public int getRadius()
    {
        return this.radius;
    }
    
    /**
     * Gets width of bounding rectangle.
     * @return width of bounding rectangle
     */
    public int getWidth()
    {
        return this.width;
    }
    
    /**
     * Gets height of bounding rectangle.
     * @return height of bounding rectangle
     */
    public int getHeight()
    {
        return this.height;
    }
    
    /**
     * Gets coordinates of center.
     * @return center coordinates
     */
    public Point center()
    {
        return this.center;
    }
    
    /**
     * Sets geometry according to given points. Circle is drawn in square defined
     * by points.
     * @param p1 First point
     * @param p2 Second point
     * @param enlarge determines if width and height are chosen as minimal or
     * maximal value of both 
     */
    @Override
    public void setGeometry(Point p1, Point p2, boolean enlarge)
    {
        int x = p1.x <= p2.x ? p1.x : p2.x;
        int y = p1.y <= p2.y ? p1.y : p2.y;
        int w = p1.x <= p2.x ? p2.x - p1.x : p1.x - p2.x;
        int h = p1.y <= p2.y ? p2.y - p1.y : p1.y - p2.y;

        w = enlarge ? Integer.max(w, h) : Integer.min(w, h);
        h = enlarge ? Integer.max(w, h) : Integer.min(w, h);
        this.setPosition(x, y);

        this.width    = w;
        this.height   = h;
        this.calculateProperties();
    }
    
    /**
     * Sets geometry according to given rectangle. Circle is drawn in square
     * defined by rectangle.
     * @param r Rectangle
     * @param enlarge determines if width and height are chosen as minimal or
     * maximal value of both 
     */
    @Override
    public void setGeometry(Rectangle r, boolean enlarge)
    {
        Point p1 = new Point((int)r.getX(), (int)r.getY());
        Point p2 = new Point((int)(r.getX()+r.getWidth()),
            (int)(r.getY()+r.getHeight()));
        this.setGeometry(p1, p2, enlarge);
    }

    /**
     * Checks if circle contains point. In resize mode checks if point is
     * contained in bounding square.
     * @param p point
     * @return true if circle contains point
     */
    @Override
    public boolean contains(Point p)
    {
        if(!this.isResized())
        {
            if(Math.pow((p.x - this.center.x), 2) +
            Math.pow((p.y - this.center.y), 2) < Math.pow(this.radius, 2))
                return true;
        }
        else
        {
            Point pt = this.getPosition();
            if(p.x >= pt.x && p.x <= pt.x + this.width &&
            p.y >= pt.y && p.y <= pt.y + this.height)
                return true;
        }
        return false;
    }

    /**
     * Moves shape to given position.
     * @param p new position
     */
    @Override
    public void moveTo(Point p)
    {
        this.setPosition(p);
        this.calculateProperties();
        this.setUpdated();
    }
    
    /**
     * Calculate radius and center coordinates.
     */
    private void calculateProperties()
    {
        this.radius   = this.getWidth()/2;
        this.center.x = this.getX() + this.getWidth()/2;
        this.center.y = this.getY() + this.getHeight()/2;
    }

    /**
     * Draw circle
     * @param g graphic context
     * @param sketch determines if circle is being sketched
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        if(!sketch)
        {
            g.setColor(this.getColorForState(false));
            g.fillOval(this.getX(), this.getY(), this.getWidth(),
                this.getHeight());
            g.setStroke(new BasicStroke(3));
        }
        else
        {
            g.setStroke(ShapeBase.DASH);
        }
        g.setColor(this.getColorForState(true));
        g.drawOval(this.getX(), this.getY(), this.getWidth(), this.getHeight());
        if(this.toResize)
        {
            g.setColor(Color.BLACK);
            g.drawRect(this.getX(), this.getY(), this.getWidth(), this.getHeight());
        }
    }
    
    /**
     * Resize circle.
     * @param side which size is resized
     * @param p position of cursor
     * @param origin original shape position
     */
    @Override
    public void resize(ResizeSide side, Point p, Point origin)
    {
        Point p1, p2;
        boolean el;
        switch(side)
        {
            case TOP:
                p1 = new Point(origin.x, p.y);
                p2 = new Point(origin.x + this.getWidth(), origin.y + this.getHeight());
                el = p.y < this.getY();
                break;
            case BOTTOM:
                p1 = new Point(origin.x, origin.y);
                p2 = new Point(origin.x + this.getWidth(), p.y);
                el = p.y > this.getY()+this.getHeight();
                break;
            case LEFT:
                p1 = new Point(p.x, origin.y);
                p2 = new Point(origin.x + this.getWidth(), origin.y + this.getHeight());
                el = p.x < this.getX();
                break;
            case RIGHT:
                p1 = new Point(origin.x, origin.y);
                p2 = new Point(p.x, this.getY()+this.getHeight());
                el = p.x > this.getX()+this.getWidth();
                break;
            default:
                p1 = new Point(this.getX(), this.getY());
                p2 = new Point(this.getX()+this.getWidth(), this.getY()+this.getHeight());
                el = false;
                break;
        }
        this.setGeometry(p1, p2, el);
        this.setUpdated();
    }

    /**
     * Creates circle from JGeometry.
     * @param id shape id
     * @param name shape name
     * @param geo JGeometry
     * @return circle
     */
    public static ShapeCircle fromJGeometry(int id, String name, JGeometry geo)
    {
        Shape s = geo.createShape();
        Rectangle r = s.getBounds();
        ShapeCircle c = new ShapeCircle(id, name, 0, 0);
        c.setGeometry(r, false);
        c.setState(ShapeState.OK);
        return c;
    }

    /**
     * Converts circle to JGeometry.
     * @return JGeometry
     */
    @Override
    public JGeometry toJGeometry()
    {
        return JGeometry.createCircle((double) this.center.x,
            (double) this.center.y, (double) this.radius, 0);
    }
}
