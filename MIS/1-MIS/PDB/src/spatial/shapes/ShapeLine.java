// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ShapeState;
import enums.ShapeType;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.UUID;
import oracle.spatial.geometry.JGeometry;

/**
 * Line or multiline
 */
public class ShapeLine extends ShapeBase
{
    private ArrayList<Point> points;
    private int checkedPoint;
    
    /**
     * Class constructor.
     * @param id shape id
     * @param x first point's x coordinate
     * @param y first point's y coordinate
     */
    public ShapeLine(int id, int x, int y)
    {
        super(id, "Line-" + UUID.randomUUID().toString(), 0, 0);
        this.points       = new ArrayList<>();
        this.checkedPoint = -1;
        this.type = ShapeType.LINE;
        this.points.add(new Point(x, y));
    }
    
    /**
     * Gets all control points.
     * @return control points
     */
    public ArrayList<Point> getPoints()
    {
        return this.points;
    }
    
    /**
     * Add control point.
     * @param p point
     */
    public void addPoint(Point p)
    {
        this.points.add(p);
        System.out.format("SL: Adding point %d, %d (%d)\n", p.x, p.y, this.points.size());
    }
    
    /**
     * Add control point by x, y coordinates.
     * @param x x coordinate
     * @param y y coordinate
     */
    public void addPoint(int x, int y)
    {
        this.points.add(new Point(x, y));
    }
    
    /**
     * Checks if is drawable. Line is drawble if contains at least 2 control points.
     * @return true if line is drawable
     */
    public boolean isDrawable()
    {
        return this.getPoints().size() >= 2;
    }
    
    /**
     * Gets coordinates of first control point.
     * @return position
     */
    @Override
    public Point getPosition()
    {
        return this.points.get(this.checkedPoint);
    }

    /**
     * Checks if line contains point. Check is only performed on control points
     * and thei close surrounding.
     * @param p point
     * @return true if line contains point
     */
    @Override
    public boolean contains(Point p)
    {
        Point pt;
        for(int i=0; i<this.points.size(); ++i)
        {
            pt = this.points.get(i);
            // Contains only if p is control point or it's surrounding
            if(p.x <= pt.x+PT_OFFSET && p.x >= pt.x-PT_OFFSET &&
            p.y <= pt.y+PT_OFFSET && p.y >= pt.y-PT_OFFSET)
            {
                this.checkedPoint = i;
                return true;
            }
        }
        return false;
    }

    /**
     * Move selected point to new location.
     * @param p point
     */
    @Override
    public void moveTo(Point p)
    {
        this.points.set(this.checkedPoint, p);
        this.setUpdated();
    }

    /**
     * Draw line
     * @param g graphic context
     * @param sketch determines if line is being sketched
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        g.setColor(this.getColorForState(true));
        if(sketch) g.setStroke(ShapeBase.DASH);
        Point p1, p2;
        int max = this.getPoints().size()-1;
        int o = ShapeLine.PT_OFFSET;
        int s = o*2+1;
        
        if(this.getPoints().size() == 1)
        {
            p1 = this.getPoints().get(0);
            g.fillOval(p1.x-o, p1.y-o, s, s);
        }

        for(int pt=0; pt<max; ++pt)
        {
            p1 = this.getPoints().get(pt);
            p2 = this.getPoints().get(pt+1);
            g.drawLine(p1.x, p1.y, p2.x, p2.y);

            // Control points
            g.fillOval(p1.x-o, p1.y-o, s, s);
            if(pt == max-1)
            {
                g.fillOval(p2.x-o, p2.y-o, s, s);
            }
        }
    }

    /**
     * Creates line from JGeometry.
     * @param id shape id
     * @param name shape name
     * @param geo JGeometry
     * @return line
     */
    public static ShapeLine fromJGeometry(int id, String name, JGeometry geo)
    {
        Point2D[] pts = geo.getJavaPoints();
        ShapeLine l = new ShapeLine(id, (int)pts[0].getX(),
            (int)pts[0].getY());
        l.setName(name);
        for(int i=1; i<pts.length; ++i)
            l.addPoint((int)pts[i].getX(), (int)pts[i].getY());
        l.setState(ShapeState.OK);
        return l;
    }

    /**
     * Converts line to JGeometry.
     * @return JGeometry
     */
    @Override
    public JGeometry toJGeometry()
    {
        Object[] o = new Object[this.getPoints().size()];
        for(int i=0; i<this.getPoints().size(); ++i)
        {
            double[] pt = new double[2];
            pt[0] = this.getPoints().get(i).x;
            pt[1] = this.getPoints().get(i).y;
            o[i] = pt;
        }
        return JGeometry.createMultiPoint(o, 2, 0);
    }
}
