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
 * Rectangle
 */
public class ShapeRectangle extends ShapeResizeable
{
    public ShapeRectangle(int id, String name, int x, int y)
    {
        super(id, name, x, y);
        this.width  = 0;
        this.height = 0;
        this.type   = ShapeType.RECTANGLE;
        this.setOwnable();
    }
    
    /**
     * Sets geometry accordnig to given points.
     * @param p1 first point
     * @param p2 second point
     * @param enlarge not used
     */
    @Override
    public void setGeometry(Point p1, Point p2, boolean enlarge)
    {
        int x = p1.x <= p2.x ? p1.x : p2.x;
        int y = p1.y <= p2.y ? p1.y : p2.y;
        int w = p1.x <= p2.x ? p2.x - p1.x : p1.x - p2.x;
        int h = p1.y <= p2.y ? p2.y - p1.y : p1.y - p2.y;
        this.setPosition(x, y);
        this.width  = w;
        this.height = h;
    }
    
    /**
     * Sets geometry according to given rectangle.
     * @param r rectangle
     * @param enlarge not used
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
     * Checks if rectangle contains point.
     * @param p point
     * @return true if contains point
     */
    @Override
    public boolean contains(Point p)
    {
        Point pt = this.getPosition();
        if(p.x >= pt.x && p.x <= pt.x + this.width &&
        p.y >= pt.y && p.y <= pt.y + this.height)
        {
            return true;
        }
        return false;
    }

    /**
     * Moves rectangle to new location.
     * @param p point
     */
    @Override
    public void moveTo(Point p)
    {
        this.setPosition(p);
        this.setUpdated();
    }

    /**
     * Draw rectangle.
     * @param g graphic context
     * @param sketch determines if rectangle is being sketched
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        if(!sketch)
        {
            g.setColor(this.getColorForState(false));
            g.fillRect(this.getX(), this.getY(), this.getWidth(),
                this.getHeight());
            g.setStroke(new BasicStroke(3));
        }
        else
        {
            g.setStroke(ShapeBase.DASH);
        }
        if(this.toResize)
            g.setColor(Color.BLACK);
        else
            g.setColor(this.getColorForState(true));
        g.drawRect(this.getX(), this.getY(), this.getWidth(), this.getHeight());
    }
    
    /**
     * Resize.
     * @param side resized side
     * @param p cursor position
     * @param origin original position
     */
    @Override
    public void resize(ResizeSide side, Point p, Point origin)
    {
        switch(side)
        {
            case TOP:
                this.setHeight(this.getHeight() - (p.y - origin.y));
                this.setPosition(this.getX(), p.y);
                break;
            case BOTTOM:
                this.setHeight(this.getHeight() + (p.y - (this.getY() +
                    this.getHeight())));
                break;
            case LEFT:
                this.setWidth(this.getWidth() - (p.x - origin.x));
                this.setPosition(p.x, this.getY());
                break;
            case RIGHT:
                this.setWidth(this.getWidth() + (p.x - (this.getX() +
                    this.getWidth())));
                break;
            default:
                break;
        }
        this.setUpdated();
    }
    
    /**
     * GEts array of points.
     * @return array of points
     */
    private double[] pts()
    {
        double[] p = new double[8];
        p[0] = this.getX();
        p[1] = this.getY();
        p[2] = this.getX()+this.getWidth();
        p[3] = this.getY();
        p[4] = this.getX()+this.getWidth();
        p[5] = this.getY()+this.getHeight();
        p[6] = this.getX();
        p[7] = this.getY()+this.getHeight();
        return p;
    }

    /**
     * Creates rectangle from JGeometry.
     * @param id shape id
     * @param name shape name
     * @param geo JGeometry
     * @return ShapeRectangle
     */
    public static ShapeRectangle fromJGeometry(int id, String name, JGeometry geo)
    {
        Shape s = geo.createShape();
        Rectangle r = s.getBounds();
        ShapeRectangle rc = new ShapeRectangle(id, name, 0, 0);
        rc.setGeometry(r, false);
        rc.setState(ShapeState.OK);
        return rc;
    }

    /**
     * Converts rectangle to JGeometry.
     * @return JGeometry
     */
    @Override
    public JGeometry toJGeometry()
    {
        return JGeometry.createLinearPolygon(this.pts(), 2, 0);
    }
}
