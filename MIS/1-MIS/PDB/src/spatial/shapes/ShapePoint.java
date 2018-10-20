// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ShapeState;
import enums.ShapeType;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Point2D;
import oracle.spatial.geometry.JGeometry;
import java.util.UUID;

/**
 * Point.
 */
public class ShapePoint extends ShapeBase
{
    /**
     * Class constructor.
     * @param id shape id
     * @param x x coordinate
     * @param y y coordinate
     */
    public ShapePoint(int id, int x, int y)
    {
        super(id, "Point-" + UUID.randomUUID().toString(), x, y);
        this.type = ShapeType.POINT;
    }
    
    /**
     * Checks if point contains point. Close surrounding is also checked.
     * @param p point
     * @return true if cointains p
     */
    @Override
    public boolean contains(Point p)
    {
        Point pt = this.getPosition();
        if(p.x <= pt.x+PT_OFFSET && p.x >= pt.x-PT_OFFSET &&
        p.y <= pt.y+PT_OFFSET && p.y >= pt.y-PT_OFFSET)
        {
            return true;
        }
        return false;
    }

    /**
     * Move point to new location.
     * @param p new position
     */
    @Override
    public void moveTo(Point p)
    {
        this.setPosition(p);
        this.setUpdated();
    }

    /**
     * Draw point.
     * @param g graphic context
     * @param sketch not used for points 
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        g.setColor(this.getColorForState(true));
        int o = ShapePoint.PT_OFFSET;
        int s = o*2+1;
        g.fillOval(this.getX()-o, this.getY()-o, s, s);
    }

    /**
     * Creates point from JGeometry.
     * @param id shape id
     * @param name shape name
     * @param geo JGeometry
     * @return point
     */
    public static ShapePoint fromJGeometry(int id, String name, JGeometry geo)
    {
        Point2D p = geo.getJavaPoint();
        ShapePoint sp = new ShapePoint(id, (int)p.getX(), (int)p.getY());
        sp.setName(name);
        sp.setState(ShapeState.OK);
        return sp;
    }

    /**
     * Converts point to JGeometry.
     * @return JGeometry
     */
    @Override
    public JGeometry toJGeometry()
    {
        double[] pts = new double[2];
        pts[0] = this.getX();
        pts[1] = this.getY();
        return JGeometry.createPoint(pts, 2, 0);
    }
}
